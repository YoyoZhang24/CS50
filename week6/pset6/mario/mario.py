from cs50 import get_int

height = 0

while height < 1 or height > 8:
    height = get_int("Height: ")
    
space = height - 1
hash_number = 1
width = 1

while hash_number <= height:
    
    # Spaces
    while space > 0:
        print(" ", end="")
        space -= 1
    
    # Left Hashes
    width = hash_number
    while hash_number > 0:
        print("#", end="")
        hash_number -= 1
    hash_number = width
    
    # Gaps
    print("  ", end="")
    
    # Right Hashes
    while hash_number > 0:
        print("#", end="")
        hash_number -= 1
        
    # New Line
    print("")
    hash_number = width + 1
    space = height - hash_number
    