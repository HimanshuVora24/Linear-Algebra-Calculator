#ifndef MATRIX_H
#define MATRIX_H

#include <vector> 
#include <stdexcept> 

class Matrix {

private: 
    
    std::vector<std::vector<double>> mat; 
    int m; //rows 
    int n; //columns

    
public: 
    
    Matrix();
    Matrix(int rows, int cols);
    Matrix(const Matrix &matrix);
    void addEntry(int row, int col, double entry); 
    double getEntry(int row, int col);
    std::vector<double> getRowVector(int row);
    std::vector<double> getColVector(int col);
    void changeRow(int row, std::vector<double> vec);
    void changeCol(int col, std::vector<double> vec);
    void printMatrix();
    void rowScale(int row, double scale);
    void rowSwitch(int row_1, int row_2);
    void rowAdd(int row_target, int row_source, double scale);
    Matrix getTranspose();
    Matrix multiply(Matrix * matrix);
    double dot(std::vector<double> vec1, std::vector<double> vec2);
    int sortByPivot(int start_row);
    Matrix echelon();
    Matrix reducedEchelon();
    Matrix inverse();

    std::vector<double> vectorMultiply(std::vector<double> vec);
    bool linearIndependence();
    bool spanning();
    double determinant();
    
};

#endif