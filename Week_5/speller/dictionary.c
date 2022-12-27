// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Choose number of buckets in hash table
const unsigned int N = LENGTH;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hash_value = hash(word);

    // Create a lowercased word duplication for case-insensitive comparison
    char word_dup[LENGTH + 1];
    memset(word_dup, '\0', LENGTH + 1);
    for (int i = 0; i < strlen(word); i++)
    {
        word_dup[i] = tolower(word[i]);
    }

    // Check the hash table for the provided word
    node *current = table[hash_value];
    while (current != NULL)
    {
        if (strcmp(word_dup, current->word) == 0)
        {
            return true;
        }
        current = current->next;
    }

    return false;
}

/**
 * Hashes word to a number;
 * Kinda experimental hash function:
 * 1. Takes an average char number in the word;
 * 2. Adds length of the word, divides with remainder.
*/
unsigned int hash(const char *word)
{
    int sum = 0;
    int length = strlen(word);

    for (int i = 0; i < length; i++)
    {
        sum += toupper(word[i]) - 'A';
    }

    return ((sum / length) + length) % LENGTH;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *file = fopen(dictionary, "r");
    if (!file)
    {
        return false;
    }

    // Clear the table
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Read the dictionary
    int read;
    int word_index = 0;
    char word[LENGTH + 1];
    memset(word, '\0', LENGTH + 1);
    char buffer[LENGTH + 1];
    while ((read = fread(&buffer, sizeof(char), LENGTH + 1, file)))
    {
        for (int i = 0; i < read; i++)
        {
            if (buffer[i] == '\n')
            {
                if (strlen(word) != 0)
                {
                    int hash_value = hash(word);

                    node *new_node = malloc(sizeof(node));
                    if (!new_node)
                    {
                        return false;
                    }

                    strcpy(new_node->word, word);
                    new_node->next = table[hash_value];
                    table[hash_value] = new_node;

                    word_index = 0;
                    memset(word, '\0', LENGTH + 1);
                }
            }
            else
            {
                word[word_index++] = buffer[i];
            }
        }
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    int total = 0;
    for (int i = 0; i < N; i++)
    {
        node *current = table[i];
        while (current != NULL)
        {
            total++;
            current = current->next;
        }
    }

    return total;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *current = table[i];
        while (current != NULL)
        {
            node *previous = current;
            current = current->next;
            free(previous);
        }
    }

    return true;
}
