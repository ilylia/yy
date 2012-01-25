// YYnickDlg.h : ͷ�ļ�
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
	HWND m_hWnd;	// ���ھ��
	//POINT m_pos;	// ����λ��
	RECT m_rect;	// ����λ�ô�С
	CString m_sNick;			// �ǳ�
};

// struct SYYSetup
// {
// 	HWND m_hWndParent;			// YY���ô���
// 	HWND m_hEditName;			// �û���
// 	HWND m_hEditNick;			// �ǳ�
// 	HWND m_hEditSign;			// ǩ��
// 	HWND m_hBtnApply;			// Ӧ�ð�ť
// 	HWND m_hLabelEdit;          // 
// 	HWND m_hEdit;				//
// 	CString m_sNick;			// �ǳ�
// };

// CYYnickDlg �Ի���
class CYYnickDlg : public CDialog
{
// ����
public:
	CYYnickDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CYYnickDlg();

// �Ի�������
	enum { IDD = IDD_YYNICK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	int initList(CString strLrcFile);	// ��ʼ�����������
	int initList(int idx);				// ��ʼ����Ӧidx��ŵ�YY����Ļ
	int setNick(int idx, CString nick);			// ��idx��YY���ǳƣ�ʵ����Ļ
	int setNick();
	int startLRC();
	int pauseLRC();
	int stopLRC();

	bool readConfig();					// ��ʼ�����ã�ǰ��׺�����˹ؼ���
	bool saveConfig();					// ��ʼ�����ã�ǰ��׺�����˹ؼ���

	int delStrBlank(CString& str);

	int changeHandle(HANDLE hWnd);		// 
	int getYYFromMain(CString strClassName);
	int getYYFromSetup(CString strClassName);

	void regHotKey();
	void unregHotKey();

	void clearClip();
	void setClip(CString str);
	CString getClip();

// ʵ��
protected:
	HICON m_hIcon;
	//HANDLE m_hCtrl;
	CMyPic m_pic;

	CTreeCtrl m_treeLrcCon;		// �������
	CStatic m_cTime;			// ʱ��
	CListCtrl m_lYY;			// YY�б�

	CComboBox m_cNickName;		// �ǳ�
	CComboBox m_cPrefix;		// ǰ׺
	CComboBox m_cPostfix;		// ��׺
	CEdit m_editFilter;			// ���˹ؼ���
	CSliderCtrl m_sliderTime;	// ʱ�������

	vector<CString> m_vNickName;
	vector<CString> m_vPrefix;
	vector<CString> m_vPostfix;
	vector<CString> m_vFilter;
	SKeySet		m_key[5];		// ��ݼ�
	BOOL        m_bUseKey;		// 

	int m_iYYType;				// yy������ͣ�0:YY������  1:YY���ý���
	BOOL m_bUseSBox;			// �Ƿ�ʹ��ɳ��
	CString m_strSBoxName;		// ɳ����
	//vector<SYYSetup> m_sYY;		// yy
	vector<SYYPos> m_sYYP;		// yy pos 

	int m_iLrcType;				// lrc����	// 0:��  1:lrc  2:txt  3:syr
	LRC_INFO_T* m_lrc;			// lrc����
	CStyler     m_syr;			// ��ʽ��Ļ

	int m_iTimePos;				// ��ǰ���ʱ��, ms
	int m_iTimeAll;				// ȫ��ʱ��
	int m_iCurYY;				// ��ǰ��ĻYY
	BOOL m_isAlign;				// ��Ļ�Ƿ����˶���

	DECLARE_MESSAGE_MAP()

public:
	// ���ɵ���Ϣӳ�亯��
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
