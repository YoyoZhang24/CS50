#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Prompt for card number
    long number = get_long("Number: ");

    // Calculate Checksum
    long copy = number;
    long sum = 0;
    long x = 0;
    long remain = copy % 10;

    // 1st step - sum of the digits of (every other digit of card * 2)
    while (copy > 1)
    {
        copy = copy - remain;
        copy = copy / 10;
        remain = copy % 10;
        remain = remain * 2;

        if (remain >= 10)
        {
            sum = sum + 1 + remain % 10;
        }
        else
        {
            sum = sum + remain;
        }
        remain = remain / 2;
        copy = copy - remain;
        copy = copy / 10;
        remain = copy % 10;
    }

    // 2nd step - add the rest of the digits
    copy = number;
    while (copy > 1)
    {
        remain = copy % 10;
        sum = sum + remain;
        copy = copy - remain;
        copy = copy / 10;
        remain = copy % 10;
        copy = copy - remain;
        copy = copy / 10;
    }

    // Check whether it passes Checksum
    int count = 0;
    // Output - if it passes Checksum
    if (sum % 10 == 0)
    {
        while (number > 100)
        {
            number = number / 10;
            count = count + 1;
        }

        //Checks type of card
        if ((number == 34 || number == 37) && count == 13)
        {
            printf("AMEX\n");
        }
        else if ((number == 51 || number == 52 || number == 53 || number == 54 || number == 55) && count == 14)
        {
            printf("MASTERCARD\n");
        }
        else if (number / 10 == 4 && (count == 11 || count == 14))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }

    // Output - if it doesn't pass Checksum
    else
    {
        printf("INVALID\n");
    }
    
}