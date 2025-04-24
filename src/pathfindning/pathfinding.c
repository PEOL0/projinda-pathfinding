#include "pathfinding.h"
#include <stdlib.h>
#include <float.h>

Grid* createGrid(int width, int height, float** heightInfo) {
    Grid* grid = (Grid*)malloc(sizeof(Grid));
    if (!grid) {
        return NULL;
    }

    grid->width = width;
    grid->height = height;
    
    grid->cells = (Node**)malloc(height * sizeof(Node*));
    if (!grid->cells) {
        free(grid);
        return NULL;
    }
    
    for (int y = 0; y < height; y++) {
        grid->cells[y] = (Node*)malloc(width * sizeof(Node));
        if (!grid->cells[y]) {
            for (int i = 0; i < y; i++) {
                free(grid->cells[i]);
            }
            free(grid->cells);
            free(grid);
            return NULL;
        }
        
        for (int x = 0; x < width; x++) {
            grid->cells[y][x].x = x;
            grid->cells[y][x].y = y;
            grid->cells[y][x].z = heightInfo ? heightInfo[y][x] : 0.0f;
            grid->cells[y][x].before = NULL;
            grid->cells[y][x].costFromStart = FLT_MAX;
            grid->cells[y][x].estimatedCostToTarget = FLT_MAX;
            grid->cells[y][x].totalCost = FLT_MAX;
            grid->cells[y][x].visited = 0;
            grid->cells[y][x].impassable = 0;
        }
    }
    
    return grid;
}