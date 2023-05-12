#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* 采用的二叉树(兄弟孩子)数据结构 */
/* 注意：这些函数不支持xml数据中有多个同名的元素的情况。*/
#ifndef TRUE
#define TRUE  1
#define FALSE (!(TRUE))
#endif

#define MAX_ELEMENT_ATTR_NUM 32

typedef struct xml_attr xml_attr_t;

struct xml_attr {
    char* attr_name;
    char* attr_value;
};

typedef struct xml_emt {
    char*           emt_name;
    char*           emt_value;
    int             attr_num;
    xml_attr_t      attr[MAX_ELEMENT_ATTR_NUM];
    struct xml_emt* pnext;        /* 兄弟元素 */
    struct xml_emt* pfirst_child; /* 子元素 */
    struct xml_emt* pparent;      /* 父元素 */
} xml_emt_t;                      /* emt := EleMenT */

static xml_emt_t* in_get_next_node(const char* xml_buf, char** node_start, char** node_end);
static char*      in_import_node(xml_emt_t* pparent, const char* xml_buf);
static int        in_xml_export(xml_emt_t* pxml, char* buf, int max_size, int level);
static int        in_xml_export_file(xml_emt_t* pxml, FILE* fp, int level);
static void       trim(char* str);

/* xml_emt_t * xml_new(const char *root_name, const char *root_value);
 * 功能：
 *      创建新的XML root元素，注意：在意外的时候释放内存。
 * 参数：
 *      root_name:ROOT元素的名称.
 *      root_value:ROOT元素的值.
 * Returns:
 *       成功返回建立的XML树,失败返回NULL
 */
xml_emt_t* xml_new(const char* root_name, const char* root_value)
{
    xml_emt_t* pxml;

    pxml = (xml_emt_t*)calloc(1, sizeof(xml_emt_t));
    if (pxml == NULL) {
        return pxml;
    }
    pxml->pnext        = NULL;
    pxml->pfirst_child = NULL;
    pxml->pparent      = NULL;
    pxml->attr_num     = 0;

    if (root_name)
        pxml->emt_name = strdup(root_name);
    else
        pxml->emt_name = NULL;

    if (root_value && root_name)
        pxml->emt_value = strdup(root_value);
    else
        pxml->emt_value = NULL;

    return pxml;
}

/**
 */
static xml_emt_t* in_get_next_node(const char* xml_buf, char** node_start, char** node_end)
{
    xml_emt_t* pxml;
    char *     pstart, *pend, *pattr;
    int        len;

    if (xml_buf == NULL)
        return NULL;

    pstart = strchr(xml_buf, '<');
    if (pstart == NULL)
        return NULL;
    *node_start = pstart;

    if (memcmp(pstart, "<?", 2) == 0) {
        /*
         * 处理指令
         */
        pstart = strstr(pstart + 2, "?>");
        if (pstart == NULL)
            return NULL;

        pstart = strchr(pstart + 2, '<');
        if (pstart == NULL)
            return NULL;
    }

    if (memcmp(pstart, "<!--", 4) == 0) {
        /*
         * 注释
         */
        pstart = strstr(pstart + 4, "-->");
        if (pstart == NULL)
            return NULL;

        pstart = strchr(pstart + 2, '<');
        if (pstart == NULL)
            return NULL;
    }

    pstart++;
    pend = strchr(pstart, '>');
    if (pend == NULL)
        return NULL;
    *node_end = pend + 1;

    pxml               = (xml_emt_t*)calloc(1, sizeof(xml_emt_t));
    pxml->emt_name     = NULL;
    pxml->emt_value    = NULL;
    pxml->attr_num     = 0;
    pxml->pnext        = NULL;
    pxml->pfirst_child = NULL;
    pxml->pparent      = NULL;

    pattr = strchr(pstart, ' ');
    if ((pattr == NULL) || (pattr > pend))
        pattr = pend;

    len            = pattr - pstart;
    pxml->emt_name = (char*)calloc(1, len + 1);
    memcpy(pxml->emt_name, pstart, len);
    trim(pxml->emt_name); /* xinz+ */

    if (pxml->emt_name[0] == '/')
        return pxml;

    while (pattr < pend) {
        pstart = pattr;
        pattr  = strstr(pstart, "=\"");
        if (pattr == NULL)
            break;

        /*
         * 属性名称
         */
        len                                  = pattr - pstart;
        pxml->attr[pxml->attr_num].attr_name = (char*)calloc(1, len + 1);
        memcpy(pxml->attr[pxml->attr_num].attr_name, pstart, len);
        trim(pxml->attr[pxml->attr_num].attr_name); /* xinz+ */

        /*
         * 属性值
         */
        pstart = pattr + 2;
        pattr  = strchr(pstart, '"');
        if (pattr == NULL)
            break;
        len                                   = pattr - pstart;
        pxml->attr[pxml->attr_num].attr_value = (char*)calloc(1, len + 1);
        memcpy(pxml->attr[pxml->attr_num].attr_value, pstart, len);
        trim(pxml->attr[pxml->attr_num].attr_value); /* xinz+ */

        pxml->attr_num++;
        if (pxml->attr_num >= MAX_ELEMENT_ATTR_NUM)
            break;
        pattr++;
    }
    return pxml;
}

/*
 */
static char* in_import_node(xml_emt_t* pparent, const char* xml_buf)
{
    xml_emt_t *pxml, *pnext_xml;
    char *     node_start, *node_end;
    int        len;

    pxml     = pparent;
    node_end = (char*)xml_buf;
    while (TRUE) {
        xml_buf   = node_end;
        pnext_xml = in_get_next_node(xml_buf, &node_start, &node_end);
        if (pnext_xml == NULL)
            return NULL;
        len = node_start - xml_buf;
        if ((pparent->emt_value == NULL) && (len > 0)) {
            /*
             * 获取节点的值
             */
            pparent->emt_value = (char*)calloc(1, len + 1);
            memcpy(pparent->emt_value, xml_buf, len);
            trim(pparent->emt_value); /* xinz */
        }
        if (pnext_xml->emt_name[0] == '/') {
            free(pnext_xml->emt_name);
            free(pnext_xml);
            return node_end;
        } else {
            if (pparent->pfirst_child == NULL) {
                pparent->pfirst_child = pnext_xml;
            } else {
                pxml = pparent->pfirst_child;
                while (pxml->pnext)
                    pxml = pxml->pnext;
                pxml->pnext = pnext_xml;
            }
            node_end = in_import_node(pnext_xml, node_end);
        }
    }
}

/** xml_emt_t * xml_import(const char *xml_buf);
 * 功能:
 *      导入XML格式数据到XML结构。
 * 参数：
 *      xml_buf:包含XML数据的缓冲区。
 * Returns:
 *       成功返回建立的XML树,失败返回NULL。
 */
xml_emt_t* xml_import(const char* xml_buf)
{
    xml_emt_t* pxml;
    char *     node_start, *node_end;

    pxml = in_get_next_node(xml_buf, &node_start, &node_end);
    if (pxml == NULL)
        return NULL;

    in_import_node(pxml, node_end);
    return pxml;
}

/**xml_emt_t * xml_import_from_file(const char *xml_file);
 * 功能：
 *     导入XML文件内容到XML结构
 * 参数：
 *     xml_file:包含XML数据的文件名称.
 * Returns:
 *     成功返回建立的XML树,失败返回NULL
 */
xml_emt_t* xml_import_from_file(const char* xml_file)
{
    FILE*      fp;
    long       size;
    char*      buf;
    xml_emt_t* pxml;
    /*
    size = file_get_size(xml_file);
    if(size <= 0)
    return NULL;
    */
    fp = fopen(xml_file, "r");
    if (fp == NULL) {
        return NULL;
    }

    /* get file size, xinz+*/
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    if (size <= 0) {
        return NULL;
    }
    fseek(fp, 0, SEEK_SET);

    buf = (char*)calloc(1, size + 1);
    fread(buf, size, 1, fp);
    fclose(fp);

    pxml = xml_import(buf);
    free(buf);

    return pxml;
}

/*
 */
static int in_xml_export(xml_emt_t* pxml, char* buf, int max_size, int level)
{
    int   i, len;
    char* pstart = buf;
    if (pxml == NULL)
        return 0;

    max_size -= level;
    if (max_size <= 0)
        return -1;
    for (i = 0; i < level; i++)
        strcat(pstart, "\t");
    pstart += i;

    max_size -= 1;
    if (max_size <= 0)
        return -1;
    strcat(pstart, "<");
    pstart++;

    len = strlen(pxml->emt_name);
    max_size -= len;
    if (max_size <= 0)
        return -1;
    strcat(pstart, pxml->emt_name);
    pstart += len;

    for (i = 0; i < pxml->attr_num; i++) {
        len = strlen(pxml->attr[i].attr_name) + strlen(pxml->attr[i].attr_value) + 6;
        max_size -= len;
        if (max_size <= 0)
            return -1;
        strcat(pstart, " ");
        strcat(pstart, pxml->attr[i].attr_name);
        strcat(pstart, " = \"");
        strcat(pstart, pxml->attr[i].attr_value);
        strcat(pstart, "\"");
        pstart += len;
    }
    max_size -= 1;
    if (max_size <= 0)
        return -1;
    strcat(pstart, ">");
    pstart++;

    if (pxml->emt_value) {
        len = strlen(pxml->emt_value);
        max_size -= len;
        if (max_size <= 0)
            return -1;
        strcat(pstart, pxml->emt_value);
        pstart += len;
    }

    if (pxml->pfirst_child) {
        max_size -= 1;
        if (max_size <= 0)
            return -1;
        strcat(pstart, "\n");
        pstart++;

        in_xml_export(pxml->pfirst_child, pstart, max_size, level + 1);

        max_size -= 1;
        if (max_size <= 0)
            return -1;
        strcat(pstart, "\n");
        pstart++;

        max_size -= level;
        if (max_size <= 0)
            return -1;
        for (i = 0; i < level; i++)
            strcat(pstart, "\t");
        pstart += i;
    }

    max_size -= 2;
    if (max_size <= 0)
        return -1;
    strcat(pstart, "</");

    len = strlen(pxml->emt_name);
    max_size -= len;
    if (max_size <= 0)
        return -1;
    strcat(pstart, pxml->emt_name);
    pstart += len;

    max_size -= 1;
    if (max_size <= 0)
        return -1;
    strcat(pstart, ">");

    if (pxml->pnext) {
        max_size -= 1;
        if (max_size <= 0)
            return -1;
        strcat(pstart, "\n");
        pstart++;
        in_xml_export(pxml->pnext, pstart, max_size, level);
    }
    return 0;
}

/**int xml_export(xml_emt_t *pxml, char *buf, int max_size)
 * 功能：
 * 导出XML树到缓冲区。
 * 参数：
 *     pxml:包含数据的XML树.
 *     buf: 存放数据的缓存
 *     max_size :数据的最大空间
 * Returns:
 *     成功返回0，失败返回-1
 */
int xml_export(xml_emt_t* pxml, char* buf, int max_size)
{
    return in_xml_export(pxml, buf, max_size, 0);
}

static int in_xml_export_file(xml_emt_t* pxml, FILE* fp, int level)
{
    int i;

    if (pxml == NULL)
        return 0;
    for (i = 0; i < level; i++)
        fprintf(fp, "\t");
    fprintf(fp, "<%s", pxml->emt_name);
    for (i = 0; i < pxml->attr_num; i++)
        fprintf(fp, " %s = \"%s\"", pxml->attr[i].attr_name, pxml->attr[i].attr_value);
    if (pxml->emt_value)
        fprintf(fp, ">%s", pxml->emt_value);
    else
        fprintf(fp, ">");
    if (pxml->pfirst_child) {
        fprintf(fp, "\n");
        in_xml_export_file(pxml->pfirst_child, fp, level + 1);
        fprintf(fp, "\n");
        for (i = 0; i < level; i++)
            fprintf(fp, "\t");
    }
    fprintf(fp, "</%s>", pxml->emt_name);
    if (pxml->pnext) {
        fprintf(fp, "\n");
        in_xml_export_file(pxml->pnext, fp, level);
    }
    return 0;
}

/**int xml_export_to_file(xml_emt_t *pxml, const char *xml_file);
 * 功能:
 *     导出XML树数据到文件。
 * 参数：
 *     pxml:包含数据的XML树.
 *     xml_file:保存数据的文件,如果为NULL则输出到stderr
 * Returns:
 *     成功返回0,失败返回-1
 */
int xml_export_to_file(xml_emt_t* pxml, const char* xml_file)
{
    FILE* fp;

    if (xml_file == NULL) {
        fp = stderr;
    } else {
        fp = fopen(xml_file, "w");
        if (fp == NULL) {
            return -1;
        }
    }
    in_xml_export_file(pxml, fp, 0);
    fprintf(fp, "\n");
    fflush(fp);
    if (xml_file != NULL)
        fclose(fp);
    return 0;
}

/**void xml_destroy(xml_emt_t * pxml);
 * 功能：
 *    释放XML树结构的内存空间。
 * 参数：
 *     pxml:包含数据的XML树。
 */
void xml_destroy(xml_emt_t* pxml)
{
    int i;

    if (pxml == NULL)
        return;

    if (pxml->pfirst_child)
        xml_destroy(pxml->pfirst_child);
    if (pxml->pnext)
        xml_destroy(pxml->pnext);

    for (i = 0; i < pxml->attr_num; i++) {
        if (pxml->attr[i].attr_name)
            free(pxml->attr[i].attr_name);
        if (pxml->attr[i].attr_value)
            free(pxml->attr[i].attr_value);
    }

    if (pxml->emt_name)
        free(pxml->emt_name);
    if (pxml->emt_value)
        free(pxml->emt_value);
    free(pxml);
}

/**const char * xml_select(xml_emt_t *pxml, const char *xpath);
 * 功能：
 *     按照路径获取节点（元素）的值。
 * 参数：
 *     pxml:包含数据的XML树.
 *     xpath:XML的路径比如 root/abc/def
 * Returns:
 *     成功返回节点（元素）的值，失败返回NULL
 */
const char* xml_select(xml_emt_t* pxml, const char* xpath)
{
    char* emt_name;
    char* pend;
    int   len;

    if (pxml == NULL)
        return NULL;

    pend = strchr(xpath, '/');
    if (pend == NULL) {
        if (strcmp(pxml->emt_name, xpath) == 0)
            return pxml->emt_value;
        return xml_select(pxml->pnext, xpath);
    }

    len      = pend - xpath;
    emt_name = (char*)calloc(1, len + 1);
    memcpy(emt_name, xpath, len);

    if (strcmp(pxml->emt_name, emt_name) == 0) {
        free(emt_name);
        return xml_select(pxml->pfirst_child, pend + 1);
    }
    while (pxml->pnext) {
        pxml = pxml->pnext;
        if (strcmp(pxml->emt_name, emt_name) == 0) {
            free(emt_name);
            return xml_select(pxml->pfirst_child, pend + 1);
        }
    }
    free(emt_name);
    return NULL;
}

const char* xml_select_n(xml_emt_t* pxml, const char* xpath, int n)
{
    char* emt_name;
    char* pend;
    int   len;

    if (pxml == NULL)
        return NULL;

    pend = strchr(xpath, '/');
    if (pend == NULL) {
        if (strcmp(pxml->emt_name, xpath) == 0) {
            if (n == 1)
                return pxml->emt_value;
        }

        n--;
        return xml_select_n(pxml->pnext, xpath, n);
    }

    len      = pend - xpath;
    emt_name = (char*)calloc(1, len + 1);
    memcpy(emt_name, xpath, len);

    if (strcmp(pxml->emt_name, emt_name) == 0) {
        free(emt_name);
        return xml_select_n(pxml->pfirst_child, pend + 1, n);
    }

    while (pxml->pnext) {
        pxml = pxml->pnext;
        if (strcmp(pxml->emt_name, emt_name) == 0) {
            free(emt_name);
            return xml_select_n(pxml->pfirst_child, pend + 1, n);
        }
    }

    free(emt_name);
    return NULL;
}

/**int xml_update(xml_emt_t *pxml, const char *xpath, const char *value);
 * 功能：
 *      按照路径更新节点（元素）的值。
 * 参数：
 *      pxml:包含数据的XML树
 *      xpath:XML的路径比如 root/abc/def
 *      value:节点（元素）的新值
 * Returns:
 *      成功返回0，失败返回-1
 */
int xml_update(xml_emt_t* pxml, const char* xpath, const char* value)
{
    char* emt_name;
    char* pend;
    int   len;

    if (pxml == NULL)
        return -1;

    pend = strchr(xpath, '/');
    if (pend == NULL) {
        if (strcmp(pxml->emt_name, xpath) == 0) {
            if (pxml->emt_value)
                free(pxml->emt_value);
            pxml->emt_value = strdup(value);
            return 0;
        }
        return xml_update(pxml->pnext, xpath, value);
    }

    len      = pend - xpath;
    emt_name = (char*)calloc(1, len + 1);
    memcpy(emt_name, xpath, len);

    if (strcmp(pxml->emt_name, emt_name) == 0) {
        free(emt_name);
        return xml_update(pxml->pfirst_child, pend + 1, value);
    }
    while (pxml->pnext) {
        pxml = pxml->pnext;
        if (strcmp(pxml->emt_name, emt_name) == 0) {
            free(emt_name);
            return xml_update(pxml->pfirst_child, pend + 1, value);
        }
    }
    free(emt_name);
    return -1;
}

/**int xml_delete(xml_emt_t *pxml, const char *xpath);
 * 功能：
 *     按照路径删除一个节点（元素）(TODO尚未实现该函数)。
 * 参数：
 *     pxml:包含数据的XML树.
 *     xpath:XML的路径比如 root/abc/def
 * Returns:
 *    成功返回0，失败返回-1
 */
int xml_delete(xml_emt_t* pxml, const char* xpath)
{
    return -1;
}

/**int xml_insert(xml_emt_t *pxml, const char *xpath, const char *value);
 * 功能：
 *     按照路径增加一个节点（元素）。
 *  参数：
 *     pxml:包含数据的XML树.
 *     xpath:XML的路径比如 root/abc/def
 *     value:要求插入的值
 * Returns:
 *     成功返回的0，失败返回-1
 */
int xml_insert(xml_emt_t* pxml, const char* xpath, const char* value)
{
    char* emt_name;
    char* pend;
    int   len;

    if (pxml == NULL)
        return -1;

    pend = strchr(xpath, '/');
    if (pend == NULL) {
        while (pxml->pnext)
            pxml = pxml->pnext;

        pxml->pnext           = (xml_emt_t*)calloc(1, sizeof(xml_emt_t));
        pxml->pnext->emt_name = strdup(xpath);
        if (value)
            pxml->pnext->emt_value = strdup(value);
        else
            pxml->pnext->emt_value = NULL;
        pxml->pnext->attr_num     = 0;
        pxml->pnext->pnext        = NULL;
        pxml->pnext->pfirst_child = NULL;
        pxml->pnext->pparent      = pxml->pparent;

        return 0;
    }

    len      = pend - xpath;
    emt_name = (char*)calloc(1, len + 1);
    memcpy(emt_name, xpath, len);
    while (pxml) {
        if (strcmp(pxml->emt_name, emt_name) == 0) {
            free(emt_name);
            if (pxml->pfirst_child)
                return xml_insert(pxml->pfirst_child, pend + 1, value);

            pend++;
            if (strchr(pend, '/') != NULL)
                return -1;

            pxml->pfirst_child           = (xml_emt_t*)calloc(1, sizeof(xml_emt_t));
            pxml->pfirst_child->emt_name = strdup(pend);
            if (value)
                pxml->pfirst_child->emt_value = strdup(value);
            else
                pxml->pfirst_child->emt_value = NULL;
            pxml->pfirst_child->attr_num     = 0;
            pxml->pfirst_child->pnext        = NULL;
            pxml->pfirst_child->pfirst_child = NULL;
            pxml->pfirst_child->pparent      = pxml;

            return 0;
        }
        pxml = pxml->pnext;
    }
    free(emt_name);
    return -1;
}

/**const char * xml_attr_select(xml_emt_t *pxml, const char *xpath, const char *attr_name);
 * 功能：
 *    按照路径获取节点（元素）的属性值。
 * 参数：
 *     pxml:包含数据的XML树.
 *     xpath:XML的路径比如 root/abc/def
 *     attr_name:属性的名称
 * Returns:
 *     成功返回节点（元素）的属性值，失败返回NULL
 */
const char* xml_attr_select(xml_emt_t* pxml, const char* xpath, const char* attr_name)
{
    char* emt_name;
    char* pend;
    int   len, i;

    if (pxml == NULL)
        return NULL;

    pend = strchr(xpath, '/');
    if (pend == NULL) {
        if (strcmp(pxml->emt_name, xpath) == 0) {
            for (i = 0; i < pxml->attr_num; i++) {
                if (strcmp(pxml->attr[i].attr_name, attr_name) == 0)
                    return pxml->attr[i].attr_value;
            }
            return NULL;
        }
        return xml_attr_select(pxml->pnext, xpath, attr_name);
    }

    len      = pend - xpath;
    emt_name = (char*)calloc(1, len + 1);
    memcpy(emt_name, xpath, len);

    if (strcmp(pxml->emt_name, emt_name) == 0) {
        free(emt_name);
        return xml_attr_select(pxml->pfirst_child, pend + 1, attr_name);
    }
    while (pxml->pnext) {
        pxml = pxml->pnext;
        if (strcmp(pxml->emt_name, emt_name) == 0) {
            free(emt_name);
            return xml_attr_select(pxml->pfirst_child, pend + 1, attr_name);
        }
    }
    free(emt_name);
    return NULL;
}

const char* xml_attr_select_n(xml_emt_t* pxml, const char* xpath, const char* attr_name, int n)
{
    char* emt_name;
    char* pend;
    int   len, i;

    if (pxml == NULL)
        return NULL;

    pend = strchr(xpath, '/');
    if (pend == NULL) {
        if (strcmp(pxml->emt_name, xpath) == 0) {
            if (n == 1) {
                for (i = 0; i < pxml->attr_num; i++) {
                    if (strcmp(pxml->attr[i].attr_name, attr_name) == 0)
                        return pxml->attr[i].attr_value;
                }
                return NULL;
            }
        }

        n--;
        return xml_attr_select_n(pxml->pnext, xpath, attr_name, n);
    }

    len      = pend - xpath;
    emt_name = (char*)calloc(1, len + 1);
    memcpy(emt_name, xpath, len);

    if (strcmp(pxml->emt_name, emt_name) == 0) {
        free(emt_name);
        return xml_attr_select_n(pxml->pfirst_child, pend + 1, attr_name, n);
    }

    while (pxml->pnext) {
        pxml = pxml->pnext;
        if (strcmp(pxml->emt_name, emt_name) == 0) {
            free(emt_name);
            return xml_attr_select_n(pxml->pfirst_child, pend + 1, attr_name, n);
        }
    }

    free(emt_name);
    return NULL;
}

/**int xml_attr_update(xml_emt_t *pxml, const char *xpath, const char *attr_name, const char *value);
* 功能：
*     按照路径更新节点（元素）属性的值。
参数：
*     pxml:包含数据的XML树.
*     xpath:XML的路径比如 root/abc/def
*     attr_name:属性的名称
*     value:属性的新值
* Returns:
*     成功返回0，失败返回-1
*/
int xml_attr_update(xml_emt_t* pxml, const char* xpath, const char* attr_name, const char* value)
{
    char* emt_name;
    char* pend;
    int   len, i;

    if (pxml == NULL)
        return -1;

    pend = strchr(xpath, '/');
    if (pend == NULL) {
        if (strcmp(pxml->emt_name, xpath) == 0) {
            for (i = 0; i < pxml->attr_num; i++) {
                if (strcmp(pxml->attr[i].attr_name, attr_name) == 0) {
                    if (pxml->attr[i].attr_value)
                        free(pxml->attr[i].attr_value);
                    pxml->attr[i].attr_value = strdup(value);
                    return 0;
                }
            }
            return -1;
        }
        return xml_attr_update(pxml->pnext, xpath, attr_name, value);
    }

    len      = pend - xpath;
    emt_name = (char*)calloc(1, len + 1);
    memcpy(emt_name, xpath, len);

    if (strcmp(pxml->emt_name, emt_name) == 0) {
        free(emt_name);
        return xml_attr_update(pxml->pfirst_child, pend + 1, attr_name, value);
    }
    while (pxml->pnext) {
        pxml = pxml->pnext;
        if (strcmp(pxml->emt_name, emt_name) == 0) {
            free(emt_name);
            return xml_attr_update(pxml->pfirst_child, pend + 1, attr_name, value);
        }
    }
    free(emt_name);
    return -1;
}

/**int xml_attr_delete(xml_emt_t *pxml, const char *xpath, const char *attr_name);
 * 功能：
 *     按照路径删除一个节点（元素）的属性(TODO尚未实现该函数)。
 * 参数：
 *      pxml:包含数据的XML树.
 *      xpath:XML的路径比如 root/abc/def
 *      attr_name:要求删除的属性
 * Returns:
 *      成功返回0，失败返回-1。
 */
int xml_attr_delete(xml_emt_t* pxml, const char* xpath, const char* attr_name)
{
    return -1;
}

/* int xml_attr_insert(xml_emt_t *pxml, const char *xpath, const char *attr_name, const char *value);
 * 功能：
 *     按照路径增加一个节点（元素）的属性。
 * 参数：
 *     pxml:包含数据的XML树。
 *     xpath:XML的路径比如 root/abc/def。
 *     attr_name:要求插入的属性。
 *     value:要求插入的值。
 * Returns:
 *     成功返回的0，失败返回-1
 */
int xml_attr_insert(xml_emt_t* pxml, const char* xpath, const char* attr_name, const char* value)
{
    char* emt_name;
    char* pend;
    int   len;

    if (pxml == NULL)
        return -1;

    pend = strchr(xpath, '/');
    if (pend == NULL) {
        if (strcmp(pxml->emt_name, xpath) == 0) {
            if (pxml->attr_num >= MAX_ELEMENT_ATTR_NUM)
                return -1;
            pxml->attr[pxml->attr_num].attr_name  = strdup(attr_name);
            pxml->attr[pxml->attr_num].attr_value = strdup(value);
            pxml->attr_num++;
            return 0;
        }
        return xml_attr_insert(pxml->pnext, xpath, attr_name, value);
    }

    len      = pend - xpath;
    emt_name = (char*)calloc(1, len + 1);
    memcpy(emt_name, xpath, len);

    if (strcmp(pxml->emt_name, emt_name) == 0) {
        free(emt_name);
        return xml_attr_insert(pxml->pfirst_child, pend + 1, attr_name, value);
    }
    while (pxml->pnext) {
        pxml = pxml->pnext;
        if (strcmp(pxml->emt_name, emt_name) == 0) {
            free(emt_name);
            return xml_attr_insert(pxml->pfirst_child, pend + 1, attr_name, value);
        }
    }
    free(emt_name);
    return -1;
}

/* xinz +
 */
static void trim(char* str)
{
    int   pos  = 0;
    char* dest = str;

    /* skip leading blanks */
    while (str[pos] <= ' ' && str[pos] > 0) {
        pos++;
    }

    while (str[pos]) {
        *(dest++) = str[pos];
        pos++;
    }

    *(dest--) = '\0'; /* store the null */

    /* remove trailing blanks */
    while (dest >= str && *dest <= ' ' && *dest > 0) {
        *(dest--) = '\0';
    }
}

#ifdef TEST_XML

int main(int argc, char** argv)
{
    xml_emt_t*  pstruXml;
    char        szBuffer[8192];
    char        szTemp[1024], szTemp2[1024];
    const char* pszValue;
    char        szNewValue[256];

    if (argc == 1) {
        fprintf(stderr, "Input an xml file:");
        scanf("%s", szTemp);

        pstruXml = xml_import_from_file(szTemp);
        if (pstruXml == NULL)
            return 0;

        memset(szBuffer, 0, sizeof(szBuffer));
        xml_export(pstruXml, szBuffer, sizeof(szBuffer));
        xml_export_to_file(pstruXml, NULL);

        fprintf(stderr, "\n\nInput an xpath:");
        scanf("%s", szTemp);

        pszValue = xml_select(pstruXml, szTemp);
        pszValue = xml_select_n(pstruXml, szTemp, 3);
        pszValue = xml_attr_select_n(pstruXml, szTemp, "TABLE_ID", 3);
        pszValue = xml_select_n(pstruXml, szTemp, 2);
        pszValue = xml_attr_select_n(pstruXml, szTemp, "TABLE_ID", 2);
        pszValue = xml_select_n(pstruXml, szTemp, 1);
        pszValue = xml_attr_select_n(pstruXml, szTemp, "TABLE_ID", 1);
        pszValue = xml_select_n(pstruXml, szTemp, 4);
        pszValue = xml_attr_select_n(pstruXml, szTemp, "TABLE_ID", 4);
        if (pszValue) {
            fprintf(stderr, "Old Value:%s\n", pszValue);
            fprintf(stderr, "Input new value:");
            scanf("%s", szNewValue);
            xml_update(pstruXml, szTemp, szNewValue);

            fprintf(stderr, "Input an attribute:");
            scanf("%s", szTemp2);
            pszValue = xml_attr_select(pstruXml, szTemp, szTemp2);
            if (pszValue) {
                fprintf(stderr, "Old Value:%s\n", pszValue);
                fprintf(stderr, "Input new Value:");
                scanf("%s", szNewValue);
                xml_attr_update(pstruXml, szTemp, szTemp2, szNewValue);
            }
            xml_export_to_file(pstruXml, NULL);
        }

        xml_destroy(pstruXml);
    }

    pstruXml = xml_new("test", NULL);
    xml_insert(pstruXml, "test/first", "first");
    xml_insert(pstruXml, "test/second", "second");
    xml_insert(pstruXml, "test/3", NULL);
    xml_insert(pstruXml, "test/3/1", "1");
    xml_insert(pstruXml, "test/3/2", NULL);
    xml_insert(pstruXml, "test/3/2/1", "L");
    xml_insert(pstruXml, "test/third", "third");
    xml_insert(pstruXml, "test/second/good", "good");
    xml_insert(pstruXml, "test/good", "good");
    xml_attr_insert(pstruXml, "test", "version", "1.0");
    xml_export_to_file(pstruXml, NULL);
    xml_destroy(pstruXml);

    return 0;
}

#endif
