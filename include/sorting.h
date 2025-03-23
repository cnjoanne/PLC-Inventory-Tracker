#ifndef SORTING_H
#define SORTING_H

#include "item.h"

void sort_items(Item **items, int count, int sort_type, char choice);
void handle_sort_choice(int choice,int *item_count);
void handle_filter_by_expiry(int *item_count);
void handle_filter_by_quantity(int *item_count);
int compare_by_name(const void *a, const void *b);
int compare_by_expiry(const void *a, const void *b);
int compare_by_quantity(const void *a, const void *b);


#endif
