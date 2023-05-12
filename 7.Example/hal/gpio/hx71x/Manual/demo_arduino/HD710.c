int Weight = 0; 
void setup() {                                
 Serial.begin(9600);
 Init_Hx710();
 Get_Maopi();   //clear the weight 
 //delay(3000);                
}
void loop() {
Weight = Get_Weight();              
Serial.println(Weight);        
 delay(100);        
}
