// YYnickDlg.h : 头文件
//

#pragma once

#include "MyPic.h"
#include "ListCtrlEditable.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "lrc.h"
#include "Styler.h"
#include "keySetDlg.h"

struct SYYPos
{
	HWND m_hWnd;		// 窗口句柄
	POINT m_pos;		// 窗口焦点位置（主界面为昵称所在位置，频道界面为左下角昵称）
	int m_idx;			// 麦序
	CString m_sNick;	// 昵称
};

// CsyrPlayerDlg 对话框
class CsyrPlayerDlg : public CDialog
{
// 构造
public:
	CsyrPlayerDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CsyrPlayerDlg();

// 对话框数据
	enum { IDD = IDD_YYNICK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
public:
	int changeHandle(HANDLE hWnd);		// 获取yy
	int modifyYY(int idx, int item, CString val);
	void setYYtoMap(SYYPos syp, int idx);

	int initList(CString strLrcFile);	// 初始化歌词内容树
	int initList(int idx);				// 初始化对应idx序号的YY的字幕

	int startLRC();
	int pauseLRC();
	int stopLRC();

	int delStrBlank(CString& str);
	int setNick(int idx, CString nick);	// 改idx号YY的昵称，实现字幕
	int setNick();

	void regHotKey();
	void unregHotKey();

	bool readConfig();					// 读取设置
	bool saveConfig();					// 保存设置

protected:
	HICON m_hIcon;
	CMyPic m_pic;

	CListCtrlEditable m_elYY;	// YY列表
	CEdit m_editFilter;			// 过滤关键词
	
	CTreeCtrl m_treeLrcCon;		// 歌词内容
	CSliderCtrl m_sliderTime;	// 时间进度条
	CStatic m_cTime;			// 时间


	vector<SYYPos> m_sYYP;		// yy pos 
	map<int, map<HWND, SYYPos> > m_mmyy;

	int m_iLrcType;				// lrc类型	// 0:无  1:lrc  2:txt  3:syr
	LRC_INFO_T* m_lrc;			// lrc内容
	CStyler     m_syr;			// 花式字幕

	int m_iTimePos;				// 当前歌词时间, ms
	int m_iTimeAll;				// 全部时间
	int m_iCurYY;				// 当前字幕YY

	vector<CString> m_vFilter;
	BOOL m_bUseKey;				// 是否使用快捷键
	SKeySet m_key[4];			// 快捷键

public:
	DECLARE_MESSAGE_MAP()

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
	afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnLvnItemchangedListYy(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedButtonDel();

	afx_msg void OnEnChangeEditFilter();
	afx_msg void OnBnClickedCheckUsekey();
	afx_msg void OnBnClickedButtonKeyset();
	afx_msg void OnBnClickedButtonAbout();

	afx_msg void OnBnClickedBtnBegin();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnForward();
	afx_msg void OnBnClickedBtnBackward();
	afx_msg void OnNMClickTreeLrccon(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTreeLrccon(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawTreeLrccon(NMHDR *pNMHDR, LRESULT *pResult);
};
