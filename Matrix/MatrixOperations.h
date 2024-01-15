#pragma once

#include "MatrixConstants.h"
#include <complex>
#include <vector>

namespace matrix
{
   class Matrix;
   class iMatrixOpStrategy;

   class MatrixOperations
   {
   public:
      static iMatrixOpStrategy& getMatrixOpStrategy();
      static iMatrixOpStrategy& setMatrixOpStrategy(OperationType operationType);
      ~MatrixOperations();

   private:
      MatrixOperations() = default;
   };
}