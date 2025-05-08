#include <stdio.h>
#include <stdlib.h>

const int listgorareCols = 1000;
const int listgorareRows = 1000;

float** listgorare() {
    float **heightinfo = (float **)malloc(listgorareCols * sizeof(float *));
    if (heightinfo == NULL) {
        printf("Något gick snett i minnesallokeringen för heightinfo :(\n");
        return NULL;
    }

    // listY koordinater 
    for (int i = 0; i < listgorareCols; i++) {
        heightinfo[i] = (float *)malloc(listgorareRows * sizeof(float));
        if (heightinfo[i] == NULL) {
            printf("Något gick snett i minnesallokeringen för rad %d :(\n", i);
            for (int j = 0; j < i; j++) {
                free(heightinfo[j]);
            }
            free(heightinfo);
            return NULL;
        }
    }

    // INitalize hela grid med 0 
    for (int i = 0; i < listgorareCols; i++) {
        for (int j = 0; j < listgorareRows; j++) {
            heightinfo[i][j] = 0;
        }
    }

    /*for (int i = 0; i < listgorareRows; i++) {
        free(heightinfo[i]);
    }
    //free(heightinfo);*/

    return heightinfo;
}