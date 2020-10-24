#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t i;

  for (i = 0; s[i] != '\0'; i++) ;

  return i;
}

char *strcpy(char* dst,const char* src) {
  size_t i;

  for (i = 0; src[i] != '\0'; i++) dst[i] = src[i];
  dst[i] = '\0';

  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  size_t i;
 
  for (i = 0; i < n && src[i] != '\0'; i++) dst[i] = src[i]; 
  for ( ; i < n; i++) dst[i] = '\0';

  return dst;
}

char* strcat(char* dst, const char* src) {
  size_t dst_len = strlen(dst);
  size_t i;

  for (i = 0; src[i] != '\0'; i++) dst[dst_len + i] = src[i];
  dst[dst_len + i] = src[i];

  return dst;
}

int strcmp(const char* s1, const char* s2) {
  size_t i;

  for (i = 0; s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i]; i++) ;

  if (s1[i] > s2[i]) return 1;
  else if (s1[i] == s2[i]) return 0;
  else return -1;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  size_t i;

  for (i = 0; i < n && s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i]; i++) ;

  if (s1[i] > s2[i]) return 1;
  else if (s1[i] == s2[i]) return 0;
  else return -1;
}

void* memset(void* v,int c,size_t n) {
  size_t i;

  for (i = 0; i < n; i++) ((char*)v)[i] = c;

  return v;
}

void* memmove(void* dst,const void* src,size_t n) {
  size_t i;
  char *p = malloc(n);

  if (p == NULL) return NULL;

  for (i = 0; i < n; i++) p[i] = ((char*)src)[i];
  for (i = 0; i < n; i++) ((char*)dst)[i] = p[i];

  free(p);
  
  return dst;
}

void* memcpy(void* out, const void* in, size_t n) {
  size_t i;
  
  for (i = 0; i < n; i++) ((char*)out)[i] = ((char*)in)[i];

  return out;
}

int memcmp(const void* s1, const void* s2, size_t n) {
  size_t i;
  
  for (i = 0; i < n - 1 && ((unsigned char*)s1)[i] == ((unsigned char*)s2)[i]; i++) ;
  
  if (((unsigned char*)s1)[i] > ((unsigned char*)s2)[i]) return 1;
  else if (((unsigned char*)s1)[i] == ((unsigned char*)s2)[i]) return 0;
  else return -1;
}

#endif
