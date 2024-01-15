#include "MatrixMultSubJob.h"
#include "Matrix.h"

namespace matrix
{
   MatrixMultSubJob::MatrixMultSubJob(const Matrix& lhs, const Matrix& rhs, size_t row) :
      _lhs(lhs),
      _rhs(rhs),
      _rowIndex(row)
   {
   }

   MatrixMultSubJob::~MatrixMultSubJob() = default;

   std::vector<std::complex<double>> MatrixMultSubJob::operator()() const
   {
      std::vector <std::complex<double>> answer(_rhs.getNumColumns(), 0);
      for (uint32_t columnIndex = 0; columnIndex < _rhs.getNumColumns(); ++columnIndex)
      {
         for (uint32_t index = 0; index < _rhs.getNumRows(); ++index)
         {
            answer[columnIndex] += _lhs.getNumber(_rowIndex, index) * _rhs.getNumber(index, columnIndex);
         }
      }
      return answer;
   }
}