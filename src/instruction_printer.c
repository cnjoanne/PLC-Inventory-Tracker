#include <stdio.h>


/* process input csv */
void print_upload_instructions(void)
{
    printf("\033[34mHello! To upload csv path, enter the file path below.\033[0m\n");
    printf("\033[34mExample: ./data/input_01.csv \033[0m\n");
    printf("\033[34mTo quit, enter: quit\033[0m\n");
}

/* instructions */
void print_user_instructions(void)
{
    printf("===============================================\n");
    printf("\033[34mInstruction for user....\033[0m\n");
    printf("\033[34mTo sort items by name, Enter: 1.\033[0m\n");
    printf("\033[34mTo sort items by quantity, Enter: 2.\033[0m\n");
    printf("\033[34mTo sort items by expiry date, Enter: 3.\033[0m\n");

    printf("\033[34mTo filter items by expiry date, Enter: 4.\033[0m\n");
    printf("\033[34mTo filter items by quantity limit, Enter: 5.\033[0m\n");
    printf("\033[34mTo quit (which will save as LaTeX), Enter: 6.\033[0m\n");
}
