//
// Created by admin2 on 08.11.2019.
//

#include "Matrix.h"

Matrix::Matrix(const std::vector<std::vector<int>>& vector2d) :
        rows(vector2d.size()),
        columns(0),
        values(nullptr)
{
    if (!vector2d.empty())
        columns = vector2d[0].size();
    AllocateMemory();
    CopyFrom(vector2d);
}

const int& Matrix::getValueBy(std::size_t row, std::size_t column) const {
    if (row >= rows || column >= columns)
        throw std::out_of_range("Error! Indices (" + std::to_string(row) + ", " + std::to_string(column) +
                                ") are out of range");
    return values[row][column];
}

int &Matrix::getValueBy(std::size_t row, std::size_t column) {
    if (row >= rows || column >= columns)
        throw std::out_of_range("Error! Indices (" + std::to_string(row) + ", " + std::to_string(column) +
                                ") are out of range");
    return values[row][column];
}

bool Matrix::operator == (const Matrix& matrix) const {
    if (rows != matrix.rows || columns != matrix.columns)
        return false;
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < columns; j++)
            if (values[i][j] != matrix.values[i][j])
                return false;
    return true;
}

bool Matrix::operator != (const Matrix& matrix) const {
    return !(*this == matrix);
}

Matrix &Matrix::operator = (const Matrix& matrix) {
    if (this == &matrix)
        return *this;
    FreeMemory();
    rows = matrix.rows;
    columns = matrix.columns;
    AllocateMemory();
    CopyFrom(matrix.values);
    return *this;
}

Matrix &Matrix::operator = (Matrix&& matrix) noexcept {
    if (this == &matrix)
        return *this;
    FreeMemory();
    rows = matrix.rows;
    columns = matrix.columns;
    values = matrix.values;
    matrix.Reset();
    return *this;
}

Matrix Matrix::operator * (int multiplier) const {
    Matrix result = *this;
    result *= multiplier;
    return result;
}

Matrix &Matrix::operator *= (int multiplier) {
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < columns; j++)
            values[i][j] = values[i][j] * multiplier;
    return *this;
}

void Matrix::AllocateMemory() {
    if (rows > 0 && columns > 0) {
        values = new int *[rows];
        for (std::size_t i = 0; i < rows; i++) {
            values[i] = new int [columns]();
        }
    }
}

template <typename T>
void Matrix::CopyFrom(const T& data) {
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < columns; j++)
            values[i][j] = data[i][j];
}

void Matrix::FreeMemory() {
    if (values != nullptr) {
        for (std::size_t i = 0; i < rows; i++)
            delete[] values[i];
        delete[] values;
        values = nullptr;
    }
}

void Matrix::Reset() {
    rows = columns = 0;
    values = nullptr;
}

Matrix operator * (int multiplier, const Matrix &b) {
    return b * multiplier;
}
