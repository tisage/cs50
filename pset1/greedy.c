#include <stdio.h>
#include <cs50.h>
#include <math.h>


int main(void) {
	float change;
	printf("O hai! ");
	do {
		printf("How much change is owed?\n");
		change = get_float();
	} while (change < 0);

	int remain = round(change * 100);
	int coins = 0;

	while (remain > 0) {
		if (remain >= 25) {
			remain -= 25;
			coins++;
		} else if (remain >= 10) {
			remain -= 10;
			coins++;
		} else if (remain >= 5) {
			remain -= 5;
			coins++;
		} else if (remain >= 1) {
			remain -= 1;
			coins++;
		}
	}
	printf("%i\n", coins);
}