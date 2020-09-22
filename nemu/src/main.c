void init_monitor(int, char *[]);
void engine_start();
int is_exit_status_bad();

#include "./monitor/debug/expr.c"
char buf[66000];

int main(int argc, char *argv[]) {

	if (freopen("../tools/gen-expr/input","r",stdin)!=0) return 0;

	unsigned int ans1,ans2; bool suc;
	while (~scanf("%u %s",&ans1,buf)) {
		ans2=expr(buf,&suc);
		if (ans1!=ans2) printf("%u %u\n",ans1,ans2);
	}

	return 0;

  /* Initialize the monitor. */
  init_monitor(argc, argv);

  /* Start engine. */
  engine_start();

  return is_exit_status_bad();
}
