#pragma once
#include "iMatrixOpStrategy.h"

namespace matrix
{
class Matrix;

class SerialMatrixOpStrategy :
   public iMatrixOpStrategy
{
public:
   SerialMatrixOpStrategy();
   ~SerialMatrixOpStrategy() final;

   Matrix matrixMult(const Matrix& lhs, const Matrix& rhs) final;

   Matrix scalarMult(const Matrix& lhs, const std::complex<double>& rhs) final;

   Matrix matrixAdd(const Matrix& lhs, const Matrix& rhs) final;

   Matrix matrixSub(const Matrix& lhs, const Matrix& rhs) final;

   Matrix matrixTranspose(const Matrix& lhs) final;

   std::complex<double> matrixDeterminant(const Matrix& lhs) final;

   Matrix matrixMinor(const Matrix& lhs) final;

   Matrix matrixCofactor(const Matrix& lhs) final;

   Matrix matrixAdjoint(const Matrix& lhs) final;

   Matrix matrixInverse(const Matrix& lhs) final;

private:
   Matrix _matrixExcludedRowAndColumn(const Matrix& lhs, uint32_t excludedRowIndex, uint32_t excludedColumnIndex);
};
}
