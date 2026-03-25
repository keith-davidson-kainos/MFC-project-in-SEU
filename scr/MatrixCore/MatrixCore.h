#pragma once

#include <array>
#include <string>

namespace matrixcore {

struct Matrix2x2 {
    double m11;
    double m12;
    double m21;
    double m22;
};

enum class MatrixError {
    None = 0,
    NullInput,
    EmptyInput,
    InvalidNumber,
    OutOfRange
};

enum class MatrixOperation {
    Add,
    Subtract,
    Multiply,
    TransposeLeft,
    TransposeRight,
    DeterminantLeft,
    DeterminantRight,
    RankLeft,
    RankRight
};

enum class ResponseKind {
    None = 0,
    Matrix,
    Scalar,
    Rank
};

struct OperationRequest {
    MatrixOperation operation;
    std::array<const char*, 4> left;
    std::array<const char*, 4> right;
};

struct OperationResponse {
    bool ok;
    MatrixError error;
    ResponseKind kind;
    Matrix2x2 matrix;
    double scalar;
    int rank;
    std::string message;
};

bool TryParseDouble(const char* text, double& value, MatrixError& error);
bool TryParseMatrix2x2(const std::array<const char*, 4>& fields, Matrix2x2& matrix, MatrixError& error, int& errorIndex);

Matrix2x2 Add(const Matrix2x2& lhs, const Matrix2x2& rhs);
Matrix2x2 Subtract(const Matrix2x2& lhs, const Matrix2x2& rhs);
Matrix2x2 Multiply(const Matrix2x2& lhs, const Matrix2x2& rhs);
Matrix2x2 Transpose(const Matrix2x2& matrix);

double Determinant(const Matrix2x2& matrix);
int Rank(const Matrix2x2& matrix);

const char* ToString(MatrixError error);
OperationResponse Execute(const OperationRequest& request);

} // namespace matrixcore
