// Matrix_Cal.cpp: 实现文件
//

#include "pch.h"
#include "Calculate_atom.h"
#include "Matrix_Cal.h"
#include "MatrixCore/MatrixCore.h"
#include "afxdialogex.h"

#include <array>


// Matrix_Cal 对话框

IMPLEMENT_DYNAMIC(Matrix_Cal, CDialogEx)

namespace {

std::array<const char*, 4> BuildMatrixFields(const CString& v11, const CString& v12, const CString& v21, const CString& v22)
{
	return { v11.GetString(), v12.GetString(), v21.GetString(), v22.GetString() };
}

CString BuildMatrixMessage(const matrixcore::Matrix2x2& matrix)
{
	CString c11;
	CString c12;
	CString c21;
	CString c22;
	c11.Format(_T("%.4f"), matrix.m11);
	c12.Format(_T("%.4f"), matrix.m12);
	c21.Format(_T("%.4f"), matrix.m21);
	c22.Format(_T("%.4f"), matrix.m22);

	CString message(_T("结果：新矩阵：["));
	message += c11 + CString(_T(", ")) + c12 + CString(_T("; ")) + c21 + CString(_T(", ")) + c22 + CString(_T("] "));
	return message;
}

CString BuildScalarMessage(double value)
{
	CString x;
	x.Format(_T("%.4f"), value);
	CString message(_T("结果:"));
	message += x;
	return message;
}

CString BuildRankMessage(int rank)
{
	CString x;
	x.Format(_T("%d"), rank);
	CString message(_T("结果:"));
	message += x;
	return message;
}

CString BuildErrorMessage(const matrixcore::OperationResponse& response)
{
	CString message;
	message.Format(_T("输入错误：%S"), response.message.c_str());
	return message;
}

matrixcore::OperationRequest BuildRequest(matrixcore::MatrixOperation operation,
	const CString& a11,
	const CString& a12,
	const CString& a21,
	const CString& a22,
	const CString& b11,
	const CString& b12,
	const CString& b21,
	const CString& b22)
{
	matrixcore::OperationRequest request {
		operation,
		BuildMatrixFields(a11, a12, a21, a22),
		BuildMatrixFields(b11, b12, b21, b22)
	};
	return request;
}

} // namespace

Matrix_Cal::Matrix_Cal(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG7, pParent)
	, a11(_T(""))
	, a12(_T(""))
	, a21(_T(""))
	, a22(_T(""))
	, b21(_T(""))
	, b11(_T(""))
	, b12(_T(""))
	, b22(_T(""))
{

}

Matrix_Cal::~Matrix_Cal()
{
}

void Matrix_Cal::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, a11);
	DDX_Text(pDX, IDC_EDIT4, a12);
	DDX_Text(pDX, IDC_EDIT2, a21);
	DDX_Text(pDX, IDC_EDIT5, a22);
	DDX_Text(pDX, IDC_EDIT9, b21);
	DDX_Text(pDX, IDC_EDIT6, b11);
	DDX_Text(pDX, IDC_EDIT7, b12);
	DDX_Text(pDX, IDC_EDIT8, b22);
}


BEGIN_MESSAGE_MAP(Matrix_Cal, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Matrix_Cal::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Matrix_Cal::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &Matrix_Cal::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON13, &Matrix_Cal::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON9, &Matrix_Cal::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &Matrix_Cal::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &Matrix_Cal::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &Matrix_Cal::OnBnClickedButton12)
END_MESSAGE_MAP()


// Matrix_Cal 消息处理程序


void Matrix_Cal::OnBnClickedButton1()
{
	UpdateData(TRUE);
	auto request = BuildRequest(matrixcore::MatrixOperation::Multiply, a11, a12, a21, a22, b11, b12, b21, b22);
	auto response = matrixcore::Execute(request);
	if (!response.ok) {
		MessageBox(BuildErrorMessage(response), _T("The answer"));
		return;
	}

	MessageBox(BuildMatrixMessage(response.matrix), _T("The answer"));
	UpdateData(FALSE);
}


void Matrix_Cal::OnBnClickedButton2()
{
	UpdateData(TRUE);
	auto request = BuildRequest(matrixcore::MatrixOperation::Add, a11, a12, a21, a22, b11, b12, b21, b22);
	auto response = matrixcore::Execute(request);
	if (!response.ok) {
		MessageBox(BuildErrorMessage(response), _T("The answer"));
		return;
	}

	MessageBox(BuildMatrixMessage(response.matrix), _T("The answer"));
	UpdateData(FALSE);
}


void Matrix_Cal::OnBnClickedButton4()
{
	UpdateData(TRUE);
	auto request = BuildRequest(matrixcore::MatrixOperation::Subtract, a11, a12, a21, a22, b11, b12, b21, b22);
	auto response = matrixcore::Execute(request);
	if (!response.ok) {
		MessageBox(BuildErrorMessage(response), _T("The answer"));
		return;
	}

	MessageBox(BuildMatrixMessage(response.matrix), _T("The answer"));
	UpdateData(FALSE);
}


void Matrix_Cal::OnBnClickedButton13()
{
	UpdateData(TRUE);
	auto leftRequest = BuildRequest(matrixcore::MatrixOperation::TransposeLeft, a11, a12, a21, a22, b11, b12, b21, b22);
	auto rightRequest = BuildRequest(matrixcore::MatrixOperation::TransposeRight, a11, a12, a21, a22, b11, b12, b21, b22);
	auto leftResponse = matrixcore::Execute(leftRequest);
	auto rightResponse = matrixcore::Execute(rightRequest);

	if (!leftResponse.ok) {
		MessageBox(BuildErrorMessage(leftResponse), _T("The answer"));
		return;
	}

	if (!rightResponse.ok) {
		MessageBox(BuildErrorMessage(rightResponse), _T("The answer"));
		return;
	}

	CString M(_T("结果：新矩阵1：[") );
	CString left11;
	CString left12;
	CString left21;
	CString left22;
	CString right11;
	CString right12;
	CString right21;
	CString right22;
	left11.Format(_T("%.4f"), leftResponse.matrix.m11);
	left12.Format(_T("%.4f"), leftResponse.matrix.m12);
	left21.Format(_T("%.4f"), leftResponse.matrix.m21);
	left22.Format(_T("%.4f"), leftResponse.matrix.m22);
	right11.Format(_T("%.4f"), rightResponse.matrix.m11);
	right12.Format(_T("%.4f"), rightResponse.matrix.m12);
	right21.Format(_T("%.4f"), rightResponse.matrix.m21);
	right22.Format(_T("%.4f"), rightResponse.matrix.m22);

	M += left11 + CString(_T(", ")) + left12 + CString(_T("; ")) + left21 + CString(_T(", ")) + left22 + CString(_T("] "));
	M += CString(_T(";  新矩阵2：[ "));
	M += right11 + CString(_T(", ")) + right12 + CString(_T("; ")) + right21 + CString(_T(", ")) + right22 + CString(_T("] "));
	MessageBox(M, _T("The answer"));
	UpdateData(FALSE);
}


void Matrix_Cal::OnBnClickedButton9()
{
	UpdateData(TRUE);
	auto request = BuildRequest(matrixcore::MatrixOperation::DeterminantLeft, a11, a12, a21, a22, b11, b12, b21, b22);
	auto response = matrixcore::Execute(request);
	if (!response.ok) {
		MessageBox(BuildErrorMessage(response), _T("The answer"));
		return;
	}

	MessageBox(BuildScalarMessage(response.scalar), _T("The answer"));
	UpdateData(FALSE);
}


void Matrix_Cal::OnBnClickedButton10()
{
	UpdateData(TRUE);
	auto request = BuildRequest(matrixcore::MatrixOperation::DeterminantRight, a11, a12, a21, a22, b11, b12, b21, b22);
	auto response = matrixcore::Execute(request);
	if (!response.ok) {
		MessageBox(BuildErrorMessage(response), _T("The answer"));
		return;
	}

	MessageBox(BuildScalarMessage(response.scalar), _T("The answer"));
	UpdateData(FALSE);
}


void Matrix_Cal::OnBnClickedButton11()
{
	UpdateData(TRUE);
	auto request = BuildRequest(matrixcore::MatrixOperation::RankLeft, a11, a12, a21, a22, b11, b12, b21, b22);
	auto response = matrixcore::Execute(request);
	if (!response.ok) {
		MessageBox(BuildErrorMessage(response), _T("The answer"));
		return;
	}

	MessageBox(BuildRankMessage(response.rank), _T("The answer"));
	UpdateData(FALSE);
}


void Matrix_Cal::OnBnClickedButton12()
{
	UpdateData(TRUE);
	auto request = BuildRequest(matrixcore::MatrixOperation::RankRight, a11, a12, a21, a22, b11, b12, b21, b22);
	auto response = matrixcore::Execute(request);
	if (!response.ok) {
		MessageBox(BuildErrorMessage(response), _T("The answer"));
		return;
	}

	MessageBox(BuildRankMessage(response.rank), _T("The answer"));
	UpdateData(FALSE);
}
