import sys
import cs50

def main():
    if len(sys.argv) == 2: # argv[1:] is the argument array, len() method return the length of a string or array
        # User input and inital variables
        k = int(sys.argv[1]) # convert to int
        print("plaintext: ", end="")
        message = cs50.get_string() # get inputs

        if message != '':
            print("ciphertext: ", end="")
            # loop check string
            for i in message:
                if i.isalpha(): # check whether the character is alphabet, use .isaplha() method
                    if i.isupper(): # upper alphabet # starts from 65 in ASCII
                        print(chr((ord(i) - 65 + k) % 26 + 65), end="")
                    elif i.islower():
                        print(chr((ord(i) - 97 + k) % 26 + 97), end="") # lower alphabet # starts from 97 in ASCII
                else:
                    print(message) # other case
            print("")
            exit(0)
    else:
        print("Usage: ./caesar k\n")
        exit(1)

if __name__ == "__main__":
    main()