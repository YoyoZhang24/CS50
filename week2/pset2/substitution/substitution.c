#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Forward declaration
int cipher(string text, string key);
int check(string key);

// Wait for the key at command line
int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        exit(1);
    }
    string key = argv[1];
    check(key);
}

// Check key
int check(string key)
{

    // Check letters
    int i = 0;
    int n = strlen(key);
    while (i < n)
    {
        if (!isalpha(key[i]))
        {
            printf("Key must only contain letters.\n");
            exit(1);
        }
        i++;
    }

    // Check length
    if (n != 26)
    {
        printf("Key must contain 26 characters.\n");
        exit(1);
    }

    // Check duplicate characters
    for (int x = 0, y = 1, m = strlen(key); x < m; x++)
    {
        while (y < m)
        {
            if ((key[x] == key[y]) && (x != y))
            {
                printf("Key should not contain repeated characters. \n");
                exit(1);
            }
            y++;
        }
        y = 0;
    }

    // Prompt for plaintext
    string text = get_string("plaintext: ");
    cipher(text, key);
    return 0;
}

// Generate ciphertext
int cipher(string text, string key)
{
    printf("ciphertext: ");
    for (int j = 0, m = strlen(text); j < m; j++)
    {
        if (isalpha(text[j]))
        {
            //maintains capitalization
            if (isupper(text[j]))
            {
                int x = text[j] - 65;
                text[j] = toupper(key[x]);
            }
            else
            {
                int x = text[j] - 97;
                text[j] = tolower(key[x]);
            }
        }
        printf("%c", text[j]);
    }
    printf("\n");
    return 0;
}
