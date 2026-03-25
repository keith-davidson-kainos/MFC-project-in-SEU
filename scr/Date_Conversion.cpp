// Date_Conversion.cpp: 实现文件
//

#include "pch.h"
#include "Calculate_atom.h"
#include "Date_Conversion.h"
#include "DateConversionCore/DateConversionCore.h"
#include "afxdialogex.h"


// Date_Conversion 对话框

IMPLEMENT_DYNAMIC(Date_Conversion, CDialogEx)

Date_Conversion::Date_Conversion(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG9, pParent)
	, y1(_T(""))
	, m1(_T(""))
	, d1(_T(""))
	, y2(_T(""))
	, m2(_T(""))
	, d2(_T(""))
{

}

Date_Conversion::~Date_Conversion()
{
}

void Date_Conversion::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, y1);
	DDX_Text(pDX, IDC_EDIT2, m1);
	DDX_Text(pDX, IDC_EDIT4, d1);
	DDX_Text(pDX, IDC_EDIT5, y2);
	DDX_Text(pDX, IDC_EDIT6, m2);
	DDX_Text(pDX, IDC_EDIT7, d2);
}


BEGIN_MESSAGE_MAP(Date_Conversion, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &Date_Conversion::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &Date_Conversion::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &Date_Conversion::OnBnClickedButton4)
END_MESSAGE_MAP()


// Date_Conversion 消息处理程序
void Date_Conversion::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	//获取系统时间
	CTime tm;
	tm = CTime::GetCurrentTime();//获取系统日期
	str = tm.Format("现在北京时间是%Y年%m月%d日");
	MessageBox(str,CString("当前日期"), MB_OK);
}


void Date_Conversion::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
    dateconv::Date A = { int(_ttof(y1)), int(_ttof(m1)), int(_ttof(d1)) };
    dateconv::Date B = { int(_ttof(y2)), int(_ttof(m2)), int(_ttof(d2)) };

    if (!dateconv::IsValidDate(A) || !dateconv::IsValidDate(B))
    {
        MessageBox(CString("请输入有效日期（年份>=1900，月份1-12，日期合法）"), CString("输入错误"), MB_OK | MB_ICONWARNING);
        return;
    }

    int count = dateconv::DaysBetween(A, B);
    CString m;
    m.Format(_T("%d"), count);
    MessageBox(CString("第一个日期与第二个日期的差值为：") + m, CString("日期差值"));
	UpdateData(FALSE);
}


void Date_Conversion::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码

	
}
