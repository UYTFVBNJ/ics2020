#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include <stdio.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  int argc = (int)*args;
  printf("args : %p\n", args);
  printf("argc : %d\n", argc);
  char ** argv = (char **)(args + 1);
  char ** envp = (char **)(args + 1 + argc + 1); 
  printf("argv[0] : %p\n", argv[0]);
  printf("envp[0] %p : %p\n", envp, envp[0]);
  environ = envp;
  printf("envpon %p : %p\n", environ, environ[0]);
  exit(main(argc, argv, envp));
  assert(0);
}
