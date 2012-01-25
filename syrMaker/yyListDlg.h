#pragma once


#include <vector>
using namespace std;

// CyyListDlg �Ի���

struct SYYInfo
{
	HWND m_hWndParent;			// YY���ô���
	HWND m_hEditName;			// �û���
	HWND m_hEditNick;			// �ǳ�
	HWND m_hEditSign;			// ǩ��
	HWND m_hBtnApply;			// Ӧ�ð�ť
	HWND m_hLabelEdit;          // 
	HWND m_hEdit;				//
	CString m_sNick;			// �ǳ�
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
	int setNick(int idx, CString nick);			// ��idx��YY���ǳƣ�ʵ����Ļ
	int setNick();

	int getYYFromMain(CString strClassName);
	int getYYFromSetup(CString strClassName);

private:
	CListCtrl m_lYY;			// YY�б�
	vector<SYYInfo> m_sYY;		// yy

// 	int m_iYYType;				// yy������ͣ�0:YY������  1:YY���ý���
// 	BOOL m_bUseSBox;			// �Ƿ�ʹ��ɳ��
// 	CString m_strSBoxName;		// ɳ����

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedFresh();
	afx_msg void OnBnClickedButtonFreshset();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
