#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "utils.h"
#include "filtering.h"
#include "sorting.h"
#include "binary_cache.h"

#include <time.h>

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

    printf("\n\033[33mFiltering items expiring between %s and %s...\033[0m\n", start_date, end_date);

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

Item **filter_items_by_quantity_fsm(Item **items, int count, int quantity, int *filtered_count)
{
    enum State
    {
        START,
        CHECK_ITEM,
        ALLOC_ITEM,
        COPY_ITEM,
        ERROR,
        DONE
    } state = START;

    int i = 0, j = 0;
    Item **filtered_items = NULL;
    *filtered_count = 0;

    while (1)
    {
        switch (state)
        {
        case START:
            printf("\nFiltering items with quantity %d and lower...\n", quantity);

            filtered_items = malloc(count * sizeof(Item *));
            if (!filtered_items)
            {
                perror("Memory allocation failed for filtered items array");
                state = ERROR;
            }
            else if (count > 0)
            {
                state = CHECK_ITEM;
            }
            else
            {
                state = DONE;
            }
            break;

        case CHECK_ITEM:
            printf("entered CHECK_ITEM\n");
            if (i < count)
            {
                if (items[i]->quantity <= quantity)
                {
                    state = ALLOC_ITEM;
                }
                else
                {
                    i++;
                    state = CHECK_ITEM;
                }
            }
            else
            {
                state = DONE;
            }
            break;

        case ALLOC_ITEM:
            printf("entered ALLOC_ITEM\n");
            filtered_items[*filtered_count] = malloc(sizeof(Item));
            if (!filtered_items[*filtered_count])
            {
                perror("Memory allocation failed for individual item");
                state = ERROR;
            }
            else
            {
                (*filtered_count)++;
                state = COPY_ITEM;
            }
            break;

        case COPY_ITEM:
            printf("entered COPY_ITEM\n");
            memcpy(filtered_items[*filtered_count - 1], items[i], sizeof(Item));
            i++;
            state = CHECK_ITEM;
            break;

        case ERROR:
            printf("entered ERROR\n");
            for (j = 0; j < *filtered_count; j++)
            {
                free(filtered_items[j]);
            }
            free(filtered_items);
            return NULL;

        case DONE:
            printf("entered DONE\n");
            return filtered_items;
        }
    }
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

    printf("\n\033[33mFiltering items with quantity %d and lower...\033[0m\n", quantity);

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

void handle_filter_by_expiry(int *item_count)
{
    Item **items;
    Item **filtered;
    int filtered_count, i;
    char start_date[11], end_date[11];
    int start_day, start_month, start_year;
    int end_day, end_month, end_year;

    items = read_binary_cache(item_count);
    if (!items)
        return;

    while (1)
    {
        printf("\nEnter start date (DD/MM/YYYY): ");
        if (scanf("%10s", start_date) != 1 || sscanf(start_date, "%d/%d/%d", &start_day, &start_month, &start_year) != 3)
        {
            printf("Invalid date format! Please enter in DD/MM/YYYY format.\n");
            while (getchar() != '\n')
            {
            }
            continue;
        }

        printf("Enter end date (DD/MM/YYYY): ");
        if (scanf("%10s", end_date) != 1 || sscanf(end_date, "%d/%d/%d", &end_day, &end_month, &end_year) != 3)
        {
            printf("Invalid date format! Please enter in DD/MM/YYYY format.\n");
            while (getchar() != '\n')
            {
            }
            continue;
        }

        if (end_year < start_year || (end_year == start_year && end_month < start_month) ||
            (end_year == start_year && end_month == start_month && end_day < start_day))
        {
            printf("Invalid date range! End date cannot be earlier than start date.\n");
            while (getchar() != '\n')
            {
            }
            continue;
        }

        break;
    }

    filtered = filter_items_by_expiry(items, *item_count, start_date, end_date, &filtered_count);

    if (filtered_count > 0)
    {
        /* NOTE: Using write_binary_cache here will not modify item_count but it will override the existing binary file. Do be careful about using this overwritten binary file when implementing get_low_stock and expiry date*/
        write_binary_cache(filtered, &filtered_count);

        for (i = 0; i < filtered_count; i++)
        {
            free(filtered[i]);
        }
        free(filtered);
        *item_count = filtered_count;
        free_items(items, *item_count);
    }
    else
    {
        printf("No items found in this range. Try again.\n");
    }
}

void handle_filter_by_quantity(int *item_count)
{
    Item **items;
    Item **filtered;
    int quantity, filtered_count, i;

    items = read_binary_cache(item_count);
    if (!items)
        return;
    while (1)
    {
        printf("\nEnter quantity: ");
        if (scanf("%d", &quantity) == 1)
            break;
        printf("Invalid input.\n");
        while (getchar() != '\n')
        {
        }
        continue;
    }

    /* filtered = filter_items_by_quantity(items, *item_count, quantity, &filtered_count); */
    filtered = filter_items_by_quantity_fsm(items, *item_count, quantity, &filtered_count);
    if (filtered_count > 0)
    {
        /* NOTE: Using write_binary_cache here will not modify item_count but it will override the existing binary file. Do be careful about using this overwritten binary file when implementing get_low_stock and expiry date*/
        write_binary_cache(filtered, &filtered_count);

        for (i = 0; i < filtered_count; i++)
        {
            free(filtered[i]);
        }
        free(filtered);

        *item_count = filtered_count;
        free_items(items, *item_count);
    }
    else
    {
        printf("No items found. Try again.\n");
    }
}

Item **get_low_stock_items(int *low_stock_count, int limit, int item_count)
{
    Item **items;
    Item **low_stock_items;

    items = read_binary_cache(&item_count);
    if (!items)
        return NULL;

    low_stock_items = filter_items_by_quantity(items, item_count, limit, low_stock_count);
    /* sort low_stock_items based on quantity */
    qsort(low_stock_items, *low_stock_count, sizeof(Item *), compare_by_quantity);
    printf("%d\n", *low_stock_count);
    if (*low_stock_count <= 0)
    {
        printf("No items found.\n");
    }

    free_items(items, item_count);

    return low_stock_items;
}

Item **get_expirying_items(int *expirying_soon_count, char *expiry_limit, int item_count)
{
    Item **items;
    Item **expirying_soon_items;
    char today_date[11];

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(today_date, 11, "%d/%m/%Y", &tm);

    items = read_binary_cache(&item_count);
    if (!items)
        return NULL;

    expirying_soon_items = filter_items_by_expiry(items, item_count, expiry_limit, today_date, expirying_soon_count);
    /* sort expirying_soon_items based on expiry date */
    qsort(expirying_soon_items, *expirying_soon_count, sizeof(Item *), compare_by_expiry);

    printf("%d\n", *expirying_soon_count);
    if (*expirying_soon_count <= 0)
    {
        printf("No items found.\n");
    }

    free_items(items, item_count);

    return expirying_soon_items;
}
