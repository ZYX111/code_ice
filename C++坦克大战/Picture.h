// Picture.h: interface for the CPicture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICTURE_H__CA0FAB0B_DDDB_41D6_829D_577464AE54EE__INCLUDED_)
#define AFX_PICTURE_H__CA0FAB0B_DDDB_41D6_829D_577464AE54EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//##ModelId=3EBA49A10128
class CPicture  
{
public:
	//##ModelId=3EBA49A1014A
	bool Draw(CDC * pDC,CPoint pos,int x,int y,CSize size);
	//##ModelId=3EBA49A10149
	CPicture();
	//##ModelId=3EBA49A10148
	virtual ~CPicture();

	//##ModelId=3EBA49A10143
	bool			Load(CString sResourceType, CString sResource);
	//##ModelId=3EBA49A10146
	bool			Load(CString sFileName);
	//##ModelId=3EBA49A10150
	bool			Draw(CDC* pDC);
	//##ModelId=3EBA49A10152
	bool			Draw(CDC* pDC, CPoint Pos);
	//##ModelId=3EBA49A10155
	bool			Draw(CDC* pDC, CPoint Pos, CSize Size);
	//##ModelId=3EBA49A10159
	bool			Draw(CDC* pDC, double nSizeRatio);
	//##ModelId=3EBA49A1015C
	bool			Draw(CDC* pDC, CPoint Pos, double nSizeRatio);
	//##ModelId=3EBA49A10141
	CSize			GetSize(CDC* pDC);
private:
	//##ModelId=3EBA49A1013C
	static	bool	GetResource(LPSTR lpName, LPSTR lpType, void* pResource, int& nBufSize);
	//##ModelId=3EBA49A1013B
	void			UnLoad();
	//##ModelId=3EBA49A10138
	bool			LoadFromBuffer(BYTE* pBuff, int nSize);
	//##ModelId=3EBA49A10160
	bool			Draw(CDC* pDC, int x, int y, int cx, int cy);
	//##ModelId=3EBA49A1012B
	IPicture*		m_pPicture;
	enum
	{
		HIMETRIC_INCH	= 2540
	};
};

#endif // !defined(AFX_PICTURE_H__CA0FAB0B_DDDB_41D6_829D_577464AE54EE__INCLUDED_)
