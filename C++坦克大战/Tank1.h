// Tank1.h: interface for the CTank class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TANK1_H__1BBBE5C8_0B87_48BC_84F2_F69FC88FD3C3__INCLUDED_)
#define AFX_TANK1_H__1BBBE5C8_0B87_48BC_84F2_F69FC88FD3C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TankMap.h"
#include "Tank_Defs.h"	// Added by ClassView
#include "NormalBullet.h"

//##ModelId=3EBA49A203A9
class CTank  
{
public:
public:
	//##ModelId=3EBA49A3009D
	BOOL DownCanGo();
	//##ModelId=3EBA49A3008E
	BOOL HasBlock(int sx,int sy,int ex,int ey);
	//##ModelId=3EBA49A3007F
	void Init(CGame* pGame,int X,int Y,LPDIRECTDRAWSURFACE Tankbmp,LPDIRECTDRAWSURFACE TankStatebmp,int nSkin=0);
	//##ModelId=3EBA49A30074
	Obj HasObject(int xpos,int ypos);
	//##ModelId=3EBA49A30072
	void DrawTank(LPDIRECTDRAWSURFACE pdds);
	//##ModelId=3EBA49A30071
	void DestroyTank();
	//##ModelId=3EBA49A3006F
	BOOL TankMove2(TankDirection direction);
	//##ModelId=3EBA49A3006D
	Result IsWhat(TankDirection direction);
	//##ModelId=3EBA49A30063
	void SetGame(CGame* pGame=NULL);
	//##ModelId=3EBA49A30060
	void SetTankPos(int x,int y);
	//##ModelId=3EBA49A3005F
	void SetBullet();  //设置子弹的类型
	//##ModelId=3EBA49A3005E
	CTank();
	//##ModelId=3EBA49A3005D
	virtual ~CTank();
	
	//##ModelId=3EBA49A30054
	void SetTankBmp(LPDIRECTDRAWSURFACE bmp){m_bmpTank=bmp;}
	//##ModelId=3EBA49A30053
	LPDIRECTDRAWSURFACE GetTankBmp(){return m_bmpTank;}
	//##ModelId=3EBA49A30051
	void SetTankStateBmp(LPDIRECTDRAWSURFACE bmp){m_bmpTankState=bmp;}
	//##ModelId=3EBA49A30050
	LPDIRECTDRAWSURFACE GetTankStateBmp(){return m_bmpTankState;}

	//##ModelId=3EBA49A3004F
	void Shoot();
	//##ModelId=3EBA49A3004E
	TankDirection GetDirection()const{return m_nDirection;}
	//##ModelId=3EBA49A30046
	void SetDirection(TankDirection direction){m_nDirection=direction;}
	//##ModelId=3EBA49A30045
	void AddTankSpeed(){m_nTankSpeed=min(m_nTankSpeed+1,4);}
	//##ModelId=3EBA49A30043
	int SetTankLife(int life)
	{
		if(life>=0)m_nTankLife=life;
		else m_nTankLife=0;
		return m_nTankLife;
	}
	//##ModelId=3EBA49A30042
	int GetTankLife(){return m_nTankLife;}
	//##ModelId=3EBA49A30041
	void TankMove();
	//##ModelId=3EBA49A30040
	int GetX()const{return m_nX;}
	//##ModelId=3EBA49A3003F
	int GetY()const{return m_nY;}
	//##ModelId=3EBA49A3003E
	int GetBulletSpeed(){return m_Normal->GetSpeed();}
	//##ModelId=3EBA49A3001F
	BOOL m_bindex;
	//##ModelId=3EBA49A30014
	CRect rect;

	//##ModelId=3EBA49A30013
	int m_nFireSpeed;
	//##ModelId=3EBA49A30035
    inline void SetRect(){rect.SetRect(x,y,x+TANKWIDTH,y+TANKHEIGHT);} 
	//##ModelId=3EBA49A30034
	inline int Getx(){return x;}
	//##ModelId=3EBA49A30033
	inline int Gety(){return y;}
	//##ModelId=3EBA49A30031
	inline void SetSkin(int nSkin=0);
	//##ModelId=3EBA49A30030
	inline int GetSkin(){return m_nSkin;}
private:
	//##ModelId=3EBA49A30012
	int  m_nDoubleBullet;  //设置子弹单发、双发，还是多发 默认为单发  1--单, 2--双发，3--3发，最多3发 
	//##ModelId=3EBA49A3000F
	TankDirection m_nDirection;
	//##ModelId=3EBA49A30002
	int m_nX;
	//##ModelId=3EBA49A30001
	int m_nY;
	//##ModelId=3EBA49A30000
	int x;
	//##ModelId=3EBA49A203DB
	int y;
	//##ModelId=3EBA49A203DA
	int m_nTankSpeed;
	//##ModelId=3EBA49A203D9
	int m_nTankLife;
	//##ModelId=3EBA49A203D8
	int m_nSkin;
	//##ModelId=3EBA49A203D1
	int m_nDamage;           //子弹威力
	//##ModelId=3EBA49A203D0
	int tempDamage;          //用来作为临时变量保存子弹威力
	//##ModelId=3EBA49A203CD
	CGame* m_pGame;
	//##ModelId=3EBA49A203C8
	CNormalBullet * m_Normal;   //设定子弹
	//##ModelId=3EBA49A203C3
	LPDIRECTDRAWSURFACE m_bmpTank;
	//##ModelId=3EBA49A203BE
	LPDIRECTDRAWSURFACE m_bmpTankState;
	//##ModelId=3EBA49A203BB
	int m_nStep;
	//##ModelId=3EBA49A203BA
	int sort;        //子弹的类型
	//##ModelId=3EBA49A203B9
	int number;      //特殊子弹的弹药数 
private:
	//##ModelId=3EBA49A3002E
	BOOL HeadHaveTank(TankDirection dir);
	//##ModelId=3EBA49A30024
	BYTE Belong();        //判断该坦克是player还是npc 1:LFET-PLAYER,2:RIGHT-PLAYER,3:NPC
	//##ModelId=3EBA49A30023
	int MapxyTonXnY();
	//##ModelId=3EBA49A30020
	void SetCell(Cell type,BYTE mask=0);
};

#endif // !defined(AFX_TANK1_H__1BBBE5C8_0B87_48BC_84F2_F69FC88FD3C3__INCLUDED_)
