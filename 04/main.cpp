#include <iostream>
#include "Matrix.h"
#include <cassert>
#include <vector>

int main() {

    //Example from assignment
    const size_t rows = 5;
    const size_t cols = 3;

    Matrix m(rows, cols);

    assert(m.getRows() == 5);
    assert(m.getColumns() == 3);

    m[1][2] = 5; // строка 1, колонка 2
    double x = m[4][1];

    m *= 3; // умножение на число

    Matrix m1(rows, cols);

    if (m1 == m) {
        std::cout << "Yes" << std::endl;
    } else {
        std::cout << "No" << std::endl;
    }


    //Example for a constant matrix
    const Matrix constant_matrix({{1, 2}, {3, 4}});

    const int value1 = constant_matrix[1][1];
    const int value2 = constant_matrix.getValueBy(1, 1);
    const int row_count = constant_matrix.getRows();
    const int column_count = constant_matrix.getColumns();
    Matrix constant_matrix2 = constant_matrix * 2 * 4;

    return 0;
}