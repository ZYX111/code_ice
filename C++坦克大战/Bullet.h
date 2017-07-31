// Bullet.h: interface for the CBullet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BULLET_H__B5E6C45D_3C68_4CF3_B36A_A062FCEC8ED5__INCLUDED_)
#define AFX_BULLET_H__B5E6C45D_3C68_4CF3_B36A_A062FCEC8ED5__INCLUDED_



#include "Tank_Defs.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


const int START=1;
const int MIDDLE=2;
const int END=3;

class CGame;
//##ModelId=3EBA49A3029F
class CBullet 
/****************************************************************
*基类															*
*归纳各种子弹的共性，并提供纯虚函数接口							*	
****************************************************************/  
{
public:
	//##ModelId=3EBA49A30303
	CBullet();
	//##ModelId=3EBA49A30302
	virtual ~CBullet();
	
public:
	//##ModelId=3EBA49A30300
	void DestroyObj(Obj &object);
	//##ModelId=3EBA49A302FD
	Obj HasObject(int xpos,int ypos);
	//##ModelId=3EBA49A302F5
	void SetBulletBmp(LPDIRECTDRAWSURFACE bmp);
	//##ModelId=3EBA49A302F3
	void CBullet::DrawBullet(LPDIRECTDRAWSURFACE pdds);
	//##ModelId=3EBA49A302F0
	void SetPosition(int n,int m);
	//##ModelId=3EBA49A302EF
	int MapxyTonXnY();
	//##ModelId=3EBA49A302E6
	Result IsWhat(int xt,int yt);//从地图里面判断前方是什么
	//##ModelId=3EBA49A302E4
	BOOL ChangeColor(COLORREF x=0x00000000ff);			 //改变颜色
	//##ModelId=3EBA49A302E3
    virtual	BOOL BulletTrack()=0;						 //各种子弹的弹道
	//##ModelId=3EBA49A302E1
	BOOL BulletAnimate(int state=START);				 //各种子弹得动画(三阶段:出发，飞行途中，击中)
	//##ModelId=3EBA49A302DF
	void SetSpeed(int speed=4){m_nSpeed=speed;}
	//##ModelId=3EBA49A302D7
	void SetDamage(int n=50){m_nDamage=n;}
	//##ModelId=3EBA49A302D6
	int GetDamage()const{return m_nDamage;}
	//##ModelId=3EBA49A302D5
	int GetSpeed(){return m_nSpeed;}
	//##ModelId=3EBA49A302BF
	CRect rect;
	//##ModelId=3EBA49A302D4
	void SetRect(){rect.SetRect(x,y,x+BULLETWIDTH,y+BULLETHEIGHT);}
	//##ModelId=3EBA49A302D2
	inline void SetDirection(TankDirection direction){m_nDirection=direction;}
	//##ModelId=3EBA49A302D0
	inline void SetGame(CGame * pGame){ m_pGame=pGame;}
	//##ModelId=3EBA49A302CF
	inline int Getx(){return x;}
	//##ModelId=3EBA49A302CE
	inline int Gety(){return y;}
	//##ModelId=3EBA49A302C4
	inline void SetBelong(BYTE a){Belong=a;}
	//##ModelId=3EBA49A302C2
	inline void Setx(int m){x=m;}  //以下两种方法主要针对子弹双发而设置
	//##ModelId=3EBA49A302C0
	inline void Sety(int n){y=n;}

protected:
	//##ModelId=3EBA49A302BC
	LPDIRECTDRAWSURFACE m_bmpBullet;
	//##ModelId=3EBA49A302B9
	CString strMsg;
	//##ModelId=3EBA49A302B6
	CGame *m_pGame;
	//##ModelId=3EBA49A302B1
	TankDirection m_nDirection;
	//##ModelId=3EBA49A302A5
	int m_nSpeed;  //用来记录子弹速度基数
	//##ModelId=3EBA49A302A4
	int m_nX;      //记录子弹的x位置--以中轴线为基准
	//##ModelId=3EBA49A302A3
	int m_nY;      //记录子弹的y位置
	int x,y;       //对应象素坐标
	//##ModelId=3EBA49A302A2
	int m_nDamage; //记录子弹伤害力
	//##ModelId=3EBA49A302A1
	int index;     //标定是那一种子弹
	//##ModelId=3EBA49A302A0
	BYTE Belong;   //标定子弹的出处/* 3,NPC、1,left-play、2,right-play*/

};

#endif // !defined(AFX_BULLET_H__B5E6C45D_3C68_4CF3_B36A_A062FCEC8ED5__INCLUDED_)
