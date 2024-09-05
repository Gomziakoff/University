#pragma once
#include <iostream>
#include <vector>

void printMatrix(const std::vector<std::vector<double>>& matrix) {
    for (const auto& row : matrix) {
        for (double element : row) {
            std::cout << element << "\t";
        }
        std::cout << std::endl;
    }
}

// Функция для нахождения минора матрицы
std::vector<std::vector<double>> getMinor(const std::vector<std::vector<double>>& matrix, int row, int col) {
    std::vector<std::vector<double>> minor;
    for (size_t i = 0; i < matrix.size(); ++i) {
        if (i != row) {
            std::vector<double> temp_row;
            for (size_t j = 0; j < matrix[i].size(); ++j) {
                if (j != col) {
                    temp_row.push_back(matrix[i][j]);
                }
            }
            if (!temp_row.empty()) {
                minor.push_back(temp_row);
            }
        }
    }
    return minor;
}

// Функция для нахождения определителя матрицы
double determinant(const std::vector<std::vector<double>>& matrix) {
    if (matrix.size() != matrix[0].size()) {
        std::cerr << "Матрица не квадратная!" << std::endl;
        return 0.0;
    }

    if (matrix.size() == 1) {
        return matrix[0][0];
    }

    double det = 0.0;
    int sign = 1;

    for (size_t i = 0; i < matrix.size(); ++i) {
        det += sign * matrix[0][i] * determinant(getMinor(matrix, 0, i));
        sign *= -1;
    }

    return det;
}

 //Функция для нахождения обратной матрицы
std::vector<std::vector<double>> inverseMatrix(const std::vector<std::vector<double>>& matrix) {
    double det = determinant(matrix);
    if (det == 0.0) {
        std::cerr << "Определитель матрицы равен нулю, обратная матрица не существует!" << std::endl;
        return std::vector<std::vector<double>>();
    }

    int n = matrix.size();
    std::vector<std::vector<double>> inverse(n, std::vector<double>(n));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double minor_det = determinant(getMinor(matrix, i, j));
            inverse[j][i] = (i + j) % 2 == 0 ? minor_det / det : -minor_det / det;
        }
    }

    return inverse;
}