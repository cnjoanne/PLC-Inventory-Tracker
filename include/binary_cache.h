#ifndef BINARY_CACHE_H
#define BINARY_CACHE_H


#include "item.h"


void write_binary_cache(Item **items, int num_items);
Item ** read_binary_cache(void);

#endif
