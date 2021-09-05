// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 500;

// Hash table
node *table[N];

// Dictionary size;
int count_size = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO

    // 1 - hash word to obtain a hash value
    int index = hash(word);

    // 2 - access linked list
    node *cursor = table[index];

    // 3 - traverse linked list
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO

    // DJB2 hash function
    /*
    unsigned int hash = 5381;
    int c;
    while ((c = *word++))
        hash = ((hash << 5) + hash) + c; */ /* hash * 33 + c */

    unsigned int value = 0;
    unsigned int key_len = strlen(word);
    for (int i = 0; i < key_len; i++)
    {
        value = value + 37 * tolower(word[i]);
    }
    value = value % N;
    return value;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO

    // 1 - open dict
    FILE *open_dictionary = fopen(dictionary, "r");
    if (open_dictionary == NULL)
    {
        return false;
    }

    // 2 - read string from file one at a time
    char str[LENGTH + 1];

    while (fscanf(open_dictionary, "%s", str) != EOF)
    {
        // 3 - create a new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, str);
        n->next = NULL;

        // 4 - hash word to obtain a hash value
        int index = hash(str);

        // 5 - insert node into hash table
        if (table[index] == NULL)
        {
            table[index] = n;
        }
        else
        {
            n -> next = table[index];
            table[index] = n;
        }
        count_size++;
    }

    fclose(open_dictionary);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return count_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];

        while (cursor)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
        if (i == N - 1 && cursor == NULL)
        {
            return true;
        }
    }
    return false;
}
