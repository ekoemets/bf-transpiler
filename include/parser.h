#ifndef PARSER_H
#define PARSER_H

struct List *parse_bf_str(char *str);
void optimize_bf_list(struct List *list);

#endif