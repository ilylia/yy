// timeCopyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "syrMaker.h"
#include "timeCopyDlg.h"


// CtimeCopyDlg 对话框

IMPLEMENT_DYNAMIC(CtimeCopyDlg, CDialog)

CtimeCopyDlg::CtimeCopyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CtimeCopyDlg::IDD, pParent)
	, m_iMM(0)
	, m_iSS(0)
	, m_iMS(0)
	, m_iMSMS(0)
	, m_time(0)
	, m_isTo(1)
{

}

CtimeCopyDlg::~CtimeCopyDlg()
{
}

void CtimeCopyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MIN, m_iMM);
	DDX_Text(pDX, IDC_EDIT_SEC, m_iSS);
	DDV_MinMaxInt(pDX, m_iSS, 0, 59);
	DDX_Text(pDX, IDC_EDIT_MS, m_iMS);
	DDV_MinMaxInt(pDX, m_iMS, 0, 999);
	DDX_Text(pDX, IDC_EDIT_MSMS, m_iMSMS);
}


BEGIN_MESSAGE_MAP(CtimeCopyDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_MSM, &CtimeCopyDlg::OnBnClickedRadioMsm)
	ON_BN_CLICKED(IDC_RADIO_MS, &CtimeCopyDlg::OnBnClickedRadioMs)
	ON_BN_CLICKED(IDOK, &CtimeCopyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_COPYTO, &CtimeCopyDlg::OnBnClickedRadioCopyto)
	ON_BN_CLICKED(IDC_RADIO_TIMEOFFSET, &CtimeCopyDlg::OnBnClickedRadioTimeoffset)
END_MESSAGE_MAP()


// CtimeCopyDlg 消息处理程序

BOOL CtimeCopyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CButton* p1 = (CButton*)GetDlgItem(IDC_RADIO_MSM);
	CButton* p2 = (CButton*)GetDlgItem(IDC_RADIO_COPYTO);
	p1->SetCheck(1);
	p2->SetCheck(1);

	m_iMSMS = m_time;
	m_iMM = m_time / 60000;
	m_iSS = m_time % 60000 / 1000;
	m_iMS = m_time % 60000 % 1000;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CtimeCopyDlg::setTime(int time)
{
	m_time = time;
}

int CtimeCopyDlg::getTime()
{
	return m_time;
}

int CtimeCopyDlg::isCopyTo()
{
	return m_isTo;
}

void CtimeCopyDlg::OnBnClickedRadioCopyto()
{
	m_iMSMS = m_time;
	m_iMM = m_time / 60000;
	m_iSS = m_time % 60000 / 1000;
	m_iMS = m_time % 60000 % 1000;

	UpdateData(FALSE);
}

void CtimeCopyDlg::OnBnClickedRadioTimeoffset()
{
	m_iMSMS = 0;
	m_iMM = 0;
	m_iSS = 0;
	m_iMS = 0;

	UpdateData(FALSE);
}

void CtimeCopyDlg::OnBnClickedRadioMsm()
{
	GetDlgItem(IDC_EDIT_MIN)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_SEC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_MS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_MM)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_SS)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_EDIT_MSMS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_MS)->ShowWindow(SW_HIDE);
}

void CtimeCopyDlg::OnBnClickedRadioMs()
{
	GetDlgItem(IDC_EDIT_MIN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_SEC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_MS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_MM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_SS)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_EDIT_MSMS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_MS)->ShowWindow(SW_SHOW);
}

void CtimeCopyDlg::OnBnClickedOk()
{
	if (!UpdateData(TRUE))
		return;

	CButton* p0 = (CButton*)GetDlgItem(IDC_CHECK_MINUS);
	int iminus = p0->GetCheck() ? -1 : 1;
	CButton* p1 = (CButton*)GetDlgItem(IDC_RADIO_MSM);
	if (p1->GetCheck() == 1)
	{
		m_time = m_iMM*60000 + m_iSS*1000 + m_iMS;
		m_time *= iminus;
	}
	else
	{
		m_time = m_iMSMS;
		m_time *= iminus;
	}

	CButton* p2 = (CButton*)GetDlgItem(IDC_RADIO_COPYTO);
	m_isTo = p2->GetCheck();

	OnOK();
}
