#include "board.h"

#define TAG       "chip_id"
#define DBG_LVL   SDK_DEBUG

#define VERI_ADDR 0x0801F000  // 校验地址
#define VERI_CODE 0x28522072  // 校验字

static uint32_t ID[3];

void     get_unique_id(void);
bool     get_activation_status(void);
uint64_t generate_serial_number(char* pwd);
bool     activate_device(void);

int main(void)
{
    uint64_t verify_lic, user_lic;

    board_init();

    // 获取设备标识
    get_unique_id();

    // 生成激活序列号
    verify_lic = generate_serial_number("VJH}JH-H{9912-PKPWE}R-I@$I("); // 串口发送该段注册码即可

    // 获取激活状态
    if (!get_activation_status()) {
        println("please activate first");
        while (1) {
            if (uart2_rxflg) {
                uart2_rxflg = 0;  // clear flag
                user_lic    = generate_serial_number((char*)uart2_rxbuf);
                if (user_lic == verify_lic) {
                    if(activate_device()) break;
                } else {
                    println("illegal password: %s", uart2_rxbuf);
                }
            }
        }
    }

    // 设备已激活
    println("device activated");

    while (1) {
        LED = 0;
        delay_ms(1000);
        LED = 1;
        delay_ms(1000);
    }

}

void get_unique_id(void)
{
    ID[0] = *(__IO u32*)(0x1ffff7e8);
    ID[1] = *(__IO u32*)(0x1ffff7ec);
    ID[2] = *(__IO u32*)(0x1ffff7f0);

    LOGD("ID: % X - % X - % X", ID[0], ID[1], ID[2]);
}

bool get_activation_status(void)
{
    return VERI_CODE == *(vu32*)(VERI_ADDR);
}

bool activate_device(void)
{
    uint32_t veri_code;

    veri_code = *(vu32*)(VERI_ADDR);

    if (VERI_CODE == veri_code)
        return true;

    FLASH_Unlock();

    // 非空则需在页擦除后才能写
    if (0xffffffff != veri_code) {
        FLASH_ErasePage(VERI_ADDR);
    }

    FLASH_ProgramWord(VERI_ADDR, VERI_CODE);

    FLASH_Lock();

    return VERI_CODE == *(vu32*)(VERI_ADDR);
}

uint64_t generate_serial_number(char* pwd)  // 算法自定义
{
    char ch;

    uint64_t ser = 0;  // serial number

    uint64_t mask[3];

    mask[0] = ID[0] & 0xFF00FF00;
    mask[1] = ID[1] & 0x00FF00FF;
    mask[2] = ID[2] & 0xF0F0F0F0;

    // 需勾选 GUNC 拓展（AC5 -> C/C++ -> GUNC extensions √）
    // 因为标准 C 中的 case 后是不能跟范围

    while ((ch = *pwd++) != '\0') {
        switch (ch) {
            case '0' ... '9':
                ser += (ch * mask[0]) << 10;
                break;
            case 'a' ... 'z':
            case 'A' ... 'Z':
                ser += (ch * mask[1]) << 20;
                break;
            default:
                ser += (ch * mask[2]) << 30;
                break;
        }
    }

    LOGD("ser: %llu", ser);

    return ser;
}
