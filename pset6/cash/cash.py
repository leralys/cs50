from cs50 import get_float
# function that repeats asking for a POSITIVE float if it has not yet been provided


def get_positive_float():
    while True:
        f = get_float("Change owed: ")
        if f >= 0:
            break
    return f


# ask a user for a positive amount of change to be given in dollars
change = get_positive_float()
# convert the dollars to cents, and round to the nearest integer
cents = round(change * 100)
# initialize a coin counter
coins = 0
# start giving change with quartes, then proceed to dimes, nickels and pennies
while (cents - 25) >= 0:
    coins += 1
    cents -= 25
while (cents - 10) >= 0:
    coins += 1
    cents -= 10
while (cents - 5) >= 0:
    coins += 1
    cents -= 5
while (cents - 1) >= 0:
    coins += 1
    cents -= 1
print(coins)