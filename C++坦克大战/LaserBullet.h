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
	virtual BOOL BulletTrack();     //�����ӵ��ĵ���
	//##ModelId=3EBA499D0011
	virtual BOOL BulletAnimate(int state=START);   //�����ӵ��ö���(���׶�:����������;�У�����)
private:

};

#endif // !defined(AFX_LASERBULLET_H__249A2F5D_9BDA_47A4_A636_6CD02382D76D__INCLUDED_)
