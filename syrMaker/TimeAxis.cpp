#include "StdAfx.h"
#include "TimeAxis.h"
#include "timeSetDlg.h"

#define TIME_BREV_H 16

CTimePoint::CTimePoint()
: _time(0)
{
}

CTimePoint::CTimePoint(int time)
: _time(time)
{
}

CTimePoint::~CTimePoint()
{
}

void CTimePoint::set(int time)
{
	_time = time;
}

int CTimePoint::get()
{
	return _time;
}



CTimeAxis::CTimeAxis(void)
: m_ptTime(0, 0)
{
	m_pMemDC = 0;	m_pMemBM = 0;
	m_dw = m_dh = 0;

	m_pSyr = NULL;
	m_minTime = 0;
	m_maxTime = 300000;
	m_allTime = 300000;
// 	m_zoom = 1.0;
	m_bLBtnDown = FALSE;
	m_bRBtnDown = FALSE;
	m_iTimeClick = 0;
	m_iCurTime = 0;
	m_bBrevPick = FALSE;
	m_iBrevX = 0;
	m_bTimePick = FALSE;

	m_bCtrl = FALSE;
}

CTimeAxis::~CTimeAxis(void)
{
	if(m_pMemDC != 0)	delete m_pMemDC;
	if(m_pMemBM != 0)	delete m_pMemBM;
}

BEGIN_MESSAGE_MAP(CTimeAxis, CStatic)
	ON_WM_MOUSEWHEEL()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()

BOOL CTimeAxis::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
}

int CTimeAxis::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}

void CTimeAxis::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// 不为绘图消息调用 CStatic::OnPaint()

	drawBackground(&dc);
	drawTimeLine(&dc);
	drawTimePoints(&dc);
	drawTimePtSel(&dc);
	drawCurTime(&dc);
	drawTimeBrev(&dc);

// 	if (0 != m_pMemDC)
// 	{
// 		PatBlt(m_pMemDC->m_hDC, 0, 0, m_dw, m_dh, BLACKNESS);
// 
// 		drawBackground(m_pMemDC);
// 		drawTimeLine(m_pMemDC);
// 		drawTimePoints(m_pMemDC);
// 		drawTimePtSel(m_pMemDC);
// 		drawCurTime(m_pMemDC);
// 		drawTimeBrev(m_pMemDC);
// 
// 		dc.BitBlt(0, 0, m_dw, m_dh, m_pMemDC, 0, 0, SRCCOPY);
// 	}
}

void CTimeAxis::drawBackground(CDC* pDC)
{
	if (pDC == NULL)
		return;

	CRect rect;
	this->GetWindowRect(&rect);
	ScreenToClient(&rect);
	CRect rect1(rect);
	rect.top += TIME_BREV_H;
	rect1.top += 2;
	rect1.bottom = rect1.top + 10;

	CBrush brush(RGB(64, 64, 64));
	CBrush *pOldBrush = pDC->SelectObject(&brush);
	pDC->FillRect(&rect, &brush);
	pDC->FillRect(&rect1, &brush);
	pDC->SelectObject(pOldBrush);
}

void CTimeAxis::drawTimeLine(CDC* pDC)
{
	if (pDC == NULL)
		return;

	if (m_pSyr == NULL)
		return;

	int num = m_pSyr->getYYerCount();
	if (num < 1)
		return;

	CRect rect;
	this->GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.top += TIME_BREV_H;

	int dy = rect.Height() / (num + 1);

	CPen pen(PS_SOLID, 1, RGB(0, 255, 255));
	CPen *pOldPen = pDC->SelectObject(&pen);
	for (int i=0; i<num; i++)
	{
		pDC->MoveTo(rect.left, rect.top+dy*(i+1));
		pDC->LineTo(rect.right-1, rect.top+dy*(i+1));
	}
	pDC->SelectObject(pOldPen);
}

void CTimeAxis::drawTimePoint(CDC* pDC, int idxYY, int idxLrc/*, COLORREF clr*/)
{
	if (pDC == NULL)
		return;

	if (m_pSyr == NULL)
		return;

	int num = m_pSyr->getYYerCount();
	if (num < 1)
		return;

	if (idxYY < 0 || idxYY >= num)
		return;

	int numLrc = m_pSyr->getYYerLrcsCount(idxYY);
	if (numLrc < 1)
		return;

	if (idxLrc < 0 || idxLrc >= numLrc)
		return;

	int time = m_pSyr->getYYerLrcTime(idxYY, idxLrc);
	if (time < m_minTime || time > m_maxTime)
		return;

	CRect rect;
	this->GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.top += TIME_BREV_H;

// 	int dy = rect.Height() / (num + 1);

	int x = rect.Width() * (time - m_minTime) / (m_maxTime - m_minTime) + rect.left;
	int y = rect.Height() * (idxYY + 1) / (num + 1) + rect.top;

	CPen *pOldPen = NULL;
	CPen pen1(PS_SOLID, 1, RGB(0, 255, 255));
	CPen pen2(PS_SOLID, 1, RGB(255, 0, 255));
	int state = m_pSyr->getYYerLrcState(idxYY, idxLrc);
	if (state == 0)
	{
		pOldPen = pDC->SelectObject(&pen1);
	}
	else if (state == 1)
	{
		pOldPen = pDC->SelectObject(&pen2);
	}

	int l = x - 2;
	int r = x + 2;
	int t = y - 5;
	int b = y + 5;
	if (l < rect.left)			l = rect.left;
	if (r > rect.right-1)		r = rect.right-1;
	if (t < rect.top+1)			t = rect.top+1;
	if (b > rect.bottom-1)		b = rect.bottom-1;
	pDC->MoveTo(l, t);
	pDC->LineTo(r, t);
	pDC->LineTo(r, b);
	pDC->LineTo(l, b);
	pDC->LineTo(l, t-1);

	t = rect.top - TIME_BREV_H + 3;
	b = t + 8;
	pDC->MoveTo(x, t);
	pDC->LineTo(x, b);

	pDC->SelectObject(pOldPen);
}

void CTimeAxis::drawTimePoints(CDC* pDC)
{
	if (pDC == NULL)
		return;

	if (m_pSyr == NULL)
		return;

	int num = m_pSyr->getYYerCount();
	if (num < 1)
		return;

	for (int i=0; i<num; i++)
	{
		int numLrc = m_pSyr->getYYerLrcsCount(i);
		if (numLrc < 1)
			continue;

		for (int j=0; j<numLrc; j++)
		{
			drawTimePoint(pDC, i, j/*, RGB(0, 255, 255)*/);
		}
	}
}

void CTimeAxis::drawTimePtSel(CDC* pDC)
{
	if (pDC == NULL)
		return;

	if (m_pSyr == NULL)
		return;

	int num = m_pSyr->getYYerCount();
	if (num < 1)
		return;
}

void CTimeAxis::drawCurTime(CDC* pDC)
{
	if (pDC == NULL)
		return;

	if (m_pSyr == NULL)
		return;

	int num = m_pSyr->getYYerCount();
	if (num < 1)
		return;

	if (m_iCurTime < m_minTime || m_iCurTime > m_maxTime || m_minTime >= m_maxTime)
		return;

	CRect rect;
	this->GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.top += 20;
	int x = rect.Width() * (m_iCurTime - m_minTime) / (m_maxTime - m_minTime);

	CPen pen(PS_DOT, 1, RGB(255, 0, 255));
	CPen *pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(x, rect.top);
	pDC->LineTo(x, rect.bottom);
	pDC->SelectObject(pOldPen);
}

void CTimeAxis::drawTimeBrev(CDC* pDC)
{
	if (pDC == NULL)
		return;

	if (m_pSyr == NULL)
		return;

	int num = m_pSyr->getYYerCount();
	if (num < 1)
		return;

	CRect rect;
	this->GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.top += 4;
	rect.bottom = rect.top + 6;
	CPen pen(PS_SOLID, 3, RGB(0, 255, 255));
	CPen pen1(PS_SOLID, 1, RGB(255, 0, 0));
	CPen *pOldPen = NULL;
	int maxTime = 2*m_pSyr->getMaxTime();
	if (maxTime >= m_allTime)
	{
// 		pOldPen = pDC->SelectObject(&pen);
// 		pDC->MoveTo(rect.left, rect.top + 3);
// 		pDC->LineTo(rect.right-1, rect.top + 3);

		CRect rect1;
		rect1.left = rect.Width() * m_minTime / maxTime + rect.left;
		rect1.right = rect.Width() * m_maxTime / maxTime + rect.left;
		rect1.top = rect.top;
		rect1.bottom = rect.bottom;
		pDC->SelectObject(&pen1);
// 		pDC->Rectangle(&rect1);
		pDC->MoveTo(rect1.left, rect1.top);
		pDC->LineTo(rect1.left, rect1.bottom-1);
		pDC->LineTo(rect1.right-1, rect1.bottom-1);
		pDC->LineTo(rect1.right-1, rect1.top);
		pDC->LineTo(rect1.left, rect1.top);
		pDC->SelectObject(pOldPen);
	}
	else if (maxTime > m_minTime)
	{
		CRect rect1(rect);
		rect1.left = rect.Width() * m_minTime / m_maxTime + rect.left;
		rect1.right = rect.Width() * maxTime / m_maxTime + rect.left;

// 		pOldPen = pDC->SelectObject(&pen);
// 		pDC->MoveTo(rect1.left, rect1.top + 3);
// 		pDC->LineTo(rect1.right-1, rect1.top + 3);

		pDC->SelectObject(&pen1);
// 		pDC->Rectangle(&rect);
		pDC->MoveTo(rect.left, rect.top);
		pDC->LineTo(rect.left, rect.bottom-1);
		pDC->LineTo(rect.right-1, rect.bottom-1);
		pDC->LineTo(rect.right-1, rect.top);
		pDC->LineTo(rect.left, rect.top);
		pDC->SelectObject(pOldPen);
	}
// 	Invalidate();
}

void CTimeAxis::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 在此处添加消息处理程序代码
}

void CTimeAxis::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_pSyr == NULL)
		return;

	int num = m_pSyr->getYYerCount();
	if (num < 1)
		return;

	CRect rect;
	this->GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.top += TIME_BREV_H;
// 	ScreenToClient(&point);

	if (rect.PtInRect(point))	// 时间轴区
	{
// 		int num = m_pSyr->getYYerCount();
		int dy = rect.Height() / (num + 1);

		int idxYY = 0;
		for (; idxYY<num; idxYY++)
		{
			if (point.y > dy*(idxYY+1)+rect.top-5 && point.y < dy*(idxYY+1)+rect.top+5)
				break;
		}
		if (idxYY == num)
			return;

		int cn = m_pSyr->getYYerLrcsCount(idxYY);
		int idxLrc = 0;
		for (; idxLrc<cn; idxLrc++)
		{
			int x = rect.Width() * (m_pSyr->getYYerLrcTime(idxYY, idxLrc) - m_minTime) / (m_maxTime - m_minTime);
			if (point.x > x-8 && point.x < x+8)
				break;
		}
		if (idxLrc == cn)	// 添加新时间点
		{
			int time = (m_maxTime - m_minTime) * point.x / rect.Width() + m_minTime;

			CtimeSetDlg dlg;
			dlg.setTime(time);
			if (dlg.DoModal() != IDOK)
				return;

			time = dlg.getTime();
			m_pSyr->addYYerTime(idxYY, time);

			Invalidate();
		}
		else	// 修改时间点
		{
			int time = m_pSyr->getYYerLrcTime(idxYY, idxLrc);

			CtimeSetDlg dlg;
			dlg.setTime(time);
			if (dlg.DoModal() != IDOK)
				return;

			time = dlg.getTime();
// 			m_pSyr->addYYerTime(idxYY, time);
			m_pSyr->editTimeLrc(idxYY, idxLrc, time);

			Invalidate();
		}
	}
	else	// 导航栏
	{
		int find = 0;
		int idxYY = 0;
		for (; idxYY<num; idxYY++)
		{
			int cn = m_pSyr->getYYerLrcsCount(idxYY);
			int idxLrc = 0;
			for (; idxLrc<cn; idxLrc++)
			{
				int x = rect.Width() * (m_pSyr->getYYerLrcTime(idxYY, idxLrc) - m_minTime) / (m_maxTime - m_minTime);
				if (point.x > x-5 && point.x < x+5)
				{
					find = m_pSyr->getYYerLrcTime(idxYY, idxLrc);
					break;
				}
			}
		}
		if (find == 0)
		{
			int maxTime = 2*m_pSyr->getMaxTime();
			// 		if (maxTime < 1)
			// 			return;

// 			int time = 0;
// 			if (m_allTime < maxTime/2)
// 			{
// 				time = maxTime * point.x / rect.Width();
// 			}
// 			else
// 			{
// 				time = m_allTime * point.x / rect.Width();
// 			}
			int time = (m_maxTime - m_minTime) * point.x / rect.Width() + m_minTime;

			CtimeSetDlg dlg;
			dlg.setTime(time);
			if (dlg.DoModal() != IDOK)
				return;

			time = dlg.getTime();

			m_pSyr->addYYersTime(time);

			Invalidate();
		}
		else
		{
			CtimeSetDlg dlg;
			dlg.setTime(find);
			if (dlg.DoModal() != IDOK)
				return;

			int time = dlg.getTime();

			m_pSyr->editYYersTime(find, time);

			Invalidate();
		}
	}

	// 	CStatic::OnLButtonDblClk(nFlags, point);
}

void CTimeAxis::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pSyr == NULL)
		return;

	int num = m_pSyr->getYYerCount();
	if (num < 1)
		return;

	m_bLBtnDown = TRUE;

// 	if (!m_bCtrl)
// 		m_pSyr->setTimesState(0);

	CRect rect;
	this->GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.top += TIME_BREV_H;
// 	ScreenToClient(&point);

	if (rect.PtInRect(point))
	{
// 		int num = m_pSyr->getYYerCount();
		int dy = rect.Height() / (num + 1);

		int idxYY = 0;
		for (; idxYY<num; idxYY++)
		{
			if (point.y > dy*(idxYY+1)+rect.top-5 && point.y < dy*(idxYY+1)+rect.top+5)
				break;
		}
		if (idxYY < num)
		{
			int cn = m_pSyr->getYYerLrcsCount(idxYY);
			int idxLrc = 0;
			for (; idxLrc<cn; idxLrc++)
			{
				int x = rect.Width() * (m_pSyr->getYYerLrcTime(idxYY, idxLrc) - m_minTime) / (m_maxTime - m_minTime);
				if (point.x > x-8 && point.x < x+8)
					break;
			}
			if (idxLrc < cn)
			{
				m_bTimePick = TRUE;
				m_ptTime = point;
				return;
			}
		}

		if (!m_bCtrl)
		{
			m_pSyr->setTimesState(0);
			Invalidate();
		}
	}
	else
	{
		int maxTime = 2*m_pSyr->getMaxTime();
		if (maxTime < 1)
			return;
		CRect re(rect);
		re.top = 3;
		re.bottom = re.top + 8;
		re.left = rect.Width() * m_minTime / maxTime + rect.left;
		re.right = rect.Width() * m_maxTime / maxTime + rect.left;
		if (re.PtInRect(point))
		{
			m_bBrevPick = TRUE;
			SetCapture();
			m_iBrevX = point.x;
		}
		m_ptTime = point;
	}

	// 	CStatic::OnLButtonDown(nFlags, point);
}

void CTimeAxis::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_pSyr == NULL)
		return;

	int num = m_pSyr->getYYerCount();
	if (num < 1)
		return;

	m_bLBtnDown = FALSE;
	m_bBrevPick = FALSE;
	m_bTimePick = FALSE;
	ReleaseCapture();

	CRect rect;
	this->GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.top += TIME_BREV_H;
// 	ScreenToClient(&point);

	if (rect.PtInRect(point))
	{
		CSize sz = point - m_ptTime;
		if ((sz.cx < 3 && sz.cx > -3)
			&& (sz.cy < 3 && sz.cy > -3))	// 单击
		{
			if (!m_bCtrl)
				m_pSyr->setTimesState(0);

			int dy = rect.Height() / (num + 1);

			int idxYY = 0;
			for (; idxYY<num; idxYY++)
			{
				if (point.y > dy*(idxYY+1)+rect.top-5 && point.y < dy*(idxYY+1)+rect.top+5)
					break;
			}
			if (idxYY == num)
			{
				Invalidate();
				return;
			}

			int cn = m_pSyr->getYYerLrcsCount(idxYY);
			int idxLrc = 0;
			for (; idxLrc<cn; idxLrc++)
			{
				int x = rect.Width() * (m_pSyr->getYYerLrcTime(idxYY, idxLrc) - m_minTime) / (m_maxTime - m_minTime);
				if (point.x > x-8 && point.x < x+8)
					break;
			}
			if (idxLrc == cn)
			{
				return;
				Invalidate();
			}

			m_pSyr->setTimeStateByIdx(idxYY, idxLrc, 1);

			Invalidate();
		}
		else		// 拖动
		{
		}
	}
	else
	{
// 		int maxTime = 2*m_pSyr->getMaxTime();
// 		if (maxTime < 1)
// 			return;
// 		CRect re(rect);
// 		re.top = 3;
// 		re.bottom = re.top + 8;
// 		re.left = rect.Width() * m_minTime / maxTime + rect.left;
// 		re.right = rect.Width() * m_maxTime / maxTime + rect.left;
// 		if (re.PtInRect(point))
// 		{
// 			m_bBrevPick = TRUE;
// 			SetCapture();
// 			m_iBrevX = point.x;
// 		}

		CSize sz = point - m_ptTime;
		if ((sz.cx < 3 && sz.cx > -3)
			&& (sz.cy < 3 && sz.cy > -3))	// 单击
		{
			int find = 0;
			int idxYY = 0;
			for (; idxYY<num; idxYY++)
			{
				int cn = m_pSyr->getYYerLrcsCount(idxYY);
				int idxLrc = 0;
				for (; idxLrc<cn; idxLrc++)
				{
					int tmp = m_pSyr->getYYerLrcTime(idxYY, idxLrc);
					int x = rect.Width() * (tmp - m_minTime) / (m_maxTime - m_minTime);
					if (point.x > x-5 && point.x < x+5)
					{
						find = tmp;
						break;
					}
				}
				if (find != 0)
					break;
			}

			if (!m_bCtrl)
				m_pSyr->setTimesState(0);

			if (find != 0)
			{
				for (; idxYY<num; idxYY++)
					m_pSyr->setTimeState(idxYY, find, 1);
				Invalidate();
			}

// 			int dy = rect.Height() / (num + 1);
// 
// 			int idxYY = 0;
// 			for (; idxYY<num; idxYY++)
// 			{
// 				if (point.y > dy*(idxYY+1)+rect.top-5 && point.y < dy*(idxYY+1)+rect.top+5)
// 					break;
// 			}
// 			if (idxYY == num)
// 			{
// 				Invalidate();
// 				return;
// 			}
// 
// 			int cn = m_pSyr->getYYerLrcsCount(idxYY);
// 			int idxLrc = 0;
// 			for (; idxLrc<cn; idxLrc++)
// 			{
// 				int x = rect.Width() * (m_pSyr->getYYerLrcTime(idxYY, idxLrc) - m_minTime) / (m_maxTime - m_minTime);
// 				if (point.x > x-8 && point.x < x+8)
// 					break;
// 			}
// 			if (idxLrc == cn)
// 			{
// 				return;
// 				Invalidate();
// 			}
// 
// 			m_pSyr->setTimeState(idxYY, idxLrc, 1);

			Invalidate();
		}
		else		// 拖动
		{
		}
	}

	// 	CStatic::OnLButtonUp(nFlags, point);
}

void CTimeAxis::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// 	CStatic::OnRButtonDblClk(nFlags, point);
}

void CTimeAxis::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_pSyr == NULL)
		return;

	int num = m_pSyr->getYYerCount();
	if (num < 1)
		return;

	m_bRBtnDown = TRUE;
	SetCapture();
// 	ScreenToClient(&point);
	m_iBrevX = point.x;
	m_ptTimeR = point;

	CRect rect;
	this->GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.top += TIME_BREV_H;
// 	ScreenToClient(&point);
	m_iTimeClick = (m_maxTime - m_minTime) * point.x / rect.Width() + m_minTime;

	// 	CStatic::OnRButtonDown(nFlags, point);
}

void CTimeAxis::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_bRBtnDown = FALSE;
	ReleaseCapture();

	CSize sz = point - m_ptTimeR;
	if ((sz.cx < 3 && sz.cx > -3)
		&& (sz.cy < 3 && sz.cy > -3))	// 单击
	{
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU_TIMEAXIS));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		ClientToScreen(&point);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	}

	// 	CStatic::OnRButtonUp(nFlags, point);
}

void CTimeAxis::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_pSyr == NULL)
		return;

	int num = m_pSyr->getYYerCount();
	if (num < 1)
		return;

	if (m_bRBtnDown)
	{
		CRect rect;
		this->GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.top += TIME_BREV_H;
// 		ScreenToClient(&point);
		int time = (m_maxTime - m_minTime) * (point.x - m_iBrevX) / rect.Width();
		m_iBrevX = point.x;
		m_minTime -= time;
		m_maxTime -= time;
// 		int time = (m_maxTime - m_minTime) * point.x / rect.Width() + m_minTime;
// 		m_minTime -= time - m_iTimeClick;
// 		m_maxTime -= time - m_iTimeClick;
// 		m_iTimeClick = time;
		if (m_minTime < 0)
		{
// 			m_iTimeClick -= m_minTime;
			m_maxTime -= m_minTime;
			m_minTime = 0;
		}
		int maxTime = m_pSyr->getMaxTime();
		if (maxTime > 0 && m_maxTime > 2*maxTime)
		{
// 			m_iTimeClick -= m_maxTime - 2*maxTime;
			m_minTime -= m_maxTime - 2*maxTime;
			m_maxTime = 2*maxTime;
		}

		Invalidate();
	}

	if ( m_bBrevPick )
	{
		CRect rect;
		this->GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.top += TIME_BREV_H;
// 		ScreenToClient(&point);
		int maxTime = m_pSyr->getMaxTime();
		int time = 2*maxTime * (point.x - m_iBrevX) / rect.Width();
		m_iBrevX = point.x;

		m_minTime += time;
		m_maxTime += time;
		if (m_minTime < 0)
		{
			m_maxTime -= m_minTime;
			m_minTime = 0;
		}
		if (maxTime > 0 && m_maxTime > 2*maxTime)
		{
			m_minTime -= m_maxTime - 2*maxTime;
			m_maxTime = 2*maxTime;
		}

		Invalidate();
	}

	// 	CStatic::OnMouseMove(nFlags, point);
}

BOOL CTimeAxis::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (m_pSyr == NULL)
		return FALSE;

	int num = m_pSyr->getYYerCount();
	if (num < 1)
		return FALSE;

	int maxTime = m_pSyr->getMaxTime();
	if (maxTime < 1000)
		return FALSE;

	CRect rect;
	this->GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.top += TIME_BREV_H;
// 	ScreenToClient(&pt);
	int time = m_allTime * pt.x / rect.Width() + m_minTime;
	if (zDelta > 0)
		m_allTime = int(m_allTime / 1.2);
	else
		m_allTime = int(m_allTime * 1.2);
	if (m_allTime < 1000)
		m_allTime = 1000;
	if (m_allTime > 2*maxTime)
	{
		m_allTime = 2*maxTime;
	}
	m_minTime = (int)(time - m_allTime * pt.x / rect.Width());
	if (m_minTime < 0)
	{
		m_minTime = 0;
	}
	m_maxTime = (int)(m_minTime + m_allTime);
	m_allTime = m_maxTime - m_minTime;
	if (m_maxTime < 1000)
	{
		m_maxTime = 2*maxTime;
		m_minTime = (int)(m_maxTime - m_allTime);
	}
	else if (m_maxTime > 2*maxTime)
	{
		m_maxTime = 2*maxTime;
		m_minTime = (int)(m_maxTime - m_allTime);
	}

	Invalidate();

	// 	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
	return TRUE;
}

void CTimeAxis::init()
{
	CRect rect;
	GetWindowRect(&rect);
	m_dw = rect.Width();
	m_dh = rect.Height();

	creatMemDC();
}

void CTimeAxis::creatMemDC()
{
	m_pMemDC = new CDC;	m_pMemBM = new CBitmap;
	if (m_pMemDC != 0 && m_pMemBM != 0)
	{
		CDC *pDC = GetDC();
		if (pDC == 0)
		{
			delete m_pMemDC;	delete m_pMemBM;
			m_pMemDC = 0;	m_pMemBM = 0;
		}
		else
		{
			long nIni = 1;
			if (m_pMemDC->CreateCompatibleDC(pDC))
			{
				if (m_pMemBM->CreateCompatibleBitmap(pDC, m_dw, m_dh))
				{
					if (m_pMemDC->SelectObject(m_pMemBM))
						::SetStretchBltMode(m_pMemDC->m_hDC, COLORONCOLOR);
					else
						nIni = 0;
				}
				else
					nIni = 0;
			}
			else
				nIni = 0;

			if (!nIni)
			{
				delete m_pMemDC;	delete m_pMemBM;
				m_pMemDC = 0;	m_pMemBM = 0;
			}

			ReleaseDC(pDC);
		}
	}
	else
	{
		if (m_pMemDC != 0)	delete m_pMemDC;
		if (m_pMemBM != 0)	delete m_pMemBM;
		m_pMemDC = 0;		m_pMemBM = 0;
	}
}

void CTimeAxis::setSyr(CStyler* syr)
{
	m_pSyr = syr;
}

void CTimeAxis::reset()
{
	m_allTime = m_pSyr->getMaxTime();
	if (m_allTime == 0)
		m_allTime = 300000;
	m_minTime = 0;
	m_maxTime = m_allTime;
// 	m_zoom = 1.0;
}

int& CTimeAxis::getCurTime()
{
	return m_iCurTime;
}

void CTimeAxis::delSelectTime()
{
	m_pSyr->delYYerTimes();

	int maxTime = 2*m_pSyr->getMaxTime();
	if (maxTime > 0 && m_maxTime > maxTime)
	{
		m_minTime -= m_maxTime - 2*maxTime;
		m_maxTime = 2*maxTime;
	}
}

void CTimeAxis::setCtrlState(BOOL bCtrl)
{
	m_bCtrl = bCtrl;
}
