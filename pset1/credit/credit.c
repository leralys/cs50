#include <stdio.h>
#include <cs50.h>

int sum2(long card), sum1(long card), length(long card);
int first(long card), second(long card);
void ifvalid(long card);

int main(void)
{
    //prompt user for a card number
    long cc = get_long("Number: ");
    // validity test
    ifvalid(cc);
}

//function that calculates the sum of digits starting with the 2nd-to-last digit
int sum2(long card)
{
    int sum = 0;
    do
    {
        //go to second digit from the end
        card /= 10;

        //take the last number and multiply it by 2
        int n = 2 * (card % 10);
        // printf("N is %i\n", n);

        // if after multiplying the number consists of more then 2 digits add one to another
        if (n > 9)
        {
            int lastdigit = n % 10;
            n = (n / 10) + lastdigit;
            // printf("Sum of N digits is: %i\n", n);
        }
        //add n to the sum
        sum += n;
        //"dispose of" the used n
        card /= 10;
        // printf("sum is %i\n", sum1);
    }
    while (card > 0);
    return sum;
}

//function that calculates the sum of digits starting with the last digit
int sum1(long card)
{
    int sum = 0;
    do
    {
        //take the last digit
        int n = card % 10;
        //add n to the sum
        sum += n;
        //"dispose of" the used n and the digit that goes after
        card /= 100;
    }
    while (card > 0);
    return sum;
}

// function that calculates cc length
int length(long card)
{
    //initialize the counter
    int count = 0;
    // Run loop till num is greater than 0
    do
    {
        //Increment digits counter
        count ++;
        //Remove last digit of cc number
        card /= 10;
    }
    while (card > 0);
    return count;
}

//function to get the 1st digit
int first(long card)
{
    while (card > 10)
    {
        card /= 10;
    }
    return card % 10;
}

//function to get the 2nd digit
int second(long card)
{
    while (card > 100)
    {
        card /= 10;
    }
    return card % 10;
}

//function that test if card is valid and the type if yes
void ifvalid(long card)
{
    if ((sum2(card) + sum1(card)) % 10 == 0)
    {
        //VISA check
        if (first(card) == 4 && (length(card) == 13 || length(card) == 16))
        {
            printf("VISA\n");
        }
        //AMEX check
        else if (first(card) == 3 && (second(card) == 4 || second(card) == 7) && length(card) == 15)
        {
            printf("AMEX\n");
        }
        //MASTERCARD check
        else if (first(card) == 5 && (second(card) > 0 && second(card) < 6) && length(card) == 16)
        {
            printf("MASTERCARD\n");
        }
        //passed the test of Luhn’s algorithm, but not any type from above
        else
        {
            printf("INVALID\n");
        }
    }
    //invalid cc number - didn't pass the Luhn’s algorithm
    else
    {
        printf("INVALID\n");
    }
}

