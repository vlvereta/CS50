from cs50 import get_string

# Write a program called readability that takes a text and determines its reading level.
# First version of the program was written on C and can be found here in the Week 2.
# Problem homepage: https://cs50.harvard.edu/x/2022/psets/6/readability/


def main():
    # Getting the text to analyzing
    text = get_text()

    # Getting the list with letters [0], words [1], sentences [2]
    data = get_data(text)

    # Getting the grade number
    grade = get_grade(data)

    # Print the result accoring to the requirement
    print_result(grade)


def get_text():
    while True:
        input = get_string("Text: ")

        if (len(input) > 0):
            return input


def get_data(text):
    data = [0, 0, 0]

    for c in text:
        # Count letters
        if c.isalpha() == True:
            data[0] += 1
        # Count sentences
        elif c == '.' or c == '?' or c == '!':
            data[2] += 1

    # Count words
    data[1] = len(text.split(' '))

    return data


# Coleman-Liau index is computed using the formula:
# index = 0.0588 * L - 0.296 * S - 15.8, where
# L - the average number of letters per 100 words,
# S - the average number of sentences per 100 words.
def get_grade(data):
    # Get the average number of letters per 100 words: letters / words * 100
    L = data[0] / data[1] * 100

    # Get the average number of sentences per 100 words: sentences / words * 100
    S = data[2] / data[1] * 100

    # Return the result of the formula rounded to the nearest int
    return round(0.0588 * L - 0.296 * S - 15.8)


def print_result(grade):
    if grade < 1:
        print("Before Grade 1")
    elif grade >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {grade}")


main()
