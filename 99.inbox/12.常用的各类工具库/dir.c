/*
 +----------------------------------------------------------------------+
 | Author: Xingzhi Liu  <dudubird2006@163.com>                          |
 +----------------------------------------------------------------------+
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include "windows.h"

struct _win32_dir {
    WIN32_FIND_DATA FindFileData;
    HANDLE          hFind;
    int             isfirst;
};

void* _opendir(const char* dir)
{
    struct _win32_dir* dirdata;
    char               DirSpec[MAX_PATH + 1];

    dirdata = (struct _win32_dir*)calloc(1, sizeof(struct _win32_dir));
    if (!dirdata)
        return NULL;
    dirdata->hFind = INVALID_HANDLE_VALUE;

    if (dir[strlen(dir) - 1] == '\\')
        _snprintf(DirSpec, MAX_PATH + 1, "%s*", dir);
    else
        _snprintf(DirSpec, MAX_PATH + 1, "%s\\*", dir);

    dirdata->hFind = FindFirstFile(DirSpec, &(dirdata->FindFileData));
    if (dirdata->hFind == INVALID_HANDLE_VALUE) {
        free(dirdata);
        return NULL;
    }

    dirdata->isfirst = 1;

    return dirdata;
}

// 依次返回指定目录下所有文件名
const char* _readdir(void* vp)
{
    struct _win32_dir* dirdata = (struct _win32_dir*)vp;

    if (dirdata->isfirst) {
        dirdata->isfirst = 0;
        if (dirdata->FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)  // 找到文件
            return dirdata->FindFileData.cFileName;
    }

    while (FindNextFile(dirdata->hFind, &(dirdata->FindFileData)) != 0) {
        if (dirdata->FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)  // 找到文件
            return dirdata->FindFileData.cFileName;
    }

    return NULL;
}

void _closedir(void* vp)
{
    struct _win32_dir* dirdata = (struct _win32_dir*)vp;
    FindClose(dirdata->hFind);
    free(vp);
}

#else

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include "xstring.h"
#include "comn.h"

void* _opendir(const char* dir)
{
    DIR* dp;

    dp = opendir(dir);
    if (dp == NULL)
        return NULL;

    return dp;
}

// 注意：目录名也会返回，因为结构体dirent中的d_type项指明了是文件还是目录，但是d_type某些系统不支持。
const char* _readdir(void* vp)
{
    DIR*           dp = (DIR*)vp;
    struct dirent* pdirent;

    while ((pdirent = readdir(dp)) != NULL) {
        if (strcmp(pdirent->d_name, ".") != 0 && strcmp(pdirent->d_name, "..") != 0)
            return pdirent->d_name;
    }

    return NULL;
}

// prefix和suffix可以为空, prefix和suffix是逻辑与的关系
const char* _readdirft(void* vp, char** prefixpp, int prefixnum, char** suffixpp, int suffixnum)
{
    char* filename;
    int   i;

    while ((filename = (char*)_readdir(vp)) != NULL) {
        for (i = 0; i < prefixnum; i++) {
            if (strstartwith(prefixpp[i], filename))
                break;
        }

        if (i && i == prefixnum)
            continue;

        for (i = 0; i < suffixnum; i++) {
            if (strendwith(suffixpp[i], filename))
                break;
        }

        if (i && i == suffixnum)
            continue;

        return filename;
    }

    return NULL;
}

void _rewinddir(void* vp)
{
    DIR* dp = (DIR*)vp;

    rewinddir(dp);
}

void _closedir(void* vp)
{
    DIR* dp = (DIR*)vp;

    closedir(dp);
}

//@attention: path如果是目录则最后为'/',最后没有'/'则当作文件路径
int mkdir_inpath(const char* path)
{
    char  p[1024];
    char *b, *e;

    snprintf(p, 1024, "%s", path);
    trim(p);
    b = p;
    if (b[0] == '/')
        b++;

    while ((e = strchr(b, '/'))) {
        *e = 0;
        if (!DIRISEXIST(p))
            if (mkdir(p, S_IRWXU | S_IRWXG) < 0)  // 目录权限: 用户和组: rwx  drwxr-x---
                return -1;

        *e = '/';
        e++;
        b = e;
    }

    return 0;
}

#endif

#ifdef TEST_DIR
#include "assert.h"

int main()
{
    void* vp;
    char* dir = " /usr/local/dirtest/ ";
    int   i;
    char  path[1024], *fn;
    char* suffix[] = {"3"};

    assert(i == 0);

    if (mkdir_inpath(dir) < 0) {
        printf("mkdir error.\n");
        return 1;
    }

    for (i = 0; i < 20; i++) {
        snprintf(path, sizeof(path), "/usr/local/dirtest/testdir%d/", i);
        if (mkdir_inpath(path) < 0) {
            printf("mkdir %s error.\n", path);
            return 1;
        }
    }

    vp = _opendir("/usr/local/dirtest/");
    if (!vp) {
        printf("can't open dir\n");
        return 1;
    }

    while ((fn = (char*)_readdir(vp))) {
        printf("%s\n", fn);
    }

    printf("filter dir: \n");
    _rewinddir(vp);  // 注意: 第二次扫描时需要重置目录
    while ((fn = (char*)_readdirft(vp, NULL, 0, suffix, 1))) {
        printf("%s\n", fn);
    }

    _closedir(vp);
    return 0;
}

#endif
