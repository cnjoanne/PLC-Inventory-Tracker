#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "utils.h"
#include "filtering.h"
#include "sorting.h"
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

void handle_sort_choice(int choice, int *item_count)
{
    char option;
    Item **items;
    
    items = read_binary_cache(item_count);  

    if (!items) {
        printf("Error reading binary cache\n");
        return;
    }

    while (1) {
        printf("\nEnter 'a' for ascending, 'b' for descending: ");
        scanf(" %c", &option);
        if (option == 'a' || option == 'b') break;
        printf("Invalid input.\n");
    }

    sort_items(items, *item_count, choice, option);
    write_binary_cache(items, item_count);
    free_items(items, *item_count); 
}

void handle_filter_by_expiry(int* item_count)
{
    Item **items;
    Item **filtered;
    int filtered_count,i;
    char start_date[11], end_date[11];
    int start_day, start_month, start_year;
    int end_day, end_month, end_year;

    items = read_binary_cache(item_count);
    if (!items) return;

    while (1)
    {
        printf("\nEnter start date (DD/MM/YYYY): ");
        if (scanf("%10s", start_date) != 1 || sscanf(start_date, "%d/%d/%d", &start_day, &start_month, &start_year) != 3)
        {
            printf("Invalid date format! Please enter in DD/MM/YYYY format.\n"); while (getchar() != '\n') {}
            continue;
        }

        printf("Enter end date (DD/MM/YYYY): ");
        if (scanf("%10s", end_date) != 1 || sscanf(end_date, "%d/%d/%d", &end_day, &end_month, &end_year) != 3)
        {
            printf("Invalid date format! Please enter in DD/MM/YYYY format.\n"); while (getchar() != '\n'){}
            continue;
        }

        if (end_year < start_year || (end_year == start_year && end_month < start_month) ||
            (end_year == start_year && end_month == start_month && end_day < start_day))
        {
            printf("Invalid date range! End date cannot be earlier than start date.\n"); while (getchar() != '\n'){}
            continue;
        }

        break;
    }
    
    filtered = filter_items_by_expiry(items, *item_count, start_date, end_date, &filtered_count);

    if (filtered_count > 0) {
        /* NOTE: Using write_binary_cache here will not modify item_count but it will override the existing binary file. Do be careful about using this overwritten binary file when implementing get_low_stock and expiry date*/
        write_binary_cache(filtered, &filtered_count); 
    } else {
        printf("No items found in this range.\n");
    }

    for (i = 0; i < filtered_count; i++) {
        free(filtered[i]);
    }
    free(filtered); 
    free_items(items, *item_count);
}

void handle_filter_by_quantity(int* item_count)
{
    Item **items;
    Item **filtered;
    int quantity,filtered_count,i;


    items = read_binary_cache(item_count);
    if (!items) return;
    while (1) {
        printf("\nEnter quantity: ");
        if (scanf("%d", &quantity) == 1) break;
        printf("Invalid input.\n"); while (getchar() != '\n');
    }

   
    filtered = filter_items_by_quantity(items, *item_count, quantity, &filtered_count);
    if (filtered_count > 0) {
        /* NOTE: Using write_binary_cache here will not modify item_count but it will override the existing binary file. Do be careful about using this overwritten binary file when implementing get_low_stock and expiry date*/
        write_binary_cache(filtered, &filtered_count);
    } else {
        printf("No items found.\n");
    }

    for (i = 0; i < filtered_count; i++) {
        free(filtered[i]);
    }
    free(filtered); 
    free_items(items, *item_count);
}
