
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    // remember to modify return # to 1
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    int n = atoi(argv[1]); // check n
    if (n < 0 || n > 100) {
        fprintf(stderr, "n should be postive and less or equal to 100.\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 1;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
            bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 1;
    }

    // resize work after check 24-bit and before write outfile's bitmap header

    // record old paramters' value
    int old_biWidth = bi.biWidth;
    int old_biHeight = bi.biHeight;
    int old_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // new parameters
    bi.biWidth *= n;
    bi.biHeight *= n;
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;     // determine padding for scanlines

    // recalculate values
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    // iterate over infile's scanlines
    // also needs to modify iteration block

    /* Choose "Recopy" method

    for each row (vertical)
        for n times (horizontal)
            read pix
            write pix
        skip over infile padding
        write padding to outfile
        set infile cursor the begining of row

    */

    // Attention on the nested loop
    for (int i = 0, biHeight = abs(old_biHeight); i < biHeight; i++) { // for each row of infile

        for (int row = 0; row < n; row++) { // repeat n times vertically

            // iterate over pixels in scanline
            for (int j = 0; j < old_biWidth; j++) { // read infile pix by pix
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                for (int col = 0; col < n; col++) // repeat n times horizontally
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

            }
            // skip infile cursor over padding
            // old_padding is step lenth (how many paddle to skip), SEEK_CUR means starts from the current cursor
            fseek(inptr, old_padding, SEEK_CUR);

            // print outfile's padding
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }


            // set the length of offset step
            long offset = sizeof(RGBTRIPLE) * old_biWidth + old_padding;

            if (row != (n - 1)) // Check if it is the last row to repeat, if so, set cursor back to the begining of row
                fseek(inptr, -offset, SEEK_CUR); // backward, so offset value should be negative
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}