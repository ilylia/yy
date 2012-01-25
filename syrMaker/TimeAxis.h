#pragma once
#include "afxwin.h"
#include "Styler.h"

// ʱ�����
class CTimePoint
{
public:
	CTimePoint();
	CTimePoint(int time);
	~CTimePoint();

public:
	void set(int time);
	int get();

private:
	int _idxYY;	// yy���
	int _time;	// �õ�ʱ��
};

// ʱ������
class CTimeAxis :public CStatic
{
public:
	CTimeAxis(void);
	virtual ~CTimeAxis(void);

public:
	void init();
	void creatMemDC();

	void drawBackground(CDC* pDC);
	void drawTimeLine(CDC* pDC);
	void drawTimePoints(CDC* pDC);
	void drawTimePoint(CDC* pDC, int idxYY, int idxLrc/*, COLORREF clr*/);
	void drawTimePtSel(CDC* pDC);
	void drawCurTime(CDC* pDC);
	void drawTimeBrev(CDC* pDC);

	int& getCurTime();

	void setSyr(CStyler* syr);
	void reset();

	void delSelectTime();

	void setCtrlState(BOOL bCtrl);

public:
	// �ڴ�DC
	CDC     *m_pMemDC;
	CBitmap *m_pMemBM;
	// �ڴ�λͼ��С
	int m_dw, m_dh;
	// 	// �ͻ�����С
	// 	int m_cw, m_ch;

	CStyler* m_pSyr;		// 
	int m_minTime;			// 
	int m_maxTime;			// 
	int m_allTime;			// 

	BOOL m_bCtrl;			// Ctrl �Ƿ���

	BOOL m_bLBtnDown;		// ����Ƿ���
	BOOL m_bRBtnDown;		// �Ҽ�
	int m_iTimeClick;		// ���������ʱ��
	BOOL m_bBrevPick;		// �������������
	int m_iBrevX;			// 
	BOOL m_bTimePick;		// ʱ����������
	CPoint m_ptTime;		// 
	CPoint m_ptTimeR;		// 

	int m_iCurTime;			// 

public:
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
