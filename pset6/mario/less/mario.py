#!/usr/bin/env python3

from cs50 import get_int

num = get_int("Height: ")

# keep asking for a number if it doen't fit the condition
while num < 1 or num > 8:
    num = get_int("Height: ")

# print " " num-1 times, each time deacreasing it by 1
# on the same line print "#" starting from 1 and increasing it on each iteration by 1
for n in range(num):
    print(" " * (num - 1 - n), end="")
    print("#" * (n + 1))
