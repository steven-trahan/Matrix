#include "MatrixOperations.h"
#include "iMatrixOpStrategy.h"
#include "ParallelMatrixOpStrategy.h"
#include "SerialMatrixOpStrategy.h"

namespace matrix
{
   static std::unique_ptr<iMatrixOpStrategy> matrixOpStrategy = std::make_unique <SerialMatrixOpStrategy>();

   iMatrixOpStrategy& MatrixOperations::getMatrixOpStrategy()
   {
      return *matrixOpStrategy;
   }

   iMatrixOpStrategy& MatrixOperations::setMatrixOpStrategy(OperationType operationType)
   {
      switch (operationType)
      {
         case OperationType::Serial:
            matrixOpStrategy.reset(nullptr);
            matrixOpStrategy = std::make_unique <SerialMatrixOpStrategy>();
            break;
         case OperationType::Parallel:
            matrixOpStrategy.reset(nullptr);
            matrixOpStrategy = std::make_unique <ParallelMatrixOpStrategy>();
            break;
         default:
            throw UnknownOperationType();
      }

      return *matrixOpStrategy;
   }

   MatrixOperations::~MatrixOperations() = default;
}
