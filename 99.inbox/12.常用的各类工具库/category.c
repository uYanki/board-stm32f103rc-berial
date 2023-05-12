/* Copyright (c) 2008 Liu xing zhi. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

#include "category.h"
#include "utils.h"
#include "xmalloc.h"
#include "xstring.h"

/* 分类编码说明:
   每一级分类编码从1开始，编码0保留。
   第一级编码从10开始，编码0-9保留。
   每一级分类编码尽量采用连续编码，这样可以节省内存。
   分类ID格式: Level1Level2...，例如001,012,120第三级分类编码。
   分类NAME-ID map table文件格式: ID:NAME[:SEAECH_CONTENT]\n
   例如分类ID: 001012120,是由三个单元unit ID 组成的。
*/

#define CATEGORY_UNIT_MIN_VALUE -1
#define CATEGORY_NOT_UNIT_ID    0

#define CATEGORY_CHAIN_END      0
#define CATEGORY_POOL_ALLOC_MEM 20
#define CATEGORY_ID_MAX_LEN     256

#define return_error_msg(MSG)                                            \
    {                                                                    \
        if (error_msg)                                                   \
            snprintf(error_msg, error_size, "Line%d: %s", line_no, MSG); \
        return NULL;                                                     \
    }

/** 参数说明:
    category_map: 分类NAME - ID map table 文件路径。
    category_level: 分类tree的层数。
    category_id_unit_len:分类id单元的长度，比如分类id=001012002 id unit length=3。
    注意:这个函数使用了realloc函数，这个函数对新增的内存不会像calloc那样初始化为0
*/
struct category_tree* category_tree_init(char* category_map, int category_level, int category_id_unit_len, char* error_msg, int error_size)
{
    struct category_tree* tree;
    struct category_pool *pool, *parent_pool;
    struct category *     category, *parent, *tmp;
    int                   i, len;
    int                   line_len, line_no, level;
    int                   self_unit_id, parent_unit_id;
    char *                map, *line_buf_p, *name, *id, *search_content;
    char *                beg, *end;

    if (!category_map || category_level <= 0 || category_id_unit_len <= 0)
        return NULL;

    tree           = (struct category_tree*)xcalloc(1, sizeof(struct category_tree));
    tree->level    = category_level;
    tree->unit_len = category_id_unit_len;
    tree->p        = (struct category_pool*)xcalloc(1, sizeof(struct category_pool) * tree->level);
    for (i = 0; i < tree->level; i++) {
        pool       = tree->p + i;
        pool->p    = (struct category*)xcalloc(1, sizeof(struct category) * CATEGORY_POOL_ALLOC_MEM);
        pool->size = CATEGORY_POOL_ALLOC_MEM;
    }

    map = loadfile(category_map, NULL);
    if (!map)
        return NULL;

    line_buf_p = map;
    line_no    = 1;
    while ((beg = getaline(line_buf_p, "\n", &line_len)) != NULL) {
        trim(beg);
        if (*beg == 0 || *beg == '#')
            goto next_line;
        id  = beg;
        end = strchr(id, ':');
        if (!end)
            return_error_msg("missing ':'");
        *end = 0;
        trim(id);
        if (*id == 0)
            return_error_msg("missing category id");

        name = end + 1;
        end  = strchr(name, ':');
        if (!end) {
            trim(name);
            if (*name == 0)
                return_error_msg("missing category name");

            search_content = name;
        } else {
            *end = 0;
            trim(name);
            if (*name == 0)
                return_error_msg("missing category name");

            search_content = end + 1;
            trim(search_content);
            if (*search_content == 0)
                return_error_msg("missing category search content");
        }

        self_unit_id = category_get_unit_id(id, CATEGORY_UNIT_ID_SELF, category_id_unit_len, category_level);
        if (self_unit_id == -1)
            return_error_msg("category id syntax error");
        parent_unit_id = category_get_unit_id(id, CATEGORY_UNIT_ID_PARENT, category_id_unit_len, category_level);
        if (parent_unit_id == -1)
            return_error_msg("category id syntax error");
        len   = strlen(id);
        level = len / category_id_unit_len;
        if (level > category_level)
            return_error_msg("category id's level exceed the tree's level");
        pool = tree->p + (level - 1);
    INSERT_POOL:
        if (self_unit_id <= pool->size - 1) {
            category = pool->p + self_unit_id;
            if (category->unit_id != 0)
                return_error_msg("category space in pool is occupied");
            category->unit_id             = self_unit_id;
            category->data.name           = xstrdup(name);
            category->data.search_content = xstrdup(search_content);
            category->data.result_num     = -1;
            category->data.cid            = xstrdup(id);
            category->parent_id           = parent_unit_id;
            /*不要设置以下这些，可能该分类的子分类已经设置了这些了
            category->first_child_id = 0;
            category->last_child_id = 0;
            category->child_num = 0;

            category->brother_id = 0;*/
            if (parent_unit_id != 0 && level != 1) {
                parent_pool = tree->p + (level - 1 - 1);
            INSERT_PARENT_POOL:
                if (parent_unit_id <= parent_pool->size - 1) {
                    // 可能该分类的父分类在pool中还不存在
                    parent = parent_pool->p + parent_unit_id;
                    if (parent->child_num == 0 && parent->first_child_id == 0 && parent->last_child_id == 0) {
                        parent->first_child_id = self_unit_id;
                        parent->last_child_id  = self_unit_id;
                    } else if (parent->child_num && parent->first_child_id && parent->last_child_id) {
                        category = pool->p + parent->last_child_id;
                        if (category->brother_id)
                            return_error_msg("category's brother is too many");

                        category->brother_id  = self_unit_id;
                        parent->last_child_id = self_unit_id;
                    } else
                        return_error_msg("Father category's child number error");

                    parent->child_num++;
                } else {
                    tmp = (struct category*)xcalloc(1, sizeof(struct category) * (parent_pool->size + CATEGORY_POOL_ALLOC_MEM));
                    memcpy(tmp, parent_pool->p, sizeof(struct category) * parent_pool->size);
                    parent_pool->p = tmp;
                    parent_pool->size += CATEGORY_POOL_ALLOC_MEM;
                    // parent_pool->p =(struct category *)xrealloc(parent_pool->p,sizeof(struct category) * (parent_pool->size + CATEGORY_POOL_ALLOC_MEM));
                    goto INSERT_PARENT_POOL;
                }
            }
        } else {
            tmp = (struct category*)xcalloc(1, sizeof(struct category) * (pool->size + CATEGORY_POOL_ALLOC_MEM));
            memcpy(tmp, pool->p, sizeof(struct category) * pool->size);
            pool->p = tmp;
            pool->size += CATEGORY_POOL_ALLOC_MEM;
            // pool->p =(struct category *)xrealloc(pool->p,sizeof(struct category) * (pool->size + CATEGORY_POOL_ALLOC_MEM));

            goto INSERT_POOL;
        }

    next_line:
        line_buf_p = line_buf_p + line_len + 1;
        line_no++;
    }

    xfree(map);
    return tree;
}

/** 参数说明:
    id: 分类ID，例如: 023010001
    unit: 需要获得哪一级单元ID，例如第一级单元ID=023
    unit_len: 分类ID长度，例如unit_len=3
    level: 整个分类是几级分类，例如3级分类
    返回值:
    分类ID错误: -1
    没有相应的unitID: 0
    正常情况下返回unitID
*/
int category_get_unit_id(char* id, int unit, int unit_len, int level)
{
    char  temp[CATEGORY_ID_MAX_LEN + 1];
    int   len, unit_id;
    char* p;

    // 检查ID串的语法:长度、组成字符
    len = strlen(id);
    if (len > CATEGORY_ID_MAX_LEN)
        return -1;
    if (len > unit_len * level)
        return -1;
    if (len % unit_len != 0)
        return -1;

    p = id;
    while (isdigit(*p)) {
        p++;
    }
    if (*p != 0)
        return -1;

    // 检查是否包含指定单元的ID
    if (unit < CATEGORY_UNIT_MIN_VALUE)
        return -1;
    if (unit == CATEGORY_UNIT_ID_PARENT && len < unit_len * 2)
        return CATEGORY_NOT_UNIT_ID;
    if (unit > 0 && len < unit_len * unit)
        return CATEGORY_NOT_UNIT_ID;

    strcpy(temp, id);

    switch (unit) {
        case CATEGORY_UNIT_ID_SELF:
            p       = temp + len - unit_len;
            unit_id = atoi(p);
            break;
        case CATEGORY_UNIT_ID_PARENT:
            p       = temp + len - unit_len;
            *p      = 0;
            p       = p - unit_len;
            unit_id = atoi(p);
            break;
        default:
            p               = temp + unit_len * (unit - 1);
            *(p + unit_len) = 0;
            unit_id         = atoi(p);
            break;
    }

    // 检查返回的unit id取值是否正确
    // 编码为0的unit id 都保留。
    if (unit_id == 0)
        return -1;
    // 第一级分类从10开始编码
    if (len == unit_len && unit_id > 0 && unit_id <= 9)
        return -1;

    return unit_id;
}

void category_tree_free(struct category_tree* tree)
{
    int i, j;

    for (i = 0; i < tree->level; i++) {
        for (j = 0; j < (tree->p + i)->size; j++) {
            xfree((tree->p + i)->p[j].data.name);
            xfree((tree->p + i)->p[j].data.cid);
            xfree((tree->p + i)->p[j].data.search_content);
        }

        xfree((tree->p + i)->p);
    }

    xfree(tree->p);
    xfree(tree);
}

/* 根据TYPE获得给定category对应的type category
   返回值: 错误返回-1，无相应分类返回0，有相应分类，返回分类个数和分类明指针数组
*/
int category_tree_get(struct category_tree* tree, char* category_id, int type, struct category_result* result)
{
    struct category_pool *pool, *parent_pool, *child_pool, *brother_pool, *grand_pool;
    struct category *     category, *parent, *child, *brother, *grand;
    int                   self_unit_id, parent_unit_id, level, child_unit_id, brother_unit_id, grand_unit_id;
    int                   i, len;

    self_unit_id = category_get_unit_id(category_id, CATEGORY_UNIT_ID_SELF, tree->unit_len, tree->level);
    if (self_unit_id < 0)
        return -1;

    len   = strlen(category_id);
    level = len / tree->unit_len;
    if (level <= 0 || level > tree->level)
        return -1;

    // 检查self_unit_id是否超出了相应pool 的size
    // 注意即使没有超过pool size，也有可能分类实际不存在
    if (self_unit_id >= (tree->p + level - 1)->size)
        return -1;
    pool     = tree->p + (level - 1);
    category = pool->p + self_unit_id;
    // 由于pool的大小大于实际的分类数，所以会出现pool中有空间但没实际分类的情况
    if (category->unit_id == 0 || category->data.name == NULL)
        return -1;

    if (strcasecmp(category->data.cid, category_id) != 0)
        return -1;

    switch (type) {
        case CATEGORY_TYPE_SELF:
            result->num   = 1;
            result->pp[0] = &category->data;
            return result->num;
        case CATEGORY_TYPE_GRANDFATHER:
            if (level == 1 || level == 2) {
                result->num = 0;
                return 0;
            } else {
                if (category->parent_id == 0)
                    return -1;
                parent_unit_id = category->parent_id;
                parent_pool    = tree->p + (level - 1 - 1);
                parent         = parent_pool->p + parent_unit_id;  // 不会超出pool，父分类是由儿子来设置的
                if (parent->unit_id == 0 || parent->data.name == NULL)
                    return -1;

                if (parent->parent_id == 0)
                    return -1;
                grand_unit_id = parent->parent_id;
                grand_pool    = tree->p + (level - 1 - 1 - 1);
                grand         = grand_pool->p + grand_unit_id;  // 不会超出pool，父分类是由儿子来设置的
                if (grand->unit_id == 0 || grand->data.name == NULL)
                    return -1;
                result->num   = 1;
                result->pp[0] = &grand->data;
                return result->num;
            }
        case CATEGORY_TYPE_PARENT:
            if (level == 1) {
                result->num = 0;
                return 0;
            } else {
                // 也有可能只设置了二级分类，而没有设置它的一级分类
                if (category->parent_id == 0)
                    return -1;
                parent_unit_id = category->parent_id;
                parent_pool    = tree->p + (level - 1 - 1);
                parent         = parent_pool->p + parent_unit_id;  // 不会超出pool，父分类是由儿子来设置的
                if (parent->unit_id == 0 || parent->data.name == NULL)
                    return -1;

                result->num   = 1;
                result->pp[0] = &parent->data;
                return result->num;
            }
        case CATEGORY_TYPE_CHILD:
            if (level == tree->level || category->child_num == 0) {
                result->num = 0;
                return 0;
            }

            if (category->child_num <= result->size)
                result->num = category->child_num;
            else
                result->num = result->size;

            if (category->first_child_id == 0)
                return -1;
            child_unit_id = category->first_child_id;
            child_pool    = tree->p + level;
            child         = child_pool->p + child_unit_id;  // 不会超出池，儿子已经存在了才找父亲的
            result->pp[0] = &child->data;
            if (child->brother_id == 0) {
                if (result->num == 1)
                    return result->num;
                else
                    return -1;
            }

            for (i = 1; i < result->num; i++) {
                child_unit_id = child->brother_id;
                child         = child_pool->p + child_unit_id;
                result->pp[i] = &child->data;
                if (child->brother_id == 0) {
                    if (i + 1 != result->num)
                        return -1;
                }
            }

            return result->num;
        case CATEGORY_TYPE_BROTHER:
            if (level == 1)  // 第一级分类没有兄弟，因为它们都没有父亲
            {
                result->num = 0;
                return 0;
            } else {
                // 获得该分类的父亲
                if (category->parent_id == 0)
                    return -1;
                parent_unit_id = category->parent_id;
                parent_pool    = tree->p + (level - 1 - 1);
                parent         = parent_pool->p + parent_unit_id;
                if (parent->unit_id == 0 || parent->data.name == NULL)
                    return -1;

                // 获得该分类的父亲的儿子，就是该分类的兄弟了
                if (parent->child_num == 0)
                    return -1;
                if (parent->child_num <= result->size)
                    result->num = parent->child_num;
                else
                    result->num = result->size;

                if (parent->first_child_id == 0)
                    return -1;
                brother_unit_id = parent->first_child_id;
                brother_pool    = tree->p + (level - 1);
                brother         = brother_pool->p + brother_unit_id;
                result->pp[0]   = &brother->data;
                if (brother->brother_id == 0) {
                    if (result->num == 1)
                        return result->num;
                    else
                        return -1;
                }

                for (i = 1; i < result->num; i++) {
                    brother_unit_id = brother->brother_id;
                    brother         = brother_pool->p + brother_unit_id;
                    result->pp[i]   = &brother->data;
                    if (brother->brother_id == 0) {
                        if (i + 1 != result->num)
                            return -1;
                    }
                }

                return result->num;
            }
        default:
            return -1;
    }
}

#ifdef TEST_CATEGORY

int main()
{
    int                          i, result, type;
    struct category_tree*        ctree;
    char                         errmsg[256];
    struct category_result_data* pp[100];
    struct category_result       cc_result;
    char                         id[256], cmd[256];

    errmsg[0] = 0;
    ctree     = category_tree_init("category.map", 3, 3, errmsg, 256);
    if (ctree == NULL) {
        printf("Initialize the Categroy Tree error\n");
        if (errmsg[0])
            printf("Reason: %s\n", errmsg);
        return -1;
    }

    while (1) {
        printf("Type: ");
        fgets(cmd, 256, stdin);
        trim(cmd);
        if (*cmd == 0)
            continue;
        if (strcasecmp(cmd, "exit") == 0)
            break;
        else if (strncasecmp(cmd, "grand", 2) == 0)
            type = CATEGORY_TYPE_GRANDFATHER;
        else if (strncasecmp(cmd, "self", 2) == 0)
            type = CATEGORY_TYPE_SELF;
        else if (strncasecmp(cmd, "parent", 2) == 0)
            type = CATEGORY_TYPE_PARENT;
        else if (strncasecmp(cmd, "child", 2) == 0)
            type = CATEGORY_TYPE_CHILD;
        else if (strncasecmp(cmd, "brother", 2) == 0)
            type = CATEGORY_TYPE_BROTHER;
        else
            continue;

        printf("ID: ");
        fgets(id, 256, stdin);
        trim(id);

        cc_result.pp   = pp;
        cc_result.num  = 0;
        cc_result.size = 100;

        result         = category_tree_get(ctree, id, type, &cc_result);
        if (result < 0)
            return -1;

        if (result == 0) {
            printf("No Category\n");
            continue;
        }

        for (i = 0; i < result; i++) {
            printf("%s:%s:%s\n", cc_result.pp[i]->cid, cc_result.pp[i]->name, cc_result.pp[i]->search_content);
        }
    }

    category_tree_free(ctree);
    return 0;
}
#endif
