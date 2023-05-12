# STM32基于HAL库实现SD卡使用FATFS操作系统读写数据

## 相关功能文件

- 在`..\Middlewares\Third_Party\FatFs`中提供了FATFS文件系统相关的中间件原码
- 在`..\FATFS\Target\bsp_driver_sd.c/.h`中实现SD卡的相关初始化、读写等操作
- 在`..\FATFS\Target\sd_diskio.c/.h`中实现SD卡的底层磁盘IO相关初始化、读写等操作
- 在`..\FATFS\App\fatfs.c/.h`中实现通过fatfs文件系统挂载SD卡

## STM32CubeMX相关配置

- SDIO配置选择`SD 4 bits Wide bus`模式，使用SDIO_D0-D3四条数据线
- 开启SDIO中断
- 开启SDIO_RX和SDIO_TX的DMA
- 更改SDIOCLK时钟分频因子，使SDIO时钟大约20MHz左右
- 添加SD Card的FATFS
- 目标代码页选择`Simplified Chinese(DBCS)`简体中文926，以支持中文读写操作
- 长文件名可使能在栈区
- 开启DMA模板
- 生成代码时将堆栈适当加大

## SD卡相关读写操作

- 在fatfs.h中以定义文件操作相关变量

 ```C
uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */
 ```

- 在fatfs.c中的`MX_FATFS_Init()`中以实现FATFS连接SD驱动程序，判断`retSD`为0则成功

> - 常用SD卡使用FATFS文件系统相关函数
>
> ```C
> FRESULT f_mount (FATFS* fs, const TCHAR* path, BYTE opt); /* Mount/Unmount a logical drive */
> FRESULT f_open (FIL* fp, const TCHAR* path, BYTE mode);   /* Open or create a file */
> FRESULT f_close (FIL* fp);                                /* Close an open file object */
> FRESULT f_read (FIL* fp, void* buff, UINT btr, UINT* br); /* Read data from the file */
> FRESULT f_write (FIL* fp, const void* buff, UINT btw, UINT* bw); /* Write data to the file */
> FRESULT f_lseek (FIL* fp, FSIZE_t ofs);                          /* Move file pointer of the file object */
> ...
> ```

**更多功能函数在`ff.c`中，函数功能描述文档http://elm-chan.org/fsw/ff/00index_e.html**

- 测试程序

```C
  void print_fatfs_error(FRESULT ret);
  int main(void)
  {
  #define SD_DEBUG 1
    FRESULT f_ret;
    uint32_t writtenbytes, readbytes; //文件读写字节数
    char file_path[] = "stm32_sd.txt";
    uint8_t wtext[] = "123qweasdzxc";
    uint8_t rtext[100];
    if (retSD == 0) //link sd success
    {
      f_ret = f_mount(&SDFatFS, (TCHAR const *)SDPath, 0); //装载逻辑驱动(FATFS)
      if (f_ret != FR_OK)
      {
  #if SD_DEBUG
        print_fatfs_error(f_ret);
  #endif
      }
  #if SD_DEBUG
      printf("已装载逻辑驱动器\r\n");
  #endif
      f_ret = f_open(&SDFile, file_path, FA_OPEN_APPEND | FA_WRITE); //打开文件写入数据
      if (f_ret != FR_OK)
      {
  #if SD_DEBUG
        print_fatfs_error(f_ret);
  #endif
      }
  #if SD_DEBUG
      printf("已打开文件，准备写入数据\r\n");
  #endif
      f_ret = f_write(&SDFile, wtext, sizeof(wtext), (UINT *)&writtenbytes);
      if (writtenbytes == 0 || f_ret != FR_OK)
      {
  #if SD_DEBUG
        print_fatfs_error(f_ret);
  #endif
      }
  #if SD_DEBUG
      printf("已写入数据\r\n");
  #endif
      if (f_close(&SDFile) != FR_OK)
      {
  #if SD_DEBUG
        print_fatfs_error(f_ret);
  #endif
      }
  #if SD_DEBUG
      printf("已关闭文件\r\n");
  #endif
      if (f_open(&SDFile, file_path, FA_READ) != FR_OK) //打开文件读取数据
      {
  #if SD_DEBUG
        print_fatfs_error(f_ret);
  #endif
      }
  #if SD_DEBUG
      printf("已打开文件，准备读取数据\r\n");
  #endif
      f_ret = f_read(&SDFile, rtext, sizeof(rtext), (UINT *)&readbytes); //读取数据
      if (readbytes == 0 || f_ret != FR_OK)
      {
  #if SD_DEBUG
        print_fatfs_error(f_ret);
  #endif
      }
  #if SD_DEBUG
      printf("已读取数据\r\n");
      printf("Read:%s\r\n", rtext);
  #endif
      if (f_close(&SDFile) != FR_OK)
      {
  #if SD_DEBUG
        print_fatfs_error(f_ret);
  #endif
      }
  #if SD_DEBUG
      printf("已关闭文件\r\n");
  #endif
    }
    FATFS_UnLinkDriver(SDPath);
    while(1)
    {
    }
  }

#if SD_DEBUG
void print_fatfs_error(FRESULT ret)
{
  switch (ret)
  {
  case FR_OK:
    printf("成功_FR_OK\r\n");
    break;
  case FR_DISK_ERR:
    printf("磁盘错误_FR_DISK_ERR\r\n");
    break;
  case FR_INT_ERR:
    printf("断言失败_FR_INT_ERR\r\n");
    break;
  case FR_NOT_READY:
    printf("硬件驱动无法工作_FR_NOT_READY\r\n");
    break;
  case FR_NO_FILE:
    printf("找不到文件_FR_NO_FILE\r\n");
    break;
  case FR_NO_PATH:
    printf("找不到路径_FR_NO_PATH\r\n");
    break;
  case FR_INVALID_NAME:
    printf("路径名格式无效_FR_INVALID_NAME\r\n");
    break;
  case FR_DENIED:
    printf("由于访问被禁止或目录已满而拒绝访问_FR_DENIED\r\n");
    break;
  case FR_EXIST:
    printf("由于禁止访问而拒绝访问_FR_EXIST\r\n");
    break;
  case FR_INVALID_OBJECT:
    printf("文件/目录对象无效_FR_INVALID_OBJECT\r\n");
    break;
  case FR_WRITE_PROTECTED:
    printf("物理驱动器受写保护_FR_WRITE_PROTECTED\r\n");
    break;
  case FR_INVALID_DRIVE:
    printf("逻辑驱动器号无效_FR_INVALID_DRIVE\r\n");
    break;
  case FR_NOT_ENABLED:
    printf("该卷没有工作区_FR_NOT_ENABLED\r\n");
    break;
  case FR_NO_FILESYSTEM:
    printf("没有有效的FAT卷_FR_NO_FILESYSTEM\r\n");
    break;
  case FR_MKFS_ABORTED:
    printf("由于任何问题f_mkfs()中止_FR_MKFS_ABORTED\r\n");
    break;
  case FR_TIMEOUT:
    printf("超时_FR_TIMEOUT\r\n");
    break;
  case FR_LOCKED:
    printf("根据文件共享策略,操作被拒绝_FR_LOCKED\r\n");
    break;
  case FR_NOT_ENOUGH_CORE:
    printf("无法分配LFN工作缓冲区_FR_NOT_ENOUGH_CORE\r\n");
    break;
  case FR_TOO_MANY_OPEN_FILES:
    printf("打开的文件数太多_FR_TOO_MANY_OPEN_FILES\r\n");
    break;
  case FR_INVALID_PARAMETER:
    printf("给定参数无效_FR_INVALID_PARAMETER\r\n");
    break;
  default:
    printf("SD_Other error\r\n");
    break;
  }
}
#endif
```
