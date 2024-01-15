#include "TransposeSubJob.h"
#include "Matrix.h"

namespace matrix
{
   TransposeSubJob::TransposeSubJob(const Matrix& matrix, int row) : _matrix(matrix), _row(row)
   {
   }

   TransposeSubJob::~TransposeSubJob() = default;

   std::vector<std::complex<double>> TransposeSubJob::operator()() const
   {
      std::vector<std::complex<double>> result(_matrix.getNumColumns());
      for (int i = 0; i < _matrix.getNumColumns(); ++i)
      {
         result[i] = _matrix.getNumber(i, _row);
      }
      return result;
   }
}