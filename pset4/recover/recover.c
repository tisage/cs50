
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./copy infile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[1];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // Read Files

    // buffer
    int BLOCK_SIZE = 512;
    uint8_t buffer[BLOCK_SIZE]; // need to include libary <stdint.h>

    int counter = 0; // counter for the names of image file
    FILE *outptr = NULL;

    // Loop keeps reading input file until the end of file
    while (fread(buffer, BLOCK_SIZE, 1, inptr)) {

        // Check Header's 4 bytes of each block
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) { // if so, create a jpg and start to write
            // check file is open
            if (outptr != NULL) {
                fclose(outptr);
            }
            char filename[5]; // char array to store the filename of recovered image's
            sprintf(filename, "%03i.jpg", counter);

            // open the jpg file
            outptr = fopen(filename, "w");
            counter++;
        }

        // Check whether the file is open
        if (outptr != NULL)
            // Write Files
            fwrite(buffer, BLOCK_SIZE, 1, outptr);
    }

    // Close Remaining Files
    fclose(inptr);
    fclose(outptr);
    return 0;

}