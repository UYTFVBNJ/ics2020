#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  va_list vl;
  va_start(vl,fmt);
  char buf[256];
  
  int outp = vsprintf(buf, fmt, vl);

  va_end(vl);

  return outp;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  size_t fmtp = 0, outp = 0, bufp = 0;
  char buf[15];

  int v_int; 
  char *v_str;


  for (fmtp = 0; fmt[fmtp] != '\0'; fmtp++) switch (fmt[fmtp]) {
    case '%':
    fmtp++;
    switch (fmt[fmtp]) {
      case 'd':
        v_int = va_arg(ap, int);
        bufp = 0;

        if (v_int < 0) {
          v_int = -v_int;
          out[outp++] = '-';
        }

        while (v_int) {
          buf[bufp++] = v_int % 10 + '0';
          v_int /= 10;
        }

        while (bufp) {
          out[outp++] = buf[--bufp];
        }

        break;
      case 's':
        v_str = va_arg(ap, char *);
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

  return outp;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list vl;
  va_start(vl,fmt);
  
  int outp = vsprintf(out, fmt, vl);

  va_end(vl);

  return outp;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {

  return 0;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  
  return 0;
}

#endif
