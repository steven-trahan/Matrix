#pragma once

#include <stdint.h>

namespace matrix
{
class Matrix;
class PartialMatrixSubJob
{
public:
   PartialMatrixSubJob(const Matrix& matrix, size_t discludedRow, size_t discludedColumn);

   ~PartialMatrixSubJob();

   Matrix operator()() const;

private:
   const Matrix& _matrix;
   size_t _discludedRow;
   size_t _discludedColumn;
};
}