#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "binary_cache.h"
#include "csv_parser.h"
#include "sorting.h"
#include "filtering.h"
#include "utils.h"

int main(void)
{
    Item **items = NULL;
    int item_count = 0;
    int choice;

    /* Print instruction to upload CSV*/

    if (!load_and_parse_csv(&items, &item_count))
    {
        return 1;
    }

    printf("\n\033[33mWriting items to binary cache...\033[0m\n");
    write_binary_cache(items, &item_count);
    free_items(items, item_count);

    print_user_instructions();

    /* NOTE: At this stage of execution, **items has been freed but item_count still holds a non zero value*/
    while (1)
    {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        case 2:
        case 3:
            handle_sort_choice(choice, &item_count);
            break;
        case 4:
            handle_filter_by_expiry(&item_count);
            break;
        case 5:
            handle_filter_by_quantity(&item_count);
            break;
        case 6:
            printf("Exiting... Saving to LaTeX\n");
            return 0;
        default:
            printf("Invalid option! Please enter 1 to 6.\n");
            while (getchar() != '\n')
                ;
        }
    }
    return 0;
}
