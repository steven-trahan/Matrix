#include "SerialMatrixOpStrategy.h"
#include "Matrix.h"
#include "MatrixConstants.h"
#include "MatrixUtility.h"

namespace matrix
{
SerialMatrixOpStrategy::SerialMatrixOpStrategy() = default;
SerialMatrixOpStrategy::~SerialMatrixOpStrategy() = default;

Matrix SerialMatrixOpStrategy::matrixMult(const Matrix& lhs, const Matrix& rhs)
{
   if (lhs.getNumColumns() != rhs.getNumRows())
   {
      throw matrix::WrongMatrixSize();
   }

   Matrix answer(lhs.getNumRows(), rhs.getNumColumns());
   for (uint32_t rowIndex = 0; rowIndex < answer.getNumRows(); ++rowIndex)
   {
      for (uint32_t columnIndex = 0; columnIndex < answer.getNumColumns(); ++columnIndex)
      {
         std::complex<double> cellAnswer = 0;
         for (uint32_t index = 0; index < rhs.getNumRows(); ++index)
         {
            if (lhs.getNumber(rowIndex, index) == 0.0 || rhs.getNumber(index, columnIndex) == 0.0)
            {
               continue;
            }
            cellAnswer += lhs.getNumber(rowIndex, index) * rhs.getNumber(index, columnIndex);
         }
         answer.setNumber(rowIndex, columnIndex, cellAnswer);
      }
   }

   return answer;
}

Matrix SerialMatrixOpStrategy::scalarMult(const Matrix& lhs, const std::complex<double>& rhs)
{
   Matrix answer(lhs.getNumRows(), lhs.getNumColumns());
   for (uint32_t i = 0; i < lhs.getNumRows(); ++i)
   {
      for (uint32_t j = 0; j < lhs.getNumColumns(); ++j)
      {
         answer.setNumber(i, j, lhs.getNumber(i, j) * rhs);
      }
   }

   return answer;
}

Matrix SerialMatrixOpStrategy::matrixAdd(const Matrix& lhs, const Matrix& rhs)
{
   if (lhs.getNumRows() != rhs.getNumRows() || lhs.getNumColumns() != rhs.getNumColumns())
   {
      throw matrix::WrongMatrixSize();
   }

   Matrix answer(lhs.getNumRows(), rhs.getNumColumns());
   for (uint32_t rowIndex = 0; rowIndex < lhs.getNumRows(); ++rowIndex)
   {
      for (uint32_t columnIndex = 0; columnIndex < lhs.getNumColumns(); ++columnIndex)
      {
         answer.setNumber(rowIndex, columnIndex, lhs.getNumber(rowIndex, columnIndex) + rhs.getNumber(rowIndex, columnIndex));
      }
   }

   return answer;
}

Matrix SerialMatrixOpStrategy::matrixSub(const Matrix& lhs, const Matrix& rhs)
{
   if (lhs.getNumRows() != rhs.getNumRows() || lhs.getNumColumns() != rhs.getNumColumns())
   {
      throw matrix::WrongMatrixSize();
   }

   Matrix answer(lhs.getNumRows(), rhs.getNumColumns());
   for (uint32_t rowIndex = 0; rowIndex < lhs.getNumRows(); ++rowIndex)
   {
      for (uint32_t columnIndex = 0; columnIndex < lhs.getNumColumns(); ++columnIndex)
      {
         answer.setNumber(rowIndex, columnIndex, lhs.getNumber(rowIndex, columnIndex) - rhs.getNumber(rowIndex, columnIndex));
      }
   }

   return answer;
}

Matrix SerialMatrixOpStrategy::matrixTranspose(const Matrix& lhs)
{
   Matrix answer(lhs.getNumColumns(), lhs.getNumRows());
   for (uint32_t rowIndex = 0; rowIndex < lhs.getNumRows(); ++rowIndex)
   {
      for (uint32_t columnIndex = 0; columnIndex < lhs.getNumColumns(); ++columnIndex)
      {
         answer.setNumber(columnIndex, rowIndex, lhs.getNumber(rowIndex, columnIndex));
      }
   }

   return answer;
}

std::complex<double> SerialMatrixOpStrategy::matrixDeterminant(const Matrix& lhs)
{
   if (lhs.getNumRows() != lhs.getNumColumns())
   {
      throw matrix::WrongMatrixSize();
   }

   if (lhs.getNumRows() == 1)
   {
      return lhs.getNumber(0, 0);
   }

   std::complex<double> answer = 0;
   double sign = 1;
   for (uint32_t columnIndex = 0; columnIndex < lhs.getNumColumns(); ++columnIndex)
   {
      answer += sign * lhs.getNumber(0, columnIndex) * matrixDeterminant(_matrixExcludedRowAndColumn(lhs, 0, columnIndex));
      sign *= -1;
   }

   return answer;
}

Matrix SerialMatrixOpStrategy::matrixMinor(const Matrix& lhs)
{
   if (lhs.getNumRows() != lhs.getNumColumns())
   {
      throw matrix::WrongMatrixSize();
   }

   auto answerData = generateMatrixData(lhs.getNumRows(), lhs.getNumColumns());
   for (uint32_t rowIndex = 0; rowIndex < lhs.getNumRows(); ++rowIndex)
   {
      for (uint32_t columnIndex = 0; columnIndex < lhs.getNumColumns(); ++columnIndex)
      {
         answerData[rowIndex][columnIndex] = matrixDeterminant(_matrixExcludedRowAndColumn(lhs, rowIndex, columnIndex));
      }
   }

   return Matrix(lhs.getNumRows(), lhs.getNumColumns(), answerData);
}

Matrix SerialMatrixOpStrategy::matrixCofactor(const Matrix& lhs)
{
   if (lhs.getNumRows() != lhs.getNumColumns())
   {
      throw matrix::WrongMatrixSize();
   }

   auto answerData = generateMatrixData(lhs.getNumRows(), lhs.getNumColumns());
   for (uint32_t rowIndex = 0; rowIndex < lhs.getNumRows(); ++rowIndex)
   {
      for (uint32_t columnIndex = 0; columnIndex < lhs.getNumColumns(); ++columnIndex)
      {
         answerData[rowIndex][columnIndex] = matrixDeterminant(_matrixExcludedRowAndColumn(lhs, rowIndex, columnIndex));
         if ((rowIndex + columnIndex) % 2 == 1)
         {
            answerData[rowIndex][columnIndex] *= -1;
         }
      }
   }

   Matrix returnValue(lhs.getNumRows(), lhs.getNumColumns(), answerData);
   return returnValue;
}

Matrix SerialMatrixOpStrategy::matrixAdjoint(const Matrix& lhs)
{
   if (lhs.getNumRows() != lhs.getNumColumns())
   {
      throw matrix::WrongMatrixSize();
   }

   return matrixTranspose(matrixCofactor(lhs));
}

Matrix SerialMatrixOpStrategy::matrixInverse(const Matrix& lhs)
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

Matrix SerialMatrixOpStrategy::_matrixExcludedRowAndColumn(const Matrix& lhs, uint32_t excludedRowIndex, uint32_t excludedColumnIndex)
{
   auto answerData = generateMatrixData(lhs.getNumRows() - 1, lhs.getNumColumns() - 1);
   for (uint32_t rowIndex = 0; rowIndex < lhs.getNumRows(); ++rowIndex)
   {
      for (uint32_t columnIndex = 0; columnIndex < lhs.getNumColumns(); ++columnIndex)
      {
         if (rowIndex == excludedRowIndex || columnIndex == excludedColumnIndex)
         {
            continue;
         }

         uint32_t answerRowIndex = rowIndex;
         uint32_t answerColumnIndex = columnIndex;
         if (rowIndex > excludedRowIndex)
         {
            --answerRowIndex;
         }

         if (columnIndex > excludedColumnIndex)
         {
            --answerColumnIndex;
         }

         answerData[answerRowIndex][answerColumnIndex] = lhs.getNumber(rowIndex, columnIndex);
      }
   }

   return Matrix(lhs.getNumRows() - 1, lhs.getNumColumns() - 1, answerData);
}
}