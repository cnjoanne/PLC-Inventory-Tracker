#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"

void parse_date(const char *date_str, int *day, int *month, int *year)
{
    sscanf(date_str, "%d/%d/%d", day, month, year);
}

int is_within_range(const char *expiry_date, const char *start_date, const char *end_date)
{
    int exp_day, exp_month, exp_year;
    int start_day, start_month, start_year;
    int end_day, end_month, end_year;

    parse_date(expiry_date, &exp_day, &exp_month, &exp_year);
    parse_date(start_date, &start_day, &start_month, &start_year);
    parse_date(end_date, &end_day, &end_month, &end_year);

    /* Compare expiry date with start date */
    if (exp_year < start_year || (exp_year == start_year && exp_month < start_month) ||
        (exp_year == start_year && exp_month == start_month && exp_day < start_day))
    {
        return 0;
    }

    /* Compare expiry date with end date */
    if (exp_year > end_year || (exp_year == end_year && exp_month > end_month) ||
        (exp_year == end_year && exp_month == end_month && exp_day > end_day))
    {
        return 0;
    }

    return 1;
}

Item **filter_items_by_expiry(Item **items, int count, const char *start_date, const char *end_date, int *filtered_count)
{
    int i;
    Item **filtered_items;

    *filtered_count = 0;

    /* Allocate memory for filtered results (maximum possible size) */
    filtered_items = malloc(count * sizeof(Item *));
    if (!filtered_items)
    {
        perror("Memory allocation failed for filtered items");
        return NULL;
    }

    printf("\nFiltering items expiring between %s and %s...\n", start_date, end_date);

    for (i = 0; i < count; i++)
    {
        if (is_within_range(items[i]->expiry_date, start_date, end_date))
        {
            filtered_items[*filtered_count] = malloc(sizeof(Item));
            if (!filtered_items[*filtered_count])
            {
                perror("Memory allocation failed for individual item");
                return NULL;
            }
            memcpy(filtered_items[*filtered_count], items[i], sizeof(Item));
            (*filtered_count)++;
        }
    }

    return filtered_items;
}

Item **filter_items_by_quantity(Item **items, int count, int quantity, int *filtered_count)
{
    int i;
    Item **filtered_items;

    *filtered_count = 0;

    filtered_items = malloc(count * sizeof(Item *));
    if (!filtered_items)
    {
        perror("Memory allocation failed for filtered items");
        return NULL;
    }

    printf("\nFiltering items with quantity %d and lower...\n", quantity);

    for (i = 0; i < count; i++)
    {
        if (items[i]->quantity <= quantity)
        {
            filtered_items[*filtered_count] = malloc(sizeof(Item));
            if (!filtered_items[*filtered_count])
            {
                perror("Memory allocation failed for individual item");
                return NULL;
            }
            memcpy(filtered_items[*filtered_count], items[i], sizeof(Item));
            (*filtered_count)++;
        }
    }

    return filtered_items;
}
