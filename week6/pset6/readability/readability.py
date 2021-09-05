from cs50 import get_string


def main():

    # Prompt for text
    text = get_string("Text: ")

    # Run the tests
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)
    level = coleman_liau(letters, words, sentences)
    print(f"{letters}, {words}, {sentences}")

    # Print level
    answer = round(level)
    if answer < 1:
        print("Before Grade 1")
    elif answer > 16:
        print("Grade 16+")
    else:
        print(f"Grade {answer}")


# Count letters

def count_letters(text):
    letters = 0
    for x in range(len(text)):
        if(text[x].isalpha()):
            letters += 1
    return letters


# Count words

def count_words(text):
    words = 1
    for x in range(len(text)):
        if(text[x].isspace()):
            words += 1
    return words


# Count sentences

def count_sentences(text):
    sentences = 0
    for x in range(len(text)):
        if text[x] == "!" or text[x] == "." or text[x] == "?":
            sentences += 1
    return sentences


# Run Coleman-Liau index

def coleman_liau(l, w, s):
    level = (0.0588 * 100 * l / w) - (0.296 * 100 * s / w) - 15.8
    print(level)
    return level


main()