#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>

int main(void) {
    string s = get_string();
    if (s != NULL)
    {
        printf("%c", toupper(s[0])); // first letter
        for (int i = 0, n = strlen(s); i < n; i++) // loop the whole argument string
        {
            if (s[i] == ' ') // check whether it's a space
                printf("%c", toupper(s[i + 1])); // if so, print the next letter and convert it to uppercase
        }
        printf("\n");
    }
}
