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
    
    grid->cells = (Node**)malloc(width * sizeof(Node*));
    if (!grid->cells) {
        free(grid);
        return NULL;
    }
    
    for (int x = 0; x < width; x++) {
        grid->cells[x] = (Node*)malloc(height * sizeof(Node));
        if (!grid->cells[x]) {
            for (int i = 0; i < x; i++) {
                free(grid->cells[i]);
            }
            free(grid->cells);
            free(grid);
            return NULL;
        }
        
        for (int y = 0; y < height; y++) {
            grid->cells[x][y].x = x;
            grid->cells[x][y].y = y;
            grid->cells[x][y].z = heightInfo ? heightInfo[x][y] : 0.0f;
            grid->cells[x][y].before = NULL;
            grid->cells[x][y].costFromStart = FLT_MAX;
            grid->cells[x][y].estimatedCostToTarget = FLT_MAX;
            grid->cells[x][y].totalCost = FLT_MAX;
            grid->cells[x][y].visited = 0;
            grid->cells[x][y].impassable = 0;
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
        for (int x = 0; x < grid->width; x++) {
            if (grid->cells[x]) {
                free(grid->cells[x]);  
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
      
    grid->cells[x][y].impassable = value;
}

/**
 * Returns a pointer to the node at specified coordinates.
 * Returns NULL if coordinates are invalid or grid is NULL.
 */
Node* getNode(Grid* grid, int x, int y) {
    if (!grid || !isValid(grid, x, y)) {
        return NULL;
    }
    
    return &(grid->cells[x][y]);
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

/**
 * Resets all pathfinding-related values in the grid to prepare for a new search.
 * Preserves grid structure, coordinates, and impassable status of cells.
 */
void resetGrid(Grid* grid) {
    if (!grid) {
        return;
    }
    
    for (int x = 0; x < grid->width; x++) {
        for (int y = 0; y < grid->height; y++) {
            grid->cells[x][y].before = NULL;
            grid->cells[x][y].costFromStart = FLT_MAX;
            grid->cells[x][y].estimatedCostToTarget = FLT_MAX;
            grid->cells[x][y].totalCost = FLT_MAX;
            grid->cells[x][y].visited = 0;
        }
    }
}