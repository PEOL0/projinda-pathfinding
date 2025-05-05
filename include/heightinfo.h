#include <stdio.h>
#include <stdlib.h>

float** listgorare() {
    int cols = 1450;
    int rows = 740;


    float **heightinfo = (float **)malloc(cols * sizeof(float *));
    if (heightinfo == NULL) {
        printf("Något gick snett i minnesallokeringen :(\n");
        return NULL;
    }

    // listY koordinater 
    for (int i = 0; i < cols; i++) {
        heightinfo[i] = (float *)malloc(rows * sizeof(float));
        if (heightinfo == NULL) {
            printf("Något gick snett i minnesallokeringen :(\n");
            return NULL;
        }
    }

    // INitalize hela grid med 0 
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            heightinfo[i][j] = 0;
        }
    }

    /*for (int i = 0; i < rows; i++) {
        free(heightinfo[i]);
    }
    //free(heightinfo);*/


    return heightinfo;
}