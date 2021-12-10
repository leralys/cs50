from cs50 import SQL
import sys
import csv


# Your program should behave per the example below:
#  $ python import.py characters.csv


db = SQL("sqlite:///students.db")

if len(sys.argv) != 2:
    print("Error. Usage: python import.py characters.csv")
    sys.exit()

with open(sys.argv[1], "r") as characters:
    reader = csv.DictReader(characters)
    for row in reader:
        name_li = row["name"].split()
        l = len(name_li)
        first = name_li[0]
        last = name_li[l - 1]
        db.execute("INSERT INTO students (house, birth, first, last) VALUES(?, ?, ?, ?)", row["house"], row["birth"], first, last)
        if l == 3:
            middle = name_li[1]
            db.execute("UPDATE students SET middle=? WHERE first=?", middle, first)

# print(db.execute("SELECT first, middle, last FROM students"))
