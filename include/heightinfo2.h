#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int WIDTH = 1000;
const int HEIGHT = 1000;

void loadPGM(const char *filename, float **image) {

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    }

    // Check if the specified file exists, not just "test.pgm"
    if (access(filename, F_OK) != 0) {
        perror("File does not exist at expected location \n");
        exit(1);
    }

    FILE *f = fopen(filename, "rb");

    printf("File exists \n");

    if (!f) {
        fprintf(stderr, "Failed to open file '%s'\n", filename);
        perror("Reason");
        exit(1);
    }

    char format[3];
    int width, height, maxval;

    fscanf(f, "%2s", format);
    if (strcmp(format, "P5") != 0) {
        fprintf(stderr, "Only binary PGM (P5) format supported.\n");
        exit(1);
    }

    int c;
    while ((c = fgetc(f)) == '#') {
        while (fgetc(f) != '\n');
    }
    ungetc(c, f);

    fscanf(f, "%d %d %d", &width, &height, &maxval);
    fgetc(f);

    if (width != WIDTH || height != HEIGHT) {
        fprintf(stderr, "Expected %dx%d image, got %dx%d.\n", WIDTH, HEIGHT, width, height);
        exit(1);
    }

    // Correct allocation for a buffer of unsigned char values
    unsigned char *buffer = (unsigned char *)malloc(WIDTH * HEIGHT * sizeof(unsigned char));
    if (!buffer) {
        perror("Failed to allocate buffer");
        fclose(f);
        exit(1);
    }

    size_t bytesRead = fread(buffer, sizeof(unsigned char), WIDTH * HEIGHT, f);
    fclose(f);
    if (bytesRead != WIDTH * HEIGHT) {
        fprintf(stderr, "Failed to read full image. Read %zu bytes, expected %d.\n", 
                bytesRead, WIDTH * HEIGHT);
        free(buffer);
        exit(1);
    }

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            image[x][y] = (float)buffer[y * WIDTH + x];
        }
    }

    free(buffer);
}

float **listgorare2() {
    printf("Kom in i listgorare2 \n");
    float **image = (float **)malloc(WIDTH * sizeof(float *));
    if (!image) {
        printf("Failed to allocate outer array.\n");
        return NULL;
    }

    for (int x = 0; x < WIDTH; x++) {
        image[x] = (float *)malloc(HEIGHT * sizeof(float));
        if (!image[x]) {
            printf("Failed to allocate column %d\n", x);
            for (int i = 0; i < x; i++) free(image[i]);
            free(image);
            return NULL;
        }
    }

    printf("Klarade att ska en image och ska nu köra loadPGM \n");
    loadPGM("helluheightInfoBILD.pgm", image);
    printImagePreview(image, WIDTH, HEIGHT, 10, 10);  // Print a 10×10 preview
    return image;
}


void printImagePreview(float **image, int width, int height, int maxX, int maxY) {
    printf("\nImage preview (%dx%d):\n", maxX, maxY);
    for (int y = 0; y < maxY && y < height; y++) {
        for (int x = 0; x < maxX && x < width; x++) {
            printf("%3.0f ", image[x][y]);
        }
        printf("\n");
    }
}