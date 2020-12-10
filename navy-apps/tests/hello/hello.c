#include <unistd.h>
#include <stdio.h>
#include <NDL.h>

int main() {
  if (write(1, "Hello World!\n", 13) == 0) puts("OK");
  printf("%d\n",write(1, "Hello World!\n", 13));

  puts("abc");

  int i = 2;
  volatile int j = 0;
  while (1) {
    // struct timeval tv;
    // gettimeofday(&tv, NULL);

    uint32_t time = NDL_GetTicks();
    // if (tv.tv_usec < 1500) {
    if (time <= 2) {

      printf("Hello World from Navy-apps at %dth time!\n", i ++);
      // printf("%d %d\n", tv.tv_sec, tv.tv_usec);
      printf("%d\n", time);


      j ++;
      if (j == 10000) {
        printf("Hello World from Navy-apps for the %dth time!\n", i ++);
        j = 0;
      }
    }
  }
  return 0;
}
