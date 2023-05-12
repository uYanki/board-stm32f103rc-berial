
#include "driver_fm24clxx.h"

#define MANUFACTURER_NAME  "Cypress"  // manufacturer name
#define SUPPLY_VOLTAGE_MIN 2.7f       // chip min supply voltage
#define SUPPLY_VOLTAGE_MAX 3.65f      // chip max supply voltage

uint8_t fm24clxx_set_addr_pin(fm24clxx_address_t addr_pin)
{
    iic_addr = 0xA0;           /* set iic addr */
    iic_addr |= addr_pin << 1; /* set iic address */
    return 0;                  /* success return 0 */
}
uint8_t fm24clxx_get_addr_pin(fm24clxx_address_t* addr_pin)
{
    *addr_pin = (fm24clxx_address_t)((iic_addr & (~0xA0)) >> 1); /* get iic address */
    return 0;                                                    /* success return 0 */
}

uint8_t fm24clxx_read(uint16_t address, uint8_t* buf, uint16_t len)
{
    uint8_t page_remain;

    if ((uint16_t)(address + len) > id) /* check length */
    {
    }
    page_remain = (uint8_t)(8 - address % 8); /* get page remain */
    if (len <= page_remain)                   /* page remain */
    {
        page_remain = (uint8_t)len; /* set page remain */
    }
    if (id > (uint16_t)FM24CL16B) /* choose id to set different address */
    {
        while (1) {
            if (iic_read_address16(iic_addr, address, buf, page_remain) != 0) /* read data */
            {
            }
            if (page_remain == len) /* check break */
            {
                break; /* break loop */
            } else {
                address += page_remain; /* address increase */
                buf += page_remain;     /* buffer point increase */
                len -= page_remain;     /* length decrease */
                if (len < 8)            /* check length */
                {
                    page_remain = (uint8_t)len; /* set the reset length */
                } else {
                    page_remain = 8; /* set page */
                }
            }
        }
    } else {
        while (1) {
            if (iic_read((uint8_t)(iic_addr + ((address / 256) << 1)), address % 256, buf, page_remain) != 0) {
            }
            if (page_remain == len) {
                break;
            } else {
                address += page_remain;
                buf += page_remain;
                len -= page_remain;
                if (len < 8) {
                    page_remain = (uint8_t)len;
                } else {
                    page_remain = 8;
                }
            }
        }
    }
    return 0; /* success return 0 */
}
uint8_t fm24clxx_write(uint16_t address, uint8_t* buf, uint16_t len)
{
    uint8_t page_remain;

    if ((uint16_t)(address + len) > id) {
    }
    page_remain = (uint8_t)(8 - address % 8);
    if (len <= page_remain) {
        page_remain = (uint8_t)len;
    }
    if (id > (uint16_t)FM24CL16B) {
        while (1) {
            if (iic_write_address16(iic_addr, address, buf, page_remain) != 0) {
            }
            if (page_remain == len) {
                break;
            } else {
                address += page_remain;
                buf += page_remain;
                len -= page_remain;
                if (len < 8) {
                    page_remain = (uint8_t)len;
                } else {
                    page_remain = 8;
                }
            }
        }
    } else {
        while (1) {
            if (iic_write((uint8_t)(iic_addr + ((address / 256) << 1)), address % 256, buf, page_remain) != 0) {
            }
            if (page_remain == len) {
                break;
            } else {
                address += page_remain;
                buf += page_remain;
                len -= page_remain;
                if (len < 8) {
                    page_remain = (uint8_t)len;
                } else {
                    page_remain = 8;
                }
            }
        }
    }
    return 0; /* success return 0 */
}
