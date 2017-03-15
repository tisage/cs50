/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 * 
 * Tianyu
 * Spring 2017
 */

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
               DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
    // TODO
    int numTiles = d * d; // total number of tiles
    // loop to assign values for board, left-upper corner is original point [0,0]
    for (int i = 0; i < d; i++) { // row 0, 1, 2, ... i
        for (int j = 0; j < d; j++) // col [i,0], [i,1], [i,2] ... [i,j]
            board[i][j] = --numTiles; // decrement starts from d^2 to 1
    }
    // Check whether swap
    /*
     If, however, and only if the board contains an odd number of tiles
     (i.e., the height and width of the board are even),
     the positions of tiles numbered 1 and 2 must be swapped.
    */
    if ((d * d) % 2 == 0) { // actually only swap when d^2 is an even number
        board[d - 1][d - 2] = 2; // the last second is [i][j-1] but i=[0, d-1], j = [0, d-1], so convert it to [d-1][d-2]
        board[d - 1][d - 3] = 1; // the last third
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // TODO
    // print out board values in consol
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            // replace the last tile with _
            if (board[i][j] == 0)
                printf("  _");
            else
                printf("%3i", board[i][j]); // format 3%i for right align
        }
        printf("\n\n\n"); // next three line for nice board interface.
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
bool move(int tile)
{
    // TODO
    // Check valid values of tile
    if (tile < 1 || tile > d * d - 1) // tile's value should be in the range [0, d^2 - 1]
        return false;

    // locate our tile position according to the value of tile, return row and col number
    int row = 0;
    int col = 0;
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if (board[i][j] == tile) {
                row = i;
                col = j;
            }
        }
    }
    // The most important part
    // Check empty space around this tile,
    // if it is empty one (value = 0), swap values.
    if (board[row][col + 1] == 0 && (col + 1) < d) { // right side, remember to check boarder boundary issue, 
                                                    // the new moved tile should not exceed the boundary d, 
                                                    // so we check col + 1 < d
        board[row][col + 1] = board[row][col]; // swap these two tile's value
        board[row][col] = 0;
        return true;
    }
    if (board[row][col - 1] == 0 && (col - 1) >= 0) { // left side, remember to check boarder issue col - 1 > 0
        board[row][col - 1] = board[row][col];
        board[row][col] = 0;
        return true;
    }
    if (board[row - 1][col] == 0 && (row - 1) >= 0) { // up side, remember to check boarder issue row - 1 > 0
        board[row - 1][col] = board[row][col];
        board[row][col] = 0;
        return true;
    }
    if (board[row + 1][col] == 0 && (row + 1) < d) { // bottom side, remember to check boarder issue row + 1 < d
        board[row + 1][col] = board[row][col];
        board[row][col] = 0;
        return true;
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
bool won(void)
{
    // TODO
    // Use a counter value and compare it with tile's value which is board[i][j]
    // pay attention on the last tile, which has value = 0
    int counter = 1; // counter starts from 1 and compare it with board[i][j]
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if (board[i][j] == 0)
                counter = 0; // the last tile situation, compare board[i][j] with 0
            if (board[i][j] != counter) // check board[i][j] is in a sorted order
                return false;
            counter++; // attention on the counter place
                       // after we check the tile's value with counter, 
                       // this counter should increase 1
        }
    }
    return true;
}
