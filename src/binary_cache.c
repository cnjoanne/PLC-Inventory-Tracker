#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"


void write_binary_cache(Item **items, int *item_count){
    /* 
    Converts array of pointers (items) to binary and writes to cache folder 
    Input: array(pointer of array) of items, count of valid num of items
    Output: void
    */

    /* Variables*/
    int i;
   
    /* create pointer to binary cache file */
    FILE *bin_cache;

    /* buffer to store path of binary cache file */
    #ifdef _WIN32 /* Windows */
        char bin_cache_path[256] = "./data/cache/bin_cache.bin";
    #else
        char bin_cache_path[256] = "../data/cache/bin_cache.bin";
    #endif
    

    /* open binary cache file in write mode. If it doesn't exist, create it */
    bin_cache = fopen(bin_cache_path, "wb");

    /* error handling */
    if (!bin_cache){
        perror("\033[31mError opening file\033[0m");
        printf("\033[34mTry again :,)\033[0m\n");
        return;
    }

    /* TODO: Ensure it properly overwrites binary cache file if there was another content stored inside -cn*/
    /* write items to binary cache file */
    for (i=0; i < *item_count; i++){
        fwrite(items[i], sizeof(Item), 1, bin_cache);
    }

    /* close binary cache file */
    fclose(bin_cache);
    printf("\033[32mBinary cache written successfully\033[0m\n");
}

Item ** read_binary_cache(int *item_count){
    /* 
    Read binary cache file and convert to array of pointers
    Input: pointer to array of items, pointer to 
    Output: array of pointers to items
    */

    /* Variables */
    int i, j, num_items;
    long file_size;
    Item ** items;


    /* create pointer to binary cache file */
    FILE *bin_cache;

    /* buffer to store path of binary cache file*/
    #ifdef _WIN32 /* Windows */
        char bin_cache_path[256] = "./data/cache/bin_cache.bin";
    #else
        char bin_cache_path[256] = "../data/cache/bin_cache.bin"; 
    #endif

    /* open binary cache file in read mode */
    bin_cache = fopen(bin_cache_path, "rb");

    /* error handling */
    if (!bin_cache){
        perror("\033[31mError opening file\033[0m");
        printf("\033[34mTry again :,)\033[0m\n");
        return NULL;
    }

    /* Move file pointer to end of file to get file size */
    fseek(bin_cache, 0, SEEK_END);
    file_size = ftell(bin_cache);

    /* Move file pointer back to beginning of file */
    rewind(bin_cache);
 
    /*  Calculate number of items */
    num_items = file_size / sizeof(Item);

    /* error handling */
    if (num_items == 0) {
        fclose(bin_cache);
        return NULL; 
    }

    /* error handling */ 
    if (num_items != *item_count){
        printf("Number of items read not equal to original number of valid items\n");
        return NULL;
    }

    /* Allocate memory for an array of pointers */
    items = malloc(num_items * sizeof(Item *));

    /* error handling */
    if (!items) {
        fclose(bin_cache);
        return NULL;
    }

    /*  Read items into allocated structs */
    for (i = 0; i < num_items; i++) {

        /*  Allocate memory for each item */
        (items)[i] = malloc(sizeof(Item));
        
        /*  error handling */
        if (!items[i]) {
            for (j = 0; j < i; j++) {
                free(items[j]); 
            }
            free(items);
            fclose(bin_cache);
            return NULL;
        }
         
        /* Read item from binary cache file */
        if (fread(items[i], sizeof(Item), 1, bin_cache) != 1) {
            fprintf(stderr, "Error reading item %d from binary cache\n", i);
        }
    }

    fclose(bin_cache);
    return items;
}
