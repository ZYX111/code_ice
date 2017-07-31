// Egg.cpp: implementation of the CEgg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tank.h"
#include "Egg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEgg::CEgg():m_nYPosition(-1),m_nXPosition(-1),m_nTime(30),m_bHas(FALSE)
{
  index=-1;
  m_bmpEgg=NULL;
  m_nLife=1700;
}

CEgg::~CEgg()
{

}


void CEgg::SetEggBmp(LPDIRECTDRAWSURFACE bmp)
{
	m_bmpEgg=bmp;
}

void CEgg::DrawEgg(LPDIRECTDRAWSURFACE pdds)
{
	RECT rc;
	rc.left=0;
	rc.top=index*TANKHEIGHT;
	rc.right=rc.left + TANKWIDTH;
	rc.bottom=rc.top+TANKHEIGHT;
	pdds->BltFast( m_nXPosition, m_nYPosition, m_bmpEgg, &rc, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );
}

void CEgg::RandomSet()//ÀÊº¥…Ë÷√EggµƒŒª÷√
{
	srand(unsigned(timeGetTime()));
	int i=rand()%15;
	SetIndex(i);
	m_nXPosition=rand()%(SCREEN_WIDTH-TANKWIDTH);
	m_nYPosition=rand()%(SCREEN_HEIGHT-TANKHEIGHT);
}

