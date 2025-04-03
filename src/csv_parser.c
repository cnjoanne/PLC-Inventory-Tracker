#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "csv_parser.h"

#include <regex.h>

#define MAX_LINE_LENGTH 200
#define MAX_ITEM_NAME_LENGTH 40
#define MAX_EXPIRY_DATE_LENGTH 11


/* TODO: Checks to ensure input csv contents are correct */
int quantity_is_valid(const char *quantity_str){
    int i;
    for (i = 0; i < strlen(quantity_str); i++){
        if (quantity_str[i] < 48 || quantity_str[i] > 57){
            printf("Invalid character. ");
            return 1;
        }
    }
    return 0;
}

int date_is_valid(const char *date){
    int year, month, day;
    int is_leap;
    if (sscanf(date, "%2d/%2d/%4d", &day, &month, &year) != 3){
        printf("Date format is invalid. ");
        return 1;
    }
    if (day < 1 || day > 31){ /* Day */
        printf("Date out of range. ");
        return 1;
    }
    if (month < 1 || month > 12){ /* Month */
        printf("Date out of range. ");
        return 1;
    }
    if ((month == 2 || month == 4 || month == 6 || month == 9 || month == 11) && day > 30){ /* For feb, apr, june, sep, nov*/
        printf("Date out of range. ");
        return 1;
    }
    if (month == 2){ /* For feb */
        is_leap = ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
        if (is_leap && day > 29){
            printf("Date out of range. ");
            return 1;
        }
        if (!is_leap && day > 28){
            printf("Date out of range. ");
            return 1;
        }
    }
    return 0;
}

int item_name_is_valid(const char *item_name){
    /* Only accepts alphabets, " ", "(", ")", ",", "-" */
    regex_t reegex;
    int value;

    const char *pattern = "^[A-Za-z][A-Za-z,() -]*[A-Za-z)]$";
    
    value = regcomp(&reegex, pattern, REG_EXTENDED); /* value = 0 when successful */
    if (value){
        fprintf(stderr, "Could not compile regex. ");
        return -1;
    }

    value = regexec(&reegex, item_name, 0, NULL, 0);
    regfree(&reegex);

    if (value == 0){
        return 0; /*valid string*/
    } else {
        /* printf("Invalid character in item name. "); */
        return -1;
    }

}

int data_is_valid(const char *item_name, const char *quantity_str, const char *expiry_date){
    int is_valid = 0;
    if (item_name_is_valid(item_name) != 0 ){
        printf("Invalid item name %s\n", item_name);
        is_valid = 1;
    }
    if (quantity_is_valid(quantity_str) != 0){
        printf("Invalid quantity %s\n", quantity_str);
        is_valid = 1;
    }
    if (date_is_valid(expiry_date) != 0){
        printf("Invalid expiry date %s\n", expiry_date);
        is_valid = 1;
    }
    return is_valid;
}

int parse_csv(const char *filepath, Item ***items, int *item_counter)
{
    FILE *input_file;
    int field_count, count, valid_count, i;
    char line[MAX_LINE_LENGTH];
    char *token;
    char item_name[MAX_ITEM_NAME_LENGTH];
    char quantity_str[11];
    char expiry_date[MAX_EXPIRY_DATE_LENGTH];

    /* if file exists */
    input_file = fopen(filepath, "r");
    if (!input_file)
    {
        perror("\033[31mError opening file\033[0m");
        printf("\033[34mTry again :,)\033[0m\n");
        return -1;
    }
    printf("\033[32mFile uploaded successfully\033[0m\n");

    /* check if csv contents isnt empty or has any error */
    if (fgets(line, sizeof(line), input_file) == NULL)
    {
        if (feof(input_file))
        {
            fprintf(stderr, "Error: File is empty. \n");
        }
        else
        {
            perror("Error reading file");
        }
        fclose(input_file);
        return -1;
    }

    /* get number of lines, excluding header */
    count = 0;
    while (fgets(line, sizeof(line), input_file)){
        count ++;
    }

    rewind(input_file);

    /* allocate memory for items array*/
    fgets(line, sizeof(line), input_file);   /* to ignore header line */
    *items = malloc(count * sizeof(Item *)); /* Allocates: {item0_ptr, item1_ptr, .. , itemn_ptr}*/
    if (!(*items))
    {
        perror("Memory allocation failed");
        return -1;
    }

    valid_count = 0;
    i = 0;
    while (fgets(line, sizeof(line), input_file) != NULL && i < count){
        field_count = 0; /* checks if all contents in line are present, it should be 3 */

        /* splits contents */
        token = strtok(line, ","); /* split "apple, 3, 29/01/2003", token = "apple" */
        if (token != NULL && strlen(token) > 0){
            field_count += 1;
            strncpy(item_name, token, MAX_ITEM_NAME_LENGTH - 1);
            item_name[MAX_ITEM_NAME_LENGTH - 1]= '\0';
        } else {
            item_name[0] = '\0';
        }
        token = strtok(NULL, ","); /* token = "3" */
        if (token != NULL && strlen(token) > 0){
            field_count += 1;
            strncpy(quantity_str, token, 11 - 1);
            quantity_str[11 - 1]= '\0';
        } else {
            quantity_str[0] = '\0';
        }
        token = strtok(NULL, ","); /* token =  "29/01/2003" */
        if (token !=NULL && strlen(token) > 0){
            field_count += 1;
            strncpy(expiry_date, token, MAX_EXPIRY_DATE_LENGTH - 1);
            expiry_date[MAX_EXPIRY_DATE_LENGTH - 1] = '\0';
        } else {
            expiry_date[0] = '\0';
        }

        /* check for valid input*/
        if (field_count != 3){ /* checks if all fields are filled per line */
            printf("\033[31mError: Not all columns are filled at %d\033[0m\n", i + 1);
            i++;
            continue;
        }

        if (data_is_valid(item_name, quantity_str, expiry_date) != 0){ /* if not valid, skip*/
            printf("\033[31mError: Invalid line %d is not included\033[0m\n", i + 1);
            i++;
            continue;
        }

        (*items)[valid_count] = (Item *)malloc(sizeof(Item)); /* allocate memory for struct Item*/
        if (!(*items)[valid_count]){
            perror("Memory allocation failed");
            return -1;
        }

        strcpy((*items)[valid_count]->item_name, item_name);
        (*items)[valid_count]->quantity = atoi(quantity_str);
        strcpy((*items)[valid_count]->expiry_date, expiry_date);

        valid_count++;
        i++;
    }

    *item_counter = valid_count;
    fclose(input_file);
    return 0;
}

int load_and_parse_csv(Item ***items, int *item_count)
{
    char filepath[256];
    int csv_is_valid;
    int i;

    print_upload_instructions();
    printf("Enter csv path: ");
    scanf("%255s", filepath);

    /* 
    Parse CSV file
    Note: global item_count updated here 
    */
    csv_is_valid = parse_csv(filepath, items, item_count); 

    /* Checks if CSV is valid */
    if (csv_is_valid == 0) {
        printf("\033[32mcsv is valid.\033[0m\n");
        printf("\033[32mSuccessfully parsed %d valid items:\033[0m\n", *item_count);

        for (i = 0; i < *item_count; i++) {
            printf("Item: %s, Quantity: %d, Expiry Date: %s\n",
                   (*items)[i]->item_name, (*items)[i]->quantity, (*items)[i]->expiry_date);
        }
        return 1;
    } else {
        printf("\033[34mcsv is not valid.\033[0m\n");
        return 0;
    }
}


/* Input: input.csv file pointer*/
/* Output: returns array(point of array) of items*/
