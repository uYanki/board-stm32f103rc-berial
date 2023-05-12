/*******************************************************************************
  Arduino firmware controller for the LIS3DH device over SPI
*******************************************************************************/
#include <SPI.h>
#include <SerialCommand.h>
#include <Time.h>
#include "LIS3DH.h"
#include "CircularBuffer2.h"

#define SERIAL_BAUDRATE 115200
#define INFO_SIZE sizeof(unsigned long)
#define DATA_RECORD_SIZE (INFO_SIZE + LIS3DH_RDATA_NUM_BYTES)
//LIS3DH_RDATA_NUM_BYTES
#define BUFFER_CAPACITY 128*DATA_RECORD_SIZE
//Warning: do not make the capacity too large or the controller may crash
//         and become unresponsive
#define MAX_SERIAL_COMMANDS 20

//Bluetooth modem is connected to UART1 (RX1 = pin 0, TX1 = pin 1) of Teensy 3.0
#define BluetoothModem Serial1
//time to delay while polling Serial1 slave RTS
#define HWFC_DELAY_MILLIS 10

#define SER_INTER_BYTE_DELAY_MICROS 50
#define SER_INTER_PACKET_DELAY_MICROS 100

//FOR DEBUGGING UNCOMMENT NEXT LINE
#define DEBUG

int arduinoLED = 13;
/******************************************************************************/
// Global Objects
SerialCommand sCmd_USB(Serial, MAX_SERIAL_COMMANDS);         // (Stream, int maxCommands)
SerialCommand sCmd_BT(BluetoothModem, MAX_SERIAL_COMMANDS);  // (Stream, int maxCommands)

//configure the ADS chip
LIS3DH_CoreSPIClass acc(6,   //slaveSelectLowPin
                        15     //dataReadyLowPin
                       );
                           
// Circular overwriting buffer for samples
CircularBuffer sampleBuffer(BUFFER_CAPACITY);

byte rawDataRecord[RDATA_NUM_BYTES];

//misc signal pins
int btRTS_pin = 3;       //HIGH should signal the MCU to pause writing to Serial1
int ledPin = 13;
/******************************************************************************/
// Setup

void setup() {
  //----------------------------------------------------------------------------
  //configure signal pins
  pinMode(btRTS_pin, INPUT);
  // Setup callbacks for SerialCommand commands
  // over USB interface
  sCmd_USB.addCommand("LED.ON", LED_ON_command);       //set the system to acquire and push data in default test mode at 250 SPS
  sCmd_USB.addCommand("LED.OFF", LED_OFF_command);     //set the system to acquire and push data in default test mode at 250 SPS
  sCmd_USB.addCommand("CLK.SET",     CLK_SET_command); //set the realtime clock
  sCmd_USB.addCommand("CLK.READ",    CLK_READ_command); //read the realtime clock
  sCmd_USB.addCommand("PRESET.DAQ250", PRESET_DAQ250_command); //set the system to acquire and push data in default test mode at 250 SPS
  sCmd_USB.addCommand("SER.DMODE", SER_DMODE_command); //set the data mode of the serial link
  sCmd_USB.addCommand("SER.BUFSZ", SER_BUFSZ_command); //get the number of records in the buffer
  sCmd_USB.addCommand("SER.RDBUF", SER_RDBUF_command); //dump the data buffer over the serial link
  sCmd_USB.addCommand("SER.FLBUF", SER_FLBUF_command); //flush contents of circular buffer
  //sCmd_USB.addCommand("ACC.START", ACC_START_command); //start conversions
  //sCmd_USB.addCommand("ACC.STOP",  ACC_STOP_command);   //stop conversions
  //sCmd_USB.addCommand("ACC.RDATA_RAW", ACC_RDATA_RAW_command); //reads sample data as sends as binary
  sCmd_USB.addCommand("ACC.RREG" , ACC_RREG_command);  //writes hardware register on the ADS129x
  sCmd_USB.addCommand("ACC.WREG" , ACC_WREG_command);  //reads a hardware register on the ADS129x
  sCmd_USB.setDefaultHandler(unrecognizedCommand);
  // over BluetoothModem interface
  //----------------------------------------------------------------------------
  // set the Time library to use Teensy 3.0's RTC to keep time
  setSyncProvider(getTeensy3Time);
  delay(100);
  //----------------------------------------------------------------------------
  // intialize the serial devices
  Serial.begin(SERIAL_BAUDRATE);                  //USB serial on the Teensy 3.0
  //BluetoothModem.begin(SERIAL_BAUDRATE);

  //----------------------------------------------------------------------------
  // initialize the LIS3DH chip
  // first enable the analog power supply (5.0V) on controller board
  // DO NOT toggle ledPin after this point since it will interfere with SPI initialization
   
  
  //wait for ADS system to come on line for 1 sec 
  delay(1000);
 
  
  //----------------------------------------------------------------------------
  //start up the SPI bus, note CANNOT ledPin for blinking beyond here
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE3);
  SPI.setClockDivider(24); //96MHz clock /24 = 4 MHz
  
  digitalWrite(SCK, HIGH);                    //assure that clock starts
  
  //lastly configure ADS system
  acc.begin();
  //----------------------------------------------------------------------------
  // FOR DEBUGGING OUTPUT ONLY
  #ifdef DEBUG
  if (timeStatus()!= timeSet) {
    Serial.print("# Time: Unable to sync with the RTC\n");
  } else {
    Serial.print("# Time: RTC has set the system time\n");
  }
  Serial.print(F("# memory_free = "));
  Serial.print(memoryFree());
  Serial.print(F("\n"));
  Serial.print(F("# Initialized.\n"));
  #endif
}
/******************************************************************************/
// Loop

enum datamode_type {
  PULL,
  PUSH_USB,
  PUSH_BT,
  PUSH_BT_HWFC
};

enum datamode_type datamode = PULL;
Print &data_output = Serial;
volatile bool dataReady_flag = false;

void loop() {
  unsigned long t1, t2, acq_t;
  //process serial commands on both interfaces
  //noInterrupts();  //prevent possible race condition
  sCmd_USB.readSerial();
  sCmd_BT.readSerial();
  //interrupts();
  // handle signals
  if (dataReady_flag){
    //Serial.print(F("# data ready\n"));
    switch(datamode){
      case PULL:
        //do nothing
        break;
      case PUSH_USB:
        push_out_buffer(Serial);
        break;
      case PUSH_BT:
        push_out_buffer(BluetoothModem);
        break;
      case PUSH_BT_HWFC:
        push_out_buffer_hwfc(BluetoothModem);
        break;
    }
    dataReady_flag = false;
    delay(1);
  }
}

/******************************************************************************/
// Functions

// variables created by the build process when compiling the sketch
extern int __bss_end;
extern void *__brkval;

// function to return the amount of free RAM
int memoryFree()
{
  int freeValue;

  if ((int)__brkval == 0)
     freeValue = ((int)&freeValue) - ((int)&__bss_end);
  else
    freeValue = ((int)&freeValue) - ((int)__brkval);

  return freeValue;
}

time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}

/*void softwareReset() // Restarts program from beginning but does not reset the peripherals and registers*/
/*{*/
/*  asm volatile ("  jmp 0");*/
/*} */

void buffer_raw_data(byte *record, unsigned long *acq_time_micros){
  //store data in the buffer
  sampleBuffer.write((byte *) acq_time_micros, sizeof(unsigned long));
  sampleBuffer.write(record, RDATA_NUM_BYTES);
}

// Serial output single data packet
void send_raw_data(byte *record, unsigned long acq_time_micros, Print &output){
  output.write((byte *) &acq_time_micros, sizeof(unsigned long));
  for(int i = 0; i < RDATA_NUM_BYTES; i++){
    output.write(record[i]);  //output the data as binary
    delayMicroseconds(SER_INTER_BYTE_DELAY_MICROS);
  }
}

// Serial output single data packet with hardware flow control (polling RTS of slave)
void send_raw_data_hwfc(byte *record, unsigned long acq_time_micros, Print &output){
  output.write((byte *) &acq_time_micros, sizeof(unsigned long));
  for(int i = 0; i < RDATA_NUM_BYTES; i++){
    //stall sending next byte while slave is signalling to halt transmission
    while(digitalRead(btRTS_pin)){
      Serial.print("RTS wait\n");
      delay(HWFC_DELAY_MILLIS);
    }
    //now slave is ready for transmission
    output.write(record[i]);  //output the data as binary
  }
}

// Serial output contents of sample buffer
void push_out_buffer(Print &output){
  size_t num;
  unsigned long acq_t;
  num = sampleBuffer.size()/DATA_RECORD_SIZE;
  for (int i=0; i < num; i++){
    //grab record from buffer
    sampleBuffer.read((byte *) &acq_t, sizeof(unsigned long));
    sampleBuffer.read(rawDataRecord, RDATA_NUM_BYTES);
    //send it out to the interface that requested it
    send_raw_data(rawDataRecord, acq_t, output);
    delayMicroseconds(SER_INTER_PACKET_DELAY_MICROS);
  }
}

// Serial output contents of sample buffer with hardware flow control
void push_out_buffer_hwfc(Print &output){
  size_t num;
  unsigned long acq_t;
  num = sampleBuffer.size()/DATA_RECORD_SIZE;
  for (int i=0; i < num; i++){
    //grab record from buffer
    sampleBuffer.read((byte *) &acq_t, sizeof(unsigned long));
    sampleBuffer.read(rawDataRecord, RDATA_NUM_BYTES);
    //send it out to the interface that requested it
    send_raw_data_hwfc(rawDataRecord, acq_t, output);
  }
}

/******************************************************************************/
// Interrupt Service Routines

void handle_dataReadyLow_interrupt(){
  unsigned long t1, t2, acq_t;
  dataReady_flag = true;
  //Serial.print(F("# dataReadyLow ISR\n"));
  //read raw data into record
  acq_t = micros();
  //acc.readDataRaw(rawDataRecord);
  //buffer_raw_data(rawDataRecord, &acq_t);
}

/******************************************************************************/

void LED_ON_command(SerialCommand this_scmd) {
  char *arg = this_scmd.next();
  if (arg == NULL){
    this_scmd.print(F("LED ON\n"));
    digitalWrite(ledPin, HIGH);
  }
}

void LED_OFF_command(SerialCommand this_scmd) {
  char *arg = this_scmd.next();
  if (arg == NULL){
    this_scmd.print(F("LED OFF\n"));
    digitalWrite(ledPin, LOW);
  }
}

// Preset Configurations
void PRESET_DAQ250_command(SerialCommand this_scmd) {
  char *arg = this_scmd.next();
  if (arg == NULL){
    this_scmd.print(F("### Error: PRESET.DAQ250 requires 1 argument, (str datamode): PULL, PUSH_USB, PUSH_BT\n"));
    return;
  }
  else{
    int n, addr;
    byte reg;
    //set the global serial datamode from the argument
    if (strcmp(arg,"PULL") == 0){
      datamode = PULL;
    }
    else if (strcmp(arg,"PUSH_USB") == 0){
      datamode = PUSH_USB;
    }
    else if (strcmp(arg,"PUSH_BT") == 0){
      datamode = PUSH_BT;
    }
    else if (strcmp(arg,"PUSH_BT_HWFC") == 0){
      datamode = PUSH_BT_HWFC;
    }
    else{
      this_scmd.print(F("### Error: PRESET.DAQ250 datamode not recognized\n"));
      return;
    }
    //ensure that ADS is in a mode to read and write registers
/*    acc.stopConversions();*/
    //set the data rate DR = 250 => self.set_reg_bits("CONFIG1",['DR2','DR1','DR0'], [1,1,0])
/*    addr = 0x01; //CONFIG1*/
/*    reg = acc._readRegister(addr);*/
/*    reg &= ~0x01; //clear bits 0*/
/*    reg |=  0x06; //set   bits 2,1*/
/*    acc._writeRegister(addr, reg);*/
    
    //flush serial buffer
    sampleBuffer.flush();
    
    ////startup the ADC
/*    acc.attach_dataReadyLow_interrupt(handle_dataReadyLow_interrupt);*/
/*    acc.startConversions();*/
  }
}

// Clock commands
void CLK_SET_command(SerialCommand this_scmd) {
  char *arg = this_scmd.next();
  if (arg == NULL){
    this_scmd.print(F("### Error: CLK.SET requires 1 argument (int addr)\n"));
  }
  else{
    this_scmd.print(F("### Error: CLK.SET not implemented\n"));
  }
}

void CLK_READ_command(SerialCommand this_scmd) {
  time_t now;
  char *arg = this_scmd.next();
  if (arg != NULL){
    this_scmd.print(F("### Error: CLK.READ requires no arguments\n"));
  }
  else{
    now = getTeensy3Time();
    this_scmd.print(now);
    this_scmd.print("\n");
  }
}

// Serial Commands

void SER_DMODE_command(SerialCommand this_scmd) {
  char *arg = this_scmd.next();
  if (arg == NULL){
    this_scmd.print(F("### Error: SER.DMODE requires 1 argument (str mode)\n"));
  }
  else if (strcmp(arg,"PULL") == 0){
    datamode = PULL;
  }
  else if (strcmp(arg,"PUSH_USB") == 0){
    datamode = PUSH_USB;
  }
  else if (strcmp(arg,"PUSH_BT") == 0){
    datamode = PUSH_BT;
  }
  else if (strcmp(arg,"PUSH_BT_HWFC") == 0){
    datamode = PUSH_BT_HWFC;
  }
  else{
    this_scmd.print(F("### Error: SER.DMODE mode not recognized\n"));
  }
}

void SER_BUFSZ_command(SerialCommand this_scmd) {
  char *arg = this_scmd.next();
  size_t num;
  if (arg != NULL){
    this_scmd.print(F("### Error: SER.BUFSZ requires no arguments\n"));
  }
  else{
    num = sampleBuffer.size()/DATA_RECORD_SIZE;
    this_scmd.print(num);
    this_scmd.print("\n");
  }
}

void SER_RDBUF_command(SerialCommand this_scmd) {
  char *arg = this_scmd.next();
  size_t num, max_num; //number of data records to read
  max_num = sampleBuffer.size()/DATA_RECORD_SIZE;
  if (arg != NULL){
    num = min(atoi(arg), max_num);
  }
  else{
    num = max_num;
  }
  unsigned long acq_t;
  for (int i=0; i < num; i++){
    //grab record from buffer
    sampleBuffer.read((byte *) &acq_t, sizeof(unsigned long));
    sampleBuffer.read(rawDataRecord, RDATA_NUM_BYTES);
    //send it out to the interface that requested it
    send_raw_data(rawDataRecord, acq_t, this_scmd);
  }
}

void SER_FLBUF_command(SerialCommand this_scmd) {
  char *arg = this_scmd.next();
  size_t records_flushed;
  if (arg != NULL){
    this_scmd.print(F("### Error: SER.FLBUF requires no arguments\n"));
  }
  else{
    records_flushed = sampleBuffer.flush()/DATA_RECORD_SIZE;
    this_scmd.print(records_flushed);
    this_scmd.print("\n");
  }
}


void ACC_START_command(SerialCommand this_scmd) {
  char *arg = this_scmd.next();
  if (arg != NULL){
    this_scmd.print(F("### Error: ACC.START requires no arguments\n"));
  }
  else{
    //acc.attach_dataReadyLow_interrupt(handle_dataReadyLow_interrupt);
    //acc.startConversions();
  }
}

void ACC_STOP_command(SerialCommand this_scmd) {
  char *arg = this_scmd.next();
  if (arg != NULL){
    this_scmd.print(F("### Error: ACC.STOP requires no arguments\n"));
  }
  else{
    noInterrupts();  //prevent possible race condition
    //acc.stopConversions();
    //acc.detach_dataReadyLow_interrupt();
    interrupts();
  }
}

void ACC_RDATA_RAW_command(SerialCommand this_scmd) {
  char *arg = this_scmd.next();
  if (arg != NULL){
    this_scmd.print(F("### Error: ACC.RDATA_RAW requires no arguments\n"));
  }
  else{
    unsigned long t1, t2, acq_t;
/*    t1 = micros();*/
/*    acc.readDataRaw(rawDataRecord);*/
/*    t2 = micros();*/
/*    acq_t = (t1 + t2)/2;*/
/*    send_raw_data(rawDataRecord, acq_t, this_scmd);*/
  }
}

void ACC_RREG_command(SerialCommand this_scmd) {
  int addr;
  byte data;
  char *arg = this_scmd.next();
  if (arg == NULL){
    this_scmd.print(F("### Error: ACC.RREG requires 1 argument (int addr)\n"));
  }
  else{
    addr = atoi(arg);
    data = acc._readRegister(addr);
    this_scmd.print(data);
    this_scmd.print('\n');
  }
}

void ACC_WREG_command(SerialCommand this_scmd) {
  int addr;
  byte value;
  char *arg = this_scmd.next();
  if (arg == NULL){
    this_scmd.print(F("### Error: ACC.WREG requires 2 arguments (int addr, byte value), none given\n"));
  }
  else{
    addr = atoi(arg);
    arg = this_scmd.next();
    if (arg == NULL){
        this_scmd.print(F("### Error: ACC.WREG requires 2 arguments (int addr, byte value), 1 given\n"));
    }
    else{
        value = atoi(arg);
        //acc._writeRegister(addr, value);
    }
  }
}

// Unrecognized command
void unrecognizedCommand(const char* command, SerialCommand this_scmd)
{
  this_scmd.print(F("### Error: command '"));
  this_scmd.print(command);
  this_scmd.print(F("' not recognized ###\n"));
}


/******************************************************************************/
