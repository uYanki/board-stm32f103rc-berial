
#ifndef _CMD_H_
#define _CMD_H_

#include "board.h"

#define CMD_HASH 0xb433e5c6 /* 'jiejie' string hash */

#if defined(__CC_ARM) || defined(__CLANG_ARM) /* ARM Compiler */
#define SECTION(x) __attribute__((section(x)))
#define CMD_USED   __attribute__((used))
#elif defined(__IAR_SYSTEMS_ICC__) /* IAR Compiler */
#define SECTION(x) @x
#define CMD_USED   __root
#else
#error "not supported tool chain..."
#endif

typedef void (*cmd_handler)(void);

typedef struct cmd {
    const char*  cmd;
    const char*  desc;
    unsigned int hash;
    cmd_handler  handler;
} cmd_t;

#define REGISTER_CMD(cmd, handler, desc)                    \
    const char     _register_##cmd##_cmd[]         = #cmd;  \
    const char     _register_##cmd##_desc[]        = #desc; \
    CMD_USED cmd_t _register_##cmd SECTION("CMDS") = {      \
        _register_##cmd##_cmd,                              \
        _register_##cmd##_desc,                             \
        (unsigned int)CMD_HASH,                             \
        (cmd_handler)&handler};

void cmd_init(void);
bool cmd_parse(const char* str);

#endif
