#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Prompts user for number
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // Prints pyramid
    int space = height - 1;
    int hash = 1;
    int width = 1;

    while (hash <= height)
    {
        // SPACES
        while (space > 0)
        {
            printf(" ");
            space--;
        }

        // LEFT HASHES
        width = hash;
        while (hash > 0)
        {
            printf("#");
            hash--;
        }
        hash = width;

        // GAP
        printf("  ");

        //RIGHT HASHES
        while (hash > 0)
        {
            printf("#");
            hash--;
        }

        // NEW LINE
        printf("\n");
        hash = width + 1;
        space = height - hash;
    }
}