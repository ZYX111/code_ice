///////////////////////////////////////////////////////////////////////
/*
  Ŀ�ģ�BMPλͼ�����ࡣ��Դ�����ļ���

  ��������ΰ(SXZ)
  ���ڣ�2000.01
  �汾��2.4

  SXZ software workgroup. No.200001
*/
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
/*
  ע�ͣ�

  # ���е�λͼ���Ǽ����ڵ�ǰ��ʾ����ɫ��ʽ��DDBλͼ
  # ��������λͼ��ʽ�뵱ǰ��ʾ������ɫ��ʽ��ͬ������뺯�����Ƚ���ת��
    Ϊ�����ڵ�ǰ��ʾ����ɫ��ʽ��λͼ֮��ŷ�������
  # ��������BOOL�ͷ��صĺ�����TRUE��ʾ�ɹ���FALSE��ʾʧ��


*/
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>
#include <io.h>
#include <errno.h>
#include <direct.h>

#include "BmpProc.h"


////////////////////////////// defines ///////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// ���ڵ���Ŀ�ļ�������������ʾ�����й����CBmpProc��ʵ������
// ����������static DWORD GetObjectCount()��Ա�������ʸñ���
// ע�⣺�ñ����ͷ������ĺ�����ֻ���ڵ���ģʽ�²Ż���Ч�����
//		 ת��Ϊ����ģʽ�����Ƕ���ʧЧ��������ʹ��GetObjectCount()
//		 �����Ա����ʱ��Ӧ����#ifdef _DEBUG .... #endif����
#ifdef _DEBUG
	DWORD CBmpProc::m_ObjectCount = 0;
#endif




/*************************************************************************
 * ���캯������ʼ����Ϊ�ա�
*************************************************************************/


CBmpProc::CBmpProc()
{
	// ��ʼ����������Ϊ��
	m_mark = FALSE;			// ��Ч��־����TRUE-����Чλͼ������ΪFALSE��
	m_type = IT_NULL;		// ͼ�����͡�
	m_addInfo = 0;			// ������Ϣ��
	m_cFileName.Empty();	// �ļ�·����
	m_pInfo = NULL;			// DIB��Ϣ�ṹ
	m_hObject = NULL;		// �������DDBλͼ������������Ա��

#ifdef _DEBUG
	CBmpProc::m_ObjectCount++;
#endif
}



/*************************************************************************
 * �����ú�������ȡ������CBmpProc��Ŀ��ĸ�����
*************************************************************************
#ifdef _DEBUG
DWORD CBmpProc::GetObjectCount()
{
	return m_ObjectCount;
}
#endif
*************************************************************************/


/*************************************************************************
 * ��������������಻Ϊ�����������
*************************************************************************/


CBmpProc::~CBmpProc()
{
	Clear();

#ifdef _DEBUG
	CBmpProc::m_ObjectCount--;
#endif
}




/*************************************************************************
 *
 * operator=()
 *
 * ����˵����
 * 
 * const CBmpProc& ds		- Դλͼ��
 * 
 * ����ֵ��
 * 
 * CBmpProc&				- ����������
 * 
 * ������
 *
 * ����ȡָ����λͼ��
 * 
 * ��������ɹ���������ԭ����λͼ������еĻ�������ɾ����������ɹ���������
 * ԭλͼ��ִ�б�����֮�󣬿ɵ��ø����IsValid()�����ж��Ƿ�ɹ����ƣ�����
 * ���ֻʹ����û��ͼ����࣬�������ǰ���оʹ���ͼ�񣬿���==�������жϸ���
 * ��Դλͼ���Ƿ���ͬ�������ͬ���ʾ���Ƴɹ�����ͬ��˵��ʧ�ܣ�
 * 
 ************************************************************************/


CBmpProc& CBmpProc::operator=(const CBmpProc& ds)
{
	// �������û��ͼ��ֱ�ӷ���
	if (!ds.IsValid())
		return *this;

	ASSERT(ds.m_pInfo);
	ASSERT(ds.m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	// ����Դͼ��Ϣ��
	LPBITMAPINFO pInfo = (LPBITMAPINFO)CreateMemoryBlockFromAddress(
		(LPVOID)ds.m_pInfo);
	if (!pInfo)
		return *this;

	// ����Դͼ
	CBitmap		tempBmp;
	HBITMAP		hBmp;

	HWND hWnd = ::GetDesktopWindow();
	ASSERT(hWnd);
	HDC hDC = ::GetDC(hWnd);
	CDC dc;
	dc.Attach(hDC);

	if (tempBmp.CreateCompatibleBitmap(&dc, ds.Width(), ds.Height()))
	{
		CDC			compDC, compDC2;
		CBitmap		*pOldBmp, *pOldBmp2;

		// �����뵱ǰ��ʾ�豸���ݵ��ڴ��豸������
		compDC.CreateCompatibleDC(NULL);
		compDC2.CreateCompatibleDC(NULL);

		pOldBmp	= compDC.SelectObject((CBitmap*)&tempBmp);
		pOldBmp2= compDC2.SelectObject((CBitmap*)&ds);

		// ����ָ���ߴ��Դλͼ��Ŀ��λͼ
		compDC.BitBlt(0, 0, ds.Width(), ds.Height(), 
					&compDC2, 0, 0, SRCCOPY);

		compDC2.SelectObject(pOldBmp2);
		compDC.SelectObject(pOldBmp);

		hBmp = (HBITMAP)tempBmp.Detach();
		// ����������ʧ�ܣ�����FALSE
		ASSERT(hBmp);
		if (!hBmp)
		{
			::ReleaseDC(hWnd, dc.Detach());
			::free((void*)pInfo);
			return *this;;
		}
		::ReleaseDC(hWnd, dc.Detach());

		// ɾ��ԭ����ͼ�񣬲��������µ�λͼ
		if (!ClearAndSetData(IT_MEMORY,0,(LPCTSTR)"",pInfo,hBmp))
		{
			::free((void*)pInfo);
			::DeleteObject(hBmp);
			return *this;
		}
		return *this;
	}
	else
	{
		::ReleaseDC(hWnd, dc.Detach());
		::free((void*)pInfo);

		return *this;;
	}
}



/*************************************************************************
 *
 * operator=()
 *
 * ����˵����
 * 
 * const HBITMAP sou		- Դλͼ���
 * 
 * ����ֵ��
 * 
 * CBmpProc&				- ����������
 * 
 * ������
 *
 * ����ȡָ����λͼ
 * 
 * ��������ɹ���������ԭ����λͼ������еĻ�������ɾ����������ɹ���������
 * ԭλͼ��ִ�б�����֮�󣬿ɵ��ø����IsValid()�����ж��Ƿ�ɹ����ƣ�����
 * ���ֻʹ����û��ͼ����࣬�������ǰ���оʹ���ͼ�񣬿���==�������жϸ���
 * ��Դλͼ���Ƿ���ͬ�������ͬ���ʾ���Ƴɹ�����ͬ��˵��ʧ�ܣ�
 * �����ڲ���ָ����λͼ�����Ч������ʧ�ܡ�
 * 
 ************************************************************************/


CBmpProc& CBmpProc::operator=(const HBITMAP sou)
{
	// ��ڼ��
	if (!sou)
		return *this;

	CBmpProc tmp;

	if (!tmp.Attach(sou))
		return *this;

	ASSERT(sou == (HBITMAP)tmp);

	// �����ำֵ����������
	*this = tmp;
	// ����Դλͼ����ֹ������������ɾ��
	tmp.Detach();

	return *this;
}



/*************************************************************************
 *
 * operator==()
 *
 * ����˵����
 * 
 * const CBmpProc& ds		- Դλͼ��
 * 
 * ����ֵ��
 * 
 * BOOL						- �����ͬ����TRUE����ͬ����FALSE
 * 
 * ������
 *
 * �ж�Դλͼ���뱾�����Ƿ���ͬ
 * 
 * �ú���ͨ���Ƚ��������λͼ���ж������Ƿ������ͬ��ͼ��
 * 
 * ע��# ��������඼�ǿյģ�����û��ͼ�񣩣���ú������������ǲ���ͬ��
 *
 ************************************************************************/


BOOL CBmpProc::operator==(const CBmpProc& ds)
{
	// �������������һ�����ǿյģ�����û��ͼ�񣩣���ú�����������
	// �ǲ���ͬ��
	if ((ds.IsValid()==FALSE)||(IsValid()==FALSE))
		return FALSE;

	// �����������Ƚϣ���ֱ�ӷ���TRUE
	if (this == &ds)
		return TRUE;

	ASSERT(m_pInfo);
	ASSERT(ds.m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));
	ASSERT(ds.m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	// ��Ϣ�鳤�ȱȽ�
	if (::_msize(m_pInfo) != ::_msize(ds.m_pInfo))
		return FALSE;

	// ��Ϣ�����ݱȽ�
	if (::memcmp((const void*)m_pInfo, \
		(const void*)ds.m_pInfo,::_msize(m_pInfo)))
		return FALSE;

	// �������DIBλ���ݵĻ�����
	LPSTR pBits1 = (LPSTR)CreateMemoryBlockFromAddress((LPVOID)NULL, 
		CalculateDIBitsSize((LPBITMAPINFOHEADER)m_pInfo));
	if (!pBits1)
		return FALSE;

	LPSTR pBits2 = (LPSTR)CreateMemoryBlockFromAddress((LPVOID)NULL, 
		CalculateDIBitsSize((LPBITMAPINFOHEADER)m_pInfo));
	if (!pBits2)
	{
		::free((void*)pBits1);
		return FALSE;
	}

	// ����λͼ��Ϣ�飬��ΪGetDIBits()�����п��ܻ�ı���ɫ������
	LPBITMAPINFO pInfo = (LPBITMAPINFO)CreateMemoryBlockFromAddress(
		(LPVOID)m_pInfo);
	if (!pInfo)
	{
		::free((void*)pBits1);
		::free((void*)pBits2);
		return FALSE;
	}

	ASSERT(pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	HWND hWnd = ::GetDesktopWindow();
	ASSERT(hWnd);
	HDC hDC;

	// ��ȡλͼ��DIBλ����
	hDC = ::GetDC(hWnd);
	ASSERT(m_hObject);
	ASSERT(ds.m_hObject);
	if (!::GetDIBits(hDC,(HBITMAP)m_hObject,0,pInfo->bmiHeader.biHeight, \
			(LPVOID)pBits1,pInfo,DIB_RGB_COLORS))
	{
		::free((void*)pBits1);
		::free((void*)pBits2);
		::free((void*)pInfo);
		::ReleaseDC(hWnd, hDC);
		return FALSE;
	}
	if (!::GetDIBits(hDC,(HBITMAP)ds.m_hObject,0,pInfo->bmiHeader.biHeight, \
			(LPVOID)pBits2,pInfo,DIB_RGB_COLORS))
	{
		::free((void*)pBits1);
		::free((void*)pBits2);
		::free((void*)pInfo);
		::ReleaseDC(hWnd, hDC);
		return FALSE;
	}
	::ReleaseDC(hWnd, hDC);
	::free((void*)pInfo);

	// λ���ݱȽ�
	LONG leng = (LONG)CalculateDIBitsSize((LPBITMAPINFOHEADER)m_pInfo)/4;
	LPDWORD lp1 = (LPDWORD)pBits1;	// DIBλ������DWORD�����
	LPDWORD lp2 = (LPDWORD)pBits2;

	// ��Ϊ�����memcmp()�����Ƿ���ԱȽϴ���64K�����ݣ����ԲŲ�������
	// �ϱ��ķ�����
	while(leng--)
	{
		if (*lp1 != *lp2) break;
		lp1++; lp2++;
	}

	// �Ƚ���֮��λ���������ã��ͷ�
	::free((void*)pBits1);
	::free((void*)pBits2);
	
	if (leng != -1L)
		return FALSE;	// λ���ݲ�ͬ����FALSE

	return TRUE;
}



/*************************************************************************
 *
 * operator!=()
 *
 * ����˵����
 * 
 * const CBmpProc& ds		- Դλͼ��
 * 
 * ����ֵ��
 * 
 * BOOL						- �����ͬ����TRUE����ͬ����FALSE
 * 
 * ������
 *
 * �ж�Դλͼ���Ƿ��뱾���಻ͬ
 * 
 * �ú���ͨ���Ƚ��������λͼ���ж������Ƿ������ͬ��ͼ��
 * 
 * ע��# ��������඼�ǿյģ�����û��ͼ�񣩣���ú������������ǲ���ͬ��
 *
 ************************************************************************/


BOOL CBmpProc::operator!=(const CBmpProc& ds)
{
	return ((*this) == ds) ? FALSE:TRUE;
}



/*************************************************************************
 *
 * Draw()
 *
 * ����˵����
 *
 * CDC& dc					- ��ʾλͼ���豸������
 * const CRect* rcDst		- Ŀ�����
 * const CRect* rcSrc		- Դ����
 * 
 * ����ֵ��
 *
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ������ָ�������ͼ�����Ŀ���豸�������ָ���������Դ���򣨼����Σ�
 * ��Ŀ������ͬ�������������ѹ������
 * 
 * ���Դ���κ�Ŀ�������NULL,�����������������ζ���������ͼ��ĳߴ�
 * 
 * �������û��ͼ����ʲôҲ������ֱ�ӷ���
 * ��������Ƶ�ͼ���ڵ�ǰ���������ڣ���ֱ�ӷ���
 * 
 ************************************************************************/


BOOL CBmpProc::Draw(CDC& dc, const CRect* rcDst, const CRect* rcSrc)
{
	// �������û��ͼ��ֱ�ӷ���
	if (!IsValid())
		return TRUE;

	ASSERT(m_hObject);

	// ȱʡ���ε���ͼ��ߴ�
	CRect DCRect(Rect());			// Ŀ��λ������
	CRect DibRect(Rect());			// Դλ������

	if (rcDst)
		DCRect = *rcDst;
	if (rcSrc)
		DibRect = *rcSrc;

	// ��������Ƶ�ͼ���ڵ�ǰ���������ڣ���ֱ�ӷ���
	if (!dc.RectVisible(&DCRect))
		return TRUE;

	CDC			compDC;
	CBitmap		*pOldBmp;

	compDC.CreateCompatibleDC(NULL);
	pOldBmp	= compDC.SelectObject((CBitmap*)this);

	/* ����Ŀ��DC������ģʽΪSTRETCH_DELETESCANS��Ҳ���ǲ���ʾ�������ͼ�� */
	int srlold = dc.SetStretchBltMode(STRETCH_DELETESCANS);

	// ��ʾλͼ
	dc.StretchBlt(DCRect.left, DCRect.top, DCRect.Width(), DCRect.Height(), 
					&compDC, DibRect.left, DibRect.top, 
					DibRect.Width(), DibRect.Height(), SRCCOPY);
	
	// �ָ��豸������ԭ��������
	dc.SetStretchBltMode(srlold);
	compDC.SelectObject(pOldBmp);

	return TRUE;
}



/*************************************************************************
 *
 * DrawTile()
 *
 * ����˵����
 *
 * CDC& dc					- ��ʾλͼ���豸������
 * CRect* crArea			- ���跶Χ���������꣩
 * 
 * ����ֵ��
 *
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * �����е�ͼ����ƽ�̷�ʽ����ָ������
 * 
 * �����ڲ���crArea��NULL,��������FALSE������������겻���Ϲ淶������
 * ��ߵ���������ұߵ����꣩�������Զ�����淶���������������Ŀ��
 * ��߶���0������������FALSE
 * 
 * �������û��ͼ����ʲôҲ������ֱ�ӷ���
 * 
 ************************************************************************/


BOOL CBmpProc::DrawTile(CDC& dc, CRect* crArea)
{
	if (!crArea)
		return FALSE;

	// �������û��ͼ��ֱ�ӷ���
	if (!IsValid())
		return TRUE;

	CRect rect = *crArea;
	CRect tprc;

	// ��������淶��
	rect.NormalizeRect();

	if ((rect.Width()==0)||(rect.Height()==0))
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_hObject);

	// ��Ϊ����Ĵ��뽫�ı��豸�������м�����������ã������Ƚ��䱣��
	int savedc = dc.SaveDC();

	CRgn newrgn;
	newrgn.CreateRectRgnIndirect(&rect);
	// ֻ��ʾָ�������пɼ��Ĳ��֣���AND������
	dc.SelectClipRgn(&newrgn, RGN_AND);

	for (int y=rect.top;y<rect.bottom;y+=Height())
	{
		for (int x=rect.left;x<rect.right;x+=Width())
		{
			// ����Ƭ��ʽ�ظ���ʾͼ��
			tprc.SetRect(x,y,x+Width(),y+Height());
			Draw(dc, &tprc);
		}
	}

	dc.RestoreDC(savedc);
	// �ָ��豸�����������֮�󣬽�����ľ����ʾ��ɾ��
	// ��΢�����ĵ�������ע��Ҫ��CGdiObject::DeleteObject()
	//   ������ʾ��ɾ������������֪��ʲôԭ��
	newrgn.CGdiObject::DeleteObject();

	return TRUE;
}



/*************************************************************************
 *
 * DrawCenter()
 *
 * ����˵����
 *
 * CDC& dc					- ��ʾλͼ���豸������
 * CRect* crArea			- ��ʾ��Χ���������꣩
 * 
 * ����ֵ��
 *
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ������ͼ�����Ŀ���豸�������ָ�����򣨾��Σ������룬��ͼ���е����
 * ���е��غ�
 * 
 * ������εĳߴ�С��ͼ��ߴ磬��ͼ������ܲ��ֻ᲻�ɼ�
 * �����ڲ���crArea��NULL������������FALSE
 * �������û��ͼ����ʲôҲ������ֱ�ӷ���
 * ����������겻���Ϲ淶��������ߵ���������ұߵ����꣩�������Զ���
 * ��淶���������������Ŀ�Ȼ�߶���0������������FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::DrawCenter(CDC& dc, CRect* crArea)
{
	if (!crArea)
		return FALSE;

	// �������û��ͼ��ֱ�ӷ���
	if (!IsValid())
		return TRUE;

	CRect rect = *crArea;

	// ��������淶��
	rect.NormalizeRect();

	if ((rect.Width()==0)||(rect.Height()==0))
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_hObject);

	// ȡָ�����ε��е�����
	CPoint cpce = rect.CenterPoint();
	CRect  tprc(CPoint(cpce.x-Width()/2,cpce.y-Height()/2), Size());

	// ��Ϊ����Ĵ��뽫�ı��豸�������м�����������ã������Ƚ��䱣��
	int savedc = dc.SaveDC();

	CRgn newrgn;
	newrgn.CreateRectRgnIndirect(&rect);
	// ֻ��ʾָ�������пɼ��Ĳ��֣���AND������
	dc.SelectClipRgn(&newrgn, RGN_AND);

	Draw(dc, &tprc);

	dc.RestoreDC(savedc);
	newrgn.CGdiObject::DeleteObject();

	return TRUE;
}



/*************************************************************************
 *
 * DrawTranCenter()
 *
 * ����˵����
 *
 * CDC& dc					- ��ʾλͼ���豸������
 * CRect* crArea			- ��ʾ��Χ���������꣩
 * COLORREF crColour		- Ҫ�˵���������ɫ��ȱʡֵ�ǰ�ɫ��
 * int mode					- �����0�򽫲���ʾָ������ɫ�������1����ָ��
 *						      ����ɫ��ȱʡֵΪ0��
 * 
 * ����ֵ��
 *
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ������ͼ��͸���Ļ���Ŀ���豸�������ָ�����򣨾��Σ������룬��ͼ���е�
 * ������е��غ�
 * 
 * ������εĳߴ�С��ͼ��ߴ磬�򳬳����ε�ͼ�񲿷ֻ᲻�ɼ�
 * �����ڲ���crArea��NULL������������FALSE
 * �������û��ͼ����ʲôҲ������ֱ�ӷ���
 * ����������겻���Ϲ淶��������ߵ���������ұߵ����꣩�������Զ���
 * ��淶���������������Ŀ�Ȼ�߶���0������������FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::DrawTranCenter(CDC& dc, 
							  CRect* crArea, 
							  COLORREF crColour, 
							  int mode)
{
	if (!crArea)
		return FALSE;

	// �������û��ͼ��ֱ�ӷ���
	if (!IsValid())
		return TRUE;

	CRect rect = *crArea;

	// ��������淶��
	rect.NormalizeRect();

	if ((rect.Width()==0)||(rect.Height()==0))
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_hObject);

	// ȡָ�����ε��е�����
	CPoint cpce = rect.CenterPoint();
	CRect  tprc(CPoint(cpce.x-Width()/2,cpce.y-Height()/2), Size());

	// ��Ϊ����Ĵ��뽫�ı��豸�������м�����������ã������Ƚ��䱣��
	int savedc = dc.SaveDC();

	CRgn newrgn;
	newrgn.CreateRectRgnIndirect(&rect);
	// ֻ��ʾָ�������пɼ��Ĳ��֣���AND������
	dc.SelectClipRgn(&newrgn, RGN_AND);

	DrawTransparentInRect(dc, crColour, &tprc, NULL, mode);	// ��ʾ͸��ͼ��

	dc.RestoreDC(savedc);
	newrgn.CGdiObject::DeleteObject();

	return TRUE;
}



/*************************************************************************
 *
 * DrawTranTile()
 *
 * ����˵����
 *
 * CDC& dc					- ��ʾλͼ���豸������
 * CRect* crArea			- ��ʾ��Χ���������꣩
 * COLORREF crColour		- Ҫ�˵���������ɫ��ȱʡֵ�ǰ�ɫ��
 * int mode					- �����0�򽫲���ʾָ������ɫ�������1����ָ��
 *						      ����ɫ��ȱʡֵΪ0��
 * 
 * ����ֵ��
 *
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * �����е�ͼ����ƽ�̲�����͸���ķ�ʽ����ָ������
 * 
 * �����ڲ���crArea��NULL,��������FALSE������������겻���Ϲ淶������
 * ��ߵ���������ұߵ����꣩�������Զ�����淶���������������Ŀ��
 * ��߶���0������������FALSE
 * 
 * �������û��ͼ����ʲôҲ������ֱ�ӷ���
 * 
 ************************************************************************/


BOOL CBmpProc::DrawTranTile(CDC& dc, 
							CRect* crArea, 
							COLORREF crColour, 
							int mode)
{
	if (!crArea)
		return FALSE;

	// �������û��ͼ��ֱ�ӷ���
	if (!IsValid())
		return TRUE;

	CRect rect = *crArea;
	CRect tprc;

	// ��������淶��
	rect.NormalizeRect();

	if ((rect.Width()==0)||(rect.Height()==0))
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_hObject);

	// ��Ϊ����Ĵ��뽫�ı��豸�������м�����������ã������Ƚ��䱣��
	int savedc = dc.SaveDC();

	CRgn newrgn;
	newrgn.CreateRectRgnIndirect(&rect);
	// ֻ��ʾָ�������пɼ��Ĳ��֣���AND������
	dc.SelectClipRgn(&newrgn, RGN_AND);

	for (int y=rect.top;y<rect.bottom;y+=Height())
	{
		for (int x=rect.left;x<rect.right;x+=Width())
		{
			// ����Ƭ��ʽ�ظ���ʾͼ��
			tprc.SetRect(x,y,x+Width(),y+Height());
			// ��ʾ͸��ͼ��
			DrawTransparentInRect(dc, crColour, &tprc, NULL, mode);
		}
	}

	dc.RestoreDC(savedc);
	newrgn.CGdiObject::DeleteObject();

	return TRUE;
}



/*************************************************************************
 *
 * Clear()
 *
 * ����˵������
 *
 *
 * ����ֵ����
 *
 *
 * ������
 *
 * ������е�ͼ��
 * 
 * ���������ɾ�����е�λͼ�������������е���������
 * 
 * �������û��ͼ����ʲôҲ������ֱ�ӷ���
 * 
 ************************************************************************/


void CBmpProc::Clear()
{
	// ���������û����Ч��BMP����ֱ�ӷ���
	if (m_mark == FALSE)
	{
		ASSERT(m_type == IT_NULL);
		ASSERT(m_pInfo == NULL);
		ASSERT(m_hObject == NULL);
		return;
	}

	switch(m_type)
	{
	case	IT_NULL:
		ASSERT(FALSE);	// ��ʱm_type��Ӧ����IT_NULL
		break;
	case	IT_DISKFILE:
		ASSERT(!m_cFileName.IsEmpty());	// �ļ���Դ��λͼӦ�����ļ���
	case	IT_RESOURCE:
	case	IT_MEMORY:
	case	IT_CLIP:
	case	IT_CREATE:
		m_cFileName.Empty();

		ASSERT(m_pInfo);
		::free((void*)m_pInfo);
		m_pInfo = NULL;

		// �ȷ����ԭ����BMP�����Ȼ��ɾ��
		ASSERT((HBITMAP)m_hObject);
		::DeleteObject(CGdiObject::Detach());
		m_hObject = NULL;

		m_addInfo = 0;
		m_type = IT_NULL;
		m_mark = FALSE;
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}



/*************************************************************************
 *
 * Attach()
 *
 * ����˵����
 * 
 * HBITMAP hBmp				- ��ճ����λͼ���
 * 
 * ����ֵ��
 * 
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ��ָ����λͼ���뱾�����С�
 * 
 * ע��
 * 
 * # ���hBmp��һ����Ч�ľ������NULL����������FALSE
 * # �����������ԭ������ͼ������ú����ɹ�֮��ԭ����ͼ�񽫱�ɾ��
 * # hBmp������һ�������ڵ�ǰ��ʾ����ɫ��ʽ��DDBλͼ���
 * # �������ִ�гɹ��󣬵������ĳ���Ӧ��ʹ��hBmp�������Ϊ���ѱ���������
 * # �������ʧ�ܣ��������ĳ����Կ�������ʹ��hBmp���
 * 
 ************************************************************************/


BOOL CBmpProc::Attach(HBITMAP hBmp)
{
	ASSERT(hBmp);
	// ���hBmp��NULL������������
	if (!hBmp)
		return FALSE;

	HWND hWnd = ::GetDesktopWindow();
	ASSERT(hWnd);
	HDC hDC;
	BITMAP	bmp;

	// ��ȡ����λͼ�Ŀ�ȡ��߶ȼ���ɫ��ʽ��Ϣ
	if (::GetObject(hBmp, sizeof(BITMAP), (LPVOID)&bmp) != sizeof(BITMAP))
		return FALSE;

	// ������ʱλͼ��Ϣ�飨������ɫ��
	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)CreateMemoryBlockFromAddress(
		(LPVOID)NULL, sizeof(BITMAPINFOHEADER));
	if (!lpbi)
		return FALSE;

	// ����DIB��Ϣ������
	lpbi->biSize = sizeof(BITMAPINFOHEADER);
	lpbi->biWidth = bmp.bmWidth;
	lpbi->biHeight = bmp.bmHeight;
	lpbi->biPlanes = 1;
	// ��16ɫϵͳ�У�ƽ������4����λ����1��������DIB���ǲ������
	lpbi->biBitCount = max(bmp.bmPlanes, bmp.bmBitsPixel);
	if ((lpbi->biBitCount==16)||(lpbi->biBitCount==32))
		lpbi->biCompression = BI_BITFIELDS;
	else
		lpbi->biCompression = BI_RGB;

	// �����豸�ֱ���
	hDC = ::GetDC(hWnd);
	lpbi->biXPelsPerMeter = \
		(GetDeviceCaps(hDC,HORZRES)*1000)/GetDeviceCaps(hDC,HORZSIZE);
	lpbi->biYPelsPerMeter = \
		(GetDeviceCaps(hDC,VERTRES)*1000)/GetDeviceCaps(hDC,VERTSIZE);
	::ReleaseDC(hWnd, hDC);

	lpbi->biClrUsed = 0;
	lpbi->biClrImportant = 0;
	lpbi->biSizeImage = CalculateDIBitsSize(lpbi);

	// ������ɫ��ĳߴ�
	WORD wPalSize = PaletteSize(lpbi);

	// ��������ɫ�����Ϣ�飬������lpbi��Ϣ���е�����
	LPBITMAPINFO pInfo = (LPBITMAPINFO)CreateMemoryBlockFromAddress(
		(LPVOID)lpbi, sizeof(BITMAPINFOHEADER)+wPalSize);
	if (!pInfo)
	{
		::free((void*)lpbi);
		return FALSE;
	}

	ASSERT(pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	// ��ʱ�ýṹ�Ѿ����ã��ͷ�
	::free((void*)lpbi);

	// �������DIBλ���ݵĻ�����
	LPSTR pBits = (LPSTR)CreateMemoryBlockFromAddress((LPVOID)NULL, 
		pInfo->bmiHeader.biSizeImage);
	if (!pBits)
	{
		::free((void*)pInfo);
		return FALSE;
	}

	// ����Ĵ��벢����Ҫ��ȡλͼ��DIBλ���ݣ�����Ҫȡ��λͼ����ɫ��
	// ��ɫ�������ں�����GetDIBits()�����óɹ�����д��pInfo�ṹ��
	hDC = ::GetDC(hWnd);
	if (!::GetDIBits(hDC,hBmp,0,pInfo->bmiHeader.biHeight, \
			(LPVOID)pBits,pInfo,DIB_RGB_COLORS))
	{
		::free((void*)pBits);
		::free((void*)pInfo);
		::ReleaseDC(hWnd, hDC);
		return FALSE;
	}
	::ReleaseDC(hWnd, hDC);

	// ��ʱ������ҪDIBλ���ݣ��ͷ�
	::free((void*)pBits);

	// ɾ��ԭ����ͼ�񣬲��������µ�λͼ
	if (!ClearAndSetData(IT_MEMORY,0,(LPCTSTR)"",pInfo,hBmp))
	{
		::free((void*)pInfo);
		// hBmpָ����λͼ����ɾ��
		return FALSE;
	}

	return TRUE;
}



/*************************************************************************
 *
 * Detach()
 *
 * ����˵������
 * 
 * 
 * ����ֵ��
 * 
 * HBITMAP					- ����ɹ������ط������λͼ��������򷵻�NULL
 * 
 * ������
 *
 * ���������е�λͼ���������
 * 
 * ע��
 * 
 * # �����������ԭ������λͼ�������ɹ�����֮�󣬱����ཫ�����
 * # �����������ԭ��û��λͼ����������NULL
 * 
 ************************************************************************/


HBITMAP CBmpProc::Detach()
{
	if (!IsValid())
		return NULL;

	ASSERT(m_pInfo);
	ASSERT(m_hObject);

	HBITMAP hBmp = (HBITMAP)CGdiObject::Detach();
	ASSERT(hBmp);
	if (!hBmp)
		return NULL;

	m_hObject = NULL;

	switch(m_type)
	{
	case	IT_NULL:
		ASSERT(FALSE);	// ��ʱm_type��Ӧ����IT_NULL
		break;
	case	IT_DISKFILE:
		ASSERT(!m_cFileName.IsEmpty());	// �ļ���Դ��λͼӦ�����ļ���
	case	IT_RESOURCE:
	case	IT_MEMORY:
	case	IT_CLIP:
	case	IT_CREATE:
		m_cFileName.Empty();

		ASSERT(m_pInfo);
		::free((void*)m_pInfo);
		m_pInfo = NULL;

		m_addInfo = 0;
		m_type = IT_NULL;
		m_mark = FALSE;
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return hBmp;
}



/*************************************************************************
 *
 * Save()
 *
 * ����˵����
 * 
 * LPCTSTR lpszNewFileName	- ָ�������ļ�����������NULL��
 * WORD nBitsPerPixel		- ָ��������ɫ��ʽ����ÿ���ض���λ��������0��
 * 
 * ����ֵ��
 * 
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ���������е�λͼ���µĸ�ʽ���µ����Ʊ��浽�ļ���
 * 
 * ע��
 * 
 * # ���û��ָ���µ��ļ�������ڲ���lpszNewFileName��ָ��ΪNULL��������
 *   ��ʹ������ԭ�����ļ������������ԭ��û���ļ�������ʧ�ܣ�����FALSE
 * # ���û��ָ���µ�λ������ڲ���nBitsPerPixel��ָ��Ϊ0����������ʹ����
 *   ��ԭ����λ��ֵ�������ָ����λ��ֵ�Ƿ�����ʧ�ܣ�����FALSE���Ϸ���
 *   λ��ֵ��1,4,8,16,24,32 ����֮һ
 * # �����������ԭ��û��λͼ�����ࣩ����������FALSE
 * # ����ļ�û�д򿪻���д�ļ�ʱ������������FALSE
 * # ����ִ�гɹ�֮�󣬱������е��ļ�����λ������Ϊ��ֵ
 * # ���ۺ����ɹ���񣬱������е�λͼ����ı�
 * 
 ************************************************************************/


BOOL CBmpProc::Save(LPCTSTR lpszNewFileName, WORD nBitsPerPixel)
{
	// �����������ԭ��û��λͼ����������FALSE
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	LPBITMAPINFO pInfo;
	BOOL bRet;
	CString cOldFN;

	// ��ԭ���ĸ�ʽ���浽ԭ�����ļ���
	if ((lpszNewFileName==NULL)&&(nBitsPerPixel==0))
	{
		// Ӧ�����ļ���
		if (m_cFileName.IsEmpty())
			return FALSE;

		return SaveBaseSpecifyFileName((LPCTSTR)m_cFileName);
	}

	// ��ԭ���ĸ�ʽ���浽�µ��ļ���
	if ((lpszNewFileName!=NULL)&&(nBitsPerPixel==0))
	{
		bRet = SaveBaseSpecifyFileName((LPCTSTR)lpszNewFileName);

		if (bRet)
			m_cFileName = lpszNewFileName;

		return bRet;
	}

	// ���µĸ�ʽ���浽ԭ�����ļ���
	if ((lpszNewFileName==NULL)&&(nBitsPerPixel!=0))
	{
		// ���λ���Ϸ���
		if (!IsValidDibFormat(nBitsPerPixel))
			return FALSE;

		// Ӧ�����ļ���
		if (m_cFileName.IsEmpty())
			return FALSE;

		bRet = SaveBaseSpecifyFormat(nBitsPerPixel);

		if (bRet)
		{
			pInfo = GetSpecifyFormatInfo(nBitsPerPixel);
			if (!pInfo)
				return FALSE;
			::free((void*)m_pInfo);
			m_pInfo = pInfo;
		}
		return bRet;
	}

	// ���µĸ�ʽ���浽�µ��ļ���
	if ((lpszNewFileName!=NULL)&&(nBitsPerPixel!=0))
	{
		// ���λ���Ϸ���
		if (!IsValidDibFormat(nBitsPerPixel))
			return FALSE;

		cOldFN = m_cFileName;
		m_cFileName = lpszNewFileName;

		bRet = SaveBaseSpecifyFormat(nBitsPerPixel);

		if (bRet)
		{	// ��ȡָ����ʽ����Ϣ��
			pInfo = GetSpecifyFormatInfo(nBitsPerPixel);
			if (!pInfo)
			{
				m_cFileName = cOldFN;
				return FALSE;
			}
			::free((void*)m_pInfo);
			m_pInfo = pInfo;
		}
		else
			m_cFileName = cOldFN;

		return bRet;
	}

	return FALSE;
}



/*************************************************************************
 *
 * SaveToClipboard()
 *
 * ����˵������
 * 
 * 
 * ����ֵ��
 * 
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ���汾�����е�λͼ��������
 * 
 * ע��
 * 
 * # �����������ԭ��û��λͼ����������FALSE
 * # ����ڴ򿪻�д�������ʱ������������FALSE
 * # ���ۺ����ɹ���񣬱������е����ݶ�����ı�
 * 
 ************************************************************************/


BOOL CBmpProc::SaveToClipboard()
{
	// �����������ԭ��û��λͼ����������FALSE
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	// �򿪼�����
	if (::OpenClipboard(NULL))
	{
		// ��ռ�����
		if (::EmptyClipboard())
		{
			DWORD DibSize = CalculateDIBSize((LPBITMAPINFOHEADER)m_pInfo);
			DWORD infosize =CalculateDIBInfoSize((LPBITMAPINFOHEADER)m_pInfo);

			// ����DIB�ڲ���ʽ�ڴ��
			HANDLE hDib = ::GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, DibSize);
			if (hDib)
			{
				LPSTR lpDib = (LPSTR)::GlobalLock(hDib);
				if (lpDib)
				{
					::memcpy((void*)lpDib, (const void*)m_pInfo, infosize);

					// ��ȡDIBλ����
					HWND hWnd = ::GetDesktopWindow();
					ASSERT(hWnd);
					HDC hDC = ::GetDC(hWnd);
					if (!::GetDIBits(hDC,(HBITMAP)m_hObject,0,m_pInfo->bmiHeader.biHeight,\
							(LPVOID)((DWORD)lpDib+infosize),(LPBITMAPINFO)lpDib,DIB_RGB_COLORS))
					{
						::GlobalUnlock(hDib);
						::GlobalFree(hDib);
						::CloseClipboard();
						::ReleaseDC(hWnd, hDC);
						return FALSE;
					}
					::ReleaseDC(hWnd, hDC);

					::GlobalUnlock(hDib);

					// ��DIB�������������
					if (::SetClipboardData(CF_DIB, hDib))
					{
						// ����ɹ���DIB�����Ӧ�ͷ�
						::CloseClipboard();
						return TRUE;		// �ɹ�
					}
					else
					{
						::GlobalFree(hDib);
						::CloseClipboard();
						return FALSE;		// ʧ��
					}
				}
				::GlobalFree(hDib);
			}
		}
		::CloseClipboard();
	}

	return FALSE;	// ʧ��
}



/*************************************************************************
 *
 * SaveBaseSpecifyFileName()
 *
 * ����˵����
 * 
 * LPCTSTR fn				- ָ�����ļ�����
 * 
 * ����ֵ��
 * 
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ���汾�����е�λͼ��ָ�����ļ���
 * 
 * ע��
 * 
 * # ���ָ�����ļ��Ѿ����ڣ����������Ǹ��ļ�
 * # �����������ԭ��û��λͼ����������FALSE
 * # ����ļ�û�д򿪻���д�ļ�ʱ������������FALSE
 * # ���������޸������ļ���
 * 
 ************************************************************************/


BOOL CBmpProc::SaveBaseSpecifyFileName(LPCTSTR fn)
{
	// �����������ԭ��û��λͼ����������FALSE
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);

	// ���û��ָ���ļ���������FALSE
	if (!fn)
		return FALSE;

	FILE *file;

	// �ô�����ʽ���ļ��������ƣ�
	file = ::fopen(fn, "w+b");
	if (!file)
		return FALSE;
	
	::fseek(file, 0, SEEK_SET);

	BITMAPFILEHEADER	bmf;

	bmf.bfType = DIB_HEADER_MARKER;		//((WORD) ('M' << 8) | 'B')
    bmf.bfSize = CalculateDIBFileSize((LPBITMAPINFOHEADER)m_pInfo);
    bmf.bfReserved1 = 0; 
    bmf.bfReserved2 = 0; 
    bmf.bfOffBits   = CalculateDIBitsOff((LPBITMAPINFOHEADER)m_pInfo);

	// д���ļ�ͷ��Ϣ
	if (!::fwrite((const void *)&bmf, sizeof(BITMAPFILEHEADER), 1, file))
	{
		::fclose(file);
		return FALSE;
	}

	// ������Ϣ�����ݣ�֮���Ը�����Ϣ�����ֱ��ʹ��m_pInfo������Ϊ
	// GetDIBits()�����п��ܸı���Ϣ����ɫ�������
	int infosize = CalculateDIBInfoSize((LPBITMAPINFOHEADER)m_pInfo);
	ASSERT(infosize == (int)::_msize((void*)m_pInfo));
	LPBITMAPINFO pInfo = (LPBITMAPINFO)CreateMemoryBlockFromAddress(
		(LPVOID)m_pInfo);
	if (!pInfo)
	{
		::fclose(file);
		return FALSE;
	}

	// д����Ϣ������
	if (!::fwrite((const void *)pInfo, infosize, 1, file))
	{
		::free((void*)pInfo);
		::fclose(file);
		return FALSE;
	}

	LPSTR pBits = (LPSTR)CreateMemoryBlockFromAddress((LPVOID)NULL, 
		CalculateDIBitsSize((LPBITMAPINFOHEADER)pInfo));
	if (!pBits)
	{
		::free((void*)pInfo);
		::fclose(file);
		return FALSE;
	}

	// ��ȡDIBλ����
	HWND hWnd = ::GetDesktopWindow();
	ASSERT(hWnd);
	HDC hDC = ::GetDC(hWnd);
	if (!::GetDIBits(hDC,(HBITMAP)m_hObject,0,pInfo->bmiHeader.biHeight,\
			(LPVOID)pBits,pInfo,DIB_RGB_COLORS))
	{
		::free((void*)pInfo);
		::free((void*)pBits);
		::fclose(file);
		::ReleaseDC(hWnd, hDC);
		return FALSE;
	}
	::ReleaseDC(hWnd, hDC);

	// ��Ϣ�������ã��ͷ�
	::free((void*)pInfo);

	DWORD dwA, dwB, dwC;

	LPSTR lp = pBits;

	// �Էֶη�ʽд��λ���ݣ�ÿ���γ���Ϊ32KB��
	dwA = CalculateDIBitsSize((LPBITMAPINFOHEADER)m_pInfo);	// �ܳ���
	dwB = dwA/32768;			// ������32768��
	dwC = dwA - (dwB*32768);	// ����

	for (;dwB!=0;dwB--)
	{
		if (!::fwrite((const void *)lp, 32768, 1, file))
		{
			::free((void*)pBits);
			::fclose(file);
			return FALSE;
		}
		lp = (LPSTR)((DWORD)lp+32768UL);
	}

	// д��ʣ���λ����
	if (!::fwrite((const void *)lp, dwC, 1, file))
	{
		::free((void*)pBits);
		::fclose(file);
		return FALSE;
	}

	// λ���������ã��ͷ�
	::free((void*)pBits);

	// �ر��ļ�
	::fclose(file);

	return TRUE;
}



/*************************************************************************
 *
 * SaveBaseSpecifyFormat()
 *
 * ����˵����
 * 
 * WORD nBitsPerPixel		- ָ���ĸ�ʽ����ÿ����ռ����λ��
 * 
 * ����ֵ��
 * 
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ��ָ������ɫ��ʽ���汾�����е�λͼ��ԭ�����ļ���
 * 
 * ע��
 * 
 * # ���ָ�����ļ��Ѿ����ڣ����������Ǹ��ļ�
 * # �����������ԭ��û��λͼ����������FALSE
 * # ����ļ�û�д򿪻���д�ļ�ʱ������������FALSE
 * # ���ָ����λ���ǷǷ�ֵ��1,4,8,16,24,32�ǺϷ�ֵ������������FALSE
 * # ���������޸�������Ϣ�������
 * 
 ************************************************************************/


BOOL CBmpProc::SaveBaseSpecifyFormat(WORD nBitsPerPixel)
{
	// �����������ԭ��û��λͼ����������FALSE
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);

	// Ӧ�����ļ���
	if (m_cFileName.IsEmpty())
		return FALSE;

	// ���λ���Ϸ���
	if (!IsValidDibFormat(nBitsPerPixel))
		return FALSE;

	LPBITMAPINFO pTempInfo;
	BOOL bRet;

	// ��ȡָ����ʽDIB����Ϣ��
	LPBITMAPINFO pInfo = GetSpecifyFormatInfo(nBitsPerPixel);
	if (!pInfo)
		return FALSE;

	// ����ԭʼ��Ϣ��
	pTempInfo = m_pInfo;

	m_pInfo = pInfo;			   // ��ԭ�ļ����洢
	bRet = SaveBaseSpecifyFileName((LPCTSTR)m_cFileName);
	m_pInfo = pTempInfo;

	::free((void*)pInfo);

	return bRet;
}



/*************************************************************************
 *
 * Create()
 *
 * ����˵����
 * 
 * int width				- ����λͼ�Ŀ��
 * int height				- ����λͼ�ĸ߶�
 * WORD nBitCount			- ����λͼ��ÿ��������ռ��λ��
 * const void *lpBits		- ��λͼ�ĳ�ʼ��λ���ݣ�������NULL��
 * 
 * ����ֵ��
 * 
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ����ָ�������ݴ���һ��λͼ�������뱾������
 * 
 * ע��
 * 
 * # ��ڲ���width��height������0�����ָ����0����������FALSE
 * # ��ڲ���nBitCount����Чֵ������1,4,8,16,24,32��������ֵ����Ϊ�Ƿ�
 * # �����������ԭ������ͼ������ú����ɹ�֮��ԭ����ͼ�񽫱�ɾ��
 * # �����ڲ���lpBits��NULL�����´�����λ������ʼ��
 * 
 ************************************************************************/


BOOL CBmpProc::Create(CSize size, 
					  WORD nBitCount, 
					  const void *lpBits)
{ 
	return Create(size.cx, size.cy, nBitCount, lpBits); 
}

BOOL CBmpProc::Create(int width, 
					  int height, 
					  WORD nBitCount, 
					  const void *lpBits)
{
	// ����Ⱥ͸߶�ֵ
	if ((width==0)||(height==0))
		return FALSE;

	// ���λ���Ϸ���
	if (!IsValidDibFormat(nBitCount))
		return FALSE;

	CBitmap		cBmp;
	HBITMAP		hBmp;
	int			iCurDevBitPix, iCurDevPlanes;

	// ��ȡ��ǰϵͳ��ʾ������ɫ��ʽ��λ����ƽ������
	HWND hWnd = ::GetDesktopWindow();
	ASSERT(hWnd);
	HDC hDC = ::GetDC(hWnd);
	iCurDevBitPix = GetDeviceCaps(hDC,BITSPIXEL);
	iCurDevPlanes = GetDeviceCaps(hDC,PLANES);
	::ReleaseDC(hWnd, hDC);

	// ����ָ���ߴ粢���ݵ�ǰ��ʾ����ɫ��ʽ��λͼ��DDB��
	if (!cBmp.CreateBitmap(width, height, iCurDevPlanes, iCurDevBitPix, lpBits))
		return FALSE;

	hBmp = (HBITMAP)cBmp.Detach();
	ASSERT(hBmp);
	if (!hBmp)
		return FALSE;

	// ������ʱλͼ��Ϣ�飨������ɫ��
	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)CreateMemoryBlockFromAddress(
		(LPVOID)NULL, sizeof(BITMAPINFOHEADER));
	if (!lpbi)
	{
		::DeleteObject(hBmp);
		return FALSE;
	}

	// ����DIB��Ϣ������
	lpbi->biSize = sizeof(BITMAPINFOHEADER);
	lpbi->biWidth = width;
	lpbi->biHeight = height;
	lpbi->biPlanes = 1;
	lpbi->biBitCount = nBitCount;
	if ((nBitCount==16)||(nBitCount==32))
		lpbi->biCompression = BI_BITFIELDS;
	else
		lpbi->biCompression = BI_RGB;

	// �����豸�ֱ���
	hDC = ::GetDC(hWnd);
	lpbi->biXPelsPerMeter = \
		(GetDeviceCaps(hDC,HORZRES)*1000)/GetDeviceCaps(hDC,HORZSIZE);
	lpbi->biYPelsPerMeter = \
		(GetDeviceCaps(hDC,VERTRES)*1000)/GetDeviceCaps(hDC,VERTSIZE);
	::ReleaseDC(hWnd, hDC);

	lpbi->biClrUsed = 0;
	lpbi->biClrImportant = 0;
	lpbi->biSizeImage = CalculateDIBitsSize(lpbi);

	// ������ɫ��ĳߴ�
	WORD wPalSize = PaletteSize(lpbi);

	// ��������ɫ�����Ϣ�飬������lpbi��Ϣ���е�����
	LPBITMAPINFO pInfo = (LPBITMAPINFO)CreateMemoryBlockFromAddress(
		(LPVOID)lpbi, sizeof(BITMAPINFOHEADER)+wPalSize);
	if (!pInfo)
	{
		::free((void*)lpbi);
		::DeleteObject(hBmp);
		return FALSE;
	}

	ASSERT(pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	// ��ʱ�ýṹ�Ѿ����ã��ͷ�
	::free((void*)lpbi);

	// �������DIBλ���ݵĻ�����
	LPSTR pBits = (LPSTR)CreateMemoryBlockFromAddress((LPVOID)NULL, 
		pInfo->bmiHeader.biSizeImage);
	if (!pBits)
	{
		::free((void*)pInfo);
		::DeleteObject(hBmp);
		return FALSE;
	}

	// ����Ĵ��벢����Ҫ��ȡλͼ��DIBλ���ݣ�����Ҫȡ��λͼ����ɫ��
	// ��ɫ�������ں�����GetDIBits()�����óɹ�����д��pInfo�ṹ��
	hDC = ::GetDC(hWnd);
	if (!::GetDIBits(hDC,hBmp,0,height,(LPVOID)pBits,pInfo,DIB_RGB_COLORS))
	{
		::free((void*)pBits);
		::free((void*)pInfo);
		::ReleaseDC(hWnd, hDC);
		::DeleteObject(hBmp);
		return FALSE;
	}
	::ReleaseDC(hWnd, hDC);

	// ��ʱ������ҪDIBλ���ݣ��ͷ�
	::free((void*)pBits);

	// ɾ��ԭ����ͼ�񣬲��������µ�λͼ
	if (!ClearAndSetData(IT_CREATE,0,(LPCTSTR)"",pInfo,hBmp))
	{
		::free((void*)pInfo);
		::DeleteObject(hBmp);
		return FALSE;
	}

	return TRUE;
}



/*************************************************************************
 *
 * CreateCompatible()
 *
 * ����˵����
 * 
 * int width				- ����λͼ�Ŀ��
 * int height				- ����λͼ�ĸ߶�
 * CDC *pDC, 				- �豸������������NULL��
 * const void *lpBits		- ��λͼ�ĳ�ʼ��λ���ݣ�������NULL��
 * 
 * ����ֵ��
 * 
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ����ָ�������ݴ���һ����ɫ��ʽ������ָ���������λͼ�������뱾������
 * 
 * ע��
 * 
 * # ��ڲ���width��height������0�����ָ����0����������FALSE
 * # ��ڲ���pDC������NULL����ʱ����������ǰ��ʾ������ɫ��ʽ����λͼ�����
 *   pDC����NULL����������ָ��һ����Ч���豸������
 * # �����������ԭ������ͼ������ú����ɹ�֮��ԭ����ͼ�񽫱�ɾ��
 * # �����ڲ���lpBits��NULL�����´�����λ������ʼ��
 * 
 ************************************************************************/


BOOL CBmpProc::CreateCompatible(int width, 
								int height, 
								CDC *pDC, 
								const void *lpBits)
{
	// ����Ⱥ͸߶�ֵ
	if ((width==0)||(height==0))
		return FALSE;

	int	iCurDevBitPix, iCurDevPlanes;

	// ��ȡ��ǰϵͳ��ʾ������ɫ��ʽ��λ����ƽ������
	HWND hWnd = ::GetDesktopWindow();
	ASSERT(hWnd);
	if (pDC == NULL)
	{
		// ���û��ָ���豸��������ʹ�õ�ǰ��ʾ������ɫ��ʽ
		HDC hDC = ::GetDC(hWnd);
		iCurDevBitPix = GetDeviceCaps(hDC,BITSPIXEL);
		iCurDevPlanes = GetDeviceCaps(hDC,PLANES);
		::ReleaseDC(hWnd, hDC);
	}
	else
	{
		iCurDevBitPix = GetDeviceCaps(pDC->GetSafeHdc(),BITSPIXEL);
		iCurDevPlanes = GetDeviceCaps(pDC->GetSafeHdc(),PLANES);
	}

	// ��16ɫ��ϵͳ�У�ÿ���ص�λ����1����ƽ����4������ߵ������״���
	if ((iCurDevPlanes!=1)&&(iCurDevBitPix==1))
	{
		iCurDevBitPix = iCurDevPlanes;
		iCurDevPlanes = 1;
	}

	// ����λͼ��DDB��
	return Create(width, height, iCurDevBitPix, lpBits);
}



/*************************************************************************
 *
 * CopyFromObject()
 *
 * ����˵����
 * 
 * CBmpProc& souBmp			- ԴλͼĿ��
 * CRect *pDesRect			- Ŀ�����
 * CRect *pSouRect			- Դ����
 * 
 * ����ֵ��
 * 
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ����ԴλͼĿ����ָ�����򣨾��Σ���ͼ�񵽱�������ͼ���ָ������
 * 
 * �����ڲ���pDesRect��ΪNULL,��Ŀ����ε���Դͼ��ĳߴ�
 * �����ڲ���pSouRect��ΪNULL,��ԴĿ����ε���Դͼ��ĳߴ�
 * �����ڲ���pDesRect��pSouRect��ͬ���ú������ܻ���������ѹ������
 * 
 * ע��# ��������ԭ�������Ѵ���λͼ���ҵ���֮���λͼ���ᱻɾ��
 *     # ���Դλͼ���ǿյģ��򷵻�FALSE
 *	   # �����������û��λͼ����������FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::CopyFromObject(CBmpProc& souBmp, CRect *pDesRect, CRect *pSouRect)
{
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	if (!souBmp.IsValid())
		return FALSE;

	ASSERT(souBmp.m_pInfo);
	ASSERT(souBmp.m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));


	// ���δָ���������ݣ���ʹ��Դͼ�ľ�������
	CRect dr;
	CRect sr;

	if (pDesRect == NULL)
		dr = souBmp.Rect();
	else
		dr = *pDesRect;

	if (pSouRect == NULL)
		sr = souBmp.Rect();
	else
		sr = *pSouRect;


	// ����Դͼ
	CDC			compDC, compDC2;
	CBitmap		*pOldBmp, *pOldBmp2;

	// �����뵱ǰ��ʾ�豸���ݵ��ڴ��豸������
	compDC.CreateCompatibleDC(NULL);
	compDC2.CreateCompatibleDC(NULL);

	pOldBmp	= compDC.SelectObject((CBitmap*)this);
	pOldBmp2= compDC2.SelectObject((CBitmap*)&souBmp);

	// ����Ŀ��DC������ģʽΪCOLORONCOLOR��Ҳ���ǲ���ʾ�������ͼ��
	compDC.SetStretchBltMode(COLORONCOLOR);

	// ����ָ���ߴ��Դλͼ��Ŀ��λͼ
	compDC.StretchBlt(dr.left, dr.top, dr.Width(), dr.Height(), 
			&compDC2, sr.left, sr.top, sr.Width(), sr.Height(), SRCCOPY);

	compDC2.SelectObject(pOldBmp2);
	compDC.SelectObject(pOldBmp);

	return TRUE;
}



/*************************************************************************
 *
 * CopyFromHbmp()
 *
 * ����˵����
 * 
 * HBITMAP souBmp			- Դλͼ���
 * CRect *pDesRect			- Ŀ�����
 * CRect *pSouRect			- Դ����
 * 
 * ����ֵ��
 * 
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ����Դλͼ��ָ�����򣨾��Σ���ͼ�񵽱�������ͼ���ָ������
 * 
 * �����ڲ���pDesRect��ΪNULL,��Ŀ����ε���Դͼ��ĳߴ�
 * �����ڲ���pSouRect��ΪNULL,��ԴĿ����ε���Դͼ��ĳߴ�
 * �����ڲ���pDesRect��pSouRect��ͬ���ú������ܻ���������ѹ������
 * 
 * ע��# ��������ԭ�������Ѵ���λͼ���ҵ���֮���λͼ���ᱻɾ��
 *     # ���Դλͼ���ǿյģ��򷵻�FALSE
 *	   # �����������û��λͼ����������FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::CopyFromHbmp(HBITMAP souBmp, 
							CRect *pDesRect, 
							CRect *pSouRect)
{
	// �����ڲ���
	if (!souBmp)
		return FALSE;

	// ��鱾�����Ƿ���ͼ��
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	CBmpProc tmp;

	// �����������ʱ����
	if (!tmp.Attach(souBmp))
		return FALSE;

	BOOL ret = CopyFromObject(tmp, pDesRect, pSouRect);

	tmp.Detach();

	return ret;
}



/*************************************************************************
 *
 * CopyTran()
 *
 * ����˵����
 * 
 * CBmpProc& souBmp			- ԴλͼĿ��
 * COLORREF crColour		- Ҫ�˵���������ɫ��ȱʡֵ�ǰ�ɫ��
 * int x					- Ŀ��������Ͻ�X���꣨ȱʡֵΪ0��
 * int y					- Ŀ��������Ͻ�Y���꣨ȱʡֵΪ0��
 * CRect *pSouRect			- Դ���Σ�ȱʡֵΪNULL��
 * int mode					- �����0�򽫲�����ָ������ɫ�������1����ָ��
 *						      ����ɫ��ȱʡֵΪ0��
 * 
 * ����ֵ��
 * 
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ͸���ĸ���ԴλͼĿ����ָ�����ε�ͼ�񵽱�������ͼ���ָ������
 * 
 * �����ڲ���pSouRect��ΪNULL,��ԴĿ����ε���Դͼ��ĳߴ硣������
 * �����������������ѹ��������
 * �����ڲ���pSouRect����ΪNULL���ú�������������ѹ������
 * 
 * ע��# ��������ԭ�������Ѵ���λͼ���ҵ���֮���λͼ���ᱻɾ��
 *     # ���Դλͼ���ǿյģ��򷵻�FALSE
 *	   # ����������в�����λͼ������������FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::CopyTran(CBmpProc& souBmp, 
						COLORREF crColour, 
						int x, int y, 
						CRect *pSouRect, 
						int mode)
{
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	if (!souBmp.IsValid())
		return FALSE;

	ASSERT(souBmp.m_pInfo);
	ASSERT(souBmp.m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	CRect dr(souBmp.Rect());
	dr.OffsetRect(x, y);

	return CopyTran(souBmp, crColour, &dr, pSouRect, mode);
}



/*************************************************************************
 *
 * CopyTran()
 *
 * ����˵����
 * 
 * CBmpProc& souBmp			- ԴλͼĿ��
 * COLORREF crColour		- Ҫ�˵���������ɫ��ȱʡֵ�ǰ�ɫ��
 * CRect *pDesRect			- Ŀ����Σ�ȱʡֵΪNULL��
 * CRect *pSouRect			- Դ���Σ�ȱʡֵΪNULL��
 * int mode					- �����0�򽫲�����ָ������ɫ�������1����ָ��
 *						      ����ɫ��ȱʡֵΪ0��
 * 
 * ����ֵ��
 * 
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ͸���ĸ���ԴλͼĿ����ָ�����ε�ͼ�񵽱�������ͼ���ָ������
 * 
 * �����ڲ���pDesRect��ΪNULL,��Ŀ����ε���Դͼ��ĳߴ硣
 * �����ڲ���pSouRect��ΪNULL,��ԴĿ����ε���Դͼ��ĳߴ硣
 * ���pDesRect��pSouRect��ͬ����ú�����������������ѹ��������
 * 
 * ע��# ��������ԭ�������Ѵ���λͼ���ҵ���֮���λͼ���ᱻɾ��
 *     # ���Դλͼ���ǿյģ��򷵻�FALSE
 *	   # ����������в�����λͼ������������FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::CopyTran(CBmpProc& souBmp, 
						COLORREF crColour, 
						CRect *pDesRect, 
						CRect *pSouRect, 
						int mode)
{
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	if (!souBmp.IsValid())
		return FALSE;

	ASSERT(souBmp.m_pInfo);
	ASSERT(souBmp.m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));


	// ���δָ���������ݣ���ʹ��Դͼ�ľ�������
	CRect dr;
	CRect sr;

	if (pDesRect == NULL)
		dr = souBmp.Rect();
	else
		dr = *pDesRect;

	if (pSouRect == NULL)
		sr = souBmp.Rect();
	else
		sr = *pSouRect;

	// ����Դͼ
	CDC			compDC;
	CBitmap		*pOldBmp;

	// �����뵱ǰ��ʾ�豸���ݵ��ڴ��豸������
	compDC.CreateCompatibleDC(NULL);
	pOldBmp	= compDC.SelectObject((CBitmap*)this);

	// ����ָ���ߴ��Դλͼ��Ŀ��λͼ
	souBmp.DrawTransparentInRect(compDC, crColour, pDesRect, pSouRect, mode);

	compDC.SelectObject(pOldBmp);

	return TRUE;
}



/*************************************************************************
 *
 * CopyTranCenter()
 *
 * ����˵����
 * 
 * CBmpProc& souBmp			- ԴλͼĿ��
 * CRect* crArea 			- ���еķ�Χ��ȱʡֵΪNULL��
 * COLORREF crColour		- Ҫ�˵���������ɫ��ȱʡֵ�ǰ�ɫ��
 * int mode					- �����0�򽫲�����ָ������ɫ�������1����ָ��
 *						      ����ɫ��ȱʡֵΪ0��
 * 
 * ����ֵ��
 * 
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ��Դλͼ͸���ĸ��Ƶ�������ָ�����ε�����
 * 
 * �����ڲ���crArea��ΪNULL,����о��ε��ڱ�������ͼ��ĳߴ硣
 * �ú���������������ѹ��������
 * 
 * ע��# ��������ԭ�������Ѵ���λͼ���ҵ���֮���λͼ���ᱻɾ��
 *     # ���Դλͼ���ǿյģ��򷵻�FALSE
 *	   # ����������в�����λͼ������������FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::CopyTranCenter(CBmpProc& souBmp, 
							  CRect* crArea, 
							  COLORREF crColour, 
							  int mode)
{
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	if (!souBmp.IsValid())
		return FALSE;

	ASSERT(souBmp.m_pInfo);
	ASSERT(souBmp.m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	CRect rect(Rect());

	if (crArea)
		rect = *crArea;

	// ��������淶��
	rect.NormalizeRect();

	if ((rect.Width()==0)||(rect.Height()==0))
		return FALSE;

	// ����Դͼ
	CDC			compDC;
	CBitmap		*pOldBmp;

	// �����뵱ǰ��ʾ�豸���ݵ��ڴ��豸������
	compDC.CreateCompatibleDC(NULL);
	pOldBmp	= compDC.SelectObject((CBitmap*)this);

	// ����ָ���ߴ��Դλͼ��Ŀ��λͼ
	souBmp.DrawTranCenter(compDC, &rect, crColour, mode);

	compDC.SelectObject(pOldBmp);

	return TRUE;
}



/*************************************************************************
 *
 * CopyTranTile()
 *
 * ����˵����
 * 
 * CBmpProc& souBmp			- ԴλͼĿ��
 * CRect* crArea 			- ƽ�̵ķ�Χ��ȱʡֵΪNULL��
 * COLORREF crColour		- Ҫ�˵���������ɫ��ȱʡֵ�ǰ�ɫ��
 * int mode					- �����0�򽫲�����ָ������ɫ�������1����ָ��
 *						      ����ɫ��ȱʡֵΪ0��
 * 
 * ����ֵ��
 * 
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ��Դλͼ͸����ƽ�̵������ָ��������
 * 
 * �����ڲ���crArea��ΪNULL,��ƽ�̾��ε��ڱ�������ͼ��ĳߴ硣
 * �ú������������ѹ��Դλͼͼ��
 * 
 * ע��# ��������ԭ�������Ѵ���λͼ���ҵ���֮���λͼ���ᱻɾ��
 *     # ���Դλͼ���ǿյģ��򷵻�FALSE
 *	   # ����������в�����λͼ������������FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::CopyTranTile(CBmpProc& souBmp, 
							CRect* crArea, 
							COLORREF crColour, 
							int mode)
{
	// ���л������
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	if (!souBmp.IsValid())
		return FALSE;

	ASSERT(souBmp.m_pInfo);
	ASSERT(souBmp.m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	// ȱʡ��������跶Χ���Ǳ���λͼ�ĳߴ�
	CRect rect(Rect());

	if (crArea)
		rect = *crArea;	// ����ָ���ߴ�

	// ��������淶��
	rect.NormalizeRect();

	if ((rect.Width()==0)||(rect.Height()==0))
		return FALSE;

	// ����Դͼ
	CDC			compDC;
	CBitmap		*pOldBmp;

	// �����뵱ǰ��ʾ�豸���ݵ��ڴ��豸������
	compDC.CreateCompatibleDC(NULL);
	pOldBmp	= compDC.SelectObject((CBitmap*)this);

	// ����ָ���ߴ��Դλͼ��Ŀ��λͼ
	souBmp.DrawTranTile(compDC, &rect, crColour, mode);

	compDC.SelectObject(pOldBmp);
	
	return TRUE;
}



/*************************************************************************
 *
 * LoadFromClipboard()
 *
 * ����˵������
 * 
 * 
 * ����ֵ��
 * 
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ��ȡ�������е�DIBλͼ������
 * 
 * ��������ɹ���������ԭ����λͼ������еĻ�������ɾ��
 * 
 ************************************************************************/


BOOL CBmpProc::LoadFromClipboard()
{
	// �����������Ч��
	if (!PasteAvailable())
		return FALSE;

	// �򿪼�����
	if (::OpenClipboard(NULL))
	{
		// ��ȡ�������е�DIB����
		HANDLE h = ::GetClipboardData(CF_DIB);

		if (!h)
		{
			CloseClipboard();
			return FALSE;
		}

		LPSTR lpDIB = (LPSTR)CreateMemoryBlockFromHandle(h);
		if (!lpDIB)
		{	// ����ļ��������ݾ������Ҫ�ͷţ�ϵͳ���Զ��ͷ�
			CloseClipboard();
			return FALSE;
		}

		CloseClipboard();

		// ���DIB��Ϣ�����Ч��
		if (*((DWORD*)lpDIB) != sizeof(BITMAPINFOHEADER))
		{
			::free((void*)lpDIB);
			return FALSE;
		}

		// ������Ϣ��
		ASSERT(CalculateDIBInfoSize((LPBITMAPINFOHEADER)lpDIB) >= sizeof(BITMAPINFOHEADER));
		LPBITMAPINFO pInfo = (LPBITMAPINFO)CreateMemoryBlockFromAddress(
			(LPVOID)lpDIB, CalculateDIBInfoSize((LPBITMAPINFOHEADER)lpDIB));
		if (!pInfo)
		{
			::free((void*)lpDIB);
			return FALSE;
		}

		// ����λ�����׵�ַ
		LPSTR pBits = (LPSTR)FindDIBBits(lpDIB);
		ASSERT(pBits);

		// ����DDBλͼ
		HBITMAP hBmp;
		HWND hWnd = ::GetDesktopWindow();
		ASSERT(hWnd);
		HDC hDC = ::GetDC(hWnd);

		hBmp = ::CreateDIBitmap(hDC,		// ����DDBλͼ������DIBλ����
			(BITMAPINFOHEADER *)pInfo,		// ת��ΪDDBλ���ݣ�
			CBM_INIT, pBits, 
			(BITMAPINFO *)pInfo, 
			DIB_RGB_COLORS);

		::ReleaseDC(hWnd, hDC);

		// �������߼�λͼ�󣬴Ӽ������ж�ȡ�����������ã��ͷ�
		::free((void*)lpDIB);

		if (hBmp)	// �ɹ�
		{
			// ɾ��ԭ����ͼ�񣬲��������µ�λͼ
			if (!ClearAndSetData(IT_CLIP,0,(LPCTSTR)"",pInfo,hBmp))
			{
				::free((void*)pInfo);
				::DeleteObject(hBmp);
				return FALSE;
			}
			return TRUE;
		}
		else	// ʧ��
		{
			::free((void*)pInfo);
			return FALSE;
		}
	}

  return FALSE;
}



/*************************************************************************
 *
 * LoadFromFile()
 *
 * ����˵����
 * 
 * LPCTSTR lpszPathName		- �������λͼ�ļ����ƣ�·����
 * 
 * ����ֵ��
 * 
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ��ȡָ��λͼ�ļ�������
 * 
 * ��������ɹ���������ԭ����λͼ������еĻ�������ɾ��
 * �����ڲ���ָ�����ļ������ڻ���һ��λͼ�ļ�����������FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::LoadFromFile(LPCTSTR lpszPathName)
{
	CFile file;
	LPBITMAPINFO lpbi;
	LPSTR pBits;
	HBITMAP hBmp;

	if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite))
		return FALSE;

	// �ж��ļ��Ƿ�����Ч��BMP�ļ�
	if (IsValidBmpFile(file) == FALSE)
	{
		file.Close();
		return FALSE;
	}

	// ��ȡBMP�ļ���Ϣ
	if ((lpbi=GetBmpFileInfo(file)) == NULL)
	{
		file.Close();
		return FALSE;
	}

	// ���಻֧��ѹ��λͼ�����������ѹ��λͼ�򷵻�FALSE
	if ((lpbi->bmiHeader.biCompression==BI_RLE4)||(lpbi->bmiHeader.biCompression==BI_RLE8))
	{
		::free((void*)lpbi);
		file.Close();
		return FALSE;
	}

	// ��ȡBMPλ����
	if ((pBits=GetBmpFileBits(file)) == NULL)
	{
		::free((void*)lpbi);
		file.Close();
		return FALSE;
	}


	HWND hWnd = ::GetDesktopWindow();
	ASSERT(hWnd);
	HDC hDC = ::GetDC(hWnd);

	hBmp = ::CreateDIBitmap(hDC,		// ����DDBλͼ������DIBλ����
		(BITMAPINFOHEADER *)lpbi,		// ת��ΪDDBλ���ݣ�
		CBM_INIT, pBits, 
		(BITMAPINFO *)lpbi, 
		DIB_RGB_COLORS);

	::ReleaseDC(hWnd, hDC);

	// �������߼�λͼ�󣬴��ļ��ж�ȡ��λ���������ã��ͷ�
	::free((void*)pBits);

	file.Close();

	if (hBmp)	// �ɹ�
	{
		// ɾ��ԭ����ͼ�񣬲��������µ�λͼ
		if (!ClearAndSetData(IT_DISKFILE,0,lpszPathName,lpbi,hBmp))
		{
			::free((void*)lpbi);
			::DeleteObject(hBmp);
			return FALSE;
		}
		return TRUE;
	}
	else	// ʧ��
	{
		::free((void*)lpbi);
		return FALSE;
	}
}



/*************************************************************************
 *
 * LoadFromResourceID()
 *
 * ����˵����
 * 
 * UINT uID					- �������λͼ��Դ���ƣ���ʶֵ��
 * HINSTANCE hInstance		- λͼ��Դ����ʵ���ľ�������������ΪNULL����
 *							  ��������ֵ��Ϊ��ǰӦ�ó�����̣�
 * 
 * ����ֵ��
 * 
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ��ȡָ��λͼ��Դ������
 * 
 * ��������ɹ���������ԭ����λͼ������еĻ�������ɾ��
 * 
 ************************************************************************/


BOOL CBmpProc::LoadFromResourceID(UINT uID, HINSTANCE hInst) 
{ 
	return LoadFromResourceName((LPCTSTR)MAKEINTRESOURCE(uID), hInst); 
}



/*************************************************************************
 *
 * LoadFromResourceName()
 *
 * ����˵����
 * 
 * LPCTSTR lpResourceName	- �������λͼ��Դ���ƣ��ַ�����
 * HINSTANCE hInstance		- λͼ��Դ����ʵ���ľ�������������ΪNULL����
 *							  ��������ֵ��Ϊ��ǰӦ�ó�����̣�
 * 
 * ����ֵ��
 * 
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ��ȡָ��λͼ��Դ������
 * 
 * ��������ɹ���������ԭ����λͼ������еĻ�������ɾ��
 * 
 ************************************************************************/


BOOL CBmpProc::LoadFromResourceName(LPCTSTR lpResourceName, 
									HINSTANCE hInstance)
{
	HINSTANCE	hInst = hInstance;
	HBITMAP hBmp = NULL;

	//�����ڲ���hInstance��NULL���������õ�ǰӦ�ó�����̵�ʵ�����
	if (hInst == NULL)
		hInst = ::AfxGetResourceHandle();

	ASSERT(hInst);

	HRSRC hSrc = ::FindResource(hInst, lpResourceName, RT_BITMAP);
	BOOL bSuccess = FALSE;

	if (!hSrc)
		return bSuccess;

	// ��LoadResource()�����������ԴĿ�겻��Ҫ��ʾ��ɾ������Windows
	// ϵͳ���Զ�ɾ����LockResource()��������ԴĿ��Ҳ����Ҫ��ʾ�Ľ���
	HGLOBAL hResData = LoadResource(hInst, hSrc);
	if (hResData)
	{
		LPVOID lpResData = LockResource(hResData);
		if (lpResData)
		{
			// �������Ч��DIB��Դ������Դ����λӦ����BITMAPINFOHEADER
			// �ṹ
			ASSERT(*((DWORD*)lpResData) == sizeof(BITMAPINFOHEADER));

			LPSTR pBits = FindDIBBits((LPSTR)lpResData);
			ASSERT(pBits > (LPSTR)lpResData);

			HWND hWnd = ::GetDesktopWindow();
			ASSERT(hWnd);
			HDC hDC = ::GetDC(hWnd);

			hBmp = CreateDIBitmap(hDC,			// ����DDBλͼ������DIBλ����
				(BITMAPINFOHEADER *)lpResData,	// ת��ΪDDBλ���ݣ�
				CBM_INIT, pBits, 
				(BITMAPINFO *)lpResData, 
				DIB_RGB_COLORS);

			::ReleaseDC(hWnd, hDC);

			if (hBmp)	// �ɹ�
			{
				DWORD dwInfoSize = (DWORD)((LPSTR)pBits-(LPSTR)lpResData);
				ASSERT(dwInfoSize > (DWORD)(sizeof(BITMAPFILEHEADER)));

				LPBITMAPINFO lpbi = (LPBITMAPINFO)CreateMemoryBlockFromAddress(
					(LPVOID)NULL, dwInfoSize);
				if (lpbi)
				{
					// ����λͼ��Ϣ�飬��ΪlpResData��ָ���ڴ治������ʱ̬��
					// ����������ڴ�飬����δ��CreateMemoryBlockFromAddress()
					// �ĸ��ƹ���
					::memcpy((void*)lpbi, (const void*)lpResData, dwInfoSize);
				}
				else
				{
					::DeleteObject(hBmp);
					return FALSE;
				}

				if ((((DWORD)lpResourceName)>>16) == 0)
				{// ���ָ����Դʱʹ�õ��Ǳ�ʶ����ָ��ĸ�16λ��0
					// ɾ��ԭ����ͼ�񣬲��������µ�λͼ
					if (!ClearAndSetData(IT_RESOURCE,(DWORD)lpResourceName,(LPCTSTR)"",lpbi,hBmp))
					{
						::free((void*)lpbi);
						::DeleteObject(hBmp);
						return FALSE;
					}
				}
				else
				{
					// ɾ��ԭ����ͼ�񣬲��������µ�λͼ
					if (!ClearAndSetData(IT_RESOURCE,0, lpResourceName,lpbi,hBmp))
					{
						::free((void*)lpbi);
						::DeleteObject(hBmp);
						return FALSE;
					}
				}
				bSuccess = TRUE;
			}
		}
	}

	return bSuccess;
}



/*************************************************************************
 *
 * LoadFromObject()
 *
 * ����˵����
 * 
 * CBmpProc& souBmp			- ԴλͼĿ��
 * CRect *pSouRect			- ���ƾ���
 * 
 * ����ֵ��
 * 
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ��ȡԴλͼĿ����ָ�����򣨾��Σ���ͼ������
 * 
 * �����ڲ���pSouRect��ΪNULL,����ԴĿ�������λͼ
 * ��ڲ���pSouRect������һ�������ľ����������ݣ������Ͻ�����С�����½�
 * ���꣩
 * 
 * ��������ɹ���������ԭ����λͼ������еĻ�������ɾ��
 * 
 ************************************************************************/


BOOL CBmpProc::LoadFromObject(CBmpProc& souBmp, CRect *pSouRect)
{
	CRect souRect;

	// һ������£�Դͼ�������
	ASSERT(souBmp.IsValid());	
	if (souBmp.IsValid() == FALSE)
		return FALSE;

	// ȷ��Ҫ����Դλͼ����Щ����
	if (pSouRect == NULL)
		souRect = souBmp.Rect();	// ȱʡΪ����
	else
	{
		CRect sou = souBmp.Rect();
		souRect = *pSouRect;
		// ���ָ���ľ����г���Դͼ�ĵط���������õ�
		if (!sou.PtInRect((POINT)CPoint(souRect.left, souRect.top)))
			souRect.OffsetRect(-souRect.left, -souRect.top);

		if (!sou.PtInRect((POINT)CPoint(souRect.right, souRect.bottom)))
		{
			if (souRect.right > sou.right)
				souRect.right = sou.right;
			if (souRect.bottom > sou.bottom)
				souRect.bottom = sou.bottom;
		}
	}

	ASSERT(souBmp.m_pInfo);
	LPBITMAPINFO lpSouInfo = souBmp.m_pInfo;

	// ����Դͼ�����Ϣ��
	LPBITMAPINFO pInfo = (LPBITMAPINFO)CreateMemoryBlockFromAddress(
		(LPVOID)lpSouInfo);
	if (!pInfo)
		return FALSE;

	// ȷ��Դͼ��Ϣ�����Ч��
	ASSERT(pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	// �޸�����Ϣ���е�����
	pInfo->bmiHeader.biWidth = souRect.Width();
	pInfo->bmiHeader.biHeight = souRect.Height();
	pInfo->bmiHeader.biSizeImage = CalculateDIBitsSize((LPBITMAPINFOHEADER)pInfo);

	CBitmap		tempBmp;
	HBITMAP		hBmp;

	HWND hWnd = ::GetDesktopWindow();
	ASSERT(hWnd);
	HDC hDC = ::GetDC(hWnd);
	CDC dc;
	dc.Attach(hDC);

	if (tempBmp.CreateCompatibleBitmap(&dc, souRect.Width(), souRect.Height()))
	{
		CDC			compDC, compDC2;
		CBitmap		*pOldBmp, *pOldBmp2;

		// �����뵱ǰ��ʾ�豸���ݵ��ڴ��豸������
		compDC.CreateCompatibleDC(NULL);
		compDC2.CreateCompatibleDC(NULL);

		pOldBmp	= compDC.SelectObject((CBitmap*)&tempBmp);
		pOldBmp2= compDC2.SelectObject((CBitmap*)&souBmp);

		// ����ָ���ߴ��Դλͼ��Ŀ��λͼ
		compDC.BitBlt(0, 0, souRect.Width(), souRect.Height(), 
					&compDC2, souRect.left, souRect.top, SRCCOPY);

		compDC2.SelectObject(pOldBmp2);
		compDC.SelectObject(pOldBmp);

		::ReleaseDC(hWnd, dc.Detach());

		hBmp = (HBITMAP)tempBmp.Detach();
		// ����������ʧ�ܣ�����FALSE
		ASSERT(hBmp);
		if (!hBmp)
		{
			::free((void*)pInfo);
			return FALSE;
		}

		// ɾ��ԭ����ͼ�񣬲��������µ�λͼ
		if (!ClearAndSetData(IT_MEMORY,0,(LPCTSTR)"",pInfo,hBmp))
		{
			::free((void*)pInfo);
			::DeleteObject(hBmp);
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		::ReleaseDC(hWnd, dc.Detach());
		::free((void*)pInfo);

		return FALSE;
	}
}



/*************************************************************************
 *
 * LoadFromHbmp()
 *
 * ����˵����
 * 
 * HBITMAP sou				- Դλͼ���������ΪNULL��
 * CRect *pSouRect			- ���ƾ���
 * 
 * ����ֵ��
 * 
 * BOOL						- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ��ȡԴλͼ�����ָ�����ε�ͼ������
 * 
 * �����ڲ���pSouRect��ΪNULL,����ԴĿ�������λͼ
 * ��ڲ���pSouRect������һ�������ľ����������ݣ������Ͻ�����С�����½�
 * ���꣩
 * ��ڲ���ָ����λͼ������۳ɹ���񶼲�����Ӱ��
 * 
 * ��������ɹ���������ԭ����λͼ������еĻ�������ɾ��
 * 
 ************************************************************************/


BOOL CBmpProc::LoadFromHbmp(HBITMAP sou, CRect *pSouRect)
{
	// �����ڲ���
	if (!sou)
		return FALSE;

	CBmpProc tmp;

	// �����������ʱ����
	if (!tmp.Attach(sou))
		return FALSE;

	BOOL ret = LoadFromObject(tmp, pSouRect);

	tmp.Detach();

	return ret;
}



/*************************************************************************
 *
 * CalculateDIBFileSize()
 *
 * ����˵����
 *
 * LPBITMAPINFOHEADER lpbi	- ָ���ڴ�DIB��BITMAPINFOHEADER�ṹ
 *
 * ����ֵ��
 *
 * DWORD					- ����DIB�ļ��ĳߴ�
 *
 * ������
 *
 * �������ָ����Ϣ��ʽ��DIBλͼ�ĳߴ磨���ֽ�Ϊ��λ��
 *
 ************************************************************************/


DWORD CBmpProc::CalculateDIBFileSize(LPBITMAPINFOHEADER lpbi) const
{
	ASSERT(lpbi);
	ASSERT(lpbi->biSize == sizeof(BITMAPINFOHEADER));

	DWORD filesize;
	DWORD dwScanWidth = DIBSCANLINE_WIDTHBYTES(DIBWidth(lpbi)*GetBitsPerPixel(lpbi));
	DWORD dwPaletteSize = (DWORD)PaletteSize(lpbi);

	// �ļ�ͷ�ߴ磫��Ϣͷ�ߴ磫��ɫ��ߴ磫λ���ݳߴ�
	filesize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 
		dwPaletteSize + dwScanWidth*DIBHeight(lpbi);

	return filesize;
}



/*************************************************************************
 *
 * CalculateDIBSize()
 *
 * ����˵����
 *
 * LPBITMAPINFOHEADER lpbi	- ָ���ڴ�DIB��BITMAPINFOHEADER�ṹ
 *
 * ����ֵ��
 *
 * DWORD					- ����DIB�ڲ���ʽ�ĳߴ�
 *
 * ������
 *
 * �������ָ����Ϣ��ʽ��DIB�ڲ���ʽ�ĳߴ磨���ֽ�Ϊ��λ��
 *
 * �ڲ���ʽ��ָ��ȥ�ļ�ͷ�ṹ��DIB�ļ��������ڴ��е�ʵ��
 *
 ************************************************************************/


DWORD CBmpProc::CalculateDIBSize(LPBITMAPINFOHEADER lpbi) const
{
	ASSERT(lpbi);
	ASSERT(lpbi->biSize == sizeof(BITMAPINFOHEADER));

	DWORD dwScanWidth = DIBSCANLINE_WIDTHBYTES(DIBWidth(lpbi)*GetBitsPerPixel(lpbi));
	DWORD dwPaletteSize = (DWORD)PaletteSize(lpbi);

	// ��Ϣͷ�ߴ磫��ɫ��ߴ磫λ���ݳߴ�
	return sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwScanWidth*DIBHeight(lpbi);
}



/*************************************************************************
 *
 * CalculateDIBitsOff()
 *
 * ����˵����
 *
 * LPBITMAPINFOHEADER lpbi	- ָ���ڴ�DIB��BITMAPINFOHEADER�ṹ
 *
 * ����ֵ��
 *
 * DWORD					- ������DIB�ļ���λ��ƫ��
 *
 * ������
 *
 * �������ָ����Ϣ��ʽ��DIBλͼ��λ��ƫ�ƣ����ֽ�Ϊ��λ��
 *
 ************************************************************************/


DWORD CBmpProc::CalculateDIBitsOff(LPBITMAPINFOHEADER lpbi) const
{
	DWORD bitsoff;

	ASSERT(lpbi);
	ASSERT(lpbi->biSize == sizeof(BITMAPINFOHEADER));

	DWORD dwPaletteSize = (DWORD)PaletteSize(lpbi);

	// �ļ�ͷ�ߴ磫��Ϣͷ�ߴ磫��ɫ��ߴ�
	bitsoff = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + \
		dwPaletteSize;

	return bitsoff;
}



/*************************************************************************
 *
 * CalculateDIBInfoSize()
 *
 * ����˵����
 *
 * LPBITMAPINFOHEADER lpbi	- ָ���ڴ�DIB��BITMAPINFOHEADER�ṹ
 *
 * ����ֵ��
 *
 * DWORD					- ������DIB�ļ�����Ϣ��ĳ���
 *
 * ������
 *
 * �������ָ����Ϣ��ʽ��DIBλͼ����Ϣ��ĳ��ȣ����ֽ�Ϊ��λ��
 *
 * ��Ϣ����ָBITMAPINFO�ṹ��ע�⣬���к�����ɫ��!
 *
 ************************************************************************/


DWORD CBmpProc::CalculateDIBInfoSize(LPBITMAPINFOHEADER lpbi) const
{
	ASSERT(lpbi);
	ASSERT(lpbi->biSize == sizeof(BITMAPINFOHEADER));
	// ��Ϣͷ�ߴ磫��ɫ��ߴ�
	return sizeof(BITMAPINFOHEADER)+(DWORD)PaletteSize(lpbi);
}



/*************************************************************************
 *
 * CalculateDIBitsSize()
 *
 * ����˵����
 *
 * LPBITMAPINFOHEADER lpbi	- ָ���ڴ�DIB��BITMAPINFOHEADER�ṹ
 *
 * ����ֵ��
 *
 * DWORD					- ������DIB�ļ���λ�ĳߴ�
 *
 * ������
 *
 * �������ָ����Ϣ��ʽ��DIBλͼ��λ�ģ����ֽ�Ϊ��λ��
 *
 ************************************************************************/


DWORD CBmpProc::CalculateDIBitsSize(LPBITMAPINFOHEADER lpbi) const
{
	ASSERT(lpbi);
	ASSERT(lpbi->biSize == sizeof(BITMAPINFOHEADER));

	DWORD s = DIBSCANLINE_WIDTHBYTES(DIBWidth(lpbi)*GetBitsPerPixel(lpbi));

	return s*DIBHeight(lpbi);
}



/*************************************************************************
 *
 * IsValidBmpFile()
 *
 * ����˵����
 *
 * CFile& file      - ���жϵ��ļ����ļ������ѱ��򿪣�
 *
 * ����ֵ��
 *
 * BOOL             - ����TRUE�����ļ�����Ч��BMP�ļ������򷵻�FALSE
 *
 * ������
 *
 * �ж�ָ�����ļ��Ƿ���һ����Ч��BMP�ļ�
 *
 * ע������֧��OS/2ϵͳ������BMP�ļ�������������ļ�����������FALSE
 *
 ************************************************************************/


BOOL CBmpProc::IsValidBmpFile(CFile& file)
{
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;
	DWORD dwSize, dwCurPos;
	UINT uRet, uRet2;

	/* ��ȡDIB�ļ��ĳ��ȣ����ֽ�Ϊ��λ��*/
	dwSize = file.GetLength();
	if (dwSize < (sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)))
		return FALSE;

	/* ��ȡDIB���ļ�ͷ�ṹ�������������Ч�� */
	dwCurPos = file.GetPosition();
	file.SeekToBegin();
	uRet  = file.Read((LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER));
	uRet2 = file.Read((LPSTR)&bmiHeader, sizeof(BITMAPINFOHEADER));
	file.Seek(dwCurPos, CFile::begin);

	if (uRet != sizeof(BITMAPFILEHEADER))
		return FALSE;
	if (bmfHeader.bfType != DIB_HEADER_MARKER) //((WORD) ('M' << 8) | 'B')
		return FALSE;

	if (uRet2 != sizeof(BITMAPINFOHEADER))
		return FALSE;
	// ����֧��OS/2��ʽ��λͼ
	if (bmiHeader.biSize != sizeof(BITMAPINFOHEADER))
		return FALSE;

	int size = PaletteSize(&bmiHeader);

	if (bmfHeader.bfOffBits != \
		(sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+size))
		return FALSE;

	return TRUE;	// TRUE�����ļ�����Ч��BMP�ļ�
}



/*************************************************************************
 *
 * GetBmpFileInfo()
 *
 * ����˵����
 *
 * CFile& file      - ��ȡ����Ϣ��λͼ�ļ����ļ������ѱ��򿪣�
 *
 * ����ֵ��
 *
 * LPBITMAPINFO		- ����λͼ��Ϣ���ַ�����ʧ�ܷ���NULL
 *
 * ������
 *
 * ��ȡλͼ�ļ���ͼ����Ϣ����������Ϣ�ṹ�ĵ�ַ
 *
 * ע��λͼ��Ϣ�����һ���ڴ档���ñ�����ǰӦ���λͼ�ļ�����Ч��
 *
 ************************************************************************/


LPBITMAPINFO CBmpProc::GetBmpFileInfo(CFile& file)
{
	LPBITMAPINFO pDIB;
	BITMAPFILEHEADER bmfHeader;
	DWORD dwCurPos;
	UINT uRet, uDibInfoSize;

	/* ��ȡDIB���ļ�ͷ�ṹ */
	dwCurPos = file.GetPosition();
	file.SeekToBegin();
	uRet  = file.Read((LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER));
	file.Seek(dwCurPos, CFile::begin);

	if (uRet != sizeof(BITMAPFILEHEADER))
		return NULL;

	uDibInfoSize = bmfHeader.bfOffBits-sizeof(BITMAPFILEHEADER);

	// �����ڴ棬���ڴ����Ϣͷ����ɫ������У�
	pDIB = (LPBITMAPINFO)CreateMemoryBlockFromAddress((LPVOID)NULL, 
		(DWORD)uDibInfoSize);
	if (!pDIB)
		return NULL;

	// ��ȡ����
	dwCurPos = file.GetPosition();
	file.Seek((LONG)sizeof(BITMAPFILEHEADER), CFile::begin);
	uRet = file.Read((void *)pDIB, uDibInfoSize);
	file.Seek(dwCurPos, CFile::begin);

	if (uRet != uDibInfoSize)
	{
		::free((void*)pDIB);
		return NULL;
	}

	return pDIB;
}



/*************************************************************************
 *
 * GetBmpFileBits()
 *
 * ����˵����
 *
 * CFile& file      - ��ȡ��λ���ݵ�λͼ�ļ����ļ������ѱ��򿪣�
 *
 * ����ֵ��
 *
 * LPSTR			- ����λͼλ��Ϣ���ڴ���ַ�����ʧ�ܷ���NULL
 *
 * ������
 *
 * ��ȡλͼ�ļ���λ���ݣ����������ĵ�ַ
 *
 * ע�����ñ�����ǰӦ�����ļ�����Ч��
 *
 ************************************************************************/


LPSTR CBmpProc::GetBmpFileBits(CFile& file)
{
	BITMAPFILEHEADER bmfHeader;
	LPSTR pBits;
	DWORD dwSize, dwRet, dwCurPos;
	UINT uRet;

	/* ��ȡDIB���ļ�ͷ�ṹ */
	dwCurPos = file.GetPosition();
	file.SeekToBegin();
	uRet  = file.Read((LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER));
	file.Seek(dwCurPos, CFile::begin);

	if (uRet != sizeof(BITMAPFILEHEADER))
		return NULL;

	// ��ȡ�ļ�����
	dwSize = file.GetLength();

	// �����ڴ棬���ڴ��λ����
	pBits = (LPSTR)CreateMemoryBlockFromAddress((LPVOID)NULL, 
		(DWORD)(dwSize-bmfHeader.bfOffBits));
	if (!pBits)
		return NULL;

	// ��ȡλ����
	dwCurPos = file.GetPosition();
	file.Seek(bmfHeader.bfOffBits, CFile::begin);
	dwRet = file.ReadHuge((void*)pBits, (dwSize-bmfHeader.bfOffBits));
	file.Seek(dwCurPos, CFile::begin);

	if (dwRet != (dwSize-bmfHeader.bfOffBits))
	{
		::free(pBits);
		return NULL;
	}

	return pBits;
}



/*************************************************************************
 *
 * GetSpecifyFormatInfo()
 *
 * ����˵����
 *
 * WORD newBitCount			- ��ָ���ĸ�ʽ����ÿ����ռ����λ������ָ��0��
 *
 * ����ֵ��
 *
 * LPBITMAPINFO				- ����ɹ������µ���Ϣ���ַ�����ʧ�ܷ���NULL
 *
 * ������
 *
 * �������е���Ϣ���ָ�����¸�ʽ����һ���µ���Ϣ��
 *
 * ע��λͼ��Ϣ�����һ���ڴ档
 *     ��������Ҫ����ڴ��ʱ��Ӧ�ͷţ���free()������
 *     �ú�������Ӱ�����е�����
 *     ���ԭ������û��λͼ����������NULL
 *     �����ڲ���ָ��0ֵ���������������Ѵ��ڵ���Ϣ���һ������
 *
 ************************************************************************/


LPBITMAPINFO CBmpProc::GetSpecifyFormatInfo(WORD newBitCount)
{
	// �����������ԭ��û��λͼ����������NULL
	if (!IsValid())
		return NULL;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	int infosize;
	HWND hWnd = ::GetDesktopWindow();
	ASSERT(hWnd);
	HDC hDC;
	BITMAPINFOHEADER bmi;
	LPBITMAPINFO pInfo;
	LPSTR pBits;

	// ���û��ָ���µĸ�ʽ�����µ�λ��ָ����0��������������Ϣ��ĸ���
	if (newBitCount == 0)
	{
		pInfo = (LPBITMAPINFO)CreateMemoryBlockFromAddress((LPVOID)m_pInfo);
		return pInfo;
	}

	// ���λ���Ϸ���
	if (!IsValidDibFormat(newBitCount))
		return NULL;

	::memcpy((void*)&bmi, (const void*)m_pInfo, sizeof(BITMAPINFOHEADER));
	ASSERT(bmi.biSize == sizeof(BITMAPINFOHEADER));

	bmi.biBitCount = newBitCount;

	if ((bmi.biBitCount==16)||(bmi.biBitCount==32))
		bmi.biCompression = BI_BITFIELDS;
	else
		bmi.biCompression = BI_RGB;

	bmi.biSizeImage = CalculateDIBitsSize(&bmi);

	infosize = CalculateDIBInfoSize(&bmi);
	pInfo = (LPBITMAPINFO)CreateMemoryBlockFromAddress((LPVOID)NULL, 
		(DWORD)infosize);
	if (!pInfo)
		return NULL;

	::memcpy((void*)pInfo, (const void*)&bmi, sizeof(BITMAPINFOHEADER));
	ASSERT(pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	pBits = (LPSTR)CreateMemoryBlockFromAddress((LPVOID)NULL, 
		CalculateDIBitsSize((LPBITMAPINFOHEADER)pInfo));
	if (!pBits)
	{
		::free((void*)pInfo);
		return NULL;
	}

	// ��ȡDIB��ɫ������
	hDC = ::GetDC(hWnd);
	if (!::GetDIBits(hDC,(HBITMAP)m_hObject,0,pInfo->bmiHeader.biHeight,\
			(LPVOID)pBits,pInfo,DIB_RGB_COLORS))
	{
		::free((void*)pInfo);
		::free((void*)pBits);
		::ReleaseDC(hWnd, hDC);
		return NULL;
	}
	::ReleaseDC(hWnd, hDC);

	// λ���������ã��ͷ�
	::free((void*)pBits);

	return pInfo;
}



/****************************** DIB Function ****************************/

/*
 DIB ����ע�ͣ�

  # ��֧��OS/2����ϵͳ������DIBλͼ
  # ��֧��ѹ��λͼ��RLEѹ�����룩
  # FindDIBBits()����ڲ���lpbi��ָ�������Ϣͷ����ɫ��λ���ݵ��ڴ��
  # ������������ڲ���lpbi��ָ����Ϣ�飬��BITMAPINFOHEADER�ṹ
  # ��������ڲ���lpbi�е����ݱ�������Ч��!

*/

/*************************************************************************
 *
 * FindDIBBits()
 *
 * ����˵����
 *
 * LPSTR lpbi       - ָ���ڴ�DIB���׵�ַ
 *
 * ����ֵ��
 *
 * LPSTR            - ָ��DIBλ���ݵĵ�ַ
 *
 * ������
 *
 * �����������DIB��λ���ݵ�ַ�����������ء�
 *
 ************************************************************************/


LPSTR CBmpProc::FindDIBBits(LPSTR lpbi) const
{
	ASSERT(lpbi);
	ASSERT(((LPBITMAPINFOHEADER)lpbi)->biSize == sizeof(BITMAPINFOHEADER));
	return (lpbi + *(LPDWORD)lpbi + PaletteSize((LPBITMAPINFOHEADER)lpbi));
}



/*************************************************************************
 *
 * DIBWidth()
 *
 * ����˵����
 *
 * LPBITMAPINFOHEADER lpbi	- ָ���ڴ�DIB��BITMAPINFOHEADER �ṹ
 *
 * ����ֵ��
 *
 * DWORD				- DIB�Ŀ��
 *
 * ������
 *
 * ��ȡDIB�Ŀ�ȡ������أ�
 *
 * ����BITMAPINFOHEADER�ṹ��biWidth��Ա��ȡ���ֵ��
 *
 ************************************************************************/


DWORD CBmpProc::DIBWidth(LPBITMAPINFOHEADER lpbi) const
{
	ASSERT(lpbi);
	ASSERT(lpbi->biSize == sizeof(BITMAPINFOHEADER));
	return 	lpbi->biWidth;
}



/*************************************************************************
 *
 * DIBHeight()
 *
 * ����˵����
 *
 * LPBITMAPINFOHEADER lpbi	- ָ���ڴ�DIB��BITMAPINFOHEADER�ṹ
 *
 * ����ֵ��
 *
 * DWORD					- DIB�ĸ߶�
 *
 * ������
 *
 * ��ȡDIB�ĸ߶ȡ������أ�
 *
 * ����BITMAPINFOHEADER�ṹ��biHeight��Ա��ȡ�߶�ֵ��
 *
 ************************************************************************/


DWORD CBmpProc::DIBHeight(LPBITMAPINFOHEADER lpbi) const
{
	ASSERT(lpbi);
	ASSERT(lpbi->biSize == sizeof(BITMAPINFOHEADER));
	return 	lpbi->biHeight;
}



/*************************************************************************
 *
 * PaletteSize()
 *
 * ����˵����
 *
 * LPBITMAPINFOHEADER lpbi	- ָ���ڴ�DIB��BITMAPINFOHEADER�ṹ
 *
 * ����ֵ��
 *
 * WORD						- DIB��ɫ��ĳߴ磨���ֽ�Ϊ��λ��
 *
 * ������
 *
 * ��ȡָ��DIB�ĵ�ɫ��ߴ磨���ֽ�Ϊ��λ��
 *
 * ��������Ȼ�ȡDIB����ɫ������Ȼ������ɫ����ֵ����RGBQUAD�Ľṹ�ߴ硣
 *
 ************************************************************************/


WORD CBmpProc::PaletteSize(LPBITMAPINFOHEADER lpbi) const
{
	WORD size, wBitCount;

	ASSERT(lpbi);
	ASSERT(lpbi->biSize == sizeof(BITMAPINFOHEADER));
	// ���ÿ��������ռ��λ��
	wBitCount = GetBitsPerPixel(lpbi);
				
	// 16λ��32λλͼ����ɫ����ռ������DWORD��ֵ����ʾ
	// �졢�̡�����λ�����е�����
	if ((wBitCount == 16)||(wBitCount == 32))
	{
		ASSERT(lpbi->biCompression == BI_BITFIELDS);
		size = sizeof(DWORD)*3;
	}
	else
	{
		// ��֧��ѹ��λͼ
		ASSERT(lpbi->biCompression == BI_RGB);
		size = (WORD)(DIBNumColors(lpbi) * sizeof(RGBQUAD));
	}

	return size;
}



/*************************************************************************
 *
 * DIBNumColors()
 *
 * ����˵����
 *
 * LPBITMAPINFOHEADER lpbi	- ָ���ڴ�DIB��BITMAPINFOHEADER�ṹ
 *
 * ����ֵ��
 *
 * WORD						- ��ɫ������ɫ�ĸ���
 *
 * ������
 *
 * ��ȡDIB��ɫ���е���ɫ������
 * 
 * �������ͨ��DIB��ÿ��������ռ��λ��������������ɫ������ɫ�ĸ���������
 * һ������ռ��һλ������������ɫ��ռ����λ������16����ɫ��ռ��8λ������
 * 256����ɫ����ռ��16�����λ������ɫ���о�û����ɫ�����ˡ�
 *
 * ��ע�⣺16λ��32λλͼ����ɫ�����ǿյģ�����������DWORD�͵���ֵ����
 * �Ա�ʾ�졢�̡�����λ�����е����룬��24λλͼ����ɫ�����ǿյģ�����ɫ
 * ��ĳߴ�Ϊ0����������������������Ϊ��ɫ�����������16λ��32λλͼ����
 * �����Խ�����0�����λͼ��Ϣ��biClrUsed��Ա����0����������ʹ�����ֵ����
 * ��ȥ���㣩
 ************************************************************************/

WORD CBmpProc::DIBNumColors(LPBITMAPINFOHEADER lpbi) const
{
	ASSERT(lpbi);
	ASSERT(lpbi->biSize == sizeof(BITMAPINFOHEADER));
	WORD wBitCount = GetBitsPerPixel(lpbi);  // DIBλ����

	// ���λͼָ������ɫ������ɫ�ĸ�������ֱ��ʹ�����ֵ
	if (lpbi->biClrUsed)
		return (WORD)(lpbi->biClrUsed);

	/*  ͨ��DIB��ÿ��������ռ��λ��������������ɫ������ɫ�ĸ��� */
	switch (wBitCount)
	{
		case 1:
			return 2;		// ��ɫλͼ��ֻ�кڰ�������ɫ
		case 4:
			return 16;		// ��׼VGAλͼ����16����ɫ
		case 8:
			return 256;		// SVGAλͼ����256����ɫ
		case	16:			// 64Kɫλͼ
		case	24:			// 16Mɫλͼ�����ɫ��
		case	32:			// 16M+ɫλͼ�����ɫ��
			return 0;		// ��ɫ����û����ɫ���ݷ���0
		default:
			ASSERT(FALSE);
			return 0;
	}
}



/*************************************************************************
 *
 * CreateMemoryBlockFromAddress()
 *
 * ����˵����
 *
 * LPVOID pSou		- �����Ƶ��ڴ���ַ��������NULL���μ�ע�ͣ�
 * DWORD dwCount	- ���Ʒ�Χ���������ֽڵĸ�����������0���μ�ע�ͣ�
 *
 * ����ֵ��
 *
 * LPVOID           - �µ��ڴ���ַ�����ʧ�ܣ�����NULL
 *
 * ������
 *
 * �����ڴ�顣��Ҳ�����ڴ����ڴ�飩
 *
 * �ú����ȴ���һ������ΪdwCount���ڴ棬�����´������ڴ��ʼ��Ϊ0��Ȼ��
 * ����pSou�г���ΪdwCount�����ݵ��µ��ڴ�顣���pSouΪNULL���򲻸���
 *
 * �����ڲ���dwCount��ָ��Ϊ0����������������ԭ��ַ�����ݣ����ڴ��
 * �ĳߴ罫��Դ��ַ�ڴ��ĳߴ���ͬ
 * ��ڲ���pSou��dwCount����ͬʱΪ0��������������NULL
 * 
 * �������ص��ڴ�鲻��ʹ��ʱ��Ӧ����free()���������ͷ�
 *
 * pSou����ָ�����ڴ���������malloc(),calloc(),realloc()���������!!!
 * 
 ************************************************************************/


LPVOID CBmpProc::CreateMemoryBlockFromAddress(LPVOID pSou, DWORD dwCount)
{
	// ��ڲ���pSou��dwCount����ͬʱΪ0
	if ((pSou==NULL)&&(dwCount==0))
		return NULL;

	LPSTR lpt;

	// ֻ������������
	if (pSou == NULL)
	{
		ASSERT(dwCount);
		lpt = (LPSTR)::malloc((size_t)dwCount);
		if (!lpt)
			return NULL;

		::memset((void *)lpt, 0, dwCount);

		return lpt;
	}

	DWORD dwCopyNum, dwAllocNum;

	if (dwCount == 0)
	{
		dwCopyNum = (DWORD)::_msize((void*)pSou);
		dwAllocNum = dwCopyNum;
	}
	else
	{
		// ���pSou�ڴ��ĳߴ�С��dwCountָ����ֵ����pSou�е�����
		// ȫ�����Ƶ��µ��ڴ���У�����ֻ����dwCount���ȵ�����
		dwCopyNum = min((DWORD)::_msize((void*)pSou), dwCount);
		dwAllocNum = dwCount;
	}

	lpt = (LPSTR)::malloc((size_t)dwAllocNum);

	if (!lpt)
		return NULL;

	// ��ʼ�����ڴ��
	::memset((void *)lpt, 0, dwAllocNum);

	// ����Դ�ڴ��е����ݵ��µ��ڴ���
	::memcpy((void*)lpt, (const void*)pSou, (size_t)dwCopyNum);

	return (LPVOID)lpt;
}



/*************************************************************************
 *
 * CreateMemoryBlockFromHandle()
 *
 * ����˵����
 *
 * HANDLE handle	- �����Ƶ��ڴ�����������NULL���μ�ע�ͣ�
 * DWORD dwCount	- �´��ڴ��ĳߴ磨������0���μ�ע�ͣ�
 *
 * ����ֵ��
 *
 * LPVOID           - �µ��ڴ���ַ�����ʧ�ܣ�����NULL
 *
 * ������
 *
 * ����ָ���ߴ���ڴ������ݵ�һ���µ��ڴ���У�Ҳ�����ڴ����ڴ�飩
 *
 * ע��
 *
 * �ú����ȴ���һ��ָ�����ȵ��ڴ�飬�����´������ڴ��ʼ��Ϊ0��Ȼ��
 * ���ƾ��handle��ָ����С�����ݵ��µ��ڴ���С�
 *
 * �����ڲ���handle��ָ��ΪNULL������ֻ�����ڴ棬������
 * �����ڲ���dwCount��ָ��Ϊ0����������������ԭ��������ݣ����ڴ��
 * �ĳߴ罫��Դ����ĳߴ���ͬ
 * ��ڲ���handle��dwCount����ͬʱΪ0��������������NULL
 *
 * �������ص��ڴ�鲻��ʹ��ʱ��Ӧ����free()���������ͷ�
 *
 ************************************************************************/


LPVOID CBmpProc::CreateMemoryBlockFromHandle(HANDLE handle, DWORD dwCount)
{
	// ��ڲ���handle��dwCount����ͬʱΪ0
	if ((handle==NULL)&&(dwCount==0))
		return NULL;

	LPSTR lpt;

	// ֻ������������
	if (handle == NULL)
	{
		ASSERT(dwCount);
		lpt = (LPSTR)::malloc((size_t)dwCount);
		if (!lpt)
			return NULL;

		::memset((void *)lpt, 0, dwCount);

		return lpt;
	}

	DWORD dwCopyNum, dwAllocNum;

	if (dwCount == 0)
	{
		dwCopyNum = ::GlobalSize((HGLOBAL)handle);
		dwAllocNum = dwCopyNum;
	}
	else
	{
		// ���ָ���ĳߴ�ֵ���ھ���ߴ磬��ʹ�þ���ߴ�ֵ
		dwCopyNum = min(::GlobalSize((HGLOBAL)handle), dwCount);
		dwAllocNum = dwCount;
	}

	lpt = (LPSTR)::malloc((size_t)dwAllocNum);

	if (!lpt)
		return NULL;

	::memset((void *)lpt, 0, dwAllocNum);

	// �����ڴ��������ݵ��µ��ڴ����
	void* lp = ::GlobalLock((HGLOBAL)handle);
	if (!lp)
	{
		::free((void*)lpt);
		return NULL;
	}
	::memcpy((void*)lpt, (const void*)lp, (size_t)dwCopyNum);
	::GlobalUnlock(handle);

	return (LPVOID)lpt;
}



/*************************************************************************
 *
 * GetBitsPerPixel()
 * 
 * ����˵����
 *
 * LPBITMAPINFOHEADER lpbi	- ָ���ڴ�DIB��BITMAPINFOHEADER�ṹ
 *
 * ����ֵ��
 *
 * int						- ����ÿ��������ռ��λ����
 *
 * ������
 *
 * ��ȡָ��DIB��ÿ��������ռ��λ����
 *
 ************************************************************************/


int	CBmpProc::GetBitsPerPixel(LPBITMAPINFOHEADER lpbi) const
{
	ASSERT(lpbi);
	ASSERT(lpbi->biSize == sizeof(BITMAPINFOHEADER));
	return lpbi->biBitCount;  // DIBλ����
}






/***************************** �ڲ����ʺ��� **************************/

/*************************************************************************
 *
 * Size()
 * 
 * ����˵������
 *
 *
 * ����ֵ��
 *
 * CSize				- ���ذ���ͼ��ߴ��CSizeĿ�꣬���ʧ�ܣ�CSizeĿ��
 *						  �еĿ�Ⱥ͸߶ȶ���0
 *
 * ������
 *
 * ��ȡ����ͼ��Ŀ�Ⱥ͸߶ȣ�������Ϊ��λ��
 *
 ************************************************************************/


CSize CBmpProc::Size() const
{ 
	if (m_mark == FALSE)
		return CSize(0,0);

	ASSERT(m_pInfo);
	ASSERT(DIBWidth((LPBITMAPINFOHEADER)m_pInfo));	// ��Ⱥ͸߶Ȳ���Ϊ0
	ASSERT(DIBHeight((LPBITMAPINFOHEADER)m_pInfo));

	return CSize(DIBWidth((LPBITMAPINFOHEADER)m_pInfo), DIBHeight((LPBITMAPINFOHEADER)m_pInfo));
}



/*************************************************************************
 *
 * Rect()
 * 
 * ����˵������
 *
 *
 * ����ֵ��
 *
 * CRect				- ���ذ���ͼ��ߴ��CRectĿ�꣬���ʧ�ܣ�CRectĿ��
 *						  �е����ݾ���0
 *
 * ������
 *
 * ��ȡ����ͼ��ĳߴ����ݣ�������Ϊ��λ��
 *
 ************************************************************************/


CRect CBmpProc::Rect() const
{ 
	if (m_mark == FALSE)
		return CRect(0,0,0,0);

	ASSERT(m_pInfo);

	return CRect(CPoint(0, 0), Size()); 
}



/*************************************************************************
 *
 * Width()
 * 
 * ����˵������
 *
 *
 * ����ֵ��
 *
 * int					- ����ͼ��Ŀ�ȣ����ʧ�ܷ���0
 *
 * ������
 *
 * ��ȡ����ͼ��Ŀ�����ݣ�������Ϊ��λ��
 *
 ************************************************************************/


int CBmpProc::Width() const
{
	if (m_mark == FALSE)
		return 0;

	ASSERT(m_pInfo);

	return (int)DIBWidth((LPBITMAPINFOHEADER)m_pInfo);
}



/*************************************************************************
 *
 * Height()
 * 
 * ����˵������
 *
 *
 * ����ֵ��
 *
 * int					- ����ͼ��ĸ߶ȣ����ʧ�ܷ���0
 *
 * ������
 *
 * ��ȡ����ͼ��ĸ߶����ݣ�������Ϊ��λ��
 *
 ************************************************************************/


int CBmpProc::Height() const
{ 
	if (m_mark == FALSE)
		return 0;

	ASSERT(m_pInfo);

	return (int)DIBHeight((LPBITMAPINFOHEADER)m_pInfo);
}



/*************************************************************************
 *
 * ScanWidth()
 * 
 * ����˵������
 *
 *
 * ����ֵ��
 *
 * int					- ����ͼ��ÿһɨ���еĿ�ȣ����ʧ�ܷ���0
 *
 * ������
 *
 * ��ȡ����ͼ���ɨ���п�ȣ����ֽ�Ϊ��λ��
 *
 * ע��DIBλͼɨ������DWORD�Ͷ���ģ�DDBλͼɨ������WORD�����
 *
 ************************************************************************/


int CBmpProc::ScanWidth() const
{ 
	if (m_mark == FALSE)
		return 0;

	ASSERT(m_pInfo);

	return DIBSCANLINE_WIDTHBYTES(Width()*GetBitsPerPixel((LPBITMAPINFOHEADER)m_pInfo));
}



/*************************************************************************
 *
 * BitsPerPixel()
 * 
 * ����˵������
 *
 *
 * ����ֵ��
 *
 * int					- ����ͼ����ÿ��������ռ��λ�������ʧ�ܷ���0
 *
 * ������
 *
 * ��ȡ����ͼ��ÿ��������ռ��λ�ĸ���������16ɫλͼÿ������ռ4λ
 *
 ************************************************************************/


int CBmpProc::BitsPerPixel() const
{ 
	if (m_mark == FALSE)
		return 0;

	ASSERT(m_pInfo);

	return GetBitsPerPixel((LPBITMAPINFOHEADER)m_pInfo);
}



/*************************************************************************
 *
 * IsValid()
 * 
 * ����˵������
 *
 *
 * ����ֵ��
 *
 * BOOL					- ������д���λͼ�򷵻�TRUE�����򷵻�FALSE
 *
 * ������
 *
 * �ж������Ƿ������Ч��λͼ
 *
 ************************************************************************/


BOOL  CBmpProc::IsValid() const
{ 
	return m_mark;
}



/*************************************************************************
 *
 * GetImageType()
 * 
 * ����˵������
 *
 *
 * ����ֵ��
 *
 * int					- ͼ������ͱ���
 *
 * ������
 *
 * ��ȡ����λͼ����Դ���ͱ���
 * 
 * ע�����ص���ֵ����CBmpProc::IT_MEMORY�����Ƶķ������б�������������
 *     ֵ֮һ��
 *
 * 		CBmpProc::IT_NULL,		�� ��λͼ����	0   ������û��λͼ��
 *		CBmpProc::IT_DISKFILE,	�� �ļ�λͼ		1
 *		CBmpProc::IT_RESOURCE,	�� ��Դλͼ		2
 *		CBmpProc::IT_MEMORY,	�� �ڴ�λͼ		3
 *		CBmpProc::IT_CLIP,		�� ������λͼ	4
 *		CBmpProc::IT_CREATE		�� ������λͼ	5
 *
 ************************************************************************/


int CBmpProc::GetImageType() const
{ 
	return (int)m_type; 
}



/*************************************************************************
 *
 * GetInAddInfo()
 * 
 * ����˵������
 *
 *
 * ����ֵ��
 *
 * DWORD				- ������Ϣ����
 *
 * ������
 *
 * ��ȡ����λͼ�ĸ�����Ϣ
 * 
 * ע��������Ϣ�����ݻ���λͼ��Դ���͵Ĳ�ͬ����ͬ����λͼ��Դ�������
 *     CBmpProc::IT_RESOURCE������ʹ�õ��Ǳ�ʶ���򸽼���Ϣ�͵������
 *     ��ʶ��ֵ���������������0��û�����壩
 *
 ************************************************************************/


DWORD CBmpProc::GetInAddInfo() const
{ 
	return m_addInfo; 
}



/*************************************************************************
 *
 * GetFileName()
 * 
 * ����˵������
 *
 *
 * ����ֵ��
 *
 * const CString&		- �ļ�������������
 *
 * ������
 *
 * ��ȡ����λͼ���ļ�����
 * 
 * ע����Ҫ�ñ������ķ���ֵ���ı��ļ����ƣ�������ʹ��SetFileName()������
 *
 ************************************************************************/


const CString& CBmpProc::GetFileName() const
{ 
	return m_cFileName; 
}



/*************************************************************************
 *
 * SetFileName()
 * 
 * ����˵����
 *
 * LPSTR pSou			- �µ��ļ�����
 *
 * ����ֵ��
 *
 * BOOL					- ������óɹ�����TRUE�����򷵻�FALSE
 *
 * ������
 *
 * ��������λͼ���ļ�����
 * ���ԭ������û��λͼ������ִ��ʧ��
 * 
 ************************************************************************/


BOOL CBmpProc::SetFileName(LPSTR pSou)
{
	if (m_mark == FALSE)
		return FALSE;

	ASSERT(pSou);

	m_cFileName = pSou; 

	return TRUE;
}



/*************************************************************************
 *
 * GetObject()
 * 
 * ����˵����
 *
 * LPBITMAP lpBitmap	- ������Ϣ���ݵĽṹ������ַ
 *
 * ����ֵ��
 *
 * int					- ����ɹ������Ի�ȡ���ֽڸ��������ʧ�ܣ�����0
 * 
 * ������
 *
 * ��ȡλͼ��BITMAP��Ϣ
 *
 * ע��# �����ɹ�����֮��BITMAP�ṹ��ֻ��width,height����ɫ��ʽ��Ϣ��Ч
 *     # �������ԭ��û��λͼ������ʧ�ܣ�����0
 * 
 ************************************************************************/


int CBmpProc::GetObject(LPBITMAP lpBitmap) const
{ 
	if (m_mark == FALSE)
		return 0;

	ASSERT(m_hObject);
	ASSERT(lpBitmap);

	return CGdiObject::GetObject(sizeof(BITMAP), (LPVOID)lpBitmap); 
}



/*************************************************************************
 *
 * GetVersion()
 * 
 * ����˵������
 *
 *
 * ����ֵ��
 *
 * WORD					- �汾��
 * 
 * ������
 *
 * ��ȡCBmpProc��İ汾��
 *
 * ע������ֵ�еĸ�8λ�����汾�ţ���8λ���Ӱ汾��
 * 
 ************************************************************************/


WORD CBmpProc::GetVersion() const
{ 
	return (WORD)BMPPROC_VERSION; 
}



/*************************************************************************
 *
 * PasteAvailable()
 * 
 * ����˵������
 *
 *
 * ����ֵ��
 *
 * BOOL					- ����������д���DIB��ʽ�����ݣ��򷵻�TRUE������
 *						  ����FALSE
 * 
 * ������
 *
 * �жϼ��������Ƿ����DIB��ʽ�����ݡ�
 *
 ************************************************************************/


BOOL CBmpProc::PasteAvailable()
{
	return (IsClipboardFormatAvailable(CF_DIB)) ? TRUE:FALSE;
}



/*************************************************************************
 *
 * IsFileExist()
 * 
 * ����˵����
 *
 * LPCTSTR lpFileName	- ���жϵ��ļ�·�������ƣ��ļ�����
 *
 * ����ֵ��
 *
 * BOOL					- ���ָ�����ļ����ڷ���TRUE�����򷵻�FALSE��
 * 
 * ������
 *
 * �ж�ָ�����ļ��Ƿ����
 * 
 * ���ļ�������Ա�����д
 *
 ************************************************************************/


BOOL CBmpProc::IsFileExist(LPCTSTR lpFileName)
{
	CFile	file;
	BOOL	bExist = FALSE;	// �ļ�������TRUE����������FALSE

	// ȷ��ָ�����ļ��Ƿ����
	if (file.Open(lpFileName, CFile::modeReadWrite))
	{
		bExist = TRUE;
		file.Close();
	}

	return bExist;
}



/*************************************************************************
 *
 * GetHDib()
 * 
 * ����˵������
 *
 *
 * ����ֵ��
 *
 * HANDLE				- ����ɹ�����DIB�ڲ���ʽ���ڴ����
 * 
 * ������
 *
 * ��ȡ������λͼ��DIB�ڲ���ʽ���ڴ����
 * 
 * # �����������ԭ��û��λͼ����������NULL
 * # ���ص��ڴ�������GlobalAlloc()��������ģ��������ʹ��ʱӦ��ʱ�ͷ�
 * # ���صľ��������������壬����������DIB��ͨ��
 *
 ************************************************************************/


HANDLE CBmpProc::GetHDib() const
{ 
	// �����������ԭ��û��λͼ����������NULL
	if (!IsValid())
		return NULL;

	ASSERT(m_pInfo);

	// DIB�ڲ���ʽ�ߴ磨dibsize��
	DWORD infosize = CalculateDIBInfoSize((LPBITMAPINFOHEADER)m_pInfo);
	DWORD dibsize = CalculateDIBSize((LPBITMAPINFOHEADER)m_pInfo);

	// ����DIB�ڴ�飬��GMEM_DDESHARE��ʶ����ʹ�þ�����ڼ�����
	HANDLE h = ::GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, dibsize);
	if (!h)
		return NULL;

	LPSTR lp = (LPSTR)::GlobalLock(h);
	if (lp)
	{
		// ����DIB��Ϣ
		::memcpy((void *)lp, (const void *)m_pInfo, (size_t)infosize);

		// ��ȡDIBλ����
		HWND hWnd = ::GetDesktopWindow();
		ASSERT(hWnd);
		HDC hDC = ::GetDC(hWnd);
		if (!::GetDIBits(hDC,(HBITMAP)m_hObject,0,m_pInfo->bmiHeader.biHeight,\
				(LPVOID)((DWORD)lp+infosize),(LPBITMAPINFO)lp,DIB_RGB_COLORS))
		{
			::GlobalUnlock(h);
			::GlobalFree(h);
			::ReleaseDC(hWnd, hDC);
			return NULL;
		}
		::ReleaseDC(hWnd, hDC);
		::GlobalUnlock(h);

		return h;	// �ɹ�
	}

	::GlobalFree(h);

	return NULL;
}



/*************************************************************************
 *
 * IsValidDibFormat()
 * 
 * ����˵����
 *
 * WORD nBitsPerPixel	- ���ж���λ��ֵ
 *
 * ����ֵ��
 *
 * BOOL					- ����Чֵ�򷵻�TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * �ж�ָ����λ��ֵ����ÿ����ռ����λ���Ƿ��ǺϷ�ֵ
 * 
 * # CBmpProc�൱ǰ֧��1,4,8,16,24,32λ��ʽ��λͼ
 *
 ************************************************************************/


BOOL CBmpProc::IsValidDibFormat(WORD nBitsPerPixel)
{
	// ���λ���Ϸ���
	switch(nBitsPerPixel)
	{
	case	1:
	case	4:
	case	8:
	case	16:
	case	24:
	case	32:
		// ���ϸ�ʽ��λͼ�ǺϷ���
		return TRUE;
	default:
		ASSERT(FALSE);	// ��ƴ���
		break;
	}

	return FALSE;
}



/*************************************************************************
 *
 * ClearAndSetData()
 * 
 * ����˵����
 *
 * int type				- ��λͼ��Դ����
 * DWORD addinfo		- ��λͼ�ĸ�����Ϣֵ
 * LPCTSTR lpFn			- ��λͼ���ļ����ַ���
 * LPBITMAPINFO pInfo	- ��λͼ����Ϣ���ַ
 * HBITMAP hBmp			- ��λͼ��λͼ���
 *
 * ����ֵ��
 *
 * BOOL					- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * �������ԭ����λͼ������ָ������������λͼ
 * 
 * # �����������ԭ����λͼ���������������
 * # ���ָ���Ĳ������뱣֤����Ч������
 *
 ************************************************************************/


BOOL CBmpProc::ClearAndSetData(int type, 
							   DWORD addinfo, 
							   LPCTSTR lpFn, 
							   LPBITMAPINFO pInfo, 
							   HBITMAP hBmp)
{
	ASSERT(pInfo);
	ASSERT(hBmp);

	// ���ԭ������λͼ
	if (m_mark == TRUE)
	{
		// �����ԭ����λͼ���
		ASSERT((HBITMAP)m_hObject);
		HBITMAP hOldBmp = (HBITMAP)CGdiObject::Detach();
		ASSERT(hOldBmp);
		if (!CGdiObject::Attach((HGDIOBJ)hBmp))
		{
			// ���ʧ�ܣ��ָ�ԭ����λͼ�����
			CGdiObject::Attach((HGDIOBJ)hOldBmp);
			return FALSE;
		}
		// ����ɹ���ԭ����λͼ��������ã��ͷ�
		::DeleteObject((HGDIOBJ)hOldBmp);

		m_type = type,			// λͼ��Դ
		m_addInfo = addinfo;	// ������Ϣ
		m_cFileName.Empty();
		m_cFileName = lpFn;		// �ļ�·����
		ASSERT(m_pInfo);
		::free((void*)m_pInfo);
		m_pInfo = pInfo;		// DIB��Ϣ�ṹ

		return TRUE;
	}
	else	// ԭ��û��λͼ
	{
		ASSERT(m_pInfo == NULL);
		ASSERT(m_hObject == NULL);

		if (!CGdiObject::Attach((HGDIOBJ)hBmp))
			return FALSE;

		m_mark = TRUE;			// ��Ч��־��
		m_type = type,			// λͼ��Դ
		m_addInfo = addinfo;	// ������Ϣ
		m_cFileName = lpFn;		// �ļ�·����
		m_pInfo = pInfo;		// DIB��Ϣ�ṹ

		return TRUE;
	}
}



/*************************************************************************
 *
 * DrawTransparentInPoint()
 * 
 * ����˵����
 *
 * CDC& dc					- ��ʾλͼ���豸������
 * COLORREF crColour		- ͼ����Ҫ���˵�����ɫ��ȱʡֵΪ��ɫ��
 * int x 					- Ŀ����ε����Ͻ����� X��ȱʡֵΪ0��
 * int y 					-           ''         Y��ȱʡֵΪ0��
 * CRect* crSou				- Դͼ�����ʾ���ֵľ������꣨ȱʡֵ��NULL��
 * int mode					- �����0�򽫲���ʾָ������ɫ�������1����ָ��
 *						      ����ɫ��ȱʡֵΪ0��
 *
 * ����ֵ��
 *
 * BOOL					- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * �����е�ͼ��͸���Ļ���Ŀ���豸������ָ���������б�Ҫ���Բ��������ѹ��
 * ����
 * 
 * �������û��ͼ����ʲôҲ������ֱ�ӷ���
 * ��������Ƶ�ͼ���ڵ�ǰ���������ڣ���ֱ�ӷ���
 * �����ڲ���crSou����ΪNULL���������������ͼ��������ѹ��
 * 
 ************************************************************************/


BOOL CBmpProc::DrawTransparentInPoint(CDC& dc, 
									  COLORREF crColour, 
									  int x, 
									  int y, 
									  CRect* crSou, 
									  int mode)
{
	// �������û��ͼ��ֱ�ӷ��� 
	if (!IsValid())
		return TRUE;

	ASSERT(m_hObject);

	CRect DCRect(Rect());			// Ŀ��λ��
	DCRect.OffsetRect(x, y);

	return DrawTransparentInRect(dc, crColour, &DCRect, crSou, mode);
}



/*************************************************************************
 *
 * DrawTransparentInRect()
 * 
 * ����˵����
 *
 * CDC& dc					- ��ʾλͼ���豸������
 * COLORREF crColour		- ͼ����Ҫ���˵�����ɫ��ȱʡֵΪ��ɫ��
 * CRect* crDest			- Ŀ������������ݣ�ȱʡֵ��NULL��
 * CRect* crSou				- Դͼ�����ʾ���ֵľ������꣨ȱʡֵ��NULL��
 * int mode					- �����0�򽫲���ʾָ������ɫ�������1����ָ��
 *						      ����ɫ��ȱʡֵΪ0��
 *
 * ����ֵ��
 *
 * BOOL					- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * �����е�ͼ��͸���Ļ���Ŀ���豸������ָ���������б�Ҫ���Բ��������ѹ��
 * ����
 * 
 * �������û��ͼ����ʲôҲ������ֱ�ӷ���
 * ��������Ƶ�ͼ���ڵ�ǰ���������ڣ���ֱ�ӷ���
 * �����ڲ���crDest��crSou����NULL�����������豸�������0,0λ�ò����ε�
 * ��������λͼ����������
 * �����ڲ���crDest��crSou�Ŀ�Ȼ�߶�ֵ��ͬ������Ƴ�����ͼ�����Σ���
 * �����ѹ����
 * 
 ************************************************************************/


BOOL CBmpProc::DrawTransparentInRect(CDC& dc, 
							   COLORREF crColour, 
							   CRect* crDest, 
							   CRect* crSou, 
							   int mode)
{
	// �������û��ͼ��ֱ�ӷ��� 
	if (!IsValid())
		return TRUE;

	ASSERT(m_hObject);

	// ȱʡ���ε���ͼ��ߴ�
	CRect DCRect(Rect());			// Ŀ��λ��
	CRect SORect(Rect());			// Դλ��

	if (crDest)
		DCRect = *crDest;
	if (crSou)
		SORect = *crSou;

	// ��������Ƶ�ͼ���ڵ�ǰ���������ڣ���ֱ�ӷ���
	if (!dc.RectVisible(&DCRect))
		return TRUE;

	COLORREF crOldBack;
	COLORREF crOldText;

	if (mode==0)
	{	// ȥ��ָ������ɫ
		crOldBack = dc.SetBkColor(RGB(255,255,255));
		crOldText = dc.SetTextColor(RGB(0,0,0));
	}
	else
	{	// ����ָ������ɫ
		crOldBack = dc.SetBkColor(RGB(0,0,0));
		crOldText = dc.SetTextColor(RGB(255,255,255));
	}

	CDC dcImage, dcTrans;

	// �����ڴ��豸������
	dcImage.CreateCompatibleDC(&dc);
	dcTrans.CreateCompatibleDC(&dc);

	// ѡ��ԭʼͼ��DC��
	CBitmap* pOldBitmapImage = dcImage.SelectObject(this);

	// ��������λͼ
	CBitmap bitmapTrans;
	int nWidth = SORect.Width();
	int nHeight = SORect.Height();
	bitmapTrans.CreateBitmap(nWidth, nHeight, 1, 1, NULL);

	// ѡ������λͼ��DC��
	CBitmap* pOldBitmapTrans = dcTrans.SelectObject(&bitmapTrans);

	// ��������ͼ�񣨻���ָ������ɫ��
	dcImage.SetBkColor(crColour);
	dcTrans.BitBlt(0, 0, nWidth, nHeight, &dcImage, SORect.left, SORect.top, SRCCOPY);

	// ��ʾ͸��λͼ

	/* ����Ŀ��DC������ģʽΪSTRETCH_DELETESCANS��Ҳ���ǲ���ʾ�������ͼ�� */
	int srlold = dc.SetStretchBltMode(STRETCH_DELETESCANS);

	// ��ʾλͼ
	dc.StretchBlt(DCRect.left,DCRect.top,DCRect.Width(),DCRect.Height(),
		&dcImage,SORect.left,SORect.top,SORect.Width(),SORect.Height(),SRCINVERT);
	dc.StretchBlt(DCRect.left,DCRect.top,DCRect.Width(),DCRect.Height(),
		&dcTrans,SORect.left,SORect.top,SORect.Width(),SORect.Height(),SRCAND);
	dc.StretchBlt(DCRect.left,DCRect.top,DCRect.Width(),DCRect.Height(),
		&dcImage,SORect.left,SORect.top,SORect.Width(),SORect.Height(),SRCINVERT);

	// �ָ��豸������ԭ��������
	dc.SetStretchBltMode(srlold);

	// �ָ�����
	dcImage.SelectObject(pOldBitmapImage);
	dcTrans.SelectObject(pOldBitmapTrans);
	dc.SetBkColor(crOldBack);
	dc.SetTextColor(crOldText);

	return TRUE;
}



/*************************************************************************
 *
 * ExportRegion()
 * 
 * ����˵����
 *
 * COLORREF cTranColor		- ָ��Ҫ�˵�����ɫ��ȱʡֵ�ǰ�ɫ��
 *
 * ����ֵ��
 *
 * HRGN 					- ����ɹ�����λͼ�������������򷵻�NULL
 * 
 * ������
 *
 * �������λͼ��������
 * 
 * �ú������ǽ�ָ������ɫ����������֮�ڣ�����������ɫ���������һ������
 * �������û��ͼ����ʲôҲ������ֱ�ӷ���NULL
 * ����ʱ�䲻��Ҫ������ص�������ʱ��Ӧ����DeleteObject()���������ͷ�
 * 
 ************************************************************************/


HRGN CBmpProc::ExportRegion(COLORREF cTranColor)
{
	HRGN hRgn = NULL;

	// �������û��ͼ��ֱ�ӷ���
	if (!IsValid())
		return NULL;

	ASSERT(m_hObject);

	int wi, he;

	wi = Width();
	he = Height();

	// �����ڴ��豸���������ڻ�ȡ������λͼ��λ����
	HDC hMemDC = ::CreateCompatibleDC(NULL);
	if (hMemDC)
	{
		BITMAPINFOHEADER bmi = {sizeof(BITMAPINFOHEADER), wi, he, 1, 32, BI_RGB, 0, 0, 0, 0, 0};
		VOID * pbits32; 
		// ����λͼ����(Section)����ȡλ����
		HBITMAP hbm32 = ::CreateDIBSection(hMemDC, (BITMAPINFO *)&bmi, DIB_RGB_COLORS, &pbits32, NULL, 0);
		if (hbm32)
		{
			HBITMAP holdBmp = (HBITMAP)::SelectObject(hMemDC, hbm32);
			HDC hDC = ::CreateCompatibleDC(hMemDC);
			if (hDC)
			{
				BITMAP bm32;
				::GetObject(hbm32, sizeof(BITMAP), &bm32);
				while (bm32.bmWidthBytes % 4)	// DIBɨ����ΪDWORD����
					bm32.bmWidthBytes++;

				HBITMAP hob = (HBITMAP)::SelectObject(hDC, (HBITMAP)m_hObject);
				::BitBlt(hMemDC, 0, 0, wi, he, hDC, 0, 0, SRCCOPY);
				::SelectObject(hDC, hob);
				::DeleteDC(hDC);

				// ������θ����Ĳ���ֵ��Ϊ200
				#define ALLOC_UNIT	200

				DWORD maxRects = ALLOC_UNIT;
				RGNDATA *pData = (RGNDATA *)CreateMemoryBlockFromAddress(NULL, sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRects));
				if (!pData)
				{
					::DeleteObject(::SelectObject(hMemDC, holdBmp));
					::DeleteDC(hMemDC);
					return NULL;
				}

				// ��ʼ���������ݽṹ
				pData->rdh.dwSize = sizeof(RGNDATAHEADER);
				pData->rdh.iType = RDH_RECTANGLES;
				pData->rdh.nCount = 0;
				pData->rdh.nRgnSize = 0;
				::SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

				// ��������
				BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight-1)*bm32.bmWidthBytes;
				for (int y = 0; y < he; y++)
				{
					for (int x = 0; x < wi; x++)
					{
						int x0 = x;
						LONG *p = (LONG *)p32 + x;
						while (x < wi)
						{	// ָ��Ҫ�˵�����ɫ
							if (((*p)&0xFFFFFF)==((LONG)cTranColor&0xFFFFFF))
								break;
							p++;
							x++;
						}

						if (x > x0)	// �����з��˵�����ɫ
						{
							if (pData->rdh.nCount >= maxRects)
							{	// �������ṹ�в����������µľ������ݣ�����׷���ڴ�
								ASSERT(pData);
								ASSERT(pData->rdh.dwSize == sizeof(RGNDATAHEADER));
								maxRects += ALLOC_UNIT;
								RGNDATA *p = (RGNDATA *)CreateMemoryBlockFromAddress((LPVOID)pData, sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRects));
								::free((void*)pData);
								if (!p)
								{
									if (hRgn)
										::DeleteObject(hRgn);
									::DeleteObject(::SelectObject(hMemDC, holdBmp));
									::DeleteDC(hMemDC);
									return NULL;
								}
								ASSERT(p->rdh.dwSize == sizeof(RGNDATAHEADER));
								pData = p;
							}

							// ˢ������ṹ�е�����
							RECT *pr = (RECT *)&pData->Buffer;
							SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
							if (x0 < pData->rdh.rcBound.left)
								pData->rdh.rcBound.left = x0;
							if (y < pData->rdh.rcBound.top)
								pData->rdh.rcBound.top = y;
							if (x > pData->rdh.rcBound.right)
								pData->rdh.rcBound.right = x;
							if (y+1 > pData->rdh.rcBound.bottom)
								pData->rdh.rcBound.bottom = y+1;
							pData->rdh.nCount++;

							// ��Ϊ��������ĺ���ExtCreateRegion()Ҫ���������ݽṹ�еľ��θ������ܳ���4000����
							// ��������ȡ����ֵ��һ�롣
							if (pData->rdh.nCount == 2000)
							{
								HRGN h = ::ExtCreateRegion(NULL, sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRects), pData);
								if (hRgn)
								{
									::CombineRgn(hRgn, hRgn, h, RGN_OR);
									::DeleteObject(h);
								}
								else
									hRgn = h;
								pData->rdh.nCount = 0;
								::SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
							}
						}
					}
					// ָ����һ�е��׵�ַ��DIBɨ�����Ǵ��µ��ϣ�
					p32 -= bm32.bmWidthBytes;
				}

				ASSERT(pData);

				// �������λͼȫ��ָ����Ҫ�˵�����ɫ������������θ�������
				// 2000���������´�����ɴ�������Ĺ���
				HRGN hg = ::ExtCreateRegion(NULL, sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRects), pData);
				if (hRgn)
				{
					::CombineRgn(hRgn, hRgn, hg, RGN_OR);
					::DeleteObject(hg);
				}
				else
					hRgn = hg;

				::free((void*)pData);
			}
			::DeleteObject(::SelectObject(hMemDC, holdBmp));
		}
		::DeleteDC(hMemDC);
	}	

	return hRgn;
}



/*************************************************************************
 *
 * Fill()
 * 
 * ����˵����
 *
 * COLORREF crColour		- ָ��Ҫ������ɫ��ȱʡֵ�ǰ�ɫ��
 * CRect *crArea			- ����������Σ�ȱʡֵ��NULL��
 *
 * ����ֵ��
 *
 * BOOL 					- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ��ָ������ɫ��䱾������λͼ��ָ�����򣨾��Σ�
 * 
 * �����ڲ���crArea����ΪNULL�����������������λͼ
 * �������û��ͼ��ֱ�ӷ���FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::Fill(COLORREF crColour, CRect *crArea)
{
	// �������б������λͼ
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	CRect	rect(Rect());

	if (crArea)
		rect = *crArea;

	// ����Դͼ
	CDC			compDC;
	CBitmap		*pOldBmp;

	// �����뵱ǰ��ʾ�豸���ݵ��ڴ��豸������
	compDC.CreateCompatibleDC(NULL);
	pOldBmp	= compDC.SelectObject((CBitmap*)this);

	// ���ָ����ɫ
	compDC.FillSolidRect((LPCRECT)&rect, crColour);

	compDC.SelectObject(pOldBmp);

	return TRUE;
}



/*************************************************************************
 *
 * ReSetColor()
 *
 * ����˵����
 *
 * COLORREF oldcolr			- ���滻�ľ���ɫ
 * COLORREF newcolr			- ��Ҫ�滻��ȥ���µ���ɫ
 * CRect *crArea			- Ҫ�滻�ķ�Χ��ȱʡֵ��NULL��
 *
 * ����ֵ��
 *
 * BOOL 					- ����ɹ�����TRUE�����򷵻�FALSE
 * 
 * ������
 *
 * ��һ����ɫ�滻��һ����ɫ����λͼ��ָ��������
 * 
 * �����ڲ���crArea����ΪNULL���������滻������λͼָ����ɫ
 * �������û��ͼ��ֱ�ӷ���FALSE
 * 
 ************************************************************************/


BOOL CBmpProc::ReSetColor(COLORREF oldcolr, COLORREF newcolr, CRect *crArea)
{
	// �������б������λͼ
	if (!IsValid())
		return FALSE;

	ASSERT(m_pInfo);
	ASSERT(m_pInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	// ���Ʊ������е�λͼ��Ϊ����
	CBmpProc tmp;
	
	tmp = *this;

	if (!tmp.IsValid())
		return FALSE;

	// ���ָ������Ϊ�µ���ɫ
	Fill(newcolr, crArea);
	// ����ȥ������ɫ��ͼ��
	CopyTran(tmp, oldcolr, crArea, crArea, 0);

	return TRUE;
}

void CBmpProc::RotatedBitmap(RateDirection radians)
{
	int i;
	switch(radians)
	{
	case sequence:
		RotatedBitmap(sequence,RGB(255,255,255));
		break;
	case reversion:
		for(i=0;i<3;i++)
			RotatedBitmap(sequence,RGB(255,255,255));
		break;
	case perversion:
		for(i=0;i<2;i++)
			RotatedBitmap(sequence,RGB(255,255,255));
		break;
	}
}

void CBmpProc::RotatedBitmap(RateDirection radians, COLORREF clrBack)
{
	CDC sourceDC, destDC;
	sourceDC.CreateCompatibleDC( NULL );
	destDC.CreateCompatibleDC( NULL );

	// Get logical coordinates
	BITMAP bm;
	HBITMAP hBitmap=(HBITMAP)this->GetSafeHandle();
	::GetObject( hBitmap, sizeof( bm ), &bm );

	float cosine ;//= (float)cos(radians);
	float sine ;//= (float)sin(radians);
	if(radians==sequence)
	{//˳ʱ��
		sine=-1.0;
		cosine=0.0;
	}
	else if(radians==reversion)
	{//��ʱ��
		sine=1.0;
		cosine=0.0;
	}
	else if(radians==perversion)
	{
		sine=0.0;
		cosine=-1.0;
	}
	else
		return;
	// Compute dimensions of the resulting bitmap
	// First get the coordinates of the 3 corners other than origin
	int x1 = (int)(-bm.bmHeight * sine);
	int y1 = (int)(bm.bmHeight * cosine);
	int x2 = (int)(bm.bmWidth * cosine - bm.bmHeight * sine);
	int y2 = (int)(bm.bmHeight * cosine + bm.bmWidth * sine);
	int x3 = (int)(bm.bmWidth * cosine);
	int y3 = (int)(bm.bmWidth * sine);

	int minx = min(0,min(x1, min(x2,x3)));
	int miny = min(0,min(y1, min(y2,y3)));
	int maxx = max(x1, max(x2,x3));
	int maxy = max(y1, max(y2,y3));

	int w = maxx - minx;
	int h = maxy - miny;


	// Create a bitmap to hold the result
	HBITMAP hbmResult = ::CreateCompatibleBitmap(CClientDC(NULL), w, h);

	HBITMAP hbmOldSource = (HBITMAP)::SelectObject( sourceDC.m_hDC, hBitmap );
	HBITMAP hbmOldDest = (HBITMAP)::SelectObject( destDC.m_hDC, hbmResult );

	// Draw the background color before we change mapping mode
	HBRUSH hbrBack = CreateSolidBrush( clrBack );
	HBRUSH hbrOld = (HBRUSH)::SelectObject( destDC.m_hDC, hbrBack );
	destDC.PatBlt( 0, 0, w, h, PATCOPY );
	::DeleteObject( ::SelectObject( destDC.m_hDC, hbrOld ) );

	// Set mapping mode so that +ve y axis is upwords
	sourceDC.SetMapMode(MM_ISOTROPIC);
	sourceDC.SetWindowExt(1,1);
	sourceDC.SetViewportExt(1,-1);
	sourceDC.SetViewportOrg(0, bm.bmHeight-1);

	destDC.SetMapMode(MM_ISOTROPIC);
	destDC.SetWindowExt(1,1);
	destDC.SetViewportExt(1,-1);
	destDC.SetWindowOrg(minx, maxy);

	// Now do the actual rotating - a pixel at a time
	// Computing the destination point for each source point
	// will leave a few pixels that do not get covered
	// So we use a reverse transform - e.i. compute the source point
	// for each destination point
//	if(miny>=0){miny+=1;maxy+=1;}
//	if(minx<=0){minx+=1;maxx+=1;}
	if(radians==reversion)
	{
		for( int y = miny+1; y <= maxy; y++ )
		{
			for( int x = minx; x < maxx; x++ )
			{
				int sourcex = (int)(x*cosine + y*sine)-1;
				int sourcey = (int)(y*cosine - x*sine)-1;
				if( sourcex >= 0 && sourcex < bm.bmWidth && sourcey >= 0 
						&& sourcey < bm.bmHeight )
					destDC.SetPixel(x+1,y-1,sourceDC.GetPixel(sourcex,sourcey));
			}
		}
	}
	else if(radians==sequence)
	{
		for( int y = miny; y < maxy; y++ )
		{
			for( int x = minx+1; x <= maxx; x++ )
			{
				int sourcex = (int)(x*cosine + y*sine)-1;
				int sourcey = (int)(y*cosine - x*sine)-1;
				if( sourcex >= 0 && sourcex < bm.bmWidth && sourcey >= 0 
						&& sourcey < bm.bmHeight )
					destDC.SetPixel(x-1,y+1,sourceDC.GetPixel(sourcex,sourcey));
			}
		}
	}
	else if(radians==perversion)
	{
		for( int y = miny; y < maxy; y++ )
		{
			for( int x = minx; x < maxx; x++ )
			{
				int sourcex = (int)(x*cosine + y*sine)-1;
				int sourcey = (int)(y*cosine - x*sine)-1;
				if( sourcex >= 0 && sourcex < bm.bmWidth && sourcey >= 0 
						&& sourcey < bm.bmHeight )
					destDC.SetPixel(x+1,y+1,sourceDC.GetPixel(sourcex,sourcey));
			}
		}

	}
	// Restore DCs
	::SelectObject( sourceDC.m_hDC, hbmOldSource );
	::SelectObject( destDC.m_hDC, hbmOldDest );
	this->LoadFromHbmp(hbmResult);
}
