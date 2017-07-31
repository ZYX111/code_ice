// tank.h : main header file for the TANK application
//

#if !defined(AFX_TANK_H__CBF43B42_338C_4379_8878_C84D6E993543__INCLUDED_)
#define AFX_TANK_H__CBF43B42_338C_4379_8878_C84D6E993543__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTankApp:
// See tank.cpp for the implementation of this class
//

//##ModelId=3EBA49A3031C
class CTankApp : public CWinApp
{
public:
	//##ModelId=3EBA49A3032F
	CTankApp();
	//##ModelId=3EBA49A3032D
    HANDLE m_gExit;  
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTankApp)
	public:
	//##ModelId=3EBA49A3032E
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTankApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TANK_H__CBF43B42_338C_4379_8878_C84D6E993543__INCLUDED_)
