from cs50 import get_string

# 1. prompt a user for a text
# 2. calculate the num of letters, words and sentences in the text
# (for words each space indicates a new word + 1 for the very first word)
# 3. calculate the average num of letters and sentences per 100 words (L and S)
# 4. calculate the Coleman-Liau index by formula: index = 0.0588 * L - 0.296 * S - 15.8
# 5. print the result

text = get_string("Text: ")
letters = 0
words = 1
sentences = 0


def average(x, y):
    return x * 100 / y


for char in text:
    if char.isalpha():
        letters += 1
    if char == " ":
        words += 1
    if char == "!" or char == "?" or char == ".":
        sentences += 1

l = average(letters, words)
s = average(sentences, words)

index = round((0.0588 * l) - (0.296 * s) - 15.8)

if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {index}")