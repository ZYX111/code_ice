// TankMap.cpp: implementation of the CTankMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tank.h"
#include "TankMap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTankMap::CTankMap()
{
	m_bmpSoil=NULL;
	m_bmpStone=NULL;
	m_bmpSteel=NULL;
	m_bmpWater=NULL;
	m_bmpIce=NULL;
	m_bmpTree=NULL;
	m_bLockState=false;
	memset(m_nGrid,0,sizeof(CellType)*TANKMAPWIDTH*TANKMAPHEIGHT);
}

CTankMap::~CTankMap()
{
//	DestroyBmp();
}

bool CTankMap::LoadMapFromFile(LPCTSTR fn)
{
	CStdioFile file;
	if(!file.Open( fn, CFile::modeRead | CFile::typeText ) )
		return false;
	CString str;
	int i=0,j=0;
	while(file.ReadString(str)||i<TANKMAPWIDTH)
	{
         if(str.GetLength()!=0)
		 {
			m_nGrid[i][j].cell=(Cell)atoi(str.GetBuffer(str.GetLength()));
			switch(m_nGrid[i][j].cell)
			{
				case CT_SOIL:
                     m_nGrid[i][j].defence=100;
					 break;
				case CT_STONE:
					 m_nGrid[i][j].defence=150;
					 break;
				case CT_STEEL:
					 m_nGrid[i][j].defence=10000;
					 break;
				case CT_GENERAL:
					 m_nGrid[i][j].defence=50;
					 break;
			}
			m_nGrid[i][j].mask=0;
			str.ReleaseBuffer();
		 }
		j++;
		if(j==TANKMAPHEIGHT)
		{
			j=0;
			i++;
			if(i==TANKMAPWIDTH)
			{
				i=0;
				break;
			}
		}


	}			
	file.Close();
	return true;
}

void CTankMap::SetHeadQuarterBorder(CellType type)
{

	int i,j;
	for(i=17,j=36;j<40;j++)
		m_nGrid[i][j]=type;
	for(i=22,j=36;j<40;j++)
		m_nGrid[i][j]=type;
	for(i=17,j=36;i<23;i++)
		m_nGrid[i][j]=type;
}

void CTankMap::SetMapCell(int x,int y,Cell type,BYTE mask)
{
	if(m_bLockState==false)
	{
		if( x<0 || x>=TANKMAPWIDTH)return;
		if( y<0 || y>=TANKMAPHEIGHT)return;
		m_nGrid[x][y].cell=type;
		m_nGrid[x][y].mask=mask;

	}
}

void CTankMap::DrawShield(LPDIRECTDRAWSURFACE pdds)
{
	RECT rc;
	rc.left=0;
	rc.right=rc.left+TANKMAPCELL;
	rc.top=0;
	rc.bottom=rc.top+TANKMAPCELL;
	int xs;
	int ys;
	for(int i=0;i<TANKMAPWIDTH;i++)
		for(int j=0;j<TANKMAPHEIGHT;j++)
			if(m_nGrid[i][j].cell==CT_TREE)
			{
				xs=i*TANKMAPCELL;
				ys=j*TANKMAPCELL;
				pdds->BltFast( xs, ys, m_bmpTree, &rc, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );
			}
}

void CTankMap::DrawMap(LPDIRECTDRAWSURFACE pdds)
{
	for(int i=0;i<TANKMAPWIDTH;i++)
		for(int j=0;j<TANKMAPHEIGHT;j++)
			DrawCell(i,j,pdds);

}

void CTankMap::DrawCell(int x, int y,LPDIRECTDRAWSURFACE pdds)
{
	if( x<0 || x>=TANKMAPWIDTH)return;
	if( y<0 || y>=TANKMAPHEIGHT)return;
	RECT rc;
	int xs=x*TANKMAPCELL;
	int ys=y*TANKMAPCELL;
	rc.left=0;
	rc.right=rc.left+TANKMAPCELL;
	rc.top=0;
	rc.bottom=rc.top+TANKMAPCELL;
	switch(m_nGrid[x][y].cell)
	{
	case CT_SOIL:
		if(m_nGrid[x][y].defence <= 60)
		{
			rc.left+=TANKMAPCELL;
			rc.right+=TANKMAPCELL;
		}
		pdds->BltFast( xs, ys, m_bmpSoil, &rc, DDBLTFAST_WAIT );
		break;
	case CT_STONE:
		if(m_nGrid[x][y].defence <= 70)
		{
			rc.left+=TANKMAPCELL;
			rc.right+=TANKMAPCELL;
		}
		pdds->BltFast( xs, ys, m_bmpStone, &rc, DDBLTFAST_WAIT );
		break;
	case CT_STEEL:
		pdds->BltFast( xs, ys, m_bmpSteel, &rc, DDBLTFAST_WAIT );
		break;
	case CT_WATER:
		pdds->BltFast( xs, ys, m_bmpWater, &rc, DDBLTFAST_WAIT );
		break;
	case CT_ICE:
		pdds->BltFast( xs, ys, m_bmpIce, &rc, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );
		break;
	}
}

bool CTankMap::WriteMapToFile(LPCTSTR fn)
{
	CStdioFile file;
	if(!file.Open( fn, CFile::modeCreate | CFile::modeWrite | CFile::typeText ) )
		return false;
	CString str;
	for(int i=0;i<TANKMAPWIDTH;i++)
		for(int j=0;j<TANKMAPHEIGHT;j++)
		{
			str.Format("%d",m_nGrid[i][j].cell);
			str+="\n";
			file.WriteString(str);
		}			
	file.Close();
	return true;
}

CellType& CTankMap::GetCellType(int x, int y)
{
	ASSERT( x>=0 && x<TANKMAPWIDTH);
	ASSERT( y>=0 && y<TANKMAPHEIGHT);
	return m_nGrid[x][y];
}

void CTankMap::DestroyBmp()
{
	if(m_bmpSoil)
	{
		m_bmpSoil->Release();
		m_bmpSoil=NULL;
	}
	if(m_bmpStone)
	{
		m_bmpStone->Release();
		m_bmpStone=NULL;
	}
	if(m_bmpSteel)
	{
		m_bmpSteel->Release();
		m_bmpSteel=NULL;
	}
	if(m_bmpWater)
	{
		m_bmpWater->Release();
		m_bmpWater=NULL;
	}
	if(m_bmpIce)
	{
		m_bmpIce->Release();
		m_bmpIce=NULL;
	}
	if(m_bmpTree)
	{
		m_bmpTree->Release();
		m_bmpTree=NULL;
	}
}

void CTankMap::Lock()
{
	m_bLockState=true;
}

void CTankMap::UnLock()
{
	m_bLockState=false;
}

void CTankMap::ResetSurface()
{
	if(m_bmpSoil)
		m_bmpSoil->Restore();
	if(m_bmpStone)
		m_bmpStone->Restore();
	if(m_bmpSteel)
		m_bmpSteel->Restore();
	if(m_bmpWater)
		m_bmpWater->Restore();
	if(m_bmpIce)
		m_bmpIce->Restore();
	if(m_bmpTree)
		m_bmpTree->Restore();

}
