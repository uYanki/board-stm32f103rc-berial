
#include "abs_addr.h"

#define flash_start_address 0x8006000

const signed short int x[0x770C] __at(flash_start_address + 2) = {
    0XBAD8, 0X0000, 0X0000, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020,
    0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020, 0X2020,
    0X2020, 0X2020, 0X2020, 0X09F4, 0X0BFF, 0X01F4, 0X03E7, 0X0E64, 0X0EC7, 0X0EDC,
    0X0EFF, 0X0190, 0X03FE, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
    0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
    0x2400, 0xC500, 0X2401, 0XC501, 0X2402, 0XC502, 0X2403, 0XC503, 0X000F,
    0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF,
    0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF,
    0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF,
    0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF,
    0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF,
    0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF,
    0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF};

const u8 p_x[2] __at(flash_start_address) = {0x08, 0x00};

const u16 special_d[256] = {
    0X00C8, 0X5EF6, 0X0002, 0X0010, 0X0000, 0X0025, 0X001E, 0X0000, 0XFFFF, 0X0000,
    0X0000, 0X0000, 0X0000, 0X00A5, 0X00A5, 0X00A5, 0X00A5, 0X00A5, 0X0811, 0X000F,
    0X000A, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
    0X003D, 0X001C, 0X0000, 0X0000, 0X0014, 0X00FF, 0X03D7, 0X0000, 0X0000, 0X0000,
    0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0XFFFF, 0X0000, 0XFFFF,
    0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
    0X0000, 0X0000, 0X183B, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
    0X01F4, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
    0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
    0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
    0X0064, 0X5EF6, 0X0008, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000,
    0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0000, 0X0DDC, 0X3DB6, 0X0000, 0X0000,
    0X0000, 0X0000, 0X0000, 0X0000, 0X0002, 0X0003, 0X0000, 0X0000};

static const char bcd_ascll[20] = {
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
    0x37, 0x38, 0x39, 0X41, 0X42, 0X43, 0X44, 0X45, 0X46};

const char ascll_bcd[25] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0X0B, 0X0C, 0X0D, 0X0E, 0X0F};

const char plc_type[2] = {0XF6, 0X5E};  // PLC型号  FX2N

u16 prog_write_buffer[2050];

u16       prog_count, rx_temp, tx_temp, rx_count, tx_count;
u16       all_data[16600] __at(0x20005004);
u8        p_all_data[4] __at(0x20005000);
u8        step_status[1000] __at(0x2000D200);
char*     str;
u16*      px;
char      tx_data[610], rx_data[610];
u16       prog_address, data_address;
u8        data_size, block_contol[2], rx_end;
extern u8 edit_prog;
extern u8 Run_Flag;
void      recover_data(void);

void data_init(void)
{
    u16 temp;
    u16 temp_address;
    for (temp = 0; temp < 126; temp++) {
        temp_address                  = 0x0700;
        all_data[temp_address + temp] = special_d[temp];
    }
    temp_address             = 0x01C0 + 4;
    p_all_data[temp_address] = 0x09;
    block_contol[0]          = 200;
    block_contol[1]          = 200;
    recover_data();
}

void usart_init(void)
{
    {
        GPIO_InitTypeDef GPIO_InitStructure;

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    {
        USART_InitTypeDef USART_InitStructure;

        USART_DeInit(USART1);

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

        USART_InitStructure.USART_BaudRate            = 19200;
        USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits            = USART_StopBits_1;
        USART_InitStructure.USART_Parity              = USART_Parity_Even;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
        USART_Init(USART1, &USART_InitStructure);

        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
        USART_Cmd(USART1, ENABLE);
    }

    data_init();
}

void write_block(u16 number)
{
    u16 temp, wait_write, appoint_address;
    if (number < 12) {
        FLASH_Unlock();
        FLASH_ErasePage(flash_start_address + number * 0x800);
        for (temp = 0; temp < 1024; temp++) {
            appoint_address = flash_start_address + number * 0x800 + temp * 2;
            wait_write      = prog_write_buffer[temp * 2] + prog_write_buffer[temp * 2 + 1] * 0X100;
            FLASH_ProgramHalfWord(appoint_address, wait_write);
        }
        FLASH_Lock();
    }
}

void backup_block(u16 number)
{
    u16 temp, appoint_address;
    if (number < 12) {
        for (temp = 0; temp < 2048; temp++) {
            appoint_address         = number * 0x800 + temp;
            prog_write_buffer[temp] = p_x[appoint_address];
        }
    }
}

void rx_data_sum(void)
{
    u16 temp;
    u8  all_sum;
    rx_data[599] = 0;
    all_sum      = 0;
    str          = rx_data;
    str += 3;
    for (temp = 3; temp < (rx_count - 1); temp++) {
        all_sum += *str;
        str++;
    }
    if ((rx_data[rx_count - 1] == bcd_ascll[all_sum / 0x10]) && (rx_data[rx_count] == bcd_ascll[all_sum % 0x10]))
        rx_data[599] = 1;
    else
        rx_data[599] = 5;
}

void switch_read_data(void)
{
    u16 temp;
    for (temp = 4; temp < (rx_count - 2); temp++) {
        tx_data[temp / 2] = ascll_bcd[rx_data[temp] - 0x30] * 0x10;
        tx_data[temp / 2] += ascll_bcd[rx_data[temp + 1] - 0x30];
        temp++;
    }
}

void setup_HL(void) { prog_address = (tx_data[3] << 8) + tx_data[2]; }

void setup_LH(void) { data_address = (tx_data[3] << 8) + tx_data[4]; }

void read_plc_tyte(void)
{
    u16 temp;
    u8  temp_sum;
    tx_data[1] = 0x02;
    temp_sum   = 0;
    for (temp = 0; temp < data_size; temp++) {
        tx_data[temp * 2 + 2] = bcd_ascll[plc_type[temp] / 0x10];
        tx_data[temp * 2 + 3] = bcd_ascll[plc_type[temp] % 0x10];
        temp_sum += tx_data[temp * 2 + 2] + tx_data[temp * 2 + 3];
    }
    tx_data[temp * 2 + 2] = 0x03;
    temp_sum += 0x03;
    tx_data[temp * 2 + 3] = bcd_ascll[temp_sum / 0x10];
    tx_data[temp * 2 + 4] = bcd_ascll[temp_sum % 0x10];
    tx_count              = temp * 2 + 4;
}

void read_other_data(void)
{
    u16 temp;
    u8  temp_sum;
    tx_data[1] = 0x02;
    temp_sum   = 0;
    for (temp = 0; temp < data_size; temp++) {
        tx_data[temp * 2 + 2] = bcd_ascll[p_all_data[temp + prog_address + 4] / 0x10];
        tx_data[temp * 2 + 3] = bcd_ascll[p_all_data[temp + prog_address + 4] % 0x10];
        temp_sum += tx_data[temp * 2 + 2] + tx_data[temp * 2 + 3];
    }
    tx_data[temp * 2 + 2] = 0x03;
    temp_sum += 0x03;
    tx_data[temp * 2 + 3] = bcd_ascll[temp_sum / 0x10];
    tx_data[temp * 2 + 4] = bcd_ascll[temp_sum % 0x10];
    tx_count              = temp * 2 + 4;
}

void PC_READ_byte(void)
{
    prog_address = tx_data[2] * 0x100 + tx_data[3];
    switch (prog_address) {
        case 0x0ECA: read_plc_tyte(); break;
        case 0x0E02: read_plc_tyte(); break;
        default: read_other_data(); break;
    }
}

void PC_WRITE_byte(void)
{
    u16 temp;
    prog_address = tx_data[2] * 0x100 + tx_data[3] + 4;
    for (temp = 0; temp < data_size; temp++) {
        p_all_data[temp + prog_address] = tx_data[5 + temp];
    }
    tx_data[1] = 0x06, tx_count = 1, rx_end = 5;
}

void PC_FORCE_ON(void)
{
    u8* p_bit;
    p_bit        = p_all_data;
    prog_address = (tx_data[2] * 0x100 + tx_data[3]);
    p_bit += prog_address / 0x08 + 4;
    *p_bit |= 1 << (prog_address % 0x08);
    tx_data[1] = 0x06, tx_count = 1, rx_end = 5;
}

void PC_FORCE_OFF(void)
{
    u8* p_bit;
    p_bit        = p_all_data;
    prog_address = (tx_data[2] * 0x100 + tx_data[3]);
    p_bit += prog_address / 0x08 + 4;
    *p_bit &= !(1 << (prog_address % 0x08));
    tx_data[1] = 0x06, tx_count = 1, rx_end = 5;
}

void EPC_FORCE_ON(void)
{
    u8* p_bit;
    p_bit = p_all_data;
    p_bit += prog_address / 0x08 + 4;
    switch (prog_address) {
        case 0x0E23:; break;
        case 0x0E24: prog_address = 0X01C4, p_all_data[prog_address] = 0x09; break;
        case 0x0E25: prog_address = 0X01C4, p_all_data[prog_address] = 0x0A; break;
        default: *p_bit |= 1 << (prog_address % 0x08); break;
    }
    tx_data[1] = 0x06, tx_count = 1, rx_end = 5;
}

void EPC_FORCE_OFF(void)
{
    u8* p_bit;
    p_bit = p_all_data;
    p_bit += prog_address / 0x08 + 4;
    *p_bit &= ~(1 << (prog_address % 0x08));
    tx_data[1] = 0x06, tx_count = 1, rx_end = 5;
}

void PC_READ_Parameter(void)
{
    u16 temp, temp_bit, temp_addr, mov_bit;
    u8  temp_sum;
    u8  send, monitor, monitor1, monitor2;
    tx_data[1]   = 0x02;
    temp_sum     = 0;
    prog_address = (tx_data[3] * 0x100 + tx_data[4]);
    if ((prog_address == 0x1790) || (prog_address == 0x17D0)) {
        if (prog_address == 0x1790) {
            monitor1 = all_data[0x1400 / 2] % 0X100;
            for (monitor = 0; monitor < monitor1; monitor++) {
                temp_bit                       = all_data[0x1400 / 2 + 2 + monitor] / 2;
                all_data[0x1790 / 2 + monitor] = all_data[temp_bit];
            }
            monitor2 = all_data[0x1400 / 2 + 1] % 0X100;
            for (monitor1 = 0; monitor1 < monitor2; monitor1++) {
                temp_addr = all_data[0x1400 / 2 + 2 + monitor + monitor1];
                temp_bit  = all_data[temp_addr / 0x10];
                mov_bit   = temp_addr % 0x10;
                if ((temp_bit & (1 << mov_bit)) == (1 << mov_bit))
                    all_data[0x1790 / 2 + monitor + monitor1 / 0x10] |= 1 << (monitor1 % 0x10);
                else
                    all_data[0x1790 / 2 + monitor + monitor1 / 0x10] &= ~(1 << (monitor1 % 0x10));
            }
        }
    }
    if (prog_address > 0x7fff) {
        prog_address -= 0x8000;
        for (temp = 0; temp < data_size; temp++) {
            tx_data[temp * 2 + 2] = bcd_ascll[p_x[prog_address + temp] / 0x10];
            tx_data[temp * 2 + 3] = bcd_ascll[p_x[prog_address + temp] % 0x10];
            temp_sum += tx_data[temp * 2 + 2] + tx_data[temp * 2 + 3];
        }
    } else {
        prog_address += 0x04;
        for (temp = 0; temp < data_size; temp++) {
            send                  = p_all_data[prog_address + temp];
            tx_data[temp * 2 + 2] = bcd_ascll[send / 0x10];
            tx_data[temp * 2 + 3] = bcd_ascll[send % 0x10];
            temp_sum += tx_data[temp * 2 + 2] + tx_data[temp * 2 + 3];
        }
    }
    tx_data[temp * 2 + 2] = 0x03;
    temp_sum += 0x03;
    tx_data[temp * 2 + 3] = bcd_ascll[temp_sum / 0x10];
    tx_data[temp * 2 + 4] = bcd_ascll[temp_sum % 0x10];
    tx_count              = temp * 2 + 4;
}

void PC_WRITE_Parameter(void)
{
    u16 temp;
    prog_address = (tx_data[3] * 0x100 + tx_data[4]);
    if (prog_address > 0x7fff) {
        edit_prog = 0;
        prog_address -= 0x8000;
        for (temp = 0; temp < data_size; temp++) {
            block_contol[0] = (prog_address + temp) / 0x800;
            if (block_contol[0] == block_contol[1]) {
                prog_write_buffer[(prog_address + temp) - block_contol[0] * 0x800] = tx_data[6 + temp];
            } else {
                write_block(block_contol[1]);
                backup_block(block_contol[0]);
                block_contol[1]                                                    = block_contol[0];
                prog_write_buffer[(prog_address + temp) - block_contol[0] * 0x800] = tx_data[6 + temp];
            }
        }
    } else {
        prog_address += 0x04;
        for (temp = 0; temp < data_size; temp++) {
            p_all_data[prog_address + temp] = tx_data[6 + temp];
            ;
        }
    }
    tx_data[1] = 0x06, tx_count = 1, rx_end = 5;
}

void PC_READ_PORG(void)
{
    u16 temp;
    u8  temp_sum;
    tx_data[1] = 0x02;
    temp_sum   = 0;
    if (data_address > 0x7fff) {
        data_address -= 0x8000;
        for (temp = 0; temp < data_size; temp++) {
            tx_data[temp * 2 + 2] = bcd_ascll[p_x[data_address + temp] / 0x10];
            tx_data[temp * 2 + 3] = bcd_ascll[p_x[data_address + temp] % 0x10];
            temp_sum += tx_data[temp * 2 + 2] + tx_data[temp * 2 + 3];
        }
    } else {
        data_address += 0x4;
        for (temp = 0; temp < data_size; temp++) {
            tx_data[temp * 2 + 2] = bcd_ascll[p_all_data[data_address + temp] / 0x10];
            tx_data[temp * 2 + 3] = bcd_ascll[p_all_data[data_address + temp] % 0x10];
            temp_sum += tx_data[temp * 2 + 2] + tx_data[temp * 2 + 3];
        }
    }
    tx_data[temp * 2 + 2] = 0x03;
    temp_sum += 0x03;
    tx_data[temp * 2 + 3] = bcd_ascll[temp_sum / 0x10];
    tx_data[temp * 2 + 4] = bcd_ascll[temp_sum % 0x10];
    tx_count              = temp * 2 + 4;
}

void PC_WRITE_PORG(void)
{
    u16 temp;
    prog_address = (tx_data[3] * 0x100 + tx_data[4]);
    if (prog_address > 0x7fff) {
        edit_prog = 0;
        prog_address -= 0x8000;
        for (temp = 0; temp < data_size; temp++) {
            block_contol[0] = (prog_address + temp) / 0x800;
            if (block_contol[0] == block_contol[1]) {
                prog_write_buffer[(prog_address + temp) - block_contol[0] * 0x800] = tx_data[6 + temp];
            } else {
                write_block(block_contol[1]);
                backup_block(block_contol[0]);
                block_contol[1]                                                    = block_contol[0];
                prog_write_buffer[(prog_address + temp) - block_contol[0] * 0x800] = tx_data[6 + temp];
            }
        }
    } else {
        prog_address += 0x04;
        for (temp = 0; temp < data_size; temp++) {
            p_all_data[prog_address + temp] = tx_data[6 + temp];
            ;
        }
    }
    tx_data[1] = 0x06, tx_count = 1, rx_end = 5;
}

u16 find_data(u16 addr, u16 find_data)
{
    u8 find_ok, data_H, data_L;
    find_ok = 5;
    data_H  = find_data / 0x100;
    data_L  = find_data % 0x100;
    if (addr > 0x7fff) {
        addr -= 0x8000;
        do {
            if ((p_x[addr] == data_L) && (p_x[addr + 1] == data_H))
                find_ok = 0;
            else
                addr += 2;
            if (addr > (0xdedb - 0x8000))
                find_ok = 1;
        } while (find_ok > 3);
    }
    addr += 0X8000;
    return addr;
}

void find_data_address(void)
{
    u8 temp_sum, data_H, data_L;
    if (data_address > 0x7fff) {
        data_L       = tx_data[5];
        data_H       = tx_data[6];
        data_address = find_data(data_address, data_H * 0X100 + data_L);
        tx_data[1]   = 0x02;
        temp_sum     = 0;
        tx_data[2]   = 0x31;
        temp_sum += tx_data[2];
        data_H     = data_address / 0x100;
        data_L     = data_address % 0x100;
        tx_data[3] = bcd_ascll[data_H / 0X10];
        tx_data[4] = bcd_ascll[data_H % 0X10];
        tx_data[5] = bcd_ascll[data_L / 0X10];
        tx_data[6] = bcd_ascll[data_L % 0X10];
        tx_data[7] = 0X03;
        temp_sum += tx_data[3];
        temp_sum += tx_data[4];
        temp_sum += tx_data[5];
        temp_sum += tx_data[6];
        temp_sum += tx_data[7];
        tx_data[8] = bcd_ascll[temp_sum / 0x10];
        tx_data[9] = bcd_ascll[temp_sum % 0x10];
        tx_count   = 9;
    } else {
    }
}

void backup_mov_block(u16 number)
{
    u16 temp, appoint_address;
    if (number < 10) {
        for (temp = 0; temp < 2048; temp++) {
            appoint_address         = number * 0x800 + temp;
            prog_write_buffer[temp] = p_x[appoint_address];
        }
    }
}

void mov_flash(u16 addr, u8 mov_addr)
{
    u16       start_addr, end_addr, backup_addr, temp, temp1, temp2, mov_byte, addr_mov;
    static u8 offset;
    offset     = mov_addr;
    end_addr   = find_data(addr + 0x8000, 0x000f) + mov_addr - 0x8000;
    start_addr = end_addr;
    addr_mov   = addr;

    if (addr > 0x5B) {
        addr_mov -= 0X5C;
        end_addr -= 0x5C;
        addr_mov /= 2;
        end_addr /= 2;
        addr_mov /= 8;
        end_addr /= 8;
        offset /= 2;
        mov_byte = offset / 8;
        offset %= 8;
        while (!(end_addr == addr_mov)) {
            temp = step_status[end_addr] * 0x100 + step_status[end_addr - 1];
            temp <<= offset;
            step_status[end_addr + mov_byte] = temp / 0x100;
            end_addr--;
        }
        temp = step_status[end_addr] * 0x100 + step_status[end_addr - 1];
        temp <<= offset;
        step_status[end_addr + mov_byte] = temp / 0x100;
    }
    end_addr = start_addr;
    temp     = start_addr;
    do {
        if ((end_addr / 0x800) == (addr / 0x800))
            start_addr = addr % 0x800;
        else
            start_addr = 0;
        if ((temp / 0x800) == (end_addr / 0x800))
            temp1 = end_addr % 0x800 + 1;
        else
            temp1 = 2048;
        backup_block(end_addr / 0x800);
        for (temp2 = start_addr; temp2 < temp1 + 1; temp2++) {
            backup_addr              = (end_addr / 0x800) * 0x800 + temp2 - mov_addr;
            prog_write_buffer[temp2] = p_x[backup_addr];
        }
        write_block(end_addr / 0x800);
        end_addr -= (temp1 - start_addr);
    } while (end_addr > addr + mov_addr);
}

void online_write_data(void)
{
    u16 temp;
    u8  temp1, temp2;
    temp1 = tx_data[5];
    temp2 = tx_data[6];
    temp2 -= temp1;
    if (temp2 > 0) {
        mov_flash(data_address - 0x8000, temp2);
    }
    edit_prog       = 0;
    block_contol[0] = 100;
    block_contol[1] = 100;
    prog_address    = (tx_data[3] * 0x100 + tx_data[4]) - 0x8000;
    data_size       = tx_data[6];
    for (temp = 0; temp < data_size; temp++) {
        block_contol[0] = (prog_address + temp) / 0x800;
        if (block_contol[0] == block_contol[1]) {
            prog_write_buffer[(prog_address + temp) - block_contol[0] * 0x800] = tx_data[7 + temp];
        } else {
            write_block(block_contol[1]);
            backup_block(block_contol[0]);
            block_contol[1]                                                    = block_contol[0];
            prog_write_buffer[(prog_address + temp) - block_contol[0] * 0x800] = tx_data[7 + temp];
        }
    }
    write_block(block_contol[0]);
    tx_data[1] = 0x06, tx_count = 1, rx_end = 5;
}

void all_flash_unlock(void)
{
    block_contol[1] = 200;
    block_contol[0] = 200;
    tx_data[1] = 0x06, tx_count = 1, rx_end = 5;
}

void all_flash_lock(void)
{
    write_block(block_contol[1]);
    block_contol[1] = 200;
    block_contol[0] = 200;
    FLASH_Lock();
    tx_data[1] = 0x06, tx_count = 1, rx_end = 5;
}

void PC_OPTION_PROG(void)
{
    u16 temp;
    if ((rx_count == 11) && ((rx_data[4] == 0x37) || (rx_data[4] == 0x38))) {
        prog_address = ascll_bcd[rx_data[5] - 0x30] * 0x10 + ascll_bcd[rx_data[6] - 0x30] + ascll_bcd[rx_data[7] - 0x30] * 0x1000 + ascll_bcd[rx_data[8] - 0x30] * 0x100;
        if (rx_data[4] == 0x37)
            EPC_FORCE_ON();
        else
            EPC_FORCE_OFF();
    } else {
        setup_LH();
        temp = tx_data[2];
        switch (temp) {
            case 0x00: PC_READ_Parameter(); break;
            case 0x10: PC_WRITE_Parameter(); break;
            case 0x01: PC_READ_PORG(); break;
            case 0x11:
                PC_WRITE_PORG(), edit_prog = 0;
                ;
                break;
            case 0x77: all_flash_unlock(); break;
            case 0x87: all_flash_lock(); break;
            case 0x41: find_data_address(); break;
            case 0xD1: online_write_data(); break;
            default: tx_data[1] = 0x15, tx_count = 1, rx_end = 5; break;
        }
    }
}

void find_end(void)
{
    if (rx_count == 13)
        tx_data[1] = 0x06, tx_count = 1, rx_end = 5;
    else
        tx_data[1] = 0x06, tx_count = 1, rx_end = 5;
}

void Process_switch(void)
{
    u8 temp;
    switch_read_data();
    temp = rx_data[3];
    switch (temp) {
        case 0x30: data_size = tx_data[4], PC_READ_byte(); break;
        case 0x31: data_size = tx_data[4], PC_WRITE_byte(); break;
        case 0x34: find_end(); break;
        case 0x37: setup_HL(), PC_FORCE_ON(); break;
        case 0x38: setup_HL(), PC_FORCE_OFF(); break;
        case 0x42: all_flash_lock(); break;
        case 0x45: data_size = tx_data[5], PC_OPTION_PROG(); break;
        default: break;
    }
    if ((tx_count == 0) && (rx_count == 0))
        tx_data[1] = 0x15, tx_count = 1, rx_end = 5;
}

void TX_Process(void)
{
    u16 temp;
    if ((tx_count > 0) && (rx_end == 0x00)) {
        USART_SendData(USART1, (0x80 | (tx_data[1])));
        USART_ITConfig(USART1, USART_IT_TC, ENABLE);
        for (temp = 0; temp < tx_count; temp++) {
            tx_data[temp] = tx_data[temp + 1];
        }
        tx_count--;
    } else
        USART_ITConfig(USART1, USART_IT_TC, DISABLE);
}

void RX_Process(void)
{
    rx_data[0] = 0x7f & USART_ReceiveData(USART1);
    if (rx_data[0] == 0X05)
        rx_count = 0, tx_data[1] = 0x06, tx_count = 1, rx_end = 5;
    if (rx_data[0] == 0X02) {
        rx_count = 0x01;
    }
    if (rx_count > 0) {
        rx_count++;
        rx_data[rx_count] = rx_data[0];
        if (rx_count > 500)
            tx_count = 0, rx_count = 0;
        if ((rx_count > 3) && (rx_data[rx_count - 2] == 0x03)) {
            rx_data_sum();
            if (rx_data[599] == 1)
                rx_end = 1;
            else
                tx_data[1] = 0x15, tx_count = 1, rx_end = 5;
        }
    }
}

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        RX_Process();
    }

    if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) == SET) {
        USART_ClearFlag(USART1, USART_FLAG_ORE);
        USART_ReceiveData(USART1);
    }
    if (USART_GetITStatus(USART1, USART_IT_TXE) == SET) {
        USART_ClearITPendingBit(USART1, USART_IT_TXE);
        USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    }
    if (USART_GetITStatus(USART1, USART_IT_TC) == SET) {
        USART_ClearITPendingBit(USART1, USART_IT_TC);
        TX_Process();
    }
}

void write_data(u16 number)
{
    u16 temp, appoint_address;
    if (number < 12) {
        FLASH_Unlock();
        FLASH_ErasePage(flash_start_address + number * 0x800);
        for (temp = 0; temp < 1024; temp++) {
            appoint_address = flash_start_address + number * 0x800 + temp * 2;
            FLASH_ProgramHalfWord(appoint_address, prog_write_buffer[temp]);
        }
        FLASH_Lock();
    }
}

void read_data(u16 number)
{
    u16 temp, appoint_address;
    if (number < 12) {
        for (temp = 0; temp < 1024; temp++) {
            appoint_address         = number * 0x800 + temp * 2;
            prog_write_buffer[temp] = p_x[appoint_address] + (p_x[appoint_address + 1] * 0x100);
        }
    }
}

void backup_data(void)
{
    u16 temp = 0, backup_addr;
    for (backup_addr = (0x2000 + 500); backup_addr < (0x2000 + 950); backup_addr++)
        prog_write_buffer[temp] = all_data[backup_addr], temp++;
    for (backup_addr = (0x0500 + 100); backup_addr < (0x0500 + 150); backup_addr++)
        prog_write_buffer[temp] = all_data[backup_addr], temp++;
    for (backup_addr = (0x0800 + 100); backup_addr < (0x0800 + 150); backup_addr++)
        prog_write_buffer[temp] = all_data[backup_addr], temp++;
    for (backup_addr = (0x0020); backup_addr < (0x0020 + 32); backup_addr++)
        prog_write_buffer[temp] = all_data[backup_addr], temp++;
    write_data(10);
}

void recover_data(void)
{
    u16 temp = 0, backup_addr;
    read_data(10);
    for (backup_addr = (0x2000 + 500); backup_addr < (0x2000 + 950); backup_addr++)  // DM
        all_data[backup_addr] = prog_write_buffer[temp], temp++;
    for (backup_addr = (0x0500 + 100); backup_addr < (0x0500 + 150); backup_addr++)  // C
        all_data[backup_addr] = prog_write_buffer[temp], temp++;
    for (backup_addr = (0x0800 + 100); backup_addr < (0x0800 + 150); backup_addr++)  // T
        all_data[backup_addr] = prog_write_buffer[temp], temp++;
    for (backup_addr = (0x0020); backup_addr < (0x0020 + 32); backup_addr++)  // M512开始-M1024保存
        all_data[backup_addr] = prog_write_buffer[temp], temp++;
}
