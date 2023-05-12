/****************************************************************************
* Title                 :   I2C Hardware Access Layer
* Filename              :   heartrate1_hal.c
* Author                :   Nikola Golubovic
* Origin Date           :   23/10/2015
* Notes                 :   None
*****************************************************************************/
/*************** MODULE REVISION LOG ******************************************
*
*    Date    Software Version    Initials    Description
*  10/26/15         1.0            NG
*
*****************************************************************************/
/** @file heartrate1_hal.c
 *  @brief Heartrate 1 [MAX30100]  Hardware Access Layer
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "heartrate1_hal.h"

#if defined( __GNUC__ )
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
/**
 * Doxygen tag for documenting variables and constants
 */

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define WRITE 0
#define READ  1

/******************************************************************************
* Module Typedefs
*******************************************************************************/
#if defined(__MIKROC_PRO_FOR_ARM__)
typedef unsigned int ( *i2c_start_t )( void );
typedef void ( *i2c_read_t )( unsigned char slave_address,
                              unsigned char *ptrdata,
                              unsigned long count, 
                              unsigned long END_mode );
typedef unsigned int ( *i2c_write_t )( unsigned char slave_address,
                                       unsigned char *buf,
                                       unsigned long count,
                                       unsigned long END_mode );
#elif defined(__MIKROC_PRO_FOR_AVR__)
typedef unsigned char ( *i2c_start_t )( void );
typedef unsigned char ( *i2c_read_t )( unsigned char ack );
typedef void ( *i2c_write_t )( unsigned char data_ );
typedef void ( *i2c_stop_t )( void );

#elif defined(__MIKROC_PRO_FOR_8051__)

#elif defined(__MIKROC_PRO_FOR_FT90x__)
typedef char ( *i2c_read_t )( char *bufferIn, unsigned int numBytesToReceive );
typedef char ( *i2c_write_t )( char *bufferOut, unsigned int numBytesToWrite);

#elif defined(__MIKROC_PRO_FOR_PIC__)   || \
      defined(__MIKROC_PRO_FOR_DSPIC__) || \
      defined(__MIKROC_PRO_FOR_PIC32__)
typedef char ( *i2c_start_t )( void );
typedef char ( *i2c_read_t )( char ack );
typedef void ( *i2c_write_t )( char data_ );
typedef void ( *i2c_stop_t )( void );
#endif

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static uint8_t i2c_address;
#if defined(__MIKROC_PRO_FOR_AVR__)   || \
    defined(__MIKROC_PRO_FOR_8051__)  || \
    defined(__MIKROC_PRO_FOR_DSPIC__) || \
    defined(__MIKROC_PRO_FOR_PIC32__)
static i2c_start_t _hal_i2c_start;
static i2c_read_t _hal_i2c_read;
static i2c_write_t _hal_i2c_write;
static i2c_stop_t _hal_i2c_stop;

#elif defined(__MIKROC_PRO_FOR_FT90x__)
static i2c_read_t _hal_i2c_read;
static i2c_write_t _hal_i2c_write;

#elif defined(__MIKROC_PRO_FOR_ARM__)
static i2c_start_t _hal_i2c_start;
static i2c_read_t _hal_i2c_read;
static i2c_write_t _hal_i2c_write;

#elif defined(__MIKROC_PRO_FOR_PIC__)
extern uint8_t I2C_port_num;
static i2c_start_t _hal_i2c_start;
static i2c_read_t _hal_i2c_read;
static i2c_write_t _hal_i2c_write;
static i2c_stop_t _hal_i2c_stop;

#elif defined(__GNUC__)
extern uint8_t bits;
extern uint32_t speed;
extern uint16_t delay;
#define heartrate1_hal_i2c_start
#define heartrate1_hal_i2c_read
#define heartrate1_hal_i2c_write
#endif

/******************************************************************************
* Function Prototypes
*******************************************************************************/
#if defined( __GNUC__ )
static void transfer( int fd, uint8_t const *tx,
                      uint8_t const *rx, size_t len );
#endif

/******************************************************************************
* Function Definitions
*******************************************************************************/

int heartrate1_i2c_init( uint8_t address_id )
{
#if defined( __MIKROC_PRO_FOR_ARM__ ) || \
    defined(__MIKROC_PRO_FOR_FT90x__)
    i2c_address = address_id;
#else
    i2c_address = ( address_id << 1 );
#endif

#if defined(__MIKROC_PRO_FOR_AVR__)
    _hal_i2c_start = TWI_Start;
    _hal_i2c_read  = TWI_Read;
    _hal_i2c_write = TWI_Write;
    _hal_i2c_stop  = TWI_Stop;

#elif defined(__MIKROC_PRO_FOR_ARM__ )
    _hal_i2c_start =  I2C_Start_Ptr;
    _hal_i2c_read  =  I2C_Read_Ptr;
    _hal_i2c_write =  I2C_Write_Ptr;

#elif defined(__MIKROC_PRO_FOR_8051__)

#elif defined(__MIKROC_PRO_FOR_FT90x__)
    _hal_i2c_read = I2CM_Read_Bytes_Ptr;
    _hal_i2c_write = I2CM_Write_Bytes_Ptr;
    #define _hal_i2c_start 1
#elif defined(__MIKROC_PRO_FOR_DSPIC__) || \
      defined(__MIKROC_PRO_FOR_PIC32__) || \
      defined(__MIKROC_PRO_FOR_PIC__)
    _hal_i2c_start = I2C_Start;
    _hal_i2c_read  = I2C_Read;
    _hal_i2c_write = I2C_Write;
    _hal_i2c_stop  = I2C_Stop;
#endif

    if( _hal_i2c_start == 0 || _hal_i2c_read == 0 || _hal_i2c_write == 0 )
        return -1;

    return 0;
}

void heartrate1_i2c_hal_write( uint8_t address, uint16_t num, uint8_t *buff )
{
    uint8_t buffer[MAX_READ_SIZE];

    buffer[0] = address;
    memcpy( &buffer[1], buff, num );
#if defined(__MIKROC_PRO_FOR_ARM__)
    _hal_i2c_start();
    _hal_i2c_write( i2c_address, buffer, num + 1, END_MODE_STOP );

#elif defined(__MIKROC_PRO_FOR_FT90x__)
    I2CM1_Set_Slave_Address( i2c_address );
    _hal_i2c_write( buffer, num + 1 );

#elif       defined(__MIKROC_PRO_FOR_AVR__)   || \
            defined(__MIKROC_PRO_FOR_8051__)  || \
            defined(__MIKROC_PRO_FOR_DSPIC__) || \
            defined(__MIKROC_PRO_FOR_PIC32__) || \
            defined(__MIKROC_PRO_FOR_PIC__)
    uint8_t *ptr = buff;
    _hal_i2c_start();
    _hal_i2c_write( i2c_address | WRITE );
    _hal_i2c_write( address );

    while( num-- )
        _hal_i2c_write( *( ptr++ ) );

    _hal_i2c_stop();
#endif
}

void heartrate1_i2c_hal_read(  uint8_t address, uint16_t num, uint8_t *buff )
{
    uint8_t buffer[1];
    buffer[0] = address;
    
#if defined( __MIKROC_PRO_FOR_ARM__ )
    _hal_i2c_start();              // issue I2C start signal
    _hal_i2c_write( i2c_address, buffer, 1, END_MODE_RESTART );
    _hal_i2c_read( i2c_address, buff, num, END_MODE_STOP );

#elif defined( __GNUC__ )
    char input;

    while( num-- )
    {
        getc( &input );
        *( buff++ ) = input;
    }

#elif defined(__MIKROC_PRO_FOR_FT90x__)
    I2CM1_Set_Slave_Address( i2c_address );

    _hal_i2c_write( buffer, 1 );
    _hal_i2c_read( buff, num );

#elif defined(__MIKROC_PRO_FOR_AVR__)
    _hal_i2c_write( i2c_address | WRITE );
    _hal_i2c_write( buffer );
    _hal_i2c_start();

    _hal_i2c_write( i2c_address | READ );
    while( num >= 2 )
    {    
         *( buff++ ) = _hal_i2c_read( 1 );
         num--;
    }

    *( buff ) = _hal_i2c_read( 0 );
    _hal_i2c_stop();
#endif
}

/*************** END OF FUNCTIONS ***************************************************************************/