#ifndef LOG_H
#define LOG_H

#include <kernel/types.h>

typedef enum
{
  LOG_TRACE,
  LOG_DEBUG,
  LOG_INFO,
  LOG_ERROR
} log_level;

int log(log_level level, const char *fmt, ...);

#define LOG_TRACE(...) log(LOG_TRACE, __VA_ARGS__)
#define LOG_DEBUG(...) log(LOG_DEBUG, __VA_ARGS__)
#define LOG_INFO(...) log(LOG_INFO, __VA_ARGS__)
#define LOG_ERROR(...) log(LOG_ERROR, __VA_ARGS__)

#define panic(...)                                             \
  log(LOG_ERROR, "Kernel panic at %s:%u", __FILE__, __LINE__); \
  log(LOG_ERROR, __VA_ARGS__);                                 \
  asm("cli");                                                  \
  asm("hlt")

#endif // LOG_H