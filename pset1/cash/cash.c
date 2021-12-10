#include <cs50.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
    float changeindol;
    int coins = 0;
    int quarter = 25;
    int dime = 10;
    int nickel = 5;
    int pennie = 1;
    //ask a user for a number of changed owed
    do
    {
        changeindol = get_float("Change owed: \n");
    }
    while (changeindol < 0);
    //convert amount of dollars owed to amount of cents and round
    int cents = round(changeindol * 100);
    //check if you can give quarter
    while ((cents - quarter) >= 0)
    {
        coins += 1;
        cents = cents - quarter;
    }
    //if you can't give quarter start giving dimes
    while ((cents - dime) >= 0)
    {
        coins += 1;
        cents = cents - dime;
    }
    //if you can't give quarter start giving nickels
    while ((cents - nickel) >= 0)
    {
        coins += 1;
        cents = cents - nickel;
    }
    //if you can't give quarter start giving pennies
    while ((cents - pennie) >= 0)
    {
        coins += 1;
        cents = cents - pennie;
    }

    printf("%i\n", coins);
}