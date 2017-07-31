// LaserBullet.h: interface for the CLaserBullet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LASERBULLET_H__249A2F5D_9BDA_47A4_A636_6CD02382D76D__INCLUDED_)
#define AFX_LASERBULLET_H__249A2F5D_9BDA_47A4_A636_6CD02382D76D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Bullet.h"

//##ModelId=3EBA499D000F
class CLaserBullet : public CBullet  
{
public:
	//##ModelId=3EBA499D0015
	CLaserBullet();
	//##ModelId=3EBA499D0014
	virtual ~CLaserBullet();
	//##ModelId=3EBA499D0013
	virtual BOOL BulletTrack();     //各种子弹的弹道
	//##ModelId=3EBA499D0011
	virtual BOOL BulletAnimate(int state=START);   //各种子弹得动画(三阶段:出发，飞行途中，击中)
private:

};

#endif // !defined(AFX_LASERBULLET_H__249A2F5D_9BDA_47A4_A636_6CD02382D76D__INCLUDED_)
