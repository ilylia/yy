// YYnickDlg.h : ͷ�ļ�
//

#pragma once

#include "MyPic.h"
#include "ListCtrlEditable.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "lrc.h"
#include "Styler.h"
#include "keySetDlg.h"

enum TYYWndType
{
	EYYWndNull = 0,
	EYYWndMain,
	EYYWndOption
};

struct SYYPos
{
	HWND m_hWnd;		// ���ھ��
	POINT m_pos;		// ���ڽ���λ�ã�������Ϊ�ǳ�����λ�ã�Ƶ������Ϊ���½��ǳƣ�
	POINT m_applyPos;	// ����Ӧ��λ��
	int m_idx;			// ����
	CString m_sNick;	// �ǳ�
	//HBITMAP m_bmp;		// �ǳƽ�ͼ
	int m_img;			// �ǳƽ�ͼ

	SYYPos():m_hWnd(NULL), m_idx(0), m_img(0)
	{
		m_pos.x = 0, m_pos.y = 0;
		m_applyPos.x = 0, m_applyPos.y = 0;
	}
	//~SYYPos()
	//{
	//	if (m_bmp != NULL)
	//	{
	//		::DeleteObject(m_bmp);
	//	}
	//}
};

struct SPosSet
{
	int m_nickX;	//�ǳ�λ��
	int m_nickY;
	int m_nickW;
	int m_nickH;
	int m_applyX;	//Ӧ��λ��
	int m_applyY;

	SPosSet() : m_nickX(0), m_nickY(0), m_nickW(0), m_nickH(0), m_applyX(0), m_applyY(0)
	{
	}
};

// CsyrPlayerDlg �Ի���
class CsyrPlayerDlg : public CDialog
{
// ����
public:
	CsyrPlayerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CsyrPlayerDlg();

// �Ի�������
	enum { IDD = IDD_YYNICK_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
public:
	int changeHandle(HANDLE hWnd);		// ��ȡyy
	int modifyYY(int idx, int item, CString val);

	int startLRC();
	int pauseLRC();
	int stopLRC();

	void regHotKey();
	void unregHotKey();

private:
	void setYYtoMap(SYYPos syp, int idx);

	int initList(CString strLrcFile);	// ��ʼ�����������
	int initList(int idx);				// ��ʼ����Ӧidx��ŵ�YY����Ļ

	int delStrBlank(CString& str);
	int setNick(int idx, CString nick);	// ��idx��YY���ǳƣ�ʵ����Ļ
	int setNick();
	void SendNick(SYYPos& syp, CString str);

	bool readConfig();					// ��ȡ����
	bool saveConfig();					// ��������

protected:
	HICON m_hIcon;
	CMyPic m_pic;

	CListCtrlEditable m_elYY;	// YY�б�
	CImageList m_imgList;		// ��ͼ
	CEdit m_editFilter;			// ���˹ؼ���
	
	CTreeCtrl m_treeLrcCon;		// �������
	CSliderCtrl m_sliderTime;	// ʱ�������
	CStatic m_cTime;			// ʱ��


	vector<SYYPos> m_sYYP;		// yy pos 
	map<int, map<HWND, SYYPos> > m_mmyy;

	int m_iLrcType;				// lrc����	// 0:��  1:lrc  2:txt  3:syr
	LRC_INFO_T* m_lrc;			// lrc����
	CStyler     m_syr;			// ��ʽ��Ļ

	int m_iTimePos;				// ��ǰ���ʱ��, ms
	int m_iTimeAll;				// ȫ��ʱ��
	int m_iCurYY;				// ��ǰ��ĻYY

	vector<CString> m_vFilter;
	BOOL m_bUseKey;				// �Ƿ�ʹ�ÿ�ݼ�
	SKeySet m_key[4];			// ��ݼ�
	SPosSet m_mainPosSet;		// ������
	SPosSet m_optionPosSet;		// �ҵ����ý���

	TYYWndType m_eYYWndType;
	SPosSet m_posSet;			// λ������

public:
	DECLARE_MESSAGE_MAP()

	// ���ɵ���Ϣӳ�亯��
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
