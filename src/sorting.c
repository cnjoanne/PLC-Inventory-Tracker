#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "binary_cache.h"

#define MAX_ITEMS 1000

/* Comparator for Item Name (A-Z) */
int compareByName(const void *a, const void *b)
{
    Item *item1 = *(Item **)a;
    Item *item2 = *(Item **)b;
    return strcmp(item1->item_name, item2->item_name);
}

/* Comparator for Quantity (Lowest to Highest) */
int compareByQuantity(const void *a, const void *b)
{
    Item *item1 = *(Item **)a;
    Item *item2 = *(Item **)b;
    return item1->quantity - item2->quantity;
}

/* Comparator for Expiry Date (Earliest to Latest) */
int compareByExpiry(const void *a, const void *b)
{
    Item *item1 = *(Item **)a;
    Item *item2 = *(Item **)b;

    int day1, month1, year1, day2, month2, year2;
    sscanf(item1->expiry_date, "%d/%d/%d", &day1, &month1, &year1);
    sscanf(item2->expiry_date, "%d/%d/%d", &day2, &month2, &year2);

    if (year1 != year2)
        return year1 - year2;
    if (month1 != month2)
        return month1 - month2;
    return day1 - day2;
}

void sort_items(Item **items, int count, int sort_type)
{
    /* Sorting based on user's choice */
    if (sort_type == 1)
    {
        printf("Sorting by Item Name (A-Z)...\n");
        qsort(items, count, sizeof(Item *), compareByName);
    }
    else if (sort_type == 2)
    {
        printf("Sorting by Quantity (Lowest to Highest)...\n");
        qsort(items, count, sizeof(Item *), compareByQuantity);
    }
    else if (sort_type == 3)
    {
        printf("Sorting by Expiry Date (Earliest to Latest)...\n");
        qsort(items, count, sizeof(Item *), compareByExpiry);
    }

    /* Write sorted data to binary cache */
    /* FIXME: ideally, we shouldnt be writing to binary here, so need to remove these lines below */
    /* write_binary_cache(items, count);
    printf("Sorting complete. Data saved to binary cache.\n"); */
}
