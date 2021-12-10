// Implements a dictionary's functionality

#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in a hash table
const unsigned int N = 150000;

// Hash table
node *table[N];

//initialise the result of calling the hash function on a word as a positive num
unsigned int hash_value;

//initialise the counter for words in dictionary
unsigned int word_count = 0;

//make some pointers to use
node *n;
node *cursor;
node *tmp;

// Returns true if the word is in dictionary else false
bool check(const char *word)
{
    //hash a word to obtain a hash value (index)
    hash_value = hash(word);

    //access a linked list at that index in the hash table setting a cursor to the first item in that linked list
    cursor = table[hash_value];

    //move the cursor traversing the linked list until you reach the end of the list
    while (cursor != NULL)
    {
        //compare your word with the word to which cursor points case insesitively
        if (strcasecmp(word, cursor->word) == 0)
        {
            //return true if you find the word in the linked list
            return true;
        }
        //else move the cursor to the next node
        cursor = cursor->next;
    }
    //if the word hasn't been found return false
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //djb2
    //this algorithm (k=33) was first reported by dan bernstein many years ago in comp.lang.c.
    //another version of this algorithm (now favored by bernstein) uses xor: hash(i) = hash(i - 1) * 33 ^ str[i];
    //the magic of number 33 (why it works better than many other constants, prime or not) has never been adequately explained.
    //http://www.cse.yorku.ca/~oz/hash.html

    // unsigned long hash (unsigned char *str)
    // {
    //     unsigned long hash = 5381;
    //     int c;

    //     while (c = *str++)
    //         hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    //     return hash;
    // }

    unsigned long hash = 5381;
    int c;
    while ((c = tolower(*word++)))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //take as argument a char* (or a string) which is the dictionary to open up and read from
    FILE *file = fopen(dictionary, "r"); //
    if (file == NULL)
    {
        //if file points to NULL, close the file and return false (memory allocation error)
        fclose(file);
        return false;
    }

    //storage to read a word into (like buffer)
    char temp_word[LENGTH + 1];

    //Use fscanf to read the strings in the file untill it reaches the EOF (end of file)
    //file - result of calling fopen(file pointer); temp_word - some place to read a word into
    while (fscanf(file, "%s", temp_word) != EOF)
    {
        // Use malloc to create and allocate memory for each new node
        n = malloc(sizeof(node));

        //If malloc returns NULL, free n and return false
        if (n == NULL)
        {
            free(n);
            return false;
        }

        //copy word into the new node using strcpy (where n->word - is where to copy and temp_word is what to copy)
        strcpy(n->word, temp_word);

        //use hash function to take a word and return a hash value - index
        hash_value = hash(temp_word);

        //insert the new node into the hash table
        n->next = table[hash_value];
        table[hash_value] = n;

        //count the number of words in dictionary by incrementing the counter every time the word is inserted into the hash table
        word_count ++;
    }

    //close file and return true if executed successfully
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        //set the cursor to point at the current index of the hash table
        cursor = table[i];

        //while the current index is not empty or not reached the end of the individual linked list at that index
        while (cursor != NULL)
        {
            //set tmp to point at the same place as the cursor
            tmp = cursor;

            //move the cursor to the next node at the linked list
            cursor = cursor->next;

            //free the node tmp points at
            free(tmp);
        }

        //when the cursor points at NULL and reached the end of the array (hash table)
        if (cursor == NULL && i == N - 1)
        {
            return true;
        }
    }
    //if not successfull close file and return false
    return false;
}

