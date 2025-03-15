#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "binary_cache.h"
#include "csv_parser.h"
#include "sorting.h"
#include "filtering.h"

int main(void)
{
    char filepath[256];
    Item **items;
    int item_count; /* Number of valid items */
    int csv_is_valid;
    int i;
    /*int i, choice, filtered_count;
    int start_day, start_month, start_year;
    int quantity;
    int end_day, end_month, end_year;
    char option;
    char start_date[11], end_date[11];
    Item **read_items;
    Item **filtered_items;*/

    /* uploading input csv */
    print_upload_instructions();
    printf("Enter csv path: ");
    scanf("%255s", filepath);

    items = NULL;
    item_count = 0;
    csv_is_valid = parse_csv(filepath, &items, &item_count);

    if (csv_is_valid == 0)
    {
        printf("csv is valid.\n");
    }
    else
    {
        printf("csv is not valid.\n");
        return 1;
    }
    /* =====This is a checker, can remove if you are sure of the struct ====*/
    if (csv_is_valid == 0)
    {
        printf("Successfully parsed %d valid items:\n", item_count);

        for (i = 0; i < item_count; i++)
        {
            printf("Item: %s, Quantity: %d, Expiry Date: %s\n",
                   (items)[i]->item_name, (items)[i]->quantity, (items)[i]->expiry_date);
        }
    }
    /* ==================================================================== */

    /* Write items to the binary cache */
    printf("\n\033[33mWriting items to binary cache...\033[0m\n");
    write_binary_cache(items, &item_count);


    /* Free memory of array */
    for (i = 0; i < item_count; i++)
    {
        free(items[i]);
        printf("Freeing item %d\n", i);

    }
    free(items);

    /* ----Finish parsing of csv and storing contents as binary---- */
    /* Print instructions eg. enter sort name to sort name alphabetically */
    print_user_instructions();

    /* Read items from binary cache */
    printf("\n\033[33mReading items from binary cache...\033[0m\n");
    items = NULL; 
    items = read_binary_cache(&items, &item_count);
    
    /* Verify the data read */
    if (items)
    {
        printf("\033[32mItems Read from Binary Cache:\033[0m\n");
        for (i = 0; i < item_count; i++)
        {
            printf("Item Name: %s\n", items[i]->item_name);
            printf("Quantity: %d\n", items[i]->quantity);
            printf("Expiry Date: %s\n\n", items[i]->expiry_date);
        }
    }
    else 
    {
        printf("\033[31mFailed to read from binary cache\033[0m\n");
    }


    /* Free memory of array */
    for (i = 0; i < item_count; i++)
    {
        free(items[i]);
    }
    free(items);

    return 0;
}

/* TODO: process input csv [./csv_parser.c] */

/* TODO: Checks to ensure input csv contents are correct [./csv_parser.c] */

/* TODO: create struct of item: item_name; quantity; expiry_date  [./csv_parser.c] */

/* TODO: create array (pointer to array) to store many items [./csv_parser.c] */

/* TODO: convert data into binary, and store in cache folder [./binary_cache.c] */

/* TODO: process user's commands, eg. sort item, filter, export, quit [./main.c]*/

/* TODO: read binary cache [./binary_cache.c] */

/* TODO: sorting logic for item_name, quantity, expiry_date [in ./sorting.c]*/

/* TODO: filter logic for low quantity, expiry_date [in ./filtering.c]*/

/* TODO: write binary, overwrite previous binary cache [./binary_cache.c] */

/* TODO: write binary cache into LaTeX [in ./latex_export.c]*/

/* TODO: export LaTeX as output in ./data [in ./latex_export.c]*/
