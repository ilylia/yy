#pragma once

#include "resource.h"


enum KEY_TYPE
{
	KEYSET_BEGIN = 0, //开始
	KEYSET_STOP, //停止
	KEYSET_FORWARD, //快进
	KEYSET_BACKWARD //快退
};

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

// CkeySetDlg 对话框

class CkeySetDlg : public CDialog
{
	DECLARE_DYNAMIC(CkeySetDlg)

public:
	CkeySetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CkeySetDlg();

// 对话框数据
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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
// 	afx_msg void OnBnClickedCheckUsekey();
};
