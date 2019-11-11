//
// Created by admin2 on 08.11.2019.
//

#ifndef HW4_MATRIX_H
#define HW4_MATRIX_H

#include <cstdlib>
#include <stdexcept>
#include <cstring>
#include <vector>
#include <iostream>

class Matrix {
public:

    class MatrixRowProxy {

    public:

        const int& operator [] (std::size_t column) const {
            return matrix.getValueBy(row, column);
        }

        int& operator [] (std::size_t column) {
            return matrix.getValueBy(row, column);
        }

    private:
        Matrix& matrix;
        std::size_t row;

        explicit MatrixRowProxy(Matrix& matrix, std::size_t row):
                matrix(matrix),
                row(row)
        {
        }

        explicit MatrixRowProxy(const Matrix& matrix, std::size_t row):
                MatrixRowProxy(const_cast<Matrix&>(matrix), row)
        {
        }

        MatrixRowProxy(const MatrixRowProxy& matrixRow) = default;
        MatrixRowProxy(MatrixRowProxy&& matrixRow) noexcept = default;

        friend Matrix;

    };

    explicit Matrix(std::vector<std::vector<int>> matrix);

    explicit Matrix(std::size_t rows = 0, std::size_t columns = 0):
        rows(rows),
        columns(columns),
        values(nullptr)
    {
        AllocateMemory();
    }

    Matrix(const Matrix& matrix):
        rows(matrix.rows),
        columns(matrix.columns),
        values(nullptr)
    {
        AllocateMemory();
        CopyFrom(matrix);
    }

    Matrix(Matrix&& matrix) noexcept:
        rows(matrix.rows),
        columns(matrix.columns),
        values(matrix.values)
    {
        matrix.Reset();
    }

    virtual ~Matrix() {
        FreeMemory();
    }

    [[nodiscard]] std::size_t getRows() const {
        return rows;
    }

    [[nodiscard]] std::size_t getColumns() const {
        return columns;
    }

    [[nodiscard]] const int& getValueBy(std::size_t row, std::size_t column) const;

    int& getValueBy(std::size_t row, std::size_t column);

    MatrixRowProxy operator [] (std::size_t row) {
        return MatrixRowProxy(*this, row);
    }

    MatrixRowProxy operator [] (std::size_t row) const {
        return MatrixRowProxy(*this, row);
    }

    bool operator == (const Matrix& matrix) const;

    bool operator != (const Matrix& matrix) const;

    Matrix& operator = (const Matrix& matrix);

    Matrix& operator = (Matrix&& matrix) noexcept;

    Matrix operator * (int multiplier) const;

    Matrix& operator *= (int multiplier);

private:
    size_t rows;
    size_t columns;
    int** values;

    void AllocateMemory();

    void CopyFrom(const Matrix& matrix);

    void FreeMemory();

    void Reset();

};

Matrix operator * (int multiplier, const Matrix& b);


#endif //HW4_MATRIX_H
