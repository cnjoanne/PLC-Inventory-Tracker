#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"

#define MAX_LINE_LENGTH 200
#define MAX_ITEM_NAME_LENGTH 40
#define MAX_EXPIRY_DATE_LENGTH 11

/* process input csv */
void print_upload_instructions(void){
    printf("\033[34mHello! To upload csv path, enter the file path below.\033[0m\n");
    printf("\033[34mExample: ../data/input.csv\033[0m\n");
}

void print_user_instructions(void){
    /* TODO: Add on instructions */
    printf("===============================================\n");
    printf("Instruction for user....\n");
    printf("To sort item by name, Enter: __.\n");
    printf("To sort item by quantity, Enter: __.\n");
    printf("To sort item by expiry date, Enter: __.\n");

    printf("To filter item by expiry date, Enter:__.\n");
    printf("To filter item by quality limit, Enter:__.\n");
    printf("To quit, Enter: 'quit'\n");
}
 

/* TODO: Checks to ensure input csv contents are correct */
int is_quantity_valid(void){

    return 0;
}

int is_date_valid(const char *date){
    int year, month, day;
    if (sscanf(date, "%2d-%2d-%4d", &day, &month, &year) != 3){
        return 0;
    }
    return 0;
}

int is_item_name_valid(void){

    return 0;
}

int is_data_valid(){
    
    return 0;
}

int parse_csv(const char *filepath, Item ***items, int *item_counter){
    FILE *input_file;
    int count;
    char line[MAX_LINE_LENGTH];
    int i;
    char *token;

    /* if file exists */
    input_file = fopen(filepath, "r");
    if (!input_file){
        perror("\033[31mError opening file\033[0m");
        printf("\033[34mTry again :,)\033[0m\n");
        return -1;
    }
    printf("\033[32mFile uploaded successfully\033[0m\n");

    /* check if csv contents isnt empty or has any error */
    if (fgets(line, sizeof(line), input_file) == NULL){
        if (feof(input_file)){
            fprintf(stderr, "Error: File is empty. \n");
        } else {
            perror("Error reading file");
        }
        fclose(input_file);
        return -1;
    }

    /* check contents are valid and get number of lines, excluding header */
    count = 0;
    while (fgets(line, sizeof(line), input_file)){
        /* TODO: Add checks for content of csv*/
        count ++;
    }
    rewind(input_file);

    /* allocate memory for items array*/
    fgets(line, sizeof(line), input_file); /* to ignore header line */
    *items = malloc(count * sizeof(Item *)); /* Allocates: {item0_ptr, item1_ptr, .. , itemn_ptr}*/
    if (!(*items)){
        perror("Memory allocation failed");
        return -1;
    }

    i = 0;
    while (fgets(line, sizeof(line), input_file) && i < count){
        (*items)[i] = (Item *)malloc(sizeof(Item)); /* allocate memory for struct Item*/
        if (!(*items)[i]){
            perror("Memory allocation failed");
            return -1;
        }
        token = strtok(line, ","); /* split "apple, 3, 29-01-2003", token = "apple" */
        if (token != NULL){
            strncpy((*items)[i]->item_name, token, MAX_ITEM_NAME_LENGTH);
            (*items)[i]->item_name[sizeof((*items)[i]->item_name) - 1] = '\0';
        }
        token = strtok(NULL, ","); /* token = "3" */
        if (token != NULL){
            (*items)[i]->quantity = atoi(token); 
        }
        token = strtok(NULL, ","); /* token =  "29-01-2003" */
        if (token !=NULL){ 
            strncpy((*items)[i]->expiry_date, token, MAX_EXPIRY_DATE_LENGTH);
            (*items)[i]->expiry_date[sizeof((*items)[i]->expiry_date) - 1] = '\0';
        }
        i++;
    }

    *item_counter = count;
    fclose(input_file);
    return 0;
}

/* Input: input.csv file pointer*/
/* Output: returns array(point of array) of items*/


