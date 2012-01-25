#pragma once

#include "resource.h"


struct SKeySet
{
	SKeySet()
	{
		bCtrl = FALSE;
		bCtrl = FALSE;
		bAlt  = FALSE;
		uCode = 0;
	}

	BOOL bCtrl;
	BOOL bShift;
	BOOL bAlt;
	UINT uCode;
};

// CkeySetDlg �Ի���

class CkeySetDlg : public CDialog
{
	DECLARE_DYNAMIC(CkeySetDlg)

public:
	CkeySetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CkeySetDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_KEYSET };

public:
	void initCombox();

// 	void setUseKey(BOOL use);
// 	BOOL getUseKey();
	void setKeySet(SKeySet* pKeySet);
	SKeySet* getKeySet();

	int setKey();
	int getKey();

	int getIndex(UINT code);
	int getIndex(CString key);
	CString codeToKey(UINT code);
	UINT keyToCode(CString key);

public:
// 	BOOL m_bUseKey;
	SKeySet* m_pKeySet;
// 	CString m_cFresh;
// 	CString m_cBegin;
// 	CString m_cStop;
// 	CString m_cForward;
// 	CString m_cBackward;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
// 	afx_msg void OnBnClickedCheckUsekey();
};
