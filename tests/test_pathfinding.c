#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "../include/pathfinding.h"
#include <float.h>
#include <math.h>

/* Test fixtures */
Grid* testGrid = NULL;
float** testHeightData = NULL;

/**
 * Setup function for tests - allocates and initializes test data.
 * Creates a 10x10 grid with mostly flat terrain and a few height variations.
 */
int init_suite(void) {
    testHeightData = (float**)malloc(10 * sizeof(float*));
    if (!testHeightData) return -1;
    
    for (int i = 0; i < 10; i++) {
        testHeightData[i] = (float*)malloc(10 * sizeof(float));
        if (!testHeightData[i]) {
            for (int j = 0; j < i; j++) {
                free(testHeightData[j]);
            }
            free(testHeightData);
            return -1;
        }
        
        for (int j = 0; j < 10; j++) {
            testHeightData[i][j] = 0.0f;
        }
    }
    
    testHeightData[5][5] = 1.0f;
    testHeightData[6][6] = 2.0f;
    testHeightData[7][7] = 3.0f;
    
    return 0;
}

/**
 * Cleanup function for tests - frees all allocated test resources.
 */
int clean_suite(void) {
    if (testHeightData) {
        for (int i = 0; i < 10; i++) {
            free(testHeightData[i]);
        }
        free(testHeightData);
        testHeightData = NULL;
    }
    
    if (testGrid) {
        freeGrid(testGrid);
        testGrid = NULL;
    }
    
    return 0;
}

/**
 * Tests the createGrid function to ensure it properly initializes a grid.
 * Verifies grid dimensions, cell initialization, and specific height values.
 */
void test_createGrid(void) {
    testGrid = createGrid(10, 10, testHeightData);
    
    CU_ASSERT_PTR_NOT_NULL(testGrid);
    CU_ASSERT_EQUAL(testGrid->width, 10);
    CU_ASSERT_EQUAL(testGrid->height, 10);
    
    CU_ASSERT_PTR_NOT_NULL(testGrid->cells);
    
    CU_ASSERT_DOUBLE_EQUAL(testGrid->cells[5][5].z, 1.0f, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(testGrid->cells[6][6].z, 2.0f, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(testGrid->cells[7][7].z, 3.0f, 0.001);
    
    Node* testNode = &testGrid->cells[0][0];
    CU_ASSERT_EQUAL(testNode->x, 0);
    CU_ASSERT_EQUAL(testNode->y, 0);
    CU_ASSERT_PTR_NULL(testNode->before);
    CU_ASSERT_EQUAL(testNode->visited, 0);
    CU_ASSERT_EQUAL(testNode->impassable, 0);
    CU_ASSERT_DOUBLE_EQUAL(testNode->costFromStart, FLT_MAX, 0.001);
}

/**
 * Tests the setImpassable function to ensure it correctly marks cells.
 * Checks that specified cells are marked impassable and others remain passable.
 */
void test_setImpassable(void) {
    if (!testGrid) {
        testGrid = createGrid(10, 10, testHeightData);
    }
    
    setImpassable(testGrid, 2, 2, 1);
    setImpassable(testGrid, 2, 3, 1);
    setImpassable(testGrid, 2, 4, 1);
    
    CU_ASSERT_EQUAL(testGrid->cells[2][2].impassable, 1);
    CU_ASSERT_EQUAL(testGrid->cells[2][3].impassable, 1);
    CU_ASSERT_EQUAL(testGrid->cells[2][4].impassable, 1);
    
    CU_ASSERT_EQUAL(testGrid->cells[1][1].impassable, 0);
    
    setImpassable(testGrid, -1, -1, 1);
    setImpassable(testGrid, 20, 20, 1);
}

/**
 * Tests the getNode function to ensure it correctly returns nodes.
 * Verifies valid node retrieval and handling of invalid cases.
 */
void test_getNode(void) {
    if (!testGrid) {
        testGrid = createGrid(10, 10, testHeightData);
    }
    
    Node* node = getNode(testGrid, 5, 5);
    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_EQUAL(node->x, 5);
    CU_ASSERT_EQUAL(node->y, 5);
    CU_ASSERT_DOUBLE_EQUAL(node->z, 1.0f, 0.001);
    
    CU_ASSERT_PTR_NULL(getNode(testGrid, -1, -1));
    CU_ASSERT_PTR_NULL(getNode(testGrid, 20, 20));
    
    CU_ASSERT_PTR_NULL(getNode(NULL, 5, 5));
}

/**
 * Tests the isValid function to ensure it correctly validates coordinates.
 * Checks boundary conditions and invalid cases.
 */
void test_isValid(void) {
    if (!testGrid) {
        testGrid = createGrid(10, 10, testHeightData);
    }
    
    CU_ASSERT_TRUE(isValid(testGrid, 0, 0));
    CU_ASSERT_TRUE(isValid(testGrid, 9, 9));
    CU_ASSERT_TRUE(isValid(testGrid, 5, 5));
    
    CU_ASSERT_FALSE(isValid(testGrid, -1, 5));
    CU_ASSERT_FALSE(isValid(testGrid, 5, -1));
    CU_ASSERT_FALSE(isValid(testGrid, 10, 5));
    CU_ASSERT_FALSE(isValid(testGrid, 5, 10));
    
    CU_ASSERT_FALSE(isValid(NULL, 5, 5));
}

/**
 * Tests the resetGrid function to ensure it correctly resets pathfinding values.
 * Verifies that node properties are reset while preserving impassable status.
 */
void test_resetGrid(void) {
    if (!testGrid) {
        testGrid = createGrid(10, 10, testHeightData);
    }
    
    testGrid->cells[1][1].costFromStart = 10.0f;
    testGrid->cells[1][1].visited = 1;
    testGrid->cells[1][1].before = &testGrid->cells[0][0];
    testGrid->cells[1][1].impassable = 1;
    
    resetGrid(testGrid);
    
    CU_ASSERT_DOUBLE_EQUAL(testGrid->cells[1][1].costFromStart, FLT_MAX, 0.001);
    CU_ASSERT_EQUAL(testGrid->cells[1][1].visited, 0);
    CU_ASSERT_PTR_NULL(testGrid->cells[1][1].before);
    
    CU_ASSERT_EQUAL(testGrid->cells[1][1].impassable, 1);
}

/**
 * Tests the calculateEstimatedCost function to ensure it correctly calculates distances.
 * Tests horizontal, vertical, diagonal, and complex movements.
 */
void test_calculateEstimatedCost(void) {
    CU_ASSERT_DOUBLE_EQUAL(calculateEstimatedCost(0, 0, 5, 0), 5.0f, 0.001);
    CU_ASSERT_DOUBLE_EQUAL(calculateEstimatedCost(0, 0, 0, 5), 5.0f, 0.001);
    
    CU_ASSERT_DOUBLE_EQUAL(calculateEstimatedCost(0, 0, 5, 5), 5.0f, 0.001);
    
    CU_ASSERT_DOUBLE_EQUAL(calculateEstimatedCost(2, 3, 7, 5), 5.0f, 0.001);
}

/**
 * Tests the findPath function with a straight-line path.
 * Verifies that the function correctly finds the shortest path
 * in a vertical straight line from (0,0) to (0,9).
 */
void test_findPath_straight(void) {
    if (!testGrid) {
        testGrid = createGrid(10, 10, testHeightData);
    }
    
    resetGrid(testGrid);
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            testGrid->cells[x][y].impassable = 0;
        }
    }
    
    Node** path = findPath(testGrid, 0, 0, 0, 9);
    
    CU_ASSERT_PTR_NOT_NULL(path);
    
    int length = 0;
    while (path[length] != NULL) length++;
    
    CU_ASSERT_TRUE(length == 10);
    
    if (path) {
        CU_ASSERT_EQUAL(path[0]->x, 0);
        CU_ASSERT_EQUAL(path[0]->y, 0);
        
        CU_ASSERT_EQUAL(path[length-1]->x, 0);
        CU_ASSERT_EQUAL(path[length-1]->y, 9);
        
        freePath(path);
    }
}

/**
 * Tests the findPath function with a simple empty grid case.
 * Verifies path existence, length, and correctness of start/end points.
 */
void test_findPath_simple(void) {
    if (!testGrid) {
        testGrid = createGrid(10, 10, testHeightData);
    }
    
    resetGrid(testGrid);
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            testGrid->cells[x][y].impassable = 0;
        }
    }
    
    Node** path = findPath(testGrid, 0, 0, 9, 9);
    
    CU_ASSERT_PTR_NOT_NULL(path);
    
    int length = 0;
    while (path[length] != NULL) length++;
    
    CU_ASSERT_TRUE(length > 0);
    CU_ASSERT_TRUE(length <= 18);
    
    if (path) {
        CU_ASSERT_EQUAL(path[0]->x, 0);
        CU_ASSERT_EQUAL(path[0]->y, 0);
        
        CU_ASSERT_EQUAL(path[length-1]->x, 9);
        CU_ASSERT_EQUAL(path[length-1]->y, 9);
        
        freePath(path);
    }
}

/**
 * Tests the findPath function with obstacles in the grid.
 * Verifies path existence and that it correctly navigates around obstacles.
 */
void test_findPath_obstacle(void) {
    if (!testGrid) {
        testGrid = createGrid(10, 10, testHeightData);
    }
    
    resetGrid(testGrid);
    
    for (int y = 0; y < 8; y++) {
        setImpassable(testGrid, 5, y, 1);
    }
    
    Node** path = findPath(testGrid, 0, 0, 9, 0);
    
    CU_ASSERT_PTR_NOT_NULL(path);
    
    if (path) {
        int length = 0;
        while (path[length] != NULL) length++;
        
        CU_ASSERT_TRUE(length > 9);
        
        CU_ASSERT_EQUAL(path[0]->x, 0);
        CU_ASSERT_EQUAL(path[0]->y, 0);
        CU_ASSERT_EQUAL(path[length-1]->x, 9);
        CU_ASSERT_EQUAL(path[length-1]->y, 0);
        
        int wentAround = 0;
        for (int i = 0; i < length; i++) {
            if (path[i]->y >= 8) {
                wentAround = 1;
                break;
            }
        }
        CU_ASSERT_TRUE(wentAround);
        
        freePath(path);
    }
}

/**
 * Tests the findPath function with an impossible path scenario.
 * Verifies that the function correctly returns NULL when no path is possible.
 */
void test_findPath_impossible(void) {
    if (!testGrid) {
        testGrid = createGrid(10, 10, testHeightData);
    }
    
    resetGrid(testGrid);
    
    for (int y = 0; y < 10; y++) {
        setImpassable(testGrid, 5, y, 1);
    }
    
    Node** path = findPath(testGrid, 0, 5, 9, 5);
    
    CU_ASSERT_PTR_NULL(path);
}

/**
 * Tests the getPathLength function to ensure it correctly calculates path lengths.
 * Verifies diagonal vs orthogonal movement costs and handling of NULL paths.
 */
void test_getPathLength(void) {
    Node nodes[5];
    Node* path[6];
    
    nodes[0].x = 0; nodes[0].y = 0;
    nodes[1].x = 1; nodes[1].y = 1;
    nodes[2].x = 2; nodes[2].y = 1;
    nodes[3].x = 2; nodes[3].y = 2;
    nodes[4].x = 3; nodes[4].y = 3;
    
    path[0] = &nodes[0];
    path[1] = &nodes[1];
    path[2] = &nodes[2];
    path[3] = &nodes[3];
    path[4] = &nodes[4];
    path[5] = NULL;
    
    int length = getPathLength(path);
    
    CU_ASSERT_EQUAL(length, (int)(2*1.414 + 2));
    
    CU_ASSERT_EQUAL(getPathLength(NULL), 0);
}

/**
 * Main function to set up and run CUnit tests.
 * Initializes registry, adds test suite, adds individual tests, and runs the test suite.
 */
int main() {
    CU_pSuite pSuite = NULL;
    
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    pSuite = CU_add_suite("Pathfinding_Suite", init_suite, clean_suite);
    
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if ((NULL == CU_add_test(pSuite, "Test createGrid", test_createGrid)) ||
        (NULL == CU_add_test(pSuite, "Test setImpassable", test_setImpassable)) ||
        (NULL == CU_add_test(pSuite, "Test getNode", test_getNode)) ||
        (NULL == CU_add_test(pSuite, "Test isValid", test_isValid)) ||
        (NULL == CU_add_test(pSuite, "Test resetGrid", test_resetGrid)) ||
        (NULL == CU_add_test(pSuite, "Test calculateEstimatedCost", test_calculateEstimatedCost)) ||
        (NULL == CU_add_test(pSuite,"Test findPath in a straight line", test_findPath_straight)) ||
        (NULL == CU_add_test(pSuite, "Test findPath simple case", test_findPath_simple)) ||
        (NULL == CU_add_test(pSuite, "Test findPath with obstacle", test_findPath_obstacle)) ||
        (NULL == CU_add_test(pSuite, "Test findPath impossible path", test_findPath_impossible)) ||
        (NULL == CU_add_test(pSuite, "Test getPathLength", test_getPathLength))) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}