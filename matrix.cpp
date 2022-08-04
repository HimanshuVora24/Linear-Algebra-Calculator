#include "matrix.h"
#include <iostream>
#include <iomanip>
using std::cout;
using std::endl; 

Matrix::Matrix() : mat(2, std::vector<double>(2, 0)) {
    m = 2;
    n = 2; 
}

Matrix::Matrix(int rows, int columns) : mat(rows, std::vector<double>(columns, 0)) {
    m = rows; 
    n = columns; 
}

Matrix::Matrix(const Matrix &matrix) : mat(matrix.m, std::vector<double>(matrix.n, 0)) {
    m = matrix.m;
    n = matrix.n;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            mat[i][j] = matrix.mat[i][j];
        }
    }
}

void Matrix::addEntry(int row, int col, double entry) {
    if ( row < 0 || row > m || col < 0 || col > n) {
        throw std::invalid_argument("Index out of matrix bounds");
    }

    mat[row][col] = entry; 
}

double Matrix::getEntry(int row, int col) {
    if (row < 0 || row > m || col < 0 || col > n) {
        throw std::invalid_argument("Index out of matrix bounds");
    }

    return mat[row][col];
}

std::vector<double> Matrix::getRowVector(int row) {
    if (row < 0 || row > m) {
        throw std::invalid_argument("Row out of matrix bounds");
    }

    return mat[row]; 
}

std::vector<double> Matrix::getColVector(int col) {
    if (col < 0 || col > n) {
        throw std::invalid_argument("Column out of matrix bounds");
    }

    std::vector<double> vec(m);
    for (int i = 0; i < m; i++) {
        vec[i] = mat[i][col];
    }

    return vec; 
}

void Matrix::changeRow(int row, std::vector<double> vec) {
    if (row < 0 || row > m) {
        throw std::invalid_argument("Row out of matrix bounds");
    } else if ((int)vec.size() != n) {
        throw std::invalid_argument("Row length does not match");
    }

    for (int i = 0; i < n; i++) { 
        mat[row][i] = vec[i];
    }
}

void Matrix::changeCol(int col, std::vector<double> vec) {
    if (col < 0 || col > n) {
        throw std::invalid_argument("Row out of matrix bounds");
    } else if ((int)vec.size() != m) {
        throw std::invalid_argument("Row length does not match");
    }

    for (int i = 0; i < m; i++) { 
        mat[i][col] = vec[i];
    }
}

void Matrix::printMatrix() {
    for (int i = 0; i < m; i++) {
        cout << "|";
        for (int j = 0; j < n; j++) {
            if (j != 0) cout << std::setfill(' ') << std::setw(3);
            cout << mat[i][j];
        }
        cout << "|" << endl; 
    }
    cout << endl;
}

void Matrix::rowScale(int row, double scale) {
    if (row < 0 || row > m) {
        throw std::invalid_argument("Row out of matrix bounds");
    }

    for (int i = 0; i < n; i++) {
        if (mat[row][i] != 0) mat[row][i] *= scale; 
    }
}

void Matrix::rowSwitch(int row_1, int row_2) {
    if (row_1 < 0 || row_1 > m) {
        throw std::invalid_argument("Row_1 out of matrix bounds");
    } else if (row_2 < 0 || row_2 > m) {
        throw std::invalid_argument("Row_2 out of matrix bounds");
    }

    /*std::vector<double> temp_row;
    temp_row = getRowVector(row_1);
    changeRow(row_1, getRowVector(row_2)); 
    changeRow(row_2, temp_row);*/
    for (int i = 0; i < n; i++) {
        double temp = mat[row_1][i];
        mat[row_1][i] = mat[row_2][i];
        mat[row_2][i] = temp; 
    }
}

void Matrix::rowAdd(int row_target, int row_source, double scale) {
    if (row_target < 0 || row_target > m) {
        throw std::invalid_argument("Row_target out of matrix bounds");
    } else if (row_source < 0 || row_source > m) {
        throw std::invalid_argument("Row_source out of matrix bounds");
    }

    for (int i = 0; i < n; i++) {
        mat[row_target][i] += (mat[row_source][i] * scale);
    }
}

Matrix Matrix::getTranspose() {
    Matrix transpose(n, m); 
    for (int i = 0; i < n; i++) {
        transpose.changeRow(i, this->getColVector(i));
    }
    return transpose; 
}

double Matrix::dot(std::vector<double> vec1, std::vector<double> vec2) {
    if (vec1.size() != vec2.size()) {
        throw std::invalid_argument("Vector sizes do not match");
    }

    double dot_product = 0;

    for (int i = 0; i < (int)vec1.size(); i++) {
        dot_product += (vec1[i] * vec2[i]);
    }

    return dot_product; 
}

Matrix Matrix::multiply(Matrix * matrix) {
    if (this->n != matrix->m) {
        throw std::invalid_argument("Matrix dimensions do not match");
    } 

    Matrix product(this->m, matrix->n);
    for (int i = 0; i < product.m; i++) {
        for (int j = 0; j < product.n; j++) {
            product.mat[m][n] = dot(this->getRowVector(i), matrix->getColVector(j));  
        }
    }

    return product; 
}

int Matrix::sortByPivot(int start_row) {
    int left_most = start_row;
    int given_pivot = 0;
    
    while (mat[start_row][given_pivot] == 0) given_pivot++;   
    
    for (int i = start_row+1; i < m; i++) {
        int pivot = 0;
        while (mat[i][pivot] == 0) pivot++; 
        if (pivot < given_pivot) {
            left_most = i; 
            given_pivot = pivot; 
        }
    }

    if (left_most != start_row) {
        rowSwitch(left_most, start_row);
    }

    return left_most; 
}

Matrix Matrix::reducedEchelon() {
    Matrix matrix(*this);
    for (int i = 0; i < m; i++) {
        matrix.sortByPivot(i);
        int pivot = 0; 
        while (matrix.mat[i][pivot] == 0) pivot++; 
        if (pivot >= n) continue; 
        matrix.rowScale(i, 1/matrix.mat[i][pivot]);
        for (int j = 0; j < n; j++) {
            if (i != j) matrix.rowAdd(j, i, matrix.mat[j][pivot] * -1);
        }
        matrix.printMatrix();
    }

    return matrix; 
}

Matrix Matrix::inverse() {
    if (m != n || !(this->linearIndependence())) {
        throw std::invalid_argument("Matrix does not have an inverse"); 
    }

    Matrix matrix(*this);
    Matrix iden(this->m, this->n);
    
    for (int i = 0; i < m; i++) iden.mat[i][i] = 1; //set identity matrix 

    for (int i = 0; i < m; i++) {
        iden.rowSwitch(i, matrix.sortByPivot(i));

        int pivot = 0; 
        while (matrix.mat[i][pivot] == 0) pivot++; 
        
        iden.rowScale(i, 1/matrix.mat[i][pivot]);
        matrix.rowScale(i, 1/matrix.mat[i][pivot]);
        
        for (int j = 0; j < n; j++) {
            if (i != j) {
                iden.rowAdd(j, i, matrix.mat[j][pivot] * -1);
                matrix.rowAdd(j, i, matrix.mat[j][pivot] * -1);
            }
        }
    }
    
    return iden; 
}

bool Matrix::linearIndependence() {
    if (n > m) return false; 
    Matrix matrix = this->reducedEchelon();
    for (int i = 0; i < n; i++) {
        bool non_zero_found = false; 
        for (int j = i; j < m; j++) {
            if (matrix.mat[j][i] != 0) {
                if (non_zero_found) {
                   return false; 
                } else {
                    non_zero_found = true; 
                } 
            }
        }
        if (!non_zero_found) return false; 
    }
    return true; 
}

bool Matrix::spanning() {
    if (m > n) return false; 
    Matrix matrix = this->reducedEchelon();
    for (int i = 0; i < m; i++) {
        bool non_zero_found = false; 
        for (int j = i; j < n; j++) {
            if (matrix.mat[i][j] != 0) {
                non_zero_found = true; 
                break; 
            }
        }
        if (!non_zero_found) return false; 
    }
    return true; 
}

std::vector<double> Matrix::vectorMultiply(std::vector<double> vec) {
    if ((int)vec.size() != n) {
        throw std::invalid_argument("Vector is of wrong dimension");
    } 

    std::vector<double> return_vec;
    for (int i = 0; i < m; i++) {
        return_vec.push_back(dot(getRowVector(i), vec));
    }

    return return_vec; 
}

double Matrix::determinant() {
    Matrix matrix(*this);
    double det = 1;
    for (int i = 0; i < m; i++) {
        if (matrix.sortByPivot(i) != i) det *= -1;
        int pivot = 0; 
        while (matrix.mat[i][pivot] == 0) pivot++;
        if (pivot >= n) return 0;
        det *= matrix.mat[i][pivot];
        if (det == 0) break; 
        matrix.rowScale(i, 1/matrix.mat[i][pivot]);
        for (int j = i + 1; j < n; j++) {
            matrix.rowAdd(j, i, matrix.mat[j][pivot] * -1);
        }
    }
    //matrix.printMatrix();
    return det;
}