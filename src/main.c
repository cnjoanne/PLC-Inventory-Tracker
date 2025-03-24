#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "binary_cache.h"
#include "csv_parser.h"
#include "sorting.h"
#include "filtering.h"
#include "latex_export.h"

#define LOW_STOCK_QUANTITY 5
#define EXPIRING_SOON_LIMIT 12/12/2025


int main(void)
{
    char filepath[256];
    Item **items; /* array of items */
    int item_count; /* number of valid items */
    int csv_is_valid;
    int i;
    int choice, filtered_count;
    int start_day, start_month, start_year;
    int quantity;
    int end_day, end_month, end_year;
    char option;
    char start_date[11], end_date[11];
    Item **filtered_items;

    /* SECTION: uploading input csv */
    print_upload_instructions();
    printf("Enter csv path: ");
    scanf("%255s", filepath);

    items = NULL;
    item_count = 0;
    csv_is_valid = parse_csv(filepath, &items, &item_count);

    if (csv_is_valid == 0)
    {
        printf("\033[32mcsv is valid.\033[0m\n");
    }
    else
    {
        printf("\033[34mcsv is not valid.\033[0m\n");
        return 1;
    }

    if (csv_is_valid == 0)
    {
        printf("\033[32mSuccessfully parsed %d valid items:\033[0m\n", item_count);

        for (i = 0; i < item_count; i++)
        {
            printf("Item: %s, Quantity: %d, Expiry Date: %s\n",
                   (items)[i]->item_name, (items)[i]->quantity, (items)[i]->expiry_date);
        }
    }

    /* SECTION: Write items to the binary cache */
    printf("\n\033[33mWriting items to binary cache...\033[0m\n");
    write_binary_cache(items, &item_count);


    /* Free memory of array */
    for (i = 0; i < item_count; i++)
    {
        free(items[i]);

    }
    free(items);

    /* SECTION: ----Finish parsing of csv and storing contents as binary---- */
    print_user_instructions(); /* Print instructions eg. enter sort name to sort name alphabetically */

    /* SECTION: Read items from binary cache (to parse user instructions )*/
    printf("\n\033[33mReading items from binary cache...\033[0m\n");
    items = NULL;
    items = read_binary_cache(&item_count);
    if (items == NULL){ /*to make sure no. of malloc  ==  no. of free*/
        printf("binary was not read properly, \n");
        return 1;
    }
    
    /* Verify the data read 
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
    */

    /*TODO: Get Low Stock Warning array*/

    /*TODO: Get Expirying soon array*/

    /* SECTION: Process user choice */
    while (1)
    {
        /*TODO: maybe runner.c ? */
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        if (choice == 1 || choice == 2 || choice == 3)
        {
            /* Read cache to make new items array */
            printf("\n\033[33mReading items from binary cache...\033[0m\n");
            items = NULL;
            items = read_binary_cache(&item_count);
            if (items == NULL){ /*to make sure no. of malloc  ==  no. of free*/
                printf("binary was not read properly, \n");
                return 1;
            }

            printf("\nIf you would like to sort in ascending order, Enter: a.\n");
            printf("If you would like to sort in descending order, Enter: b.\n");
            while (1)
            {
                printf("\nEnter your option: ");
                scanf(" %c", &option);

                if (option == 'a' || option == 'b')
                {
                    sort_items(items, item_count, choice, option);
                    break;
                }
                else
                {
                    printf("Invalid option! Please enter 'a' or 'b'.\n");
                    while (getchar() != '\n')
                        ;
                }
            }
            /* Write the sorted items to the binary cache */
            printf("\n\033[33mWriting sorted items to binary cache...\033[0m\n");
            write_binary_cache(items, &item_count);

            /* Free memory of items array */
            for (i = 0; i < item_count; i++)
            {
                free(items[i]);
            }
            free(items);
        }
        else if (choice == 4)
        {
            /* Read cache to make new items array */
            printf("\n\033[33mReading items from binary cache...\033[0m\n");
            items = NULL;
            items = read_binary_cache(&item_count);
            if (items == NULL){ /*to make sure no. of malloc  ==  no. of free*/
                printf("binary was not read properly, \n");
                return 1;
            }

            while (1)
            {
                printf("\nEnter start date (DD/MM/YYYY): ");
                if (scanf("%10s", start_date) != 1 || sscanf(start_date, "%d/%d/%d", &start_day, &start_month, &start_year) != 3)
                {
                    printf("Invalid date format! Please enter in DD/MM/YYYY format.\n");
                    while (getchar() != '\n')
                        ;
                }

                printf("Enter end date (DD/MM/YYYY): ");
                if (scanf("%10s", end_date) != 1 || sscanf(end_date, "%d/%d/%d", &end_day, &end_month, &end_year) != 3)
                {
                    printf("Invalid date format! Please enter in DD/MM/YYYY format.\n");
                    while (getchar() != '\n')
                        ;
                }

                /* Check if end date is before start date */
                if (end_year < start_year || (end_year == start_year && end_month < start_month) ||
                    (end_year == start_year && end_month == start_month && end_day < start_day))
                {
                    printf("Invalid date range! End date cannot be earlier than start date.\n");
                    while (getchar() != '\n')
                        ;
                }

                break;
            }

            filtered_items = filter_items_by_expiry(items, item_count, start_date, end_date, &filtered_count);

            if (filtered_items == NULL)
            {
                printf("Error: Could not allocate memory for filtering.\n");
            }
            else if (filtered_count > 0)
            {
                /* Write the filtered list to the binary cache */
                printf("\n\033[33mWriting filtered items to binary cache...\033[0m\n");
                write_binary_cache(filtered_items, &filtered_count);
            }
            else
            {
                printf("No items found in this date range.\n");
            }

            /* Free the filtered list (but NOT the original items) */
           /* for (i = 0; i < filtered_count; i++)
            {
                free(filtered_items[i]);
            }*/
            free(filtered_items);

            /* Free memory of items array */
            for (i = 0; i < item_count; i++)
            {
                free(items[i]);
            }
            free(items);

            item_count = filtered_count;
        }

        else if (choice == 5)
        {
            /* Read cache to make new items array */
            printf("\n\033[33mReading items from binary cache...\033[0m\n");
            items = NULL;
            items = read_binary_cache(&item_count);
            if (items == NULL){ /*to make sure no. of malloc  ==  no. of free*/
                printf("binary was not read properly, \n");
                return 1;
            }

            while (1)
            {
                printf("\nEnter quantity you would like to filter by: ");

                if (scanf("%d", &quantity) == 1)
                {
                    break;
                }
                else
                {
                    printf("Invalid input! Please enter a valid integer.\n");
                    while (getchar() != '\n')
                        ;
                }
            }
            filtered_items = filter_items_by_quantity(items, item_count, quantity, &filtered_count);

            if (filtered_items == NULL)
            {
                printf("Error: Could not allocate memory for filtering.\n");
            }
            else if (filtered_count > 0)
            {
                /* Write the filtered list to the binary cache */
                printf("\n\033[33mWriting filtered items to binary cache...\033[0m\n");
                write_binary_cache(filtered_items, &filtered_count);
            }
            else
            {
                printf("No items with quantity lesser than equal to this value.\n");
            }

            /* Free the filtered list (but NOT the original items) */
            /* for (i = 0; i < filtered_count; i++)
            {
                free(filtered_items[i]);
            }*/
            free(filtered_items);

            /* Free memory of items array */
            for (i = 0; i < item_count; i++)
            {
                free(items[i]);
            }
            free(items);

            item_count = filtered_count;
        }

        else if (choice == 6)
        {
            items = NULL;
            items = read_binary_cache(&item_count);
            printf("Exiting...\n");
            /* TODO: LaTeX conversion here*/
            /* sample testing -- zhenghan */ 
            to_latex(items, items, items, 2, 2, 2);

            for (i = 0; i < item_count; i++)
            {
                free(items[i]);
            }
            free(items);
            
            break;
        }
        else
        {
            printf("Invalid option! Please enter 1, 2, 3, 4, 5, or 6.\n");
            while (getchar() != '\n');
        }
    }



    /* =====This is a checker, To check if the sorting/filtering algorithm works. Read items from binary cache ====*/
    printf("\n\033[33mReading items from binary cache...\033[0m\n");
    items = NULL; 
    items = read_binary_cache(&item_count);
    
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


    /* ============================================================================================================ */


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
