// ICEBullet.h: interface for the CICEBullet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICEBULLET_H__95B4467A_801E_4380_A876_139FE637D088__INCLUDED_)
#define AFX_ICEBULLET_H__95B4467A_801E_4380_A876_139FE637D088__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Bullet.h"

//##ModelId=3EBA49A201A5
class CICEBullet : public CBullet  
{
public:
	//##ModelId=3EBA49A201BB
	CICEBullet();
	//##ModelId=3EBA49A201BA
	virtual ~CICEBullet();
	//##ModelId=3EBA49A201B9
	virtual BOOL BulletTrack();     //各种子弹的弹道
	//##ModelId=3EBA49A201B7
	virtual BOOL BulletAnimate(int state=START);   //各种子弹得动画(三阶段:出发，飞行途中，击中)
private:
	//##ModelId=3EBA49A201B6
	BOOL m_bIce;
};

#endif // !defined(AFX_ICEBULLET_H__95B4467A_801E_4380_A876_139FE637D088__INCLUDED_)
