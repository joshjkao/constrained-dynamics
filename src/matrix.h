#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <iostream>

class Matrix {
    public:
        Matrix();
        Matrix(int rows, int cols);
        Matrix(std::vector< std::vector<double> >&);
        ~Matrix();

        // Matrix editing operations
        void addRow(std::vector<double>&);
        friend Matrix transpose(const Matrix&);

        // (Using form Ax = B) Member function of A that takes solution vector B as a parameter, returns x
        std::vector<double> gaussElim(const std::vector<double>&);

        // Necessary math operations
        friend std::vector<double> multiply(const Matrix&, const std::vector<double>&);
        friend std::vector<double> multiply(const Matrix&, const std::vector<double>&);
        friend Matrix multiply(const Matrix&, const Matrix&);
        friend Matrix multiply(const Matrix&, const double&);

        // Allows array-esque access
        std::vector<double>& operator[](int index);

        // Operators
        friend std::vector<double> operator*(const Matrix&, const std::vector<double>&);
        friend std::vector<double> operator*(const Matrix&, const std::vector<double*>&);
        friend Matrix operator*(const Matrix&, const Matrix&);
        friend Matrix operator*(const Matrix&, const double&);
        friend std::ostream& operator<<(std::ostream&, const Matrix&);

        // Data members
        std::vector< std::vector<double> > matrix;
        unsigned int rows = 0;
        unsigned int cols = 0;
};

#endif