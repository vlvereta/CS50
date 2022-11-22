import csv
import sys

# Implement a program that identifies a person based on their DNA.
# Problem homepage: https://cs50.harvard.edu/x/2022/psets/6/dna/

def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    strs = {}
    individuals = []

    individualsFile = open(sys.argv[1])
    for individual in csv.DictReader(individualsFile):
        individuals.append(individual)

    for key in dict.keys(individuals[0]):
        if (key != "name"):
            strs[key] = 0

    # Read DNA sequence file into a variable
    sequenceFile = open(sys.argv[2])
    reader = csv.reader(sequenceFile)
    sequence = next(reader)[0]

    # Find longest match of each STR in DNA sequence
    for str in strs:
        strs[str] = longest_match(sequence, str)

    # Check database for matching profiles
    check_for_match(individuals, strs)

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


def check_for_match(individuals, strs):
    for individual in individuals:
        match = True
        for str in strs:
            if int(individual[str]) != strs[str]:
                match = False
                break
        if match == True:
            print(individual["name"])
            return

    print("No match")


main()
