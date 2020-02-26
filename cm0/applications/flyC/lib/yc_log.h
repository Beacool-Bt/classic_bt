/*************************************************************************
*
* Yichip  
*
*************************************************************************/

#ifndef YC_LOG_H
#define YC_LOG_H
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "yc_drv_common.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifndef YC_LOG_DEFAULT_LEVEL
#define YC_LOG_DEFAULT_LEVEL 3U                       /*LOG default on/off*/
#endif

#ifndef YC_LOG_MODULE_NAME
#define YC_LOG_MODULE_NAME "DEFAULT"
#endif

#ifndef YC_LOG_LEVEL
#define YC_LOG_LEVEL 3U
#endif


#define YC_LOG_LEVEL_ERROR        1U
#define YC_LOG_LEVEL_WARNING      2U
#define YC_LOG_LEVEL_INFO         3U
#define YC_LOG_LEVEL_DEBUG        4U
#define YC_LOG_LEVEL_INTERNAL     5U


#define LOG_INTERNAL_0(type, prefix, str) \
    yc_log_std_0(type, prefix str)
#define LOG_INTERNAL_1(type, prefix, str, arg0) \
    yc_log_std_1(type, prefix str, arg0)
#define LOG_INTERNAL_2(type, prefix, str, arg0, arg1) \
    yc_log_std_2(type, prefix str, arg0, arg1)
#define LOG_INTERNAL_3(type, prefix, str, arg0, arg1, arg2) \
    yc_log_std_3(type, prefix str, arg0, arg1, arg2)
#define LOG_INTERNAL_4(type, prefix, str, arg0, arg1, arg2, arg3) \
    yc_log_std_4(type, prefix str, arg0, arg1, arg2, arg3)
#define LOG_INTERNAL_5(type, prefix, str, arg0, arg1, arg2, arg3, arg4) \
    yc_log_std_5(type, prefix str, arg0, arg1, arg2, arg3, arg4)
#define LOG_INTERNAL_6(type, prefix, str, arg0, arg1, arg2, arg3, arg4, arg5) \
    yc_log_std_6(type, prefix str, arg0, arg1, arg2, arg3, arg4, arg5)

#define CONCAT_2(p1, p2)      CONCAT_2_(p1, p2)
/** Auxiliary macro used by @ref CONCAT_2 */
#define CONCAT_2_(p1, p2)     p1##p2
#define NUM_VA_ARGS_LESS_1_IMPL(                       \
    _ignored,                                          \
    _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,       \
    _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,  \
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,  \
    _31, _32, _33, _34, _35, _36, _37, _38, _39, _40,  \
    _41, _42, _43, _44, _45, _46, _47, _48, _49, _50,  \
    _51, _52, _53, _54, _55, _56, _57, _58, _59, _60,  \
    _61, _62, N, ...) N

#define NUM_VA_ARGS_LESS_1(...) NUM_VA_ARGS_LESS_1_IMPL(__VA_ARGS__, 63, 62, 61,  \
        60, 59, 58, 57, 56, 55, 54, 53, 52, 51,                         \
        50, 49, 48, 47, 46, 45, 44, 43, 42, 41,                         \
        40, 39, 38, 37, 36, 35, 34, 33, 32, 31,                         \
        30, 29, 28, 27, 26, 25, 24, 23, 22, 21,                         \
        20, 19, 18, 17, 16, 15, 14, 13, 12, 11,                         \
        10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, ~)



#define LOG_INTERNAL_X(N, ...)          CONCAT_2(LOG_INTERNAL_, N) (__VA_ARGS__)
#define LOG_INTERNAL(type, prefix, ...) LOG_INTERNAL_X(NUM_VA_ARGS_LESS_1( \
                                                           __VA_ARGS__), type, prefix, __VA_ARGS__)

#define YC_LOG_BREAK      ":"

#define LOG_ERROR_PREFIX   YC_LOG_MODULE_NAME YC_LOG_BREAK "ERROR:"
#define LOG_WARNING_PREFIX YC_LOG_MODULE_NAME YC_LOG_BREAK "WARNING:"
#define LOG_INFO_PREFIX    YC_LOG_MODULE_NAME YC_LOG_BREAK "INFO:"
#define LOG_DEBUG_PREFIX   YC_LOG_MODULE_NAME YC_LOG_BREAK "DEBUG:"

#define YC_LOG_INTERNAL_ERROR(...)                                       \
    if ((YC_LOG_LEVEL >= YC_LOG_LEVEL_ERROR) &&                         \
        (YC_LOG_LEVEL_ERROR <= YC_LOG_DEFAULT_LEVEL))                   \
    {                                                                     \
        LOG_INTERNAL(YC_LOG_LEVEL_ERROR, LOG_ERROR_PREFIX, __VA_ARGS__); \
    }
#define YC_LOG_INTERNAL_WARNING(...)                                         \
    if ((YC_LOG_LEVEL >= YC_LOG_LEVEL_WARNING) &&                           \
        (YC_LOG_LEVEL_WARNING <= YC_LOG_DEFAULT_LEVEL))                     \
    {                                                                         \
        LOG_INTERNAL(YC_LOG_LEVEL_WARNING, LOG_WARNING_PREFIX, __VA_ARGS__); \
    }
#define YC_LOG_INTERNAL_INFO(...)                                      \
    if ((YC_LOG_LEVEL >= YC_LOG_LEVEL_INFO) &&                        \
        (YC_LOG_LEVEL_INFO <= YC_LOG_DEFAULT_LEVEL))                  \
    {                                                                   \
        LOG_INTERNAL(YC_LOG_LEVEL_INFO, LOG_INFO_PREFIX, __VA_ARGS__); \
    }
#define YC_LOG_INTERNAL_DEBUG(...)                                       \
    if ((YC_LOG_LEVEL >= YC_LOG_LEVEL_DEBUG) &&                         \
        (YC_LOG_LEVEL_DEBUG <= YC_LOG_DEFAULT_LEVEL))                   \
    {                                                                     \
        LOG_INTERNAL(YC_LOG_LEVEL_DEBUG, LOG_DEBUG_PREFIX, __VA_ARGS__); \
    }



#define YC_LOG_ERROR(...)                     YC_LOG_INTERNAL_ERROR(__VA_ARGS__)
#define YC_LOG_WARNING(...)                   YC_LOG_INTERNAL_WARNING( __VA_ARGS__)
#define YC_LOG_INFO(...)                      YC_LOG_INTERNAL_INFO( __VA_ARGS__)
#define YC_LOG_DEBUG(...)                     YC_LOG_INTERNAL_DEBUG( __VA_ARGS__)


void yc_log_std_0(uint8_t severity, char const * const p_str);
void yc_log_std_1(uint8_t            severity,
                        char const * const p_str,
                        uint32_t           val0);
void yc_log_std_2(uint8_t            severity,
                        char const * const p_str,
                        uint32_t           val0,
                        uint32_t           val1);
void yc_log_std_3(uint8_t            severity,
                        char const * const p_str,
                        uint32_t           val0,
                        uint32_t           val1,
                        uint32_t           val2);
void yc_log_std_4(uint8_t            severity,
                        char const * const p_str,
                        uint32_t           val0,
                        uint32_t           val1,
                        uint32_t           val2,
                        uint32_t           val3);
void yc_log_std_5(uint8_t            severity,
                        char const * const p_str,
                        uint32_t           val0,
                        uint32_t           val1,
                        uint32_t           val2,
                        uint32_t           val3,
                        uint32_t           val4);
void yc_log_std_6(uint8_t            severity,
                        char const * const p_str,
                        uint32_t           val0,
                        uint32_t           val1,
                        uint32_t           val2,
                        uint32_t           val3,
                        uint32_t           val4,
                        uint32_t           val5);


uint32_t yc_log_init(void);



#ifdef __cplusplus
}
#endif

#endif /* YC_LOG_H */

