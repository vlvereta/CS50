#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int count_letters(string text, int text_length);
int count_words(string text, int text_length);
int count_sentences(string text, int text_length);
int get_grade(int letters, int words, int sentences);
void print_result(int grade);

int main(void)
{
    string text = NULL;
    int text_length = 0;

    do
    {
        text = get_string("Text: ");
    }
    while (text == NULL);

    /**
     * Not an optimal way of gathering needed information but,
     * this flow was recommended by CS50's problem walkthough.
    */
    text_length = strlen(text);
    int letters = count_letters(text, text_length);
    int words = count_words(text, text_length);
    int sentences = count_sentences(text, text_length);

    int grade = get_grade(letters, words, sentences);
    print_result(grade);
}

// Each alphabetical character
int count_letters(string text, int text_length)
{
    int letters = 0;

    for (int i = 0; i < text_length; i++)
    {
        char c = tolower(text[i]);
        if (c >= 'a' && c <= 'z')
        {
            letters++;
        }
    }

    return letters;
}

// Any sequence of characters separated by a space
int count_words(string text, int text_length)
{
    int words = 0;
    bool initial_space = true; // as start of the text considered as start of the word

    for (int i = 0; i < text_length; i++)
    {
        if (text[i] != ' ' && initial_space)
        {
            words++;
            initial_space = false;
        }
        else if (text[i] == ' ' && !initial_space)
        {
            initial_space = true;
        }
    }

    return words;
}

// Any sequence of characters that ends with a . or a ! or a ?
int count_sentences(string text, int text_length)
{
    int sentences = 0;

    for (int i = 0; i < text_length; i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            sentences++;
        }
    }

    return sentences;
}

/**
 * Coleman-Liau index is computed using the formula:
 * index = 0.0588 * L - 0.296 * S - 15.8, where
 * L - the average number of letters per 100 words,
 * S - the average number of sentences per 100 words.
*/
int get_grade(int letters, int words, int sentences)
{
    // Get the average number of letters per 100 words
    float L = (float) letters / words * 100;

    // Get the average number of sentences per 100 words
    float S = (float) sentences / words * 100;

    // Return the result of the formula rounded to the nearest int
    return round(0.0588 * L - 0.296 * S - 15.8);
}

void print_result(int grade)
{
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}
