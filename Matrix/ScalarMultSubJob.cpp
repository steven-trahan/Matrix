#include "ScalarMultSubJob.h"

namespace matrix
{
ScalarMultSubJob::ScalarMultSubJob(const Matrix& matrix, size_t row, std::complex<double> scalar)
   : _matrix(matrix)
   , _row(row)
   , _scalar(scalar)
{
}
ScalarMultSubJob::~ScalarMultSubJob() = default;

std::vector<std::complex<double>> ScalarMultSubJob::operator()() const
{
   std::vector<std::complex<double>> result(_matrix.getNumColumns());
   for (size_t i = 0; i < _matrix.getNumColumns(); ++i)
   {
      result[i] = _matrix.getNumber(_row, i) * _scalar;
   }
   return result;
}
}