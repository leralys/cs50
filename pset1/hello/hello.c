#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //asking user for a name amd saving it to a variable
    string name = get_string("What is your name?\n");
    // print hello + name
    printf("Hello, %s\n", name);
}