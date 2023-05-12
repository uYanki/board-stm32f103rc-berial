#include "board.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

void onchip_flash_read(uint32_t addr, uint16_t* buf, uint16_t len);
void onchip_flash_write_nocheck(uint32_t addr, uint16_t* buf, uint16_t len);

#define ONCHIP_FLASH_BASE 0x08000000
#define ONCHIP_FLASH_SIZE 256  // KB

#if ONCHIP_FLASH_SIZE < 256
#define ONCHIP_PAGE_SIZE 1024
#else
#define ONCHIP_PAGE_SIZE 2048
#endif

#define ONCHIP_FLASH_BUFSIZE (ONCHIP_PAGE_SIZE >> 1)

uint16_t onchip_flash_buf[ONCHIP_FLASH_BUFSIZE];

void flash_write(uint32_t addr, uint16_t* buf, uint16_t len)
{
    uint32_t address;
    uint32_t addr_ofs;  // offset

    // sector
    uint32_t sec_idx;   // index
    uint16_t sec_ofs;   // offset
    uint16_t sec_rmng;  // remain

    uint16_t i;

    if (addr < ONCHIP_FLASH_BASE) return;
    if (addr >= (ONCHIP_FLASH_BASE + (ONCHIP_FLASH_SIZE << 10))) return;  // ×1024

    /* unit: sector: 8bit, buffer: 16bit */

    addr_ofs = addr - ONCHIP_FLASH_BASE;
    sec_idx  = addr_ofs / ONCHIP_PAGE_SIZE;
    sec_ofs  = (addr_ofs % ONCHIP_PAGE_SIZE) >> 1;  // ÷2
    sec_rmng = (ONCHIP_PAGE_SIZE >> 1) - sec_ofs;

    if (sec_rmng >= len) sec_rmng = len;

    address = ONCHIP_FLASH_BASE + sec_idx * ONCHIP_PAGE_SIZE;

    FLASH_Unlock();

    onchip_flash_read(address, onchip_flash_buf, ONCHIP_FLASH_BUFSIZE);

    for (i = 0; i <)

        while (1) {
            // erase before write
        }

    FLASH_Lock();
}

void STMFLASH_Write(uint32_t addr, uint16_t* buf, uint16_t len)
{
    uint32_t sector;
    uint16_t sec_ofs;
    uint16_t sec_rmng;
    uint16_t i;
    uint32_t addr_ofs;
    uint32_t address = STM32_FLASH_BASE + sector * PAGE_SIZE;
    if (addr < STM32_FLASH_BASE || (addr >= (STM32_FLASH_BASE + 1024 * ONCHIP_FLASH_SIZE))) return;
    FLASH_Unlock();
    uint16_t step = PAGE_SIZE >> 1;
    addr_ofs      = addr - STM32_FLASH_BASE;
    sector        = addr_ofs / PAGE_SIZE;
    sec_ofs       = (addr_ofs % PAGE_SIZE) / 2;
    sec_rmng      = step - sec_ofs;
    if (len <= sec_rmng) sec_rmng = len;
    while (1) {
        STMFLASH_Read(address, STMFLASH_BUF, step);
        for (i = 0; i < sec_rmng; i++) {
            if (STMFLASH_BUF[sec_ofs + i] != 0XFFFF) break;
        }
        if (i < sec_rmng) {
            FLASH_ErasePage(address);
            for (i = 0; i < sec_rmng; i++) {
                STMFLASH_BUF[i + sec_ofs] = buf[i];
            }
            STMFLASH_Write_NoCheck(address, STMFLASH_BUF, step);
        } else
            STMFLASH_Write_NoCheck(addr, buf, sec_rmng);
        if (len == sec_rmng)
            break;
        else {
            sector++;
            sec_ofs = 0;
            buf += sec_rmng;
            addr += sec_rmng;
            len -= sec_rmng;
            if (len > (step))
                sec_rmng = step;
            else
                sec_rmng = len;
        }
    };
    FLASH_Lock();
}

void onchip_flash_read(uint32_t addr, uint16_t* buf, uint16_t len)
{
    while (len--) {
        *buf++ = *(volatile uint16_t*)addr;
        addr += 2;
    }
}

void onchip_flash_write_nocheck(uint32_t addr, uint16_t* buf, uint16_t len)
{
    while (len--) {
        FLASH_ProgramHalfWord(addr, *buf++);
        addr += 2;
    }
}

//

int main(void)
{
    board_init();

    println("hello world");

    while (1) {
#if ENABLE_UART_RX

        if (uart1_rxflg) {
            println("recvice: %s", uart1_rxbuf);
            uart1_rxflg = 0;
        }

#endif
    }

    return 0;
}
