/**
 * @file test_new_matrix.cpp
 * @brief Unit tests for the improved matrix implementation.
 *
 * Tests:
 *  - MatrixOperations
 *  - Transpose
 *  - Classical multiplication
 *  - Strassen multiplication
 *  - Edge cases
 *
 * Uses Catch2 framework.
 */

#define CATCH_CONFIG_MAIN
#define TESTING
#include "catch.hpp"
#include <limits>
#include <cmath>
#include <memory>
#include <cstdlib>
#include "../fixed_matrix.cpp"

// HELPERS

/**
 * @brief Compares two integer matrices.
 */
template<typename T>
bool compareMatrix(const Matrix<T>& A, const Matrix<T>& B, double eps=1e-6) {
    if(A.rows() != B.rows() || A.cols() != B.cols()) return false;
    for(int i=0;i<A.rows();i++)
        for(int j=0;j<A.cols();j++)
            if(std::fabs(static_cast<double>(A.get(i,j)) - static_cast<double>(B.get(i,j))) > eps)
                return false;
    return true;
}

/**
 * @brief Generates random matrix.
 */
template<typename T>
Matrix<T> randomMatrix(int r, int c, T minVal=-1000, T maxVal=1000) {
    Matrix<T> M(r,c);
    for(int i=0;i<r;i++)
        for(int j=0;j<c;j++)
            M[i][j] = minVal + std::rand() % (maxVal - minVal + 1);
    return M;
}

// ADDITION TESTS

/**
 * @brief Tests for matrix addition.
 */
TEST_CASE("MatrixOperations add", "[Addition]") {
    SECTION("Normal 2x2") {
        Matrix<long long> A(2,2), B(2,2), expected(2,2);
        A[0][0]=1; A[0][1]=2; A[1][0]=3; A[1][1]=4;
        B[0][0]=5; B[0][1]=6; B[1][0]=7; B[1][1]=8;
        expected[0][0]=6; expected[0][1]=8; expected[1][0]=10; expected[1][1]=12;
        auto res = MatrixOperations<long long>::add(A,B);
        REQUIRE(compareMatrix(res,expected));
    }

    SECTION("Zeros 5x5") {
        Matrix<long long> A(5,5), B(5,5);
        auto res = MatrixOperations<long long>::add(A,B);
        REQUIRE(compareMatrix(res,Matrix<long long>(5,5)));
    }

    SECTION("Negative numbers") {
        Matrix<long long> A(2,2), B(2,2), expected(2,2);
        A[0][0]=-1; A[0][1]=-2; A[1][0]=-3; A[1][1]=-4;
        B[0][0]=-5; B[0][1]=-6; B[1][0]=-7; B[1][1]=-8;
        expected[0][0]=-6; expected[0][1]=-8; expected[1][0]=-10; expected[1][1]=-12;
        auto res = MatrixOperations<long long>::add(A,B);
        REQUIRE(compareMatrix(res,expected));
    }

    SECTION("Single element 1x1") {
        Matrix<long long> A(1,1), B(1,1), expected(1,1);
        A[0][0]=7; B[0][0]=3; expected[0][0]=10;
        auto res = MatrixOperations<long long>::add(A,B);
        REQUIRE(compareMatrix(res,expected));
    }

    SECTION("Large 128x128") {
        Matrix<long long> A = randomMatrix<long long>(128,128,1,1000);
        Matrix<long long> B = randomMatrix<long long>(128,128,1,1000);
        auto res = MatrixOperations<long long>::add(A,B);
        REQUIRE(res.rows()==128); REQUIRE(res.cols()==128);
    }

    SECTION("Overflow check") {
        Matrix<long long> A(1,1), B(1,1), expected(1,1);
        A[0][0]=std::numeric_limits<long long>::max();
        B[0][0]=1;
        auto res = MatrixOperations<long long>::add(A,B);
        REQUIRE(res[0][0] < 0); // overflow behavior
    }
}

// SUBTRACTION TESTS

/**
 * @brief Tests for matrix subtraction.
 */
TEST_CASE("MatrixOperations subtract", "[Subtraction]") {
    SECTION("Normal 2x2") {
        Matrix<long long> A(2,2), B(2,2), expected(2,2);
        A[0][0]=9; A[0][1]=8; A[1][0]=7; A[1][1]=6;
        B[0][0]=1; B[0][1]=2; B[1][0]=3; B[1][1]=4;
        expected[0][0]=8; expected[0][1]=6; expected[1][0]=4; expected[1][1]=2;
        auto res = MatrixOperations<long long>::subtract(A,B);
        REQUIRE(compareMatrix(res,expected));
    }

    SECTION("Subtract itself") {
        Matrix<long long> A = randomMatrix<long long>(5,5);
        auto res = MatrixOperations<long long>::subtract(A,A);
        REQUIRE(compareMatrix(res,Matrix<long long>(5,5))); // zero matrix
    }

    SECTION("Underflow check") {
        Matrix<long long> A(1,1), B(1,1);
        A[0][0]=std::numeric_limits<long long>::min(); B[0][0]=1;
        auto res = MatrixOperations<long long>::subtract(A,B);
        REQUIRE(res[0][0] > 0); // underflow
    }
}

// SCALAR MULTIPLICATION TESTS

/**
 * @brief Tests for scalar multiplication.
 */
TEST_CASE("MatrixOperations scalarMultiply", "[Scalar]") {
    Matrix<long long> A(2,2);
    A[0][0]=1; A[0][1]=2; A[1][0]=3; A[1][1]=4;

    SECTION("Multiply by 0") {
        auto res = MatrixOperations<long long>::scalarMultiply(A,0);
        REQUIRE(compareMatrix(res,Matrix<long long>(2,2)));
    }

    SECTION("Multiply by -1") {
        Matrix<long long> expected(2,2);
        expected[0][0]=-1; expected[0][1]=-2; expected[1][0]=-3; expected[1][1]=-4;
        auto res = MatrixOperations<long long>::scalarMultiply(A,-1);
        REQUIRE(compareMatrix(res,expected));
    }

    SECTION("Large scalar") {
        Matrix<long long> expected(2,2);
        expected[0][0]=1000000; expected[0][1]=2000000; expected[1][0]=3000000; expected[1][1]=4000000;
        auto res = MatrixOperations<long long>::scalarMultiply(A,1000000);
        REQUIRE(compareMatrix(res,expected));
    }

    SECTION("Overflow check") {
        Matrix<long long> B(1,1); B[0][0]=std::numeric_limits<long long>::max();
        auto res = MatrixOperations<long long>::scalarMultiply(B,2);
        REQUIRE(res[0][0] < 0); // overflow behavior
    }
}

// TRANSPOSE TESTS

/**
 * @brief Tests transpose operation.
 */
TEST_CASE("Matrix transpose", "[Transpose]") {
    SECTION("2x3 matrix") {
        Matrix<long long> A(2,3), expected(3,2);
        A[0][0]=1; A[0][1]=2; A[0][2]=3;
        A[1][0]=4; A[1][1]=5; A[1][2]=6;
        expected[0][0]=1; expected[0][1]=4;
        expected[1][0]=2; expected[1][1]=5;
        expected[2][0]=3; expected[2][1]=6;
        auto res = A.transpose();
        REQUIRE(compareMatrix(res,expected));
    }

    SECTION("Square 5x5") {
        Matrix<long long> A = randomMatrix<long long>(5,5);
        auto res = A.transpose();
        REQUIRE(res.rows()==5); REQUIRE(res.cols()==5);
    }

    SECTION("Empty matrix 0x0") {
        Matrix<long long> A;
        auto res = A.transpose();
        REQUIRE(res.rows()==0); REQUIRE(res.cols()==0);
    }
}

// CLASSICAL MULTIPLICATION

/**
 * @brief Tests classical multiplication.
 */
TEST_CASE("ClassicalMultiplier multiply", "[Multiply]") {
    ClassicalMultiplier<long long> mul;

    SECTION("2x3 * 3x2") {
        Matrix<long long> A(2,3), B(3,2), expected(2,2);
        A[0][0]=1; A[0][1]=2; A[0][2]=3;
        A[1][0]=4; A[1][1]=5; A[1][2]=6;
        B[0][0]=7; B[0][1]=8;
        B[1][0]=9; B[1][1]=10;
        B[2][0]=11; B[2][1]=12;
        expected[0][0]=58; expected[0][1]=64; expected[1][0]=139; expected[1][1]=154;
        auto res = mul.multiply(A,B);
        REQUIRE(compareMatrix(res,expected));
    }

    SECTION("1x1 multiplication") {
        Matrix<long long> A(1,1), B(1,1), expected(1,1);
        A[0][0]=2; B[0][0]=3; expected[0][0]=6;
        auto res = mul.multiply(A,B);
        REQUIRE(compareMatrix(res,expected));
    }
}

// STRASSEN MULTIPLIER

/**
 * @brief Tests Strassen multiplication.
 */
TEST_CASE("StrassenMultiplier multiply", "[Strassen]") {
    StrassenMultiplier<long long> strassen;
    ClassicalMultiplier<long long> classical;

    SECTION("Small matrix fallback") {
        Matrix<long long> A(2,2), B(2,2);
        A[0][0]=1; A[0][1]=2; A[1][0]=3; A[1][1]=4;
        B[0][0]=5; B[0][1]=6; B[1][0]=7; B[1][1]=8;
        auto res1 = strassen.multiply(A,B);
        auto res2 = classical.multiply(A,B);
        REQUIRE(compareMatrix(res1,res2));
    }

    SECTION("Large 128x128 random") {
        Matrix<long long> A = randomMatrix<long long>(128,128,0,10);
        Matrix<long long> B = randomMatrix<long long>(128,128,0,10);
        auto res = strassen.multiply(A,B);
        REQUIRE(res.rows()==128); REQUIRE(res.cols()==128);
    }
}

// EDGE CASES

/**
 * @brief Tests system stability for edge cases.
 */
TEST_CASE("Edge cases", "[Edge]") {
    SECTION("Empty matrices") {
        Matrix<long long> A(0,0), B(0,0);
        auto res = MatrixOperations<long long>::add(A,B);
        REQUIRE(res.rows()==0); REQUIRE(res.cols()==0);
    }

    SECTION("Single element 1x1") {
        Matrix<long long> A(1,1), B(1,1);
        A[0][0]=5; B[0][0]=10;
        auto addRes = MatrixOperations<long long>::add(A,B);
        REQUIRE(addRes[0][0]==15);
    }
}