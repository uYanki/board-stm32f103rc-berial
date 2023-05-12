#ifndef CATEGORY_H
#define CATEGORY_H

#define CATEGORY_TYPE_PARENT      1
#define CATEGORY_TYPE_CHILD       2
#define CATEGORY_TYPE_BROTHER     3
#define CATEGORY_TYPE_SELF        4
#define CATEGORY_TYPE_GRANDFATHER 5

#define CATEGORY_UNIT_ID_PARENT   0
#define CATEGORY_UNIT_ID_SELF     -1

struct category_result_data {
    char* name;            // 分类名
    char* search_content;  // 该分类用于搜索的content
    char* cid;             // category id
    int   result_num;      // 该分类下条目数
};

struct category {
    int                         unit_id;
    int                         parent_id;
    int                         first_child_id;
    int                         last_child_id;
    int                         child_num;
    int                         brother_id;
    struct category_result_data data;
};

struct category_pool {
    struct category* p;
    int              size;
};

struct category_tree {
    int                   level;  // 有多少层分类，每一层所有分类都放到一块pool 中
    struct category_pool* p;      // 指向每一层分类pool
    int                   unit_len;
};

struct category_result {
    struct category_result_data** pp;    // 指针数组，数组里的指针都是指向分类名
    int                           size;  // 指针数组的大小，指针数组的是由调用者提供的
    int                           num;   // 实际返回的指针数组大小
};

#ifdef __cplusplus
extern "C" {
#endif

struct category_tree* category_tree_init(char* category_map, int category_level, int category_id_unit_len, char* error_msg, int error_size);
void                  category_tree_free(struct category_tree* tree);
int                   category_tree_get(struct category_tree* tree, char* category_id, int type, struct category_result* result);
int                   category_get_unit_id(char* id, int unit, int unit_len, int level);

#ifdef __cplusplus
}
#endif

#endif
