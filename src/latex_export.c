#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "latex_export.h"
#include "item.h"

#define OUTPUT_FILE "./output/stock_report.text"


void write_table(FILE *file, Item **items, int num_rows, char* title)
{
    int i;

    fprintf(file, "\\section*{%s}\n", title);
    fprintf(file, "\\begin{longtable}{|p{0.4\\linewidth}|p{0.25\\linewidth}|p{0.25\\linewidth}|}");
    fprintf(file, "\\hline\n");
    fprintf(file, "\\textbf{Name} & \\textbf{Quantity} & \\textbf{Expiry Date}");
    fprintf(file, "%s\n", "\\\\");
    fprintf(file, "\\hline\n");

    for (i = 0; i < num_rows; i++)
    {
        fprintf(file, "%s & %d & %s %s\n", items[i]->item_name, items[i]->quantity, items[i]->expiry_date, "\\\\");
        /*printf("listing table rows....\n"); 
        printf("%s & %d & %s \n", items[i]->item_name, items[i]->quantity, items[i]->expiry_date);
        printf("end of table rows....\n"); */
        fprintf(file, "\\hline\n");
    }
   fprintf(file, "\\end{longtable}\n\n"); 
}

void to_latex(Item** input_low_stock, Item** input_expiring_stock, Item** input_sorted_stock,
              int num_low_stock, int num_expiring_stock, int num_sorted_stock)
{
    char latex_export_path[256];
    FILE *file;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(latex_export_path, sizeof(latex_export_path), "./output/stock_report_%02d%02d%04d-%02d%02d%02d.tex",
         t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
         t->tm_sec, t->tm_min, t->tm_hour);

    file = fopen(latex_export_path, "w");
        if (!file)
        {
            perror("Error opening file");
            return;
        }

    fprintf(file, "\\documentclass{article}\n");

    /* packages */
    fprintf(file, "\\usepackage[margin=1in]{geometry}\n\\usepackage{longtable}\n\\usepackage{datetime}\n\\usepackage{fancyhdr}\n"); 

    /* pages */
    fprintf(file, "\\pagestyle{fancy}\n\\fancyfoot[C]{}\n\\fancyfoot[L]{}\n\\fancyfoot[R]{\\thepage}\n\\renewcommand{\\headrulewidth}{0pt}");

    fprintf(file, "\\begin{document}\n");

    /* Title and date */
    fprintf(file, "\\noindent\n\\begin{minipage}[t]{0.48\\textwidth}\n\t\\raggedright\n\t\\LARGE \\textbf{Stock Report}\n\\end{minipage}\n");
    fprintf(file, "\\begin{minipage}[t]{0.48\\textwidth}\n\t\\raggedleft\n\t\\large \\today\n\\end{minipage}\n");
    fprintf(file, "\\vspace{1.5em}\n");

    write_table(file, input_low_stock, num_low_stock, "Low Stock Items");
    write_table(file, input_expiring_stock, num_expiring_stock, "Expiring Stock Items");
    write_table(file, input_sorted_stock, num_sorted_stock, "Sorted Stock Items");

    fprintf(file, "\\end{document}\n");

    fclose(file);
}
