#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list vl;
  va_start(vl,fmt);
  size_t fmtp = 0, outp = 0, bufp = 0;
  char *buf = malloc(65);

  int v_int; 
  char *v_str;


  for (fmtp = 0; fmt[fmtp] != '\0'; fmtp++) switch (fmt[fmtp]) {
  case '%':
    fmtp++;
    switch (fmt[fmtp]) {
      case 'd':
        v_int = va_arg(vl, int);
        bufp = 0;

        while (v_int) {
          buf[bufp++] = v_int % 10;
          v_int /= 10;
        }

        while (bufp) {
          out[outp++] = buf[--bufp];
        }

        break;
      case 's':
        v_str = va_arg(vl, char *);
        size_t i;

        for (i = 0; v_str[i] != '\0'; i++) out[outp + i] = v_str[i];
        outp += i;

        break;
      default:
        printf("Not implemented!");
        return -1;
        break;
    }
    break;
  
  case '\\':
    fmtp++;
    switch (fmt[fmtp]) { 
      case 'n':
        out[outp++] = '\n';
        break;
      default:
        printf("Not implemented!");
        return -1;
        break;
    }
    break;
  default:
    out[outp++] = fmt[fmtp];
    break;
  }

  out[outp] = '\0';

  free(buf);

  return outp;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {

  return 0;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  return 0;
}

#endif
