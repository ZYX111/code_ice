// BeginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tank.h"
#include "BeginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBeginDlg dialog


CBeginDlg::CBeginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBeginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBeginDlg)
	//}}AFX_DATA_INIT
	if(!m_PictureDown.Load("pic\\Begin\\Down.JPG"))
		AfxMessageBox("读取图源出错");
	if(!m_PictureTitle.Load("pic\\Begin\\title.JPG"))
		AfxMessageBox("读取图源出错");
}


void CBeginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBeginDlg)
	DDX_Control(pDX, IDCANCEL, m_CANCEL);
	DDX_Control(pDX, IDOK, m_OK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBeginDlg, CDialog)
	//{{AFX_MSG_MAP(CBeginDlg)
	ON_WM_NCHITTEST()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBeginDlg message handlers

void CBeginDlg::OnCancel() 
{
	KillTimer(1);
	CDialog::OnCancel();
}

UINT CBeginDlg::OnNcHitTest(CPoint point) 
{
   	return HTCAPTION; 
}

BOOL CBeginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowText("SOS坦克大战");
	
	CRect rect;
	GetWindowRect(&rect);
	GetClientRect(&rect);
	rect.SetRect(0,0,340,405);
	
	SetWindowPos(&CWnd::wndTopMost,rect.left,rect.top,rect.Width(),rect.Height(),SWP_NOREDRAW );
	if(!m_PictureUp.Load("pic\\Begin\\Begin0.JPG"))
		AfxMessageBox("读取图源出错");
	CSize size=m_PictureUp.GetSize(GetDC());

	CenterWindow();

//按资源里面的位置定位

	m_OK.LoadBitmap(IDB_OK);
	m_CANCEL.LoadBitmap(IDB_CANCEL);


	m_PictureDown.Draw(GetDC(),CPoint(0,222));
	SetTimer(1,3000,NULL);



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBeginDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	m_PictureTitle.Draw(&dc,CPoint(0,0));
    m_PictureUp.Draw(&dc,CPoint(0,100));
	m_PictureDown.Draw(&dc,CPoint(0,322));
	// Do not call CDialog::OnPaint() for painting messages
}

void CBeginDlg::OnTimer(UINT nIDEvent) 
{
	static i=1;
   switch(nIDEvent)
   {
   case 1:

	   m_str.Format("pic\\Begin\\Begin%d.JPG",i);
	   if(!m_PictureUp.Load(m_str))
		   AfxMessageBox("读取图源出错");
	   Invalidate();
	   i++;
	   if(i>6)i=0;
	   break;
   }
	CDialog::OnTimer(nIDEvent);
}

BOOL CBeginDlg::PreTranslateMessage(MSG* pMsg) 
{
    if(::GetKeyState(VK_ESCAPE)<0||::GetKeyState(VK_RETURN)<0)return TRUE;	
	return CDialog::PreTranslateMessage(pMsg);
}

void CBeginDlg::OnOK() 
{
    KillTimer(1);
	CDialog::OnOK();
}
