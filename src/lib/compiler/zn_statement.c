#include "zn_statement.h"

#include <stdlib.h>
#include <string.h>

#include "zinc/zn_list.h"

struct _stmt {
  zn_stmt_handler handle;
  zn_list     *params;
};

zn_statement* 
stmt_new() {
  zn_statement* stmt = malloc(sizeof(zn_statement));
  memset(stmt, 0, sizeof(stmt));
  stmt->params = list_new(&param_destroy);
  return stmt;
}

void 
stmt_add_handler(zn_statement *stmt, zn_stmt_handler h) {
  stmt->handle = h;
}

void stmt_add_arg(zn_statement *stmt, zn_param *param) {
  list_add(stmt->params, param);
}

zn_param* 
stmt_next_param(zn_statement *stmt) {
  return list_next(stmt->params);
}

char* stmt_process(zn_statement *stmt) {
  return stmt->handle(stmt);
}
