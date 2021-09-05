from cs50 import get_int

# Prompt for number
number = get_int("Number: ")

# Checksum
copy = number
digit_sum = 0
x = 0
remain = copy % 10

# 1 - Sum of the digits of (every other digit of card * 2)
while copy > 1:
    copy = copy - remain
    copy = copy / 10
    remain = copy % 10
    remain = remain * 2

    if remain >= 10:
        digit_sum = digit_sum + 1 + remain % 10
    else:
        digit_sum = digit_sum + remain

    remain = remain / 2
    copy = copy - remain
    copy = copy / 10
    remain = copy % 10

# 2 - Add the rest of the digits
copy = number
while copy > 1:
    remain = copy % 10
    digit_sum = digit_sum + remain
    copy = copy - remain
    copy = copy / 10
    remain = copy % 10
    copy = copy - remain
    copy = copy / 10

# Check whether it passes Checksum
count = 0

# CASE I - Pass
if digit_sum % 10 == 0:
    while number > 100:
        number = number // 10
        count += 1

    # Check type of card
    if number == 34 or number == 37:
        if count == 13:
            print("AMEX")

    elif number == 51 or number == 52 or number == 53 or number == 54 or number == 55:
        if count == 14:
            print("MASTERCARD")

    elif number // 10 == 4:
        if count == 11 or count == 14:
            print("VISA")

    else:
        print("INVALID")

# CASE II - Not pass
else:
    print("INVALID")