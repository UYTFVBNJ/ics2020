#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

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
  int argc = (int)*args;
  char ** argv = (char **)(args + 1);
  char ** envp = (char **)(args + 1 + argc + 1); 
  // char *empty[] =  {NULL };
  // environ = empty;
  environ = envp;
  exit(main(argc, argv, envp));
  assert(0);
}
