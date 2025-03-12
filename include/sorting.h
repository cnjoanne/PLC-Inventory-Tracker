#ifndef SORTING_H
#define SORTING_H

#include "item.h"

void sort_items(Item **items, int count, int sort_type);
int compareByName(const void *a, const void *b);
int compareByExpiry(const void *a, const void *b);
int compareByQuantity(const void *a, const void *b);

#endif
