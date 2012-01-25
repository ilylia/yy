#if !defined(AFX_MYPIC_H__C586B8DF_EDB7_4F31_9FE7_850DCAC74579__INCLUDED_)
#define AFX_MYPIC_H__C586B8DF_EDB7_4F31_9FE7_850DCAC74579__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyPic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyPic window

class CMyPic : public CStatic
{
// Construction
public:
	CMyPic();

// Attributes
public:

// Operations
public:
	void SetWnd(void* pWnd)	{m_pWnd = pWnd;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyPic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyPic)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CStatic m_mypic;
	void* m_pWnd;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPIC_H__C586B8DF_EDB7_4F31_9FE7_850DCAC74579__INCLUDED_)
