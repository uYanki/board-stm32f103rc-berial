#include "file_op.h"
#include "fatfs.h"

uint8_t     work[_MAX_SS];
RECORD_PARA csv_file_record;

// 挂载文件系统
void Mount_Fatfs(void)
{
    retUSER = f_mount(&USERFatFS, USERPath, 1);

    if (retUSER != FR_OK) {
        if (retUSER == FR_NO_FILESYSTEM) {
            printf("f_mount 没有文件系统,开始格式化spi-flash\r\n");
            retUSER = f_mkfs(USERPath, FM_ANY, 0, work, sizeof(work));

            if (retUSER != FR_OK) {
                printf("f_mkfs 格式化失败，err = %d\r\n", retUSER);
            } else {
                printf("格式化成功，开始重新挂载spi-flash\r\n");
                retUSER = f_mount(&USERFatFS, USERPath, 1);

                if (retUSER != FR_OK) {
                    printf("f_mount 发生错误，err = %d\r\n", retUSER);
                } else {
                    printf("spi-flash文件系统挂载成功\r\n");
                }
            }
        } else {
            printf("f_mount 发生其他错误，err = %d\r\n", retUSER);
        }
    } else
        printf("spi-flash文件系统挂载成功\r\n");
}

/*解除文件系统挂载*/
void UMount_Fatfs(void)
{
    printf("不再使用文件系统，取消挂载文件系统\r\n");
    retUSER = f_mount(NULL, USERPath, 1);

    if (retUSER == FR_OK)
        printf("取消挂载文件系统成功\r\n");
    else
        printf("取消挂载文件系统失败，err = %d\r\n", retUSER);
}

/*删除检测记录*/
int Detele_Detect_Log(void)
{
    // 删除文件
    retUSER = f_unlink(DETECT_CSV);

    if (FR_OK != retUSER) {
        // 删除文件失败
        printf("删除文件失败! %d\n", retUSER);
        return -2;
    }

    return 0;
}

/*保存检测数据到csv文件中*/
int save_record_to_flash(void)
{
    uint32_t count                      = 0;
    char     filename[FILE_NAME_LEN]    = {0};
    char     Detect_Data[100]           = {0};
    /*计算当前流水号一个可以产生多少个文件，一个文件超出100个记录项则自动创建新的文件*/
    int      data_serial_num_file_count = User_Memory_Para.Detect_Log_Serial_Number / DETECT_DATA_INDEX;
    sprintf(filename, "0:SRED_EHS%d.csv", data_serial_num_file_count);
    retUSER = f_open(&USERFile, filename, FA_OPEN_ALWAYS | FA_WRITE);

    if (FR_OK != retUSER) {
        printf("打开/创建%s文件失败，err = %d\r\n", filename, retUSER);
        return -1;
    }

    printf("打开/创建%s文件成功，向文件写入数据。\r\n", filename);
    /*每一次都偏移到文件的末尾*/
    f_lseek(&USERFile, f_size(&USERFile));
    /*流水号自增*/
    ++User_Memory_Para.Detect_Log_Serial_Number;
    /*记录流水号保存处理*/
    User_Detect_Log_Save_Process();

    /*如果扫码OK，则记录检测数据以及扫码得到的数据*/
    if (1 == Scan_ok_flag) {
        sprintf(Detect_Data, "%d,%2d/%02d/%02d %02d:%02d,%d,%s\r\n",
                User_Memory_Para.Detect_Log_Serial_Number, current_time.year, current_time.month,
                current_time.day, current_time.hour, current_time.minute, sensor_data.Is_Drug_or_Explosive, sensor_data.Scan_Data);
        memset((char*)sensor_data.Scan_Data, 0, 50);
    } else {
        sprintf(Detect_Data, "%d,%2d/%02d/%02d %02d:%02d,%d\r\n",
                User_Memory_Para.Detect_Log_Serial_Number, current_time.year, current_time.month,
                current_time.day, current_time.hour, current_time.minute, sensor_data.Is_Drug_or_Explosive);
    }

    retUSER = f_write(&USERFile, Detect_Data, strlen((char*)Detect_Data), &count);

    if (retUSER != FR_OK) {
        printf("f_write 发生错误，err = %d\r\n", retUSER);
        printf("关闭打开的SRED_EHS.csv文件\r\n");
        count = 0;
        memset(Detect_Data, 0, DETECT_DATA_LEN);
        memset(Scaner.ScanerU5Buffer, 0, USART_REC_LEN);
        f_close(&USERFile);
        return -2;
    }

    printf("文件写入成功，写入字节数据：%d\n", count);
    printf("向文件写入的数据为：\r\n%s\r\n", Detect_Data);
    printf("关闭打开的SRED_EHS.csv文件\r\n");
    count = 0;
    memset(Detect_Data, 0, DETECT_DATA_LEN);
    memset(Scaner.ScanerU5Buffer, 0, USART_REC_LEN);
    f_close(&USERFile);
    return 0;
}

int flseek(FIL* fp, long offset, int whence)
{
    int error = 0;
    int value = 0;

    switch (whence) {
        case SEEK_SET:
            value = 0;
            break;

        case SEEK_CUR:
            value = f_tell(fp);
            break;

        case SEEK_END:
            value = f_size(fp);
            break;

        default:
            error = -1;
            return error;
    }

    error = f_lseek(fp, value + offset);

    if (error != FR_OK) return -2;

    return 0;
}

/*计算某个csv文件的总行数*/
int GetDetectTotalLineCount(char* Path)
{
    int   line_num = 0;
    char* strLine  = NULL;
    /*动态申请50个字节的内存，默认从内部sram申请*/
    strLine        = malloc(100);
    assert_param(NULL != strLine);
    memset(strLine, 0, 100);
    retUSER = f_open(&USERFile, Path, FA_OPEN_EXISTING | FA_READ);

    if (FR_OK != retUSER) {
        printf("当前文件:%s不存在\n", Path);
        return -1;
    }

    flseek(&USERFile, 0, SEEK_SET);

    while (f_gets(strLine, 100, &USERFile)) {
        line_num++;
    }

    flseek(&USERFile, 0, SEEK_SET);
    f_close(&USERFile);
    // 释放内存，避免内存泄露
    free(strLine);
    return line_num;
}

int ReadlLineData(int File_Count, int line_number)
{
    int   WhichLine;         // 指定要读取哪一行
    int   CurrentIndex = 0;  // 当前读取的行
    char* StrLine      = NULL;
    char* ptr          = NULL;
    int   data_serial_num_file_count;
    char  filename[FILE_NAME_LEN] = {0};
    WhichLine                     = line_number - 1;
    StrLine                       = malloc(100);
    assert_param(NULL != strLine);
    /*计算当前的流水号所对应的文件*/
    data_serial_num_file_count = File_Count;
    sprintf(filename, "0:SRED_EHS%d.csv", data_serial_num_file_count);
    retUSER = f_open(&USERFile, filename, FA_OPEN_EXISTING | FA_READ);

    if (FR_OK != retUSER) {
        printf("打开%s文件失败，err = %d\r\n", filename, retUSER);
        free(StrLine);
        return -2;
    }

    memset(csv_file_record.Scan_Data, 0, 100);

    while (!f_eof(&USERFile)) {
        if (CurrentIndex == WhichLine) {
            // 读取一行
            f_gets(StrLine, 100, &USERFile);
            printf("当前数据:%s\n", StrLine);
            // Para1:获取流水号
            csv_file_record.serial_number = atoi(StrLine);
            // Para2:获取年
            ptr                           = strstr(StrLine, ",");
            csv_file_record.year          = atoi(ptr + 1);
            // Para3:获取月
            ptr                           = strstr(ptr + 1, "/");
            csv_file_record.month         = atoi(ptr + 1);
            // Para4:获取日
            ptr                           = strstr(ptr + 1, "/");
            csv_file_record.day           = atoi(ptr + 1);
            // Para5:获取时
            ptr                           = strstr(ptr + 1, " ");
            csv_file_record.hour          = atoi(ptr + 1);
            // Para6:获取分
            ptr                           = strstr(ptr + 1, ":");
            csv_file_record.minute        = atoi(ptr + 1);
            // Para7:获取通道分类物===>为0则是毒品，为1则是爆炸物
            ptr                           = strstr(ptr + 1, ",");
            csv_file_record.detect_result = atoi(ptr + 1);
            /*添加扫码数据显示*/
            // Para8:获取扫码编号
            ptr                           = strstr(ptr + 1, ",");

            if (ptr != NULL)
                memcpy(csv_file_record.Scan_Data, ptr + 1, strlen(ptr));
        }

        // 读取一行,并定位到下一行
        f_gets(StrLine, 100, &USERFile);
        CurrentIndex++;
    }

    f_close(&USERFile);
    free(StrLine);
    return CurrentIndex;
}
