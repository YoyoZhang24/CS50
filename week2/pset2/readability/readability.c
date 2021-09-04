#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Forward declaration
long count_letters(string txt);
long count_words(string txt);
long count_sentences(string txt);
double coleman_liau(long l, long w, long s);

int main(void)
{
    // Prompt for text
    string text = get_string("Text: ");

    // Run the tests
    long letters = count_letters(text);
    long words = count_words(text);
    long sentences = count_sentences(text);
    double level = coleman_liau(letters, words, sentences);
    printf("%li, %li, %li\n", letters, words, sentences);

    // Print the level
    long answer = round(level);

    if (answer < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (answer > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %ld\n", answer);
    }
}

// Count letters
long count_letters(string txt)
{
    long letters = 0;
    for (long x = 0,  n = strlen(txt); x < n; x++)
    {
        if (isalpha(txt[x]))
        {
            letters = letters + 1;
        }
    }
    return letters;
}

// Count words
long count_words(string txt)
{
    long words = 0;
    for (long x = 0, n = strlen(txt); x < n; x++)
    {
        if (isspace(txt[x]))
        {
            words = words + 1;
        }
    }
    words = words + 1;
    return words;
}

// Count sentences
long count_sentences(string txt)
{
    long sentences = 0;
    for (long x = 0, n = strlen(txt); x < n; x++)
    {
        if (txt[x] == 46 || txt[x] == 33 || txt[x] == 63)
        {
            sentences = sentences + 1;
        }
    }
    return sentences;
}

// Run Coleman-Liau index
double coleman_liau(long l, long w, long s)
{
    double level = (0.0588 * 100 * l / w) - (0.296 * 100 * s / w) - 15.8;
    printf("%f \n", level);
    return level;
}