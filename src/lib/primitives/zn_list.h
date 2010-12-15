#ifndef ZN_LIST
#define ZN_LIST
/*----------------------------------------------------------------------------*\
** Linked List
**
** A very basic, primitive, linked list implementation
**
**           Copyright (c) 2010-2011, Blunt Jackson
**
\*----------------------------------------------------------------------------*/
#include <stdbool.h>

#include "zn_mem.h"

typedef struct _ll zn_list;

zn_list* list_new(object_destructor);
void     list_destroy(zn_list*);
void     list_empty(zn_list*);
void     list_add(zn_list*, void*);
void     list_reset(zn_list*);
bool     list_has_next(zn_list*);
void*    list_next(zn_list*);
void*    list_first(zn_list*);
unsigned long list_size(zn_list*);

#endif
