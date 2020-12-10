#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <NDL.h>
#include <BMP.h>

int main() {
  NDL_Init(0);
  int w, h;
  void *bmp = BMP_Load("/share/pictures/projectn.bmp", &w, &h);
  assert(bmp);
  printf("bmp: %d %d\n", w, h);
  NDL_OpenCanvas(&w, &h);
  printf("bmp: %d %d\n", w, h);
  NDL_DrawRect(bmp, 0, 0, w, h);
  for (int i = 0; i < h; i ++, puts(""))
    for (int j = 0; j < w; j ++) 
      printf("%d ", bmp[i * w + j]);
  free(bmp);
  NDL_Quit();
  printf("Test ends! Spinning...\n");
  while (1);
  return 0;
}
