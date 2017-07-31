// tankDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tank.h"
#include "tankDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern void TankMove(CTankDlg *pl);
extern void KeyProc(CTankDlg *pl);
extern void ClearMsgQueue();
extern CCriticalSection MapMutex;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

//##ModelId=3EBA49A4009C
class CAboutDlg : public CDialog
{
public:
	//##ModelId=3EBA49A400A0
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	//##ModelId=3EBA49A4009E
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTankDlg dialog

CTankDlg::CTankDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTankDlg::IDD, pParent),m_bPause(FALSE),m_bPaint(TRUE)
{
	//{{AFX_DATA_INIT(CTankDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nLevel=0;
	m_bEscPress=FALSE;
	if(!m_pictureTop.Load("pic\\bmp\\MainDlg\\top_Dlg.JPG"))
		AfxMessageBox("读取图源出错");
	if(!m_pictureLeft.Load("pic\\bmp\\MainDlg\\left_Dlg.JPG"))
		AfxMessageBox("读取图源出错");
	if(!m_pictureRight.Load("pic\\bmp\\MainDlg\\right_Dlg.JPG"))
		AfxMessageBox("读取图源出错");
	if(!m_pictureBloodFull.Load("pic\\bmp\\MainDlg\\BloodFull.JPG"))
		AfxMessageBox("读取图源出错");
	if(!m_pictureBloodEmpty.Load("pic\\bmp\\MainDlg\\BloodEmpty.JPG"))
		AfxMessageBox("读取图源出错");
}

void CTankDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTankDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTankDlg, CDialog)
	//{{AFX_MSG_MAP(CTankDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_MOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USERQUIT,Exit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTankDlg message handlers

BOOL CTankDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetWindowPos(&CWnd::wndTop ,0,0,757,678,SWP_DRAWFRAME);
	CenterWindow();
	
	if(game.Init(GetSafeHwnd())==FALSE)
	{
		AfxMessageBox("游戏初始化失败!");
		return FALSE;
	}
	hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)KeyProc,this,0 ,&ID);
	SetTimer(1,20,NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTankDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTankDlg::OnPaint() 
{

	if(m_bPaint)
	{
		CClientDC dc(this);
		m_pictureTop.Draw(&dc,CPoint(0,0));
        m_pictureTop.Draw(&dc,CPoint(0,645));
        m_pictureLeft.Draw(&dc,CPoint(0,0));
		m_pictureLeft.Draw(&dc,CPoint(645,0));
		m_pictureRight.Draw(&dc,CPoint(650,0));
		m_pictureLeft.Draw(&dc,CPoint(742,0));

		m_pictureBloodFull.Draw(GetDC(),CPoint(650,44));
	    m_pictureBloodFull.Draw(GetDC(),CPoint(650,370)); 
		
		int i=game.m_nLeftPlayer%10;
		int j=game.m_nRightPlayer%10;

		m_pictureBMath.Load("pic\\Bmp\\MainDlg\\Math\\B0.JPG");
		m_pictureBMath.Draw(&dc,CPoint(700,10));
		m_pictureBMath.Draw(&dc,CPoint(700,335));
		CString temp;
		temp.Format("pic\\Bmp\\MainDlg\\Math\\B%d.JPG",i);
		m_pictureBMath.Load(temp);
		m_pictureBMath.Draw(&dc,CPoint(720,10));
		temp.Format("pic\\Bmp\\MainDlg\\Math\\B%d.JPG",j);
		m_pictureBMath.Load(temp);
		m_pictureBMath.Draw(&dc,CPoint(720,335));
		if(game.m_tankleft)
		{
	  //重绘坦克生命值
	    	int i=game.m_tankleft->GetTankLife();
			m_pictureBloodEmpty.Draw(GetDC(),CPoint(650,44));
			m_pictureBloodFull.Draw(GetDC(),CPoint(650,44),(double(i)/200.00));
	 //重绘坦克杀敌数
			int m=game.m_nLeftKills;
			CString temp;
			int pos=m%10;
			if(pos!=0)
			{
			  temp.Format("pic\\Bmp\\MainDlg\\Math\\B%d.JPG",pos);
			  m_pictureBMath.Load(temp);
			  m_pictureBMath.Draw(GetDC(),CPoint(708,200));
			}
			pos=m/100;
			if(pos!=0)
			{
			  temp.Format("pic\\Bmp\\MainDlg\\Math\\B%d.JPG",pos);
			  m_pictureBMath.Load(temp);
			  m_pictureBMath.Draw(GetDC(),CPoint(666,200));	
			}
			m-=pos*100;
			pos=m/10;
			if(pos!=0)
			{
			  temp.Format("pic\\Bmp\\MainDlg\\Math\\B%d.JPG",pos);
			  m_pictureBMath.Load(temp);
			  m_pictureBMath.Draw(GetDC(),CPoint(687,200));
			}
		//绘制坦克得分
		  int k=game.m_nLeftScore;
		  pos=k%10;
		  if(pos>=0)
		  {
			temp.Format("pic\\Bmp\\MainDlg\\Math\\L%d.JPG",pos);
			m_pictureLMath.Load(temp);
			m_pictureLMath.Draw(GetDC(),CPoint(727,285));
		  }
		  pos=k/1000;
		  if(pos>=0)
		  {
			temp.Format("pic\\Bmp\\MainDlg\\Math\\L%d.JPG",pos);
			m_pictureLMath.Load(temp);
			m_pictureLMath.Draw(GetDC(),CPoint(651,285));
		  }	
		  k-=pos*1000;
		  pos=k/100;
		  if(pos>=0)
		  {
			temp.Format("pic\\Bmp\\MainDlg\\Math\\L%d.JPG",pos);
			m_pictureLMath.Load(temp);
			m_pictureLMath.Draw(GetDC(),CPoint(677,285));
		  }
		  k-=pos*100;
		  pos=k/10;
		  if(pos>=0)
		  {
			temp.Format("pic\\Bmp\\MainDlg\\Math\\L%d.JPG",pos);
			m_pictureLMath.Load(temp);
			m_pictureLMath.Draw(GetDC(),CPoint(702,285));
		  }

	  }
		if(!game.m_bSinglePlayer&&game.m_tankright)
		{
			int j=game.m_tankright->GetTankLife();
			m_pictureBloodEmpty.Draw(GetDC(),CPoint(650,370));
			m_pictureBloodFull.Draw(GetDC(),CPoint(650,370),(double(j)/200.00));
			int n=game.m_nRightKills;
			  CString temp;
			  int pos=n%10;
			  if(pos>=0)
			  {
				 temp.Format("pic\\Bmp\\MainDlg\\Math\\B%d.JPG",pos);
				 m_pictureBMath.Load(temp);
				 m_pictureBMath.Draw(GetDC(),CPoint(706,528));
			  }
			  pos=n/100;
			  if(pos>=0)
			  {
				 temp.Format("pic\\Bmp\\MainDlg\\Math\\B%d.JPG",pos);
				 m_pictureBMath.Load(temp);
				 m_pictureBMath.Draw(GetDC(),CPoint(664,528));	
			  }
              n-=pos*100;
			  pos=n/10;
			  if(pos>=0)
			  {
				 temp.Format("pic\\Bmp\\MainDlg\\Math\\B%d.JPG",pos);
				 m_pictureBMath.Load(temp);
				 m_pictureBMath.Draw(GetDC(),CPoint(685,528));
			  }

			  int h=game.m_nRightScore;
			  pos=h%10;
			  if(pos>=0)
			  {
				temp.Format("pic\\Bmp\\MainDlg\\Math\\L%d.JPG",pos);
				m_pictureLMath.Load(temp);
				m_pictureLMath.Draw(GetDC(),CPoint(727,613));
			  }
  			  pos=h/1000;
			  if(pos>=0)
			  {
				temp.Format("pic\\Bmp\\MainDlg\\Math\\L%d.JPG",pos);
				m_pictureLMath.Load(temp);
				m_pictureLMath.Draw(GetDC(),CPoint(651,613));
			  }
			  h-=pos*1000;
			  pos=h/100;
			  if(pos>=0)
			  {
				temp.Format("pic\\Bmp\\MainDlg\\Math\\L%d.JPG",pos);
				m_pictureLMath.Load(temp);
				m_pictureLMath.Draw(GetDC(),CPoint(677,613));
			  }
              h-=pos*100;
			  pos=h/10;
			  if(pos>=0)
			  {
				temp.Format("pic\\Bmp\\MainDlg\\Math\\L%d.JPG",pos);
				m_pictureLMath.Load(temp);
				m_pictureLMath.Draw(GetDC(),CPoint(702,613));
			  }
			}
//专门针对单人情况的刷新问题
		if(game.m_bSinglePlayer)
		{
		  CString temp;
		  temp.Format("pic\\Bmp\\MainDlg\\Math\\B%d.JPG",0);
		  m_pictureBMath.Load(temp);
		  m_pictureBMath.Draw(GetDC(),CPoint(720,335));
		}
		game.UpdateScreen();
		m_bPaint=FALSE;
	}
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTankDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTankDlg::OnTimer(UINT nIDEvent) 
{
	static int nNum=0;
    if(m_bPause)
	{
		if(nIDEvent==1)return;
		if(nIDEvent==2)return;
		if(nIDEvent==3)return;
	}
	GameState gstate=game.GetGameState();
    switch(nIDEvent)	
	{
	case 1:
		game.UpdateScreen();
		game.Run();
		//负责刷新dlg-right框架
		if(gstate==GAMEACTIVE||gstate==GAMENEXT)
		{
			ShowPlayerInfo();
		}

		if(gstate==GAMENEXT)
		{
			if(++m_nLevel <= GAMETOTALLEVEL)
			{
				char szMapFile[80];
				sprintf(szMapFile,"Map\\TankMap%d.map",m_nLevel);
				if(game.m_TankMap.LoadMapFromFile(szMapFile)==false)return;
//取消背景图
/*				if(game.m_pBackGroundBuffer)
				{
						game.m_pBackGroundBuffer->Release();
						game.m_pBackGroundBuffer=NULL;
				}
			sprintf(szMapFile,"pic\\bkd\\background%d.bmp",m_nLevel);
				game.m_pBackGroundBuffer = game.DDLoadBitmap(szMapFile);
					if(!game.m_pBackGroundBuffer)AfxMessageBox("载入背景图失败");
*/	
			    game.DrawLevel(m_nLevel);
				KillTimer(1);
				KillTimer(2);
				SetTimer(3,1000,NULL);
			}
			else
				game.SetGameState(GAMEWIN);
		}
		if(gstate==GAMEWIN)
		{
			game.UpdateScreen();
			KillTimer(2);
			KillTimer(3);
		}
		if(gstate==GAMEOVER)
		{
			game.UpdateScreen();
			KillTimer(2);
			KillTimer(3);
		}
		break;
	case 2:
		{
			CEgg * egg=new CEgg;
			if(egg)
			{
				egg->SetEggBmp(game.m_pEggBuffer);
				egg->RandomSet();
				game.m_Egg.AddTail(egg);
			}
		}
		break;
	case 3:
		if(nNum>3)
		{
			nNum=0;
			game.StartGame(m_nLevel);
			KillTimer(3);
			SetTimer(1,20,NULL);
			SetTimer(2,60000,NULL);
			return;
		}
		game.DrawWait(nNum);
		nNum++;	
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CTankDlg::OnOK() 
{
	
}

BOOL CTankDlg::PreTranslateMessage(MSG* pMsg) 
{
	static pausetime=0;
	if(::GetKeyState(VK_PAUSE)<0)
	{
		if(GetTickCount()-pausetime>=1000)
		{
			if(game.GetGameState()==GAMEACTIVE||game.GetGameState()==GAMEPAUSE)
			{
				if(m_bPause)m_bPause=FALSE;
				else m_bPause=TRUE;
				if(m_bPause)
				{
					game.SetGameState(GAMEPAUSE);
					SuspendThread(hThread);
				}
				else
				{
					game.SetGameState(GAMEACTIVE);
					ResumeThread(hThread);
				}	
			}
			pausetime=GetTickCount();
			return TRUE;
		}
		else return TRUE;
	}
	if(::GetKeyState(VK_ESCAPE)<0&&!m_bEscPress)
	{	
		m_OldState=game.GetGameState();
		if(m_OldState==GAMEBEGIN||m_OldState==GAMENEXT||m_OldState==GAMEPAUSE)return TRUE;
//运行时载入menu图片		
		game.m_pMenuBuffer=game.DDLoadBitmap("pic\\bmp\\menu.bmp");
		if(!game.m_pMenuBuffer){AfxMessageBox("载入菜单图片出错");return TRUE;}
		game.DDSetColorKey( game.m_pMenuBuffer, 0);

		m_bEscPress=TRUE;
		game.SetGameState(GAMEMENU);
		return TRUE;
	}
	if(::GetKeyState(VK_ESCAPE)<0&&m_bEscPress)
	{
//运行时销毁menu图片
		if(game.m_pMenuBuffer)
		{
			game.m_pMenuBuffer->Release();
			game.m_pMenuBuffer=NULL;
		}
		game.SetGameState(m_OldState);
		m_bEscPress=FALSE;
		return TRUE;
	}

	if(::GetKeyState(VK_RETURN)<0)
	{
		return TRUE;
	}

   if(pMsg->message==WM_CHAR)
	{
		CEvent event;
		event.ResetEvent();
		return TRUE;
	}
	 return CDialog::PreTranslateMessage(pMsg);
}

void CTankDlg::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	m_bPaint=TRUE;
	CDialog::OnWindowPosChanged(lpwndpos);
}


void CTankDlg::OnMove(int x, int y) 
{
	CRect rc;
	rc.left=0;
	rc.right=SCREEN_WIDTH;
	rc.top=0;
	rc.bottom=SCREEN_HEIGHT;
	ClientToScreen(&rc);
	rc.OffsetRect(5,5);
	game.SetWndRect(rc);
	m_bPaint=TRUE;
	CDialog::OnMove(x, y);
}

void CTankDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
    if(game.GetGameState()==GAMEBEGIN)
	{
		game.SetGameState(GAMENEXT);
		if(game.m_pBeginBuffer)
		{
			game.m_pBeginBuffer->Release();
			game.m_pBeginBuffer=NULL;
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CTankDlg::ShowPlayerInfo()
{
	static leftLife=0;
	static rightLife=0;
	static leftKill=0;
	static rightKill=0;
	static leftScore=0;
	static rightScore=0;
	static lefttanknumber=0;
	static righttanknumber=0;

	if(game.m_tankleft)
	{
		//绘制生命显示条
		  int i=game.m_tankleft->GetTankLife();
		  if(i!=leftLife)
		  {
			m_pictureBloodEmpty.Draw(GetDC(),CPoint(650,44));
			m_pictureBloodFull.Draw(GetDC(),CPoint(650,44),(double(i)/200.00));
			leftLife=i;
		  }
		  //绘制杀敌人坦克数
		  int m=game.m_nLeftKills;
		  if(m!=leftKill)
		  {
			 CString temp;
			 int pos=m%10;
			 if(pos>=0)
			 {
				 temp.Format("pic\\Bmp\\MainDlg\\Math\\B%d.JPG",pos);
				 m_pictureBMath.Load(temp);
				 m_pictureBMath.Draw(GetDC(),CPoint(708,200));
			 }
			 pos=m/100;
			 if(pos>=0)
			 {
				 temp.Format("pic\\Bmp\\MainDlg\\Math\\B%d.JPG",pos);
				 m_pictureBMath.Load(temp);
				 m_pictureBMath.Draw(GetDC(),CPoint(666,200));	
			 }
			 m-=pos*100;
			 pos=m/10;
			 if(pos>=0)
			 {
				temp.Format("pic\\Bmp\\MainDlg\\Math\\B%d.JPG",pos);
				 m_pictureBMath.Load(temp);
				 m_pictureBMath.Draw(GetDC(),CPoint(687,200));
			 }

			 leftKill=game.m_nLeftKills;
		  }
		  //绘制得分
		  int k=game.m_nLeftScore;
		  if(k!=leftScore)
		  {
			  CString temp;
			  int pos=k%10;
			  if(pos>=0)
			  {
				temp.Format("pic\\Bmp\\MainDlg\\Math\\L%d.JPG",pos);
				m_pictureLMath.Load(temp);
				m_pictureLMath.Draw(GetDC(),CPoint(727,285));
			  }
			  pos=k/1000;
			  if(pos>=0)
			  {
				temp.Format("pic\\Bmp\\MainDlg\\Math\\L%d.JPG",pos);
				m_pictureLMath.Load(temp);
				m_pictureLMath.Draw(GetDC(),CPoint(651,285));
			  }
			  k-=pos*1000;

			  pos=k/100;
			  if(pos>=0)
			  {
				temp.Format("pic\\Bmp\\MainDlg\\Math\\L%d.JPG",pos);
				m_pictureLMath.Load(temp);
				m_pictureLMath.Draw(GetDC(),CPoint(677,285));
			  }
			  k-=pos*100;

			  pos=k/10;
			  if(pos>=0)
			  {
				temp.Format("pic\\Bmp\\MainDlg\\Math\\L%d.JPG",pos);
				m_pictureLMath.Load(temp);
				m_pictureLMath.Draw(GetDC(),CPoint(702,285));
			  }
			  leftScore=game.m_nLeftScore;
		  }
		  //绘制坦克剩余量
		  int num=game.m_nLeftPlayer;
		  if(num!=lefttanknumber)
		  {
			num=num%10;
			CString temp;
			temp.Format("pic\\Bmp\\MainDlg\\Math\\B%d.JPG",num);
			m_pictureBMath.Load(temp);
			m_pictureBMath.Draw(GetDC(),CPoint(720,10));
			lefttanknumber=game.m_nLeftPlayer;
		 }	
	}
	if(!game.m_bSinglePlayer&&game.m_tankright)
	{
		int j=game.m_tankright->GetTankLife();
		if(j!=rightLife)
		{
			m_pictureBloodEmpty.Draw(GetDC(),CPoint(650,370));
			m_pictureBloodFull.Draw(GetDC(),CPoint(650,370),(double(j)/200.00));
			rightLife=j;
		}
	  int n=game.m_nRightKills;
	  if(n!=rightKill)
	  {
		 CString temp;
		 int pos=n%10;
		 if(pos>=0)
		 {
			 temp.Format("pic\\Bmp\\MainDlg\\Math\\B%d.JPG",pos);
			 m_pictureBMath.Load(temp);
			 m_pictureBMath.Draw(GetDC(),CPoint(706,528));
		 }
 		 pos=n/100;
		 if(pos>=0)
		 {
			 temp.Format("pic\\Bmp\\MainDlg\\Math\\B%d.JPG",pos);
			 m_pictureBMath.Load(temp);
			 m_pictureBMath.Draw(GetDC(),CPoint(664,528));	
		 }
		 n-=pos*100;

		 pos=n/10;
		 if(pos>=0)
		 {
			temp.Format("pic\\Bmp\\MainDlg\\Math\\B%d.JPG",pos);
			 m_pictureBMath.Load(temp);
			 m_pictureBMath.Draw(GetDC(),CPoint(685,528));
		 }
		 rightKill=game.m_nRightKills;
	  }
	  int h=game.m_nRightScore;
	  if(h!=rightScore)
	  {
		  CString temp;
		  int pos=h%10;
		  if(pos>=0)
		  {
			temp.Format("pic\\Bmp\\MainDlg\\Math\\L%d.JPG",pos);
			m_pictureLMath.Load(temp);
			m_pictureLMath.Draw(GetDC(),CPoint(727,613));
		  }
		  pos=h/1000;
		  if(pos>=0)
		  {
			temp.Format("pic\\Bmp\\MainDlg\\Math\\L%d.JPG",pos);
			m_pictureLMath.Load(temp);
			m_pictureLMath.Draw(GetDC(),CPoint(651,613));
		  }
		  h-=pos*1000;

  		  pos=h/100;
		  if(pos>=0)
		  {
			temp.Format("pic\\Bmp\\MainDlg\\Math\\L%d.JPG",pos);
			m_pictureLMath.Load(temp);
			m_pictureLMath.Draw(GetDC(),CPoint(677,613));
		  }
	      h-=pos*100;

		  pos=h/10;
		  if(pos>=0)
		  {
			temp.Format("pic\\Bmp\\MainDlg\\Math\\L%d.JPG",pos);
			m_pictureLMath.Load(temp);
			m_pictureLMath.Draw(GetDC(),CPoint(702,613));
		  }

		  rightScore=game.m_nRightScore;
	  }
	  int num=game.m_nRightPlayer;
	  if(num!=righttanknumber)
	  {
		  num=num%10;
		  CString temp;
		  temp.Format("pic\\Bmp\\MainDlg\\Math\\B%d.JPG",num);
		  m_pictureBMath.Load(temp);
		  m_pictureBMath.Draw(GetDC(),CPoint(720,335));
		  righttanknumber=game.m_nRightPlayer;
	  }
	}
	static BOOL flash=TRUE;
//专门针对单人情况刷新rightplayer,且只刷新一遍
	if(game.m_bSinglePlayer&&flash)
	{
		  flash=FALSE;
		  CString temp;
		  temp.Format("pic\\Bmp\\MainDlg\\Math\\B%d.JPG",0);
		  m_pictureBMath.Load(temp);
		  m_pictureBMath.Draw(GetDC(),CPoint(720,335));
	}
}

void CTankDlg::OnClose() 
{
	CDialog::OnClose();
}

void CTankDlg::Exit()
{
	SuspendThread(hThread);
	CDialog::OnCancel();
}

void CTankDlg::OnDestroy()
{
	CDialog::OnDestroy();
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();

}

void CTankDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnCancel();
}
