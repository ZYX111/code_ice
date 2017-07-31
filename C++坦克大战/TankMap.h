// TankMap.h: interface for the CTankMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TANKMAP_H__92B11BDE_6E4B_40DB_A995_2FF1754DBEDB__INCLUDED_)
#define AFX_TANKMAP_H__92B11BDE_6E4B_40DB_A995_2FF1754DBEDB__INCLUDED_

#include "Tank_Defs.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEgg;
//##ModelId=3EBA49A10271
class CTankMap
{
public:
	//##ModelId=3EBA49A10312
	void DrawShield(LPDIRECTDRAWSURFACE pdds);
	//##ModelId=3EBA49A10311
	void ResetSurface();
	//##ModelId=3EBA49A10310
	void UnLock();
	//##ModelId=3EBA49A1030F
	void Lock();
	//##ModelId=3EBA49A1030E
	void DestroyBmp();
	//##ModelId=3EBA49A10300
	CellType& GetCellType(int x,int y);
	//##ModelId=3EBA49A102FE
	bool WriteMapToFile(LPCTSTR fn);
	//##ModelId=3EBA49A102EE
	void SetMapCell(int x,int y,Cell type,BYTE mask=0);
	//##ModelId=3EBA49A102E0
	void DrawCell(int x,int y,LPDIRECTDRAWSURFACE pdds);
	//##ModelId=3EBA49A102DE
	void DrawMap(LPDIRECTDRAWSURFACE pdds);
	//##ModelId=3EBA49A102D1
	void SetHeadQuarterBorder(CellType type);
	//##ModelId=3EBA49A102CF
	bool LoadMapFromFile(LPCTSTR fn);

	//##ModelId=3EBA49A102C3
	void SetSoilBmp(LPDIRECTDRAWSURFACE bmp){m_bmpSoil=bmp;}
	//##ModelId=3EBA49A102C1
	void SetStoneBmp(LPDIRECTDRAWSURFACE bmp){m_bmpStone=bmp;}
	//##ModelId=3EBA49A102BF
	void SetSteelBmp(LPDIRECTDRAWSURFACE bmp){m_bmpSteel=bmp;}
	//##ModelId=3EBA49A102B5
	void SetWaterBmp(LPDIRECTDRAWSURFACE bmp){m_bmpWater=bmp;}
	//##ModelId=3EBA49A102B3
	void SetIceBmp(LPDIRECTDRAWSURFACE bmp){m_bmpIce=bmp;}
	//##ModelId=3EBA49A102B1
	void SetTreeBmp(LPDIRECTDRAWSURFACE bmp){m_bmpTree=bmp;}

	//##ModelId=3EBA49A102B0
	LPDIRECTDRAWSURFACE GetSoilBmp(){return m_bmpSoil;}
	//##ModelId=3EBA49A102AF
	LPDIRECTDRAWSURFACE GetStoneBmp(){return m_bmpStone;}
	//##ModelId=3EBA49A102A9
	LPDIRECTDRAWSURFACE GetSteelBmp(){return m_bmpSteel;}
	//##ModelId=3EBA49A102A8
	LPDIRECTDRAWSURFACE GetWaterBmp(){return m_bmpWater;}
	//##ModelId=3EBA49A102A7
	LPDIRECTDRAWSURFACE GetIceBmp(){return m_bmpIce;}
	//##ModelId=3EBA49A102A6
	LPDIRECTDRAWSURFACE GetTreeBmp(){return m_bmpTree;}


	//##ModelId=3EBA49A102A5
	CTankMap();
	//##ModelId=3EBA49A102A4
	virtual ~CTankMap();
	//##ModelId=3EBA49A102A1
	CellType m_nGrid[TANKMAPWIDTH][TANKMAPHEIGHT];				//地图网格
private:
	//##ModelId=3EBA49A1029A
	bool m_bLockState;
	//##ModelId=3EBA49A10297
	LPDIRECTDRAWSURFACE m_bmpSoil;
	//##ModelId=3EBA49A10292
	LPDIRECTDRAWSURFACE m_bmpStone;
	//##ModelId=3EBA49A1028C
	LPDIRECTDRAWSURFACE m_bmpSteel;
	//##ModelId=3EBA49A10287
	LPDIRECTDRAWSURFACE m_bmpWater;
	//##ModelId=3EBA49A10282
	LPDIRECTDRAWSURFACE m_bmpIce;
	//##ModelId=3EBA49A1027B
	LPDIRECTDRAWSURFACE m_bmpTree;

	//##ModelId=3EBA49A10276
	CEgg* m_pEgg;												//彩蛋数组
	//##ModelId=3EBA49A10273
	int m_nEggNum;												//当前彩蛋数

	//##ModelId=3EBA49A10272
	int m_nLevel;												//当前关

};

#endif // !defined(AFX_TANKMAP_H__92B11BDE_6E4B_40DB_A995_2FF1754DBEDB__INCLUDED_)
