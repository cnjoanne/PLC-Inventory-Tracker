#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"

/* process input csv */
void print_upload_instructions(void){
    printf("\033[34mHello! To upload csv path, enter the file path below.\033[0m\n");
    printf("\033[34mExample: ../data/input.csv\033[0m\n");
}

 

/* TODO: Checks to ensure input csv contents are correct */

int check_input_validity(const char *filepath, Item **items, int *item_counter){
    FILE *input_file;

    /* if file exists */
    input_file = fopen(filepath, "r");
    if (!input_file){
        perror("\033[31mError opening file\033[0m");
        printf("\033[34mTry again :,)\033[0m\n");
        fclose(input_file);
        return -1;
    }
    printf("\033[32mFile uploaded successfully\033[0m\n");

    /* get number of lines, excluding header */

    /* if contents in file are valid */


    fclose(input_file);
    return 0;
}


int parse_csv()

/* TODO: create array (pointer to array) to store many items */

/* Input: input.csv file pointer*/
/* Output: returns array(point of array) of items*/


