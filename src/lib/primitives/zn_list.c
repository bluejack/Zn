
#include "zn_list.h"

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
  object_destructor destroy;
};

zn_list* 
list_new(object_destructor destructor) 
{
  zn_list *ll = malloc(sizeof(zn_list));
  memset(ll, 0, sizeof(zn_list));
  ll->destroy = destructor;
  return ll;
}

void 
list_destroy(zn_list *ll)
{
  assert(ll != NULL);
  list_empty(ll);
  free(ll);
}

void
list_empty(zn_list *ll)
{
  assert(ll != NULL);
  while (ll->first) {
    ll->curr = ll->first->next;
    if(ll->destroy != NULL) {
      ll->destroy(ll->first->obj);
    }
    free(ll->first);
    ll->first = ll->curr;
  }
}

void  
list_add(zn_list *ll, void *obj)
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
list_reset(zn_list *ll)
{
  ll->curr = ll->first;
}

bool  
list_has_next(zn_list *ll)
{
  return ll->curr != NULL;
}

void* 
list_next(zn_list *ll)
{
  if (ll->curr == NULL) return NULL;
  void *obj = ll->curr->obj;
  ll->curr = ll->curr->next;
  return obj;
}

void* 
list_first(zn_list *ll)
{
  list_reset(ll);
  if (ll->first == NULL) return NULL;
  return ll->first->obj;
}

unsigned long 
list_size(zn_list *ll)
{
  return ll->size;
}
