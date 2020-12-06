#include <unistd.h>
#include <stdio.h>

int main() {
  if (write(1, "Hello World!\n", 13) == 0) puts("OK");
  printf("%d\n",write(1, "Hello World!\n", 13));

  puts("abc");

  int i = 2;
  volatile int j = 0;
  while (1) {
    j ++;
    if (j == 10000) {
      // printf("Hello World from Navy-apps for the %dth time!\n", i ++);
      j = 0;
    }
  }
  return 0;
}
