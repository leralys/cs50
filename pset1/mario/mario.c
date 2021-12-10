#include <cs50.h>
#include <stdio.h>

void hash(void);
void space(void);

int main(void)
{
    int height, row, col, sp, k, l;
    
    //ask a user for a number from 1 to 8
    do
    {
        height = get_int("Height: \n");
    }
    while (height <= 0 || height > 8);
    
    //loop for the rows
    for (row = 0; row < height; row++)
    {   
        //create a loop that prints spaces, print height-row-1 num of spaces on each row to move to the next loop
        for (sp = 0; sp < (height - row) - 1; sp ++)
        {
            space();
        }
        //loop to print necessary num of #
        for (col = 0; col <= row; col++)
        {
            hash();
        }
        for (k = 0; k < 2; k++)
        {
            space();
        }
        for (l = 0; l <= row; l++)
        {
            hash();
        }
        //add a space after each row
        printf("\n");
    }
}

void hash(void)
{
    printf("#");
}

void space(void)
{
    printf(" ");
}

// Try to establish a relationship between
//(a) the height the user would like the pyramid to be,
//(b) what row is currently being printed, and
//(c) how many spaces and how many hashes are in that row.
//Once you establish the formula, you can translate that to C!

// #include <cs50.h>
// #include <stdio.h>

// int main(void)
// {
//     int n;
//     do
//     {
//         n = get_int("Size: ");
//     }
//     while (n < 1);
//     for (int i = 0; i < n; i++)
//     {
//         for (int j = 0; j < n; j++)
//         {
//             printf("#");
//         }
//         printf("\n");
//     }
// }
