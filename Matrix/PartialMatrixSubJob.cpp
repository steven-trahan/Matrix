#include "PartialMatrixSubJob.h"
#include "Matrix.h"

namespace matrix
{
PartialMatrixSubJob::PartialMatrixSubJob(const Matrix& matrix, size_t discludedRow, size_t discludedColumn)
   : _matrix(matrix),
   _discludedRow(discludedRow),
   _discludedColumn(discludedColumn)
{
}

PartialMatrixSubJob::~PartialMatrixSubJob() = default;

Matrix PartialMatrixSubJob::operator()() const
{
   Matrix returnValue(_matrix.getNumRows() - 1, _matrix.getNumColumns() - 1);

   for (uint32_t rowIndex = 0; rowIndex < _matrix.getNumRows(); ++rowIndex)
   {
      if (rowIndex == _discludedRow)
      {
         continue;
      }

      for (uint32_t columnIndex = 0; columnIndex < _matrix.getNumColumns(); ++columnIndex)
      {
         if (columnIndex == _discludedColumn)
         {
            continue;
         }

         returnValue.setNumber(rowIndex < _discludedRow ? rowIndex : rowIndex - 1,
            columnIndex < _discludedColumn ? columnIndex : columnIndex - 1,
            _matrix.getNumber(rowIndex, columnIndex));
      }
   }

   return returnValue;
}
}