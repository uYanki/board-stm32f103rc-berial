#include "board.h"

#include "math.h"
#include "i2cdev/i2cdev.h"

#define TAG     "main"
#define DBG_LVL SDK_DEBUG

#define M_PI    3.141592

// func

void ssd1306_init(i2c_cli_t cli);
void ssd1306_update(i2c_cli_t cli);
void ssd1306_clear(void);

void cube_render(void);
void cube_rotate(double thetaX, double thetaY, double thetaZ);

// var

struct i2c_cli ssd1306_dev1;
struct i2c_cli ssd1306_dev2;

int main(void)
{
    uint16_t i;

    struct hard_i2c hwi2c_bus1;
    struct hard_i2c hwi2c_bus2;

    hwi2c_bus1.I2C     = I2C1;
    hwi2c_bus1.RemapIO = true;
    hwi2c_bus1.Speed   = 400000;
    hwi2c_bus1.Timeout = 0xFF;
    hwi2c_bus1.OwnAddr = 0;

    ssd1306_dev1.bus   = &hwi2c_bus1;
    ssd1306_dev1.drv   = &hwi2c_drv;
    ssd1306_dev1.dev   = 0x3C << 1;
    ssd1306_dev1.ops   = I2C_DEV_7BIT | I2C_REG_8BIT;

    hwi2c_bus2.I2C     = I2C2;
    hwi2c_bus2.RemapIO = false;
    hwi2c_bus2.Speed   = 400000;
    hwi2c_bus2.Timeout = 0xFFF;
    hwi2c_bus2.OwnAddr = 0;

    ssd1306_dev2.bus   = &hwi2c_bus2;
    ssd1306_dev2.drv   = &hwi2c_drv;
    ssd1306_dev2.dev   = 0x3C << 1;
    ssd1306_dev2.ops   = I2C_DEV_7BIT | I2C_REG_8BIT;

    board_init();

    hwi2c_drv.init(&hwi2c_bus1);
    i2cdrv_detector(&hwi2c_bus1, &hwi2c_drv);

    hwi2c_drv.init(&hwi2c_bus2);
    i2cdrv_detector(&hwi2c_bus2, &hwi2c_drv);

    delay_ms(1000);

    ssd1306_init(&ssd1306_dev1);
    ssd1306_init(&ssd1306_dev2);

    while (1) {
        for (i = 0; i < 360; i++) {
            cube_rotate(M_PI / 180 * i, M_PI / 90 * i, 0);
            cube_render();
            ssd1306_update(&ssd1306_dev1);
            ssd1306_update(&ssd1306_dev2);
            ssd1306_clear();
        }
        delay_ms(1);
    }

}

////////////

uint8_t framebuf[8][128] = {0xFF};

INLINE int abs(int v) { return v >= 0 ? v : -v; }

void ssd1306_init(const i2c_cli_t cli)
{
    i2cdev_write_byte(cli, 0x00, 0xAE);  // 关闭显示

    i2cdev_write_byte(cli, 0x00, 0xD5);  // 设置显示时钟分频比/振荡器频率
    i2cdev_write_byte(cli, 0x00, 0x80);

    i2cdev_write_byte(cli, 0x00, 0xA8);  // 设置多路复用率
    i2cdev_write_byte(cli, 0x00, 0x3F);

    i2cdev_write_byte(cli, 0x00, 0xD3);  // 设置显示偏移
    i2cdev_write_byte(cli, 0x00, 0x00);

    i2cdev_write_byte(cli, 0x00, 0x40);  // 设置显示开始行

    i2cdev_write_byte(cli, 0x00, 0xA1);  // 设置左右方向，0xA1正常 0xA0左右反置

    i2cdev_write_byte(cli, 0x00, 0xC8);  // 设置上下方向，0xC8正常 0xC0上下反置

    i2cdev_write_byte(cli, 0x00, 0xDA);  // 设置COM引脚硬件配置
    i2cdev_write_byte(cli, 0x00, 0x12);

    i2cdev_write_byte(cli, 0x00, 0x81);  // 设置对比度控制
    i2cdev_write_byte(cli, 0x00, 0xCF);

    i2cdev_write_byte(cli, 0x00, 0xD9);  // 设置预充电周期
    i2cdev_write_byte(cli, 0x00, 0xF1);

    i2cdev_write_byte(cli, 0x00, 0xDB);  // 设置VCOMH取消选择级别
    i2cdev_write_byte(cli, 0x00, 0x30);

    i2cdev_write_byte(cli, 0x00, 0xA4);  // 设置整个显示打开/关闭

    i2cdev_write_byte(cli, 0x00, 0xA6);  // 设置正常/倒转显示

    i2cdev_write_byte(cli, 0x00, 0x8D);  // 设置充电泵
    i2cdev_write_byte(cli, 0x00, 0x14);

    i2cdev_write_byte(cli, 0x00, 0x20);  // 设置横向地址模式
    i2cdev_write_byte(cli, 0x00, 0x00);

    i2cdev_write_byte(cli, 0x00, 0x21);
    i2cdev_write_byte(cli, 0x00, 0x00);
    i2cdev_write_byte(cli, 0x00, 127);

    i2cdev_write_byte(cli, 0x00, 0x22);
    i2cdev_write_byte(cli, 0x00, 0);
    i2cdev_write_byte(cli, 0x00, 7);

    i2cdev_write_byte(cli, 0x00, 0xAF);  // 开启显示
}

void ssd1306_update(i2c_cli_t cli)
{
    // set cursor
    i2cdev_write_byte(cli, 0x00, 0xb0);
    i2cdev_write_byte(cli, 0x00, 0x10);
    i2cdev_write_byte(cli, 0x00, 0x00);
    // send data
    i2cdev_write_block(cli, 0x40, (uint8_t*)framebuf, 1024);
}

void ssd1306_clear(void)
{
    uint16_t i;

    for (i = 0; i < 1024; ++i)
        ((uint8_t*)framebuf)[i] = 0;
}

////////////

int8_t cube[8][3] = {
    { 17,  17,  17},
    {-17,  17,  17},
    {-17, -17,  17},
    { 17, -17,  17}, // A B C D
    { 17,  17, -17},
    {-17,  17, -17},
    {-17, -17, -17},
    { 17, -17, -17}
};  // E F G H

double cube_display[8][3] = {
    { 17,  17,  17},
    {-17,  17,  17},
    {-17, -17,  17},
    { 17, -17,  17}, // A B C D
    { 17,  17, -17},
    {-17,  17, -17},
    {-17, -17, -17},
    { 17, -17, -17}
};

INLINE uint8_t cor_trans_x(double x) { return x + 64; }

INLINE uint8_t cor_trans_y(double y) { return y + 32; }

void draw_pixel(uint8_t x, uint8_t y)
{
    y -= 1;
    framebuf[7 - (y >> 3)][x - 1] |= 0x80 >> (y % 8);
}

void draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    int8_t  dx  = x1 - x0;
    int8_t  dy  = y1 - y0;
    int8_t  DIR = 1;
    int16_t d;
    if (abs(dx) >= abs(dy)) {
        uint8_t x_start, x_end, yi;
        d = -dx;
        if (dx >= 0) {
            x_start = x0;
            x_end   = x1;
            yi      = y0;
            if (dy < 0)
                DIR = -1;
            else if (dy == 0)
                DIR = 0;
        } else {
            x_start = x1;
            x_end   = x0;
            yi      = y1;
            d       = -d;
            if (dy > 0)
                DIR = -1;
        }
        for (; x_start <= x_end; x_start++) {
            if (d > 0) {
                yi = yi + DIR;
                d  = d - 2 * abs(dx);
            }
            draw_pixel((uint8_t)x_start, (uint8_t)yi);
            d = d + 2 * abs(dy);
        }
    } else {
        uint8_t y_start, y_end, xi;
        d = -dy;
        if (dy >= 0) {
            y_start = y0;
            y_end   = y1;
            xi      = x0;
            if (dx < 0)
                DIR = -1;
        } else {
            y_start = y1;
            y_end   = y0;
            xi      = x1;
            d       = -d;
            if (dx > 0)
                DIR = -1;
            else if (dx == 0)
                DIR = 0;
        }
        for (; y_start <= y_end; y_start++) {
            if (d > 0) {
                xi = xi + DIR;
                d  = d - 2 * abs(dy);
            }
            draw_pixel((uint8_t)xi, (uint8_t)y_start);
            d = d + 2 * abs(dx);
        }
    }
}

void cube_render(void)
{
    draw_line(cor_trans_x(cube_display[0][0]), cor_trans_y(cube_display[0][1]),  // AB
              cor_trans_x(cube_display[1][0]), cor_trans_y(cube_display[1][1]));

    draw_line(cor_trans_x(cube_display[1][0]), cor_trans_y(cube_display[1][1]),  // BC
              cor_trans_x(cube_display[2][0]), cor_trans_y(cube_display[2][1]));

    draw_line(cor_trans_x(cube_display[2][0]), cor_trans_y(cube_display[2][1]),  // CD
              cor_trans_x(cube_display[3][0]), cor_trans_y(cube_display[3][1]));

    draw_line(cor_trans_x(cube_display[3][0]), cor_trans_y(cube_display[3][1]),  // DA
              cor_trans_x(cube_display[0][0]), cor_trans_y(cube_display[0][1]));

    draw_line(cor_trans_x(cube_display[4][0]), cor_trans_y(cube_display[4][1]),  // EF
              cor_trans_x(cube_display[5][0]), cor_trans_y(cube_display[5][1]));

    draw_line(cor_trans_x(cube_display[5][0]), cor_trans_y(cube_display[5][1]),  // FG
              cor_trans_x(cube_display[6][0]), cor_trans_y(cube_display[6][1]));

    draw_line(cor_trans_x(cube_display[6][0]), cor_trans_y(cube_display[6][1]),  // GH
              cor_trans_x(cube_display[7][0]), cor_trans_y(cube_display[7][1]));

    draw_line(cor_trans_x(cube_display[7][0]), cor_trans_y(cube_display[7][1]),  // HE
              cor_trans_x(cube_display[4][0]), cor_trans_y(cube_display[4][1]));

    draw_line(cor_trans_x(cube_display[0][0]), cor_trans_y(cube_display[0][1]),  // AE
              cor_trans_x(cube_display[4][0]), cor_trans_y(cube_display[4][1]));

    draw_line(cor_trans_x(cube_display[1][0]), cor_trans_y(cube_display[1][1]),  // BF
              cor_trans_x(cube_display[5][0]), cor_trans_y(cube_display[5][1]));

    draw_line(cor_trans_x(cube_display[2][0]), cor_trans_y(cube_display[2][1]),  // CG
              cor_trans_x(cube_display[6][0]), cor_trans_y(cube_display[6][1]));

    draw_line(cor_trans_x(cube_display[3][0]), cor_trans_y(cube_display[3][1]),  // DH
              cor_trans_x(cube_display[7][0]), cor_trans_y(cube_display[7][1]));
}

void cube_rotate(double thetaX, double thetaY, double thetaZ)
{
    uint8_t i;

    for (i = 0; i < 8; i++) {
        cube_display[i][0] = cube[i][0];
        cube_display[i][1] = cube[i][1] * cos(thetaX) - cube[i][2] * sin(thetaX);
        cube_display[i][2] = cube[i][1] * sin(thetaX) + cube[i][2] * cos(thetaX);

        cube_display[i][0] = cube_display[i][0] * cos(thetaY) - cube_display[i][2] * sin(thetaY);
        cube_display[i][1] = cube_display[i][1];
        cube_display[i][2] = cube_display[i][0] * sin(thetaY) + cube_display[i][2] * cos(thetaY);

        cube_display[i][0] = cube_display[i][0] * cos(thetaZ) - cube_display[i][1] * sin(thetaZ);
        cube_display[i][1] = cube_display[i][0] * sin(thetaZ) + cube_display[i][1] * cos(thetaZ);
        cube_display[i][2] = cube_display[i][2];
    }
}
