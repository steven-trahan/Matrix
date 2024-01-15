#include "MatrixUtility.h"

namespace matrix
{
   std::vector<std::vector<std::complex<double>>> generateMatrixData(size_t rows, size_t columns)
   {
      std::vector<std::complex<double>> copyVector(columns, std::complex<double>{ 0.0 });
      std::vector<std::vector<std::complex<double>>> returnValue(rows, copyVector);

      return returnValue;
   }
}