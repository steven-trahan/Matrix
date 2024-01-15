#pragma once

#include <stdint.h>
#include <vector>
#include <complex>

namespace matrix
{
class Matrix;

class MatrixMultSubJob
{
public:
   MatrixMultSubJob(const Matrix& lhs, const Matrix& rhs, size_t row);
   ~MatrixMultSubJob();
   std::vector<std::complex<double>> operator()() const;

private:
   const Matrix& _lhs;
   const Matrix& _rhs;
   size_t _rowIndex;
};
}