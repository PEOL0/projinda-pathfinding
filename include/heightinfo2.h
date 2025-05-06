#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 725
#define HEIGHT 370

unsigned char image[HEIGHT][WIDTH];

void loadPGM(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("Failed to open file");
        exit(1);
    }

    char format[3];
    int width, height, maxval;

    // Read header
    fscanf(f, "%2s", format);
    if (strcmp(format, "P5") != 0) {
        fprintf(stderr, "Only binary PGM (P5) format supported.\n");
        exit(1);
    }

    // Skip comments
    int c;
    while ((c = fgetc(f)) == '#') {
        while (fgetc(f) != '\n');
    }
    ungetc(c, f);

    // Read size and max value
    fscanf(f, "%d %d %d", &width, &height, &maxval);
    fgetc(f); // Skip one byte (newline after header)

    if (width != WIDTH || height != HEIGHT) {
        fprintf(stderr, "Expected %dx%d image, got %dx%d.\n", WIDTH, HEIGHT, width, height);
        exit(1);
    }

    // Read pixel data
    fread(image, sizeof(unsigned char), WIDTH * HEIGHT, f);
    fclose(f);
}

void process() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            unsigned char value = image[y][x];
            printf("image[y][x]");
            // Do something with value
        }
    }
}