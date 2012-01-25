// YYnickDlg.h : 头文件
//

#pragma once

 #include "MyPic.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "lrc.h"
#include "Styler.h"
#include "keySetDlg.h"

struct SYYPos
{
	HWND m_hWnd;	// 窗口句柄
	//POINT m_pos;	// 窗口位置
	RECT m_rect;	// 窗口位置大小
	CString m_sNick;			// 昵称
};

// struct SYYSetup
// {
// 	HWND m_hWndParent;			// YY设置窗口
// 	HWND m_hEditName;			// 用户名
// 	HWND m_hEditNick;			// 昵称
// 	HWND m_hEditSign;			// 签名
// 	HWND m_hBtnApply;			// 应用按钮
// 	HWND m_hLabelEdit;          // 
// 	HWND m_hEdit;				//
// 	CString m_sNick;			// 昵称
// };

// CYYnickDlg 对话框
class CYYnickDlg : public CDialog
{
// 构造
public:
	CYYnickDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CYYnickDlg();

// 对话框数据
	enum { IDD = IDD_YYNICK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	int initList(CString strLrcFile);	// 初始化歌词内容树
	int initList(int idx);				// 初始化对应idx序号的YY的字幕
	int setNick(int idx, CString nick);			// 改idx号YY的昵称，实现字幕
	int setNick();
	int startLRC();
	int pauseLRC();
	int stopLRC();

	bool readConfig();					// 初始化设置，前后缀，过滤关键词
	bool saveConfig();					// 初始化设置，前后缀，过滤关键词

	int delStrBlank(CString& str);

	int changeHandle(HANDLE hWnd);		// 
	int getYYFromMain(CString strClassName);
	int getYYFromSetup(CString strClassName);

	void regHotKey();
	void unregHotKey();

	void clearClip();
	void setClip(CString str);
	CString getClip();

// 实现
protected:
	HICON m_hIcon;
	//HANDLE m_hCtrl;
	CMyPic m_pic;

	CTreeCtrl m_treeLrcCon;		// 歌词内容
	CStatic m_cTime;			// 时间
	CListCtrl m_lYY;			// YY列表

	CComboBox m_cNickName;		// 昵称
	CComboBox m_cPrefix;		// 前缀
	CComboBox m_cPostfix;		// 后缀
	CEdit m_editFilter;			// 过滤关键词
	CSliderCtrl m_sliderTime;	// 时间进度条

	vector<CString> m_vNickName;
	vector<CString> m_vPrefix;
	vector<CString> m_vPostfix;
	vector<CString> m_vFilter;
	SKeySet		m_key[5];		// 快捷键
	BOOL        m_bUseKey;		// 

	int m_iYYType;				// yy句柄类型，0:YY主界面  1:YY设置界面
	BOOL m_bUseSBox;			// 是否使用沙盘
	CString m_strSBoxName;		// 沙盘名
	//vector<SYYSetup> m_sYY;		// yy
	vector<SYYPos> m_sYYP;		// yy pos 

	int m_iLrcType;				// lrc类型	// 0:无  1:lrc  2:txt  3:syr
	LRC_INFO_T* m_lrc;			// lrc内容
	CStyler     m_syr;			// 花式字幕

	int m_iTimePos;				// 当前歌词时间, ms
	int m_iTimeAll;				// 全部时间
	int m_iCurYY;				// 当前字幕YY
	BOOL m_isAlign;				// 字幕是否两端对其

	DECLARE_MESSAGE_MAP()

public:
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedFresh();
	afx_msg void OnBnClickedBtnBegin();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnForward();
	afx_msg void OnBnClickedBtnBackward();
	afx_msg void OnNMClickTreeLrccon(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTreeLrccon(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeEditFilter();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedCheckAlign();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLvnItemchangedListYy(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboType();
	afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);

	afx_msg void OnBnClickedButtonKeyset();
	afx_msg void OnBnClickedButtonAbout();
	afx_msg void OnBnClickedButtonFreshset();
	afx_msg void OnBnClickedCheckUsekey();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNMCustomdrawTreeLrccon(NMHDR *pNMHDR, LRESULT *pResult);
};
