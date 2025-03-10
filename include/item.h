#ifndef ITEM_H
#define ITEM_H

typedef struct{
    char item_name[40];
    int quantity;
    char expiry_date[11];  /*dd-mm-yyyy*/
} Item;

#endif