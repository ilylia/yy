#pragma once


#include <vector>
using namespace std;

// CyyListDlg �Ի���

struct SYYInfo
{
	unsigned int m_imId;
	CString m_sNick;	// �ǳ�
	CString m_sSign;

	SYYInfo() : m_imId(0)
	{
	}
};

class CyyListDlg : public CDialog
{
	DECLARE_DYNAMIC(CyyListDlg)

public:
	CyyListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CyyListDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_YYLIST };

public:
	void AddYY(unsigned int imId);
	void DelYY(unsigned int imId);

	int setNick(int idx, CString nick);			// ��idx��YY���ǳƣ�ʵ����Ļ
	int setNick();

private:
	void SendNick(SYYInfo& syp, CString str);

private:
	CListCtrl m_lYY;			// YY�б�
	vector<SYYInfo> m_sYY;		// yy

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedFresh();
	afx_msg void OnBnClickedButtonHideyyapp();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
