#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "item.h"
#include "csv_parser.h"

#define MAX_LINE_LENGTH 200
#define MAX_ITEM_NAME_LENGTH 40
#define MAX_EXPIRY_DATE_LENGTH 11


/* Checks to ensure input csv contents are correct */
int quantity_is_valid(const char *quantity_str){
    int i;
    for (i = 0; i < strlen(quantity_str); i++){
        if (quantity_str[i] < 48 || quantity_str[i] > 57){
            printf("Invalid character. ");
            return 0;
        }
    }
    return 1;
}

int date_is_valid(const char *date){
    int year, month, day;
    int is_leap;
    char c;

    typedef enum {
        START,
        DAY1_VALID, DAY2_VALID,
        SEP1_VALID,
        MONTH1_VALID, MONTH2_VALID, 
        SEP2_VALID, 
        YEAR1_VALID, YEAR2_VALID, YEAR3_VALID, YEAR4_VALID, 
        DATE_VALID, 
        INVALID_INPUT
    } State;

    State state = START;
    day = 0;
    month = 0;
    year = 0;

    while (*date && state != INVALID_INPUT){
        c = *date++;
        switch(state){
            case START:
                if (c >= '0' && c <= '9'){
                    state = DAY1_VALID;
                    day = 10 * (c - '0');
                } else {
                    state = INVALID_INPUT;
                }
                break;
            case DAY1_VALID:
                if (c >= '0' && c <= '9'){
                    state = DAY2_VALID;
                    day += c - '0';
                } else {
                    state = INVALID_INPUT;
                }
                break;
            case DAY2_VALID:
                if (c == '/'){
                    state = SEP1_VALID;
                } else {
                    state = INVALID_INPUT;
                }
                break;
            case SEP1_VALID:
                if (c >= '0' && c <= '9'){
                    state = MONTH1_VALID;
                    month = 10 * (c - '0');
                } else {
                    state = INVALID_INPUT;
                }
                break;
            case MONTH1_VALID:
                if (c >= '0' && c <= '9'){
                    state = MONTH2_VALID;
                    month += c - '0';
                } else {
                    state = INVALID_INPUT;
                }
                break;
            case MONTH2_VALID:
                if (c == '/'){
                    state = SEP2_VALID;
                } else {
                    state = INVALID_INPUT;
                }
                break;
            case SEP2_VALID:
                if (c >= '0' && c <= '9'){
                    state = YEAR1_VALID;
                    year = 1000 * (c - '0');
                } else {
                    state = INVALID_INPUT;
                }
                break;
            case YEAR1_VALID:
                if (c >= '0' && c <= '9'){
                    state = YEAR2_VALID;
                    year += 100 * (c - '0');
                } else {
                    state = INVALID_INPUT;
                }
                break;
            case YEAR2_VALID:
                if (c >= '0' && c <= '9'){
                    state = YEAR3_VALID;
                    year += 10 * (c - '0');
                } else {
                    state = INVALID_INPUT;
                }
                break;
            case YEAR3_VALID:
                if (c >= '0' && c <= '9'){
                    state = YEAR4_VALID;
                    year += c - '0';
                } else {
                    state = INVALID_INPUT;
                }
                break;
            case YEAR4_VALID:
                if (c == '\0'){
                    state = DATE_VALID;
                } else {
                    state = INVALID_INPUT;
                }
                break;
            case INVALID_INPUT:
                printf("Date format is invalid. ");
                return 0;
            default:
                state = INVALID_INPUT;
        }   
    }
    /* if (sscanf(date, "%2d/%2d/%4d", &day, &month, &year) != 3){
        printf("Date format is invalid. ");
        return 0;
    } */
    if (day < 1 || day > 31){ /* Day */
        printf("Date out of range. ");
        return 0;
    }
    if (month < 1 || month > 12){ /* Month */
        printf("Date out of range. ");
        return 0;
    }
    if ((month == 2 || month == 4 || month == 6 || month == 9 || month == 11) && day > 30){ /* For feb, apr, june, sep, nov*/
        printf("Date out of range. ");
        return 0;
    }
    if (month == 2){ /* For feb */
        is_leap = ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
        if (is_leap && day > 29){
            printf("Date out of range. ");
            return 0;
        }
        if (!is_leap && day > 28){
            printf("Date out of range. ");
            return 0;
        }
    }
    return 1;
}

int item_name_is_valid(const char *item_name){
    /* Only accepts alphabets, " ", "(", ")", ",", "-" */
    regex_t reegex;
    int value;

    const char *pattern = "^[A-Za-z][A-Za-z,() -']*[A-Za-z)]$";
    
    value = regcomp(&reegex, pattern, REG_EXTENDED); /* value = 0 when successful */
    if (value){
        fprintf(stderr, "Could not compile regex for item name. ");
        return 0;
    }

    value = regexec(&reegex, item_name, 0, NULL, 0);
    regfree(&reegex);

    if (value == 0){
        return 1; /*valid string*/
    } else {
        /* printf("Invalid character in item name. "); */
        return 0;
    }

}

int data_is_valid(const char *item_name, const char *quantity_str, const char *expiry_date){
    int is_valid = 1;
    if (item_name_is_valid(item_name) != 1){
        printf("Invalid item name %s\n", item_name);
        is_valid = 0;
    }
    if (quantity_is_valid(quantity_str) != 1){
        printf("Invalid quantity %s\n", quantity_str);
        is_valid = 0;
    }
    if (date_is_valid(expiry_date) != 1){
        printf("Invalid expiry date %s\n", expiry_date);
        is_valid = 0;
    }
    return is_valid;
}

/* Parses csv, 
- stores contents in file in items array
- updates item_counter
- returns 1 if csv is parsed, 
- returns 0 if csv parsing has failed */
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
            printf("\033[31mError: Not all columns are filled at csv line %d, hence not included\033[0m\n", i + 1);
            i++;
            continue;
        }

        if (data_is_valid(item_name, quantity_str, expiry_date) != 1){ /* if not valid, skip*/
            printf("\033[31mError: Invalid csv line %d is not included for above issue\033[0m\n", i + 1);
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

    if (valid_count <= 0){
        printf("\033[31mcsv file is empty\033[0m\n");
        return 0;
    }

    *item_counter = valid_count;
    fclose(input_file);
    return 1;
}

int load_and_parse_csv(Item ***items, int *item_count)
{
    char filepath[256];
    int csv_is_valid;
    int i;

    print_upload_instructions();
    
    while (1){
        
        printf("Enter csv path: ");
        scanf("%255s", filepath);

        /* quitting */
        if (strcmp(filepath, "quit") == 0) {
            printf("\033[31mquitting...\033[0m\n");
            return 0;
        }

        /* Parse CSV file
        Note: item_count updated here */
        csv_is_valid = parse_csv(filepath, items, item_count); 

        /* Checks if CSV is valid */
        if (csv_is_valid == 1) {
            printf("\033[32mcsv is valid.\033[0m\n");
            printf("\033[32mSuccessfully parsed %d valid items:\033[0m\n", *item_count);

            for (i = 0; i < *item_count; i++) {
                printf("Item: %s, Quantity: %d, Expiry Date: %s\n",
                       (*items)[i]->item_name, (*items)[i]->quantity, (*items)[i]->expiry_date);
            }
            return 1;
        } else {
            printf("\033[31mcsv is not valid.\033[0m\n");
            printf("\033[34mTry again :,) or to quit, enter: quit\033[0m\n");
        }
    }


}
