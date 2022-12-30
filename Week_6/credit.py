import re
from cs50 import get_string


def main():
    card_number = get_card_number()
    card_provider = get_card_provider(card_number)

    if is_card_number_valid(card_number) and card_provider:
        print(card_provider)
    else:
        print("INVALID")


# Get the card number
def get_card_number():
    while True:
        input = get_string("Number: ")

        # Check input for numbers, length will be checked later
        if (re.match("^[0-9]+$", input)):
            return input


# Algorithm invented by Hans Peter Luhn of IBM to validate the card number in place.
def is_card_number_valid(card_number):
    sum = 0

    i = 10
    while i < int(card_number):
        temp_digit = int(int(card_number) / i) % 10

        temp_digit *= 2

        sum += temp_digit if temp_digit < 10 else int((int(temp_digit) / 10 + int(temp_digit) % 10))

        i *= 100

    sum += int(card_number) % 10

    i = 100
    while i < int(card_number):
        sum += int(int(card_number) / i) % 10

        i *= 100

    return True if sum % 10 == 0 else False


# Get the card provider or None
def get_card_provider(card_number):
    card_number_length = len(card_number)

    if card_number_length == 13 and card_number[0] == "4":
        return "VISA"
    elif card_number_length == 15 and (card_number[0:2] == "34" or card_number[0:2] == "37"):
        return "AMEX"
    elif card_number_length == 16:
        if card_number[0] == "4":
            return "VISA"
        elif card_number[0] == "5" and re.match("[1-5]", card_number[1:2]):
            return "MASTERCARD"
    else:
        return None


main()
