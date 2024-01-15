#pragma once

#include <complex>
#include <vector>

namespace matrix
{
class Matrix;

class TransposeSubJob
{
public:
   TransposeSubJob(const Matrix& matrix, int row);
   ~TransposeSubJob();

   std::vector<std::complex<double>> operator()() const;

private:
   const Matrix& _matrix;
   int _row;
};
}