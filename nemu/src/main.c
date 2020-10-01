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
#include <stdlib.h>
uint32_t expr();
char buf[66000];
char *str;
// GH: own changes

int main(int argc, char *argv[]) {

  /* Initialize the monitor. */
  init_monitor(argc, argv);

// GH: own changes
/*	
	FILE *fp = fopen("/home/gh/ics2020/nemu/tools/gen-expr/input","r");
	assert(fp != NULL);

	unsigned int result,i,j,exp; bool success=1;

	puts("here");

	for (i=0;i<90;i++) {

		if (fgets(buf, 65900, fp) == NULL) {
			puts("ERR FGT");
			return 0;
		}

		result = strtol(buf, &str, 10);		
		// puts("PSS FGT");

		for (j=0;j<65000;j++) if (buf[j] == '\n') break;
		buf[j]=0;

		exp=expr(str, &success);

		// puts("PSS EXP");

		if (!success || result != exp) 
		printf("%d %u %u %d\n",i, result, exp, success);
	}

	puts("PSS");
*/
// GH: own changes

  /* Start engine. */
  engine_start();

  return is_exit_status_bad();
}
