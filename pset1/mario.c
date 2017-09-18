#include <stdio.h>
#include <cs50.h>

int main(void) {
	int height;
	do {
		printf("Height: ");
		height = get_int();
	} while (height > 23 || height < 0); // check input

	for (int row = 0; row < height; row++) { // row by row
		for (int i = 0; i < height - row; i++) { // print space
			printf(" ");
		}
		for (int i = 0; i < row + 1; i++) { // print #
			printf("#");
		}
		printf("\n");
	}

}
