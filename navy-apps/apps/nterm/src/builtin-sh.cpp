#include <nterm.h>
#include <stdarg.h>
#include <unistd.h>
#include <SDL.h>

#include <string.h>

char handle_key(SDL_Event *ev);

static void sh_printf(const char *format, ...) {
  static char buf[256] = {};
  va_list ap;
  va_start(ap, format);
  int len = vsnprintf(buf, 256, format, ap);
  va_end(ap);
  term->write(buf, len);
}

static void sh_banner() {
  sh_printf("Built-in Shell in NTerm (NJU Terminal)\n\n");
}

static void sh_prompt() {
  sh_printf("sh> ");
}

static void sh_handle_cmd(const char *cmd) {
  char buf[64];
  strcpy(buf, cmd);
  // printf("sh: buf %s\n", buf);
  int len = strlen(buf);
  buf[len - 1] = '\0'; // delete the '\n' in the end
  // printf("sh: buf %s\n", buf);

  char * pathname = strtok(buf, " ");

  char * arg;
  char * argv[32];
  int arg_cnt = 0;
  argv[arg_cnt] = arg;
  arg_cnt ++;

  while ((arg = strtok(NULL, " ")) != NULL) {
    argv[arg_cnt] = arg;
    arg_cnt ++;
  }

  argv[arg_cnt] = NULL;

  printf("sh: environ %p %p\n", &environ, environ);
  execvp(pathname, argv);
}

void builtin_sh_run() {
  sh_banner();
  sh_prompt();
  // setenv("PATH", "/bin", 0);
  setenv("PATH", "/bin:/usr/bin", 0);

  while (1) {
    SDL_Event ev;
    if (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_KEYUP || ev.type == SDL_KEYDOWN) {
        const char *res = term->keypress(handle_key(&ev));
        if (res) {
          sh_handle_cmd(res);
          sh_prompt();
        }
      }
    }
    refresh_terminal();
  }
}
