#include <stdio.h>
#include <stdlib.h>

const int listgorareCols = 1450;
const int listgorareRows = 740;

float** listgorare() {


    float **heightinfo = (float **)malloc(listgorareCols * sizeof(float *));
    if (heightinfo == NULL) {
        printf("Något gick snett i minnesallokeringen :(\n");
        return NULL;
    }

    // listY koordinater 
    for (int i = 0; i < listgorareCols; i++) {
        heightinfo[i] = (float *)malloc(listgorareRows * sizeof(float));
        if (heightinfo == NULL) {
            printf("Något gick snett i minnesallokeringen :(\n");
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