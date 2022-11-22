#include <stdio.h>
#include <cs50.h>

/**
 * Write a program in such a way that it first prompts the user for their name and then prints hello,
 * so-and-so, where so-and-so is their actual name.
 * Problem homepage: https://cs50.harvard.edu/x/2022/psets/1/hello/
*/

int main(void)
{
    string name = get_string("What's your name? ");
    printf("hello, %s\n", name);
}
