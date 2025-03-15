#ifndef BINARY_CACHE_H
#define BINARY_CACHE_H


#include "item.h"


void write_binary_cache(Item **items, int *item_count);
Item ** read_binary_cache(Item ***items, int *item_count);

#endif
