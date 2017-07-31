// Tank1.cpp: implementation of the CTank class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tank.h"
#include "Tank1.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

	
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTank::CTank()
{
	tempDamage=50;
	sort=0;
	number=-1;
	m_nDoubleBullet=1;
	m_nDirection=UP;
	m_pGame=NULL;
	m_nTankSpeed=2;
	m_nTankLife=200;
	m_nFireSpeed=600;
	m_bmpTank=NULL;
	m_bmpTankState=NULL;
	m_nX=m_nY=x=y=0;
	m_nSkin=0;
	m_nDamage=50;
	m_nStep=0;
	rect.SetRectEmpty();
}

CTank::~CTank()
{
}

BOOL CTank::HeadHaveTank(TankDirection dir)
{
	CRect rct,rc;
	POSITION position;
	CTank *p;
	switch(dir)
	{
	case UP:
		rct.SetRect(x,0,x+TANKWIDTH,y);
		break;
	case DOWN:
		rct.SetRect(x,y+TANKHEIGHT,x+TANKWIDTH,SCREEN_HEIGHT);
		break;
	case LEFT:
		rct.SetRect(0,y,x,y+TANKHEIGHT);
		break;
	case RIGHT:
		rct.SetRect(x+TANKWIDTH,y,SCREEN_WIDTH,y+TANKHEIGHT);
		break;
	}
	position=m_pGame->m_Player.GetHeadPosition();
	while(position)
	{
		p=m_pGame->m_Player.GetNext(position);
		BOOL ret=rc.IntersectRect(&rct,&(p->rect));
		if(ret && rc.Width() > 12 && rc.Height() > 12)
		{
			switch(dir)
			{
			case UP:
				if(HasBlock(m_nX,p->GetY(),m_nX+1,m_nY))
					return FALSE;
			case DOWN:
				if(HasBlock(m_nX,m_nY,m_nX+1,p->GetY()))
					return FALSE;
			case LEFT:
				if(HasBlock(p->GetX(),m_nY,m_nX,m_nY+1))
					return FALSE;
			case RIGHT:
				if(HasBlock(m_nX,m_nY,p->GetX(),m_nY+1))
					return FALSE;
			}
			return TRUE;			
		}
	}
	CRect rcTemp(19*TANKMAPCELL,38*TANKMAPCELL,21*TANKMAPCELL,40*TANKMAPCELL);
	if(rc.IntersectRect(&rct,&rcTemp) && m_nY > 32)
		return TRUE;
	return FALSE;
}

void CTank::TankMove()
{
	   int flag[4];
	   memset(flag,0,4*sizeof(int));
	   
	   BYTE rnd=rand()%150;
	   if(rnd==0 && m_nDirection==LEFT && DownCanGo())
		   m_nDirection=DOWN;
	   if(rnd==1 && m_nDirection==RIGHT && DownCanGo())
		   m_nDirection=DOWN;

	   if(HeadHaveTank(UP))
		   m_nDirection=UP;
	   else if(m_pGame->m_nGameLevel>2 && HeadHaveTank(LEFT))
		   m_nDirection=LEFT;
	   else if(m_pGame->m_nGameLevel>4 && HeadHaveTank(RIGHT))
		   m_nDirection=RIGHT;
	   else if(m_pGame->m_nGameLevel>6 && HeadHaveTank(DOWN))
		   m_nDirection=DOWN;
	   else
	   {
	   
		   BOOL bCanGo=TankMove2(m_nDirection);
			int num=4,index=0;
			int i=m_nDirection;
			flag[i]=1;
			int temp;
		   while(!bCanGo && num--)
		   {
			   while(flag[i])
			   {
	   				flag[i]=0;
				   srand(unsigned(time(NULL)));
				   temp=rand()%3+1;
				   switch(i)
				   {
				   case 0:
					   i=i+temp;
					   break;
				   case 1:
				   case 2:
				   case 3:
					   i=i+temp;
					   if(i>3)i=i%4;
					   break;	
				   }
			   }
			   m_nDirection=(TankDirection)i;
				   flag[i]=1;
			   bCanGo=TankMove2(m_nDirection);
		   }
	   }
	   if(++m_nStep > 30 && HeadHaveTank(m_nDirection))
	   {
		   m_nStep=0;
		   TankMove2(m_nDirection);
		   Shoot();
	   }
}

void CTank::SetTankPos(int x, int y)
{
		m_nX=x;
		m_nY=y;
		this->x=m_nX*TANKMAPCELL;
		this->y=m_nY*TANKMAPCELL;
//		SetCell(CT_TANK);
}

void CTank::SetBullet()
{
	ASSERT(sort>=0&&sort<5);
	ASSERT(m_Normal);
	m_Normal->Choose(sort);
	m_Normal->SetDirection(m_nDirection);
	m_Normal->SetPosition(x,y);
	m_Normal->SetGame(m_pGame);
	m_Normal->SetDamage(m_nDamage);
	m_Normal->SetBulletBmp(m_pGame->m_pBulletBuffer);
	m_Normal->SetBelong(Belong());
	if(sort!=0)
	{
		
		if(number>0)number--;
		if(number == 0)
		{
			number=-1;
			sort=0;
			m_nDamage=tempDamage;
		}
	}
}
void CTank::SetGame(CGame *pGame)
{
	m_pGame=pGame;
}

Result CTank::IsWhat(TankDirection direction)
{
	Result rst;
	memset(rst.type,0,3*sizeof(CellType));
	rst.num=0;
	rst.bCanGo=TRUE;

	int i;
	int num=2;

	int m=m_nX;
	int n=m_nY;

	switch(direction)
	{
	case UP:
		if(m_nY==0)
		{
			rst.bCanGo=FALSE;
			return rst;
		}
         n-=1;
        if(x%TANKMAPCELL!=0)num=3;
		else num=2;

		for(i=0;i<num;i++)
		{
			CellType ct=m_pGame->m_TankMap.GetCellType(m+i,n);
			rst.type[i]=ct;
			if(ct.cell!=CT_EMPTY && ct.cell!=CT_TREE && ct.cell!=CT_ICE)
			{
				rst.num++;
				rst.bCanGo=FALSE;
				return rst;
			}
		}
		break;
	case DOWN:

		if(m_nY>=TANKMAPHEIGHT-2)
		{
			rst.bCanGo=FALSE;
			return rst;
		}

		if(y%TANKMAPCELL==0)n+=2;
		else 
		{
			if(m_nY==TANKMAPHEIGHT-3)
			{
				rst.bCanGo=TRUE;
				return rst;
			}
			else n+=3;
		}
        
		if(x%TANKMAPCELL!=0)num=3;
		else num=2;
        
		for(i=0;i<num;i++)
		{
			CellType ct=m_pGame->m_TankMap.GetCellType(m+i,n);
			rst.type[i]=ct;
			if(ct.cell!=CT_EMPTY && ct.cell!=CT_TREE && ct.cell!=CT_ICE)
			{
				rst.num++;
				rst.bCanGo=FALSE;
				return rst;
			}
		}
		break;
	case RIGHT:
		if(m_nX>=TANKMAPWIDTH-2)
		{
			rst.bCanGo=FALSE;
			return rst;
		}

		if(x%TANKMAPCELL==0)m+=2;
		else
		{
			if(m_nX==TANKMAPWIDTH-3)
			{
				rst.bCanGo=TRUE;
				return rst;
			}
			else m+=3;
		}

		if(y%TANKMAPCELL!=0)num=3;
		else num=2;

		for(i=0;i<num;i++)
		{
			CellType ct=m_pGame->m_TankMap.GetCellType(m,n+i);
			rst.type[i]=ct;
			if(ct.cell!=CT_EMPTY && ct.cell!=CT_TREE && ct.cell!=CT_ICE)
			{
				rst.num++;
				rst.bCanGo=FALSE;
				return rst;
			}
		}
		break;
	case LEFT:
		if(m_nX<=0)
		{
			rst.bCanGo=FALSE;
			return rst;
		}

		m-=1;

		if(y%TANKMAPCELL!=0)num=3;
		else num=2;

		for(i=0;i<num;i++)
		{
			CellType ct=m_pGame->m_TankMap.GetCellType(m,n+i);
			rst.type[i]=ct;
			if(ct.cell!=CT_EMPTY && ct.cell!=CT_TREE && ct.cell!=CT_ICE)
			{
				rst.num++;
				rst.bCanGo=FALSE;
				return rst;
			}
		}
		break;
	}
	return rst;
}

BOOL CTank::TankMove2(TankDirection direction)
{
	if(!m_pGame)return FALSE;
//    ASSERT(x>=0&&x<=SCREEN_WIDTH-TANKWIDTH);
//	ASSERT(y>=0&&y<=SCREEN_HEIGHT-TANKHEIGHT);

	MapxyTonXnY();

	int index=2;
	int Num=direction-m_nDirection;
	Result rst;
	Obj object;
	if(Num==0)
		rst=IsWhat(m_nDirection); 
	if((Num!=0)||(Num==0 && rst.bCanGo))
	{
	//判断以下边界问题
	  if(Num==0)
	  {
		if(m_nY==0&&m_nDirection==UP)return FALSE;
		if(m_nX==0&&m_nDirection==LEFT)return FALSE;
		if(m_nX==TANKMAPWIDTH-2&&m_nDirection==RIGHT)return FALSE;
		if(m_nY==TANKMAPHEIGHT-2&&m_nDirection==DOWN)return FALSE;      
	  }
	
  }
	if(Num==0)
	{
		switch(m_nDirection)
		{
			case UP:
				object=HasObject(x,y-m_nTankSpeed);
				if(!object.has)
				{
					if(rst.bCanGo)
					{
						y-=m_nTankSpeed;
						if(y<0)y=0;
					}
					else 
					{
						if(y-m_nY*TANKMAPCELL>0)
						{
							y-=m_nTankSpeed;
							if(y<=m_nY*TANKMAPCELL)y=m_nY*TANKMAPCELL;
						}
						else return FALSE;
					}
				}
				else return FALSE;
				break;
			case DOWN:
				object=HasObject(x,y+m_nTankSpeed);
				if(!object.has)
				{
					if(rst.bCanGo)
					{
						y+=m_nTankSpeed;
						if(y>=SCREEN_HEIGHT-TANKHEIGHT)y=SCREEN_HEIGHT-TANKHEIGHT;
					}
					else 
					{
						if(m_nY>=TANKMAPHEIGHT-2)return FALSE;
						if((y+TANKHEIGHT)%TANKMAPCELL!=0)
						{
							index=3;
							y+=m_nTankSpeed;
							if(y>=min(SCREEN_HEIGHT-TANKHEIGHT,(m_nY+index)*TANKMAPCELL-TANKHEIGHT))y=min(SCREEN_HEIGHT-TANKHEIGHT,(m_nY+index)*TANKMAPCELL-TANKHEIGHT);
						}
						else return FALSE;
					}
				}
				else return FALSE;
				break;
			case RIGHT:
				object=HasObject(x+m_nTankSpeed,y);
				if(!object.has)
				{
					if(rst.bCanGo)
					{
						x+=m_nTankSpeed;	
						if(x>=SCREEN_WIDTH-TANKWIDTH)x=SCREEN_WIDTH-TANKWIDTH;
					}
					else
					{
						if(m_nX>=TANKMAPWIDTH-2)return FALSE;
						if((x+TANKWIDTH)%TANKMAPCELL!=0)
						{
							index=3;
							x+=m_nTankSpeed;
							if(x>=min(SCREEN_WIDTH-TANKWIDTH,(m_nX+index)*TANKMAPCELL-TANKWIDTH))x=min(SCREEN_WIDTH-TANKWIDTH,(m_nX+index)*TANKMAPCELL-TANKWIDTH);
						}
						else return FALSE;
					}
				}
				else return FALSE;
				break;
			case LEFT:
				object=HasObject(x-m_nTankSpeed,y);
				if(!object.has)
				{
					if(rst.bCanGo)
					{
						x-=m_nTankSpeed;	
						if(x<0)x=0;
					}
					else
					{
						
						if(x-m_nX*TANKMAPCELL>0)
						{
							x-=m_nTankSpeed;
							if(x<=m_nX*TANKMAPCELL)x=m_nX*TANKMAPCELL;
						}
						else return FALSE;
					}
				}
			   else return FALSE;
				break;
		}
	    MapxyTonXnY();
		this->SetRect();
		return	TRUE;
	}
  else
  {
	  m_nDirection=direction;
	  SetRect();
	  return TRUE;
  }
  return FALSE;
 }

void CTank::Shoot()
{
	if(m_nDoubleBullet==1)
	{
		 m_Normal=new CNormalBullet;
		 if(m_Normal)
		 {
			 SetBullet();
			 m_pGame->m_BulletList.AddHead(m_Normal);
			 m_pGame->m_wavShoot.Play();
		 }
	}
	else
	{
		m_Normal=new CNormalBullet;
		if(m_Normal)
		{
			m_Normal->Choose(sort);
			m_Normal->SetDirection(m_nDirection);
			switch(m_nDirection)
			{
			case UP:
				m_Normal->Setx(x);
				m_Normal->Sety(y-BULLETHEIGHT);
				break;
			case DOWN:
				m_Normal->Setx(x);
				m_Normal->Sety(y+TANKHEIGHT);
				break;
			case LEFT:
				m_Normal->Setx(x-BULLETWIDTH);
				m_Normal->Sety(y);
				break;
			case RIGHT:
				m_Normal->Setx(x+TANKWIDTH);
				m_Normal->Sety(y);
				break;
			}
				
			m_Normal->SetGame(m_pGame);
			m_Normal->SetDamage(m_nDamage);
			m_Normal->SetBulletBmp(m_pGame->m_pBulletBuffer);
			m_Normal->SetBelong(Belong());
			m_pGame->m_BulletList.AddHead(m_Normal);
		}

	   m_Normal=new CNormalBullet;
   		if(m_Normal)
		{
			m_Normal->Choose(sort);
			m_Normal->SetDirection(m_nDirection);
			switch(m_nDirection)
			{
			case UP:
				m_Normal->Setx(x+TANKWIDTH-BULLETWIDTH);
				m_Normal->Sety(y-BULLETHEIGHT);
				break;
			case DOWN:
				m_Normal->Setx(x+TANKWIDTH-BULLETWIDTH);
				m_Normal->Sety(y+TANKHEIGHT);
				break;
			case LEFT:
				m_Normal->Setx(x-BULLETWIDTH);
				m_Normal->Sety(y+TANKHEIGHT-BULLETHEIGHT);
				break;
			case RIGHT:
				m_Normal->Setx(x+TANKWIDTH);
				m_Normal->Sety(y+TANKHEIGHT-BULLETHEIGHT);
				break;
			}
			m_Normal->SetGame(m_pGame);
			m_Normal->SetDamage(m_nDamage);
			m_Normal->SetBulletBmp(m_pGame->m_pBulletBuffer);
			m_Normal->SetBelong(Belong());
			m_pGame->m_BulletList.AddHead(m_Normal);
		}
		if(m_nDoubleBullet==3)
		{
		   m_Normal=new CNormalBullet;
   			if(m_Normal)
			{
			 SetBullet();
			 m_pGame->m_BulletList.AddHead(m_Normal);		
			}
		}
		m_pGame->m_wavShoot.Play();
	}
}

void CTank::SetCell(Cell type,BYTE mask)
{
	for(int i=m_nX;i<m_nX+2;i++)
		for(int j=m_nY;j<m_nY+2;j++)
			m_pGame->m_TankMap.SetMapCell(i,j,type,mask);
}

void CTank::DestroyTank()
{
	if(m_bmpTank)
	{
		m_bmpTank->Release();
		m_bmpTank=NULL;
	}
	if(m_bmpTankState)
	{
		m_bmpTankState->Release();
		m_bmpTankState=NULL;
	}	
}

void CTank::DrawTank(LPDIRECTDRAWSURFACE pdds)
{
	RECT rc;
	rc.left=m_nDirection * TANKWIDTH;
	rc.top=m_nSkin*TANKHEIGHT;
	rc.right=rc.left + TANKWIDTH;
	rc.bottom=rc.top + TANKHEIGHT;
	pdds->BltFast( x, y, m_bmpTank, &rc, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );
	rc.left=0;rc.top=0;rc.right=32;rc.bottom=5;
	pdds->BltFast( x, y-6, m_bmpTankState, &rc, DDBLTFAST_WAIT );
	rc.top=5;rc.bottom=10;
	if(Belong()==1||Belong()==2)rc.right=int(32 * float(m_nTankLife)/200);
	if(Belong()==3)
	{
		switch(m_nSkin)
		{
			case 0:
			case 1:
			case 2:
			case 3:
				rc.right=int(32 * float(m_nTankLife)/100);
				break;
			case 4:
			case 5:
				rc.right=int(32 * float(m_nTankLife)/150);
			    break;
		    
			case 6:
				rc.right=int(32 * float(m_nTankLife)/200);
				break;
			case 7:
				rc.right=int(32 * float(m_nTankLife)/250);
				break;			
			case 8:
				rc.right=int(32 * float(m_nTankLife)/300);
				break;
		}
	}
	pdds->BltFast( x, y-6, m_bmpTankState, &rc, DDBLTFAST_WAIT );
}

int CTank::MapxyTonXnY()
{
	int ret=0;
	m_nX= (x==0 ? 0 : x/TANKMAPCELL);
	m_nY= (y==0 ? 0 : y/TANKMAPCELL);
	return ret;
}

Obj CTank::HasObject(int xpos, int ypos)
{
	Obj object;
    BOOL Result=FALSE;
	CRect rec(xpos,ypos,xpos+TANKWIDTH,ypos+TANKHEIGHT);
	CRect recEgg;
	POSITION position,preposition;
	position=m_pGame->m_Egg.GetHeadPosition();
	CellType celltype;
	while(position)
	{
	   preposition=position;
	   CEgg * p=m_pGame->m_Egg.GetNext(position);
	   recEgg.SetRect(p->m_nXPosition,p->m_nYPosition,p->m_nXPosition+TANKWIDTH,p->m_nYPosition+TANKHEIGHT);
	   CRect rc;
	   Result=rc.IntersectRect(&this->rect,&recEgg);
	   int count=0;
	   if(Result)
	   {
		   switch(p->GetIndex())
		   {
		   case 0:
			    m_nTankLife+=100;
			   	if(Belong()==1||Belong()==2)
				{
					if(m_nTankLife>200)m_nTankLife=200;
				}
				if(Belong()==3)
				{
					switch(m_nSkin)
					{
						case 0:
						case 1:
						case 2:
						case 3:
							if(m_nTankLife>100)m_nTankLife=100;
							break;
						case 4:
						case 5:
							if(m_nTankLife>150)m_nTankLife=150;
							break;					
						case 6:
							if(m_nTankLife>200)m_nTankLife=200;
							break;
						case 7:
							if(m_nTankLife>250)m_nTankLife=250;
							break;			
						case 8:
							if(m_nTankLife>300)m_nTankLife=300;
							break;
					}
				}
				break;
		   case 1:
			   m_nDamage+=50;
			   if(m_nDamage>=300)m_nDamage=300;
			   if(m_nDamage<0)m_nDamage=50;
			   tempDamage=m_nDamage;
			   break;
		   case 2://炸掉所有敌方坦克
			   if(Belong()==3)
				{
				   if(m_pGame->m_tankleft)
				   {
					   int temp=m_pGame->m_tankleft->GetTankLife();
					   temp-=100;			  
						CExplode *pe=new CExplode;
						if(pe)
						{
							pe->SetExplodeBmp(m_pGame->m_pExplodeBuffer);
							pe->SetPostion(m_pGame->m_tankleft->Getx()-12,m_pGame->m_tankleft->Gety()-12);
							m_pGame->m_Explode.AddTail(pe);
							m_pGame->PlaySound();
						}
	 					if(temp<=0)
						{
						delete  m_pGame->m_tankleft;
						m_pGame->m_tankleft=NULL;
                        m_pGame->m_Player.RemoveHead();
						}                        
					   else m_pGame->m_tankleft->SetTankLife(temp);
					}
                    if(m_pGame->m_tankright)
					{
					   int temp=m_pGame->m_tankright->GetTankLife();
					   temp-=100;
					   	CExplode *pe=new CExplode;
						if(pe)
						{
							pe->SetExplodeBmp(m_pGame->m_pExplodeBuffer);
							pe->SetPostion(m_pGame->m_tankright->Getx()-12,m_pGame->m_tankright->Gety()-12);
							m_pGame->m_Explode.AddTail(pe);
							m_pGame->PlaySound();
						}
						if(temp<=0)
						{
							delete  m_pGame->m_tankright;
							m_pGame->m_tankright=NULL;
							m_pGame->m_Player.RemoveTail();
						}                        
					   else m_pGame->m_tankright->SetTankLife(temp);
					}
				   break;
			   }
				position=m_pGame->m_Enemy.GetHeadPosition();
  				while(position)
				{
					CTank * pm=m_pGame->m_Enemy.GetNext(position);
					CExplode *pe=new CExplode;
					if(pe)
					{
						pe->SetExplodeBmp(m_pGame->m_pExplodeBuffer);
						pe->SetPostion(pm->Getx()-12,pm->Gety()-12);
						m_pGame->m_Explode.AddTail(pe);
						m_pGame->PlaySound();
					}
					delete pm;
					count++;
				}
				m_pGame->m_Enemy.RemoveAll();
				if(Belong()==1)
				{
					m_pGame->m_nLeftKills+=count;
					m_pGame->m_nLeftScore+=500;
				}
				else
				{
					m_pGame->m_nRightKills+=count;
					m_pGame->m_nRightScore+=500;
				}
				break;
		   case 3://增加坦克人数
			   switch(Belong())
			   {
			   case 1:
				   m_pGame->m_nLeftPlayer++;
				   if(m_pGame->m_nLeftPlayer>9)
				   {
					   m_pGame->m_nLeftPlayer=9;
					   m_pGame->m_nLeftScore+=100;
				   }
				   break;
			   case 2:
				   m_pGame->m_nRightPlayer++;
 				   if(m_pGame->m_nRightPlayer>9)
				   {
					   m_pGame->m_nRightPlayer=9;
					   m_pGame->m_nRightPlayer+=100;
				   }
				   break;
			   case 3:
				   m_pGame->m_nLeftEnemy++;
				   break;
			   }
			   break;
		   case 4://提高坦克速度
			   AddTankSpeed();
			   break;
		   case 5://提高发弹数量
			   m_nDoubleBullet++;
			   if(m_nDoubleBullet>3)
			   {
				   m_nDoubleBullet=3;
				   switch(Belong())
				   {
				   case 1:
					   m_pGame->m_nLeftScore+=100;
					   break;
				   case 2:
					   m_pGame->m_nRightScore+=100;
					   break;
				   }
			   }
			   break;
		   case 6://提高发弹频率
			   m_nFireSpeed=max(m_nFireSpeed-100,200);
			   break;
		   case 7:
			   switch(Belong())
			   {
			   case 1:
				   m_pGame->m_nLeftScore+=1000;
				   break;
			   case 2:
				   m_pGame->m_nRightScore+=1000;
				   break;
			   }				   
			   break;
		   case 8:
			 switch(Belong())
			   {
			   case 1:
				   m_pGame->m_nLeftScore+=500;
				   break;
			   case 2:
				   m_pGame->m_nRightScore+=500;
				   break;
			   }				   
			   break;
		   case 9:
			    switch(Belong())
			   {
			   case 1:
				   m_pGame->m_nLeftScore+=200;
				   break;
			   case 2:
				   m_pGame->m_nRightScore+=200;
				   break;
			   }				   
			   break;
		case 10:
			   switch(Belong())
			   {
			   case 1:
				   m_pGame->m_nLeftScore+=100;
				   break;
			   case 2:
				   m_pGame->m_nRightScore+=100;
				   break;
			   }				  	
		case 11:
			sort=1;
			number=30;
			m_nDamage=10000000;
			break;
		case 12:
		    m_nTankLife=1;
			break;
		case 13:	
			if(Belong()!=3)
			{
				CellType temp;
				temp.cell=CT_STEEL;
				temp.mask=0;
				temp.defence=100000;
				m_pGame->m_TankMap.SetHeadQuarterBorder(temp);
				m_pGame->m_nBorderTime=2000;
			}
		   else
		   {
			   CellType temp;
     			temp.cell=CT_EMPTY;
				temp.mask=0;
				temp.defence=0;
				m_pGame->m_TankMap.SetHeadQuarterBorder(temp);
				m_pGame->m_nBorderTime=2000;
			}
          break;
		case 14:
			if(Belong()!=3)m_pGame->m_nFreezeTime=700;
			else m_pGame->m_bFreeze=TRUE;
			break;
		}
		m_pGame->PlaySound(3);
	    delete p;
	    m_pGame->m_Egg.RemoveAt(preposition);
	    break;
		}
	}
	   
	position=m_pGame->m_Player.GetHeadPosition();
  	while(position)
	{
		preposition=position;
		CTank * p=m_pGame->m_Player.GetNext(position);
		if(p!=this)
		{
			CRect rc;
			Result=rc.IntersectRect(&rec,&p->rect);
			if(Result)
			{
				object.position=preposition;
				object.sort=1;
				object.has=TRUE;
                return object;
			}	   
		}
	}
	position=m_pGame->m_Enemy.GetHeadPosition();
  	while(position)
	{
		preposition=position;
		CTank * p=m_pGame->m_Enemy.GetNext(position);
		if(p!=this)
		{
			CRect rc;
			Result=rc.IntersectRect(&rec,&p->rect);
			if(Result)
			{
				object.position=preposition;
				object.sort=2;
				object.has=TRUE;
                return object;
			}	   
		}
	}
//专门针对坦克新出现时有可能出现的坦克重叠现象的处理方法
	if(m_pGame->m_tankleft==NULL && m_pGame->m_nLeftPlayer>0)
	{
		CRect rc,rc1;
		rc1.SetRect(15*TANKMAPCELL,38*TANKMAPCELL,15*TANKMAPCELL+TANKWIDTH,38*TANKMAPCELL+TANKHEIGHT);
		Result=rc.IntersectRect(&rec,rc1);
		if(Result)
		{
			object.has=TRUE;
			object.sort=1;
			return object;
		}
	}
   if(m_pGame->m_tankright==NULL && m_pGame->m_nRightPlayer>0)
	{
		CRect rc,rc1;
		rc1.SetRect(23*TANKMAPCELL,38*TANKMAPCELL,23*TANKMAPCELL+TANKWIDTH,38*TANKMAPCELL+TANKHEIGHT);
		Result=rc.IntersectRect(&rec,rc1);
		if(Result)
		{
			object.has=TRUE;
			object.sort=1;
			return object;
		}
	}
       
	return object;
}
 
BYTE CTank::Belong()
{
  if(m_pGame->m_tankleft==this){return 1;}
  if(m_pGame->m_tankright==this){return 2;}
  return 3;  
}

void CTank::Init(CGame* pGame,int X,int Y,LPDIRECTDRAWSURFACE Tankbmp,LPDIRECTDRAWSURFACE TankStatebmp,int nSkin)
{
	SetGame(pGame);
	SetTankBmp(Tankbmp);
	SetTankStateBmp(TankStatebmp);
	SetSkin(nSkin);
	SetTankPos(X,Y);
	SetRect();
}
void CTank::SetSkin(int nSkin)
{
	BYTE t=Belong();
	if(t==1 || t==2)
	{
		if(nSkin >= NUM_PLAYERTANKSKINS || nSkin<0)
			nSkin=0;
	}
	else if(t==3)
	{
		if(nSkin>=NUM_ENEMYTANKSKINS || nSkin<0)
			nSkin=8;
		switch(nSkin)
		{
		case 0:
			m_nDamage=20;
			SetTankLife(100);
			break;
		case 1:
			m_nDamage=25;
			SetTankLife(100);
			break;
		case 2:
			m_nDamage=30;
			SetTankLife(100);
			break;
		case 3:
			m_nDamage=35;
			SetTankLife(100);
			break;
		case 4:
			m_nDamage=40;
			SetTankLife(150);
			break;
		case 5:
			m_nDamage=40;
			SetTankLife(150);
			break;
		case 6:
			m_nDamage=50;
			SetTankLife(200);
			break;
		case 7:
			m_nDamage=70;
			SetTankLife(250);
			break;
		case 8:
			m_nDamage=100;
			SetTankLife(300);
			break;
		}

	}
	m_nSkin=nSkin;
}


BOOL CTank::HasBlock(int sx, int sy, int ex, int ey)
{
	for(int i=sx;i<=ex;i++)
		for(int j=sy;j<=ey;j++)
		{
			CellType &celltype= m_pGame->m_TankMap.GetCellType(i,j);
				if(celltype.cell==CT_SOIL || celltype.cell==CT_STEEL ||celltype.cell==CT_STONE)
					return TRUE;
		}
	return FALSE;
}

BOOL CTank::DownCanGo()
{
	for(int i=m_nX;i<=m_nX+2;i++)
		for(int j=m_nY+2;j<=m_nY+2;j++)
		{
			if(j>=TANKMAPHEIGHT || i>=TANKMAPWIDTH)return FALSE;
			CellType &ct=m_pGame->m_TankMap.GetCellType(i,j);
			if(ct.cell==CT_SOIL||ct.cell==CT_STONE||ct.cell==CT_STEEL||ct.cell==CT_WATER)
				return FALSE;
		}
	return TRUE;
}
