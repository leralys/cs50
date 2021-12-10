import sys
import csv


# function that counts STRs' repeats in a dna sequence
def compute_repeats(string, _dict):
    # loop over the keys in _dict
    for key in _dict:
        l = len(key)
        # initialize the counter
        # initialize the max value of repeats
        max_repeats = 0
        count = 0
        # loop over the string
        for i in range(len(string)):
            # if substring from the current index equals to the key, loop
            while string[i:i + l] == key:
                # increase the counter, and skip to the end of the the substring
                count += 1
                i = i + l
            # if substring from the current index NOT equals to the key
            if not string[i:i + l] == key:
                # check if counter is larger then max_repeats
                if count > max_repeats:
                    # make the max_repeats be equal to the counter
                    max_repeats = count
                # reset counter
                count = 0
        # append the value of the max_repeats to the current key in _dic
        _dict[key] += max_repeats
    return _dict


# function that finds if there is a match of computed STRs with the database
def find_match(reader, _dict):
    # initialize the counter
    match = 0
    # iterate over the database and the dictionary where the STR counts are stored
    for row in reader:
        for key in _dict:
            # if numbers are equal increase the counter
            if int(row[key]) == int(_dict[key]):
                match += 1
                # when the counter is equal to the number of STRs in dictionary it means that we have found the match
                if match == len(_dict):
                    # print the name and exit the program
                    print(row["name"])
                    sys.exit()
        # if we are still running - it means that no match have been founs, so reset the counter
        match = 0
    # if we finished the loops and still running - it means that there is no match, so print "No match" and exit the program
    print("No match")
    sys.exit()


# --  MAIN  --

# check that the argv array contains three arguments
# if an incorrect number of command-line arguments provided, print an error message
# exit the program

if len(sys.argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    sys.exit()


# open txt file containing dna sequence needed to analyze and read

with open(sys.argv[2], "r") as dna_file:
    dna_seq = dna_file.read()


# open csv file and read
# extract the first line which contains the names of STRs
# remove the first item from the list of STRs because it is the name of the first column - which is the word "name"

with open(sys.argv[1], "r") as csv_file:
    csv_reader = csv.reader(csv_file)
    str_list = next(csv_reader)
    str_list.pop(0)


# save the STR_list to a dictionary where each STR is a key
# in order to use it after computing STRs from the dna

str_dict = dict.fromkeys(str_list, 0)


# counts the repeats of given STRs in the dna sequence and save them to the str_dict

compute_repeats(dna_seq, str_dict)


# check if there is a match

with open(sys.argv[1], "r") as csv_file:
    csv_reader = csv.DictReader(csv_file)
    find_match(csv_reader, str_dict)