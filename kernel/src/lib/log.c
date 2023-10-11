#include <kernel/log.h>
#include <kernel/port.h>
#include <stdarg.h>

void ultoa(uint64_t val, char *buf, int base)
{
  char tmp[50];
  char *tp = tmp;
  int i;

  while (val || tp == tmp)
  {
    i = val % base;
    val /= base;
    if (i < 10)
    {
      *tp++ = i + '0';
    }
    else
    {
      *tp++ = i + 'a' - 10;
    }
  }

  while (tp > tmp)
  {
    *buf++ = *--tp;
  }

  *buf = 0x00;
}

void utoa(uint32_t val, char *buf, int base)
{
  char tmp[16];
  char *tp = tmp;
  int i;

  while (val || tp == tmp)
  {
    i = val % base;
    val /= base;
    if (i < 10)
    {
      *tp++ = i + '0';
    }
    else
    {
      *tp++ = i + 'a' - 10;
    }
  }

  while (tp > tmp)
  {
    *buf++ = *--tp;
  }

  *buf = 0x00;
}

static void itoa(int val, char *buf, int base)
{
  char tmp[16];
  char *tp = tmp;
  int i;
  uint32_t v;

  int sign = (base == 10 && val < 0);
  if (sign)
  {
    v = -val;
  }
  else
  {
    v = (uint32_t)val;
  }

  while (v || tp == tmp)
  {
    i = v % base;
    v /= base;
    if (i < 10)
    {
      *tp++ = i + '0';
    }
    else
    {
      *tp++ = i + 'a' - 10;
    }
  }

  if (sign)
  {
    *buf++ = '-';
  }

  while (tp > tmp)
  {
    *buf++ = *--tp;
  }

  *buf = 0x00;
}

static void write_char(const char c)
{
  port_byte_out(0xE9, c);
}

static void write_string(const char *str)
{
  while (*str)
  {
    write_char(*str);
    str++;
  }
}

static int write_string_fmt(const char *fmt, va_list args)
{
  int i = 0;
  while (fmt[i])
  {
    if (fmt[i] == '%')
    {
      i++;
      switch (fmt[i])
      {
      case 'd':
      case 'i':
      {
        int x = va_arg(args, int);
        char buf[16];
        itoa(x, buf, 10);
        write_string(buf);
        break;
      }
      case 'u':
      {
        uint32_t x = va_arg(args, unsigned int);
        char buf[16];
        utoa(x, buf, 10);
        write_string(buf);
        break;
      }
      case 'p':
      {
        void *x = va_arg(args, void *);
        char buf[16];
        ultoa((uint64_t)x, buf, 16);
        write_string(buf);
        break;
      }
      case 'c':
      {
        int x = va_arg(args, int);
        write_char(x);
        break;
      }
      case 's':
      {
        char *x = va_arg(args, char *);
        write_string(x);
        break;
      }
      case 'x':
      case 'X':
      {
        int x = va_arg(args, unsigned int);
        char buf[16];
        utoa(x, buf, 16);
        write_string(buf);
        break;
      }
      case '%':
        write_char('%');
        break;
      default:
        write_char(fmt[i]);
        break;
      }
    }
    else
    {
      write_char(fmt[i]);
    }
    i++;
  }

  return 0;
}

int log(log_level level, const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  switch (level)
  {
  case LOG_TRACE:
  {
    write_string("[TRACE]: ");
    int res = write_string_fmt(fmt, args);
    if (res < 0)
    {
      return res;
    }
    write_char('\n');
    break;
  }

  case LOG_DEBUG:
  {
    write_string("[DEBUG]: ");
    int res = write_string_fmt(fmt, args);
    if (res < 0)
    {
      return res;
    }
    write_char('\n');
    break;
  }

  case LOG_INFO:
  {
    write_string("[INFO]:  ");
    int res = write_string_fmt(fmt, args);
    if (res < 0)
    {
      return res;
    }
    write_char('\n');
    break;
  }

  case LOG_ERROR:
  {
    write_string("[ERROR]: ");
    int res = write_string_fmt(fmt, args);
    if (res < 0)
    {
      return res;
    }
    write_char('\n');
    break;
  }

  default:
  {
    return -EINVARG;
    break;
  }
  }

  va_end(args);
  return 0;
}
