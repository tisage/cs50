#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[]) {
    if (argc == 2) {

        // key session
        string key = argv[1]; // read key

        int keyLength = strlen(key);
        // check key is alphabet
        for (int i = 0; i < keyLength; i++)
            if (!isalpha(key[i])) {
                printf("keywords argument should be alphabet char only.");
                return 1;
            }

        int keyCodes[keyLength]; // inital keycode array to hold 0-25 key codes
        int keyCounts = 0;

        // array convert to numbers with A = 0, Z =25
        for (int i = 0; i < keyLength; i++) {
            keyCodes[i] = toupper(key[i]) - 65;
        }

        // string session
        printf("plaintext: ");
        string s = get_string(); // read string
        if (s != NULL) {
            printf("ciphertext: ");

            // loop check string
            for (int i = 0, n = strlen(s); i < n; i++) {
                if (isalpha(s[i])) { // check whether is alphabet
                    if (isupper(s[i])) // upper alphabet # starts from 65 in ASCII
                        printf("%c", (s[i] - 65 + keyCodes[keyCounts]) % 26 + 65);
                    else if (islower(s[i]))
                        printf("%c", (s[i] - 97 + keyCodes[keyCounts]) % 26 + 97); // lower alphabet # starts from 97 in ASCII
                    if (keyCounts < keyLength - 1)
                        keyCounts++;
                    else
                        keyCounts = 0;
                } else
                    printf("%c", s[i]); // other case
            }
            printf("\n");
            return 0;
        }
    } else {
        printf("Usage: ./vigenere k\n");
        return 1;
    }
}