#include "zn_parser.h"

struct _parser {
  write_f write;
};

zn_parser* parser_new() {
  zn_parser *prs = malloc(sizeof(zn_parser));
  memset(prs, 0, sizeof(zn_parser));
  
  return prs;
}


zn_parse_result parse_line(const char* text)
{
  return parse_successful;
}

void parser_reg_write(write_f writer) {
  prs->write = writer;
}
