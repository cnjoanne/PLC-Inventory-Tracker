#ifndef SORTING_H
#define SORTING_H

#include "item.h"

void sort_items(Item **items, int count, int sort_type, char choice);
int compare_by_name(const void *a, const void *b);
int compare_by_expiry(const void *a, const void *b);
int compare_by_quantity(const void *a, const void *b);

#endif
