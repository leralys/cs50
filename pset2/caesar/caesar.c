#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int main(int count, string array[])
{
    char cipher;

    //check if there are only 2 command line arguments (including the name of the programm)
    if (count == 2)
    {
        //iterate over the second argument to check if all the characters are digits
        for (int i = 0, n = strlen(array[1]); i < n; i++)
        {
            //if a char is not not a digit - error
            if (isdigit(array[1][i]) == 0)
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }
        //convert that command-line argument from a string to an int
        int key = atoi(array[1]);

        //promt a user for text to convert (without \n)
        string plain_t = get_string("plaintext: ");

        //start converting the text
        printf("ciphertext: ");
        //iterate over each character of the plaintext
        for (int j = 0, x = strlen(plain_t); j < x; j ++)
        {
            //check if it is an alphabtic character
            if (isalpha(plain_t[j]) > 0)
            {
                //if it is an uppercase letter, convert it, preserving case, then print out the resulted char
                //convert ASCII to alphabetical index
                //shift alphabetical index using formula
                //convert result back to ASCII
                //formula c[i] = (p[i] + k) % 26
                if (isupper(plain_t[j]) > 0)
                {
                    cipher = (((plain_t[j] - 65) + key) % 26) + 65;
                    printf("%c", cipher);
                }
                //if it is a lower case letter, do the same
                else
                {
                    cipher = (((plain_t[j] - 97) + key) % 26) + 97;
                    printf("%c", cipher);
                }
            }
            //if not alphabetic - keep unchanged
            else
            {
                cipher = plain_t[j];
                printf("%c", cipher);
            }
        }
        //after outputting ciphertext, print new line and return 0
        printf("\n");
        return 0;
    }
    //if provided no arguments, or more than 1, print error message, and return 1
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
}