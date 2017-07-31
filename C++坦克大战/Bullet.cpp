// Bullet.cpp: implementation of the CBullet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tank.h"
#include "Bullet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBullet::CBullet():m_nSpeed(5),index(-1),m_nDamage(50)
{
  
}

CBullet::~CBullet()
{

}
BOOL CBullet::ChangeColor(COLORREF x/* =0x00000000ff */)
{
	return FALSE;
}
BOOL CBullet::BulletAnimate(int start/* =START */)
{
	switch(start)
	{
	case 1:
        strMsg="冲呀";
		break;
	case 2:
		strMsg="我要砍死他";
		break;
	case 3:
		strMsg="杀呀";
		break;
	}
	return TRUE;
}

Result CBullet::IsWhat(int xt,int yt)
{
	Result rst;
	memset(rst.type,0,3*sizeof(CellType));
	rst.num=0;
	rst.bCanGo=TRUE;
	int n1,n2;
	int m;
//	xt=min(SCREEN_WIDTH-1,max(0,xt));
//	yt=min(SCREEN_HEIGHT-1,max(0,yt));
	switch(m_nDirection)
	{
	case UP:
	case DOWN:
		if(yt<0 || yt>=SCREEN_HEIGHT-BULLETHEIGHT)
		{
			rst.bCanGo=FALSE;
			break;
		}
		if(m_nDirection==UP)
			m=yt / TANKMAPCELL;
		else
			m=(yt + BULLETHEIGHT) / TANKMAPCELL-1;
		n1=xt / TANKMAPCELL;
		n2=(xt + BULLETWIDTH) / TANKMAPCELL;
		if(n1<0 || n1>=TANKMAPWIDTH || n2<0 || n2>=TANKMAPWIDTH || m<0 || m>=TANKMAPHEIGHT)
		{
			rst.bCanGo=FALSE;
			break;
		}
		if(n1==n2)
		{
			rst.num=1;
			rst.type[0]=m_pGame->m_TankMap.GetCellType(n1,m);
			rst.position[0].x=n1;
			rst.position[0].y=m;
			if(rst.type[0].cell != CT_EMPTY && rst.type[0].cell != CT_TREE && rst.type[0].cell != CT_ICE && rst.type[0].cell != CT_WATER)
			{
				CExplode *p=new CExplode;
				if(p)
				{
					p->SetExplodeBmp(m_pGame->m_pExplodeBuffer);
					p->SetPostion(n1*TANKMAPCELL,m*TANKMAPCELL);
					m_pGame->m_Explode.AddTail(p);
					m_pGame->PlaySound();
				}
				rst.bCanGo=FALSE;
			}
		}
		else
		{
			rst.num=2;
			rst.type[0]=m_pGame->m_TankMap.GetCellType(n1,m);
			rst.position[0].x=n1;
			rst.position[0].y=m;
			if(rst.type[0].cell != CT_EMPTY && rst.type[0].cell != CT_TREE && rst.type[0].cell != CT_ICE && rst.type[0].cell != CT_WATER)
			{
				CExplode *p=new CExplode;
				if(p)
				{
					p->SetExplodeBmp(m_pGame->m_pExplodeBuffer);
					p->SetPostion(n1*TANKMAPCELL,m*TANKMAPCELL);
					m_pGame->m_Explode.AddTail(p);
					m_pGame->PlaySound();
				}
				rst.bCanGo=FALSE;
			}
			rst.type[1]=m_pGame->m_TankMap.GetCellType(n2,m);
			rst.position[1].x=n2;
			rst.position[1].y=m;
			if(rst.bCanGo && rst.type[1].cell != CT_EMPTY && rst.type[1].cell != CT_TREE && rst.type[1].cell != CT_ICE && rst.type[1].cell != CT_WATER)
			{
				CExplode *p=new CExplode;
				if(p)
				{
					p->SetExplodeBmp(m_pGame->m_pExplodeBuffer);
					if(n2==39)n2-=1;
					p->SetPostion(n2*TANKMAPCELL,m*TANKMAPCELL);
					m_pGame->m_Explode.AddTail(p);
					m_pGame->PlaySound();
				}
				rst.bCanGo=FALSE;
			}
		}
		break;
	case LEFT:
	case RIGHT:
		if(xt<0 || xt>=SCREEN_WIDTH-BULLETWIDTH)
		{
			rst.bCanGo=FALSE;
			break;
		}
		if(m_nDirection==LEFT)
			m=xt / TANKMAPCELL;
		else
			m=(xt + BULLETHEIGHT) / TANKMAPCELL-1;
		n1=yt / TANKMAPCELL;
		n2=(yt + BULLETHEIGHT) / TANKMAPCELL;
		if(n1<0 || n1>=TANKMAPHEIGHT || n2<0 || n2>=TANKMAPHEIGHT || m<0 || m>=TANKMAPWIDTH)
		{
			rst.bCanGo=FALSE;
			break;
		}
		if(n1==n2)
		{
			rst.num=1;
			rst.type[0]=m_pGame->m_TankMap.GetCellType(m,n1);
			rst.position[0].x=m;
			rst.position[0].y=n1;
			if(rst.type[0].cell != CT_EMPTY && rst.type[0].cell != CT_TREE && rst.type[0].cell != CT_ICE&& rst.type[0].cell != CT_WATER)
			{
				CExplode *p=new CExplode;
				if(p)
				{
					p->SetExplodeBmp(m_pGame->m_pExplodeBuffer);
					p->SetPostion(m*TANKMAPCELL,n1*TANKMAPCELL);
					m_pGame->m_Explode.AddTail(p);
					m_pGame->PlaySound();
				}
				rst.bCanGo=FALSE;
			}
		}
		else
		{
			rst.num=2;
			rst.type[0]=m_pGame->m_TankMap.GetCellType(m,n1);
			rst.position[0].x=m;
			rst.position[0].y=n1;
			if(rst.type[0].cell != CT_EMPTY && rst.type[0].cell != CT_TREE && rst.type[0].cell != CT_ICE&& rst.type[0].cell != CT_WATER)
			{
				CExplode *p=new CExplode;
				if(p)
				{
					p->SetExplodeBmp(m_pGame->m_pExplodeBuffer);
					p->SetPostion(m*TANKMAPCELL,n1*TANKMAPCELL);
					m_pGame->m_Explode.AddTail(p);
					m_pGame->PlaySound();
				}
				rst.bCanGo=FALSE;
			}
			rst.type[1]=m_pGame->m_TankMap.GetCellType(m,n2);
			rst.position[1].x=m;
			rst.position[1].y=n2;
			if(rst.bCanGo && rst.type[1].cell != CT_EMPTY && rst.type[1].cell != CT_TREE && rst.type[1].cell != CT_ICE&& rst.type[1].cell != CT_WATER)
			{
				CExplode *p=new CExplode;
				if(p)
				{
					p->SetExplodeBmp(m_pGame->m_pExplodeBuffer);
					p->SetPostion(m*TANKMAPCELL,n2*TANKMAPCELL);
					m_pGame->m_Explode.AddTail(p);
					m_pGame->PlaySound();
				}
				rst.bCanGo=FALSE;
			}
		}
		break;
	}
	return rst;
}
void CBullet::DrawBullet(LPDIRECTDRAWSURFACE pdds)
{
	RECT rc;
	rc.left=index * BULLETWIDTH;
	rc.top=0;
	rc.right=rc.left + BULLETWIDTH;
	rc.bottom=BULLETHEIGHT;
	pdds->BltFast( x, y, m_bmpBullet, &rc, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );
}

int CBullet::MapxyTonXnY()
{
	int ret=0;
	m_nX=(x==0?0:x/TANKMAPCELL);
    m_nY=(y==0?0:y/TANKMAPCELL);
    return ret;
}

void CBullet::SetPosition(int m, int n)
{
     switch(m_nDirection)
	 {
	 case UP:
		x=m+TANKWIDTH/2-BULLETWIDTH/2;
		y=n-BULLETHEIGHT;
		break;
	 case DOWN:
		x=m+TANKWIDTH/2-BULLETWIDTH/2;
		y=n+TANKHEIGHT+BULLETHEIGHT;
		break;
	 case LEFT:
		x=m-BULLETWIDTH;
		y=n+TANKHEIGHT/2-BULLETHEIGHT/2;
        break;
	 case RIGHT:
		 x=m+TANKWIDTH+BULLETWIDTH;
         y=n+TANKHEIGHT/2-BULLETHEIGHT/2;
        break;
	 }
	 SetRect();
}

void CBullet::SetBulletBmp(LPDIRECTDRAWSURFACE bmp)
{
   m_bmpBullet=bmp;
}

Obj CBullet::HasObject(int xpos, int ypos)
{
	Obj object;
    BOOL Result=FALSE;
	CRect rec(xpos,ypos,xpos+BULLETWIDTH,ypos+BULLETHEIGHT);
	
	POSITION position,preposition;

	position=m_pGame->m_BulletList.GetHeadPosition();
    
	while(position)
	{
		preposition=position;
		CBullet * p=m_pGame->m_BulletList.GetNext(position);
		if(p!=this)
		{
			CRect rc;
			Result=rc.IntersectRect(&rec,&(p->rect));
			if(Result)
			{
				object.position=preposition;
				object.sort=0;
				object.has=TRUE;
				CExplode *pe=new CExplode;
				if(pe)
				{
					pe->SetExplodeBmp(m_pGame->m_pExplodeBuffer);
					pe->SetPostion(p->Getx()-12,p->Gety()-12);
					m_pGame->m_Explode.AddTail(pe);
					m_pGame->PlaySound();
				}
                return object;
			}	   
		}
	}
	
	position=m_pGame->m_Player.GetHeadPosition();
    
	while(position)
	{
		preposition=position;
		CTank * p=m_pGame->m_Player.GetNext(position);
		CRect rc;
		Result=rc.IntersectRect(&rec,&p->rect);
		if(Result)
		{
			object.position=preposition;
			object.sort=1;
			object.has=TRUE;
			CExplode *pe=new CExplode;
			if(pe)
			{
				pe->SetExplodeBmp(m_pGame->m_pExplodeBuffer);
				pe->SetPostion(p->Getx(),p->Gety());
				m_pGame->m_Explode.AddTail(pe);
				m_pGame->PlaySound();
			}
            return object;
		}	   
	}
	position=m_pGame->m_Enemy.GetHeadPosition();
    
	while(position)
	{
		preposition=position;
		CTank * p=m_pGame->m_Enemy.GetNext(position);
		CRect rc;
		Result=rc.IntersectRect(&rec,&p->rect);
		if(Result)
		{
			object.position=preposition;
			object.sort=2;
			object.has=TRUE;
			CExplode *pe=new CExplode;
			if(pe)
			{
				pe->SetExplodeBmp(m_pGame->m_pExplodeBuffer);
				pe->SetPostion(p->Getx(),p->Gety());
				m_pGame->m_Explode.AddTail(pe);
				m_pGame->PlaySound();
			}
	        return object;
		}	   
	
	}   
	return object;
}

void CBullet::DestroyObj(Obj &object)
{
	int TankLife;
	int NowLife;
	int tempDamage=m_nDamage; 
	POSITION pos;
	switch(object.sort)
	{
	case 0://Bullet
		delete m_pGame->m_BulletList.GetAt(object.position);
		m_pGame->m_BulletList.RemoveAt(object.position);
		break;
	case 1://Player
		pos=m_pGame->m_Player.GetHeadPosition();
		if(pos==object.position && m_pGame->m_tankleft)
		{ 
			if(Belong==1)return;
			if(Belong==2)tempDamage=10;
            TankLife=m_pGame->m_tankleft->GetTankLife();
			NowLife=m_pGame->m_tankleft->SetTankLife(TankLife-tempDamage);
			if(NowLife==0)
			{
				m_pGame->m_nRightScore=0;  //打死了左边，右边的坦克分数减为0
                delete m_pGame->m_tankleft;  
				m_pGame->m_tankleft=NULL;
				m_pGame->m_Player.RemoveAt(object.position);
				m_pGame->PlaySound(1);
			}
		}
		else if(m_pGame->m_tankright)
		{
			if(Belong==2)return;
			if(Belong==1)tempDamage=10;
			TankLife=m_pGame->m_tankright->GetTankLife();
			NowLife=m_pGame->m_tankright->SetTankLife(TankLife-tempDamage);
			if(NowLife==0)
			{
				m_pGame->m_nLeftScore=0;//打死了右边，左边的坦克分数减为0
                delete m_pGame->m_tankright;  
				m_pGame->m_tankright=NULL;
				m_pGame->m_Player.RemoveAt(object.position);
				m_pGame->PlaySound(1);
			}
		}
		break;
	case 2://Enemy
        CTank * p=m_pGame->m_Enemy.GetAt(object.position);
		if(Belong==3)tempDamage=1;
        TankLife=p->GetTankLife();
		NowLife=p->SetTankLife(TankLife-tempDamage);
		if(NowLife==0)
		{
			int temp=p->GetSkin();
			delete p;
			m_pGame->m_Enemy.RemoveAt(object.position);
			CEgg * egg;
			if(Belong==1)
			{
				m_pGame->m_nLeftKills++;
				switch(temp)
				{
				case 0:
				case 1:
				case 2:
				case 3:
                   m_pGame->m_nLeftScore+=50;
				   break;
				case 4:
				case 5:
                   m_pGame->m_nLeftScore+=75; 
					break;
				case 6:
				    m_pGame->m_nLeftScore+=100;
					break;
				case 7:
                    m_pGame->m_nLeftScore+=125;
					egg=new CEgg;
					if(egg)
					{
						egg->SetEggBmp(m_pGame->m_pEggBuffer);
						egg->RandomSet();
						m_pGame->m_Egg.AddTail(egg);
					}
					break;
				case 8:
					m_pGame->m_nLeftScore+=150;
					egg=new CEgg;
					if(egg)
					{
						egg->SetEggBmp(m_pGame->m_pEggBuffer);
						egg->RandomSet();
						m_pGame->m_Egg.AddTail(egg);
					}
					break;
				}
				if(m_pGame->m_nLeftScore-9999>=0){m_pGame->m_nLeftScore-=9999;m_pGame->m_nLeftPlayer++;}
			}
			else if(Belong==2)
			{		
				m_pGame->m_nRightKills++;
				switch(temp)
				{
				case 0:
				case 1:
				case 2:
				case 3:
                   m_pGame->m_nRightScore+=50;
				   break;
				case 4:
				case 5:
                   m_pGame->m_nRightScore+=75; 
					break;
				case 6:
				    m_pGame->m_nRightScore+=100;
					break;
				case 7:
                    m_pGame->m_nRightScore+=125;
					egg=new CEgg;
					if(egg)
					{
						egg->SetEggBmp(m_pGame->m_pEggBuffer);
						egg->RandomSet();
						m_pGame->m_Egg.AddTail(egg);
					}
					break;
				case 8:
					m_pGame->m_nRightScore+=150;
					egg=new CEgg;
					if(egg)
					{
						egg->SetEggBmp(m_pGame->m_pEggBuffer);
						egg->RandomSet();
						m_pGame->m_Egg.AddTail(egg);
					}
					break;
				}
				if(m_pGame->m_nRightScore-9999>=0){m_pGame->m_nRightScore-=9999;m_pGame->m_nRightPlayer++;}
			}
				m_pGame->PlaySound(1);
		}
		break;
	}
}
