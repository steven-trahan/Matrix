#include <gtest/gtest.h>
#include <MatrixOperations.h>
#include <Matrix.h>

namespace matrixtest
{

class MatrixOperationTests : public ::testing::TestWithParam<matrix::OperationType>
{
public:
   void SetUp() override
   {
      param = GetParam();
      matrix::MatrixOperations::setMatrixOpStrategy(param);
   }

   matrix::OperationType param;
};

INSTANTIATE_TEST_CASE_P(
   MatrixTests,
   MatrixOperationTests,
   ::testing::Values(matrix::OperationType::Serial, matrix::OperationType::Parallel));

TEST_P(MatrixOperationTests, MatrixWithIncData_ReadAllNumbers_IncDataReturned) 
{
   matrix::Matrix matrix(2, 2, { {1, 2}, {3, 4} });

   std::vector<std::complex<double>> data = matrix.getAllNumbers();

   std::vector<std::complex<double>> expectedData = { 1, 2, 3, 4 };
   EXPECT_EQ(data, expectedData);
}

TEST_P(MatrixOperationTests, MatrixWithIncData_MultiplyByIdent_ReturnsIncData)
{
   matrix::Matrix matrix(2, 2, { {1, 2}, {3, 4} });
   
   matrix::Matrix ident(2, 2, { {1, 0}, {0, 1} });
   matrix::Matrix result = matrix * ident;

   std::vector<std::complex<double>> data = result.getAllNumbers();
   std::vector<std::complex<double>> expectedData = { 1, 2, 3, 4 };
   EXPECT_EQ(data, expectedData);
}

TEST_P(MatrixOperationTests, MatrixWithIncData_Det_Neg2Returned)
{
   matrix::Matrix matrix(2, 2, { {1, 2}, {3, 4} });

   std::complex<double> det = matrix.determinant();

   std::complex<double> expectedDet = -2;
   EXPECT_EQ(det, expectedDet);
}

TEST_P(MatrixOperationTests, Matrix3x3_Det_NegFortyFourReturned)
{
   matrix::Matrix matrix(3, 3, { {3, 0, -1}, {2, -5, 4}, {-3, 1, 3} });

   std::complex<double> det = matrix.determinant();

   std::complex<double> expectedDet = -44;
   EXPECT_EQ(det, expectedDet);
}

TEST_P(MatrixOperationTests, Matrix3x3_Minor_ExpectedReturned)
{
   matrix::Matrix matrix(3, 3, { {2, 3, 1}, {1, 1, 2}, {2, 3, 4} });

   matrix::Matrix minor = matrix.minor();

   std::vector<std::complex<double>> data = minor.getAllNumbers();
   std::vector<std::complex<double>> expectedData = { -2, 0, 1, 9, 6, 0, 5, 3, -1 };
   EXPECT_EQ(data, expectedData);
}

TEST_P(MatrixOperationTests, Matrix3x3_Cofactor_ExpectedReturned)
{
   matrix::Matrix matrix(3, 3, { {2, 3, 1}, {1, 1, 2}, {2, 3, 4} });

   matrix::Matrix cofactor = matrix.cofactor();

   std::vector<std::complex<double>> data = cofactor.getAllNumbers();
   std::vector<std::complex<double>> expectedData = { -2, 0, 1, -9, 6, 0, 5, -3, -1 };
   EXPECT_EQ(data, expectedData);
}

TEST_P(MatrixOperationTests, Matrix3x3_Adjoint_ExpectedReturned)
{
   matrix::Matrix matrix(3, 3, { {2, 3, 1}, {1, 1, 2}, {2, 3, 4} });

   matrix::Matrix adjoint = matrix.adjoint();

   std::vector<std::complex<double>> data = adjoint.getAllNumbers();
   std::vector<std::complex<double>> expectedData = { -2, -9, 5, 0, 6, -3, 1, 0, -1 };
   EXPECT_EQ(data, expectedData);
}

bool isEqual(
   const std::vector<std::complex<double>>& a, 
   std::vector<std::complex<double>>& b)
{
   if (a.size() != b.size())
   {
      return false;
   }
   
   for (size_t i = 0; i < a.size(); ++i)
   {
      if (std::abs(a[i] - b[i]) >= 0.0000001)
      {
         return false;
      }
   }
   return true;
}
TEST_P(MatrixOperationTests, Matrix3x3_Inverse_ExpectedReturned)
{
   matrix::Matrix matrix(3, 3, { {2, 3, 1}, {1, 1, 2}, {2, 3, 4} });

   matrix::Matrix inverse = matrix.inverse();

   std::vector<std::complex<double>> data = inverse.getAllNumbers();
   std::vector<std::complex<double>> expectedData = { 2.0/3.0, 3, -5.0/3, 0, -2, 1, -1.0/3, 0, 1.0/3 };
   EXPECT_TRUE(isEqual(data, expectedData));
}

TEST_P(MatrixOperationTests, Matrix3x3_MultiplyByInverse_ReturnsIdentity)
{
   matrix::Matrix matrix(3, 3, { {2, 3, 1}, {1, 1, 2}, {2, 3, 4} });
   matrix::Matrix inverse = matrix.inverse();

   matrix::Matrix result = matrix * inverse;

   std::vector<std::complex<double>> data = result.getAllNumbers();
   std::vector<std::complex<double>> expectedData = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
   EXPECT_TRUE(isEqual(data, expectedData));
}

}