#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float average;
    //itirate over a row
    for (int r = 0; r < height; r++)
    {
        //inside each row itirate over each column (pixel)
        for (int p = 0; p < width; p++)
        {
            //take the current pixel's rgb colors, sum them up, and calculate the average
            average = (image[r][p].rgbtBlue + image[r][p].rgbtGreen + image[r][p].rgbtRed) / 3.0;
            //make pixel's colors equal average ==> it makes the pixel's color to be gray
            image[r][p].rgbtBlue  = image[r][p].rgbtGreen = image[r][p].rgbtRed = round(average);
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    float sepBlue;
    float sepGreen;
    float sepRed;
    //itirate over a row
    for (int i = 0; i < height; i++)
    {
        //inside each row itirate over each column(pixel)
        for (int j = 0; j < width; j++)
        {
            //get sepia versions of RGB colors and round them
            //round the sepia result and change RGB to its sepia version
            sepRed = round((.393 * image[i][j].rgbtRed) + (.769 * image[i][j].rgbtGreen) + (.189 * image[i][j].rgbtBlue));
            sepGreen = round((.349 * image[i][j].rgbtRed) + (.686 * image[i][j].rgbtGreen) + (.168 * image[i][j].rgbtBlue));
            sepBlue = round((.272 * image[i][j].rgbtRed) + (.534 * image[i][j].rgbtGreen) + (.131 * image[i][j].rgbtBlue));
            //check that sepia colors are not larger then 255
            //checnge RGB colors to their sepia versions
            if (sepRed > 255)
            {
                sepRed = 255;
            }
            image[i][j].rgbtRed = sepRed;
            if (sepGreen > 255)
            {
                sepGreen = 255;
            }
            image[i][j].rgbtGreen = sepGreen;
            if (sepBlue > 255)
            {
                sepBlue = 255;
            }
            image[i][j].rgbtBlue = sepBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    //itirate over a row
    for (int i = 0; i < height; i++)
    {
        //inside each row itirate over each column (pixel)
        for (int j = 0; j < width / 2; j++)
        {
            // int temp = arr[i];
            // arr[i] = arr[size - 1 - i];
            // arr[size - 1 - i] = temp;
            temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}

void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int sumBlue, sumGreen, sumRed;
    float count;
    //create a new image
    RGBTRIPLE original[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            original[i][j] = image[i][j];
        }
    }

    // for each row
    for (int i = 0; i < height; i++)
    {
        //for each col
        for (int j = 0; j < width; j++)
        {
            //set count to 0
            sumBlue = sumGreen = sumRed = 0;
            count = 0.0;

            //itirate through i from -1 to +1 to check if surrounding pixels exist
            for (int h = -1; h < 2; h ++)
            {
                //same for j
                for (int w = -1; w < 2; w++)
                {
                    if (i + h < 0 || i + h >= height)
                    {
                        continue;
                    }
                    if (j + w < 0 || j + w >= width)
                    {
                        continue;
                    }
                    sumBlue += original[i + h][j + w].rgbtBlue;
                    sumGreen += original[i + h][j + w].rgbtGreen;
                    sumRed += original[i + h][j + w].rgbtRed;
                    count++;
                }
            }
            image[i][j].rgbtBlue = round(sumBlue / count);
            image[i][j].rgbtGreen = round(sumGreen / count);
            image[i][j].rgbtRed = round(sumRed / count);
        }
    }
}