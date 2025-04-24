#ifndef PATHFINDING_H
#define PATHFINDING_H

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

#endif