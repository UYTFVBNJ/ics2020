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
	
	FILE *fp = fopen("/home/gh/ics2020/nemu/tools/gen-expr/input","r");
	assert(fp != NULL);

	unsigned int result,i,j,exp; bool success=1;

	while (fgets(buf, 65900, fp) == NULL) {

		result = strtol(buf, &str, 10);		
		// puts("PSS FGT");
/*
		printf("Read: %s\n",buf);
		
		for (int j=0;j<6;j++) printf("%d ",buf[j]); 
		puts("");

		buf[4]=0;
*/

		for (j=0;j<65000;j++) if (buf[j] == '\n') break;
		buf[j]=0;

		exp=expr(str, &success);

		// puts("PSS EXP");

		if (!success || result != exp) 
		printf("%d %u %u %d\n",i, result, exp, success);
	}
// GH: own changes

  /* Start engine. */
//   engine_start();

  return is_exit_status_bad();
}
