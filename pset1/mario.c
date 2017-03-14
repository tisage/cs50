#include <stdio.h>
#include <cs50.h>

int main(void){
    int height;
    do{
        printf("Height: ");
        height = get_int();
    }while(height>23||height<0);

    int row = 1;
    while(row<=height){
        for(int i=1;i<=height-row;i++){
            printf(" ");
        }
        for(int i=1;i<=row+1;i++){
            printf("#");
        }
        printf("\n");
        row++;
    }
}