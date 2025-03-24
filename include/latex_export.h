#ifndef LATEX_EXPORT_H
#define LATEX_EXPORT_H
#include "item.h"

/* void write_table(FILE *file, const StockItem *items, int num_rows, const char *title);*/
void to_latex(Item** input_low_stock, Item** input_expiring_stock, Item** input_sorted_stock,
    int num_low_stock, int num_expiring_stock, int num_sorted_stock);

#endif