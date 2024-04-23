import random
import string

def random_string(length):
    characters = string.ascii_letters + string.digits
    return ''.join(random.choice(characters) for i in range(length));

def random_num(length):
    with open('filename.txt', 'r') as file:
        lines = file.readlines()  # Read all lines into a list
        random_line = random.choice(lines)  # Select a random line
        return random_line.strip()  # Remove the newline character

def main():
    for i in range(10):
        print("register " + random_string(2) + " " + random_string(2) + " " + random_num(10)) 

if __name__ == '__main__':
    main()
    
