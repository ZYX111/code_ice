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
	virtual BOOL BulletTrack();     //�����ӵ��ĵ���
	//##ModelId=3EBA49A10199
	virtual BOOL BulletAnimate(int state=START);   //�����ӵ��ö���(���׶�:����������;�У�����)
private:
	//##ModelId=3EBA49A10198
	BOOL m_bFire;
};

#endif // !defined(AFX_FIREBULLET_H__5646CDDC_9ADD_4834_AE14_3B9911F384C8__INCLUDED_)
