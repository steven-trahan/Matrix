#include "ParallelMatrixOpStrategy.h"
#include "Matrix.h"
#include <ThreadPool.h>
#include "MatrixMultSubJob.h"
#include "ScalarMultSubJob.h"
#include "MatrixAddSubJob.h"
#include "TransposeSubJob.h"
#include "MatrixUtility.h"
#include "PartialMatrixSubJob.h"

namespace matrix
{
   ParallelMatrixOpStrategy::ParallelMatrixOpStrategy() :
      iMatrixOpStrategy(),
      _threadpool(threadpool::getThreadPoolInstance())
   {
      if (!_threadpool.isRunning())
      {
         _threadpool.Start();
      }
   }
   ParallelMatrixOpStrategy::~ParallelMatrixOpStrategy() = default;

   Matrix ParallelMatrixOpStrategy::matrixMult(const Matrix& lhs, const Matrix& rhs)
   {
      if (lhs.getNumColumns() != rhs.getNumRows())
      {
         throw matrix::WrongMatrixSize();
      }

      std::vector<std::future<std::vector<std::complex<double>>>> results;

      for (uint32_t rowIndex = 0; rowIndex < lhs.getNumRows(); ++rowIndex)
      {
         MatrixMultSubJob job(lhs, rhs, rowIndex);
         auto result = _threadpool.postJob<std::vector<std::complex<double>>>(std::move(job));
         results.push_back(std::move(result));
      }

      Matrix answer(lhs.getNumRows(), rhs.getNumColumns());
      for (uint32_t rowIndex = 0; rowIndex < rhs.getNumRows(); ++rowIndex)
      {
         auto rowData = results[rowIndex].get();
         answer.setRow(rowIndex, rowData);
      }

      return answer;
   }

   Matrix ParallelMatrixOpStrategy::scalarMult(const Matrix& lhs, const std::complex<double>& rhs)
   {
      std::vector<std::future<std::vector<std::complex<double>>>> results;
      for (uint32_t i = 0; i < lhs.getNumRows(); ++i)
      {
         ScalarMultSubJob job(lhs, i, rhs);
         auto result = _threadpool.postJob<std::vector<std::complex<double>>>(std::move(job));
         results.push_back(std::move(result));
      }

      Matrix answer(lhs.getNumRows(), lhs.getNumColumns());
      for (uint32_t i = 0; i < lhs.getNumRows(); ++i)
      {
         auto rowData = results[i].get();
         answer.setRow(i, rowData);
      }

      return answer;
   }

   Matrix ParallelMatrixOpStrategy::matrixAdd(const Matrix& lhs, const Matrix& rhs)
   {
      if (lhs.getNumRows() != rhs.getNumRows() || lhs.getNumColumns() != rhs.getNumColumns())
      {
         throw matrix::WrongMatrixSize();
      }

      std::vector<std::future<std::vector<std::complex<double>>>> results;
      for (uint32_t rowIndex = 0; rowIndex < lhs.getNumRows(); ++rowIndex)
      {
         MatrixAddSubJob job(lhs, rhs, rowIndex);
         auto result = _threadpool.postJob<std::vector<std::complex<double>>>(std::move(job));
         results.push_back(std::move(result));
      }

      Matrix answer(lhs.getNumRows(), rhs.getNumColumns());
      for (uint32_t rowIndex = 0; rowIndex < lhs.getNumRows(); ++rowIndex)
      {
         auto rowData = results[rowIndex].get();
         answer.setRow(rowIndex, rowData);
      }
      return answer;
   }

   Matrix ParallelMatrixOpStrategy::matrixSub(const Matrix& lhs, const Matrix& rhs)
   {
      return matrixAdd(lhs, scalarMult(rhs, std::complex<double>(-1.0, 0.0)));
   }

   Matrix ParallelMatrixOpStrategy::matrixTranspose(const Matrix& lhs)
   {
      std::vector<std::future<std::vector<std::complex<double>>>> results;
      for (uint32_t rowIndex = 0; rowIndex < lhs.getNumRows(); ++rowIndex)
      {
         TransposeSubJob job(lhs, rowIndex);
         auto result = _threadpool.postJob<std::vector<std::complex<double>>>(std::move(job));
         results.push_back(std::move(result));
      }

      Matrix answer(lhs.getNumColumns(), lhs.getNumRows());
      for (uint32_t rowIndex = 0; rowIndex < lhs.getNumRows(); ++rowIndex)
      {
         auto rowData = results[rowIndex].get();
         answer.setRow(rowIndex, rowData);
      }
      return answer;
   }

   std::complex<double> ParallelMatrixOpStrategy::matrixDeterminant(const Matrix& lhs)
   {
      if (lhs.getNumRows() != lhs.getNumColumns())
      {
         throw matrix::WrongMatrixSize();
      }

      if (lhs.getNumRows() == 1)
      {
         return lhs.getNumber(0, 0);
      }

      std::vector<std::future<Matrix>> results;
      for (uint32_t columnIndex = 0; columnIndex < lhs.getNumColumns(); ++columnIndex)
      {
         PartialMatrixSubJob job(lhs, 0, columnIndex);
         auto result = _threadpool.postJob<Matrix>(std::move(job));
         results.push_back(std::move(result));
      }

      std::complex<double> answer = 0;
      double sign = 1.0;
      for (uint32_t columnIndex = 0; columnIndex < lhs.getNumColumns(); ++columnIndex)
      {
         answer += sign * lhs.getNumber(0, columnIndex) * matrixDeterminant(results[columnIndex].get());
         sign *= -1.0;
      }

      return answer;
   }

   Matrix ParallelMatrixOpStrategy::matrixMinor(const Matrix& lhs)
   {
      if (lhs.getNumRows() != lhs.getNumColumns())
      {
         throw matrix::WrongMatrixSize();
      }

      std::vector<std::future<Matrix>> results;
      for (uint32_t rowIndex = 0; rowIndex < lhs.getNumRows(); ++rowIndex)
      {
         for (uint32_t columnIndex = 0; columnIndex < lhs.getNumColumns(); ++columnIndex)
         {
            PartialMatrixSubJob job(lhs, rowIndex, columnIndex);
            auto result = _threadpool.postJob<Matrix>(std::move(job));
            results.push_back(std::move(result));
         }
      }

      auto answerData = generateMatrixData(lhs.getNumRows(), lhs.getNumColumns());
      for (uint32_t rowIndex = 0; rowIndex < lhs.getNumRows(); ++rowIndex)
      {
         for (uint32_t columnIndex = 0; columnIndex < lhs.getNumColumns(); ++columnIndex)
         {
            answerData[rowIndex][columnIndex] = matrixDeterminant(results[rowIndex * lhs.getNumColumns() + columnIndex].get());
         }
      }

      return Matrix(lhs.getNumRows(), lhs.getNumColumns(), answerData);
   }

   Matrix ParallelMatrixOpStrategy::matrixCofactor(const Matrix& lhs)
   {
      if (lhs.getNumRows() != lhs.getNumColumns())
      {
         throw matrix::WrongMatrixSize();
      }

      std::vector<std::future<Matrix>> results;
      for (uint32_t rowIndex = 0; rowIndex < lhs.getNumRows(); ++rowIndex)
      {
         for (uint32_t columnIndex = 0; columnIndex < lhs.getNumColumns(); ++columnIndex)
         {
            PartialMatrixSubJob job(lhs, rowIndex, columnIndex);
            auto result = _threadpool.postJob<Matrix>(std::move(job));
            results.push_back(std::move(result));
         }
      }

      auto answerData = generateMatrixData(lhs.getNumRows(), lhs.getNumColumns());
      for (uint32_t rowIndex = 0; rowIndex < lhs.getNumRows(); ++rowIndex)
      {
         for (uint32_t columnIndex = 0; columnIndex < lhs.getNumColumns(); ++columnIndex)
         {
            answerData[rowIndex][columnIndex] = matrixDeterminant(results[rowIndex * lhs.getNumColumns() + columnIndex].get());
            if ((rowIndex + columnIndex) % 2 == 1)
            {
               answerData[rowIndex][columnIndex] *= -1;
            }
         }
      }

      Matrix returnValue(lhs.getNumRows(), lhs.getNumColumns(), answerData);
      return returnValue;
   }

   Matrix ParallelMatrixOpStrategy::matrixAdjoint(const Matrix& lhs)
   {
      if (lhs.getNumRows() != lhs.getNumColumns())
      {
         throw matrix::WrongMatrixSize();
      }

      return matrixTranspose(matrixCofactor(lhs));
   }

   Matrix ParallelMatrixOpStrategy::matrixInverse(const Matrix& lhs)
   {
      if (lhs.getNumRows() != lhs.getNumColumns())
      {
         throw matrix::WrongMatrixSize();
      }

      std::complex<double> determinant = matrixDeterminant(lhs);
      if (determinant == 0.0)
      {
         throw matrix::WrongMatrixSize();
      }

      return scalarMult(matrixAdjoint(lhs), 1.0 / determinant);
   }
}