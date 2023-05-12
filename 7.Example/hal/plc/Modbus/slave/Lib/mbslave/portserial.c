
/* ----------------------- System includes --------------------------------- */
#include "port.h"
#include "gpio.h"
#include "usart.h"

/* ----------------------- Modbus includes --------------------------------- */
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ----------------------------------------- */

/* ----------------------- Static variables -------------------------------- */

/* ----------------------- Start implementation ---------------------------- */

// rs485

typedef enum {
    RS485_MODE_RX,
    RS485_MODE_TX,
} rs485_mode_t;

static void rs485_set_mode(rs485_mode_t mode)
{
    if (mode == RS485_MODE_TX) {
        HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_SET);
        HAL_Delay(5);
    } else {
        HAL_Delay(10);
        HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET);
    }
}

//

uint8_t rxbuf;

void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable transmitter empty interrupts. */

    // 中断使能/失能

    // rs485_set_mode()

    if (xRxEnable) {
        __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
        HAL_UART_Receive_IT(&huart2, &rxbuf, 1);
    } else {
        __HAL_UART_DISABLE_IT(&huart2, UART_IT_RXNE);
    }

    if (xTxEnable) {
        __HAL_UART_ENABLE_IT(&huart2, UART_IT_TC);
    } else {
        __HAL_UART_DISABLE_IT(&huart2, UART_IT_TC);
    }
}

BOOL xMBPortSerialInit(UCHAR ucPort, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity)
{
    UART_HandleTypeDef* huart;

    // ucPort is set by eMBInit
    if (ucPort == 2) {
        huart = &huart2;
    } else {
        return FALSE;
    }

    huart->Init.BaudRate = ulBaudRate;

    switch (eParity) {
        case MB_PAR_NONE: huart->Init.Parity = UART_PARITY_NONE; break;
        case MB_PAR_EVEN: huart->Init.Parity = UART_PARITY_EVEN; break;
        case MB_PAR_ODD: huart->Init.Parity = UART_PARITY_ODD; break;
        default: return FALSE;
    }
		
		huart->Init.WordLength = UART_WORDLENGTH_8B;

//    switch (ucDataBits) {
//        case 8:
//            huart->Init.WordLength = (eParity == UART_PARITY_NONE) ? UART_WORDLENGTH_8B : UART_WORDLENGTH_9B;
//            break;
//        case 7:
//            if (eParity == UART_PARITY_NONE) return FALSE;
//            huart->Init.WordLength = UART_WORDLENGTH_8B;
//            break;
//        default: return FALSE;
//    }

    return HAL_UART_Init(huart) == HAL_OK ? TRUE : FALSE;
}

BOOL xMBPortSerialPutByte(CHAR ucByte)
{
    /* Put a byte in the UARTs transmit buffer. This function is called by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been called. */

    return HAL_UART_Transmit_IT(&huart2, (uint8_t*)&ucByte, 1) == HAL_OK ? TRUE : FALSE;
}

BOOL xMBPortSerialGetByte(CHAR* pucByte)
{
    /* Return the byte in the UARTs receive buffer. This function is called by the protocol stack after pxMBFrameCBByteReceived( ) has been called. */

    *pucByte = rxbuf;

    return HAL_UART_Receive_IT(&huart2, (uint8_t*)&rxbuf, 1) == HAL_OK ? TRUE : FALSE;
}
