
#include "linked_list.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct _ll_ctr {
  struct _ll_ctr  *next;
  void            *obj;
} ll_ctr;

struct _ll {
  ll_ctr *first;
  ll_ctr *curr;
  unsigned long size;
};

linked_list* 
ll_new(void) 
{
  linked_list *ll = malloc(sizeof(linked_list));
  memset(ll, 0, sizeof(linked_list));
  return ll;
}

void 
ll_destroy(linked_list *ll, ll_destroyer destroy_func)
{
  assert(ll != NULL);
  ll_empty(ll, destroy_func);
  free(ll);
}

void
ll_empty(linked_list *ll, ll_destroyer destroy_func)
{
  assert(ll != NULL);
  while (ll->first) {
    ll->curr = ll->first->next;
    if(destroy_func != NULL) {
      destroy_func(ll->first->obj);
    }
    free(ll->first);
    ll->first = ll->curr;
  }
}

void  
ll_add(linked_list *ll, void *obj)
{
  assert(ll != NULL);
  assert(obj != NULL);

  ll_ctr *ctr = malloc(sizeof(ll_ctr));
  ctr->next = NULL;
  ctr->obj  = obj;
  
  if (ll->first == NULL) {
    ll->first = ctr;
    ll->curr  = ctr;
  } else {
    ll->curr->next = ctr;
    ll->curr = ll->curr->next;
  }
  ll->size++;
}

void  
ll_reset(linked_list *ll)
{
  ll->curr = ll->first;
}

bool  
ll_has_next(linked_list *ll)
{
  return ll->curr != NULL;
}

void* 
ll_next(linked_list *ll)
{
  if (ll->curr == NULL) return NULL;
  void *obj = ll->curr->obj;
  ll->curr = ll->curr->next;
  return obj;
}

void* 
ll_first(linked_list *ll)
{
  ll_reset(ll);
  if (ll->first == NULL) return NULL;
  return ll->first->obj;
}

unsigned long 
ll_size(linked_list *ll)
{
  return ll->size;
}
