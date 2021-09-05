#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

// Delete all the jpgs: $ rm *.jpg

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{

    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover file\n");
        return 1;
    }

    // Check validity
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("File cannot be opened.\n");
        return 1;
    }

    BYTE buffer[512];
    char name[8];
    void *pointer;
    FILE *out = NULL;
    int counter = 0;

    while (fread(&buffer, 512, 1, file))
    {
        // Check header
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Close previous
            if (counter != 0)
            {
                fclose(out);
            }

            // Initialize file
            sprintf(name, "%03i.jpg", counter);
            out = fopen(name, "w");
            counter++;
        }

        if (counter != 0)
        {
            fwrite(&buffer, 512, 1, out);
        }
    }

    fclose(file);
    fclose(out);
}