/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recognizer.h"
#include "utils.h"
#include "xmalloc.h"
#include "charset.h"
#include "comn.h"
#include "xstring.h"

// 注意：分词时，输入的词可能对应多个词库的词，只是词的type不同。切词目前是返回type最小的word。
// Debug
// #define  TEST_RECOGNIZER

#define MAX_KWD_LEN               128  // 最大关键词长度
#define HASH_REDUNDANCY_RATE      3
#define MAX_BUFFER                1024
#define MAX_QUERY_STR_LEN         1024

#define COLKWD_TABLE_INIT_SIZE    20000
#define COLKWD_TABLE_INC_SIZE     1000

#define KEYWORD_TABLE_STATUS      "word.n"
#define KEYWORD_TABLE             "word.tab"
#define KEYWORD_TABLE_COL         "word.col"
#define KEYWORD_TABLE_POOL        "word.pool"
#define KEYWORD_TABLE_POOL_STATUS "wordpool.n"

#define SAVE_FILE_MAX_SIZE        2000000000
#define MAX_PATH                  MAX_PATH_LEN

typedef struct
{
    char* p;
    int   size;
    int   free;
    int   pos;
} BUFFER_T;

typedef struct _Kwd {
    int offset;     // 关键词在pool中偏移，所有关键词都放在keyword pool中，用'\0'分隔。如果offset=-1，则表示该节点没有使用。
    int coloffset;  // 冲突节点在collsion keyword table中的偏移，coloffset=-1，表示没有冲突节点。
    int type;       // 关键词类型
} KWD_T;

typedef struct _KwdTable {
    int    hashsize;      // hash表大小，应该采用素数。
    int    sizeindex;     // hash表大小在size_table数组的下标
    KWD_T* KwdTableHead;  // keyword table

    KWD_T* ColKwdTable;  // collsion keyword table
    int    col_size;
    int    col_pos;
    int    col_free;

    BUFFER_T kwdpool;  // keyword pool,用来存放所有的关键词

    int codetype;  // GBK,UTF-8
    int KwdCnt;
    int TooLong;      // 太长的关键词数
    int EmptyNum;     // 关键词为空数
    int ErrorNum;     // 错误数
    int DupNum;       // 关键词重复数，重复指的是关键词和类型都一致。
    int kwd_max_len;  // 不能超过MAX_KWD_LEN，用于分词。

    int Buckets;  // hash表节点总数包括冲突节点
    int memsize;
} KWD_TABLE_T;

static uint32_t HashStr(char* Kwd, uint32_t KwdLen);
static int      ISWord(void* DIC, char* Kwd, uint32_t KwdLen);
static int      CutWord(void* DIC, char* cstr, char* word);

/* hash表大小的可能取值 */
static int size_table[] =
    {
        5, 11, 19, 53, 107, 223, 463, 983, 1979, 3907, 7963, 16229, 32531, 65407, 130987, 262237, 524521, 1048793, 2097397, 4194103, 8388857, 16777447, 33554201, 67108961, 134217487, 268435697, 536870683, 1073741621, 2147483399};

// 根据文件word文件在内存中创建Hash表。
// type: 初始化方式，1：创建分词词库并并保存，2：直接load词库。
// isprint: 是否打印出太长的关键词
// keywords 文件，存储格式为: KEYWORD1^TYPE\nKEYWORD2^TYPE\n...KEYWORDN^TYPE\n
// keywords文件可以不提供词的类型，该词的类型为0
void* recognizer_init(const char* word_path, const char* dic_dir, int type, int codetype, int isprint)
{
    KWD_TABLE_T* kwd_table;
    KWD_T *      kwd, *new_kwd;
    int          i, hashsize, len, linenum, linelen;
    uint32_t     hashvalue;
    int          tabindex;
    long         filesize;
    char *       line, *pc, *pool_start;
    int          COUNTS_OF_SIZE_TABLE;
    int          word_type, next_colkwd;

    if (type != RECOGNIZER_INIT_TYPE_CREATE && type != RECOGNIZER_INIT_TYPE_LOAD)
        return NULL;

    kwd_table = (KWD_TABLE_T*)xcalloc(1, sizeof(KWD_TABLE_T));

    if (type == RECOGNIZER_INIT_TYPE_LOAD) {
        if (recognizer_load(dic_dir, kwd_table) < 0) {
            xfree(kwd_table);
            return NULL;
        }

        return kwd_table;
    }

    if (codetype == CODETYPE_GBK || codetype == CODETYPE_UTF8)
        kwd_table->codetype = codetype;
    else {
        xfree(kwd_table);
        return NULL;
    }

    kwd_table->kwdpool.p = loadfile(word_path, &filesize);
    if (!kwd_table->kwdpool.p) {
        xfree(kwd_table);
        return NULL;
    }
    kwd_table->kwdpool.size = (int)filesize + 1;
    kwd_table->kwdpool.pos  = kwd_table->kwdpool.size;
    kwd_table->kwdpool.free = 0;

    pool_start              = kwd_table->kwdpool.p;

    linenum                 = getlinenum(kwd_table->kwdpool.p, "\n");
    if (!linenum) {
        xfree(kwd_table->kwdpool.p);
        xfree(kwd_table);
        return NULL;
    }
    kwd_table->KwdCnt    = linenum;

    /* Allocate Memory to HashTable. */
    hashsize             = kwd_table->KwdCnt * HASH_REDUNDANCY_RATE;
    COUNTS_OF_SIZE_TABLE = (sizeof(size_table) / sizeof(size_table[0]));
    for (i = 0; i < COUNTS_OF_SIZE_TABLE; ++i) {
        if (hashsize <= size_table[i]) {
            hashsize             = size_table[i];
            kwd_table->sizeindex = i;
            break;
        }
    }

    if (i == COUNTS_OF_SIZE_TABLE) {
        hashsize             = size_table[COUNTS_OF_SIZE_TABLE - 1];
        kwd_table->sizeindex = COUNTS_OF_SIZE_TABLE - 1;
    }

    kwd_table->hashsize     = hashsize;
    kwd_table->KwdTableHead = (KWD_T*)xmalloc(sizeof(KWD_T) * kwd_table->hashsize);
    for (i = 0; i < kwd_table->hashsize; i++) {
        kwd_table->KwdTableHead[i].offset    = -1;
        kwd_table->KwdTableHead[i].coloffset = -1;
        kwd_table->KwdTableHead[i].type      = WORD_TYPE_UNKNOW;
    }
    kwd_table->Buckets     = hashsize;

    // 为冲突关键词table分配空间
    kwd_table->ColKwdTable = (KWD_T*)xmalloc(sizeof(KWD_T) * COLKWD_TABLE_INIT_SIZE);
    kwd_table->col_size    = COLKWD_TABLE_INIT_SIZE;
    kwd_table->col_free    = COLKWD_TABLE_INIT_SIZE;
    kwd_table->col_pos     = 0;
    for (i = 0; i < kwd_table->col_size; i++) {
        kwd_table->ColKwdTable[i].offset    = -1;
        kwd_table->ColKwdTable[i].coloffset = -1;
        kwd_table->ColKwdTable[i].type      = WORD_TYPE_UNKNOW;
    }
    kwd_table->Buckets += COLKWD_TABLE_INIT_SIZE;

    /* loop ,read a keyword */
    linenum = 0;
    while ((line = getaline(pool_start, "\n", &linelen)) != NULL) {
        linenum++;
        trim(line);
        if (!*line) {
            kwd_table->EmptyNum++;
            kwd_table->KwdCnt--;
            goto NEXT_LINE;
        }

        if (codetype == CODETYPE_GBK) {
            gbkqj2bj(line, line);
            gbkqj2bjext(line, line);
        } else if (codetype == CODETYPE_UTF8)
            utf8qj2bj(line, line);
        strtolower(line, strlen(line), line, codetype);

        pc = strrchr(line, WORD_TYPE_SEPARATOR);  // 可以允许不定义类型
        if (!pc) {
            word_type = WORD_TYPE_UNKNOW;
        } else {
            *pc       = 0;
            word_type = atoi(pc + 1);
            if (word_type < 0) {
                kwd_table->ErrorNum++;
                kwd_table->KwdCnt--;
                goto NEXT_LINE;
            }
        }

        len = strlen(line);
        if (len > MAX_KWD_LEN) {
            if (isprint)
                fprintf(stdout, "Warning:Too Long Keyword: %s\n", line);

            kwd_table->TooLong++;
            kwd_table->KwdCnt--;
            goto NEXT_LINE;
        }

        if (len > kwd_table->kwd_max_len)
            kwd_table->kwd_max_len = len;

        /* hash */
        hashvalue = HashStr(line, len);
        tabindex  = hashvalue % kwd_table->hashsize;
        kwd       = kwd_table->KwdTableHead + tabindex;

        if (kwd->offset == -1)  // 该bucket为空
        {
            kwd->offset = line - kwd_table->kwdpool.p;
            kwd->type   = word_type;
        } else  // 已经占用
        {
            // 先判断关键词是否为重复的。如果重复则保留较小type的word
            if (memcmp(line, kwd_table->kwdpool.p + kwd->offset, len) == 0)  //&& word_type == kwd->type
            {
                if (word_type < kwd->type)
                    kwd->type = word_type;

                kwd_table->DupNum++;
                kwd_table->KwdCnt--;
                goto NEXT_LINE;
            }

            while ((next_colkwd = kwd->coloffset) != -1) {
                kwd = kwd_table->ColKwdTable + next_colkwd;
                if (memcmp(line, kwd_table->kwdpool.p + kwd->offset, len) == 0)  //&& word_type == kwd->type
                {
                    if (word_type < kwd->type)
                        kwd->type = word_type;

                    kwd_table->DupNum++;
                    kwd_table->KwdCnt--;
                    goto NEXT_LINE;
                }
            }

            // 没有重复的，从冲突table中申请一个存储单元，插在冲突链表的最后
            if (kwd_table->col_free == 0) {
                kwd_table->ColKwdTable = (KWD_T*)xrealloc(kwd_table->ColKwdTable, sizeof(KWD_T) * (kwd_table->col_size + COLKWD_TABLE_INC_SIZE));
                kwd_table->col_size += COLKWD_TABLE_INC_SIZE;
                kwd_table->col_free += COLKWD_TABLE_INC_SIZE;
                kwd_table->Buckets += COLKWD_TABLE_INC_SIZE;
            }
            new_kwd            = kwd_table->ColKwdTable + kwd_table->col_pos;
            new_kwd->offset    = line - kwd_table->kwdpool.p;
            new_kwd->coloffset = -1;
            new_kwd->type      = word_type;

            kwd->coloffset     = kwd_table->col_pos;
            kwd_table->col_pos++;
            kwd_table->col_free--;
        }

    NEXT_LINE:
        pool_start += (linelen + 1);
    }

    kwd_table->memsize = sizeof(KWD_T) * kwd_table->Buckets;

    if (linenum != (kwd_table->KwdCnt + kwd_table->TooLong + kwd_table->EmptyNum + kwd_table->ErrorNum + kwd_table->DupNum))
        return NULL;  // 没有释放内存，调用者得退出程序。

    if (recognizer_save(dic_dir, kwd_table) < 0)
        return NULL;

    return kwd_table;
}

int recognizer_save(const char* dir, void* kwd_table)
{
    char         path[MAX_PATH + 1];
    FILE*        savefp;
    KWD_TABLE_T* tab = (KWD_TABLE_T*)kwd_table;

    if (!dir || !tab)
        return -1;

    // 检查要保存的数据不能大于2G
    if ((tab->hashsize * sizeof(KWD_T)) >= SAVE_FILE_MAX_SIZE || (tab->col_size * sizeof(KWD_T)) >= SAVE_FILE_MAX_SIZE || tab->kwdpool.size >= SAVE_FILE_MAX_SIZE)
        return -1;

    // word.n
    snprintf(path, MAX_PATH + 1, "%s%s%s", dir, PATH_SEPARATOR_STR, KEYWORD_TABLE_STATUS);
    savefp = fopen(path, "wb");
    if (!savefp)
        return -1;
    fprintf(savefp, "hashsize:%d,sizeindex:%d\r\ncolsize:%d,colpos:%d,colfree:%d\r\ncodetype:%d,words:%d,long:%d,empty:%d,error:%d,dup:%d,WordMaxLen:%d\r\nbuckets:%d,memsize:%d",
            tab->hashsize, tab->sizeindex, tab->col_size, tab->col_pos, tab->col_free, tab->codetype, tab->KwdCnt, tab->TooLong, tab->EmptyNum, tab->ErrorNum, tab->DupNum, tab->kwd_max_len, tab->Buckets, tab->memsize);
    fclose(savefp);

    // word.tab
    snprintf(path, MAX_PATH + 1, "%s%s%s", dir, PATH_SEPARATOR_STR, KEYWORD_TABLE);
    savefp = fopen(path, "wb");
    if (!savefp)
        return -1;
    setvbuf(savefp, NULL, _IONBF, 0);
    fwrite(tab->KwdTableHead, sizeof(KWD_T), tab->hashsize, savefp);
    fclose(savefp);

    // word.col
    snprintf(path, MAX_PATH + 1, "%s%s%s", dir, PATH_SEPARATOR_STR, KEYWORD_TABLE_COL);
    savefp = fopen(path, "wb");
    if (!savefp)
        return -1;
    setvbuf(savefp, NULL, _IONBF, 0);
    fwrite(tab->ColKwdTable, sizeof(KWD_T), tab->col_size, savefp);
    fclose(savefp);

    // wordpool.n
    snprintf(path, MAX_PATH + 1, "%s%s%s", dir, PATH_SEPARATOR_STR, KEYWORD_TABLE_POOL_STATUS);
    savefp = fopen(path, "wb");
    if (!savefp)
        return -1;
    fprintf(savefp, "size:%d,pos:%d,free:%d", tab->kwdpool.size, tab->kwdpool.pos, tab->kwdpool.free);
    fclose(savefp);

    // word.pool
    snprintf(path, MAX_PATH + 1, "%s%s%s", dir, PATH_SEPARATOR_STR, KEYWORD_TABLE_POOL);
    savefp = fopen(path, "wb");
    if (!savefp)
        return -1;
    setvbuf(savefp, NULL, _IONBF, 0);
    fwrite(tab->kwdpool.p, sizeof(char), tab->kwdpool.size, savefp);
    fclose(savefp);

    return 0;
}

int recognizer_load(const char* dir, void* kwd_table)
{
    char         path[MAX_PATH + 1];
    FILE*        loadfp;
    KWD_TABLE_T* tab = (KWD_TABLE_T*)kwd_table;

    if (!dir || !tab)
        return -1;

    // word.n
    snprintf(path, MAX_PATH + 1, "%s%s%s", dir, PATH_SEPARATOR_STR, KEYWORD_TABLE_STATUS);
    loadfp = fopen(path, "rb");
    if (!loadfp)
        return -1;
    fscanf(loadfp, "hashsize:%d,sizeindex:%d\r\ncolsize:%d,colpos:%d,colfree:%d\r\ncodetype:%d,words:%d,long:%d,empty:%d,error:%d,dup:%d,WordMaxLen:%d\r\nbuckets:%d,memsize:%d",
           &tab->hashsize, &tab->sizeindex, &tab->col_size, &tab->col_pos, &tab->col_free, &tab->codetype, &tab->KwdCnt, &tab->TooLong, &tab->EmptyNum, &tab->ErrorNum, &tab->DupNum, &tab->kwd_max_len, &tab->Buckets, &tab->memsize);
    fclose(loadfp);

    // word.tab
    snprintf(path, MAX_PATH + 1, "%s%s%s", dir, PATH_SEPARATOR_STR, KEYWORD_TABLE);
    tab->KwdTableHead = (KWD_T*)loadfileb(path, NULL);

    // word.col
    snprintf(path, MAX_PATH + 1, "%s%s%s", dir, PATH_SEPARATOR_STR, KEYWORD_TABLE_COL);
    tab->ColKwdTable = (KWD_T*)loadfileb(path, NULL);

    // wordpool.n
    snprintf(path, MAX_PATH + 1, "%s%s%s", dir, PATH_SEPARATOR_STR, KEYWORD_TABLE_POOL_STATUS);
    loadfp = fopen(path, "rb");
    if (!loadfp)
        return -1;
    fscanf(loadfp, "size:%d,pos:%d,free:%d", &tab->kwdpool.size, &tab->kwdpool.pos, &tab->kwdpool.free);
    fclose(loadfp);

    // word.pool
    snprintf(path, MAX_PATH + 1, "%s%s%s", dir, PATH_SEPARATOR_STR, KEYWORD_TABLE_POOL);
    tab->kwdpool.p = (char*)loadfileb(path, NULL);

    return 0;
}

// 追加新增的词到词库
int recognizer_append(void* kwdtable, const char* path, int isprint)
{
    KWD_TABLE_T* kwd_table = (KWD_TABLE_T*)kwdtable;
    int          len, flag, filesize, linelen, word_type, pos, wordnum;
    char *       pc, *pool_start, *line;
    KWD_T *      kwd, *free_kwd, *new_kwd;
    uint32_t     hashvalue;
    int          tabindex;
    int          codetype = kwd_table->codetype;

    pc                    = loadfile(path, (long*)&filesize);
    if (!pc || !filesize)
        return -1;

    pos                  = kwd_table->kwdpool.pos;
    kwd_table->kwdpool.p = (char*)xrealloc(kwd_table->kwdpool.p, kwd_table->kwdpool.size + filesize + 1);
    kwd_table->kwdpool.size += (filesize + 1);
    kwd_table->kwdpool.free = 0;
    kwd_table->kwdpool.pos  = kwd_table->kwdpool.size;

    pool_start              = kwd_table->kwdpool.p + pos;
    memcpy(pool_start, pc, filesize);
    kwd_table->kwdpool.p[kwd_table->kwdpool.size - 1] = 0;
    xfree(pc);

    /* loop ,read a keyword */
    wordnum = 0;
    while ((line = getaline(pool_start, "\n", &linelen)) != NULL) {
        trim(line);
        if (!*line) {
            kwd_table->EmptyNum++;
            goto NEXT_LINE;
        }

        if (codetype == CODETYPE_GBK) {
            gbkqj2bj(line, line);
            gbkqj2bjext(line, line);
        } else if (codetype == CODETYPE_UTF8)
            utf8qj2bj(line, line);
        strtolower(line, strlen(line), line, codetype);

        pc = strrchr(line, WORD_TYPE_SEPARATOR);
        if (!pc) {
            word_type = WORD_TYPE_UNKNOW;
        } else {
            *pc       = 0;
            word_type = atoi(pc + 1);
            if (word_type < 0) {
                kwd_table->ErrorNum++;
                goto NEXT_LINE;
            }
        }

        len = strlen(line);
        if (len > MAX_KWD_LEN) {
            if (isprint)
                fprintf(stdout, "Warning:Too Long Keyword: %s\n", line);

            kwd_table->TooLong++;
            goto NEXT_LINE;
        }

        if (len > kwd_table->kwd_max_len)
            kwd_table->kwd_max_len = len;

        /* hash */
        hashvalue = HashStr(line, len);
        tabindex  = hashvalue % kwd_table->hashsize;
        kwd       = kwd_table->KwdTableHead + tabindex;

        if (kwd->offset == -1 && kwd->coloffset == -1)  // 该bucket为空，并且没有冲突节点
        {
            kwd->offset    = line - kwd_table->kwdpool.p;
            kwd->coloffset = -1;
            kwd->type      = word_type;
            wordnum++;
        } else {
            // 先判断是否重复，并找出删除后已经空闲的节点
            free_kwd = NULL;
            flag     = 0;

            do {
                if (flag)
                    kwd = kwd_table->ColKwdTable + kwd->coloffset;

                if (kwd->offset == -1)  // 该节点已经删除，但有冲突节点，需要判断冲突节点里是否有重复的。
                {
                    if (!free_kwd)
                        free_kwd = kwd;
                } else if (kwd->offset != -1 && memcmp(line, kwd_table->kwdpool.p + kwd->offset, len) == 0 && word_type == kwd->type)  // 该节点已经占用,先判断关键词是否为重复的。
                {
                    kwd_table->DupNum++;
                    goto NEXT_LINE;
                }

                if (!flag)
                    flag = 1;

            } while (kwd->coloffset != -1);

            if (free_kwd)  // 没有重复且找到已经删除后空闲的节点
            {
                free_kwd->offset = line - kwd_table->kwdpool.p;
                free_kwd->type   = word_type;
                kwd_table->KwdCnt++;
            } else  // 没有重复的且没有找到已经删除后空闲的节点，从冲突table中申请一个存储单元，插在冲突链表的最后
            {
                if (kwd_table->col_free == 0) {
                    kwd_table->ColKwdTable = (KWD_T*)xrealloc(kwd_table->ColKwdTable, sizeof(KWD_T) * (kwd_table->col_size + COLKWD_TABLE_INC_SIZE));
                    kwd_table->col_size += COLKWD_TABLE_INC_SIZE;
                    kwd_table->col_free += COLKWD_TABLE_INC_SIZE;
                    kwd_table->Buckets += COLKWD_TABLE_INC_SIZE;
                }
                new_kwd            = kwd_table->ColKwdTable + kwd_table->col_pos;
                new_kwd->offset    = line - kwd_table->kwdpool.p;
                new_kwd->coloffset = -1;
                new_kwd->type      = word_type;

                kwd->coloffset     = kwd_table->col_pos;
                kwd_table->col_pos++;
                kwd_table->col_free--;
                kwd_table->KwdCnt++;
            }

            wordnum++;
        }

    NEXT_LINE:
        pool_start += (linelen + 1);
    }

    kwd_table->memsize = sizeof(KWD_T) * kwd_table->Buckets;

    return wordnum;
}

// 删除词库中需要删除的词，注意，不会真正删除，只是打上删除标记
// 错误返回-1，正确返回删除的词数
// 删除的词不必提供type，会删除所有type的word
int recognizer_delete(void* kwdtable, const char* path)
{
    KWD_TABLE_T* kwd_table = (KWD_TABLE_T*)kwdtable;
    int          len, wordnum, linelen;
    char *       pc, *delword, *line;
    KWD_T*       kwd;
    uint32_t     hashvalue;
    int          tabindex;
    int          codetype = kwd_table->codetype;

    delword               = loadfile(path, NULL);
    if (!delword)
        return -1;

    /* loop ,read a keyword */
    wordnum = 0;
    pc      = delword;
    while ((line = getaline(pc, "\n", &linelen)) != NULL) {
        trim(line);
        if (!*line)
            goto NEXT_LINE;

        if (codetype == CODETYPE_GBK) {
            gbkqj2bj(line, line);
            gbkqj2bjext(line, line);
        } else if (codetype == CODETYPE_UTF8)
            utf8qj2bj(line, line);
        strtolower(line, strlen(line), line, codetype);

        len = strlen(line);
        if (len > MAX_KWD_LEN)
            goto NEXT_LINE;

        /* hash */
        hashvalue = HashStr(line, len);
        tabindex  = hashvalue % kwd_table->hashsize;
        kwd       = kwd_table->KwdTableHead + tabindex;

        while (1) {
            if (kwd->offset != -1 && memcmp(line, kwd_table->kwdpool.p + kwd->offset, len) == 0) {
                kwd->offset = -1;
                kwd->type   = WORD_TYPE_UNKNOW;
                wordnum++;

                // 继续删除其他的type的要删除词
            }

            if (kwd->coloffset != -1)
                kwd = kwd_table->ColKwdTable + kwd->coloffset;
            else
                break;
        }

    NEXT_LINE:
        pc += (linelen + 1);
    }

    xfree(delword);
    return wordnum;
}

void recognizer_free(void* kwd_table)
{
    KWD_TABLE_T* p = (KWD_TABLE_T*)kwd_table;

    xfree(p->KwdTableHead);
    xfree(p->ColKwdTable);
    xfree(p->kwdpool.p);
    xfree(p);
}

/*
@ 功能：切词。
@ 输入参数：
      QueryStr: 用户查询串。
      WordStr:  存放识别出来的关键词串,关键词之间用空格连接。
      不管是识别出来的还是没有识别出来的都将输出，识别出来的word都有类型，没有识别出来的则没有。
@ 返回结果：
   -1:      出现错误。
    n>=0:   识别出的关键词个数。

@ 注意：WordStr QueryStr的最大长度为MAX_QUERY_STR_LEN。
*/

int recognizer_words(void* DIC, const char* QueryStr, char* WordStr)
{
    KWD_TABLE_T* dic = (KWD_TABLE_T*)DIC;
    char *       pcs, *pscs;
    int          cslen, idx, res, wordidx, sublen, wordlen, wordcnt;
    char         word[MAX_QUERY_STR_LEN + 1] = {
        0,
    };
    char cs[MAX_QUERY_STR_LEN + 1] = {
        0,
    };
    int codetype;

    if (!DIC || !QueryStr || !WordStr)
        return -1;

    if (!*QueryStr)
        return 0;

    codetype = dic->codetype;
    strcpy(cs, QueryStr);
    strcat(cs, " ");
    cslen   = strlen(cs);
    wordidx = 0;
    wordcnt = 0;
    pcs     = cs;
    pscs    = cs;

    for (idx = 0; idx < cslen; ++idx) {
        int clen;

        if (codetype == CODETYPE_GBK)
            clen = GBKCHLEN((uint8_t)pcs[idx]);
        else if (codetype == CODETYPE_UTF8)
            clen = UTF8CHLEN((uint8_t)pcs[idx]);

        if (clen > 1) {
            idx += (clen - 1);
            continue;
        }

        if (*(pcs + idx) == ' ') {
            *(pcs + idx) = 0;
            sublen       = strlen(pscs);

            if ((res = CutWord(DIC, pscs, word)) < 0)
                return -1;

            wordlen = strlen(word);
            memcpy(WordStr + wordidx, word, wordlen);
            wordidx += wordlen;
            *(WordStr + wordidx) = ' ';
            wordidx++;
            wordcnt += res;
            pscs = pcs + idx + 1;
        }
    }

    *(WordStr + wordidx - 1) = 0;

    return wordcnt;
}

// 用于hash存储、hash查找的函数。
static uint32_t HashStr(char* Kwd, uint32_t KwdLen)
{
    uint32_t h = 0;
    uint32_t g;
    char*    KwdEnd = Kwd + KwdLen;

    while (Kwd < KwdEnd) {
        h = (h << 4) + *Kwd++;

        if ((g = (h & 0xF0000000))) {
            h = h ^ (g >> 24);
            h = h ^ g;
        }
    }
    return h;
}

// 检查关键词是否在hashtable中存在,注意: Kwd不一定是字符串，不能用字符串处理函数
// 返回关键词的类型，返回-1表示该词不存在。
static int ISWord(void* DIC, char* Kwd, uint32_t KwdLen)
{
    uint32_t hashvalue;
    int      tabindex;
    KWD_T*   kwdp;
    char     word[MAX_KWD_LEN + 1];
    int      codetype;

    KWD_TABLE_T* dic = (KWD_TABLE_T*)DIC;

    if (!dic || !Kwd || !KwdLen)
        return -1;

    memcpy(word, Kwd, KwdLen);
    word[KwdLen] = 0;

    codetype     = dic->codetype;
    if (codetype == CODETYPE_GBK) {
        gbkqj2bj(word, word);
        gbkqj2bjext(word, word);
    } else if (codetype == CODETYPE_UTF8)
        utf8qj2bj(word, word);
    strtolower(word, strlen(word), word, codetype);

    KwdLen    = strlen(word);

    hashvalue = HashStr(word, KwdLen);
    tabindex  = hashvalue % dic->hashsize;
    kwdp      = dic->KwdTableHead + tabindex;

    while (1) {
        if (kwdp->offset == -1)
            return -1;

        if (memcmp(word, dic->kwdpool.p + kwdp->offset, KwdLen) == 0)
            return kwdp->type;

        /* check collision */
        if (kwdp->coloffset == -1) {
            /* no collision */
            return -1;
        } else {
            /* collision */
            kwdp = dic->ColKwdTable + kwdp->coloffset;
        }
    }
}

#define GBK_REDUCE_CHAR_LEN(str, len, rlen)      \
    {                                            \
        int     af   = -1;                       \
        int     slen = len;                      \
        uint8_t ch;                              \
        rlen = 0;                                \
        while (slen > 0) {                       \
            ch = str[slen - 1];                  \
            if (ch < 0x80) {                     \
                if (af == -1) {                  \
                    af = 0;                      \
                    slen--;                      \
                    rlen++;                      \
                } else if (af == 0 || af == 1)   \
                    break;                       \
            } else {                             \
                if (af == -1) {                  \
                    af = 1;                      \
                    slen--;                      \
                    rlen++;                      \
                } else if (af == 0 || af == 1) { \
                    rlen++;                      \
                    break;                       \
                }                                \
            }                                    \
        }                                        \
    }

#define UTF8_REDUCE_CHAR_LEN(str, len, rlen) \
    {                                        \
        int     slen = len;                  \
        uint8_t ch;                          \
        rlen = 0;                            \
        while (slen > 0) {                   \
            ch = str[slen - 1];              \
            if (ch < 0x80) {                 \
                rlen++;                      \
                break;                       \
            } else if (ch & 0x40) {          \
                rlen++;                      \
                break;                       \
            } else {                         \
                rlen++;                      \
                slen--;                      \
            }                                \
        }                                    \
    }

/*
@ 功能：切词。

@ 输入参数：
  cstr: cut string，需要切分的字符串。
  word:用来存放关键词，多个关键词之间用空格连接。

@ 返回结果：
    -1:出现错误。
    n>=0: word数，是识别出来的word数。

@ 分词原理:
  采用正向最大匹配算法，词库采用hash存储与查找算法。
*/
static int CutWord(void* DIC, char* cstr, char* word)
{
    KWD_TABLE_T* dic = (KWD_TABLE_T*)DIC;
    int          cstrlen, left, right, wordidx, wordlen, wordcnt, word_type, res;
    char*        pword;
    int          spflag;  // word存储空间当前是否有空格标志
    int          codetype;
    int          rlen;

    if (!dic || !cstr || !word)
        return 0;

    wordidx  = 0;
    pword    = NULL;
    wordcnt  = 0;
    cstrlen  = strlen(cstr);
    spflag   = 1;
    codetype = dic->codetype;

    for (left = 0; left < cstrlen; left++) {
    NEXT_LEFT:
        for (right = cstrlen; right > left; right--) {
            pword   = cstr + left;
            wordlen = right - left;

            // 检查切分片段是否超过最大长度
            if (wordlen > dic->kwd_max_len || wordlen > MAX_KWD_LEN) {
                if (codetype == CODETYPE_GBK)
                    GBK_REDUCE_CHAR_LEN(pword, wordlen, rlen)
                else if (codetype == CODETYPE_UTF8)
                    UTF8_REDUCE_CHAR_LEN(pword, wordlen, rlen)

                right -= (rlen - 1);
                continue;
            }

            word_type = ISWord(dic, pword, wordlen);
            if (word_type >= 0) {
                if (!spflag) {
                    word[wordidx] = ' ';
                    wordidx++;
                    spflag = 1;
                }

                memcpy(word + wordidx, pword, wordlen);
                wordidx += wordlen;
                res = sprintf(word + wordidx, "^%d ", word_type);
                wordidx += res;
                spflag = 1;

                wordcnt++;

                left = right;
                goto NEXT_LEFT;
            } else  // 减少一个字符
            {
                if (codetype == CODETYPE_GBK)
                    GBK_REDUCE_CHAR_LEN(pword, wordlen, rlen)
                else if (codetype == CODETYPE_UTF8)
                    UTF8_REDUCE_CHAR_LEN(pword, wordlen, rlen)

                right -= (rlen - 1);
                continue;
            }
        }

        // 把切分片段的开始字符保存到word中并移动下一个字符
        if (right <= left && cstr[left] != 0) {
            int clen;

            if (codetype == CODETYPE_GBK)
                clen = GBKCHLEN((uint8_t)cstr[left]);
            else if (codetype == CODETYPE_UTF8)
                clen = UTF8CHLEN((uint8_t)cstr[left]);

            memcpy(word + wordidx, cstr + left, clen);
            wordidx += clen;
            spflag = 0;
            left += (clen - 1);
        }
    }

    if (spflag)
        word[wordidx - 1] = 0;
    else
        word[wordidx] = 0;

    return wordcnt;
}

#ifdef TEST_RECOGNIZER
int main(int argc, char* argv[])
{
    void* dic;
    char  str[1024], words[1024];

    if (argc != 4) {
        printf("arguments set error.\n");
        return -1;
    }

    dic = recognizer_init(argv[1], argv[2], RECOGNIZER_INIT_TYPE_CREATE, atoi(argv[3]), 1);
    if (!dic) {
        printf("recognizer_init() error.\n");
        return -1;
    }

    while (fgets(str, 1024, stdin)) {
        trim(str);
        if (strcasecmp(str, "exit") == 0)
            break;

        if (recognizer_words(dic, str, words) > 0) {
            printf("words: %s\n", words);
        } else
            break;
    }

    recognizer_free(dic);
    return 0;
}
#endif
