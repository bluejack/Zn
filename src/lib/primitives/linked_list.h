#ifndef ZINC_LINKED_LIST
#define ZINC_LINKED_LIST
/*----------------------------------------------------------------------------*\
** Linked List
**
** A very basic, primitive, linked list implementation
**
**           Copyright (c) 2010-2011, Blunt Jackson
**
\*----------------------------------------------------------------------------*/
#include <stdbool.h>

typedef struct _ll linked_list;

typedef void(*ll_destroyer)(void*);

linked_list* ll_new(void);
void ll_destroy(linked_list*, ll_destroyer);
void ll_empty(linked_list*, ll_destroyer);
void  ll_add(linked_list*, void*);
void  ll_reset(linked_list*);
bool  ll_has_next(linked_list*);
void* ll_next(linked_list*);
void* ll_first(linked_list*);
unsigned long ll_size(linked_list*);

#endif
