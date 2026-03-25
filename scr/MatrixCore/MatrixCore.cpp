#include "MatrixCore.h"

#include <cerrno>
#include <cmath>
#include <cstdlib>

namespace {

const char* kFieldNames[4] = { "11", "12", "21", "22" };

const char* FieldName(int index)
{
    return kFieldNames[index];
}

bool IsSpace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

const char* SkipLeadingSpace(const char* text)
{
    while (*text != '\0' && IsSpace(*text)) {
        ++text;
    }
    return text;
}

bool IsOnlySpace(const char* text)
{
    while (*text != '\0') {
        if (!IsSpace(*text)) {
            return false;
        }
        ++text;
    }
    return true;
}

matrixcore::OperationResponse MakeDefaultResponse()
{
    matrixcore::OperationResponse response {};
    response.ok = false;
    response.error = matrixcore::MatrixError::None;
    response.kind = matrixcore::ResponseKind::None;
    response.matrix = matrixcore::Matrix2x2 { 0.0, 0.0, 0.0, 0.0 };
    response.scalar = 0.0;
    response.rank = 0;
    return response;
}

matrixcore::OperationResponse MakeFieldErrorResponse(const char* side, matrixcore::MatrixError error, int errorIndex)
{
    matrixcore::OperationResponse response = MakeDefaultResponse();
    response.error = error;
    response.message = std::string(side) + "[" + FieldName(errorIndex) + "] " + matrixcore::ToString(error);
    return response;
}

matrixcore::OperationResponse MakeMatrixResponse(const matrixcore::Matrix2x2& matrix)
{
    matrixcore::OperationResponse response = MakeDefaultResponse();
    response.ok = true;
    response.kind = matrixcore::ResponseKind::Matrix;
    response.matrix = matrix;
    response.message = "OK";
    return response;
}

matrixcore::OperationResponse MakeScalarResponse(double scalar)
{
    matrixcore::OperationResponse response = MakeDefaultResponse();
    response.ok = true;
    response.kind = matrixcore::ResponseKind::Scalar;
    response.scalar = scalar;
    response.message = "OK";
    return response;
}

matrixcore::OperationResponse MakeRankResponse(int rank)
{
    matrixcore::OperationResponse response = MakeDefaultResponse();
    response.ok = true;
    response.kind = matrixcore::ResponseKind::Rank;
    response.rank = rank;
    response.message = "OK";
    return response;
}

} // namespace

namespace matrixcore {

bool TryParseDouble(const char* text, double& value, MatrixError& error)
{
    if (text == nullptr) {
        error = MatrixError::NullInput;
        return false;
    }

    if (text[0] == '\0' || IsOnlySpace(text)) {
        error = MatrixError::EmptyInput;
        return false;
    }

    const char* start = SkipLeadingSpace(text);
    errno = 0;
    char* end = nullptr;
    double parsed = std::strtod(start, &end);

    if (start == end) {
        error = MatrixError::InvalidNumber;
        return false;
    }

    while (*end != '\0' && IsSpace(*end)) {
        ++end;
    }

    if (*end != '\0') {
        error = MatrixError::InvalidNumber;
        return false;
    }

    if (errno == ERANGE || !std::isfinite(parsed)) {
        error = MatrixError::OutOfRange;
        return false;
    }

    value = parsed;
    error = MatrixError::None;
    return true;
}

bool TryParseMatrix2x2(const std::array<const char*, 4>& fields, Matrix2x2& matrix, MatrixError& error, int& errorIndex)
{
    double values[4] = { 0.0, 0.0, 0.0, 0.0 };

    for (int i = 0; i < 4; ++i) {
        if (!TryParseDouble(fields[static_cast<size_t>(i)], values[i], error)) {
            errorIndex = i;
            return false;
        }
    }

    matrix.m11 = values[0];
    matrix.m12 = values[1];
    matrix.m21 = values[2];
    matrix.m22 = values[3];
    error = MatrixError::None;
    errorIndex = -1;
    return true;
}

Matrix2x2 Add(const Matrix2x2& lhs, const Matrix2x2& rhs)
{
    return Matrix2x2 {
        lhs.m11 + rhs.m11,
        lhs.m12 + rhs.m12,
        lhs.m21 + rhs.m21,
        lhs.m22 + rhs.m22
    };
}

Matrix2x2 Subtract(const Matrix2x2& lhs, const Matrix2x2& rhs)
{
    return Matrix2x2 {
        lhs.m11 - rhs.m11,
        lhs.m12 - rhs.m12,
        lhs.m21 - rhs.m21,
        lhs.m22 - rhs.m22
    };
}

Matrix2x2 Multiply(const Matrix2x2& lhs, const Matrix2x2& rhs)
{
    return Matrix2x2 {
        lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21,
        lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22,
        lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21,
        lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22
    };
}

Matrix2x2 Transpose(const Matrix2x2& matrix)
{
    return Matrix2x2 {
        matrix.m11,
        matrix.m21,
        matrix.m12,
        matrix.m22
    };
}

double Determinant(const Matrix2x2& matrix)
{
    return matrix.m11 * matrix.m22 - matrix.m21 * matrix.m12;
}

int Rank(const Matrix2x2& matrix)
{
    const double determinant = Determinant(matrix);

    if (determinant != 0.0) {
        return 2;
    }

    if (matrix.m11 == 0.0 && matrix.m12 == 0.0 && matrix.m21 == 0.0 && matrix.m22 == 0.0) {
        return 0;
    }

    return 1;
}

const char* ToString(MatrixError error)
{
    switch (error) {
    case MatrixError::None:
        return "None";
    case MatrixError::NullInput:
        return "NullInput";
    case MatrixError::EmptyInput:
        return "EmptyInput";
    case MatrixError::InvalidNumber:
        return "InvalidNumber";
    case MatrixError::OutOfRange:
        return "OutOfRange";
    default:
        return "Unknown";
    }
}

OperationResponse Execute(const OperationRequest& request)
{
    OperationResponse response = MakeDefaultResponse();

    Matrix2x2 left {};
    Matrix2x2 right {};
    MatrixError parseError = MatrixError::None;
    int errorIndex = -1;

    switch (request.operation) {
    case MatrixOperation::Add:
        if (!TryParseMatrix2x2(request.left, left, parseError, errorIndex)) {
            return MakeFieldErrorResponse("left", parseError, errorIndex);
        }
        if (!TryParseMatrix2x2(request.right, right, parseError, errorIndex)) {
            return MakeFieldErrorResponse("right", parseError, errorIndex);
        }
        return MakeMatrixResponse(Add(left, right));

    case MatrixOperation::Subtract:
        if (!TryParseMatrix2x2(request.left, left, parseError, errorIndex)) {
            return MakeFieldErrorResponse("left", parseError, errorIndex);
        }
        if (!TryParseMatrix2x2(request.right, right, parseError, errorIndex)) {
            return MakeFieldErrorResponse("right", parseError, errorIndex);
        }
        return MakeMatrixResponse(Subtract(left, right));

    case MatrixOperation::Multiply:
        if (!TryParseMatrix2x2(request.left, left, parseError, errorIndex)) {
            return MakeFieldErrorResponse("left", parseError, errorIndex);
        }
        if (!TryParseMatrix2x2(request.right, right, parseError, errorIndex)) {
            return MakeFieldErrorResponse("right", parseError, errorIndex);
        }
        return MakeMatrixResponse(Multiply(left, right));

    case MatrixOperation::TransposeLeft:
        if (!TryParseMatrix2x2(request.left, left, parseError, errorIndex)) {
            return MakeFieldErrorResponse("left", parseError, errorIndex);
        }
        return MakeMatrixResponse(Transpose(left));

    case MatrixOperation::TransposeRight:
        if (!TryParseMatrix2x2(request.right, right, parseError, errorIndex)) {
            return MakeFieldErrorResponse("right", parseError, errorIndex);
        }
        return MakeMatrixResponse(Transpose(right));

    case MatrixOperation::DeterminantLeft:
        if (!TryParseMatrix2x2(request.left, left, parseError, errorIndex)) {
            return MakeFieldErrorResponse("left", parseError, errorIndex);
        }
        return MakeScalarResponse(Determinant(left));

    case MatrixOperation::DeterminantRight:
        if (!TryParseMatrix2x2(request.right, right, parseError, errorIndex)) {
            return MakeFieldErrorResponse("right", parseError, errorIndex);
        }
        return MakeScalarResponse(Determinant(right));

    case MatrixOperation::RankLeft:
        if (!TryParseMatrix2x2(request.left, left, parseError, errorIndex)) {
            return MakeFieldErrorResponse("left", parseError, errorIndex);
        }
        return MakeRankResponse(Rank(left));

    case MatrixOperation::RankRight:
        if (!TryParseMatrix2x2(request.right, right, parseError, errorIndex)) {
            return MakeFieldErrorResponse("right", parseError, errorIndex);
        }
        return MakeRankResponse(Rank(right));

    default:
        response.error = MatrixError::InvalidNumber;
        response.message = "Unknown operation";
        return response;
    }
}

} // namespace matrixcore
