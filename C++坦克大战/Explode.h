// Explode.h: interface for the CExplode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPLODE_H__3BAF2206_7621_4834_A50A_4CFB44756B92__INCLUDED_)
#define AFX_EXPLODE_H__3BAF2206_7621_4834_A50A_4CFB44756B92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//##ModelId=3EBA49A20232
class CExplode  
{
public:
	//##ModelId=3EBA49A20258
	BOOL Init(int xt,int yt,int iCycles,int iTotalBmps,LPDIRECTDRAWSURFACE bmp,int iw,int ih,int iSort,int iSide);
	//##ModelId=3EBA49A20257
	BOOL Burn();
	//##ModelId=3EBA49A20254
	void SetPostion(int xt,int yt);
	//##ModelId=3EBA49A20252
	void DrawExplode(LPDIRECTDRAWSURFACE pdds);
	//##ModelId=3EBA49A20251
	CExplode();
	//##ModelId=3EBA49A2024F
	virtual ~CExplode();

	//##ModelId=3EBA49A2024D
	void SetExplodeBmp(LPDIRECTDRAWSURFACE bmp){m_bmpExplode=bmp;}
	//##ModelId=3EBA49A2024C
	LPDIRECTDRAWSURFACE GetExplodeBmp(){return m_bmpExplode;}

	//##ModelId=3EBA49A2024B
	int GetSort(){return m_nSort;}
	//##ModelId=3EBA49A2024A
	int GetSide(){return m_nSide;}
private:
	//##ModelId=3EBA49A20247
	LPDIRECTDRAWSURFACE m_bmpExplode;
	//##ModelId=3EBA49A20244
	int m_nTotalBmps;
	//##ModelId=3EBA49A20243
	int m_nIndex;
	int x , y;
	int w,h;
	//##ModelId=3EBA49A20242
	int m_nSort;
	//##ModelId=3EBA49A20235
	int m_nCycles;
	//##ModelId=3EBA49A20234
	int m_nTimes;
	//##ModelId=3EBA49A20233
	int m_nSide;		//3: enemy 1:left 2:right
};

#endif // !defined(AFX_EXPLODE_H__3BAF2206_7621_4834_A50A_4CFB44756B92__INCLUDED_)
