#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/item.h"
#include "../include/binary_cache.h"
#include "../include/csv_parser.h"


int main(){
    // char filepath[256];
    // FILE *input_file;
    
    // /* uploading input csv */
    // print_upload_instructions();
    // printf("Enter csv path: ");
    // scanf("%255s", filepath);

    // /* check if file exists */
    // input_file = fopen(filepath, "r");
    // if (!input_file){
    //     perror("\033[31mError opening file\033[0m");
    //     printf("\033[34mTry again :,)\033[0m\n");
    //     return 1;
    // }
    // printf("\033[32mFile uploaded successfully\033[0m\n");

    // /* continue with process....*/
    
    // fclose(input_file);

    // return 0;

    /* Dummy data generation for binary_cache.c file test */

    int number_items,i;
    Item *item1, *item2;
    Item **read_items;
    Item *items[2];
   

    /* Create test items */
    item1 = malloc(sizeof(Item));
    snprintf(item1->item_name, sizeof(item1->item_name), "Blue Jazz");
    item1->quantity = 29;
    snprintf(item1->expiry_date, sizeof(item1->expiry_date), "22/06/2032");

    item2 = malloc(sizeof(Item));
    snprintf(item2->item_name, sizeof(item2->item_name), "Cauliflower");
    item2->quantity = 58;
    snprintf(item2->expiry_date, sizeof(item2->expiry_date), "20/11/2034");

    /* Create an array of item pointers */
    items[0] = item1;
    items[1] = item2;
    number_items = 2;

    /* Write items to the binary cache */
    printf("\n\033[33mWriting items to binary cache...\033[0m\n");
    write_binary_cache(items, number_items);

    /* Read items from binary cache */
    printf("\n\033[33mReading items from binary cache...\033[0m\n");
    read_items = read_binary_cache();

    /* Verify the data read */
    if (read_items) {
        printf("\n\033[32mItems Read from Binary Cache:\033[0m\n");
        for (i = 0; i < number_items; i++) {
            printf("Item Name: %s\n", read_items[i]->item_name);
            printf("Quantity: %d\n", read_items[i]->quantity);
            printf("Expiry Date: %s\n\n", read_items[i]->expiry_date);
            free(read_items[i]); 
        }
        free(read_items); 
    } else {
        printf("\033[31mFailed to read from binary cache\033[0m\n");
    }

    /* Free original test items */
    free(item1);
    free(item2);

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