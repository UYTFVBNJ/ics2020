void init_monitor(int, char *[]);
void engine_start();
int is_exit_status_bad();

// GH: own changes
//#include "expr.h"
#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
uint32_t expr();
char buf[66000];
// GH: own changes

int main(int argc, char *argv[]) {

// GH: own changes
	
	FILE *fp = fopen("/home/gh/ics2020/nemu/tools/gen-expr/input","r");
	assert(fp != NULL);

	unsigned int result,i,exp; bool success=0;

	for (i=0;i<1;i++) {

		if (~fscanf(fp, "%u", &result)) {
			puts("ERR FSC");
			return 0;	
		}
		if (fgets(buf, 65900, fp) == NULL) {
			puts("ERR FGT");
			return 0;
		}

		exp=expr(buf, success);

		printf("%u %u %d\n",result, exp, success);
	}
// GH: own changes


  /* Initialize the monitor. */
  init_monitor(argc, argv);

  /* Start engine. */
  engine_start();

  return is_exit_status_bad();
}
