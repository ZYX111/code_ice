// Explode.cpp: implementation of the CExplode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tank.h"
#include "Explode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExplode::CExplode()
{
	m_bmpExplode=NULL;
	m_nTotalBmps=NUM_EXPLODES;
	m_nIndex=0;
	m_nSort=0;
	m_nCycles=1;
	m_nTimes=0;
	x=0;y=0;w=32;h=24;
}

CExplode::~CExplode()
{

}

void CExplode::DrawExplode(LPDIRECTDRAWSURFACE pdds)
{
	if(m_bmpExplode)
	{
		RECT rc;
		rc.left=m_nIndex * w;
		rc.top=0;
		rc.right=rc.left + w;
		rc.bottom=h;
		pdds->BltFast( x, y, m_bmpExplode, &rc, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );
	}
}

void CExplode::SetPostion(int xt, int yt)
{
	x=xt;y=yt;
}

BOOL CExplode::Burn()
{
	m_nIndex++;
	if(m_nIndex>=m_nTotalBmps)
	{
		m_nIndex=0;
		m_nTimes++;
		if(m_nTimes >= m_nCycles)
			return FALSE;
	}
	return TRUE;
}

BOOL CExplode::Init(int xt, int yt, int iCycles, int iTotalBmps,LPDIRECTDRAWSURFACE bmp,int iw,int ih,int iSort,int iSide)
{
	x=xt;y=yt;
	m_nCycles=iCycles;
	m_nTotalBmps=iTotalBmps;
	m_bmpExplode=bmp;
	w=iw;h=ih;
	m_nSort=iSort;
	m_nSide=iSide;
	return TRUE;
}
