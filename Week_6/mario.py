from cs50 import get_int


def main():
    height = get_height()
    print_pyramid_by_height(height)


# Get the height with validation
def get_height():
    while True:
        n = get_int("Height: ")
        if n > 0 and n < 9:
            return n


# Print out lines of the pyramid one by one
def print_pyramid_by_height(height):
    for i in range(height):
        spaces = height - (i + 1)
        hashes = i + 1

        print(" " * spaces, end="")
        print("#" * hashes, end="")
        print("  ", end="")
        print("#" * hashes, end="")
        print()


main()
