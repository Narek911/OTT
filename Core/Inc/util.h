#ifndef UTIL_H_
#define UTIL_H_

#include <stdio.h>

#if defined(DEBUG) && (defined(USING_SEMIHOSTING) || PRINTF_DEBUG_ENABLED == 1)
#define DBG(format, ...) do { \
    printf(format, ## __VA_ARGS__); \
} while (0)
#else
#define DBG(format, ...) do { } while (0)
#endif

#if defined(USING_SEMIHOSTING) || PRINTF_DEBUG_ENABLED == 1
#define LOG(format, ...) do { \
    printf(format, ## __VA_ARGS__); \
} while (0)
#else
#define LOG(format, ...) do { } while (0)
#endif

#endif /* UTIL_H_ */
