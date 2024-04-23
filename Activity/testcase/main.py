import random
import string

def random_string(length):
    characters = string.ascii_letters
    return ''.join(random.choice(characters) for i in range(length));

def random_code():
    with open('registration_codes.txt', 'r') as file:
        lines = file.readlines()  # Read all lines into a list
        random_line = random.choice(lines)  # Select a random line
        return random_line.strip()  # Remove the newline character

def main():
    for i in range(1000):
        print("register " + random_string(4) + " " + random_string(5) + " " + random_code()) 

if __name__ == '__main__':
    main()
    
