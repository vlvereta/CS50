#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/**
 * Write a program called substitution that enables you to encrypt messages using a substitution cipher.
 * At the time the user executes the program, they should decide, by providing a command-line argument,
 * on what the key should be in the secret message they’ll provide at runtime.
 * Problem homepage: https://cs50.harvard.edu/x/2022/psets/2/substitution/
*/

// Length of the provided key
const int KEY_LENGTH = 26;

bool validate_args(int argc, char *argv[]);
bool is_key_valid(char *key);
void produce_result(char *plain_text, char *key);

int main(int argc, char *argv[])
{
    char *key = NULL;

    // Validation and return in case of error
    if (validate_args(argc, argv))
    {
        key = argv[1];
    }
    else
    {
        return 1;
    }

    // Get the text from the user
    char *plain_text = get_string("plaintext: ");

    // Print the result in realtime
    produce_result(plain_text, key);
}

// Check the args and print the error in case of some
bool validate_args(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return false;
    }

    if (strlen(argv[1]) != KEY_LENGTH)
    {
        printf("Key must contain %d characters.\n", KEY_LENGTH);
        return false;
    }
    else if (!is_key_valid(argv[1]))
    {
        printf("Key must consists only of alpha characters without duplicates!\n");
        return false;
    }

    return true;
}

/**
 * Key is invalid as by:
 * - not containing 26 characters,
 * - containing any character that is not an alphabetic character,
 * - not containing each letter exactly once
*/
bool is_key_valid(char *key)
{
    char lowered_key[KEY_LENGTH];

    for (int i = 0; i < KEY_LENGTH; i++)
    {
        if (!isalpha(key[i]) || strchr(lowered_key, tolower(key[i])) != NULL)
        {
            return false;
        }
        lowered_key[i] = tolower(key[i]);
    }

    return true;
}

void produce_result(char *plain_text, char *key)
{
    printf("ciphertext: ");

    for (int i = 0; i < strlen(plain_text); i++)
    {
        if (isalpha(plain_text[i]))
        {
            /**
             * Get the char's position in the key string by subtraction
             * first alphabet char 'a' from the lowered letter of the plain text
            */
            int position = tolower(plain_text[i]) - 'a';

            // Get the new case sensitive char from a key
            char new_char = islower(plain_text[i]) ? tolower(key[position]) : toupper(key[position]);
            printf("%c", new_char);
        }
        else
        {
            printf("%c", plain_text[i]);
        }
    }

    printf("\n");
}
