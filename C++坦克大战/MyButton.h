#if !defined(AFX_MYBUTTON_H__F0FD2E58_D40D_449D_8FBF_6BC1CDE19448__INCLUDED_)
#define AFX_MYBUTTON_H__F0FD2E58_D40D_449D_8FBF_6BC1CDE19448__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyButton window
//##ModelId=3EBA49A4003E
class CMyButton : public CButton
{
// Construction
public:
	//##ModelId=3EBA49A40070
	CMyButton();

// Attributes
public:

// Operations
protected:
	//##ModelId=3EBA49A4004E
	BOOL	 m_IsPressDown;
	//##ModelId=3EBA49A40048
	BOOL     m_InRect;
	//##ModelId=3EBA49A40045
    CBitmap  m_Bitmap;
	//##ModelId=3EBA49A40042
    UINT	 m_nWidth;
	//##ModelId=3EBA49A40041
    UINT	 m_nHeight;
	//##ModelId=3EBA49A40040
	int      m_nButtonNum;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyButton)
	public:
	//##ModelId=3EBA49A4006E
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=3EBA49A40068
	BOOL Create(DWORD dwStyle, CPoint point, CWnd *pParentWnd, UINT nID, UINT mapid);
	//##ModelId=3EBA49A40066
	BOOL LoadBitmap(UINT id);
	//##ModelId=3EBA49A40063
	void DrawBitmap(CDC * pDC,State mode);
	//##ModelId=3EBA49A40062
	virtual ~CMyButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyButton)
	//##ModelId=3EBA49A4005F
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//##ModelId=3EBA49A40058
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//##ModelId=3EBA49A40056
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//##ModelId=3EBA49A40052
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//##ModelId=3EBA49A4004F
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBUTTON_H__F0FD2E58_D40D_449D_8FBF_6BC1CDE19448__INCLUDED_)
