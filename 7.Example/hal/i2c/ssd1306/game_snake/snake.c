#include "snake.h"
#include "oled.h"
#include "key.h"
#include "stdio.h"
#include "delay.h"
#include "adc.h"

int Snake[27][15] = {0};  // 蛇能移动的地图
u8  length;               // 蛇的长度
u8  direction;            // 蛇的方向
u8  command_dir;          // 控制方向
u8  Snake_Head_X;
u8  Snake_Head_Y;
u8  Snake_Tail_X;
u8  Snake_Tail_Y;

void GUI()  // 界面初始化
{
    Command();      // 控制
    Move();         // 运动
    GUI_Refresh();  // 界面刷新
    Show();         // 显示长度
    OLED_Refresh_Gram();
}
void Show()  // 显示
{
    OLED_ShowNum(116, 0, length, 2, 12);  // 显示长度
}

//==================================================================
// 函数名: Point
// 作者:   Lv先生
// 日期:   2020/08/23
// 功能:
// 输入参数:
// 返回值:
// 修改记录:
//==================================================================

void GUI_Refresh()  // 界面刷新
{
    int i, j;
    for (i = 0; i < 27; i++)
        for (j = 0; j < 15; j++)
            Paint_Body(i, j, Snake[i][j]);  // 绘制
    Point(Snake_Head_X, Snake_Head_Y, 1);   // 给蛇头填充实心
}

void Command(void)  // 控制方向
{
    u16 shangxia, zuoyou;
    zuoyou   = Get_Adc_Average(ADC_Channel_2, 10);  // 获取摇杆左右方向的电位器电压，模拟值
    shangxia = Get_Adc_Average(ADC_Channel_1, 10);  // 获取摇杆上下方向的电位器电压，模拟值
    // 根据模拟量来判断方向
    if (shangxia > 3500)
        command_dir = DOWN;
    if (shangxia < 1500)
        command_dir = UP;
    if (zuoyou > 3500)
        command_dir = LEFT;
    if (zuoyou < 1500)
        command_dir = RIGHT;
}

//==================================================================
// 函数名: Snake_Init
// 作者:   Lv先生
// 日期:   2020/08/23
// 功能:   初始化蛇的位置以及方向
// 输入参数:
// 返回值:
// 修改记录:
//==================================================================
void Snake_Init()  // 蛇及食物初始化
{
    int i, j;
    for (i = 0; i < 27; i++)
        for (j = 0; j < 15; j++)
            Snake[i][j] = 0;
    length       = 3;
    direction    = RIGHT;  // 向右 2468
    command_dir  = RIGHT;
    Snake[7][7]  = 1;
    Snake[6][7]  = 2;
    Snake[5][7]  = 3;
    Snake_Head_X = 7;
    Snake_Head_Y = 7;
    Snake_Tail_X = 5;
    Snake_Tail_X = 7;
}

//==================================================================
// 函数名: Map
// 作者:   Lv先生
// 日期:   2020/08/23
// 功能:   描绘长方形地图边界
// 输入参数:
// 返回值:
// 修改记录:
//==================================================================
void Map()  // 初始化地图边界
{
    OLED_DrawLine(0, 0, 110, 0);
    OLED_DrawLine(0, 1, 111, 1);
    OLED_DrawLine(0, 62, 110, 62);
    OLED_DrawLine(0, 63, 111, 63);

    OLED_DrawLine(0, 0, 0, 64);
    OLED_DrawLine(1, 0, 1, 64);
    OLED_DrawLine(110, 0, 110, 64);
    OLED_DrawLine(111, 0, 111, 64);
}

//==================================================================
// 函数名: Point
// 作者:   Lv先生
// 日期:   2020/08/23
// 功能:   填充4*4的实心方块
// 输入参数:坐标:x(0~26)  坐标:y(0~14) 模式:t 1填充 0擦除
// 返回值:
// 修改记录:
//==================================================================
void Point(u8 x, u8 y, u8 t)
{
    int i, j;
    if (t == 0)
        t = 0;
    else
        t = 1;
    for (i = 4 * x + 2; i < 4 * x + 6; i++)
        for (j = 4 * y + 2; j < 4 * y + 6; j++)
            OLED_DrawPoint(i, j, t);
}

//==================================================================
// 函数名: Paint_Body
// 作者:   Lv先生
// 日期:   2020/08/23
// 功能:   描绘蛇的身体部分 挖空的4*4方块
// 输入参数:坐标:x(0~26)  坐标:y(0~14) 模式:t 1填充 0擦除
// 返回值:
// 修改记录:
//==================================================================
void Paint_Body(u8 x, u8 y, u8 t)
{
    Point(x, y, t);                           // 绘制4*4实心方块
    OLED_DrawPoint(4 * x + 3, 4 * y + 3, 0);  // 挖空4*4方块
    OLED_DrawPoint(4 * x + 4, 4 * y + 4, 0);
    OLED_DrawPoint(4 * x + 3, 4 * y + 4, 0);
    OLED_DrawPoint(4 * x + 4, 4 * y + 3, 0);
}
//==================================================================
// 函数名: Food
// 作者:   Lv先生
// 日期:   2020/08/23
// 功能:   在空白(0)处绘制食(-1)
// 输入参数:
// 返回值:
// 修改记录:
//==================================================================
void Food()  // 绘制食物
{
    u8 x, y;
    do {
        x = rand() % 27;         // 在0-26生成随机数
        y = rand() % 15;         // 在0-14生成随机数
    } while (Snake[x][y] != 0);  // 直到随机坐标处元素为空白0 避免在蛇上生成
    Snake[x][y] = -1;            // 给满足要求的坐标元素赋值食物-1
}

//==================================================================
// 函数名: New_Head
// 作者:   Lv先生
// 日期:   2020/08/23
// 功能:   在输入方向生成新蛇头
// 输入参数:方向 UP DOWN LEFT RIGHT
// 返回值:
// 修改记录:
//==================================================================
void New_Head(u8 t)
{
    switch (t) {
        case UP:
            if (Snake_Head_Y - 1 < 0)  // 判断是否越过上上边界，如果是则在下边界生成蛇头
            {
                Snake[Snake_Head_X][14] = 1;
                Snake_Head_Y            = 14;
            } else
                Snake[Snake_Head_X][--Snake_Head_Y] = 1;  // 给左边下一个坐标的元素赋值1（蛇头）
            break;
            // 以下参考UP
        case DOWN:
            if (Snake_Head_Y + 1 > 14) {
                Snake[Snake_Head_X][0] = 1;
                Snake_Head_Y           = 0;
            } else
                Snake[Snake_Head_X][++Snake_Head_Y] = 1;
            break;

        case LEFT:
            if (Snake_Head_X - 1 < 0) {
                Snake[26][Snake_Head_Y] = 1;
                Snake_Head_X            = 26;
            }
            Snake[--Snake_Head_X][Snake_Head_Y] = 1;
            ;
            break;

        case RIGHT:
            if (Snake_Head_X + 1 > 26) {
                Snake[0][Snake_Head_Y] = 1;
                Snake_Head_X           = 0;
            } else
                Snake[++Snake_Head_X][Snake_Head_Y] = 1;
            ;
            break;

        default:
            break;
    }
}

//==================================================================
// 函数名: Eat_Food
// 作者:   Lv先生
// 日期:   2020/08/23
// 功能:   处理吃到食物后的事情 移动+食物随机生成
// 输入参数:方向 UP DOWN LEFT RIGHT
// 返回值:
// 修改记录:
//==================================================================

void Eat_Food(u8 t)
{
    switch (t) {
        case UP:
            New_Head(UP);                                    // 生成新头
            Snake[Snake_Tail_X][Snake_Tail_Y] = length + 1;  // 给蛇尾赋值长度+1，之前去掉了蛇尾
            Food();                                          // 随机生成食物
            break;

        case DOWN:
            New_Head(DOWN);
            Snake[Snake_Tail_X][Snake_Tail_Y] = length + 1;
            Food();
            break;

        case LEFT:
            New_Head(LEFT);
            Snake[Snake_Tail_X][Snake_Tail_Y] = length + 1;
            Food();
            break;

        case RIGHT:
            New_Head(RIGHT);
            Snake[Snake_Tail_X][Snake_Tail_Y] = length + 1;
            Food();
            break;

        default:
            break;
    }
}

//==================================================================
// 函数名: Game_Over
// 作者:   Lv先生
// 日期:   2020/08/23
// 功能:   游戏结束后显示GAMEOVER
// 输入参数:
// 返回值:
// 修改记录:
//==================================================================

void Game_Over()
{
    OLED_ShowString(24, 25, "GAME OVER", 16);
    delay_ms(1000);
    Snake_Init();
    Map();  // 初始化地图边界
    Food();
    OLED_Refresh_Gram();
}

//==================================================================
// 函数名: Move
// 作者:   Lv先生
// 日期:   2020/08/23
// 功能:   蛇的移动
// 输入参数:
// 返回值:
// 修改记录:
//==================================================================
void Move()
{
    int i, j;
    for (i = 0; i < 27; i++)
        for (j = 0; j < 15; j++) {
            if (Snake[i][j] >= length)  // 算出蛇尾坐标
            {
                length       = Snake[i][j];
                Snake_Tail_X = i;
                Snake_Tail_Y = j;
            }
            if (Snake[i][j] == 1)  // 算出蛇头坐标
            {
                Snake_Head_X = i;
                Snake_Head_Y = j;
            }
            if (Snake[i][j] > 0)
                Snake[i][j] += 1;  // 二维遍历SNAKE所有元素，大于0的+1
        }
    Snake[Snake_Tail_X][Snake_Tail_Y] = 0;  // 除去蛇尾

    switch (command_dir)  // 对控制方向进行选择处理
    {
        case UP:
            if (Snake[Snake_Head_X][Snake_Head_Y - 1] > 3)         // 咬到蛇身
                Game_Over();                                       // 游戏结束
            else if (Snake[Snake_Head_X][Snake_Head_Y - 1] == -1)  // 吃到食物
                Eat_Food(command_dir);
            else if (direction == DOWN)  // 运动方向是下，但是控制方向是上，违背
                New_Head(DOWN);          // 按照原来的方向移动
            else {                       // 如果不违背则按照控制方向移动，运动方向变更为控制方向
                New_Head(UP);
                direction = UP;
            }
            break;

        case DOWN:
            if (Snake[Snake_Head_X][Snake_Head_Y + 1] > 3)
                Game_Over();
            else if (Snake[Snake_Head_X][Snake_Head_Y + 1] == -1)
                Eat_Food(command_dir);
            else if (direction == UP)
                New_Head(UP);
            else {
                New_Head(DOWN);
                direction = DOWN;
            }
            break;

        case LEFT:
            if (Snake[Snake_Head_X - 1][Snake_Head_Y] > 3)
                Game_Over();
            else if (Snake[Snake_Head_X - 1][Snake_Head_Y] == -1)
                Eat_Food(command_dir);
            else if (direction == RIGHT)
                New_Head(RIGHT);
            else {
                New_Head(LEFT);
                direction = LEFT;
            }
            break;

        case RIGHT:
            if (Snake[Snake_Head_X + 1][Snake_Head_Y] > 3)
                Game_Over();
            else if (Snake[Snake_Head_X + 1][Snake_Head_Y] == -1)
                Eat_Food(command_dir);
            else if (direction == LEFT)
                New_Head(LEFT);
            else {
                New_Head(RIGHT);
                direction = RIGHT;
            }
            break;

        default:
            break;
    }
}
