// tank.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "tank.h"
#include "tankDlg.h"
#include "BeginDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCriticalSection  MapMutex;
/////////////////////////////////////////////////////////////////////////////
// CTankApp

BEGIN_MESSAGE_MAP(CTankApp, CWinApp)
	//{{AFX_MSG_MAP(CTankApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTankApp construction

CTankApp::CTankApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTankApp object

CTankApp theApp;

void KeyProc(CTankDlg *pl)
{	
	DWORD input_left=0;
	DWORD input_right=0;
	DWORD time_bullet=0;
	DWORD time_bullet2=0;
//副机时间控制
	DWORD time_up=0;
	DWORD time_down=0;
	DWORD time_left=0;
	DWORD time_right=0;
//主机时间控制
    DWORD main_up=0;
	DWORD main_down=0;
	DWORD main_left=0;
	DWORD main_right=0;
	while(1)
	{
		MapMutex.Lock();
		switch(pl->game.GetGameState())
		{
		case GAMEBEGIN:
			if(::GetKeyState(VK_LEFT)<0)
			{
				pl->game.m_bSinglePlayer=TRUE;

			}
			if(::GetKeyState(VK_RIGHT)<0)
			{
				pl->game.m_bSinglePlayer=FALSE;
			}
			if(::GetKeyState(VK_RETURN)<0)
			{//游戏开始
				pl->game.SetGameState(GAMENEXT);
				if(pl->game.m_pBeginBuffer)
				{
					pl->game.m_pBeginBuffer->Release();
					pl->game.m_pBeginBuffer=NULL;
				}					
				if(pl->game.m_bSinglePlayer)pl->game.m_nRightPlayer=0;
			}
			if(::GetKeyState(VK_ESCAPE)<0)
			{
				pl->game.SetGameState(GAMENEXT);
				if(pl->game.m_pBeginBuffer)
				{
					pl->game.m_pBeginBuffer->Release();
					pl->game.m_pBeginBuffer=NULL;
				}
			}
			break;
		case GAMEMENU:
			if(::GetKeyState(VK_UP)<0)
			{
				pl->game.m_nMenuPos-=1;
				if(pl->game.m_nMenuPos<1)pl->game.m_nMenuPos=1;
				Sleep(200);
			}
			if(::GetKeyState(VK_DOWN)<0)
			{
				pl->game.m_nMenuPos+=1;
				if(pl->game.m_nMenuPos>4)pl->game.m_nMenuPos=4;
				Sleep(200);
			}
			if(::GetKeyState(VK_RETURN)<0)
			{//游戏开始
				switch(	pl->game.m_nMenuPos)
				{
				case 1:
				 
				  pl->m_nLevel=0;
				  pl->game.DestroyObject();
				  pl->game.SetGameState(GAMENEXT);
				  pl->game.m_nLeftPlayer=3;
				  pl->game.m_nLeftKills=0;
				  pl->game.m_nLeftScore=0;

				  pl->game.m_nRightPlayer=3;
				  pl->game.m_nRightKills=0;
				  pl->game.m_nRightScore=0;
                  break;
				case 2:
				  pl->m_nLevel-=1;
				  pl->game.DestroyObject();
				  pl->game.SetGameState(GAMENEXT);
				  pl->game.m_nLeftPlayer=3;
				  pl->game.m_nLeftKills=0;
				  pl->game.m_nLeftScore=0;

				  pl->game.m_nRightPlayer=3;
				  pl->game.m_nRightKills=0;
				  pl->game.m_nRightScore=0;
				  break;
				case 3:
				  pl->game.SetGameState(pl->m_OldState);
				  break;
				case 4:
					pl->PostMessage(WM_QUIT,0,0);
					break;
				}
				pl->m_bEscPress=FALSE;
			}
			break;
		case GAMENEXT:
			break;
		case GAMEWIN:
			break;
		case GAMEACTIVE:
//用户被定时了
			if(pl->game.m_bFreeze)
			{
				Sleep(4000);
				pl->game.m_bFreeze=FALSE;
			}
/**************************************************************************/
 //副机控制				
			if(::GetKeyState(VK_UP)<0)
			{
				if(GetTickCount()-time_up>TEMPSPEED)
				{
					input_right+=VK_UP;
					time_up=GetTickCount(); 
				}
			}

			if(::GetKeyState(VK_DOWN)<0)
			{
				if(GetTickCount()-time_down>TEMPSPEED)
				{
				input_right+=VK_DOWN;
				time_down=GetTickCount();
				}
			}

			if(::GetKeyState(VK_LEFT)<0)
			{
				if(GetTickCount()-time_left>TEMPSPEED)
				{
				input_right+=VK_LEFT;
				time_left=GetTickCount();
				}
			}
			if(::GetKeyState(VK_RIGHT)<0)
			{
				if(GetTickCount()-time_right>TEMPSPEED)
				{
				input_right+=VK_RIGHT;
				time_right=GetTickCount();
				}
			}
		//副机开火
				//if(::GetKeyState(VK_NUMPAD0)<0)
				if(::GetKeyState(VK_SPACE)<0)
				//监控numpad1 ,开火
				{
				   if(pl->game.m_tankright && (GetTickCount()-time_bullet2) > UINT(pl->game.m_tankright->m_nFireSpeed))
				   {
					pl->game.m_tankright->Shoot();
					time_bullet2=GetTickCount();
				   }
				}
				if(::GetKeyState(VK_DECIMAL)<0)
				//监控numpad2,开火2
				{
				}

/**************************************************************************/
/**************************************************************************/
		//主机控制

				if(::GetKeyState(0x57)<0||::GetKeyState(0x77)<0)
				//监听w 和 W
				{
					if(GetTickCount()-main_up>TEMPSPEED)
					{
						input_left+=0x57;
						main_up=GetTickCount();
					}
				}
				if(::GetKeyState(0x53)<0||::GetKeyState(0x73)<0)
				//监控s和S
				{
					if(GetTickCount()-main_down>TEMPSPEED)
					{
					input_left+=0x53;
					main_down=GetTickCount();
					}
				}
				if(::GetKeyState(0x41)<0||::GetKeyState(0x61)<0)
				//监控a和A
				{
					if(GetTickCount()-main_left>TEMPSPEED)
					{
					input_left+=0x41;
					main_left=GetTickCount();
					}
				}
				if(::GetKeyState(0x44)<0||::GetKeyState(0x64)<0)
				//监控d和D
				{
					if(GetTickCount()-main_right>TEMPSPEED)
					{
						input_left+=0x44;
						main_right=GetTickCount();
					}
				}
				if(::GetKeyState(0x4A)<0||::GetKeyState(0x6A)<0)
				//监控j和J ,开火
				{
    				if(pl->game.m_tankleft && (GetTickCount()-time_bullet) > UINT(pl->game.m_tankleft->m_nFireSpeed))
					{
						pl->game.m_tankleft->Shoot();
					   time_bullet=GetTickCount();
					}
				}
				if(::GetKeyState(0x4B)<0||::GetKeyState(0x6B)<0)
				//监控k和K,开火2
				{

				}
/****************************************************************************/

				if(input_right==VK_UP)		
				{
					if(pl->game.m_tankright)
					{
						pl->game.m_tankright->TankMove2(UP);
					//	pl->game.m_wavMove.Play();
					}
				}

				if(input_right==VK_DOWN)		
				{
					if(pl->game.m_tankright)
					{
						pl->game.m_tankright->TankMove2(DOWN);
					//	pl->game.m_wavMove.Play();
					}
				}

				if(input_right==VK_LEFT)
				{
					if(pl->game.m_tankright)
					{
						pl->game.m_tankright->TankMove2(LEFT);
					//	pl->game.m_wavMove.Play();
					}
				}

				if(input_right==VK_RIGHT)
				{
					if(pl->game.m_tankright)
					{
						pl->game.m_tankright->TankMove2(RIGHT);
					//	pl->game.m_wavMove.Play();
					}
				}

				if(input_left==0x57)
				{
					if(pl->game.m_tankleft)
					{
						pl->game.m_tankleft->TankMove2(UP);
					//	pl->game.m_wavMove.Play();
					}
				}

				if(input_left==0x53)
				{
					if(pl->game.m_tankleft)
					{
						pl->game.m_tankleft->TankMove2(DOWN);
					//	pl->game.m_wavMove.Play();
					}
				}

				if(input_left==0x41)
				{
					if(pl->game.m_tankleft)
					{
						pl->game.m_tankleft->TankMove2(LEFT);
					//	pl->game.m_wavMove.Play();
					}
				}
 
				if(input_left==0x44)
				{
					if(pl->game.m_tankleft)
					{
						pl->game.m_tankleft->TankMove2(RIGHT);
					//	pl->game.m_wavMove.Play();
					}
				}
				input_left=0; 
				input_right=0;
				break;

		}
		MapMutex.Unlock();
		Sleep(10);
	}

}
	

/////////////////////////////////////////////////////////////////////////////
// CTankApp initialization

BOOL CTankApp::InitInstance()
{
	AfxEnableControlContainer();
    if(!AfxOleInit())return FALSE;
	// Standard initialization
    m_gExit = CreateMutex(NULL, FALSE, "SJDG");
	if(m_gExit == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(m_gExit);
		return FALSE;
	}

     CBeginDlg m_dlg;
	 if(m_dlg.DoModal()==IDCANCEL)return FALSE;

#ifdef _AFXDLL
	if(!Enable3dControls())return FALSE;			// Call this when using MFC in a shared DLL
#else
	if(!Enable3dControlsStatic())return FALSE;	// Call this when linking to MFC statically
#endif

	CTankDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
