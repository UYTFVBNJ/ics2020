#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include <stdio.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
/*
void call_main(uintptr_t *args) {
  char *empty[] =  {NULL };
  environ = empty;
  exit(main(0, empty, empty));
  assert(0);
}
*/

void call_main(uintptr_t *args) {
  printf("call_main\n");
  int argc = (int)*args;
  char ** argv = (char **)(args + 1);
  char ** envp = (char **)(args + 1 + argc + 1); 
  printf("envp[0] %p\n", envp[0]);
  printf("argv[0] %p\n", argv[0]);
  printf("argv[1] %p\n", argv[1]);
  printf("argv[2] %p\n", argv[2]);
  // char *empty[] =  {NULL };
  // environ = empty;
  environ = envp;
  exit(main(argc, argv, envp));
  assert(0);
}
