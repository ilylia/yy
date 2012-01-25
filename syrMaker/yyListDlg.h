#pragma once


#include <vector>
using namespace std;

// CyyListDlg 对话框

struct SYYInfo
{
	HWND m_hWndParent;			// YY设置窗口
	HWND m_hEditName;			// 用户名
	HWND m_hEditNick;			// 昵称
	HWND m_hEditSign;			// 签名
	HWND m_hBtnApply;			// 应用按钮
	HWND m_hLabelEdit;          // 
	HWND m_hEdit;				//
	CString m_sNick;			// 昵称
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
	int setNick(int idx, CString nick);			// 改idx号YY的昵称，实现字幕
	int setNick();

	int getYYFromMain(CString strClassName);
	int getYYFromSetup(CString strClassName);

private:
	CListCtrl m_lYY;			// YY列表
	vector<SYYInfo> m_sYY;		// yy

// 	int m_iYYType;				// yy句柄类型，0:YY主界面  1:YY设置界面
// 	BOOL m_bUseSBox;			// 是否使用沙盘
// 	CString m_strSBoxName;		// 沙盘名

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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
