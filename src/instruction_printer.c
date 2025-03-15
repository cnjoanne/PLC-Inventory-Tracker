#include <stdio.h>

/* process input csv */
void print_upload_instructions(void)
{
    printf("\033[34mHello! To upload csv path, enter the file path below.\033[0m\n");
    printf("\033[34mExample: ../data/input.csv\033[0m\n");
}

void print_user_instructions(void)
{
    /* TODO: Add on instructions */
    printf("===============================================\n");
    printf("Instruction for user....\n");
    printf("To sort items by name, Enter: 1.\n");
    printf("To sort items by quantity, Enter: 2.\n");
    printf("To sort items by expiry date, Enter: 3.\n");

    printf("To filter items by expiry date, Enter: 4.\n");
    printf("To filter items by quality limit, Enter: 5.\n");
    printf("To quit (which will save as LaTeX), Enter: 6.\n");
}
