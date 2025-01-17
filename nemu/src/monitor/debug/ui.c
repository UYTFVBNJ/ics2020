#include <isa.h>
#include "expr.h"
#include "watchpoint.h"
#include <memory/vaddr.h>

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
void rtl_exit(int, vaddr_t, uint32_t); 
int is_batch_mode();
WP* new_wp();
void free_wp(int);
void wp_display();


/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  rtl_exit(4, cpu.pc, -1);
  return -1;
}

static int cmd_help(char *args);

// GH: own changes 

static int cmd_si(char *args) {
	char * arg = strtok(NULL, " ");	
  
	cpu_exec(arg?atol(arg):1);	
	return 0;
}

static int cmd_info(char *args) {
	char * arg = strtok(NULL, " ");
  check_ptr(arg);

	switch (arg[0]) {
		case 'r': isa_reg_display(); break;
		case 'w': wp_display(); break;
		default:
			puts("ERR");
	}
	return 0;
}

static int cmd_x(char *args) {
  char* arg = strtok(NULL, " ");
  check_ptr(arg);
	long long n=atol(arg);

  arg = strtok(NULL, " ");
  check_ptr(arg);
	word_t i=strtol(arg, NULL, 16);

	while (n--) {
		printf("%08X\n",vaddr_read(i,4));
		i+=4;
	}

	return 0;
}

static int cmd_p(char *args) {
	bool success = 1;
	word_t exp=expr(args, &success);
	if (success) {
		printf("%u\n", exp);
	} else puts("ERR: invalid expression");
	return 0;
}

static int cmd_w(char *args) {

  bool success = 1;
	word_t exp=expr(args, &success);
  
  if (!success) {
    puts("ERR: invalid expression");
    return 0;
  }

  WP *p = new_wp();

  p->enable = 1;

  for (int i = 0; args[i] != 0; i ++) p->exp[i] = args[i];

  p->last_result = exp;

	return 0;
}

static int cmd_d(char *args) {
  char * arg = strtok(NULL, " ");
  check_ptr(arg);

  free_wp(strtol(arg,NULL,10));
	return 0;
}



// GH: own changes

// GH: own changes

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Single Step", cmd_si }, 
	{ "info", "Show R/W Info", cmd_info },
	{ "x", "Show mem", cmd_x },
	{ "p", "Print Expression", cmd_p },
	{ "w", "Set up Watchpoint", cmd_w },
	{ "d", "Delete Watchpoint", cmd_d },


  /* TODO: Add more commands */

};

// GH: own changes

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop() {
  if (is_batch_mode()) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; } // if return value < 0 ...
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
