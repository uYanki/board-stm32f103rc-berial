#include "board.h"

// 输入
#define X00      PCin(6)
#define X01      PCin(7)
#define X02      PCin(8)
#define X03      PCin(9)
#define X04      PAin(8)
#define X05      PAin(11)
#define X06      PAin(12)
#define X07      PCin(10)

#define X10      PCin(11)
#define X11      PCin(12)
#define X12      PDin(2)
#define X13      PBin(5)

#define X14      PBin(6)
#define X15      PBin(7)
#define X16      PBin(8)
#define X17      PBin(9)

#define RUN_STOP PCin(13)
#define PVD      PCin(0)

// 输出
#define Y00      PBout(12)
#define Y01      PBout(13)
#define Y02      PBout(14)
#define Y03      PBout(15)
#define Y04      PBout(1)
#define Y05      PBout(0)
#define Y06      PCout(5)
#define Y07      PCout(4)

#define Y10      PAout(7)
#define Y11      PAout(6)
#define Y12      PAout(5)
#define Y13      PAout(4)
#define Y14      PAout(3)
#define Y15      PAout(2)
#define Y16      PAout(1)
#define Y17      PAout(0)

#define Run      PBout(3)
#define Com      PBout(4)
#define Err      PAout(15)
