import random

import string
def gename():
    lowercase_letters = string.ascii_lowercase
    random_letter = random.choice(lowercase_letters)
    s=""
    for i in range(4):
        s += random.choice(lowercase_letters)
    s += " "
    for i in range(4):
        s += random.choice(lowercase_letters)
    return s

with open('code', 'r') as f:
    i = 10000
    for line in f:
        if(i > 0):
            i -= 1
        else:
            break
        code = line.strip()
        name = gename()

        print(f"register {name} {code}")
        print(f"withdraw {name}")
        print(f"check {name}")
