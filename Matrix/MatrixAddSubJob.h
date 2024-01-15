#pragma once

#include <vector>
#include <complex>

namespace matrix
{
class Matrix;
class MatrixAddSubJob
{
public:
   MatrixAddSubJob(const Matrix& lhs, const Matrix& rhs, size_t row);
   ~MatrixAddSubJob();

   std::vector<std::complex<double>> operator()() const;

private:
   const Matrix& _lhs;
   const Matrix& _rhs;
   size_t _row;
};
}