#ifndef _SNAKE_H_
#define _SNAKE_H_
#include "sys.h"
#include "stdbool.h"

#define UP    2
#define DOWN  8
#define LEFT  4
#define RIGHT 6

extern u8 length;
extern u8 direction;    // 蛇的方向
extern u8 command_dir;  // 指挥方向

void GUI_Refresh(void);  // 界面刷新
void GUI(void);          // 界面初始化
void Map(void);          // 初始化地图边界
void Point(u8 x, u8 y, u8 t);
void Paint_Body(u8 x, u8 y, u8 t);  // 绘制身体
void Move(void);                    // 移动
void Snake_Init(void);              // 蛇及食物初始化
void New_Head(u8 t);                // 新头部
void Command(void);                 // 控制方向
void GameOver(void);                // 游戏结束
void Food(void);                    // 生成食物
void Eat_Food(u8 t);                // 吃食物
void Show(void);                    // 显示

#endif
