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
	if (pos < 65536) buf[pos++]=ch;	
//	assert(pos <= 65536);
}

static inline int choose(int n) {
	return rand() %n;
}

static inline void gen_space() {
	while (choose(2)) gen(' ');
}

static inline void gen_num() {
	int a=choose(1<<31),b=0;
	while (a) {
		str[b++]=a %10;
		a/=10;
	}
	while (b) gen('0'+str[--b]);
}

static inline void gen_rand_op() {
	switch (choose(4)) {
		case 0: gen('+'); break;
		case 1: gen('-'); break;
		case 2: gen('*'); break;
		case 3: gen('/'); break;
	}
}

static void gen_rand_expr() {
	if (pos >= 65536) return ;
	gen_space();
	switch (choose(3)) {
		case 0: gen_num(); break;
		case 1: gen('('); gen_rand_expr(); gen(')'); break;
		case 2: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
	}
	gen_space();
}

char status[50];

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
		if (pos == 65536) {
			i--;
			continue;
		}
//		strcpy(buf,"1/0\0");
//		printf("C:%s\nENDC\n",buf);

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

//	  puts("A9");	
    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
//	  puts("A8");	
    if (ret != 0) continue;
		
		
//	  puts("A7");	

	
//		puts("A3");	
		
		fp = popen("bash -c '/tmp/.expr; true' 2>&1", "r");
//	  puts("A4");	
    assert(fp != NULL);
//	  puts("A5");	
		
    if (!fgets(status, 40, fp)) assert(0);
//	  puts("A6");	
    assert(!pclose(fp));


		if (strstr(status, "Floating") != NULL) {
			i--;
			continue;
		}
//		printf("S:%s\n",status);



    int result;
		
		fp = popen("bash -c '/tmp/.expr; true' ", "r");
    assert(fp != NULL);
		
//	  puts("A1");	
    if (!fscanf(fp, "%d", &result)) assert(0);
//	  puts("A2");	
    assert(!pclose(fp));
		
	  printf("%u %s\n", result, buf);
  }
  return 0;
}
