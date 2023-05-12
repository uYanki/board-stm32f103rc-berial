/********************************************************/
// CPU需要：STM32F103--RAM内存不小于64K	Flash内存不小于128K
// 本代码已在STM32F103RDT6测试通过
// 编辑日期：20150903
//
/********************************************************/
// 20150905 :ADD command: INC  INCP  DEC  DECP	FLT
/********************************************************/

#include "abs_addr.h"

#define on  0xff
#define off 0x00

 extern u16        all_data[];
 extern u8         p_all_data[];
 extern const u16  x[0x6800];
 extern u8         step_status[];
 extern u8         Run_Flag;  // ADD
 extern void       timer_enable(u16 timer_number);
 extern void       timer_disble(u16 timer_number);
 extern void       backup_data(void);

static uint16_t * p_data, process_value;
static const u16* p_prog;
static uint8_t    T_number, C_number;
static uint16_t   T_value, C_value;
static uint16_t   mov_d_value;
static uint16_t   mov_d_addr;
u16               C_count[2];
static const u16* program_start_addr;
static const u16* prog_p_addr[129];
static const u16* p_save[129];
u16               process[64];
u16               sub_add1, sub_add2;
u8                edit_prog;

u8 find_step(u16 addr)
{
    uint16_t temp1, temp2;
    temp1 = addr >> 3;
    temp2 = addr % 0x08;
    temp2 = (1 << temp2);
    if ((step_status[temp1] & temp2) == temp2)
        return on;
    else
        return off;
}

void set_step(u16 addr)
{
    uint16_t temp1, temp2;
    temp1 = addr >> 3;
    temp2 = addr % 0x08;
    temp2 = (1 << temp2);
    step_status[temp1] |= temp2;
}

void reset_step(u16 addr)
{
    uint16_t temp1, temp2;
    uint8_t  temp4;
    temp1 = addr >> 3;
    temp2 = addr % 0x08;
    temp2 = (1 << temp2);
    temp4 = ~temp2;
    step_status[temp1] &= temp4;
}

static void LD(u16 start_addr, u8 process_addr)
{
    u16 temp2;
    p_data = all_data + start_addr + process_addr / 0x10;
    temp2  = process_addr % 0x10;
    if ((*p_data & (1 << temp2)) == (1 << temp2))
        process_value <<= 1, process_value |= 1;
    else
        process_value <<= 1, process_value &= ~1;
}

static uint8_t LDF(u16 start_addr, u8 process_addr)
{
    u16 temp2;
    p_data = all_data + start_addr + process_addr / 0x10;
    temp2  = process_addr % 0x10;
    if ((*p_data & (1 << temp2)) == (1 << temp2))
        return on;
    else
        return off;
}

static void LDI(u16 start_addr, u8 process_addr)
{
    u16 temp2;
    p_data = all_data + start_addr + process_addr / 0x10;
    temp2  = process_addr % 0x10;
    if ((*p_data & (1 << temp2)) == (1 << temp2))
        process_value <<= 1, process_value &= -1;
    else
        process_value <<= 1, process_value |= 1;
}

void AND(u16 start_addr, u8 process_addr)
{
    u16 temp2;
    p_data = all_data + start_addr + process_addr / 0x10;
    temp2  = process_addr % 0x10;
    if (((*p_data & (1 << temp2)) == (1 << temp2)) && ((process_value & 0X01) == 0X01))
        process_value |= 0X01;
    else
        process_value &= ~0X01;
}

static void ANI(u16 start_addr, u8 process_addr)
{
    u16 temp2;
    p_data = all_data + start_addr + process_addr / 0x10;
    temp2  = process_addr % 0x10;
    if ((!((*p_data & (1 << temp2)) == (1 << temp2))) && ((process_value & 0X01) == 0X01))
        process_value |= 0X01;
    else
        process_value &= ~0X01;
}

static void OR(u16 start_addr, u8 process_addr)
{
    u16 temp2;
    p_data = all_data + start_addr + process_addr / 0x10;
    temp2  = process_addr % 0x10;
    if (((*p_data & (1 << temp2)) == (1 << temp2)) || ((process_value & 0X01) == 0X01))
        process_value |= 0X01;
    else
        process_value &= ~0X01;
}

static void ORI(u16 start_addr, u8 process_addr)
{
    u16 temp2;
    p_data = all_data + start_addr + process_addr / 0x10;
    temp2  = process_addr % 0x10;
    if ((!((*p_data & (1 << temp2)) == (1 << temp2))) || ((process_value & 0X01) == 0X01))
        process_value |= 0X01;
    else
        process_value &= ~0X01;
}

static void OUT(u16 start_addr, u8 process_addr)
{
    u16 temp2;
    p_data = all_data + start_addr + process_addr / 0x10;
    temp2  = process_addr % 0x10;
    if ((process_value & 0X01) == 0X01)
        *p_data |= (1 << temp2);
    else
        *p_data &= ~(1 << temp2);
}

void force_set(u16 start_addr, u8 process_addr)
{
    u16 temp2;
    p_data = all_data + start_addr + process_addr / 0x10;
    temp2  = process_addr % 0x10;
    *p_data |= (1 << temp2);
}

static void BIT_SET(u16 start_addr, u8 process_addr)
{
    u16 temp2;
    p_data = all_data + start_addr + process_addr / 0x10;
    temp2  = process_addr % 0x10;
    if ((process_value & 0X01) == 0X01)
        *p_data |= (1 << temp2);
}

void force_reset(u16 start_addr, u8 process_addr)
{
    u16 temp2;
    p_data = all_data + start_addr + process_addr / 0x10;
    temp2  = process_addr % 0x10;
    *p_data &= ~(1 << temp2);
}

static void RST(u16 start_addr, u8 process_addr)
{
    u16 temp2;
    p_data = all_data + start_addr + process_addr / 0x10;
    temp2  = process_addr % 0x10;
    if ((process_value & 0X01) == 0X01)
        *p_data &= ~(1 << temp2);
}

static void MPS(void)
{
    process_value <<= 1;
    if ((process_value & 0x02) == 0x02)
        process_value |= 0x01;
    else
        process_value &= ~0x01;
}

static void MRD(void)
{
    if ((process_value & 0x02) == 0x02)
        process_value |= 0x01;
    else
        process_value &= ~0x01;
}

static void ORB(void)
{
    u16 temp;
    temp = process_value;
    process_value >>= 1;
    if (((process_value & 0x01) == 0x01) || ((temp & 0X01) == 0X01))
        process_value |= 0x01;
    else
        process_value &= ~0x01;
}

static void ANB(void)
{
    u16 temp;
    temp = process_value;
    process_value >>= 1;
    if (((process_value & 0x01) == 0x01) && ((temp & 0X01) == 0X01))
        process_value |= 0x01;
    else
        process_value &= ~0x01;
}

static void INV(void)
{
    if ((process_value & 0x01) == 0x00)
        process_value |= 0x01;
    else
        process_value &= ~0x01;
}

static void other_function(u8 process_addr)
{
    switch (process_addr) {
        case 0xF8: ANB(); break;  // 块串联 ANB
        case 0xF9: ORB(); break;  // 块并联 ORB
        case 0xFA: MPS(); break;  // 进栈   MPS
        case 0xFB: MRD(); break;  // 读栈   MRD
        case 0xFD: INV(); break;  // 取反   INV
        case 0xFF: break;         // ADD
        default:; break;
    }
}

uint16_t get_m_addr(void)
{
    switch (*p_prog / 0x100) {
        case 0xA8: return 0x0060;
        case 0xA9: return 0x0070;
        case 0xAA: return 0x0080;
        case 0xAB: return 0x0090;
        case 0xAC: return 0x00A0;
        case 0xAD: return 0x00B0;
        default: return 0x0000;
    }
}

uint16_t get_s_addr(void)
{
    switch (*p_prog / 0x100) {
        case 0x80: return 0X0140;
        case 0x81: return 0X0150;
        case 0x82: return 0X0160;
        case 0x83: return 0X0170;
        default: return 0x0000;
    }
}

static void extend_LD_M(void) { LD(get_m_addr(), *p_prog++); }

static void extend_LDI_M(void) { LDI(get_m_addr(), *p_prog++); }

static void extend_OR_M(void) { OR(get_m_addr(), *p_prog++); }

static void extend_ORI_M(void) { ORI(get_m_addr(), *p_prog++); }

static void extend_AND_M(void) { AND(get_m_addr(), *p_prog++); }

static void extend_ANI_M(void) { ANI(get_m_addr(), *p_prog++); }

static void extend_SET_M(void) { BIT_SET(get_m_addr(), *p_prog++); }

static void extend_RST_M(void) { RST(get_m_addr(), *p_prog++); }

static void extend_OUT_M(void) { OUT(get_m_addr(), *p_prog++); }

static void extend_SET_S(void)
{
    BIT_SET(get_s_addr(), *p_prog++);
}

static void extend_RST_S(void)
{
    RST(get_s_addr(), *p_prog++);
}

static void extend_OUT_S(void)
{
    OUT(get_s_addr(), *p_prog++);
}

static void RESET_T(u8 process_addr)
{
    if ((process_value & 0x01) == 0x01) {
        timer_disble(process_addr);
    }
    OUT(0X0380, *p_prog);
}

static void RESET_C(u8 process_addr)
{
    if ((process_value & 0x01) == 0x01) {
        p_data  = all_data + 0x0500 + process_addr;
        *p_data = 0;
        p_data  = all_data + 0x00F0 + (process_addr / 0x10);
        *p_data &= ~(1 << process_addr % 0x10);
    }
    OUT(0X0370, *p_prog);
}

static void extend_RST_T(void)
{
    switch (*p_prog / 0x100) {
        case 0x86: RESET_T(*p_prog), p_prog++; break;
        case 0x8E: RESET_C(*p_prog), p_prog++; break;
    }
}

uint16_t get_addr(uint16_t addr)
{
    switch (*p_prog % 0x100) {
        case 0x00: return addr + 0x0140;
        case 0x01: return addr + 0x0150;
        case 0x02: return addr + 0x0160;
        case 0x03: return addr + 0x0170;
        case 0x04: return addr + 0x0120;
        case 0x05: return addr + 0x00C0;
        case 0x08: return addr + 0x0000;
        case 0x09: return addr + 0x0010;
        case 0x0A: return addr + 0x0020;
        case 0x0B: return addr + 0x0030;
        case 0x0C: return addr + 0x0040;
        case 0x0D: return addr + 0x0050;
        case 0x28: return addr + 0x0060;
        case 0x29: return addr + 0x0070;
        case 0x2A: return addr + 0x0080;
        case 0x2B: return addr + 0x0090;
        case 0x2C: return addr + 0x00A0;
        case 0x2D: return addr + 0x00B0;
        default: return 0;
    }
}

static void MOV_TO_K_H(void)
{
    u32 MOV_DATA_16BIT, MOV_DATA_16BIT_BACKUP, MOV_DATA_BACKUP1;

    uint16_t addr = get_addr(mov_d_addr / 0x10);
    uint16_t mask = 0x00;

    u8 LL_BIT = mov_d_addr % 0x10;

    switch (*p_prog / 0x100) {
        case 0x82: mask &= 0X000F; break;
        case 0x84: mask &= 0X00FF; break;
        case 0x86: mask &= 0X0FFF; break;
        case 0x88: mask &= 0XFFFF; break;
        default: p_prog += 3; break;
    }

    MOV_DATA_16BIT_BACKUP = mov_d_value & mask;
    MOV_DATA_16BIT_BACKUP <<= LL_BIT;
    MOV_DATA_BACKUP1 = ~(mask << LL_BIT);

    if (addr) {
        MOV_DATA_16BIT = all_data[addr] + (all_data[addr + 1]) * 0X10000,
        MOV_DATA_16BIT &= MOV_DATA_BACKUP1, MOV_DATA_16BIT |= MOV_DATA_16BIT_BACKUP,
        all_data[addr] = MOV_DATA_16BIT, all_data[addr + 1] = MOV_DATA_16BIT / 0X10000;
    } else {
        ++p_prog;
    }

    ++p_prog;
}

static void MOV_K(void)
{
    static uint8_t LL_BIT;
    static u32     MOV_DATA_16BIT;

    uint16_t addr = get_addr(mov_d_addr / 0x10);

    LL_BIT = mov_d_value % 0x10;

    if (addr) {
        MOV_DATA_16BIT = all_data[addr] + (all_data[addr + 1]) * 0X10000,
        MOV_DATA_16BIT >>= LL_BIT, mov_d_value = MOV_DATA_16BIT;
    } else {
        p_prog += 2;
    }
}

static void K_M_MOV_D_H(void)
{
    switch (*p_prog / 0x100) {
        case 0x82: MOV_K(), mov_d_value &= 0x000F; break;
        case 0x84: MOV_K(), mov_d_value &= 0x00FF; break;
        case 0x86: MOV_K(), mov_d_value &= 0x0FFF; break;
        case 0x88: MOV_K(), mov_d_value &= 0xFFFF; break;
        default: p_prog += 3; break;
    }
}

static uint16_t h_cos_value(u16 l_value)
{
    static uint16_t temp;
    switch (*p_prog / 0x100) {
        case 0x80: temp = l_value + ((*p_prog % 0x100) * 0x100), p_prog++; break;
        case 0x82: temp = l_value + ((*p_prog % 0x100) * 0x100), temp = all_data[0x0800 + temp / 2], p_prog++; break;
        case 0x84: temp = l_value + ((*p_prog % 0x100) * 0x100), temp = all_data[0x0500 + temp / 2], p_prog++; break;
        case 0x86: temp = l_value + ((*p_prog % 0x100) * 0x100), temp = all_data[0x2000 + temp / 2], p_prog++; break;
        case 0x88: temp = l_value + ((*p_prog % 0x100) * 0x100), temp = all_data[0x2000 + temp / 2 + 1000], p_prog++; break;
    }
    return temp;
}

static uint16_t cos_value(void)
{
    static uint16_t temp;
    switch (*p_prog / 0x100) {
        case 0x80: temp = *p_prog % 0x100, p_prog++, temp = h_cos_value(temp); break;
        case 0x82: temp = *p_prog % 0x100, p_prog++, temp = h_cos_value(temp); break;
        case 0x84: mov_d_value = *p_prog % 0x100, p_prog++, K_M_MOV_D_H(), temp = mov_d_value, p_prog++; break;
        case 0x86: temp = *p_prog % 0x100, p_prog++, temp = h_cos_value(temp); break;
        case 0x88: temp = *p_prog % 0x100, p_prog++, temp = h_cos_value(temp); break;
    }
    return temp;
}

static void add_assgin_value(u16 assgin_value)
{
    uint16_t addr = assgin_value;
    addr += (*p_prog & 0xFF) << 8;
    addr >>= 1;  // addr/=2;

    switch (*p_prog >> 8) {
        case 0x82: addr += 0x0800; break;
        case 0x84: addr += 0x0500; break;
        case 0x86: addr += 0x2000; break;
        case 0x88: addr += 0x2000 + 1000; break;
    }

    all_data[addr] = sub_add1;

    ++p_prog;
}

static void add_target(void)
{
    u16 temp;
    switch (*p_prog / 0x100) {
        case 0x82: temp = *p_prog % 0x100, p_prog++, add_assgin_value(temp); break;
        case 0x84: mov_d_addr = *p_prog % 0x100, p_prog++, mov_d_value = sub_add1, MOV_TO_K_H(); break;
        case 0x86: temp = *p_prog % 0x100, p_prog++, add_assgin_value(temp); break;
        case 0x88: temp = *p_prog % 0x100, p_prog++, add_assgin_value(temp); break;
    }
}

static void div_assign_value(u16 l_value)
{
    uint16_t addr = l_value;

    addr += (*p_prog & 0xFF) << 8;
    addr >>= 1;

    switch (*p_prog >> 8) {
        case 0x82: addr += 0x0800; break;
        case 0x84: addr += 0x0500; break;
        case 0x86: addr += 0x2000; break;
        case 0x88: addr += 0x2000 + 1000; break;
    }

    all_data[addr]     = sub_add1;
    all_data[addr + 1] = sub_add2;

    ++p_prog;
}

static void mul_target(void)
{
    u16 temp;
    switch (*p_prog / 0x100) {
        case 0x82: temp = *p_prog % 0x100, p_prog++, div_assign_value(temp); break;
        case 0x84: mov_d_addr = *p_prog % 0x100, p_prog++, mov_d_value = sub_add1, MOV_TO_K_H(); break;
        case 0x86: temp = *p_prog % 0x100, p_prog++, div_assign_value(temp); break;
        case 0x88: temp = *p_prog % 0x100, p_prog++, div_assign_value(temp); break;
    }
}

static void extend_MOV(void)
{
    if ((process_value & 0X01) == 0X01)
        sub_add1 = cos_value(), add_target();
    else
        p_prog += 4;
}

static void extend_MOVP(void)
{
    u8 logic_1;

    logic_1 = find_step(p_prog - program_start_addr);
    if (logic_1 == off) {
        if ((process_value & 0x01) == 0x01) {
            set_step(p_prog - program_start_addr);
            sub_add1 = cos_value(), add_target();
        } else
            p_prog += 4;
    } else {
        if (!((process_value & 0x01) == 0x01))
            reset_step(p_prog - program_start_addr);
        p_prog += 4;
    }
}

static void add(void)
{
    signed short int temp1, temp2;
    if ((process_value & 0X01) == 0X01) {
        temp1    = cos_value();
        temp2    = cos_value();
        sub_add1 = temp1 + temp2;
        add_target();
    }
}

static void INC(void)  // ADD TKZSLD
{
    if ((process_value & 0X01) == 0X01) {
        sub_add1 = cos_value();
        p_prog -= 2;
        sub_add1++;
        add_target();
    } else
        p_prog += 2;
}

static void extend_INCP(void)
{
    u8 logic_1;

    logic_1 = find_step(p_prog - program_start_addr);
    if (logic_1 == off) {
        if ((process_value & 0x01) == 0x01) {
            set_step(p_prog - program_start_addr);
            INC();
        } else
            p_prog += 2;
    } else {
        if (!((process_value & 0x01) == 0x01))
            reset_step(p_prog - program_start_addr);
        p_prog += 2;
    }
}

static void DEC(void)  // ADD TKZSLD	//逻辑运算 减1指令
{
    if ((process_value & 0X01) == 0X01) {
        sub_add1 = cos_value();
        p_prog -= 2;
        sub_add1--;
        add_target();
    } else
        p_prog += 2;  // 跳过2步程序
}

static void extend_DECP(void)
{
    u8 logic_1;

    logic_1 = find_step(p_prog - program_start_addr);
    if (logic_1 == off) {
        if ((process_value & 0x01) == 0x01) {
            set_step(p_prog - program_start_addr);
            DEC();
        } else
            p_prog += 2;
    } else {
        if (!((process_value & 0x01) == 0x01))
            reset_step(p_prog - program_start_addr);
        p_prog += 2;
    }
}

static void extend_ADDP(void)
{
    u8 logic_1;

    logic_1 = find_step(p_prog - program_start_addr);
    if (logic_1 == off) {
        if ((process_value & 0x01) == 0x01) {
            set_step(p_prog - program_start_addr);
            add();
        } else
            p_prog += 4;
    } else {
        if (!((process_value & 0x01) == 0x01))
            reset_step(p_prog - program_start_addr);
        p_prog += 4;
    }
}

static void sub(void)
{
    signed short int temp1, temp2;
    if ((process_value & 0X01) == 0X01) {
        temp1    = cos_value();
        temp2    = cos_value();
        sub_add1 = temp1 - temp2;
        add_target();
    }
}

static void extend_SUBP(void)
{
    u8 logic_1;

    logic_1 = find_step(p_prog - program_start_addr);
    if (logic_1 == off) {
        if ((process_value & 0x01) == 0x01) {
            set_step(p_prog - program_start_addr);
            sub();
        } else
            p_prog += 4;
    } else {
        if (!((process_value & 0x01) == 0x01))
            reset_step(p_prog - program_start_addr);
        p_prog += 4;
    }
}

static void mul(void)
{
    signed int temp1, temp2;
    if ((process_value & 0X01) == 0X01) {
        temp1    = cos_value();
        temp2    = cos_value();
        sub_add1 = (temp1 * temp2) % 0x10000;
        sub_add2 = (temp1 * temp2) / 0x10000;
        mul_target();
    }
}

static void extend_MULP(void)
{
    u8 logic_1;

    logic_1 = find_step(p_prog - program_start_addr);
    if (logic_1 == off) {
        if ((process_value & 0x01) == 0x01) {
            set_step(p_prog - program_start_addr);
            mul();
        } else
            p_prog += 4;
    } else {
        if (!((process_value & 0x01) == 0x01))
            reset_step(p_prog - program_start_addr);
        p_prog += 4;
    }
}

static void div(void)
{
    signed short int temp1, temp2;
    if ((process_value & 0X01) == 0X01) {
        temp1    = cos_value();
        temp2    = cos_value();
        sub_add1 = temp1 / temp2;
        sub_add2 = temp1 % temp2;
        mul_target();
    }
}

static void extend_DIVP(void)
{
    u8 logic_1;

    logic_1 = find_step(p_prog - program_start_addr);
    if (logic_1 == off) {
        if ((process_value & 0x01) == 0x01) {
            set_step(p_prog - program_start_addr);
            div();
        } else
            p_prog += 4;
    } else {
        if (!((process_value & 0x01) == 0x01))
            reset_step(p_prog - program_start_addr);
        p_prog += 4;
    }
}

static void amount(void)
{
    signed short int temp1, temp2;
    temp1 = cos_value();
    temp2 = cos_value();
    if (temp1 == temp2)
        process_value <<= 1, process_value |= 1;
    else
        process_value <<= 1, process_value &= ~1;
}

static void big(void)
{
    signed short int temp1, temp2;
    temp1 = cos_value();
    temp2 = cos_value();
    if (temp1 > temp2)
        process_value <<= 1, process_value |= 1;
    else
        process_value <<= 1, process_value &= ~1;
}

static void less(void)
{
    signed short int temp1, temp2;
    temp1 = cos_value();
    temp2 = cos_value();
    if (temp1 < temp2)
        process_value <<= 1, process_value |= 1;
    else
        process_value <<= 1, process_value &= ~1;
}

static void less_amount(void)
{
    signed short int temp1, temp2;
    temp1 = cos_value();
    temp2 = cos_value();
    if (temp1 > temp2)
        process_value <<= 1, process_value |= 1;
    else
        process_value <<= 1, process_value &= ~1;
}

static void big_amount(void)
{
    signed short int temp1, temp2;
    temp1 = cos_value();
    temp2 = cos_value();
    if (temp1 < temp2)
        process_value <<= 1, process_value |= 1;
    else
        process_value <<= 1, process_value &= ~1;
}

static void amount_and(void)
{
    signed short int temp1, temp2;
    temp1 = cos_value();
    temp2 = cos_value();
    if ((temp1 == temp2) && ((process_value & 0X01) == 0X01))
        process_value |= 1;
    else
        process_value &= ~1;
}

static void big_and(void)
{
    signed short int temp1, temp2;
    temp1 = cos_value();
    temp2 = cos_value();
    if ((temp1 > temp2) && ((process_value & 0X01) == 0X01))
        process_value |= 1;
    else
        process_value &= ~1;
}

static void less_and(void)
{
    signed short int temp1, temp2;
    temp1 = cos_value();
    temp2 = cos_value();
    if ((temp1 < temp2) && ((process_value & 0X01) == 0X01))
        process_value |= 1;
    else
        process_value &= ~1;
}

static void less_amount_and(void)
{
    signed short int temp1, temp2;
    temp1 = cos_value();
    temp2 = cos_value();
    if ((temp1 > temp2) && ((process_value & 0X01) == 0X01))
        process_value |= 1;
    else
        process_value &= ~1;
}

static void big_amount_and(void)
{
    signed short int temp1, temp2;
    temp1 = cos_value();
    temp2 = cos_value();
    if ((temp1 < temp2) && ((process_value & 0X01) == 0X01))
        process_value |= 1;
    else
        process_value &= ~1;
}

static uint8_t bit_value(void)
{
    u8 temp;
    switch (*p_prog / 0X100) {
        case 0x80: temp = LDF(0X0140, *p_prog); break;
        case 0x81: temp = LDF(0X0150, *p_prog); break;
        case 0x82: temp = LDF(0X0160, *p_prog); break;
        case 0x83: temp = LDF(0X0170, *p_prog); break;
        case 0x84: temp = LDF(0X0120, *p_prog); break;
        case 0x85: temp = LDF(0X00C0, *p_prog); break;
        case 0x86: temp = LDF(0X0100, *p_prog); break;

        case 0x88: temp = LDF(0X0000, *p_prog); break;
        case 0x89: temp = LDF(0X0010, *p_prog); break;
        case 0x8A: temp = LDF(0X0020, *p_prog); break;
        case 0x8B: temp = LDF(0X0030, *p_prog); break;
        case 0x8C: temp = LDF(0X0040, *p_prog); break;
        case 0x8D: temp = LDF(0X0050, *p_prog); break;
        case 0x8E: temp = LDF(0X00F0, *p_prog); break;
        case 0x8F: temp = LDF(0X00E0, *p_prog); break;

        case 0xA8: temp = LDF(0X0060, *p_prog); break;
        case 0xA9: temp = LDF(0X0070, *p_prog); break;
        case 0xAA: temp = LDF(0X0080, *p_prog); break;
        case 0xAB: temp = LDF(0X0090, *p_prog); break;
        case 0xAC: temp = LDF(0X00A0, *p_prog); break;
        case 0xAD: temp = LDF(0X00B0, *p_prog); break;

        default: temp = 2; break;
    }
    return temp;
}

static void extend_LDP(void)
{
    u8 logic_1, logic_2;

    logic_1 = find_step(p_prog - program_start_addr);
    logic_2 = bit_value();
    process_value <<= 1;
    if (logic_1 == off) {
        if (logic_2 == on)
            process_value |= 1, set_step(p_prog - program_start_addr);
        else
            process_value &= ~1;

    } else {
        if (logic_2 == on)
            process_value &= ~1;
        else
            process_value &= ~1, reset_step(p_prog - program_start_addr);
    }
    p_prog++;
}

static void extend_LDF(void)
{
    u8 logic_3, logic_4;

    logic_3 = find_step(p_prog - program_start_addr);
    logic_4 = bit_value();
    process_value <<= 1;
    if (logic_3 == on) {
        if (logic_4 == off)
            process_value |= 1, reset_step(p_prog - program_start_addr);
        else
            process_value &= ~1;
    } else {
        if (logic_4 == off)
            process_value &= ~1;
        else
            process_value &= ~1, set_step(p_prog - program_start_addr);
    }
    p_prog++;
}

static void extend_ANDP(void)
{
    u8 logic_1, logic_2;

    logic_1 = find_step(p_prog - program_start_addr);
    logic_2 = bit_value();
    if (logic_1 == off) {
        if (logic_2 == on)
            logic_2 = 1, set_step(p_prog - program_start_addr);
        else
            logic_2 = 0;
    } else {
        if (logic_2 == on)
            logic_2 = 0;
        else
            logic_2 = 0, reset_step(p_prog - program_start_addr);
    }
    if (((process_value & 0x01) == 0x01) && (logic_2 == 1))
        process_value |= 1;
    else
        process_value &= ~1;
    p_prog++;
}

static void extend_ANDF(void)
{
    u8 logic_3, logic_4;

    logic_3 = find_step(p_prog - program_start_addr);
    logic_4 = bit_value();
    if (logic_3 == on) {
        if (logic_4 == off)
            logic_4 = 1, reset_step(p_prog - program_start_addr);
        else
            logic_4 = 0;
    } else {
        if (logic_4 == off)
            logic_4 = 0;
        else
            logic_4 = 0, set_step(p_prog - program_start_addr);
    }
    if (((process_value & 0x01) == 0x01) && (logic_4 == 1))
        process_value |= 1;
    else
        process_value &= ~1;
    p_prog++;
}

static void extend_ORP(void)
{
    u8 logic_1, logic_2;

    logic_1 = find_step(p_prog - program_start_addr);
    logic_2 = bit_value();
    if (logic_1 == off) {
        if (logic_2 == on)
            logic_2 = 1, set_step(p_prog - program_start_addr);
        else
            logic_2 = 0;
    } else {
        if (logic_2 == on)
            logic_2 = 0;
        else
            logic_2 = 0, reset_step(p_prog - program_start_addr);
    }
    if (((process_value & 0x01) == 0x01) || (logic_2 == 1))
        process_value |= 1;
    else
        process_value &= ~1;
    p_prog++;
}

static void extend_ORF(void)
{
    u8 logic_3, logic_4;

    logic_3 = find_step(p_prog - program_start_addr);
    logic_4 = bit_value();
    if (logic_3 == on) {
        if (logic_4 == off)
            logic_4 = 1, reset_step(p_prog - program_start_addr);
        else
            logic_4 = 0;
    } else {
        if (logic_4 == off)
            logic_4 = 0;
        else
            logic_4 = 0, set_step(p_prog - program_start_addr);
    }
    if (((process_value & 0x01) == 0x01) || (logic_4 == 1))
        process_value |= 1;
    else
        process_value &= ~1;
    p_prog++;
}

static void CJ_EX(u16 value)
{
    p_prog++;
    switch (*p_prog / 0X100) {
        case 0x80: p_prog = prog_p_addr[value / 2], p_prog++; break;
        default: break;
    }
}

static void CJ(void)
{
    if ((process_value & 0X01) == 0X01) {
        switch (*p_prog / 0X100) {
            case 0x88: CJ_EX(*p_prog % 0X100); break;
        }
    }
}

static void RET(void)
{
    u8 temp;
    process_value = process[0];
    p_prog        = p_save[0];
    for (temp = 62; temp > 0; temp--) {
        process[temp] = process[temp + 1];
        p_save[temp]  = p_save[temp + 1];
    }
}

static void P_MOV(void)
{
    u8 temp;
    for (temp = 62; temp > 0; temp--) {
        process[temp + 1] = process[temp];
        p_save[temp + 1]  = p_save[temp];
    }
    process[0] = process_value;
    p_save[0]  = p_prog;
}

static void CALL_EX(u16 value)
{
    p_prog++;
    switch (*p_prog / 0X100) {
        case 0x80: P_MOV(), p_prog = prog_p_addr[value / 2]; break;
    }
}

static void CALL(void)
{
    if ((process_value & 0X01) == 0X01) {
        switch (*p_prog / 0X100) {
            case 0x88: CALL_EX(*p_prog % 0X100), p_prog++; break;
        }
    }
}

static void extend_function(void)
{
    switch (*p_prog++) {
        case 0x0002: extend_OUT_M(); break;
        case 0x0003: extend_SET_M(); break;
        case 0x0004: extend_RST_M(); break;

        case 0x0005: extend_OUT_S(); break;
        case 0x0006: extend_SET_S(); break;
        case 0x0007: extend_RST_S(); break;

        case 0x000C: extend_RST_T(); break;
        case 0X0038: add(); break;
        case 0x0040: INC(); break;  // ADD BY TKZSLD
        case 0x0042: DEC(); break;  // ADD BY TKZSLD
        case 0X003A: sub(); break;
        case 0x003C: mul(); break;
        case 0x003E: div(); break;

        case 0x0010: CJ(); break;
        case 0x0012: CALL(); break;

        case 0x01C2: extend_LD_M(); break;
        case 0x01C3: extend_LDI_M(); break;
        case 0x01C4: extend_AND_M(); break;
        case 0x01C5: extend_ANI_M(); break;
        case 0x01C6: extend_OR_M(); break;
        case 0x01C7: extend_ORI_M(); break;

        case 0x01CA: extend_LDP(); break;
        case 0x01CB: extend_LDF(); break;
        case 0x01CC: extend_ANDP(); break;
        case 0x01CD: extend_ANDF(); break;
        case 0x01CE: extend_ORP(); break;
        case 0x01CF: extend_ORF(); break;

        case 0x0028: extend_MOV(); break;

        case 0x000f: --p_prog; break;

        case 0X01D0: amount(); break;
        case 0X01D2: big(); break;
        case 0X01D4: less(); break;
        case 0X01DA: less_amount(); break;
        case 0X01DC: big_amount(); break;
        case 0X1028: extend_MOVP(); break;
        case 0X1038: extend_ADDP(); break;
        case 0X103A: extend_SUBP(); break;
        case 0X103C: extend_MULP(); break;
        case 0X103E: extend_DIVP(); break;

        case 0x1040: extend_INCP(); break;  // 16位上升沿逻辑运算加1指令 tkzsld
        case 0x1042: extend_DECP(); break;  // 16位上升沿逻辑运算减1指令 tkzsld

        case 0X01E0: amount_and(); break;
        case 0X01E2: big_and(); break;
        case 0X01E4: less_and(); break;
        case 0X01EA: less_amount_and(); break;
        case 0X01EC: big_amount_and(); break;

        case 0XF7FF: RET(); break;
    }
}

static void enable_T_K(void)
{
    T_value = *p_prog % 0x100;
    p_prog++;
    T_value += (*p_prog % 0x100) * 0x100;
    p_data  = all_data + 0x1000 + T_number;
    *p_data = T_value;
    timer_enable(T_number);
    OUT(0X280, T_number);
}

static void enable_T_D(void)
{
    p_data  = all_data + 0x1000 + T_number;
    *p_data = all_data[0x2000 + T_value];
    timer_enable(T_number);
    OUT(0X280, T_number);
}

static void disable_T(void)
{
    timer_disble(T_number);
    OUT(0X0280, T_number);
    OUT(0x0100, T_number);
}

static void T_given_value_K(void)
{
    if ((process_value & 0X01) == 0X01)
        enable_T_K();
    else
        disable_T();
}

static void T_given_value_D(void)
{
    T_value = (*p_prog % 0x100) / 2;
    p_prog++;
    switch (*p_prog / 0x100) {
        case 0x86: T_value += (*p_prog % 0x100) * 0x80; break;
        case 0x88: T_value += (*p_prog % 0x100) * 0x80 + 1000; break;
    }
    if ((process_value & 0X01) == 0X01)
        enable_T_D();
    else
        disable_T();
}

static void operation_T(void)
{
    T_number = *p_prog;
    p_prog++;
    switch (*p_prog / 0x100) {
        case 0x80: T_given_value_K(), p_prog++; break;
        case 0x86: T_given_value_D(), p_prog++; break;
    }
}

static void enable_C_K(void)
{
    u16 temp_bit, *p_C_enable_coil;
    C_value = *p_prog % 0x100;
    p_prog++;
    C_value += (*p_prog % 0x100) * 0x100;
    p_data   = all_data + 0x0500 + C_number;
    temp_bit = 1 << (C_number % 0x10);
    if (*p_data < C_value) {
        p_C_enable_coil = all_data + 0x0270 + (C_number / 0X10);
        if (!((*p_C_enable_coil & temp_bit) == temp_bit))
            *p_data += 1;
    }
    if (*p_data < C_value) {
        p_data = all_data + 0x00F0 + (C_number / 0x10);
        *p_data &= ~(1 << (C_number % 0x10));
    } else {
        p_data = all_data + 0x00F0 + (C_number / 0x10);
        *p_data |= (1 << (C_number % 0x10));
    }
    OUT(0X270, C_number);
}

static void enable_C_D(void)
{
    u16 temp_bit, *p_C_enable_coil;
    C_value  = all_data[0x2000 + C_value];
    p_data   = all_data + 0x0500 + C_number;
    temp_bit = 1 << (C_number % 0x10);
    if (*p_data < C_value) {
        p_C_enable_coil = all_data + 0x0270 + (C_number / 0X10);
        if (!((*p_C_enable_coil & temp_bit) == temp_bit))
            *p_data += 1;
    }
    p_C_enable_coil = all_data + 0x00F0 + (C_number / 0x10);
    if (*p_data < C_value)
        *p_C_enable_coil &= ~temp_bit;
    else
        *p_C_enable_coil |= temp_bit;
    OUT(0X270, C_number);
}

static void disable_C_K(void)
{
    C_value = *p_prog % 0x100;
    p_prog++;
    C_value += (*p_prog % 0x100) * 0x100;
    p_data = all_data + 0x0500 + C_number;
    if (*p_data < C_value) {
        p_data = all_data + 0x00F0 + (C_number / 0x10);
        *p_data &= ~(1 << (C_number % 0x10));
    } else {
        p_data = all_data + 0x00F0 + (C_number / 0x10);
        *p_data |= (1 << (C_number % 0x10));
    }
    OUT(0X270, C_number);
}

static void disable_C_D(void)
{
    u16 temp_bit, *p_C_enable_coil;
    C_value         = all_data[0x2000 + C_value];
    p_data          = all_data + 0x0500 + C_number;
    temp_bit        = 1 << (C_number % 0x10);
    p_C_enable_coil = all_data + 0x00F0 + (C_number / 0x10);
    if (*p_data < C_value)
        *p_C_enable_coil &= ~temp_bit;
    else
        *p_C_enable_coil |= temp_bit;

    OUT(0X270, C_number);
}

static void C_given_value_K(void)
{
    if ((process_value & 0X01) == 0X01)
        enable_C_K();
    else
        disable_C_K();
}

static void C_given_value_D(void)
{
    C_value = (*p_prog % 0x100) / 2;
    p_prog++;
    switch (*p_prog / 0x100) {
        case 0x86: C_value += (*p_prog % 0x100) * 0x80; break;
        case 0x88: C_value += (*p_prog % 0x100) * 0x80 + 1000; break;
    }
    if ((process_value & 0X01) == 0X01)
        enable_C_D();
    else
        disable_C_D();
}

static void operation_C(void)
{
    C_number = *p_prog;
    p_prog++;
    switch (*p_prog / 0x100) {
        case 0x80: C_given_value_K(), p_prog++; break;
        case 0x86: C_given_value_D(), p_prog++; break;
    }
}

void find_p(void)
{
    u16 temp;
    p_prog = x + 0x5c / 2 - 1;
    for (temp = 0; temp < 7999; temp++) {
        if ((*p_prog / 0x100) == 0xB0)
            prog_p_addr[*p_prog % 0x100] = p_prog;
        p_prog++;
    }
}

void RST_Y(void)
{
    static uint8_t all_out_rst;
    if (all_out_rst > 10)
        all_out_rst = 0;
    if (p_all_data[0X01C4] == 0x09)
        all_out_rst = 0;
    else {
        if (all_out_rst == 0)
            all_out_rst++, all_data[0x180 / 2] = 0;
    }
}

u8 find_toend(void)
{
    u16 temp;
    p_prog = x + 0x5c / 2 - 2;
    temp   = 0;
    do {
        p_prog++;
        temp++;
    } while ((!(*p_prog == 0x000f)) && (temp < 7998));
    if (temp > 7997)
        return 1;
    else
        return 0;
}

void all_prog_process(void)  // 20151009
{
    u8             temp5, run_keep;
    static uint8_t puls;
    program_start_addr = x + 0x5c / 2 - 1;
    run_keep           = p_all_data[0X01C4];
    RST_Y();

    if (RUN_STOP == 0)  // MY PCB== 0	 AIR=1
    {
        if (Run_Flag == 1)  // RUN_STOP
        {
            Run_Flag = 0;
            run_keep = 0x09;
        }

        if (run_keep == 0x09) {
            Run = 0;
            force_set(0X00E0, 0);
            force_reset(0X00E0, 1);
            if (edit_prog == 0x00) {
                find_p(), edit_prog = 1;
                if (find_toend()) {
                    p_all_data[0X01C4] = 9;
                    goto all_end;
                }
            }
            if (puls == 0x00)
                force_set(0X00E0, 2), force_reset(0X00E0, 3);
            p_prog           = x + 0x5c / 2 - 1;
            all_data[0x070C] = 20;
            do {
                temp5 = *p_prog / 0x100;
                switch (temp5) {
                    case 0x06: operation_T(); break;
                    case 0x0E: operation_C(); break;

                    case 0x20: LD(0X0140, *p_prog++); break;
                    case 0x30: LDI(0X0140, *p_prog++); break;
                    case 0x40: AND(0X0140, *p_prog++); break;
                    case 0x50: ANI(0X0140, *p_prog++); break;
                    case 0x60: OR(0X0140, *p_prog++); break;
                    case 0x70: ORI(0X0140, *p_prog++); break;

                    case 0x21: LD(0X0150, *p_prog++); break;
                    case 0x31: LDI(0X0150, *p_prog++); break;
                    case 0x41: AND(0X0150, *p_prog++); break;
                    case 0x51: ANI(0X0150, *p_prog++); break;
                    case 0x61: OR(0X0150, *p_prog++); break;
                    case 0x71: ORI(0X0150, *p_prog++); break;

                    case 0x22: LD(0X0160, *p_prog++); break;
                    case 0x32: LDI(0X0160, *p_prog++); break;
                    case 0x42: AND(0X0160, *p_prog++); break;
                    case 0x52: ANI(0X0160, *p_prog++); break;
                    case 0x62: OR(0X0160, *p_prog++); break;
                    case 0x72: ORI(0X0160, *p_prog++); break;

                    case 0x23: LD(0X0170, *p_prog++); break;
                    case 0x33: LDI(0X0170, *p_prog++); break;
                    case 0x43: AND(0X0170, *p_prog++); break;
                    case 0x53: ANI(0X0170, *p_prog++); break;
                    case 0x63: OR(0X0170, *p_prog++); break;
                    case 0x73: ORI(0X0170, *p_prog++); break;

                    case 0x24: LD(0X0120, *p_prog++); break;
                    case 0x34: LDI(0X0120, *p_prog++); break;
                    case 0x44: AND(0X0120, *p_prog++); break;
                    case 0x54: ANI(0X0120, *p_prog++); break;
                    case 0x64: OR(0X0120, *p_prog++); break;
                    case 0x74: ORI(0X0120, *p_prog++); break;

                    case 0x25: LD(0X00C0, *p_prog++); break;
                    case 0x35: LDI(0X00C0, *p_prog++); break;
                    case 0x45: AND(0X00C0, *p_prog++); break;
                    case 0x55: ANI(0X00C0, *p_prog++); break;
                    case 0x65: OR(0X00C0, *p_prog++); break;
                    case 0x75: ORI(0X00C0, *p_prog++); break;
                    case 0XC5: OUT(0X00C0, *p_prog++); break;
                    case 0XD5: BIT_SET(0X00C0, *p_prog++); break;
                    case 0XE5: RST(0X00C0, *p_prog++); break;

                    case 0x26: LD(0X0100, *p_prog++); break;
                    case 0x36: LDI(0X0100, *p_prog++); break;
                    case 0x46: AND(0X0100, *p_prog++); break;
                    case 0x56: ANI(0X0100, *p_prog++); break;
                    case 0x66: OR(0X0100, *p_prog++); break;
                    case 0x76: ORI(0X0100, *p_prog++); break;
                    case 0XC6: OUT(0X0100, *p_prog++); break;

                    case 0x28: LD(0X0000, *p_prog++); break;
                    case 0x38: LDI(0X0000, *p_prog++); break;
                    case 0x48: AND(0X0000, *p_prog++); break;
                    case 0x58: ANI(0X0000, *p_prog++); break;
                    case 0x68: OR(0X0000, *p_prog++); break;
                    case 0x78: ORI(0X0000, *p_prog++); break;
                    case 0XC8: OUT(0X0000, *p_prog++); break;
                    case 0XD8: BIT_SET(0X0000, *p_prog++); break;
                    case 0XE8: RST(0X0000, *p_prog++); break;

                    case 0x29: LD(0X0010, *p_prog++); break;
                    case 0x39: LDI(0X0010, *p_prog++); break;
                    case 0x49: AND(0X0010, *p_prog++); break;
                    case 0x59: ANI(0X0010, *p_prog++); break;
                    case 0x69: OR(0X0010, *p_prog++); break;
                    case 0x79: ORI(0X0010, *p_prog++); break;
                    case 0XC9: OUT(0X0010, *p_prog++); break;
                    case 0XD9: BIT_SET(0X0010, *p_prog++); break;
                    case 0XE9: RST(0X0010, *p_prog++); break;

                    case 0x2A: LD(0X0020, *p_prog++); break;
                    case 0x3A: LDI(0X0020, *p_prog++); break;
                    case 0x4A: AND(0X0020, *p_prog++); break;
                    case 0x5A: ANI(0X0020, *p_prog++); break;
                    case 0x6A: OR(0X0020, *p_prog++); break;
                    case 0x7A: ORI(0X0020, *p_prog++); break;
                    case 0XCA: OUT(0X0020, *p_prog++); break;
                    case 0XDA: BIT_SET(0X0020, *p_prog++); break;
                    case 0XEA: RST(0X0020, *p_prog++); break;

                    case 0x2B: LD(0X0030, *p_prog++); break;
                    case 0x3B: LDI(0X0030, *p_prog++); break;
                    case 0x4B: AND(0X0030, *p_prog++); break;
                    case 0x5B: ANI(0X0030, *p_prog++); break;
                    case 0x6B: OR(0X0030, *p_prog++); break;
                    case 0x7B: ORI(0X0030, *p_prog++); break;
                    case 0XCB: OUT(0X0030, *p_prog++); break;
                    case 0XDB: BIT_SET(0X0030, *p_prog++); break;
                    case 0XEB: RST(0X0030, *p_prog++); break;

                    case 0x2C: LD(0X0040, *p_prog++); break;
                    case 0x3C: LDI(0X0040, *p_prog++); break;
                    case 0x4C: AND(0X0040, *p_prog++); break;
                    case 0x5C: ANI(0X0040, *p_prog++); break;
                    case 0x6C: OR(0X0040, *p_prog++); break;
                    case 0x7C: ORI(0X0040, *p_prog++); break;
                    case 0XCC: OUT(0X0040, *p_prog++); break;
                    case 0XDC: BIT_SET(0X0040, *p_prog++); break;
                    case 0XEC: RST(0X0040, *p_prog++); break;

                    case 0x2D: LD(0X0050, *p_prog++); break;
                    case 0x3D: LDI(0X0050, *p_prog++); break;
                    case 0x4D: AND(0X0050, *p_prog++); break;
                    case 0x5D: ANI(0X0050, *p_prog++); break;
                    case 0x6D: OR(0X0050, *p_prog++); break;
                    case 0x7D: ORI(0X0050, *p_prog++); break;
                    case 0XCD: OUT(0X0050, *p_prog++); break;
                    case 0XDD: BIT_SET(0X0050, *p_prog++); break;
                    case 0XED: RST(0X0050, *p_prog++); break;

                    case 0x2E: LD(0X00F0, *p_prog++); break;
                    case 0x3E: LDI(0X00F0, *p_prog++); break;
                    case 0x4E: AND(0X00F0, *p_prog++); break;
                    case 0x5E: ANI(0X00F0, *p_prog++); break;
                    case 0x6E: OR(0X00F0, *p_prog++); break;
                    case 0x7E: ORI(0X00F0, *p_prog++); break;

                    case 0x2F: LD(0X00E0, *p_prog++); break;
                    case 0x3F: LDI(0X00E0, *p_prog++); break;
                    case 0x4F: AND(0X00E0, *p_prog++); break;
                    case 0x5F: ANI(0X00E0, *p_prog++); break;
                    case 0x6F: OR(0X00E0, *p_prog++); break;
                    case 0x7F: ORI(0X00E0, *p_prog++); break;
                    case 0XCF: OUT(0X00E0, *p_prog++); break;
                    case 0XDF: BIT_SET(0X00E0, *p_prog++); break;
                    case 0XEF: RST(0X00E0, *p_prog++); break;

                    case 0XFF: other_function(*p_prog), p_prog++; break;
                    case 0xB0: p_prog++; break;
                    case 0x00: {
                        if (((*p_prog % 0x100) == 0x1C) || ((*p_prog % 0x100) == 0x0F))
                            goto all_end;
                    }
                    default: extend_function(); break;
                }
            } while (1);
        all_end:
            p_prog = p_prog;
            puls   = 0x01;
            force_reset(0X00E0, 2), force_set(0X00E0, 3);
        } else {
            RST_Y();
            force_reset(0X00E0, 0);
            force_set(0X00E0, 1);
            all_data[0x070C] = 0;
            edit_prog        = 0;
            puls             = 0;
            Run              = 1;
        }
    } else {
        RST_Y();
        force_reset(0X00E0, 0);
        force_set(0X00E0, 1);
        all_data[0x070C] = 0;
        edit_prog        = 0;
        puls             = 0;
        if (Run_Flag == 0)  // ADD
        {
            Run_Flag = 1;
        }
        Run = 1;
    }
    all_data[0X701] = 0X01;
}
