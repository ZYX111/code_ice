// NormalBullet.cpp: implementation of the CNormalBullet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tank.h"
#include "NormalBullet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNormalBullet::CNormalBullet()
{
}

CNormalBullet::~CNormalBullet()
{

}

BOOL CNormalBullet::Choose(int sort)
{
	if(sort<0||sort>=5)return FALSE;
	index=sort;
	return TRUE;
}
BOOL CNormalBullet::BulletTrack()
{
	VERIFY(index!=-1);
	ASSERT(m_pGame);
	Result rst;
	Obj object;
	switch(m_nDirection)
	{
	case UP:
		object=HasObject(x,y-m_nSpeed);
		if(object.has)
		{
			DestroyObj(object);
			return FALSE;
		}
		else
		{
			rst=IsWhat(x,y-m_nSpeed);
			if(rst.bCanGo)
			{
				y-=m_nSpeed;
				if(y<0)y=0;
			}
			else
			{
				int temp=this->GetDamage();
				int m=0;
				for(int i=0;i<rst.num;i++)
				{
				    CellType&	type=m_pGame->m_TankMap.GetCellType(rst.position[i].x,rst.position[i].y);
					if(type.mask==1)return FALSE;
					m=max(type.defence,m);
					type.defence-=this->GetDamage();
					if(type.defence<=0)
					{
						type.defence=0;
						m_pGame->m_TankMap.SetMapCell(rst.position[i].x,rst.position[i].y,CT_EMPTY);			
					}
				}
				temp-=m;
				if(temp>0 && rst.num)//rst.num 解决边界问题
				{
					this->SetDamage(temp);
					return TRUE;
				}
				return FALSE;
			}
		}
		break;
	case DOWN:
		object=HasObject(x,y+m_nSpeed);
		if(object.has)
		{
			DestroyObj(object);
			return FALSE;
		}
		else
		{
			rst=IsWhat(x,y+m_nSpeed);
			if(rst.bCanGo)
			{
				y+=m_nSpeed;
				if(y>=SCREEN_HEIGHT-BULLETHEIGHT)y=SCREEN_HEIGHT-BULLETHEIGHT;
			}
			else 
			{
				int temp=this->GetDamage();
				int m=0;
				for(int i=0;i<rst.num;i++)
				{
					CellType& type=m_pGame->m_TankMap.GetCellType(rst.position[i].x,rst.position[i].y);
					if(type.mask==1)return FALSE;

					m=max(type.defence,m);
					type.defence-=this->GetDamage();
					if(type.defence<=0)
					{
						if(type.cell==CT_GENERAL)
							m_pGame->SetGameState(GAMEOVER);
						type.defence=0;
						m_pGame->m_TankMap.SetMapCell(rst.position[i].x,rst.position[i].y,CT_EMPTY);			
					}
				}
				temp-=m;
				if(temp>0 && rst.num )
				{
					this->SetDamage(temp);
					return TRUE;
				}
				return FALSE;
			}
		}
		break;
	case RIGHT:
		object=HasObject(x+m_nSpeed,y);
		if(object.has)
		{
			DestroyObj(object);
			return FALSE;
		}
		else
		{
			rst=IsWhat(x+m_nSpeed,y);
			if(rst.bCanGo)
			{
				x+=m_nSpeed;	
				if(x>=SCREEN_WIDTH-BULLETWIDTH)x=SCREEN_WIDTH-BULLETWIDTH;
			}
			else
			{
				int temp=this->GetDamage();
				int m=0;
				for(int i=0;i<rst.num;i++)
				{
					CellType& type=m_pGame->m_TankMap.GetCellType(rst.position[i].x,rst.position[i].y);
					if(type.mask==1)return FALSE;

					m=max(type.defence,m);
					type.defence-=this->GetDamage();
					if(type.defence<=0)
					{
						if(type.cell==CT_GENERAL)
							m_pGame->SetGameState(GAMEOVER);
						type.defence=0;
						m_pGame->m_TankMap.SetMapCell(rst.position[i].x,rst.position[i].y,CT_EMPTY);			
					}
				}
				temp-=m;
				if(temp>0 &&rst.num)
				{
					this->SetDamage(temp);
					return TRUE;
				}
				return FALSE;
			}
		}
		break;
	case LEFT:
		object=HasObject(x-m_nSpeed,y);
		if(object.has)
		{
			DestroyObj(object);
			return FALSE;
		}
		else
		{
			rst=IsWhat(x-m_nSpeed,y);
			if(rst.bCanGo)
			{
				x-=m_nSpeed;	
				if(x<0)x=0;
			}
			else
			{
				int temp=this->GetDamage();
				int m=0;
				for(int i=0;i<rst.num;i++)
				{
					CellType& type=m_pGame->m_TankMap.GetCellType(rst.position[i].x,rst.position[i].y);
					if(type.mask==1)return FALSE;

					m=max(type.defence,m);
					type.defence-=this->GetDamage();
					if(type.defence<=0)
					{
						if(type.cell==CT_GENERAL)
							m_pGame->SetGameState(GAMEOVER);
						type.defence=0;
						m_pGame->m_TankMap.SetMapCell(rst.position[i].x,rst.position[i].y,CT_EMPTY);			
					}
				}
				temp-=m;
				if(temp>0 && rst.num)
				{
					this->SetDamage(temp);
					return TRUE;
				}
				return FALSE;
			}
		}
		break;
	}
	this->SetRect();
	return TRUE;
}



