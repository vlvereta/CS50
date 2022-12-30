#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// Easy to understand BYTE alias for 8 BITS
typedef uint8_t BYTE;

// FAT input_file system whose “block size” is 512 bytes
const int BLOCK_SIZE = 512;

// Filename should look like ###.jpg where ### is three-digit decimal number from 000 on up + "/0"
const int FILENAME_SIZE = 8;

/**
 * The fourth byte, meanwhile, is either
 * 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, or 0xef.
 * Put another way, the fourth byte’s first four bits are 1110. Make it 4 bits right (by >>) and get integer 14.
*/
const BYTE FORTH_JPEG_BYTE = 14; // aka 0000 1110

int write_file(BYTE *buffer, char *filename, FILE *image_file);
void free_buffers(BYTE *buffer, char *filename);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open input file
    FILE *input_file = fopen(argv[1], "r");
    if (!input_file)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    BYTE *buffer; // get buffer for reading a memory block
    if (!(buffer = malloc(sizeof(BYTE) * BLOCK_SIZE)))
    {
        printf("Could not allocate enough memory for buffer.\n");
        fclose(input_file);
        return 1;
    }

    char *filename; // get filename memory allocated to easily get the proper format
    if (!(filename = malloc(sizeof(char) * FILENAME_SIZE)))
    {
        printf("Could not allocate enough memory for filename.\n");
        free(buffer);
        fclose(input_file);
        return 1;
    }
    else
    {
        for (int i = 0; i < FILENAME_SIZE; i++)
        {
            filename[i] = 0; // nullish the filename
        }
    }

    // Reading the memory input_file and writing new JPEGs files
    int counter = 0;
    FILE *image_file;
    bool is_writing = false;
    while (fread(buffer, 1, BLOCK_SIZE, input_file) != 0)
    {
        // Check whether the jpg file started
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && buffer[3] >> 4 == FORTH_JPEG_BYTE)
        {
            if (is_writing)
            {
                fclose(image_file);
                is_writing = false;
            }

            sprintf(filename, "%03d.jpg", counter++); // prepare the filename

            // Open a new file to write a jpg
            image_file = fopen(filename, "w");
            if (!image_file)
            {
                printf("Could not open %s.\n", filename);
                free_buffers(buffer, filename);
                return 1;
            }

            // Start writing opened file
            if (write_file(buffer, filename, image_file) != 0)
            {
                return 1;
            }

            is_writing = true;
        }
        else if (is_writing)
        {
            // Continue writing previously opened file
            if (write_file(buffer, filename, image_file) != 0)
            {
                return 1;
            }
        }
    }

    free_buffers(buffer, filename);
    fclose(input_file);
    fclose(image_file);
}

int write_file(BYTE *buffer, char *filename, FILE *image_file)
{
    int written = fwrite(buffer, 1, BLOCK_SIZE, image_file);
    if (written != BLOCK_SIZE)
    {
        printf("Something went wrong on writing into %s.\n", filename);
        free_buffers(buffer, filename);
        fclose(image_file);
        return 1;
    }
    return 0;
}

void free_buffers(BYTE *buffer, char *filename)
{
    free(buffer);
    free(filename);
}
