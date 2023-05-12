#include "VS1053.h"
#include "stm32f4xx.h"
#include "delay.h"

/*
 * us 级别延时
 */
static void DelayUS(vu32 cnt)
{
    delay_us(cnt);
}

static void Delay_ms(uint16_t count)
{
    delay_ms(count);
}
// VS1053默认设置参数
_vs1053_obj vsset =
    {
        200,  // 音量:220
        6,    // 低音上线 60Hz
        15,   // 低音提升 15dB
        10,   // 高音下限 10Khz
        15,   // 高音提升 10.5dB
        0,    // 空间效果
};

/*******************************************************************************
 * Function Name  : SPI_FLASH_SendByte
 * Description    : Sends a byte through the SPI interface and return the byte
 *                  received from the SPI bus.
 * Input          : byte : byte to send.
 * Output         : None
 * Return         : The value of the received byte.
 *******************************************************************************/
unsigned char SPI2_ReadWriteByte(unsigned char writedat)
{
    /* Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(VS_SPI, SPI_I2S_FLAG_TXE) == RESET)
        ;

    /* Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(VS_SPI, writedat);

    /* Wait to receive a byte */
    while (SPI_I2S_GetFlagStatus(VS_SPI, SPI_I2S_FLAG_RXNE) == RESET)
        ;

    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(VS_SPI);
}

////////////////////////////////////////////////////////////////////////////////
// 移植时候的接口
// data:要写入的数据
// 返回值:读到的数据
u8 VS_SPI_ReadWriteByte(u8 data)
{
    return SPI2_ReadWriteByte(data);
}
static void SPI_SetSpeed(u8 SpeedSet)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
    SPI2->CR1 &= 0XFFC7;
    SPI2->CR1 |= SpeedSet;
    SPI_Cmd(SPI2, ENABLE);
}

void VS_SPI_SpeedLow(void)
{
    SPI_SetSpeed(SPI_BaudRatePrescaler_32);  // 设置到低速模式
}

void VS_SPI_SpeedHigh(void)
{
    SPI_SetSpeed(SPI_BaudRatePrescaler_8);  // 设置到高速模式
}
// 初始化VS1053的IO口
void VS_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  // 使能GPIO时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = VS_DREQ;  // DREQ

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;  // 普通输入模式
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;  // 上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(VS_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = VS_RST;         // PE6
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;  // 普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  // 推挽输出
    GPIO_Init(VS_GPIO_PORT, &GPIO_InitStructure);

    /*初始化STM32 SPI2接口*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);  // 使能SPI时钟

    GPIO_InitStructure.GPIO_Pin   = VS_SCLK | VS_MISO | VS_MOSI;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;   // 复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  // 推挽输出
    GPIO_Init(VS_SPIGPIO_PORT, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);  // PB3复用为 SPI1
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);  // PB4复用为 SPI1
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);  // PB5复用为 SPI1

    // SPI口初始化
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE);   // 复位SPI1
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);  // 停止复位SPI1

    GPIO_InitStructure.GPIO_Pin   = VS_XCS | VS_XDCS;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;  // 普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  // 推挽输出
    GPIO_Init(VS_SPIGPIO_PORT, &GPIO_InitStructure);
    //   GPIO_SetBits(WIZ_GPIO_PORT, WIZ_SCS);										 // 先把片选拉高，真正用的时候再拉低

    /* SPI2 配置 */
    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial     = 7;
    SPI_Init(VS_SPI, &SPI_InitStructure);

    /* 使能 SPI2 */
    SPI_Cmd(VS_SPI, ENABLE);

    SPI2_ReadWriteByte(0xff);  // 启动传输
}
////////////////////////////////////////////////////////////////////////////////
// 软复位VS10XX
void VS_Soft_Reset(void)
{
    u8 retry = 0;
    while (VS_DREQ_IN == 0)
        ;                        // 等待软件复位结束
    VS_SPI_ReadWriteByte(0Xff);  // 启动传输
    retry = 0;
    while (VS_RD_Reg(SPI_MODE) != 0x0800)  // 软件复位,新模式
    {
        VS_WR_Cmd(SPI_MODE, 0x0804);  // 软件复位,新模式
        Delay_ms(2);                  // 等待至少1.35ms
        if (retry++ > 100) break;
    }
    while (VS_DREQ_IN == 0)
        ;  // 等待软件复位结束
    retry = 0;
    while (VS_RD_Reg(SPI_CLOCKF) != 0X9800)  // 设置VS1053的时钟,3倍频 ,1.5xADD
    {
        VS_WR_Cmd(SPI_CLOCKF, 0X9800);  // 设置VS1053的时钟,3倍频 ,1.5xADD
        if (retry++ > 100) break;
    }
    Delay_ms(20);
}
// 硬复位MP3
// 返回1:复位失败;0:复位成功
u8 VS_HD_Reset(void)
{
    u8 retry = 0;
    VS_RST_CLR;
    Delay_ms(20);
    VS_XDCS_SET;  // 取消数据传输
    VS_XCS_SET;   // 取消数据传输
    VS_RST_SET;
    while (VS_DREQ_IN == 0 && retry < 200)  // 等待DREQ为高
    {
        retry++;
        DelayUS(50);
    };
    Delay_ms(20);
    if (retry >= 200)
        return 1;
    else
        return 0;
}
// 正弦测试
void VS_Sine_Test(void)
{
    VS_HD_Reset();
    VS_WR_Cmd(0x0b, 0X2020);      // 设置音量
    VS_WR_Cmd(SPI_MODE, 0x0820);  // 进入VS10XX的测试模式
    while (VS_DREQ_IN == 0)
        ;  // 等待DREQ为高
    // printf("mode sin:%x\n",VS_RD_Reg(SPI_MODE));
    // 向VS1053发送正弦测试命令：0x53 0xef 0x6e n 0x00 0x00 0x00 0x00
    // 其中n = 0x24, 设定VS1053所产生的正弦波的频率值，具体计算方法见VS1053的datasheet
    VS_SPI_SpeedLow();  // 低速
    VS_XDCS_CLR;        // 选中数据传输
    VS_SPI_ReadWriteByte(0x53);
    VS_SPI_ReadWriteByte(0xef);
    VS_SPI_ReadWriteByte(0x6e);
    VS_SPI_ReadWriteByte(0x24);
    VS_SPI_ReadWriteByte(0x00);
    VS_SPI_ReadWriteByte(0x00);
    VS_SPI_ReadWriteByte(0x00);
    VS_SPI_ReadWriteByte(0x00);
    Delay_ms(100);
    VS_XDCS_SET;
    // 退出正弦测试
    VS_XDCS_CLR;  // 选中数据传输
    VS_SPI_ReadWriteByte(0x45);
    VS_SPI_ReadWriteByte(0x78);
    VS_SPI_ReadWriteByte(0x69);
    VS_SPI_ReadWriteByte(0x74);
    VS_SPI_ReadWriteByte(0x00);
    VS_SPI_ReadWriteByte(0x00);
    VS_SPI_ReadWriteByte(0x00);
    VS_SPI_ReadWriteByte(0x00);
    Delay_ms(100);
    VS_XDCS_SET;

    // 再次进入正弦测试并设置n值为0x44，即将正弦波的频率设置为另外的值
    VS_XDCS_CLR;  // 选中数据传输
    VS_SPI_ReadWriteByte(0x53);
    VS_SPI_ReadWriteByte(0xef);
    VS_SPI_ReadWriteByte(0x6e);
    VS_SPI_ReadWriteByte(0x44);
    VS_SPI_ReadWriteByte(0x00);
    VS_SPI_ReadWriteByte(0x00);
    VS_SPI_ReadWriteByte(0x00);
    VS_SPI_ReadWriteByte(0x00);
    Delay_ms(100);
    VS_XDCS_SET;
    // 退出正弦测试
    VS_XDCS_CLR;  // 选中数据传输
    VS_SPI_ReadWriteByte(0x45);
    VS_SPI_ReadWriteByte(0x78);
    VS_SPI_ReadWriteByte(0x69);
    VS_SPI_ReadWriteByte(0x74);
    VS_SPI_ReadWriteByte(0x00);
    VS_SPI_ReadWriteByte(0x00);
    VS_SPI_ReadWriteByte(0x00);
    VS_SPI_ReadWriteByte(0x00);
    Delay_ms(100);
    VS_XDCS_SET;
}
// ram 测试
// 返回值:RAM测试结果
//  VS1053如果得到的值为0x83FF，则表明完好;
u16 VS_Ram_Test(void)
{
    VS_HD_Reset();
    VS_WR_Cmd(SPI_MODE, 0x0820);  // 进入VS10XX的测试模式
    while (VS_DREQ_IN == 0)
        ;               // 等待DREQ为高
    VS_SPI_SpeedLow();  // 低速
    VS_XDCS_CLR;        // xDCS = 1，选择VS10XX的数据接口
    VS_SPI_ReadWriteByte(0x4d);
    VS_SPI_ReadWriteByte(0xea);
    VS_SPI_ReadWriteByte(0x6d);
    VS_SPI_ReadWriteByte(0x54);
    VS_SPI_ReadWriteByte(0x00);
    VS_SPI_ReadWriteByte(0x00);
    VS_SPI_ReadWriteByte(0x00);
    VS_SPI_ReadWriteByte(0x00);
    Delay_ms(150);
    VS_XDCS_SET;
    return VS_RD_Reg(SPI_HDAT0);  // VS1053如果得到的值为0x83FF，则表明完好;
}
// 向VS1053写命令
// address:命令地址
// data:命令数据
void VS_WR_Cmd(u8 address, u16 data)
{
    while (VS_DREQ_IN == 0)
        ;               // 等待空闲
    VS_SPI_SpeedLow();  // 低速
    VS_XDCS_SET;
    VS_XCS_CLR;
    VS_SPI_ReadWriteByte(VS_WRITE_COMMAND);  // 发送VS10XX的写命令
    VS_SPI_ReadWriteByte(address);           // 地址
    VS_SPI_ReadWriteByte(data >> 8);         // 发送高八位
    VS_SPI_ReadWriteByte(data);              // 第八位
    VS_XCS_SET;
    VS_SPI_SpeedHigh();  // 高速
}
// 向VS10XX写数据
// data:要写入的数据
void VS_WR_Data(u8 data)
{
    VS_SPI_SpeedHigh();  // 高速,对VS1003B,最大值不能超过36.864/4Mhz，这里设置为9M
    VS_XDCS_CLR;
    VS_SPI_ReadWriteByte(data);
    VS_XDCS_SET;
}
// 读VS10XX的寄存器
// address：寄存器地址
// 返回值：读到的值
// 注意不要用倍速读取,会出错
u16 VS_RD_Reg(u8 address)
{
    u16 temp = 0;
    while (VS_DREQ_IN == 0)
        ;               // 非等待空闲状态
    VS_SPI_SpeedLow();  // 低速
    VS_XDCS_SET;
    VS_XCS_CLR;
    VS_SPI_ReadWriteByte(VS_READ_COMMAND);  // 发送VS10XX的读命令
    VS_SPI_ReadWriteByte(address);          // 地址
    temp = VS_SPI_ReadWriteByte(0xff);      // 读取高字节
    temp = temp << 8;
    temp += VS_SPI_ReadWriteByte(0xff);  // 读取低字节
    VS_XCS_SET;
    VS_SPI_SpeedHigh();  // 高速
    return temp;
}
// 读取VS10xx的RAM
// addr：RAM地址
// 返回值：读到的值
u16 VS_WRAM_Read(u16 addr)
{
    u16 res;
    VS_WR_Cmd(SPI_WRAMADDR, addr);
    res = VS_RD_Reg(SPI_WRAM);
    return res;
}
// 写VS10xx的RAM
// addr：RAM地址
// val:要写入的值
void VS_WRAM_Write(u16 addr, u16 val)
{
    VS_WR_Cmd(SPI_WRAMADDR, addr);  // 写RAM地址
    while (VS_DREQ_IN == 0)
        ;                      // 等待空闲
    VS_WR_Cmd(SPI_WRAM, val);  // 写RAM值
}
// 设置播放速度（仅VS1053有效）
// t:0,1,正常速度;2,2倍速度;3,3倍速度;4,4倍速;以此类推
void VS_Set_Speed(u8 t)
{
    VS_WRAM_Write(0X1E04, t);  // 写入播放速度
}
// FOR WAV HEAD0 :0X7761 HEAD1:0X7665
// FOR MIDI HEAD0 :other info HEAD1:0X4D54
// FOR WMA HEAD0 :data speed HEAD1:0X574D
// FOR MP3 HEAD0 :data speed HEAD1:ID
// 比特率预定值,阶层III
const u16 bitrate[2][16] =
    {
        {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0},
        {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 0}};
// 返回Kbps的大小
// 返回值：得到的码率
u16 VS_Get_HeadInfo(void)
{
    unsigned int HEAD0;
    unsigned int HEAD1;
    HEAD0 = VS_RD_Reg(SPI_HDAT0);
    HEAD1 = VS_RD_Reg(SPI_HDAT1);
    // printf("(H0,H1):%x,%x\n",HEAD0,HEAD1);
    switch (HEAD1) {
        case 0x7665:  // WAV格式
        case 0X4D54:  // MIDI格式
        case 0X4154:  // AAC_ADTS
        case 0X4144:  // AAC_ADIF
        case 0X4D34:  // AAC_MP4/M4A
        case 0X4F67:  // OGG
        case 0X574D:  // WMA格式
        case 0X664C:  // FLAC格式
        {
            ////printf("HEAD0:%d\n",HEAD0);
            HEAD1 = HEAD0 * 2 / 25;  // 相当于*8/100
            if ((HEAD1 % 10) > 5)
                return HEAD1 / 10 + 1;  // 对小数点第一位四舍五入
            else
                return HEAD1 / 10;
        }
        default:  // MP3格式,仅做了阶层III的表
        {
            HEAD1 >>= 3;
            HEAD1 = HEAD1 & 0x03;
            if (HEAD1 == 3)
                HEAD1 = 1;
            else
                HEAD1 = 0;
            return bitrate[HEAD1][HEAD0 >> 12];
        }
    }
}
// 得到平均字节数
// 返回值：平均字节数速度
u32 VS_Get_ByteRate(void)
{
    return VS_WRAM_Read(0X1E05);  // 平均位速
}
// 得到需要填充的数字
// 返回值:需要填充的数字
u16 VS_Get_EndFillByte(void)
{
    return VS_WRAM_Read(0X1E06);  // 填充字节
}
// 发送一次音频数据
// 固定为32字节
// 返回值:0,发送成功
//		 1,VS10xx不缺数据,本次数据未成功发送
u8 VS_Send_MusicData(u8* buf)
{
    u8 n;
    if (VS_DREQ_IN != 0)  // 送数据给VS10XX
    {
        VS_XDCS_CLR;
        for (n = 0; n < 32; n++) {
            VS_SPI_ReadWriteByte(buf[n]);
        }
        VS_XDCS_SET;
    } else
        return 1;
    return 0;  // 成功发送了
}
// 切歌
// 通过此函数切歌，不会出现切换“噪声”
void VS_Restart_Play(void)
{
    u16 temp;
    u16 i;
    u8  n;
    u8  vsbuf[32];
    for (n = 0; n < 32; n++) vsbuf[n] = 0;  // 清零
    temp = VS_RD_Reg(SPI_MODE);             // 读取SPI_MODE的内容
    temp |= 1 << 3;                         // 设置SM_CANCEL位
    temp |= 1 << 2;                         // 设置SM_LAYER12位,允许播放MP1,MP2
    VS_WR_Cmd(SPI_MODE, temp);              // 设置取消当前解码指令
    for (i = 0; i < 2048;)                  // 发送2048个0,期间读取SM_CANCEL位.如果为0,则表示已经取消了当前解码
    {
        if (VS_Send_MusicData(vsbuf) == 0)  // 每发送32个字节后检测一次
        {
            i += 32;                            // 发送了32个字节
            temp = VS_RD_Reg(SPI_MODE);         // 读取SPI_MODE的内容
            if ((temp & (1 << 3)) == 0) break;  // 成功取消了
        }
    }
    if (i < 2048)  // SM_CANCEL正常
    {
        temp = VS_Get_EndFillByte() & 0xff;        // 读取填充字节
        for (n = 0; n < 32; n++) vsbuf[n] = temp;  // 填充字节放入数组
        for (i = 0; i < 2052;) {
            if (VS_Send_MusicData(vsbuf) == 0) i += 32;  // 填充
        }
    } else
        VS_Soft_Reset();  // SM_CANCEL不成功,坏情况,需要软复位
    temp = VS_RD_Reg(SPI_HDAT0);
    temp += VS_RD_Reg(SPI_HDAT1);
    if (temp)  // 软复位,还是没有成功取消,放杀手锏,硬复位
    {
        VS_HD_Reset();    // 硬复位
        VS_Soft_Reset();  // 软复位
    }
}
// 重设解码时间
void VS_Reset_DecodeTime(void)
{
    VS_WR_Cmd(SPI_DECODE_TIME, 0x0000);
    VS_WR_Cmd(SPI_DECODE_TIME, 0x0000);  // 操作两次
}
// 得到mp3的播放时间n sec
// 返回值：解码时长
u16 VS_Get_DecodeTime(void)
{
    u16 dt = 0;
    dt     = VS_RD_Reg(SPI_DECODE_TIME);
    return dt;
}
// vs10xx装载patch.
// patch：patch首地址
// len：patch长度
void VS_Load_Patch(u16* patch, u16 len)
{
    u16 i;
    u16 addr, n, val;
    for (i = 0; i < len;) {
        addr = patch[i++];
        n    = patch[i++];
        if (n & 0x8000U)  // RLE run, replicate n samples
        {
            n &= 0x7FFF;
            val = patch[i++];
            while (n--) VS_WR_Cmd(addr, val);
        } else  // copy run, copy n sample
        {
            while (n--) {
                val = patch[i++];
                VS_WR_Cmd(addr, val);
            }
        }
    }
}
// 设定VS10XX播放的音量和高低音
// volx:音量大小(0~254)
void VS_Set_Vol(u8 volx)
{
    u16 volt = 0;           // 暂存音量值
    volt     = 254 - volx;  // 取反一下,得到最大值,表示最大的表示
    volt <<= 8;
    volt += 254 - volx;        // 得到音量设置后大小
    VS_WR_Cmd(SPI_VOL, volt);  // 设音量
}
// 设定高低音控制
// bfreq:低频上限频率	2~15(单位:10Hz)
// bass:低频增益			0~15(单位:1dB)
// tfreq:高频下限频率 	1~15(单位:Khz)
// treble:高频增益  	 	0~15(单位:1.5dB,小于9的时候为负数)
void VS_Set_Bass(u8 bfreq, u8 bass, u8 tfreq, u8 treble)
{
    u16         bass_set = 0;  // 暂存音调寄存器值
    signed char temp     = 0;
    if (treble == 0)
        temp = 0;  // 变换
    else if (treble > 8)
        temp = treble - 8;
    else
        temp = treble - 9;
    bass_set = temp & 0X0F;  // 高音设定
    bass_set <<= 4;
    bass_set += tfreq & 0xf;  // 高音下限频率
    bass_set <<= 4;
    bass_set += bass & 0xf;  // 低音设定
    bass_set <<= 4;
    bass_set += bfreq & 0xf;        // 低音上限
    VS_WR_Cmd(SPI_BASS, bass_set);  // BASS
}
// 设定音效
// eft:0,关闭;1,最小;2,中等;3,最大.
void VS_Set_Effect(u8 eft)
{
    u16 temp;
    temp = VS_RD_Reg(SPI_MODE);  // 读取SPI_MODE的内容
    if (eft & 0X01)
        temp |= 1 << 4;  // 设定LO
    else
        temp &= ~(1 << 5);  // 取消LO
    if (eft & 0X02)
        temp |= 1 << 7;  // 设定HO
    else
        temp &= ~(1 << 7);      // 取消HO
    VS_WR_Cmd(SPI_MODE, temp);  // 设定模式
}

///////////////////////////////////////////////////////////////////////////////
// 设置音量,音效等.
void VS_Set_All(void)
{
    VS_Set_Vol(vsset.mvol);  // 设置音量
    VS_Set_Bass(vsset.bflimit, vsset.bass, vsset.tflimit, vsset.treble);
    VS_Set_Effect(vsset.effect);  // 设置空间效果
}
