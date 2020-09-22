#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

static int pos=0;
static char str[32];

static inline void gen(char ch) {
	buf[pos++]=ch;	
	assert(pos <= 65536);
}

static inline void gen_space() {
	while (choose(1)) gen(' ');
}

static inline void gen_num() {
	int a=choose(1<<31),b=0;
	while (a) {
		str[b++]=a %10;
		a/=10;
	}
	while (b) gen(str[--b]);
}

static inline void gen_rand_op() {
	switch (choose(4)) {
		case 0: gen('+');
		case 1: gen('-');
		case 2: gen('*');
		case 3: gen('/');
	}
}

static void gen_rand_expr() {
	gen_space();
	switch (choose(3)) {
		case 0: gen_num(); break;
		case 1: gen_('('); gen_rand_expr(); gen(')'); break;
		case 2: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
	}
	gen_space();
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
		pos=0;
    gen_rand_expr();
		gen('\0');

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
