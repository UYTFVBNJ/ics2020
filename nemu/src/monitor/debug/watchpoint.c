#include "watchpoint.h"
#include "expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].in_use = 0;
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
    // wp_pool[i].exp
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

    memset(p->exp, 0, sizeof(char)*NR_WP_EXP);

    return p;
  }
}

void free_wp(int wp_no) {

  WP *p = head, *q = NULL;

  if (p == NULL) {
    puts("ERR: no watchpoint to free");
    return ;
  }

  while (p->NO != wp_no) {
    q = p;
    p = p->next;
  }

  if (q == NULL) head = NULL;
  else q->next = p->next;
  

  wp_pool[wp_no].next = free_; // the freed one will be used first
  free_ = &wp_pool[wp_no];

}

void wp_display() {
  for (int i = 0; i < 32; i ++) if (wp_pool[i].in_use) {
    printf("Num  Type  Disp  Enb  Address  What\n");
    printf("%3d expr  keep  y    unknown  %s\n", i, wp_pool[i].exp);
  }
}