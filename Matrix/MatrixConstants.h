#pragma once

#include <exception>

namespace matrix
{
   enum class OperationType
   {
      Serial,
      Parallel
   };

   class WrongMatrixSize : public std::exception {};
   class UnknownOperationType : public std::exception {};
   class UnsupportedMatrixType : public std::exception {};
}