#include <cs50.h>
#include <stdio.h>

/**
 * Mario's pyramids using hashes `#`.
 * Problem homepage: https://cs50.harvard.edu/x/2022/psets/1/mario/more/
*/

void print_char(int num, char c);

int main(void)
{
    int height;

    // Get the height with validation
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // Print out lines of the pyramid one by one
    for (int i = 1; i <= height; i++)
    {
        int spaces = height - i;
        int hashes = i; // the same amount as the row number

        print_char(spaces, ' ');
        print_char(hashes, '#');
        printf("  ");
        print_char(hashes, '#');
        printf("\n");
    }

    return 0;
}

void print_char(int num, char c)
{
    for (int i = 0; i < num; i++)
    {
        printf("%c", c);
    }
}
