// timeSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "syrMaker.h"
#include "timeSetDlg.h"


// CtimeSetDlg 对话框

IMPLEMENT_DYNAMIC(CtimeSetDlg, CDialog)

CtimeSetDlg::CtimeSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CtimeSetDlg::IDD, pParent)
	, m_iMM(0)
	, m_iSS(0)
	, m_iMS(0)
	, m_iMSMS(0)
	, m_time(0)
{

}

CtimeSetDlg::~CtimeSetDlg()
{
}

void CtimeSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MIN, m_iMM);
	DDX_Text(pDX, IDC_EDIT_SEC, m_iSS);
	DDV_MinMaxInt(pDX, m_iSS, 0, 59);
	DDX_Text(pDX, IDC_EDIT_MS, m_iMS);
	DDV_MinMaxInt(pDX, m_iMS, 0, 999);
	DDX_Text(pDX, IDC_EDIT_MSMS, m_iMSMS);
}


BEGIN_MESSAGE_MAP(CtimeSetDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_MSM, &CtimeSetDlg::OnBnClickedRadioMsm)
	ON_BN_CLICKED(IDC_RADIO_MS, &CtimeSetDlg::OnBnClickedRadioMs)
	ON_BN_CLICKED(IDOK, &CtimeSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CtimeSetDlg 消息处理程序

BOOL CtimeSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CButton* p1 = (CButton*)GetDlgItem(IDC_RADIO_MSM);
	CButton* p2 = (CButton*)GetDlgItem(IDC_RADIO_MS);
	p1->SetCheck(1);

	m_iMSMS = m_time;
	m_iMM = m_time / 60000;
	m_iSS = m_time % 60000 / 1000;
	m_iMS = m_time % 60000 % 1000;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CtimeSetDlg::setTime(int time)
{
	m_time = time;
}

int CtimeSetDlg::getTime()
{
	return m_time;
}

void CtimeSetDlg::OnBnClickedRadioMsm()
{
	GetDlgItem(IDC_EDIT_MIN)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_SEC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_MS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_MM)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_SS)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_EDIT_MSMS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_MS)->ShowWindow(SW_HIDE);
}

void CtimeSetDlg::OnBnClickedRadioMs()
{
	GetDlgItem(IDC_EDIT_MIN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_SEC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_MS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_MM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_SS)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_EDIT_MSMS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_MS)->ShowWindow(SW_SHOW);
}

void CtimeSetDlg::OnBnClickedOk()
{
	if (!UpdateData(TRUE))
		return;

	CButton* p1 = (CButton*)GetDlgItem(IDC_RADIO_MSM);
	if (p1->GetCheck() == 1)
	{
		m_time = m_iMM*60000 + m_iSS*1000 + m_iMS;
	}
	else
	{
		m_time = m_iMSMS;
	}

	OnOK();
}
