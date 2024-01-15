#include "Matrix.h"
#include "iMatrixOpStrategy.h"
#include "MatrixUtility.h"
#include "MatrixOperations.h"

namespace matrix
{
   Matrix::Matrix() :
      Matrix(0, 0, {})
   {}

   Matrix::Matrix(size_t rows, size_t columns) :
      Matrix(rows, columns, generateMatrixData(rows, columns))
   {}

   Matrix::Matrix(size_t rows, size_t columns, const std::vector<std::vector<std::complex<double>>>& data) :
      _rows(rows),
      _columns(columns),
      _data(data)
   {}

   Matrix::~Matrix() = default;

   Matrix::Matrix(const Matrix& rhs) :
      _rows(rhs._rows),
      _columns(rhs._columns),
      _data(rhs._data)
   {}

   Matrix Matrix::operator=(const Matrix& rhs)
   {
      _rows = rhs._rows;
      _columns = rhs._columns;
      _data = rhs._data;
      return *this;
   }

   Matrix Matrix::operator*(const Matrix& rhs)
   {
      return MatrixOperations::getMatrixOpStrategy().matrixMult(*this, rhs);
   }

   Matrix Matrix::operator*(std::complex<double> rhs) const
   {
      return MatrixOperations::getMatrixOpStrategy().scalarMult(*this, rhs);
   }

   Matrix Matrix::operator+(const Matrix& rhs)
   {
      return MatrixOperations::getMatrixOpStrategy().matrixAdd(*this, rhs);
   }

   Matrix Matrix::operator-(const Matrix& rhs)
   {
      return MatrixOperations::getMatrixOpStrategy().matrixSub(*this, rhs);
   }

   Matrix Matrix::transpose()
   {
      return MatrixOperations::getMatrixOpStrategy().matrixTranspose(*this);
   }

   std::complex<double> Matrix::determinant()
   {
      return MatrixOperations::getMatrixOpStrategy().matrixDeterminant(*this);
   }

   Matrix Matrix::minor()
   {
      return MatrixOperations::getMatrixOpStrategy().matrixMinor(*this);
   }

   Matrix Matrix::cofactor()
   {
      return MatrixOperations::getMatrixOpStrategy().matrixCofactor(*this);
   }

   Matrix Matrix::adjoint()
   {
      return MatrixOperations::getMatrixOpStrategy().matrixAdjoint(*this);
   }

   Matrix Matrix::inverse()
   {
      return MatrixOperations::getMatrixOpStrategy().matrixInverse(*this);
   }

   std::complex<double> Matrix::getNumber(size_t row, size_t column) const
   {
      return _data[row][column];
   }

   std::vector<std::complex<double>> Matrix::getAllNumbers() const
   {
      std::vector<std::complex<double>> allNumbers(_rows * _columns, 0.0);
      for (size_t row = 0; row < _rows; ++row)
      {
         for (size_t column = 0; column < _columns; ++column)
         {
            allNumbers[row * _columns + column] = _data[row][column];
         }
      }
      return allNumbers;
   }

   void Matrix::setNumber(size_t row, size_t column, std::complex<double> value)
   {
      _data[row][column] = value;
   }

   void Matrix::setRow(size_t row, const std::vector<std::complex<double>>& value)
   {
      _data[row] = value;
   }

   size_t Matrix::getNumRows() const
   {
      return _rows;
   }

   size_t Matrix::getNumColumns() const
   {
      return _columns;
   }
}