#ifndef PATHFINDING_H
#define PATHFINDING_H

#define DIAGONAL_COST 1.414f

typedef struct node {
    int y;
    int x;
    float z;
    struct node* before;
    float costFromStart;
    float estimatedCostToTarget;
    float totalCost;
    int visited;
    int impassable;
} Node;

typedef struct grid {
    int width, height;
    Node **cells;
} Grid;

/* ----- Grid management functions ----- */

/**
 * @brief Create a new grid with specified dimensions
 * @param width Width of the grid
 * @param height Height of the grid
 * @param heightInfo 2D array of height values for terrain elevation
 * @return Pointer to the created grid
 */
Grid* createGrid(int width, int height, float** heightInfo);

/**
 * @brief Free a grid and all associated memory
 * @param grid The grid to free
 */
void freeGrid(Grid* grid);

/**
 * @brief Set the impassable state of a cell in the grid
 * @param grid The grid to modify
 * @param x X coordinate of the cell
 * @param y Y coordinate of the cell
 * @param value 1 for impassable, 0 for passable
 */
void setImpassable(Grid* grid, int x, int y, int value);

/* ----- A* algorithm core functions ----- */

/**
 * @brief Find the shortest path between two points on the grid
 * @param grid The grid to search in
 * @param startX X coordinate of the start point
 * @param startY Y coordinate of the start point
 * @param targetX X coordinate of the target point
 * @param targetY Y coordinate of the target point
 * @return Array of Node pointers representing the path, or NULL if no path exists
 */
Node** findPath(Grid* grid, int startX, int startY, int targetX, int targetY);

/**
 * @brief Calculate the heuristic distance/estimated cost between two points
 * @param startX X coordinate of the first point
 * @param startY Y coordinate of the first point
 * @param targetX X coordinate of the second point
 * @param targetY Y coordinate of the second point
 * @return The estimated cost between the points
 */
float calculateEstimatedCost(int startX, int startY, int targetX, int targetY);

/**
 * @brief Find the unvisited node with the lowest total cost
 * @param grid The grid to search in
 * @return The lowest cost unvisited node, or NULL if none found
 */
Node* getLowestCostNode(Grid* grid);

/**
 * @brief Process neighbors of the current node during pathfinding
 * @param grid The grid being searched
 * @param current The current node being processed
 * @param target The target destination node
 */
void processNeighbors(Grid* grid, Node* current, Node* target);

/* ----- Path handling functions ----- */

/**
 * @brief Reconstruct a path from linked nodes
 * @param targetNode The final node in the path
 * @return Array of Node pointers representing the path
 */
Node** reconstructPath(Node* targetNode);

/**
 * @brief Get the length of a path
 * @param path The path to measure
 * @return The length of the path
 */
int getPathLength(Node** path);

/**
 * @brief Free the memory used by a path
 * @param path The path to free
 */
void freePath(Node** path);

/* ----- Utility functions ----- */

/**
 * @brief Get a node at specific coordinates
 * @param grid The grid containing nodes
 * @param x X coordinate
 * @param y Y coordinate
 * @return Pointer to the node, or NULL if invalid coordinates
 */
Node* getNode(Grid* grid, int x, int y);

/**
 * @brief Check if coordinates are valid for the grid
 * @param grid The grid to check against
 * @param x X coordinate
 * @param y Y coordinate
 * @return 1 if valid, 0 if invalid
 */
int isValid(Grid* grid, int x, int y);

/**
 * @brief Reset a grid to prepare for a new search
 * @param grid The grid to reset
 */
void resetGrid(Grid* grid);

#endif /* PATHFINDING_H */