#include "pathfinding.h"
#include <stdlib.h>
#include <float.h>

/**
 * Creates a 2D grid for pathfinding with specified dimensions.
 * Allocates memory for the grid structure and all cells.
 * Initializes each cell with coordinates, height (if provided), 
 * and default pathfinding values. Returns NULL if memory allocation fails.
 */
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

/**
 * Frees all memory associated with a grid including all cell rows
 * and the grid structure itself.
 */
void freeGrid(Grid* grid) {
    if (!grid) return;  
    
    if (grid->cells) {  
        for (int y = 0; y < grid->height; y++) {
            if (grid->cells[y]) {
                free(grid->cells[y]);  
            }
        }
        free(grid->cells);
    }
    
    free(grid);
}

/**
 * Sets a cell as passable or impassable.
 * Safely checks grid and coordinates validity before making changes.
 */
void setImpassable(Grid* grid, int x, int y, int value) {
    
    if (!grid || !isValid(grid, x, y)) {
        return;
    }
      
    grid->cells[y][x].impassable = value;
}

/**
 * Checks if the given coordinates are within the grid boundaries.
 */
int isValid(Grid* grid, int x, int y) {
    if (!grid) {
        return 0;
    }
    
    if (x >= 0 && x < grid->width && y >= 0 && y < grid->height) {
        return 1;
    } else {
        return 0;
    }
}