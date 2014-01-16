// YYnickDlg.h : ͷ�ļ�
//

#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "lrc.h"
#include "Styler.h"

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
	int startLRC();
	int pauseLRC();
	int stopLRC();

private:
	int initList(CString strLrcFile);	// ��ʼ�����������
	int initList(int idx);				// ��ʼ����Ӧidx��ŵ�YY����Ļ

	int delStrBlank(CString& str);
	int setNick(int idx, CString nick);	// ��idx��YY���ǳƣ�ʵ����Ļ
	int setNick();
	void SendNick(CString str);

	void GetCfgPath();
	bool readConfig();					// ��ȡ����
	bool saveConfig();					// ��������

protected:
	HICON m_hIcon;

	CTreeCtrl m_treeLrcCon;		// �������
	CSliderCtrl m_sliderTime;	// ʱ�������
	CStatic m_cTime;			// ʱ��

	CString m_yyNick;

	int m_iLrcType;				// lrc����	// 0:��  1:lrc  2:txt  3:syr
	LRC_INFO_T* m_lrc;			// lrc����
	CStyler     m_syr;			// ��ʽ��Ļ

	int m_iTimePos;				// ��ǰ���ʱ��, ms
	int m_iTimeAll;				// ȫ��ʱ��
	int m_iCurYY;				// ��ǰ��ĻYY

public:
	DECLARE_MESSAGE_MAP()

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedBtnBegin();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnForward();
	afx_msg void OnBnClickedBtnBackward();
	afx_msg void OnNMClickTreeLrccon(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTreeLrccon(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawTreeLrccon(NMHDR *pNMHDR, LRESULT *pResult);
};
