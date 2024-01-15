#include "MatrixAddSubJob.h"

#include "Matrix.h"

namespace matrix
{
   MatrixAddSubJob::MatrixAddSubJob(const Matrix& lhs, const Matrix& rhs, size_t row) :
      _lhs(lhs),
      _rhs(rhs),
      _row(row)
   {
   }

   MatrixAddSubJob::~MatrixAddSubJob() = default;

   std::vector<std::complex<double>> MatrixAddSubJob::operator()() const
   {
      std::vector<std::complex<double>> result;
      result.reserve(_lhs.getNumColumns());

      for (size_t col = 0; col < _lhs.getNumColumns(); ++col)
      {
         result.push_back(_lhs.getNumber(_row, col) + _rhs.getNumber(_row, col));
      }

      return result;
   }
}