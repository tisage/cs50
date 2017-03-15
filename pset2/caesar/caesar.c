#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[]) {
    if (argc == 2) {
        // User input and inital variables
        int k = atoi(argv[1]); // convert to int
        printf("plaintext: ");
        string s = get_string();

        if (s != NULL) {
            printf("ciphertext: ");
            // loop check string
            for (int i = 0, n = strlen(s); i < n; i++) {
                if (isalpha(s[i])) { // check whether is alphabet
                    if (isupper(s[i])) // upper alphabet # starts from 65 in ASCII
                        printf("%c", (s[i] - 65 + k) % 26 + 65);
                    else if (islower(s[i]))
                        printf("%c", (s[i] - 97 + k) % 26 + 97); // lower alphabet # starts from 97 in ASCII
                } else
                    printf("%c", s[i]); // other case
            }
            printf("\n");
            return 0;
        }
    } else {
        printf("Usage: ./caesar k\n");
        return 1;
    }
}