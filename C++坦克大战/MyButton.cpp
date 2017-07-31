// MyButton.cpp : implementation file
//

#include "stdafx.h"
#include "tank.h"
#include "MyButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyButton

CMyButton::CMyButton()
{
	m_nWidth=0;
    m_nHeight=0;
	m_nButtonNum=3;
	m_IsPressDown=FALSE;
	m_InRect=FALSE;
}

CMyButton::~CMyButton()
{
	m_Bitmap.DeleteObject();
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
	//{{AFX_MSG_MAP(CMyButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyButton message handlers

void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    CDC dc;
    dc.Attach(lpDrawItemStruct->hDC );

	UINT state = lpDrawItemStruct->itemState ;

	if (state & ODS_FOCUS)
    {
		if (state & ODS_SELECTED)
        { 
            DrawBitmap( &dc, PRESSDOWN);
		}
		else
		{
			if (m_InRect)
				DrawBitmap( &dc, INRECT);
			else
				DrawBitmap( &dc, OUTRECT);
		}
	}
	else
	{
		if (m_InRect)
			DrawBitmap( &dc, INRECT );
		else
			DrawBitmap( &dc, OUTRECT );
    }	

    dc.Detach();
	
}

void CMyButton::DrawBitmap(CDC *pDC, State mode)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* old  = memDC.SelectObject(&m_Bitmap);
	pDC->BitBlt(0,0,m_nWidth, m_nHeight, &memDC, m_nWidth*mode, 0, SRCCOPY);
	memDC.SelectObject(old);
}

BOOL CMyButton::LoadBitmap(UINT id)
{
	if(!m_Bitmap.LoadBitmap(id))
	{
		MessageBox("ÔØÈë°´Å¥Í¼Æ¬³ö´í","´íÎó",MB_ICONINFORMATION);
		return FALSE;
	}
	
    BITMAP bmpinfo;
    m_Bitmap.GetBitmap(&bmpinfo);
    m_nWidth  = bmpinfo.bmWidth/m_nButtonNum;
    m_nHeight = bmpinfo.bmHeight;
	SetWindowPos(NULL,0, 0, m_nWidth, m_nHeight, SWP_NOMOVE|SWP_NOOWNERZORDER);
	return TRUE;
}

void CMyButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_IsPressDown=TRUE;
	Invalidate();
	CButton::OnLButtonDown(nFlags, point);
}

void CMyButton::OnMouseMove(UINT nFlags, CPoint point) 
{
    CRect rect;
	GetClientRect(&rect);
	if(rect.PtInRect(point))
	{
       if(!m_InRect)
	   {
			m_InRect=TRUE;
			Invalidate();
	   }
	   	if (GetCapture()!=this)
			SetCapture();
	}
	else 
	{
		if(m_InRect)
		{
			m_InRect=FALSE;
			Invalidate();
		}
		if(!m_IsPressDown)ReleaseCapture();
	}
	  
	CButton::OnMouseMove(nFlags, point);
}

BOOL CMyButton::Create(DWORD dwStyle, CPoint point, CWnd *pParentWnd, UINT nID, UINT mapid)
{
	dwStyle|=WS_CHILD|WS_VISIBLE|BS_OWNERDRAW;
	if (! CButton::Create(NULL, dwStyle,CRect( point.x, point.y, point.x, point.y ),pParentWnd, nID)) return false;	
	if (! LoadBitmap(mapid)) return FALSE;
	return TRUE;
}


BOOL CMyButton::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

BOOL CMyButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	::SetCursor(::AfxGetApp()->LoadCursor(IDC_HAND));
    return TRUE;
}

void CMyButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
    m_IsPressDown=FALSE;
	Invalidate();
	PostMessage(WM_SETCURSOR,0,0);
	CButton::OnLButtonUp(nFlags, point);
}
