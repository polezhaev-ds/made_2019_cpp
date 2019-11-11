//
// Created by admin2 on 05.11.2019.
//

#define CATCH_CONFIG_MAIN

#include <string>
#include <vector>
#include <iostream>
#include "catch2/catch.hpp"
#include "Matrix.h"

bool AreMatricesEqual(const Matrix& matrixA, const Matrix& matrixB) {
    if (matrixA.getRows() != matrixB.getRows() || matrixA.getColumns() != matrixB.getColumns() )
        return false;
    for (std::size_t i = 0; i < matrixA.getRows(); i++)
        for (std::size_t j = 0; j < matrixA.getColumns(); j++)
            if (matrixA[i][j] != matrixB[i][j])
                return false;
    return true;
}

bool IsMatrixEqualToVector(const Matrix& matrix, const std::vector<std::vector<int>>& vector2d) {
    if (matrix.getRows() != vector2d.size() || (matrix.getRows() != 0 && matrix.getColumns() != vector2d[0].size()) )
        return false;

    for (std::size_t i = 0; i < matrix.getRows(); i++)
        for (std::size_t j = 0; j < matrix.getColumns(); j++)
            if (matrix[i][j] != vector2d[i][j])
                return false;
    return true;
}

bool IsMatrixEqualToVectorUsingGetValueBy(const Matrix& matrix, const std::vector<std::vector<int>>& vector2d) {
    if (matrix.getRows() != vector2d.size() || (matrix.getRows() != 0 && matrix.getColumns() != vector2d[0].size()) )
        return false;

    for (std::size_t i = 0; i < matrix.getRows(); i++)
        for (std::size_t j = 0; j < matrix.getColumns(); j++)
            if (matrix.getValueBy(i, j) != vector2d[i][j])
                return false;
    return true;
}

TEST_CASE( "tests for constructors" ) {

    SECTION( "Constructor (vector of vectors) works correctly for empty matrix" ) {
        Matrix matrix((std::vector<std::vector<int>>){});

        CHECK( matrix.getRows() == 0 );
        CHECK( matrix.getColumns() == 0 );
    }

    SECTION( "Constructor (vector of vectors) works correctly for non-empty matrix" ) {
        std::vector<std::vector<int>> vector2d = {{1, 2, 3}, {4, 5, 6}};
        Matrix matrix(vector2d);

        CHECK( IsMatrixEqualToVector(matrix, vector2d) );
    }

    SECTION( "Constructor for zero-filled matrix of particular sizes works correctly for empty matrix" ) {
        Matrix matrix(0, 0);

        CHECK( matrix.getRows() == 0 );
        CHECK( matrix.getColumns() == 0 );
    }

    SECTION( "Constructor for zero-filled matrix of particular sizes works correctly for non-empty matrix" ) {
        Matrix matrix(2, 3);

        CHECK( matrix.getRows() == 2);
        CHECK( matrix.getColumns() == 3 );
        IsMatrixEqualToVector( matrix, {{0, 0, 0}, {0, 0, 0}} );
    }

    SECTION( "Copy constructor works correctly for empty matrix" ) {
        Matrix matrix(0, 0);
        Matrix matrix_copy = matrix;

        CHECK( matrix_copy.getRows() == 0 );
        CHECK( matrix_copy.getColumns() == 0 );
    }

    SECTION( "Copy constructor works correctly for non-empty matrix" ) {
        std::vector<std::vector<int>> vector2d = {{1, 2, 3}, {4, 5, 6}};
        Matrix matrix(vector2d);
        Matrix matrix_copy = matrix;

        CHECK( IsMatrixEqualToVector(matrix, vector2d) );
        CHECK( IsMatrixEqualToVector(matrix_copy, vector2d) );
    }

    SECTION( "Move constructor works correctly for empty matrix" ) {
        Matrix matrixA(0, 0);
        Matrix matrixB(0, 0);
        Matrix matrix_moved(std::move(matrixA));

        CHECK( matrix_moved == matrixB );
        CHECK( matrixA == matrixB );
    }

    SECTION( "Move constructor works correctly for non-empty matrix" ) {
        std::vector<std::vector<int>> vector2d = {{1, 2, 3}, {4, 5, 6}};
        Matrix matrix(vector2d);
        Matrix matrix_moved(std::move(matrix));

        CHECK( IsMatrixEqualToVector(matrix_moved, vector2d) );
        CHECK( AreMatricesEqual(matrix, Matrix(0, 0)) );
    }

}

TEST_CASE( "tests for getters" ) {

    SECTION( "Rows and columns getters work correctly for non-constant matrix" ) {
        Matrix matrix(3, 4);

        CHECK( matrix.getRows() == 3 );
        CHECK( matrix.getColumns() == 4 );
    }

    SECTION( "Rows and columns getters work correctly for constant matrix" ) {
        const Matrix matrix(3, 4);

        CHECK( matrix.getRows() == 3 );
        CHECK( matrix.getColumns() == 4 );
    }

}

TEST_CASE( "tests for getting values by indices " ) {

    SECTION( "getValueBy works correctly for valid and invalid indices of non-constant matrices" ) {
        std::vector<std::vector<int>> vector2d = {{1, 2, 3}, {4, 5, 6}};
        Matrix matrix(vector2d);

        CHECK( IsMatrixEqualToVectorUsingGetValueBy(matrix, vector2d) );

        CHECK_THROWS_AS(matrix.getValueBy(3, 0), std::out_of_range);
        CHECK_THROWS_AS(matrix.getValueBy(3, 2), std::out_of_range);
        CHECK_THROWS_AS(matrix.getValueBy(3, 2), std::out_of_range);
        CHECK_THROWS_AS(matrix.getValueBy(0, 3), std::out_of_range);
        CHECK_THROWS_AS(matrix.getValueBy(1, 3), std::out_of_range);
        CHECK_THROWS_AS(matrix.getValueBy(2, 3), std::out_of_range);
        CHECK_THROWS_AS(matrix.getValueBy(10, 30), std::out_of_range);
    }

    SECTION( "getValueBy works correctly for valid and invalid indices of constant matrices" ) {
        const std::vector<std::vector<int>> vector2d = {{1, 2, 3}, {4, 5, 6}};
        const Matrix matrix(vector2d);

        CHECK( IsMatrixEqualToVectorUsingGetValueBy(matrix, vector2d) );

        CHECK_THROWS_AS(matrix.getValueBy(3, 0), std::out_of_range);
        CHECK_THROWS_AS(matrix.getValueBy(3, 2), std::out_of_range);
        CHECK_THROWS_AS(matrix.getValueBy(3, 2), std::out_of_range);
        CHECK_THROWS_AS(matrix.getValueBy(0, 3), std::out_of_range);
        CHECK_THROWS_AS(matrix.getValueBy(1, 3), std::out_of_range);
        CHECK_THROWS_AS(matrix.getValueBy(2, 3), std::out_of_range);
        CHECK_THROWS_AS(matrix.getValueBy(10, 30), std::out_of_range);
    }

    SECTION( "Indexing operators work correctly for valid and invalid indices of non-constant matrices" ) {
        std::vector<std::vector<int>> vector2d = {{1, 2, 3}, {4, 5, 6}};
        Matrix matrix(vector2d);

        CHECK( IsMatrixEqualToVector(matrix, vector2d) );

        CHECK_THROWS_AS(matrix[3][0], std::out_of_range);
        CHECK_THROWS_AS(matrix[3][2], std::out_of_range);
        CHECK_THROWS_AS(matrix[3][2], std::out_of_range);
        CHECK_THROWS_AS(matrix[0][3], std::out_of_range);
        CHECK_THROWS_AS(matrix[1][3], std::out_of_range);
        CHECK_THROWS_AS(matrix[2][3], std::out_of_range);
        CHECK_THROWS_AS(matrix[10][30], std::out_of_range);
    }

    SECTION( "Indexing operator work correctly for valid and invalid indices of constant matrices" ) {
        std::vector<std::vector<int>> vector2d = {{1, 2, 3}, {4, 5, 6}};
        const Matrix matrix(vector2d);

        CHECK( IsMatrixEqualToVector(matrix, vector2d) );

        CHECK_THROWS_AS(matrix[3][0], std::out_of_range);
        CHECK_THROWS_AS(matrix[3][2], std::out_of_range);
        CHECK_THROWS_AS(matrix[3][2], std::out_of_range);
        CHECK_THROWS_AS(matrix[0][3], std::out_of_range);
        CHECK_THROWS_AS(matrix[1][3], std::out_of_range);
        CHECK_THROWS_AS(matrix[2][3], std::out_of_range);
        CHECK_THROWS_AS(matrix[10][30], std::out_of_range);
    }

}

TEST_CASE( "tests for comparison operators" ) {

    SECTION( "Equal operator works correctly for non-constant empty matrices" ) {
        Matrix matrixA(0, 0);
        Matrix matrixB(0, 0);

        CHECK( matrixA == matrixA );
        CHECK( matrixA == matrixB );
    }

    SECTION( "Equal operator works correctly for constant empty matrices" ) {
        const Matrix matrixA(0, 0);
        const Matrix matrixB(0, 0);

        CHECK( matrixA == matrixA );
        CHECK( matrixA == matrixB );
    }

    SECTION( "Equal operator works correctly for non-constant non-empty matrices" ) {
        Matrix matrixA({{1, 2, 3}, {4, 5, 6}});
        Matrix matrixB({{1, 2, 3}, {4, 5, 6}});
        Matrix matrixC({{1, 2, 3}, {4, 6, 5}});

        CHECK( matrixA == matrixA );
        CHECK( matrixA == matrixB );
        CHECK( !(matrixA == matrixC) );
    }

    SECTION( "Equal operator works correctly for constant non-empty matrices" ) {
        const Matrix matrixA({{1, 2, 3}, {4, 5, 6}});
        const Matrix matrixB({{1, 2, 3}, {4, 5, 6}});
        const Matrix matrixC({{1, 2, 3}, {4, 6, 5}});

        CHECK( matrixA == matrixA );
        CHECK( matrixA == matrixB );
        CHECK( !(matrixA == matrixC) );
    }

    SECTION( "Not equal operator works correctly for constant non-empty matrices" ) {
        const Matrix matrixA({{1, 2, 3}, {4, 5, 6}});
        const Matrix matrixB({{1, 2, 3}, {4, 5, 6}});
        const Matrix matrixC({{1, 2, 3}, {4, 6, 5}});

        CHECK( !(matrixA != matrixA) );
        CHECK( !(matrixA != matrixB) );
        CHECK( matrixA != matrixC );
    }

    SECTION( "Not equal operator works correctly for non-constant non-empty matrices" ) {
        Matrix matrixA({{1, 2, 3}, {4, 5, 6}});
        Matrix matrixB({{1, 2, 3}, {4, 5, 6}});
        Matrix matrixC({{1, 2, 3}, {4, 6, 5}});

        CHECK( !(matrixA != matrixA) );
        CHECK( !(matrixA != matrixB) );
        CHECK( matrixA != matrixC );
    }

}

TEST_CASE( "tests for assignment operators" ) {

    SECTION( "Standard assignment operator works correctly for non-constant matrices" ) {
        Matrix matrixA(0, 0);
        Matrix matrixB(0, 0);
        Matrix matrixC({{1, 2, 3}, {4, 6, 5}});
        Matrix matrixD({{1, 2, 3}, {4, 6, 5}});
        Matrix matrixE({{7, 8}, {9, 10}});
        Matrix matrixF({{7, 8}, {9, 10}});

        Matrix* pointerToMatrixA = &matrixA;
        Matrix* pointerToSelfAssignmentResult = &(matrixA = matrixA);
        REQUIRE( pointerToMatrixA == pointerToSelfAssignmentResult);
        Matrix* pointerToMatrixC = &matrixC;
        pointerToSelfAssignmentResult = &(matrixC = matrixC);
        REQUIRE( pointerToMatrixC == pointerToSelfAssignmentResult);

        matrixA = matrixB;
        REQUIRE( AreMatricesEqual(matrixA, matrixB) );
        matrixA = matrixC;
        REQUIRE( !AreMatricesEqual(matrixA, matrixB) );
        REQUIRE( AreMatricesEqual(matrixA, matrixC) );
        REQUIRE( AreMatricesEqual(matrixC, matrixD) );
        matrixA = matrixE;
        REQUIRE( !AreMatricesEqual(matrixA, matrixC) );
        REQUIRE( AreMatricesEqual(matrixA, matrixE) );
        REQUIRE( AreMatricesEqual(matrixE, matrixF) );
        REQUIRE( AreMatricesEqual(matrixC, matrixD) );
        matrixC = matrixE;
        REQUIRE( !AreMatricesEqual(matrixC, matrixD) );
        REQUIRE( AreMatricesEqual(matrixC, matrixE) );
        REQUIRE( AreMatricesEqual(matrixE, matrixF) );
        matrixE = matrixB;
        REQUIRE( !AreMatricesEqual(matrixE, matrixF) );
        REQUIRE( AreMatricesEqual(matrixE, matrixB) );
        REQUIRE( AreMatricesEqual(matrixB, Matrix(0, 0)) );
    }

    SECTION( "Standard assignment operator works correctly for constant matrices that are assigned" ) {
        Matrix matrixA(0, 0);
        const Matrix matrixB(0, 0);
        Matrix matrixC({{1, 2, 3}, {4, 6, 5}});
        const Matrix matrixD({{1, 2, 3}, {4, 6, 5}});
        Matrix matrixE({{7, 8}, {9, 10}});
        const Matrix matrixF({{7, 8}, {9, 10}});

        matrixA = matrixB;
        REQUIRE( AreMatricesEqual(matrixA, matrixB) );
        matrixA = matrixC;
        REQUIRE( !AreMatricesEqual(matrixA, matrixB) );
        REQUIRE( AreMatricesEqual(matrixA, matrixC) );
        REQUIRE( AreMatricesEqual(matrixC, matrixD) );
        matrixA = matrixE;
        REQUIRE( !AreMatricesEqual(matrixA, matrixC) );
        REQUIRE( AreMatricesEqual(matrixA, matrixE) );
        REQUIRE( AreMatricesEqual(matrixE, matrixF) );
        REQUIRE( AreMatricesEqual(matrixC, matrixD) );
        matrixC = matrixE;
        REQUIRE( !AreMatricesEqual(matrixC, matrixD) );
        REQUIRE( AreMatricesEqual(matrixC, matrixE) );
        REQUIRE( AreMatricesEqual(matrixE, matrixF) );
        matrixE = matrixB;
        REQUIRE( !AreMatricesEqual(matrixE, matrixF) );
        REQUIRE( AreMatricesEqual(matrixE, matrixB) );
        REQUIRE( AreMatricesEqual(matrixB, Matrix(0, 0)) );
    }

    SECTION( "Move assignment operator works correctly" ) {
        Matrix matrixA(0, 0);
        Matrix matrixB(0, 0);
        Matrix matrixC({{1, 2, 3}, {4, 6, 5}});
        Matrix matrixD({{1, 2, 3}, {4, 6, 5}});

        matrixA = std::move(matrixC);
        CHECK( !AreMatricesEqual(matrixA, matrixB) );
        CHECK( AreMatricesEqual(matrixA, matrixD) );
        CHECK( AreMatricesEqual(matrixC, matrixB) );
    }

}

TEST_CASE( "tests for multiplication by scalar operators" ) {

    SECTION( "Multiplication by scalar operator works correctly for non-constant empty matrices" ) {
        Matrix matrixA(0, 0);
        Matrix matrixB(0, 0);
        Matrix matrixC = matrixA * 10;
        CHECK( AreMatricesEqual(matrixC, matrixB) );
        CHECK( AreMatricesEqual(matrixA, matrixB) );
        Matrix matrixD = 10 * matrixA;
        CHECK( AreMatricesEqual(matrixD, matrixB) );
        CHECK( AreMatricesEqual(matrixA, matrixB) );
    }

    SECTION( "Multiplication by scalar operator works correctly for constant empty matrices" ) {
        const Matrix matrixA(0, 0);
        const Matrix matrixB(0, 0);
        const Matrix matrixC = matrixA * 10;
        CHECK( AreMatricesEqual(matrixC, matrixB) );
        CHECK( AreMatricesEqual(matrixA, matrixB) );
        const Matrix matrixD = 10 * matrixA;
        CHECK( AreMatricesEqual(matrixD, matrixB) );
        CHECK( AreMatricesEqual(matrixA, matrixB) );
    }

    SECTION( "Multiplication by scalar operator works correctly for non-constant non-empty matrices" ) {
        Matrix matrixA({{1, 2, 3}, {4, 6, 5}});
        Matrix matrixB({{1, 2, 3}, {4, 6, 5}});
        Matrix matrixC({{10, 20, 30}, {40, 60, 50}});
        Matrix matrixD = matrixA * 10;
        CHECK( AreMatricesEqual(matrixD, matrixC) );
        CHECK( AreMatricesEqual(matrixA, matrixB) );
        Matrix matrixE = 10 * matrixA;
        CHECK( AreMatricesEqual(matrixE, matrixC) );
        CHECK( AreMatricesEqual(matrixA, matrixB) );
        CHECK( matrixA * 10 * 6 == matrixC * 6 );
        CHECK( matrixA * 10 * 6 == 6 * matrixC );
        CHECK( 10 * matrixA * 6 == 6 * matrixC );
        CHECK( 6 * matrixA * 10 == 6 * matrixC );
        CHECK( 6 * 10 * matrixA == 6 * matrixC );
    }

    SECTION( "Multiplication by scalar operator works correctly for constant non-empty matrices" ) {
        const Matrix matrixA({{1, 2, 3}, {4, 6, 5}});
        const Matrix matrixB({{1, 2, 3}, {4, 6, 5}});
        const Matrix matrixC({{10, 20, 30}, {40, 60, 50}});
        Matrix matrixD = matrixA * 10;
        CHECK( AreMatricesEqual(matrixD, matrixC) );
        CHECK( AreMatricesEqual(matrixA, matrixB) );
        Matrix matrixE = 10 * matrixA;
        CHECK( AreMatricesEqual(matrixE, matrixC) );
        CHECK( AreMatricesEqual(matrixA, matrixB) );
        CHECK( matrixA * 10 * 6 == matrixC * 6 );
        CHECK( matrixA * 10 * 6 == 6 * matrixC );
        CHECK( 10 * matrixA * 6 == 6 * matrixC );
        CHECK( 6 * matrixA * 10 == 6 * matrixC );
        CHECK( 6 * 10 * matrixA == 6 * matrixC );
    }

}

TEST_CASE( "tests for multiplication by scalar with assignment operators" ) {

    SECTION( "Multiplication by scalar with assignment operator works correctly for empty matrices" ) {
        Matrix matrixA(0, 0);
        Matrix matrixB(0, 0);
        Matrix& result = (matrixA *= 10);
        CHECK( AreMatricesEqual(matrixA, matrixB) );
        CHECK( &result == &matrixA );
    }

    SECTION( "Multiplication by scalar with assignment operator works correctly for non-empty matrices" ) {
        Matrix matrixA({{1, 2, 3}, {4, 6, 5}});
        Matrix matrixB({{10, 20, 30}, {40, 60, 50}});
        Matrix& result = (matrixA *= 10);
        CHECK( AreMatricesEqual(matrixA, matrixB) );
        CHECK( &result == &matrixA );
    }

}