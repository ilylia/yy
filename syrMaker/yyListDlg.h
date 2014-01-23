#pragma once


#include <vector>
using namespace std;

// CyyListDlg 对话框

struct SYYInfo
{
	unsigned int m_imId;
	CString m_sNick;	// 昵称
	CString m_sSign;

	SYYInfo() : m_imId(0)
	{
	}
};

class CyyListDlg : public CDialog
{
	DECLARE_DYNAMIC(CyyListDlg)

public:
	CyyListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CyyListDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_YYLIST };

public:
	void AddYY(unsigned int imId);
	void DelYY(unsigned int imId);

	int setNick(int idx, CString nick);			// 改idx号YY的昵称，实现字幕
	int setNick();

private:
	void SendNick(SYYInfo& syp, CString str);

private:
	CListCtrl m_lYY;			// YY列表
	vector<SYYInfo> m_sYY;		// yy

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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
