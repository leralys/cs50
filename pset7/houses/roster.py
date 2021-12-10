# python roster.py Gryffindor/Hufflepuff/Ravenclaw/Slytherin

from cs50 import SQL
import sys
import csv

db = SQL("sqlite:///students.db")

if len(sys.argv) != 2:
    print("Error. Usage: python roster.py House")
    sys.exit()

house = sys.argv[1]

students = db.execute("SELECT first, middle, last, birth FROM students WHERE house=? ORDER BY last", house)
for student in students:
    if student['middle']:
        print(f"{student['first']} {student['middle']} {student['last']}, born {student['birth']}")
    else:
        print(f"{student['first']} {student['last']}, born {student['birth']}")

sys.exit()