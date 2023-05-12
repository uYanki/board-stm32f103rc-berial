#include "mbslave.h"

USHORT usRegInputStart                    = REG_INPUT_START;
USHORT usRegInputBuf[REG_INPUT_NREGS]     = {0};

USHORT usRegHoldingStart                  = REG_HOLDING_START;
USHORT usRegHoldingBuf[REG_HOLDING_NREGS] = {0};

USHORT usDiscInStart                      = DISC_INPUT_START;
UCHAR  ucDiscInBuf[_DISC_INPUT_NDISCS]    = {0};

USHORT usCoilStart                        = COIL_START;
UCHAR  ucCoilBuf[_COIL_NCOILS]            = {0};

/**
 * Modbus slave input register callback function.
 *
 * @param pucRegBuffer input register buffer
 * @param usAddress input register address
 * @param usNRegs input register number
 *
 * @return result
 */

eMBErrorCode
eMBRegInputCB(UCHAR* pucRegBuffer, USHORT usAddress, USHORT usNRegs)
{
    eMBErrorCode eStatus = MB_ENOERR;
    int          iRegIndex;

    /* it already plus one in modbus function method. */
    usAddress--;

    if ((usAddress >= REG_INPUT_START) && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS)) {
        iRegIndex = (int)(usAddress - usRegInputStart);
        while (usNRegs > 0) {
            *pucRegBuffer++ = (uint8_t)(usRegInputBuf[iRegIndex] >> 8);
            *pucRegBuffer++ = (uint8_t)(usRegInputBuf[iRegIndex] & 0xFF);
            iRegIndex++;
            usNRegs--;
        }
    } else {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

/**
 * Modbus slave holding register callback function.
 *
 * @param pucRegBuffer holding register buffer
 * @param usAddress holding register address
 * @param usNRegs holding register number
 * @param eMode read or write
 *
 * @return result
 */

eMBErrorCode
eMBRegHoldingCB(UCHAR* pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode)
{
    eMBErrorCode eStatus = MB_ENOERR;
    int          iRegIndex;

    /* it already plus one in modbus function method. */
    usAddress--;

    if ((usAddress >= REG_HOLDING_START) && ((usAddress + usNRegs) <= (REG_HOLDING_START + REG_HOLDING_NREGS))) {
        iRegIndex = (int)(usAddress - usRegHoldingStart);  // offset
        switch (eMode) {
                /* read current register values from the protocol stack. */
            case MB_REG_READ:
                while (usNRegs > 0) {
                    *pucRegBuffer++ = (uint8_t)(usRegHoldingBuf[iRegIndex] >> 8);
                    *pucRegBuffer++ = (uint8_t)(usRegHoldingBuf[iRegIndex] & 0xFF);
                    iRegIndex++;
                    usNRegs--;
                }
                break;
                /* write current register values with new values from the protocol stack. */
            case MB_REG_WRITE:
                while (usNRegs > 0) {
                    usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                    usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                    iRegIndex++;
                    usNRegs--;
                }
        }
    } else {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

/**
 * Modbus slave coils callback function.
 *
 * @param pucRegBuffer coils buffer
 * @param usAddress coils address
 * @param usNCoils coils number
 * @param eMode read or write
 *
 * @return result
 */

eMBErrorCode
eMBRegCoilsCB(UCHAR* pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode)
{
    eMBErrorCode eStatus = MB_ENOERR;
    USHORT       iRegIndex, iRegBitIndex, iNReg;
    iNReg = (usNCoils >> 3) + 1;

    /* it already plus one in modbus function method. */
    usAddress--;

    if ((usAddress >= COIL_START) &&
        (usAddress + usNCoils <= COIL_START + COIL_NCOILS)) {
        iRegIndex    = (USHORT)(usAddress - usCoilStart) / 8;
        iRegBitIndex = (USHORT)(usAddress - usCoilStart) % 8;
        switch (eMode) {
            /* read current coil values from the protocol stack. */
            case MB_REG_READ:
                while (iNReg > 0) {
                    *pucRegBuffer++ = xMBUtilGetBits(&ucCoilBuf[iRegIndex++], iRegBitIndex, 8);
                    iNReg--;
                }
                pucRegBuffer--;
                /* last coils */
                usNCoils      = usNCoils % 8;
                /* filling zero to high bit */
                *pucRegBuffer = *pucRegBuffer << (8 - usNCoils);
                *pucRegBuffer = *pucRegBuffer >> (8 - usNCoils);
                break;

                /* write current coil values with new values from the protocol stack. */
            case MB_REG_WRITE:
                while (iNReg > 1) {
                    xMBUtilSetBits(&ucCoilBuf[iRegIndex++], iRegBitIndex, 8, *pucRegBuffer++);
                    iNReg--;
                }
                /* last coils */
                usNCoils = usNCoils % 8;
                /* xMBUtilSetBits has bug when ucNBits is zero */
                if (usNCoils != 0) {
                    xMBUtilSetBits(&ucCoilBuf[iRegIndex++], iRegBitIndex, usNCoils, *pucRegBuffer++);
                }
                break;
        }
    } else {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

/**
 * Modbus slave discrete callback function.
 *
 * @param pucRegBuffer discrete buffer
 * @param usAddress discrete address
 * @param usNDiscrete discrete number
 *
 * @return result
 */

eMBErrorCode
eMBRegDiscreteCB(UCHAR* pucRegBuffer, USHORT usAddress, USHORT usNDiscrete)
{
    eMBErrorCode eStatus = MB_ENOERR;
    USHORT       iRegIndex, iRegBitIndex, iNReg;

    iNReg = (usNDiscrete >> 3) + 1;

    /* it already plus one in modbus function method. */
    usAddress--;

    if ((usAddress >= DISC_INPUT_START) && (usAddress + usNDiscrete <= DISC_INPUT_START + DISC_INPUT_NDISCS)) {
        iRegIndex    = (USHORT)(usAddress - usDiscInStart) >> 3;
        iRegBitIndex = (USHORT)(usAddress - usDiscInStart) % 8;

        while (iNReg > 0) {
            *pucRegBuffer++ = xMBUtilGetBits(&ucDiscInBuf[iRegIndex++], iRegBitIndex, 8);
            iNReg--;
        }
        pucRegBuffer--;
        /* last discrete */
        usNDiscrete   = usNDiscrete % 8;
        /* filling zero to high bit */
        *pucRegBuffer = *pucRegBuffer << (8 - usNDiscrete);
        *pucRegBuffer = *pucRegBuffer >> (8 - usNDiscrete);
    } else {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}
