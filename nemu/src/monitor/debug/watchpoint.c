#include "watchpoint.h"
#include "expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].enable = 0;
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
    // wp_pool[i].exp
    wp_pool[i].last_result = 0;
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

    p->enable = 1;
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
  
  wp_pool[wp_no].enable = 0;

  wp_pool[wp_no].next = free_; // the freed one will be used first
  free_ = &wp_pool[wp_no];

}

void wp_display() {
  for (int i = 0; i < 32; i ++) if (wp_pool[i].enable) {
    printf("Num  Type  Disp  Enb  Address  What\n");
    printf("%3d  expr  keep  y    unknown  %s\n", i, wp_pool[i].exp);
  }
}

bool wp_check() {
  bool ret = 0;
  
  for (WP *p = head; p != NULL; p = p->next) {
    word_t result; bool success = 1;
    result = expr(p->exp, &success);
    if (!success) Log("ERR: invalid expression");

    if (result != p->last_result) {
      printf("Watch point %3d: %s", p->NO, p->exp);

      puts("Old value = ");
      printf("%u\n",p->last_result);

      puts("New value = ");
      printf("%u\n",result);

      p->last_result = result;

      ret = 1;
    }

  }
  return ret;
}