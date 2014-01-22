// YYnickDlg.h : 头文件
//

#pragma once

#include "ListCtrlEditable.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "lrc.h"
#include "Styler.h"
#include "keySetDlg.h"

enum TLrcType
{
	ELrcTypeNull = 0,
	ELrcTypeLrc,
	ELrcTypeTxt,
	ELrcTypeSyr
};

struct SYYPos
{
	unsigned int m_imId;
	int m_idx;			// 麦序
	CString m_sNick;	// 昵称
	CString m_sSign;

	SYYPos() : m_imId(0), m_idx(0)
	{
	}
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
	void AddYY(unsigned int unID);
	void DelYY(unsigned int unID);

	int modifyYY(int idx, int item, CString val);

	int startLRC();
	int pauseLRC();
	int stopLRC();

	void regHotKey();
	void unregHotKey();

private:
	void setYYtoMap(SYYPos syp, int idx);

	int initList(CString strLrcFile);	// 初始化歌词内容树
	int initList(int idx);				// 初始化对应idx序号的YY的字幕

	int delStrBlank(CString& str);
	int setNick(int idx, CString nick);	// 改idx号YY的昵称，实现字幕
	int setNick();
	void SendNick(SYYPos& syp, CString str);

	bool readConfig();					// 读取设置
	bool saveConfig();					// 保存设置

protected:
	HICON m_hIcon;

	CListCtrlEditable m_elYY;	// YY列表
	CEdit m_editFilter;			// 过滤关键词
	
	CTreeCtrl m_treeLrcCon;		// 歌词内容
	CSliderCtrl m_sliderTime;	// 时间进度条
	CStatic m_cTime;			// 时间

	vector<SYYPos> m_sYYP;		// yy pos 
	map<int, map<unsigned int, SYYPos> > m_mmyy;

	TLrcType    m_eLrcType;				// lrc类型 TLrcType
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
	afx_msg void OnBnClickedButtonRefresh();
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
