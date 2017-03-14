#include <stdio.h>
#include <cs50.h>

int main(void){
    printf("Minutes: ");
    int minute = get_int();
    printf("Bottles: %.f\n", minute*1.5*128/16);
}