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
 * Calculates the estimated cost (heuristic) between two points using diagonal distance.
 * Returns a float representing the diagonal distance between the points.
 */
float calculateEstimatedCost(int startX, int startY, int targetX, int targetY) {
    int dx = abs(startX - targetX);
    int dy = abs(startY - targetY);
    
    return (float)(dx > dy ? dx : dy);
}

/**
 * Finds the unvisited node with the lowest total cost in the grid.
 * Used by pathfinding algorithms to determine the next node to explore.
 */
Node* getLowestCostNode(Grid* grid) {
    if (!grid) {
        return NULL;
    }
    
    Node* lowestCostNode = NULL;
    float lowestCost = FLT_MAX;
    
    for (int x = 0; x < grid->width; x++) {
        for (int y = 0; y < grid->height; y++) {
            Node* currentNode = &grid->cells[x][y];
            if (!currentNode->visited && !currentNode->impassable && 
                currentNode->totalCost < lowestCost) {
                lowestCost = currentNode->totalCost;
                lowestCostNode = currentNode;
            }
        }
    }
    
    return lowestCostNode;
}

/**
 * Processes all valid neighboring nodes of the current node during pathfinding.
 * Updates cost values and path connections for neighbors if a better path is found.
 */
void processNeighbors(Grid* grid, Node* current, Node* target) {
    if (!grid || !current || !target) {
        return;
    }
    
    int dx[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int dy[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    
    for (int i = 0; i < 8; i++) {
        int newX = current->x + dx[i];
        int newY = current->y + dy[i];
        
        if (!isValid(grid, newX, newY)) {
            continue;
        }
        
        Node* neighbor = getNode(grid, newX, newY);
        if (!neighbor || neighbor->impassable || neighbor->visited) {
            continue;
        }
        
        
        float moveCost = (dx[i] != 0 && dy[i] != 0) ? 1.414f : 1.0f;
        
        
        if (neighbor->z != current->z) {
            moveCost += fabsf(neighbor->z - current->z) * 0.5f;
        }
        
        float newCostFromStart = current->costFromStart + moveCost;
        
        if (newCostFromStart < neighbor->costFromStart) {
            neighbor->before = current;
            neighbor->costFromStart = newCostFromStart;
            neighbor->estimatedCostToTarget = calculateEstimatedCost(
                neighbor->x, neighbor->y, target->x, target->y
            );
            neighbor->totalCost = neighbor->costFromStart + neighbor->estimatedCostToTarget;
        }
    }
}

/**
 * Reconstructs the complete path from start to target by walking backwards through the linked nodes starting from the target node.
 * Returns a NULL-terminated array of Node pointers representing the path.
 */
Node** reconstructPath(Node* targetNode) {
    if (!targetNode) {
        return NULL;
    }
    
    int length = 1;
    Node* current = targetNode;
    while (current->before) {
        length++;
        current = current->before;
    }
    
    Node** path = (Node**)malloc(sizeof(Node*) * (length + 1));
    if (!path) {
        return NULL;
    }
    

    current = targetNode;
    for (int i = length - 1; i >= 0; i--) {
        path[i] = current;
        current = current->before;
    }
    path[length] = NULL;
    
    return path;
}

int getPathLength(Node** path) {
    if (!path || !path[0]) {
        return 0;
    }
    
    int distance = 0;
    for (int i = 0; path[i] != NULL && path[i+1] != NULL; i++) {
        Node* current = path[i];
        Node* next = path[i+1];
        
        if (abs(current->x - next->x) > 0 && abs(current->y - next->y) > 0) {
            distance += 1.414;
        } else {
            distance += 1;
        }
    }
    
}

/**
 * Frees memory allocated for a path array.
 * Safely handles NULL paths
 */
void freePath(Node** path) {
    if (path) {
        free(path);
    }
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