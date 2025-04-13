#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "binary_cache.h"
#include "csv_parser.h"
#include "sorting.h"
#include "filtering.h"
#include "latex_export.h"
#include "utils.h"

#define LOW_STOCK_LIMIT 5
#define EXPIRY_DATE_LIMIT "12/12/2023"

int main(void)
{
    Item **items = NULL;
    Item **low_stock_items = NULL;
    Item **expirying_soon_items = NULL;
    int item_count = 0;
    int low_stock_count = 0;
    int expirying_soon_count = 0;
    int choice;

    int i;

    /* Print instruction to upload CSV*/

    if (!load_and_parse_csv(&items, &item_count))
    {
        return 1;
    }

    printf("\n\033[33mWriting items to binary cache...\033[0m\n");
    write_binary_cache(items, &item_count);
    free_items(items, item_count);

    /* get low stock warning items */
    low_stock_items = get_low_stock_items(&low_stock_count, LOW_STOCK_LIMIT, item_count);

    /* get expirying soon items */
    expirying_soon_items = get_expirying_items(&expirying_soon_count, EXPIRY_DATE_LIMIT, item_count);

    print_user_instructions();

    /* NOTE: At this stage of execution, **items has been freed but item_count still holds a non zero value*/
    while (1)
    {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1: /* sort by name */
        case 2: /* sort by quantity */
        case 3: /* sort by expiry date */
            handle_sort_choice(choice, &item_count);
            break;
        case 4: /* filter by expiry date */
            handle_filter_by_expiry(&item_count);
            break;
        case 5: /* filter by quantity */
            handle_filter_by_quantity(&item_count);
            break;
        case 6:
            /* TODO: convert to LATeX here*/
            printf("Exiting... Saving to LaTeX\n");

            /* ===== checker, to remove after========*/
            printf("\033[34mFor low stock quantity of %d: \033[0m \n", low_stock_count);
            for (i = 0; i < low_stock_count; i++)
            {
                printf("Item: %s, Quantity: %d, Expiry Date: %s\n",
                       low_stock_items[i]->item_name, low_stock_items[i]->quantity, low_stock_items[i]->expiry_date);
            }

            printf("\033[34mFor expirying soon of %d: \033[0m \n", expirying_soon_count);
            for (i = 0; i < expirying_soon_count; i++)
            {
                printf("Item: %s, Quantity: %d, Expiry Date: %s\n",
                       expirying_soon_items[i]->item_name, expirying_soon_items[i]->quantity, expirying_soon_items[i]->expiry_date);
            }

            items = NULL;
            items = read_binary_cache(&item_count);
            printf("\033[34mFor items user manages, count: %d \033[0m \n", item_count);
            for (i = 0; i < item_count; i++)
            {
                printf("Item: %s, Quantity: %d, Expiry Date: %s\n",
                       items[i]->item_name, items[i]->quantity, items[i]->expiry_date);
            }
            to_latex(expirying_soon_items, low_stock_items, items, expirying_soon_count, low_stock_count, item_count);
            free_items(items, item_count);
            /*=======================================*/

            free_items(low_stock_items, low_stock_count);
            free_items(expirying_soon_items, expirying_soon_count);
            return 0;
        default:
            printf("Invalid option! Please enter 1 to 6.\n");
            while (getchar() != '\n')
                ;
        }
    }
    return 0;
}
