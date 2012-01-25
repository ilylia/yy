#pragma once

#include <vector>
using namespace std;

// CfileListDlg �Ի���

class CfileListDlg : public CDialog
{
	DECLARE_DYNAMIC(CfileListDlg)

public:
	CfileListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CfileListDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_FILELIST };

public:
	vector<CString> m_vFiles;
	CTreeCtrl  m_cList;

protected:
	void FileFind(LPCTSTR lpszPath);
	void AddFile(LPCTSTR path, LPCTSTR file);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonPath();
	afx_msg void OnBnClickedButtonPathfre();
	afx_msg void OnEnChangeEditFileSearch();
	afx_msg void OnNMDblclkTreeFile(NMHDR *pNMHDR, LRESULT *pResult);
};
