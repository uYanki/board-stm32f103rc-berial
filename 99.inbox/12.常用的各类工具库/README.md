# libutils-c
平时开发使用的C函数库，一部分是自己开发的，一部分是从各个开源组件中提取出来的。

##功能说明
* 基本
  + xmalloc ---------- 对malloc free等动态内存函数封装
  + xstring ---------- 字符串函数
  + comm.h ----------- 一些通用的宏定义
  + opt -------------- 命令行选项
  + log -------------- 日志
  + conf ------------- 配置文件
  + conf_hash -------- 配置文件
* 系统
  + sig -------------- 信号处理
  + sock ------------- socket API封装，方便使用
  + threadpool ------- 线程池
  + workqueque ------- 任务队列，和线程池搭配使用
  + mempool ---------- 固定大小分配的内存池
  + mpool ------------ 内存池
  + dir -------------- 目录
* 加密、Hash
  + aes -------------- aes加密算法
  + crypt ------------ 基于aes的数据加密
  + md5 -------------- md5
  + sha1 ------------- sha1
  + crc32 ------------ crc32
  + rand ------------- 随机数生成
* 数据结构、算法、数据处理
  + fsfhash ---------- Free Software Foundation Hast table
  + phphash ---------- PHP里实现的hash table
  + hbm -------------- Boyer-Moore-Horsepool 字符串查找算法
  + mwm -------------- A Modified Wu-Manber Style Multi-Pattern Matcher （多模式匹配算法）
  + charset ---------- 字符集、全角转半角、繁体转简体等
  + regexp ----------- 正则表达式匹配，使用pcre posix，支持utf-8字符集
  + datime ----------- 日期时间转换
  + headword --------- 中心词提取
  + recognizer ------- 关键词识别
  + record ----------- 用于从文件中读取XML记录并解析
  + testif ----------- 条件测试
  + category --------- 构建分类树结构
  + xml -------------- XML解析
* web
  + html ------------- HTML过滤
  + htmlentities ----- HTML实体decode为utf8字符
  + htmlextract ------ HTML文本提取
  + htmlparse -------- HTML解析器
  + htmlurl ---------- 从HTML中提取URL
  + http ------------- HTTP请求和响应处理
  + url -------------- URL
  + urlparse --------- URL解析
* 实用类
  + utils.c ---------- 一些实用函数：文件读写、压缩、url/html/base64/十六进制编解码等

## 编译安装
所需依赖库：pcre tokyocabinet iconv , tokyocabinet是一个Key-Value DB。    
make; make install  默认安装到/usr/local/lib  /usr/loca/include

## 反馈
dudubird2006###163.com
