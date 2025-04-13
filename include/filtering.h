#ifndef FILTERING_H
#define FILTERING_H

#include "item.h"

Item **filter_items_by_expiry(Item **items, int count, const char *start_date, const char *end_date, int *filtered_count);
Item **filter_items_by_quantity(Item **items, int count, int quantity, int *filtered_count);
Item **filter_items_by_quantity_fsm(Item **items, int count, int quantity, int *filtered_count);
void handle_filter_by_expiry(int *item_count);
void handle_filter_by_quantity(int *item_count);

Item **get_low_stock_items(int *low_stock_count, int limit, int item_count);
Item **get_expirying_items(int *expirying_soon_count, char *expiry_limit, int item_count);
#endif
