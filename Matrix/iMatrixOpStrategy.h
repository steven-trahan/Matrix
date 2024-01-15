#pragma once
#include <complex>

namespace matrix
{
   class Matrix;

   class iMatrixOpStrategy
   {
   public:
      virtual ~iMatrixOpStrategy() = default;
      virtual Matrix matrixMult(const Matrix& lhs, const Matrix& rhs) = 0;
      virtual Matrix scalarMult(const Matrix& lhs, const std::complex<double>& rhs) = 0;
      virtual Matrix matrixAdd(const Matrix& lhs, const Matrix& rhs) = 0;
      virtual Matrix matrixSub(const Matrix& lhs, const Matrix& rhs) = 0;
      virtual Matrix matrixTranspose(const Matrix& lhs) = 0;
      virtual std::complex<double> matrixDeterminant(const Matrix& lhs) = 0;
      virtual Matrix matrixMinor(const Matrix& lhs) = 0;
      virtual Matrix matrixCofactor(const Matrix& lhs) = 0;
      virtual Matrix matrixAdjoint(const Matrix& lhs) = 0;
      virtual Matrix matrixInverse(const Matrix& lhs) = 0;
   };
}