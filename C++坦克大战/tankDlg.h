// tankDlg.h : header file
//

#if !defined(AFX_TANKDLG_H__588B85C8_AB07_4EB9_8810_5AF165BDEBC0__INCLUDED_)
#define AFX_TANKDLG_H__588B85C8_AB07_4EB9_8810_5AF165BDEBC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// CTankDlg dialog
#include "game.h"
#include "BeginDlg.h"
#include "Picture.h"
//##ModelId=3EBA49A30196
class CTankDlg : public CDialog
{
// Construction
public:
	//##ModelId=3EBA49A301E5
	void ShowPlayerInfo();
	//##ModelId=3EBA49A301B5
	HANDLE hThread;
	//##ModelId=3EBA49A301B1
	CGame game;
	//##ModelId=3EBA49A301AE
	int m_nLevel;
	//##ModelId=3EBA49A301AD
	bool m_bEscPress;
	//##ModelId=3EBA49A301AA
	GameState m_OldState;
	CPicture m_pictureTop,m_pictureLeft,m_pictureRight,m_pictureBloodFull,m_pictureBloodEmpty,m_pictureLMath,m_pictureBMath;
	//##ModelId=3EBA49A301A7
	BOOL m_bPaint;
	//##ModelId=3EBA49A301DD
	CTankDlg(CWnd* pParent = NULL);	// standard constructor
	
// Dialog Data
	//{{AFX_DATA(CTankDlg)
	enum { IDD = IDD_TANK_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTankDlg)
	public:
	//##ModelId=3EBA49A301DB
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	//##ModelId=3EBA49A301D9
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	

	//##ModelId=3EBA49A301A6
	HICON m_hIcon;
	//##ModelId=3EBA49A301A5
	BOOL m_bPause;
	//##ModelId=3EBA49A30199
	BOOL m_bChange;   //表示windows发生位置改变
//	CNormalBullet m_myBullet;
	//##ModelId=3EBA49A30198
	DWORD ID;


	// Generated message map functions
	//{{AFX_MSG(CTankDlg)
	//##ModelId=3EBA49A301D8
	virtual BOOL OnInitDialog();
	//##ModelId=3EBA49A301D5
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//##ModelId=3EBA49A301D4
	afx_msg void OnPaint();
	//##ModelId=3EBA49A301CE
	afx_msg HCURSOR OnQueryDragIcon();
	//##ModelId=3EBA49A301CC
	afx_msg void OnTimer(UINT nIDEvent);
	//##ModelId=3EBA49A301CB
	virtual void OnOK();
	//##ModelId=3EBA49A301C9
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//##ModelId=3EBA49A301C6
	afx_msg void OnMove(int x, int y);
	//##ModelId=3EBA49A301BA
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//##ModelId=3EBA49A301B9
	afx_msg void OnClose();
	//##ModelId=3EBA49A301B8
	afx_msg void OnDestroy();
	//##ModelId=3EBA49A301B7
	virtual void OnCancel();
	//}}AFX_MSG
	//##ModelId=3EBA49A301B6
	afx_msg void Exit();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TANKDLG_H__588B85C8_AB07_4EB9_8810_5AF165BDEBC0__INCLUDED_)
