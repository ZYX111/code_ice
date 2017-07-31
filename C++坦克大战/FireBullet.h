// FireBullet.h: interface for the CFireBullet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIREBULLET_H__5646CDDC_9ADD_4834_AE14_3B9911F384C8__INCLUDED_)
#define AFX_FIREBULLET_H__5646CDDC_9ADD_4834_AE14_3B9911F384C8__INCLUDED_
#include "Bullet.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//##ModelId=3EBA49A10196
class CFireBullet : public CBullet  
{
public:
	//##ModelId=3EBA49A1019D
	CFireBullet();
	//##ModelId=3EBA49A1019C
	virtual ~CFireBullet();
	//##ModelId=3EBA49A1019B
	virtual BOOL BulletTrack();     //各种子弹的弹道
	//##ModelId=3EBA49A10199
	virtual BOOL BulletAnimate(int state=START);   //各种子弹得动画(三阶段:出发，飞行途中，击中)
private:
	//##ModelId=3EBA49A10198
	BOOL m_bFire;
};

#endif // !defined(AFX_FIREBULLET_H__5646CDDC_9ADD_4834_AE14_3B9911F384C8__INCLUDED_)
