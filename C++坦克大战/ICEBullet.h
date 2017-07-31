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
	virtual BOOL BulletTrack();     //�����ӵ��ĵ���
	//##ModelId=3EBA49A201B7
	virtual BOOL BulletAnimate(int state=START);   //�����ӵ��ö���(���׶�:����������;�У�����)
private:
	//##ModelId=3EBA49A201B6
	BOOL m_bIce;
};

#endif // !defined(AFX_ICEBULLET_H__95B4467A_801E_4380_A876_139FE637D088__INCLUDED_)
