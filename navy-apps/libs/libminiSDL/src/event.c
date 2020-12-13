#include <NDL.h>
#include <SDL.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

#define KN_SZ (sizeof(keyname))

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  return 0;
}

int SDL_WaitEvent(SDL_Event *event) {
  char buf[64];
  while (1) {
    NDL_PollEvent(buf, 64);
    printf("Please Choose.\n");
    int i;
    char key[16];
    if (sscanf(buf, "kd %s", key)) {
    printf("kd\n");
    printf("buf: %s\n", buf);
      event->type = SDL_KEYDOWN;
      for (i = 0; i < KN_SZ; i ++) if (strcmp(key, keyname[i]) == 0) break;
      event->key.keysym.sym = i;
      return 1;
    } else 
    if (sscanf(buf, "ku %s", key)) {
    printf("ku\n");
      event->type = SDL_KEYUP;
      for (i = 0; i < KN_SZ; i ++) if (strcmp(key, keyname[i]) == 0) break;
      event->key.keysym.sym = i;
      return 1;
    } else {
      printf("???");
    }
  }

  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
