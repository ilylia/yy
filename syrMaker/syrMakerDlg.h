// syrMakerDlg.h : ͷ�ļ�
//

#pragma once
#include "resource.h"
#include "afxwin.h"
#include "timeaxis.h"
#include "lrcEdit.h"
#include "yyListDlg.h"
#include "fileListDlg.h"


// CsyrMakerDlg �Ի���
class CsyrMakerDlg : public CDialog
{
// ����
public:
	CsyrMakerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CsyrMakerDlg();

// �Ի�������
	enum { IDD = IDD_SYRMAKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	int setSyrPath(CString path);
	int openSyr();
	int openSyr(CString path);

	int startSyr();
	int pauseSyr();
	int stopSyr();

	BOOL setTime(int time);
	int setNick(int idx, CString nick);			// ��idx��YY���ǳƣ�ʵ����Ļ
	int setNick();
	int delStrBlank(CString& str);

	void adjustPos();
	void addYYer(int num=1);
	void delYYer(int index);
	void resetYYer();

	void updateYYerLrc(ClrcEdit* pEdit);

protected:
	void freeCtrl();

public:
	CString m_syrPath;
	CTimeAxis m_timeAxis;
	CStatic m_cTime;
	CStyler m_syr;			// ��ʽ��Ļ

	vector<CStatic*>   m_vStaticEdit;
	vector<ClrcEdit*>  m_vEdit;
	vector<CButton*>   m_vButton;
	int iYYerNumOld;					// 

	// yy list
	CyyListDlg*  m_pYYListDlg;
	
	// file list
	CfileListDlg* m_pFileListDlg;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPaint();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonSaveas();
	afx_msg void OnBnClickedBtnBegin();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnForward();
	afx_msg void OnBnClickedBtnBackward();
	afx_msg void OnBnClickedBtnStepforward();
	afx_msg void OnBnClickedBtnStepback();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonAbout();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedRangeDel(UINT id);
	afx_msg void OnTimeaxisCopy();
	afx_msg void OnBnClickedButtonYylist();
	afx_msg void OnBnClickedButtonFilelist();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnSyrmakerLoadlrctp();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
};
