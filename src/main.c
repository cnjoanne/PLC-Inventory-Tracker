#include <stdio.h>
#include <stdlib.h>
#include "csv_parser.h"

int main(){
    char filepath[256];
    Item **items; 
    int *item_counter;
    int csv_is_valid;
    int i;
    
    /* uploading input csv */
    print_upload_instructions();
    printf("Enter csv path: ");
    scanf("%255s", filepath);

    
    /* continue with process....*/
    item_counter = malloc(sizeof(int));
    items = NULL;
    csv_is_valid = parse_csv(filepath, &items, item_counter);
    
    if (csv_is_valid == 0){
        printf("csv is valid.\n");
    } else{
        printf("csv is not valid.\n");
        return 1;

    }
    /* =====This is a checker, can remove if you are sure of the struct ====*/
    if (csv_is_valid == 0){
        printf("Successfully parsed %d valid items:\n", *item_counter);

        for (i = 0; i < *item_counter; i++) {
            printf("Item: %s, Quantity: %d, Expiry Date: %s\n",
                   (items)[i]->item_name, (items)[i]->quantity, (items)[i]->expiry_date);
        }
    }
    /* ==================================================================== */

    
    /* TODO: store as binary cache*/

    /* TODO: Free memory, arrange with dan */
    for (i = 0; i < *item_counter; i++) {
        free(items[i]);
    }
    free(items);
    free(item_counter);

    
    /* Print instructions eg. enter sort name to sort name alphabetically */
    print_user_instructions();

    /*while (!0){
        printf("Enter your instructions: ");
    
    }*/

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