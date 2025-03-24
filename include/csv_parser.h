#ifndef CSVPARSER_H
#define CSVPARSER_H

#include "item.h"

void print_upload_instructions(void);
void print_user_instructions(void);

int load_and_parse_csv(Item ***items, int *item_count);
int parse_csv(const char *filepath, Item ***items, int *item_counter);

#endif
