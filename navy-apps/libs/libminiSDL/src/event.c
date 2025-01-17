#include <NDL.h>
#include <SDL.h>
#include <string.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

#define KN_SZ (sizeof(keyname)/sizeof(char *))

static uint8_t keystate[KN_SZ];

int SDL_PushEvent(SDL_Event *ev) {
  printf("SDL_PushEvent\n");
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  printf("SDL_PollEvent\n");
  char buf[64];
  buf[0] = '\0';
  NDL_PollEvent(buf, 64);
  int i;
  char key[16];
  if (sscanf(buf, "kd %s", key)  == 1) {
    // printf("kd: %d\n", key);
    for (i = 0; i < KN_SZ; i ++) if (strcmp(key, keyname[i]) == 0) break;
    if (ev) {
      ev->type = SDL_KEYDOWN;
      ev->key.keysym.sym = i;
    }
    keystate[i] = 1;
    return 1;
  } else 
  if (sscanf(buf, "ku %s", key) == 1) {
    // printf("%s ku: %d\n", buf, key);
    for (i = 0; i < KN_SZ; i ++) if (strcmp(key, keyname[i]) == 0) break;
    if (ev) {
      ev->type = SDL_KEYUP;
      ev->key.keysym.sym = i;
    }
    keystate[i] = 0;
    return 1;
  } else {
    // printf("???");
  }

  printf("");
  return 0;
}

int SDL_WaitEvent(SDL_Event *event) {
  printf("SDL_WaitEvent\n");
  while (SDL_PollEvent(event) == 0) {
  }

  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  printf("SDL_PeepEvents\n");
  return 0;
}


uint8_t* SDL_GetKeyState(int *numkeys) {
  printf("SDL_GetKeyState\n");
  printf("%d\n", keystate[SDLK_RETURN]);
  return keystate;
}
