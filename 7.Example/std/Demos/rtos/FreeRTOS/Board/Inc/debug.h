#ifndef __DEBUG_H__
#define __DEBUG_H__

#define println(fmt, ...) printf(fmt "\r\n", ##__VA_ARGS__);

/**
 * @brief debug
 */

#define SDK_ERROR         0
#define SDK_WARNING       1
#define SDK_INFO          2
#define SDK_DEBUG         3

#define LOG(lvl, fmt, ...)               \
    do {                                 \
        if ((lvl) <= DBG_LVL) {          \
            println(fmt, ##__VA_ARGS__); \
        }                                \
    } while (0)

#define LOGE(fmt, ...) LOG(SDK_ERROR, "[E/%s]" fmt, TAG, ##__VA_ARGS__)
#define LOGW(fmt, ...) LOG(SDK_WARNING, "[W/%s]" fmt, TAG, ##__VA_ARGS__)
#define LOGI(fmt, ...) LOG(SDK_INFO, "[I/%s]" fmt, TAG, ##__VA_ARGS__)
#define LOGD(fmt, ...) LOG(SDK_DEBUG, "[D/%s]" fmt, TAG, ##__VA_ARGS__)

/**
 * @brief error check
 * @param [in] x print error codes to the console when x != 0
 */

#define ERRCHK(x)                                                 \
    do {                                                          \
        int err = (x);                                            \
        if (err) {                                                \
            println("[%s] err: %d, file: %s, line: %s ,func: %s", \
                    TAG, err, __FILE__, __LINE__, __FUNCTION__);  \
        }                                                         \
    } while (0)

#endif
