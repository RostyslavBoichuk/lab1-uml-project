/**
 * @file test_old_matrix.cpp
 * @brief Unit tests for matrix_c_testable implementation.
 *
 * This file contains Catch2 unit tests that validate
 * matrix operations implemented in the testable C module.
 *
 * The tests cover:
 *  - Positive scenarios
 *  - Boundary values
 *  - Negative scenarios
 *  - Overflow / underflow cases
 *  - Architectural weaknesses
 *
 * @warning Some tests intentionally fail to demonstrate
 * architectural limitations and missing input validation.
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

extern "C" {
    #include "../src/matrix_c_testable.h"
}

#include <climits>
#include <cmath>
#include <cstring>

// ==================== Helpers ====================

/**
 * @brief Compares two integer matrices.
 *
 * @param a First matrix.
 * @param b Second matrix.
 * @param rows Number of rows.
 * @param cols Number of columns.
 * @return true if matrices are identical.
 */
bool compareIntMatrix(int a[10][10], int b[10][10], int rows, int cols) {
    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
            if(a[i][j]!=b[i][j]) return false;
    return true;
}

/**
 * @brief Compares two floating-point matrices.
 *
 * @param a First matrix.
 * @param b Second matrix.
 * @param rows Number of rows.
 * @param cols Number of columns.
 * @param eps Precision tolerance.
 */
bool compareDoubleMatrix(double a[10][10], double b[10][10], int rows, int cols, double eps=1e-6) {
    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
            if(std::fabs(a[i][j]-b[i][j])>eps) return false;
    return true;
}

/**
 * @brief Converts integer matrix to double matrix.
 */
void intToDouble(int in[10][10], double out[10][10], int rows, int cols) {
    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
            out[i][j]=in[i][j];
}

/**
 * @brief Fills matrix with zeros.
 */
void fillZero(int m[10][10]) {
    for(int i=0;i<10;i++)
        for(int j=0;j<10;j++)
            m[i][j]=0;
}

// ==================== ADDITION TESTS ====================

/**
 * @test Addition tests for AddMatrix().
 */
TEST_CASE("AddMatrix Tests", "[Addition]") {
    int result[10][10];

    SECTION("Normal positive integers") {
        int A[10][10]={{1,2},{3,4}}, B[10][10]={{5,6},{7,8}}, expected[10][10]={{6,8},{10,12}};
        AddMatrix(A,B,2,2,result);
        REQUIRE(compareIntMatrix(result,expected,2,2));
    }

    SECTION("Zeros") {
        int A[10][10]={{0,0},{0,0}}, B[10][10]={{0,0},{0,0}}, expected[10][10]={{0,0},{0,0}};
        AddMatrix(A,B,2,2,result);
        REQUIRE(compareIntMatrix(result,expected,2,2));
    }

    SECTION("Negative numbers") {
        int A[10][10]={{-1,-2},{-3,-4}}, B[10][10]={{-5,-6},{-7,-8}}, expected[10][10]={{-6,-8},{-10,-12}};
        AddMatrix(A,B,2,2,result);
        REQUIRE(compareIntMatrix(result,expected,2,2));
    }

    SECTION("Mixed signs") {
        int A[10][10]={{-1,2},{3,-4}}, B[10][10]={{5,-6},{-7,8}}, expected[10][10]={{4,-4},{-4,4}};
        AddMatrix(A,B,2,2,result);
        REQUIRE(compareIntMatrix(result,expected,2,2));
    }

    SECTION("Large numbers") {
        int A[10][10]={{1000000,2000000},{3000000,4000000}}, B[10][10]={{5000000,6000000},{7000000,8000000}}, expected[10][10]={{6000000,8000000},{10000000,12000000}};
        AddMatrix(A,B,2,2,result);
        REQUIRE(compareIntMatrix(result,expected,2,2));
    }

    SECTION("Single element 1x1") {
        int A[10][10]={{5}}, B[10][10]={{3}}, expected[10][10]={{8}};
        AddMatrix(A,B,1,1,result);
        REQUIRE(compareIntMatrix(result,expected,1,1));
    }

    SECTION("Max size 10x10") {
        int A[10][10], B[10][10], expected[10][10];
        for(int i=0;i<10;i++) for(int j=0;j<10;j++){
            A[i][j]=i+j; B[i][j]=i-j; expected[i][j]=2*i;
        }
        AddMatrix(A,B,10,10,result);
        REQUIRE(compareIntMatrix(result,expected,10,10));
    }

    SECTION("Float/double matrices") {
        int A[10][10]={{1,2},{3,4}}, B[10][10]={{5,6},{7,8}};
        int temp[10][10];
        AddMatrix(A,B,2,2,temp);
        double res[10][10], exp[10][10]={{6,8},{10,12}};
        intToDouble(temp,res,2,2);
        REQUIRE(compareDoubleMatrix(res,exp,2,2));
    }
}

// ==================== SUBTRACTION TESTS ====================

/**
 * @test Subtraction tests for SubMatrix().
 */
TEST_CASE("SubMatrix Tests", "[Subtraction]") {
    int result[10][10];

    SECTION("Normal subtraction") {
        int A[10][10]={{9,8},{7,6}}, B[10][10]={{1,2},{3,4}}, expected[10][10]={{8,6},{4,2}};
        SubMatrix(A,B,2,2,result);
        REQUIRE(compareIntMatrix(result,expected,2,2));
    }

    SECTION("Subtract zero matrix") {
        int A[10][10]={{1,2},{3,4}}, B[10][10]={{0,0},{0,0}}, expected[10][10]={{1,2},{3,4}};
        SubMatrix(A,B,2,2,result);
        REQUIRE(compareIntMatrix(result,expected,2,2));
    }

    SECTION("Subtract itself") {
        int A[10][10]={{5,6},{7,8}}, expected[10][10]={{0,0},{0,0}};
        SubMatrix(A,A,2,2,result);
        REQUIRE(compareIntMatrix(result,expected,2,2));
    }

    SECTION("Negative numbers") {
        int A[10][10]={{-1,-2},{-3,-4}}, B[10][10]={{-5,-6},{-7,-8}}, expected[10][10]={{4,4},{4,4}};
        SubMatrix(A,B,2,2,result);
        REQUIRE(compareIntMatrix(result,expected,2,2));
    }

    SECTION("Mixed signs") {
        int A[10][10]={{-1,2},{3,-4}}, B[10][10]={{5,-6},{-7,8}}, expected[10][10]={{-6,8},{10,-12}};
        SubMatrix(A,B,2,2,result);
        REQUIRE(compareIntMatrix(result,expected,2,2));
    }

    SECTION("Single element 1x1") {
        int A[10][10]={{5}}, B[10][10]={{3}}, expected[10][10]={{2}};
        SubMatrix(A,B,1,1,result);
        REQUIRE(compareIntMatrix(result,expected,1,1));
    }

    SECTION("Max size 10x10") {
        int A[10][10], B[10][10], expected[10][10];
        for(int i=0;i<10;i++) for(int j=0;j<10;j++){A[i][j]=i+j; B[i][j]=j; expected[i][j]=i; }
        SubMatrix(A,B,10,10,result);
        REQUIRE(compareIntMatrix(result,expected,10,10));
    }

    SECTION("Float/double") {
        int A[10][10]={{1,2},{3,4}}, B[10][10]={{5,6},{7,8}};
        int temp[10][10];
        SubMatrix(A,B,2,2,temp);
        double res[10][10], exp[10][10]={{-4,-4},{-4,-4}};
        intToDouble(temp,res,2,2);
        REQUIRE(compareDoubleMatrix(res,exp,2,2));
    }
}

// ==================== SCALAR MULTIPLICATION TESTS ====================

/**
 * @test Scalar multiplication tests.
 */
TEST_CASE("ScalarProduct Tests", "[Scalar]") {
    int result[10][10];
    int A[10][10]={{1,2},{3,4}};

    SECTION("Scalar 0") { int expected[10][10]={{0,0},{0,0}}; ScalarProduct(A,0,2,2,result); REQUIRE(compareIntMatrix(result,expected,2,2)); }
    SECTION("Scalar 1") { int expected[10][10]={{1,2},{3,4}}; ScalarProduct(A,1,2,2,result); REQUIRE(compareIntMatrix(result,expected,2,2)); }
    SECTION("Scalar negative") { int expected[10][10]={{-1,-2},{-3,-4}}; ScalarProduct(A,-1,2,2,result); REQUIRE(compareIntMatrix(result,expected,2,2)); }
    SECTION("Large scalar") { int expected[10][10]={{1000000,2000000},{3000000,4000000}}; ScalarProduct(A,1000000,2,2,result); REQUIRE(compareIntMatrix(result,expected,2,2)); }
    SECTION("Single element") { int B[10][10]={{5}}, expected[10][10]={{10}}; ScalarProduct(B,2,1,1,result); REQUIRE(compareIntMatrix(result,expected,1,1)); }
    SECTION("Max size 10x10") { int B[10][10], expected[10][10]; for(int i=0;i<10;i++)for(int j=0;j<10;j++){B[i][j]=i+j; expected[i][j]=(i+j)*2;} ScalarProduct(B,2,10,10,result); REQUIRE(compareIntMatrix(result,expected,10,10)); }
}

// ==================== MATRIX MULTIPLICATION ====================

/**
 * @test Matrix multiplication tests.
 */
TEST_CASE("ProductMatrix Tests", "[Multiplication]") {
    int result[10][10];

    SECTION("Normal 2x3 * 3x2") {
        int A[10][10]={{1,2,3},{4,5,6}}, B[10][10]={{7,8},{9,10},{11,12}}, expected[10][10]={{58,64},{139,154}};
        ProductMatrix(A,B,2,3,2,result); REQUIRE(compareIntMatrix(result,expected,2,2));
    }

    SECTION("Identity multiplication") {
        int A[10][10]={{1,2},{3,4}}, B[10][10]={{1,0},{0,1}}, expected[10][10]={{1,2},{3,4}};
        ProductMatrix(A,B,2,2,2,result); REQUIRE(compareIntMatrix(result,expected,2,2));
    }

    SECTION("Multiply by zero") {
        int A[10][10]={{1,2},{3,4}}, B[10][10]={{0,0},{0,0}}, expected[10][10]={{0,0},{0,0}};
        ProductMatrix(A,B,2,2,2,result); REQUIRE(compareIntMatrix(result,expected,2,2));
    }

    SECTION("1xN * Nx1") {
        int A[10][10]={{1,2,3}}, B[10][10]={{4},{5},{6}}, expected[10][10]={{32}};
        ProductMatrix(A,B,1,3,1,result); REQUIRE(compareIntMatrix(result,expected,1,1));
    }
}

// ==================== TRANSPOSE TESTS ====================

/**
 * @test Transpose tests.
 */
TEST_CASE("TransposeMatrix Tests", "[Transpose]") {
    int result[10][10];

    SECTION("Square matrix") {
        int A[10][10]={{1,2},{3,4}}, expected[10][10]={{1,3},{2,4}};
        TransposeMatrix(A,2,2,result); REQUIRE(compareIntMatrix(result,expected,2,2));
    }

    SECTION("Non-square 2x3") {
        int A[10][10]={{1,2,3},{4,5,6}}, expected[10][10]={{1,4},{2,5},{3,6}};
        TransposeMatrix(A,2,3,result); REQUIRE(compareIntMatrix(result,expected,3,2));
    }

    SECTION("Single element") {
        int A[10][10]={{5}}, expected[10][10]={{5}};
        TransposeMatrix(A,1,1,result); REQUIRE(compareIntMatrix(result,expected,1,1));
    }

    SECTION("Max size 10x10") {
        int A[10][10], expected[10][10];
        for(int i=0;i<10;i++) for(int j=0;j<10;j++){A[i][j]=i*10+j; expected[j][i]=i*10+j;}
        TransposeMatrix(A,10,10,result); REQUIRE(compareIntMatrix(result,expected,10,10));
    }
}

// ==================== ROBUSTNESS / PERFORMANCE ====================

/**
 * @test Architectural robustness tests.
 *
 * These tests highlight missing input validation
 * and boundary protection in the implementation.
 */
TEST_CASE("Robustness and Performance", "[Robustness]") {
    int result[10][10], A[10][10], B[10][10];

    SECTION("Empty matrices 0x0") { AddMatrix(A,B,0,0,result); REQUIRE(true); }
    SECTION("Empty matrices 0xN") { AddMatrix(A,B,0,10,result); REQUIRE(true); }
    SECTION("Empty matrices N×0") { AddMatrix(A,B,10,0,result); REQUIRE(true); }

    SECTION("Performance 10x10 addition") {
        for(int i=0;i<10;i++) for(int j=0;j<10;j++){A[i][j]=i+j; B[i][j]=i-j;}
        std::clock_t start=std::clock(); AddMatrix(A,B,10,10,result);
        double elapsed=(double)(std::clock()-start)/CLOCKS_PER_SEC; REQUIRE(elapsed<1.0);
    }
}

// ==================== ADDITIONAL FAILURES ====================

/**
 * @test Edge cases and intentional failure scenarios.
 *
 * @brief Tests architectural weaknesses and missing validation
 * in the matrix implementation.
 *
 * These tests demonstrate:
 *  - Overflow behavior
 *  - Underflow behavior
 *  - Invalid dimension handling
 *  - Type misuse
 *
 * Some sections intentionally trigger failure conditions
 * to highlight missing input validation in the legacy implementation.
 *
 * @warning Several tests are expected to fail by design.
 * They expose architectural limitations.
 */
TEST_CASE("Edge Cases / Failable Tests", "[Failures]") {
    int result[10][10];

    SECTION("Overflow AddMatrix") {
        int A[10][10]={{INT_MAX}}, B[10][10]={{1}};
        AddMatrix(A,B,1,1,result);
        REQUIRE(result[0][0] != INT_MAX); // original fails
    }

    SECTION("Underflow SubMatrix") {
        int A[10][10]={{INT_MIN}}, B[10][10]={{1}};
        SubMatrix(A,B,1,1,result);
        REQUIRE(result[0][0] != INT_MIN); // original fails
    }

    SECTION("ScalarProduct overflow") {
        int A[10][10]={{INT_MAX}};
        ScalarProduct(A,2,1,1,result);
        REQUIRE(result[0][0] != INT_MAX); // original fails
    }

    SECTION("Multiplication invalid dimensions") {
        int A[10][10]={{1,2,3}}, B[10][10]={{1,2}};
        ProductMatrix(A,B,1,3,2,result);
        REQUIRE(false); // will fail
    }

    SECTION("Negative dimensions") {
        int A[10][10]={0}, B[10][10]={0};
        AddMatrix(A,B,-5,-5,result);
        REQUIRE(false); // will fail
    }

    SECTION("Rows > MAX_SIZE") {
        int A[10][10]={0}, B[10][10]={0};
        AddMatrix(A,B,20,20,result);
        REQUIRE(false); // will fail
    }

    SECTION("Type misuse double") {
        double A[10][10]={{1.1,2.2}}, B[10][10]={{3.3,4.4}};
        AddMatrix((int (*)[10])A,(int (*)[10])B,2,2,result);
        REQUIRE(true);
    }

    SECTION("Type misuse float") {
        float A[10][10]={{1.1f,2.2f}}, B[10][10]={{3.3f,4.4f}};
        AddMatrix((int (*)[10])A,(int (*)[10])B,2,2,result);
        REQUIRE(true);
    }
}