#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"


void free_items(Item **items, int count){
/* 
Frees memory allocated for items
Note: This function does not modify count
*/
  int i;
  for (i = 0; i < count; i++) {
      free(items[i]);
  }
  free(items);
}

