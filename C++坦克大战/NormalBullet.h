// NormalBullet.h: interface for the CNormalBullet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NORMALBULLET_H__7613E07C_E48E_4ADC_9771_BA149852396C__INCLUDED_)
#define AFX_NORMALBULLET_H__7613E07C_E48E_4ADC_9771_BA149852396C__INCLUDED_

#include "Tank_Defs.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Bullet.h"

//##ModelId=3EBA49A3038A
class CNormalBullet : public CBullet
/****************************************************************
*����̹�˵Ļ�̬�ӵ�												*
*��ѡ������ѡ����Զ�Ӧ�Ļ�̬�ӵ���							*
****************************************************************/  
{
public:
	friend class CTank;
	//##ModelId=3EBA49A3039A
	BOOL Choose(int sort=0);  //��̹��ѡ���Լ������л����ӵ�
	//##ModelId=3EBA49A30399
	CNormalBullet();
	//##ModelId=3EBA49A3038D
	virtual ~CNormalBullet();
	//##ModelId=3EBA49A3038C
	virtual BOOL BulletTrack();     //�����ӵ��ĵ���
private:
};

#endif // !defined(AFX_NORMALBULLET_H__7613E07C_E48E_4ADC_9771_BA149852396C__INCLUDED_)
