#if !defined(AFX_BEGINDLG_H__755F6EB3_2D39_46CC_B867_F94DBDB5EB84__INCLUDED_)
#define AFX_BEGINDLG_H__755F6EB3_2D39_46CC_B867_F94DBDB5EB84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BeginDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBeginDlg dialog
#include "Picture.h"
#include "MyButton.h"
//##ModelId=3EBA49A103B9
class CBeginDlg : public CDialog
{
// Construction
public:
	//##ModelId=3EBA49A20002
	CBeginDlg(CWnd* pParent = NULL);   // standard constructor
	//##ModelId=3EBA49A103D3
	CString m_str;
    CPicture m_PictureUp,m_PictureDown,m_PictureTitle;

// Dialog Data
	//{{AFX_DATA(CBeginDlg)
	enum { IDD = IDD_DIALOG_BEGIN };
	//##ModelId=3EBA49A103D0
	CMyButton	m_CANCEL;
	//##ModelId=3EBA49A103CB
	CMyButton	m_OK;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBeginDlg)
	public:
	//##ModelId=3EBA49A20000
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	//##ModelId=3EBA49A103DF
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBeginDlg)
	//##ModelId=3EBA49A103DE
	virtual void OnCancel();
	//##ModelId=3EBA49A103DC
	afx_msg UINT OnNcHitTest(CPoint point);
	//##ModelId=3EBA49A103DB
	virtual BOOL OnInitDialog();
	//##ModelId=3EBA49A103DA
	afx_msg void OnPaint();
	//##ModelId=3EBA49A103D8
	afx_msg void OnTimer(UINT nIDEvent);
	//##ModelId=3EBA49A103D4
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BEGINDLG_H__755F6EB3_2D39_46CC_B867_F94DBDB5EB84__INCLUDED_)
