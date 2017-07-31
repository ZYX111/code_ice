// Egg.h: interface for the CEgg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EGG_H__DCB7E2E3_2AAB_470A_B473_43527977FFC8__INCLUDED_)
#define AFX_EGG_H__DCB7E2E3_2AAB_470A_B473_43527977FFC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//##ModelId=3EBA499E0138
class CEgg  
{
public:
	//##ModelId=3EBA499E0158
	int	 m_nXPosition;//�ʵ��ĺ�����λ��
	//##ModelId=3EBA499E0157
	int  m_nYPosition;//�ʵ���������λ��
	//##ModelId=3EBA499E0162
	void RandomSet();
	//##ModelId=3EBA499E0160
	void DrawEgg(LPDIRECTDRAWSURFACE pdds);
	//##ModelId=3EBA499E015E
	void SetEggBmp(LPDIRECTDRAWSURFACE bmp );
	//##ModelId=3EBA499E015C
	inline void SetIndex(int n){ASSERT(n>=0&&n<15);index=n;}
	//##ModelId=3EBA499E015B
	int GetIndex(){return index;}
	//##ModelId=3EBA499E015A
	CEgg();
	//##ModelId=3EBA499E0159
	virtual ~CEgg();

	//##ModelId=3EBA499E0150
	int m_nLife;		 //�ʵ�����������
private:
	//##ModelId=3EBA499E014F
	int  m_nTime;//ÿ�γ��ֵ�ʱ����
	//##ModelId=3EBA499E014E
	BOOL m_bHas;//�Ƿ��Ѿ�����
	//##ModelId=3EBA499E014D
	int index;        //�궨�Ǻ��ֲʵ�(bmpλ��)           
	//##ModelId=3EBA499E014A
	LPDIRECTDRAWSURFACE m_bmpEgg;
};

#endif // !defined(AFX_EGG_H__DCB7E2E3_2AAB_470A_B473_43527977FFC8__INCLUDED_)
