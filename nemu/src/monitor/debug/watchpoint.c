#include "watchpoint.h"
#include "expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp() {
  if (free_ == NULL) {
    Log("ERR: not enough watch point");
    assert(0);
  }
  else {
    WP *p = free_;
    free_ = p->next;

    p->next = head;
    head = p;

    return p;
  }
}

void free_wp(int wp_no) {
  wp_pool[wp_no].next = free_; // the freed one will be used first
  free_ = &wp_pool[wp_no];

  // wp* p = head;

  // if (p == NULL) {

  // }
  // while (p)
}

void wp_display() {
  // for ()
}