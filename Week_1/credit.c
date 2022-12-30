#include <cs50.h>
#include <stdio.h>
#include <math.h>

char *get_card_provider_by_number(long card_number);
int get_digits_amount(long card_number);
bool is_card_number_valid(long card_number);

int main(void)
{
    long card_number = get_long("Number: ");

    // Check provider
    char *card_provider = get_card_provider_by_number(card_number);

    // Check validity
    if (card_provider != NULL && is_card_number_valid(card_number))
    {
        printf("%s\n", card_provider);
    }
    else
    {
        printf("INVALID\n");
    }

    return 0;
}

/**
 * Gets the card_number in long
 * and
 * returns the card provider string or NULL.
*/
char *get_card_provider_by_number(long card_number)
{
    int digits_amount = get_digits_amount(card_number);

    switch (digits_amount)
    {
        // VISA with 4 start digit
        case 13 :
        {
            if ((card_number / (long)pow(10, 12)) == 4)
            {
                return "VISA";
            }
            else
            {
                return NULL;
            }
        }

        // AMEX with 34 or 37 start sequence
        case 15 :
        {
            int start_sequence = card_number / (long)pow(10, 13);

            if (start_sequence == 34 || start_sequence == 37)
            {
                return "AMEX";
            }
            else
            {
                return NULL;
            }
        }

        /*
         * VISA with 4 start digit
         * or
         * MASTERCARD with 51, 52, 53, 54, or 55 start sequence
         */
        case 16 :
        {
            int start_sequence = card_number / (long)pow(10, 14);

            if ((card_number / (long)pow(10, 15)) == 4)
            {
                return "VISA";
            }
            else if (start_sequence == 51 || start_sequence == 52 || start_sequence == 53 || start_sequence == 54 || start_sequence == 55)
            {
                return "MASTERCARD";
            }
            else
            {
                return NULL;
            }
        }

        default :
            return NULL;

    }
}

int get_digits_amount(long card_number)
{
    if (card_number >= 10)
    {
        int amount = 1;
        for (long i = 10; i <= card_number; i *= 10)
        {
            amount++;
        }
        return amount;
    }
    return 1;
}

/**
 * Algorithm invented by Hans Peter Luhn of IBM to validate the card number in place:
 * 1. Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together;
 * 2. Add the sum to the sum of the digits that weren’t multiplied by 2;
 * 3. If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!
*/
bool is_card_number_valid(long card_number)
{
    long sum = 0;

    // 1. Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together
    for (long i = 10; i < card_number; i *= 100)
    {
        // Getting every other digit starting with the number's second-to-last digit
        long temp_digit = (card_number / i) % 10;

        // Multiply digit by 2
        temp_digit *= 2;

        // Adding those products’ digits together (e.g. 18 => 1 + 8)
        sum += temp_digit < 10 ? temp_digit : (temp_digit / 10 + temp_digit % 10);
    }

    // 2. Add the sum to the sum of the digits that weren’t multiplied by 2
    sum += card_number % 10; // adding last number
    for (long i = 100; i < card_number; i *= 100)
    {
        sum += (card_number / i) % 10; // adding every other digit starting with the number's third-to-last digit
    }

    // 3. If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!
    return sum % 10 == 0 ? true : false;
}
