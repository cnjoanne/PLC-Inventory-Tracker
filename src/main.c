#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "binary_cache.h"
#include "csv_parser.h"
#include "sorting.h"

int main(void)
{
    char filepath[256];
    Item **items;
    int *item_counter;
    int csv_is_valid;
    int i, choice;
    char option;
    Item **read_items;

    /* uploading input csv */
    print_upload_instructions();
    printf("Enter csv path: ");
    scanf("%255s", filepath);

    /* continue with process....*/
    item_counter = malloc(sizeof(int));
    items = NULL;
    csv_is_valid = parse_csv(filepath, &items, item_counter);

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
        printf("Successfully parsed %d valid items:\n", *item_counter);

        for (i = 0; i < *item_counter; i++)
        {
            printf("Item: %s, Quantity: %d, Expiry Date: %s\n",
                   (items)[i]->item_name, (items)[i]->quantity, (items)[i]->expiry_date);
        }
    }
    /* ==================================================================== */

    /* Print instructions eg. enter sort name to sort name alphabetically */
    print_user_instructions();

    /* Process user choice */
    while (1)
    {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        if (choice == 1 || choice == 2 || choice == 3)
        {
            printf("\nIf you would like to sort in ascending order, Enter: a.\n");
            printf("If you would like to sort in descending order, Enter: b.\n");
            while (1)
            {
                printf("\nEnter your option: ");
                scanf(" %c", &option);

                if (option == 'a' || option == 'b')
                {
                    sort_items(items, *item_counter, choice, option);
                    break;
                }
                else
                {
                    printf("Invalid option! Please enter 'a' or 'b'.\n");
                }
            }
        }
        else if (choice == 6)
        {
            printf("Exiting...\n");
            break;
        }
        else
        {
            printf("Invalid option! Please enter 1, 2, 3, or 6.\n");
        }
    }

    /* TODO: store as binary cache*/
    /* Write items to the binary cache */
    printf("\n\033[33mWriting items to binary cache...\033[0m\n");
    write_binary_cache(items, *item_counter);

    /* Read items from binary cache */
    printf("\n\033[33mReading items from binary cache...\033[0m\n");
    read_items = read_binary_cache();

    /* Verify the data read */
    if (read_items)
    {
        printf("\n\033[32mItems Read from Binary Cache:\033[0m\n");
        for (i = 0; i < *item_counter; i++)
        {
            printf("Item Name: %s\n", read_items[i]->item_name);
            printf("Quantity: %d\n", read_items[i]->quantity);
            printf("Expiry Date: %s\n\n", read_items[i]->expiry_date);
            free(read_items[i]);
        }
        free(read_items);
    }
    else
    {
        printf("\033[31mFailed to read from binary cache\033[0m\n");
    }

    /* TODO: Free memory, arrange with dan */
    for (i = 0; i < *item_counter; i++)
    {
        free(items[i]);
    }
    free(items);
    free(item_counter);

    /* while (!0){
        printf("Enter your instructions: ");

    } */

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
