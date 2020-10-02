#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>

#define NR_WP_EXP 100

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */

  bool in_use;
  char exp[NR_WP_EXP];
} WP;


#endif
