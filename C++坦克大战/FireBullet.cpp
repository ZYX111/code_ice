// FireBullet.cpp: implementation of the CFireBullet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tank.h"
#include "FireBullet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFireBullet::CFireBullet()
{

}

CFireBullet::~CFireBullet()
{

}
BOOL CFireBullet::BulletTrack()
{
 return FALSE;
}
BOOL CFireBullet::BulletAnimate(int state)
{ 
 return FALSE;
}
