import csv
import sys


def main():

    # Check command-line argument
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # Open & Read CSV
    people = []
    with open(sys.argv[1]) as file:
        reader = csv.DictReader(file)
        fieldnames = reader.fieldnames

        for row in reader:
            person = row
            for i in fieldnames:
                if person[i] != person['name']:
                    person[i] = int(person[i])
            people.append(person)

    # Open & Read DNA
    with open(sys.argv[2]) as file:
        dna = file.readlines()
        dna = dna[0]

    # Read all the STRs
    repeats = {}
    for STR in fieldnames:
        if STR == 'name':
            continue
        repeats[STR] = str_repeats(dna, STR)

    # Compare STR counts against CSV
    for row in people:
        if repeats.items() <= row.items():
            sys.exit(f"{row['name']}")
    
    print("No match")


# Compute number of STRs

def str_repeats(dna, STR):
    dna_length = len(dna)
    str_length = len(STR)

    count = 0

    for i in range(dna_length):
        current = 0

        # If match found, count repeats
        if dna[i:(i + str_length)] == STR:
            k = 0
            while dna[(i + k):(i + k + str_length)] == STR:
                current += 1
                k += str_length

        if current > count:
            count = current

    return count


main()