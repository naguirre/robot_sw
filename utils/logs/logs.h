
#ifndef __LOGS_H__
#define __LOGS_H__

/******************************************************************************
*
*   INCLUDED FILES
*
******************************************************************************/

#include <stdio.h>

#define LOGS_COLOR_RED      "\x1b[1m\x1b[31m"
#define LOGS_COLOR_GREEN    "\x1b[1m\x1b[32m"
#define LOGS_COLOR_YELLOW   "\x1b[1m\x1b[33m"
#define LOGS_COLOR_BLUE     "\x1b[1m\x1b[34m"
#define LOGS_COLOR_MAGENTA  "\x1b[1m\x1b[35m"
#define LOGS_COLOR_CYAN     "\x1b[1m\x1b[36m"
#define LOGS_COLOR_WHITE    "\x1b[1m\x1b[37m"
#define LOGS_COLOR_RESET    "\x1b[0m"

#define LOGS_CRI_LEVEL      0
#define LOGS_ERR_LEVEL      1
#define LOGS_WRN_LEVEL      2
#define LOGS_INF_LEVEL      3
#define LOGS_DBG_LEVEL      4

#define LOGS_LEVEL          LOGS_DBG_LEVEL

#define LOGS_OUTPUT         stderr

/******************************************************************************
*
*   DEFINES, MACROS & TYPEDEF
*
******************************************************************************/

#define CRI(...) \
    if (LOGS_LEVEL >= LOGS_CRI_LEVEL)                                                                                   \
    {                                                                                                                   \
        fprintf(LOGS_OUTPUT, LOGS_COLOR_RED "CRI" LOGS_COLOR_RESET " %s:%d %s() ", __FILE__, __LINE__, __FUNCTION__);   \
        fprintf(LOGS_OUTPUT, __VA_ARGS__);                                                                              \
        fprintf(LOGS_OUTPUT, "\n");                                                                                     \
    }

#define ERR(...)                                                                                                      \
    if (LOGS_LEVEL >= LOGS_ERR_LEVEL)                                                                                 \
    {                                                                                                                 \
        fprintf(LOGS_OUTPUT, LOGS_COLOR_RED "ERR" LOGS_COLOR_RESET " %s:%d %s() ", __FILE__, __LINE__, __FUNCTION__); \
        fprintf(LOGS_OUTPUT, __VA_ARGS__);                                                                            \
        fprintf(LOGS_OUTPUT, "\n");                                                                                   \
    }

#define WRN(...)                                                                                                        \
    if (LOGS_LEVEL >= LOGS_WRN_LEVEL)                                                                                   \
    {                                                                                                                   \
        fprintf(LOGS_OUTPUT, LOGS_COLOR_ORANGE "WRN" LOGS_COLOR_RESET " %s:%d %s() ", __FILE__, __LINE__, __FUNCTION__);\
        fprintf(LOGS_OUTPUT, __VA_ARGS__);                                                                              \
        fprintf(LOGS_OUTPUT, "\n");                                                                                     \
    }

#define INF(...)                                                                                                       \
    if (LOGS_LEVEL >= LOGS_INF_LEVEL)                                                                                  \
    {                                                                                                                  \
        fprintf(LOGS_OUTPUT, LOGS_COLOR_BLUE "INF" LOGS_COLOR_RESET " %s:%d %s() ", __FILE__, __LINE__, __FUNCTION__); \
        fprintf(LOGS_OUTPUT, __VA_ARGS__);                                                                             \
        fprintf(LOGS_OUTPUT, "\n");                                                                                    \
    }

#define DBG(...)                                                                                                        \
    if (LOGS_LEVEL >= LOGS_DBG_LEVEL)                                                                                   \
    {                                                                                                                   \
        fprintf(LOGS_OUTPUT, LOGS_COLOR_WHITE "DBG" LOGS_COLOR_RESET " %s:%d %s() ", __FILE__, __LINE__, __FUNCTION__); \
        fprintf(LOGS_OUTPUT, __VA_ARGS__);                                                                              \
        fprintf(LOGS_OUTPUT, "\n");                                                                                     \
    }

#endif