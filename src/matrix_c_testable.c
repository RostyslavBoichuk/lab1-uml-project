// matrix_c_testable.c

/**
 * @file matrix_c_testable.c
 * @brief Testable implementation of matrix operations.
 *
 * This file contains matrix functions refactored to support
 * unit testing by returning results via output parameters.
 *
 * @warning Uses fixed-size arrays (MAX_SIZE = 10).
 */

#include <stdio.h>
#include "matrix_c_testable.h"

/**
 * @brief Global loop variables (legacy design).
 *
 * @details These variables are used for iteration.
 * Using global variables for loops is considered bad practice.
 */
int i, j, k; // for loops

// Matrix I/O

/**
 * @brief Reads matrix elements from standard input.
 *
 * @param array Matrix to fill.
 * @param rows Number of rows.
 * @param columns Number of columns.
 */
void ScanMatrix(int array[MAX_SIZE][MAX_SIZE], int rows, int columns) {
    for (i = 0; i < rows; i++) {
        printf("\t%d entries for row %d: ", columns, i + 1);
        for (j = 0; j < columns; j++)
            scanf("%d", &array[i][j]);
    }
}

/**
 * @brief Prints matrix to console.
 */
void DisplayMatrix(int array[MAX_SIZE][MAX_SIZE], int rows, int columns) {
    for (i = 0; i < rows; i++) {
        for (j = 0; j < columns; j++)
            printf("\t%d", array[i][j]);
        printf("\n");
    }
}

// Matrix Operations

/**
 * @brief Adds two matrices and stores result in output matrix.
 */
void AddMatrix(int arrayone[MAX_SIZE][MAX_SIZE], int arraytwo[MAX_SIZE][MAX_SIZE], int rows, int columns, int result[MAX_SIZE][MAX_SIZE]) {
    for (i = 0; i < rows; i++)
        for (j = 0; j < columns; j++)
            result[i][j] = arrayone[i][j] + arraytwo[i][j];
}

/**
 * @brief Subtracts two matrices.
 */
void SubMatrix(int arrayone[MAX_SIZE][MAX_SIZE], int arraytwo[MAX_SIZE][MAX_SIZE], int rows, int columns, int result[MAX_SIZE][MAX_SIZE]) {
    for (i = 0; i < rows; i++)
        for (j = 0; j < columns; j++)
            result[i][j] = arrayone[i][j] - arraytwo[i][j];
}

/**
 * @brief Multiplies matrix by scalar.
 */
void ScalarProduct(int array[MAX_SIZE][MAX_SIZE], int scalar, int rows, int columns, int result[MAX_SIZE][MAX_SIZE]) {
    for (i = 0; i < rows; i++)
        for (j = 0; j < columns; j++)
            result[i][j] = scalar * array[i][j];
}

/**
 * @brief Multiplies two matrices.
 */
void ProductMatrix(int arrayone[MAX_SIZE][MAX_SIZE], int arraytwo[MAX_SIZE][MAX_SIZE], int rowsA, int columnsA, int columnsB, int result[MAX_SIZE][MAX_SIZE]) {
    for (i = 0; i < rowsA; i++)
        for (j = 0; j < columnsB; j++)
            result[i][j] = 0;

    for (i = 0; i < rowsA; i++)
        for (j = 0; j < columnsB; j++)
            for (k = 0; k < columnsA; k++)
                result[i][j] += arrayone[i][k] * arraytwo[k][j];
}

/**
 * @brief Computes transpose of a matrix.
 */
void TransposeMatrix(int arrayone[MAX_SIZE][MAX_SIZE], int rows, int columns, int result[MAX_SIZE][MAX_SIZE]) {
    for (i = 0; i < rows; i++)
        for (j = 0; j < columns; j++)
            result[j][i] = arrayone[i][j];
}