#pragma once

#include "MatrixConstants.h"
#include <memory>
#include <vector>
#include <complex>

namespace matrix
{

class iMatrixOpStrategy;

class Matrix
{
public:
   Matrix();

   Matrix(size_t rows, size_t columns);

   Matrix(size_t rows, size_t columns, const std::vector<std::vector<std::complex<double>>>& data);

   ~Matrix();

   Matrix(const Matrix& rhs);

   Matrix operator=(const Matrix& rhs);

   Matrix operator*(const Matrix& rhs);

   Matrix operator*(std::complex<double> rhs) const;

   Matrix operator+(const Matrix& rhs);
   
   Matrix operator-(const Matrix& rhs);

   Matrix transpose();

   std::complex<double> determinant();

   Matrix minor();

   Matrix cofactor();

   Matrix adjoint();

   Matrix inverse();

   std::complex<double> getNumber(size_t row, size_t column) const;

   std::vector<std::complex<double>> getAllNumbers() const;

   void setNumber(size_t row, size_t column, std::complex<double> value);

   void setRow(size_t row, const std::vector<std::complex<double>>& value);

   size_t getNumRows() const;

   size_t getNumColumns() const;

private:
   size_t _rows;
   size_t _columns;

   std::vector<std::vector<std::complex<double>>> _data;
};
}