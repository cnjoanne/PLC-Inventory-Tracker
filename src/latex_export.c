
/* TODO: export LaTeX as output in ./data */
/* Input: array(pointer of array) of items */
/* Saves LaTeX file in ./data/output.tex */

#include <stdio.h>
#include <stdlib.h>
#include "latex_export.h"
#include "item.h"
int i;
void write_table(FILE *file, Item **items, int num_rows, char* title)
{
    fprintf(file, "\\section*{%s}\n", title);
    fprintf(file, "\\begin{tabular}{|c|c|c|}\n");
    fprintf(file, "\\hline\n");
    fprintf(file, "Name & Quantity & Expiry Date");
    fprintf(file, "%s\n", "\\\\");
    fprintf(file, "\\hline\n");

    for (i = 0; i < num_rows; i++)
    {
        fprintf(file, "%s & %d & %s %s\n", items[i]->item_name, items[i]->quantity, items[i]->expiry_date, "\\\\");
        printf("listing table rows....\n");
        printf("%s & %d & %s \n", items[i]->item_name, items[i]->quantity, items[i]->expiry_date);
        printf("end of table rows....\n");
        fprintf(file, "\\hline\n");
    }

    fprintf(file, "\\end{tabular}\n\n");
}

void to_latex(Item** input_low_stock, Item** input_expiring_stock, Item** input_sorted_stock,
              int num_low_stock, int num_expiring_stock, int num_sorted_stock)
{
    FILE *file = fopen("stock_report.tex", "w");
    if (!file)
    {
        perror("Error opening file");
        return;
    }

    fprintf(file, "\\documentclass{article}\n\\begin{document}\n");

    write_table(file, input_low_stock, num_low_stock, "Low Stock Items");
    write_table(file, input_expiring_stock, num_expiring_stock, "Expiring Stock Items");
    write_table(file, input_sorted_stock, num_sorted_stock, "Sorted Stock Items");

    fprintf(file, "\\end{document}\n");

    fclose(file);
}