#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"

#include <time.h> 

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

char get_time(){
  time_t t;
  struct tm *tmp ;
  char current_date[11];
  time(&t);

  tmp = localtime(&t);

  strftime(current_date, sizeof(current_date), "%d/%m/%Y", tmp);

  return current_date;
}

