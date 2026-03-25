#include "pch.h"

#include <gtest/gtest.h>

#include "MatrixCore.h"

namespace {

void ExpectMatrixEq(const matrixcore::Matrix2x2& actual, const matrixcore::Matrix2x2& expected)
{
    EXPECT_NEAR(actual.m11, expected.m11, 1e-9);
    EXPECT_NEAR(actual.m12, expected.m12, 1e-9);
    EXPECT_NEAR(actual.m21, expected.m21, 1e-9);
    EXPECT_NEAR(actual.m22, expected.m22, 1e-9);
}

} // namespace

TEST(MatrixCoreUnit, AddReturnsElementWiseSum)
{
    const matrixcore::Matrix2x2 lhs { 1.0, 2.0, 3.0, 4.0 };
    const matrixcore::Matrix2x2 rhs { 10.0, 20.0, 30.0, 40.0 };

    const matrixcore::Matrix2x2 sum = matrixcore::Add(lhs, rhs);
    ExpectMatrixEq(sum, { 11.0, 22.0, 33.0, 44.0 });
}

TEST(MatrixCoreUnit, SubtractReturnsElementWiseDifference)
{
    const matrixcore::Matrix2x2 lhs { 9.0, 7.0, 5.0, 3.0 };
    const matrixcore::Matrix2x2 rhs { 1.0, 2.0, 3.0, 4.0 };

    const matrixcore::Matrix2x2 diff = matrixcore::Subtract(lhs, rhs);
    ExpectMatrixEq(diff, { 8.0, 5.0, 2.0, -1.0 });
}

TEST(MatrixCoreUnit, MultiplyReturnsCanonical2x2Product)
{
    const matrixcore::Matrix2x2 lhs { 1.0, 2.0, 3.0, 4.0 };
    const matrixcore::Matrix2x2 rhs { 5.0, 6.0, 7.0, 8.0 };

    const matrixcore::Matrix2x2 product = matrixcore::Multiply(lhs, rhs);
    ExpectMatrixEq(product, { 19.0, 22.0, 43.0, 50.0 });
}

TEST(MatrixCoreUnit, TransposeSwapsOffDiagonalTerms)
{
    const matrixcore::Matrix2x2 matrix { 1.0, 2.0, 3.0, 4.0 };

    const matrixcore::Matrix2x2 transposed = matrixcore::Transpose(matrix);
    ExpectMatrixEq(transposed, { 1.0, 3.0, 2.0, 4.0 });
}

TEST(MatrixCoreUnit, DeterminantComputesAdMinusBc)
{
    const matrixcore::Matrix2x2 matrix { 4.0, 7.0, 2.0, 6.0 };
    EXPECT_NEAR(matrixcore::Determinant(matrix), 10.0, 1e-9);
}

TEST(MatrixCoreUnit, RankReturnsTwoWhenDeterminantNonZero)
{
    const matrixcore::Matrix2x2 matrix { 4.0, 7.0, 2.0, 6.0 };
    EXPECT_EQ(matrixcore::Rank(matrix), 2);
}

TEST(MatrixCoreUnit, RankReturnsOneWhenDeterminantZeroButNotAllZero)
{
    const matrixcore::Matrix2x2 matrix { 1.0, 2.0, 2.0, 4.0 };
    EXPECT_EQ(matrixcore::Rank(matrix), 1);
}

TEST(MatrixCoreUnit, RankReturnsZeroForZeroMatrix)
{
    const matrixcore::Matrix2x2 matrix { 0.0, 0.0, 0.0, 0.0 };
    EXPECT_EQ(matrixcore::Rank(matrix), 0);
}

TEST(MatrixCoreEdgeCase, TryParseDoubleRejectsNullInput)
{
    double value = 0.0;
    matrixcore::MatrixError error = matrixcore::MatrixError::None;

    EXPECT_FALSE(matrixcore::TryParseDouble(nullptr, value, error));
    EXPECT_EQ(error, matrixcore::MatrixError::NullInput);
}

TEST(MatrixCoreEdgeCase, TryParseDoubleRejectsEmptyAndWhitespace)
{
    double value = 0.0;
    matrixcore::MatrixError error = matrixcore::MatrixError::None;

    EXPECT_FALSE(matrixcore::TryParseDouble("", value, error));
    EXPECT_EQ(error, matrixcore::MatrixError::EmptyInput);

    EXPECT_FALSE(matrixcore::TryParseDouble("   \t  ", value, error));
    EXPECT_EQ(error, matrixcore::MatrixError::EmptyInput);
}

TEST(MatrixCoreEdgeCase, TryParseDoubleRejectsInvalidAndOutOfRange)
{
    double value = 0.0;
    matrixcore::MatrixError error = matrixcore::MatrixError::None;

    EXPECT_FALSE(matrixcore::TryParseDouble("abc", value, error));
    EXPECT_EQ(error, matrixcore::MatrixError::InvalidNumber);

    EXPECT_FALSE(matrixcore::TryParseDouble("1.2x", value, error));
    EXPECT_EQ(error, matrixcore::MatrixError::InvalidNumber);

    EXPECT_FALSE(matrixcore::TryParseDouble("1e5000", value, error));
    EXPECT_EQ(error, matrixcore::MatrixError::OutOfRange);
}

TEST(MatrixCoreEdgeCase, TryParseDoubleAcceptsLeadingAndTrailingWhitespace)
{
    double value = 0.0;
    matrixcore::MatrixError error = matrixcore::MatrixError::InvalidNumber;

    EXPECT_TRUE(matrixcore::TryParseDouble("\t  -3.5  ", value, error));
    EXPECT_EQ(error, matrixcore::MatrixError::None);
    EXPECT_NEAR(value, -3.5, 1e-12);
}

TEST(MatrixCoreEdgeCase, TryParseMatrix2x2ReturnsErrorIndex)
{
    matrixcore::Matrix2x2 matrix {};
    matrixcore::MatrixError error = matrixcore::MatrixError::None;
    int index = -1;

    const std::array<const char*, 4> fields = { "1", "", "3", "4" };
    EXPECT_FALSE(matrixcore::TryParseMatrix2x2(fields, matrix, error, index));
    EXPECT_EQ(error, matrixcore::MatrixError::EmptyInput);
    EXPECT_EQ(index, 1);
}

TEST(MatrixCoreContract, ExecuteAddParsesAndReturnsMatrixPayload)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::Add,
        { "1", "2", "3", "4" },
        { "10", "20", "30", "40" }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_TRUE(response.ok);
    EXPECT_EQ(response.kind, matrixcore::ResponseKind::Matrix);
    ExpectMatrixEq(response.matrix, { 11.0, 22.0, 33.0, 44.0 });
}

TEST(MatrixCoreContract, ExecuteSubtractParsesAndReturnsMatrixPayload)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::Subtract,
        { "9", "7", "5", "3" },
        { "1", "2", "3", "4" }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_TRUE(response.ok);
    EXPECT_EQ(response.kind, matrixcore::ResponseKind::Matrix);
    ExpectMatrixEq(response.matrix, { 8.0, 5.0, 2.0, -1.0 });
    EXPECT_EQ(response.message, "OK");
}

TEST(MatrixCoreContract, ExecuteMultiplyParsesAndReturnsMatrixPayload)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::Multiply,
        { "1", "2", "3", "4" },
        { "5", "6", "7", "8" }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_TRUE(response.ok);
    EXPECT_EQ(response.kind, matrixcore::ResponseKind::Matrix);
    ExpectMatrixEq(response.matrix, { 19.0, 22.0, 43.0, 50.0 });
    EXPECT_EQ(response.message, "OK");
}

TEST(MatrixCoreContract, ExecuteTransposeLeftReturnsMatrixPayload)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::TransposeLeft,
        { "1", "2", "3", "4" },
        { nullptr, nullptr, nullptr, nullptr }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_TRUE(response.ok);
    EXPECT_EQ(response.kind, matrixcore::ResponseKind::Matrix);
    ExpectMatrixEq(response.matrix, { 1.0, 3.0, 2.0, 4.0 });
}

TEST(MatrixCoreContract, ExecuteTransposeRightReturnsMatrixPayload)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::TransposeRight,
        { nullptr, nullptr, nullptr, nullptr },
        { "1", "2", "3", "4" }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_TRUE(response.ok);
    EXPECT_EQ(response.kind, matrixcore::ResponseKind::Matrix);
    ExpectMatrixEq(response.matrix, { 1.0, 3.0, 2.0, 4.0 });
}

TEST(MatrixCoreContract, ExecuteDeterminantLeftUsesOnlyLeftPayload)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::DeterminantLeft,
        { "4", "7", "2", "6" },
        { nullptr, nullptr, nullptr, nullptr }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_TRUE(response.ok);
    EXPECT_EQ(response.kind, matrixcore::ResponseKind::Scalar);
    EXPECT_NEAR(response.scalar, 10.0, 1e-9);
}

TEST(MatrixCoreContract, ExecuteDeterminantRightReturnsScalarPayload)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::DeterminantRight,
        { nullptr, nullptr, nullptr, nullptr },
        { "4", "7", "2", "6" }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_TRUE(response.ok);
    EXPECT_EQ(response.kind, matrixcore::ResponseKind::Scalar);
    EXPECT_NEAR(response.scalar, 10.0, 1e-9);
}

TEST(MatrixCoreContract, ExecuteRankLeftReturnsRankPayload)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::RankLeft,
        { "0", "0", "0", "0" },
        { nullptr, nullptr, nullptr, nullptr }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_TRUE(response.ok);
    EXPECT_EQ(response.kind, matrixcore::ResponseKind::Rank);
    EXPECT_EQ(response.rank, 0);
}

TEST(MatrixCoreContract, ExecuteRankRightReturnsRankPayload)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::RankRight,
        { nullptr, nullptr, nullptr, nullptr },
        { "1", "2", "2", "4" }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_TRUE(response.ok);
    EXPECT_EQ(response.kind, matrixcore::ResponseKind::Rank);
    EXPECT_EQ(response.rank, 1);
}

TEST(MatrixCoreContract, ExecuteReturnsFieldScopedParseError)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::Multiply,
        { "1", "2", "3", "4" },
        { "5", "6", "", "8" }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_FALSE(response.ok);
    EXPECT_EQ(response.error, matrixcore::MatrixError::EmptyInput);
    EXPECT_EQ(response.message, "right[21] EmptyInput");
}

TEST(MatrixCoreContract, ExecuteReturnsFieldScopedLeftError)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::Add,
        { "", "2", "3", "4" },
        { "5", "6", "7", "8" }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_FALSE(response.ok);
    EXPECT_EQ(response.error, matrixcore::MatrixError::EmptyInput);
    EXPECT_EQ(response.message, "left[11] EmptyInput");
}

TEST(MatrixCoreContract, ExecuteAddReturnsRightFieldError)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::Add,
        { "1", "2", "3", "4" },
        { "", "6", "7", "8" }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_FALSE(response.ok);
    EXPECT_EQ(response.error, matrixcore::MatrixError::EmptyInput);
    EXPECT_EQ(response.message, "right[11] EmptyInput");
}

TEST(MatrixCoreContract, ExecuteSubtractReturnsLeftFieldError)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::Subtract,
        { "", "2", "3", "4" },
        { "5", "6", "7", "8" }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_FALSE(response.ok);
    EXPECT_EQ(response.error, matrixcore::MatrixError::EmptyInput);
    EXPECT_EQ(response.message, "left[11] EmptyInput");
}

TEST(MatrixCoreContract, ExecuteMultiplyReturnsLeftFieldError)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::Multiply,
        { "", "2", "3", "4" },
        { "5", "6", "7", "8" }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_FALSE(response.ok);
    EXPECT_EQ(response.error, matrixcore::MatrixError::EmptyInput);
    EXPECT_EQ(response.message, "left[11] EmptyInput");
}

TEST(MatrixCoreContract, ExecuteReturnsUnknownOperationForInvalidEnum)
{
    const matrixcore::OperationRequest request {
        static_cast<matrixcore::MatrixOperation>(999),
        { "1", "2", "3", "4" },
        { "5", "6", "7", "8" }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_FALSE(response.ok);
    EXPECT_EQ(response.error, matrixcore::MatrixError::InvalidNumber);
    EXPECT_EQ(response.message, "Unknown operation");
}

TEST(MatrixCoreContract, ExecuteSubtractReturnsRightFieldError)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::Subtract,
        { "1", "2", "3", "4" },
        { "", "6", "7", "8" }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_FALSE(response.ok);
    EXPECT_EQ(response.error, matrixcore::MatrixError::EmptyInput);
    EXPECT_EQ(response.message, "right[11] EmptyInput");
}

TEST(MatrixCoreContract, ExecuteTransposeLeftReturnsLeftFieldError)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::TransposeLeft,
        { "", "2", "3", "4" },
        { nullptr, nullptr, nullptr, nullptr }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_FALSE(response.ok);
    EXPECT_EQ(response.error, matrixcore::MatrixError::EmptyInput);
    EXPECT_EQ(response.message, "left[11] EmptyInput");
}

TEST(MatrixCoreContract, ExecuteTransposeRightReturnsRightFieldError)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::TransposeRight,
        { nullptr, nullptr, nullptr, nullptr },
        { "", "2", "3", "4" }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_FALSE(response.ok);
    EXPECT_EQ(response.error, matrixcore::MatrixError::EmptyInput);
    EXPECT_EQ(response.message, "right[11] EmptyInput");
}

TEST(MatrixCoreContract, ExecuteDeterminantLeftReturnsLeftFieldError)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::DeterminantLeft,
        { "", "2", "3", "4" },
        { nullptr, nullptr, nullptr, nullptr }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_FALSE(response.ok);
    EXPECT_EQ(response.error, matrixcore::MatrixError::EmptyInput);
    EXPECT_EQ(response.message, "left[11] EmptyInput");
}

TEST(MatrixCoreContract, ExecuteDeterminantRightReturnsRightFieldError)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::DeterminantRight,
        { nullptr, nullptr, nullptr, nullptr },
        { "", "2", "3", "4" }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_FALSE(response.ok);
    EXPECT_EQ(response.error, matrixcore::MatrixError::EmptyInput);
    EXPECT_EQ(response.message, "right[11] EmptyInput");
}

TEST(MatrixCoreContract, ExecuteRankLeftReturnsLeftFieldError)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::RankLeft,
        { "", "2", "3", "4" },
        { nullptr, nullptr, nullptr, nullptr }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_FALSE(response.ok);
    EXPECT_EQ(response.error, matrixcore::MatrixError::EmptyInput);
    EXPECT_EQ(response.message, "left[11] EmptyInput");
}

TEST(MatrixCoreContract, ExecuteRankRightReturnsRightFieldError)
{
    const matrixcore::OperationRequest request {
        matrixcore::MatrixOperation::RankRight,
        { nullptr, nullptr, nullptr, nullptr },
        { "", "2", "3", "4" }
    };

    const matrixcore::OperationResponse response = matrixcore::Execute(request);
    EXPECT_FALSE(response.ok);
    EXPECT_EQ(response.error, matrixcore::MatrixError::EmptyInput);
    EXPECT_EQ(response.message, "right[11] EmptyInput");
}

TEST(MatrixCoreUnit, ToStringMapsAllErrorsAndUnknown)
{
    EXPECT_STREQ(matrixcore::ToString(matrixcore::MatrixError::None), "None");
    EXPECT_STREQ(matrixcore::ToString(matrixcore::MatrixError::NullInput), "NullInput");
    EXPECT_STREQ(matrixcore::ToString(matrixcore::MatrixError::EmptyInput), "EmptyInput");
    EXPECT_STREQ(matrixcore::ToString(matrixcore::MatrixError::InvalidNumber), "InvalidNumber");
    EXPECT_STREQ(matrixcore::ToString(matrixcore::MatrixError::OutOfRange), "OutOfRange");
    EXPECT_STREQ(matrixcore::ToString(static_cast<matrixcore::MatrixError>(999)), "Unknown");
}
