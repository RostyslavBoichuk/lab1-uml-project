#ifndef MATRIX_C_TESTABLE_H
#define MATRIX_C_TESTABLE_H

/**
 * @file matrix_c_testable.h
 * @brief Header file for testable matrix operations.
 *
 * This header defines the interface for matrix operations
 * that are compatible with unit testing.
 */

#define MAX_SIZE 10

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Adds two matrices.
 *
 * @param A First matrix.
 * @param B Second matrix.
 * @param rows Number of rows.
 * @param cols Number of columns.
 * @param result Output matrix containing the sum.
 */
void AddMatrix(int A[10][10], int B[10][10], int rows, int cols, int result[10][10]);

/**
 * @brief Subtracts two matrices.
 */
void SubMatrix(int A[10][10], int B[10][10], int rows, int cols, int result[10][10]);

/**
 * @brief Multiplies matrix by scalar.
 */
void ScalarProduct(int A[10][10], int scalar, int rows, int cols, int result[10][10]);

/**
 * @brief Multiplies two matrices.
 */
void ProductMatrix(int A[10][10], int B[10][10], int rowsA, int colsA, int colsB, int result[10][10]);

/**
 * @brief Computes transpose of a matrix.
 */
void TransposeMatrix(int A[10][10], int rows, int cols, int result[10][10]);

#ifdef __cplusplus
}
#endif

#endif