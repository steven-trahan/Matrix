#pragma once

#include "Matrix.h"
#include <vector>

namespace matrix
{
   class ScalarMultSubJob
   {
   public:
      ScalarMultSubJob(const Matrix& matrix, size_t row, std::complex<double> scalar);
      ~ScalarMultSubJob();
   
      std::vector<std::complex<double>> operator()() const;

   private:
      const Matrix& _matrix;
      size_t _row;
      std::complex<double> _scalar;
   };
}