#ifndef __FILE_OP_H
#define __FILE_OP_H
#include "main.h"
// 保存检测记录的csv文件
#define DETECT_CSV        "0:SRED_EHS.csv"
// 保存设置参数
#define SETTING_PARA      "0:Para.ini"
// 保存WIFI帐号
#define WIFI_CONFIG_TXT   "0:SRED_EHS_WIFI_CONFIG.txt"

// 检测数据大小限制
#define DETECT_DATA_LEN   50
// 文件名大小限制
#define FILE_NAME_LEN     20
// 一个文件存储检测记录的条数
#define DETECT_DATA_INDEX 100

// 记录页面读取到的数据
typedef struct Record_Page_Data {
    int     serial_number;   // 序号
    int     year;            // 2019-
    uint8_t month;           // 0-12
    uint8_t day;             // 0-31
    uint8_t hour;            // 0-24
    uint8_t minute;          // 0-59
    int     detect_result;   // 检测结果
    char    Scan_Data[100];  // 扫码编号

    /*当前位于文件的哪一行*/
    __IO int Current_Line;
    /*当前文件编号查询索引*/
    __IO int file_number_index;
    /*当前流水号，每次记录初始化的时候会赋值一次*/
    __IO int Current_Serial_Number;
} RECORD_PARA;

extern RECORD_PARA csv_file_record;

void Mount_Fatfs(void);
void UMount_Fatfs(void);

/*删除记录数据*/
int Detele_Detect_Log(void);
/*记录数据保存*/
int save_record_to_flash(void);
/*计算某个csv文件的总行数*/
int GetDetectTotalLineCount(char* Path);
/*从前往后读取一行数据*/
int ReadlLineData(int File_Count, int line_number);

#endif  //__FILE_OP_H
