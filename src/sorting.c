#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "binary_cache.h"

#define MAX_ITEMS 1000

/* Comparator for Item Name (A-Z) */
int compare_by_name(const void *a, const void *b)
{
    Item *item1 = *(Item **)a;
    Item *item2 = *(Item **)b;
    return strcmp(item1->item_name, item2->item_name);
}

/* Comparator for Name (Z-A) */
int compare_by_name_reverse(const void *a, const void *b)
{
    return compare_by_name(b, a);
}

/* Comparator for Quantity (Lowest to Highest) */
int compare_by_quantity(const void *a, const void *b)
{
    Item *item1 = *(Item **)a;
    Item *item2 = *(Item **)b;
    return item1->quantity - item2->quantity;
}

/* Comparator for Quantity (Highest to Lowest) */
int compare_by_quantity_reverse(const void *a, const void *b)
{
    return compare_by_quantity(b, a);
}

/* Comparator for Expiry Date (Earliest to Latest) */
int compare_by_expiry(const void *a, const void *b)
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

/* Comparator for Expiry Date (Latest to Earliest) */
int compare_by_expiry_reverse(const void *a, const void *b)
{
    return compare_by_expiry(b, a);
}

void sort_items(Item **items, int count, int sort_type, char choice)
{
    /* Sorting based on user's choice */
    if (sort_type == 1)
    {
        if (choice == 'a')
        {
            printf("Sorting by Name (A-Z)...\n");
            qsort(items, count, sizeof(Item *), compare_by_name);
        }
        else if (choice == 'b')
        {
            printf("Sorting by Name (Z-A)...\n");
            qsort(items, count, sizeof(Item *), compare_by_name_reverse);
        }
    }
    else if (sort_type == 2)
    {
        if (choice == 'a')
        {
            printf("Sorting by Quantity (Lowest to Highest)...\n");
            qsort(items, count, sizeof(Item *), compare_by_quantity);
        }
        else if (choice == 'b')
        {
            printf("Sorting by Quantity (Highest to Lowest)...\n");
            qsort(items, count, sizeof(Item *), compare_by_quantity_reverse);
        }
    }
    else if (sort_type == 3)
    {
        if (choice == 'a')
        {
            printf("Sorting by Expiry Date (Earliest to Latest)...\n");
            qsort(items, count, sizeof(Item *), compare_by_expiry);
        }
        else if (choice == 'b')
        {
            printf("Sorting by Expiry Date (Latest to Earliest)...\n");
            qsort(items, count, sizeof(Item *), compare_by_expiry_reverse);
        }
    }
}
