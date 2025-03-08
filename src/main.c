#include <stdio.h>
#include <stdlib.h>
#include "main.h"

/* TODO: process input csv [./csv_parser.c] */

/* TODO: Checks to ensure input csv contents are correct [./csv_parser.c] */

/* TODO: create struct of item: item_name; quantity; expiry_date  [./csv_parser.c] 
typedef struct{
    char *item_name;
    int quantity;
    char expiry_date[11]; 
} Item; */

/* TODO: create array (pointer to array) to store many items [./csv_parser.c] */

/* TODO: convert data into binary, and store in cache folder [./binary_cache.c] */

/* TODO: process user's commands, eg. sort item, filter, export, quit [./main.c]*/

/* TODO: read binary cache [./binary_cache.c] */

/* TODO: sorting logic for item_name, quantity, expiry_date [in ./sorting.c]*/

/* TODO: filter logic for low quantity, expiry_date [in ./filtering.c]*/

/* TODO: write binary, overwrite previous binary cache [./binary_cache.c] */

/* TODO: write binary cache into LaTeX [in ./latex_export.c]*/

/* TODO: export LaTeX as output in ./data [in ./latex_export.c]*/

int main(){
    char filepath[256];
    FILE *input_file;
    
    /* uploading input csv */
    print_instructions();
    printf("Enter csv path: ");
    scanf("%255s", filepath);

    /* check if file exists */
    input_file = fopen(filepath, "r");
    if (!input_file){
        perror("\033[31mError opening file\033[0m");
        printf("\033[34mTry again :,)\033[0m\n");
        return 1;
    }
    
    printf("\033[32mFile uploaded successfully\033[0m\n");
    fclose(input_file);

    return 0;
}


void print_instructions(void){
    printf("\033[34mHello! To upload csv path, enter the file path below.\033[0m\n");
    printf("\033[34mExample: ../data/input.csv\033[0m\n");
}