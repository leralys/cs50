#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//define 512 bytes chunk of memory
#define BLOCK_SIZE 512
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
// Check for invalid input
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    //string input is a file name provided with the command line
    char *input = argv[1];

    //open the input file (and create a pointer to it called rawFile)
    FILE *rawFile = fopen(input, "r");
    //pointer points to nothing
    if (rawFile == NULL)
    {
        printf("Unable to open: %s\n", input);
        return 1;
    }

    //create a buffer of BLOCK_SIZE (512)
    BYTE buffer[BLOCK_SIZE];

    //initialize a jpeg counter
    int jpgCounter = 0;

    //initialize a name of a recovered jpeg ( xxx.jpg'\0' )
    char outputName[8];

    //create a pointer to the recovered jpeg
    FILE *output = NULL;

    //read into the buffer, by 512 bytes blocks, one at a time, from the input file (rawFile)
    //until the end of memory card (when the size of data left is smaller then 512 bytes block (false))
    while (fread(buffer, BLOCK_SIZE, 1, rawFile) == 1)
    {
        //if START of a new jpeg was found
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //if already have a jpeg
            if (output != NULL)
            {
                //close the current file
                fclose(output);
            }

            //if no jpegs were found until then or the current jpeg has been already closed
            //we can open a new file for writing

            sprintf(outputName, "%03i.jpg", jpgCounter);
            output = fopen(outputName, "w"); // !!!HERE IS WHERE WE CHANGE OUTPUT POINTER!!!
            jpgCounter++;
        }
        //condition where we already have a jpeg but have not found another one - we continue to write 512 bytes blocks
        if (output != NULL)
        {
            fwrite(buffer, BLOCK_SIZE, 1, output);
        }
    }
    //close all open files
    //could not find any jpeg - close the output file
    if (output == NULL)
    {
        fclose(output);
    }
    //close the input file if its pointer points to nothing
    if (rawFile == NULL)
    {
        fclose(rawFile);
    }
    return 0;
}