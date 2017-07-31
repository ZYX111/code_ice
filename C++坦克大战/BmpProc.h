///////////////////////////////////////////////////////////////////////
/*
  Ŀ�ģ�BMPλͼ�����ࡣ�������ļ���

  ��������ΰ(SXZ)
  ���ڣ�2000.01
  �汾��2.4

  SXZ software workgroup. No.200001
*/
///////////////////////////////////////////////////////////////////////


#ifndef __BMPPROC_H__
#define __BMPPROC_H__

/* �汾 */
#define BMPPROC_VERSION   0x240

// DIBSCANLINE_WIDTHBYTES ִ��DIBɨ���е�DWORD����������������bits����
// DIB��Ϣ�ṹ�г�ԱbiWidth��biBitCount�ĳ˻�����Ľ���Ǿ��������һ��
// ɨ������ռ���ֽ�����
#define DIBSCANLINE_WIDTHBYTES(bits)    (((bits)+31)/32*4)
// DDBSCANLINE_WIDTHBYTES ִ��DDBɨ���е�WORD����������������bits����
// DDB��Ϣ�ṹ�г�ԱbmWidth��bmBitCount�ĳ˻�����Ľ���Ǿ��������һ��
// ɨ������ռ���ֽ�����
#define DDBSCANLINE_WIDTHBYTES(bits)    (((bits)+15)/16*2)

/* DIB �ļ�ͷ��־��"BM") */
#define DIB_HEADER_MARKER   ((WORD)('M'<<8)|'B')

// ������ɫ����
#define CBPCOLOR_BLACK		((COLORREF)RGB(0,0,0))
#define CBPCOLOR_GRAY		((COLORREF)RGB(128,128,128))
#define CBPCOLOR_HIGHGRAY	((COLORREF)RGB(192,192,192))
#define CBPCOLOR_WHITE		((COLORREF)RGB(255,255,255))
#define CBPCOLOR_RED		((COLORREF)RGB(255,0,0))
#define CBPCOLOR_GREEN		((COLORREF)RGB(0,255,0))
#define CBPCOLOR_BLUE		((COLORREF)RGB(0,0,255))


// ע�⣺��CBmpProc�Ǵ�CBitmap�๫�м̳ж����ģ�����ò�Ҫʹ����CBitmap
// �еĺ��������ʹ���ˣ������Ƕ���򴴽��͵ĺ���������Ͳ�Ҫ��ʹ��CBmpProc
// ���еĺ�����������ҡ�

class CBmpProc : public CBitmap
{
public:
	CBmpProc();
	~CBmpProc();

	BOOL Create(int width, int height, WORD nBitCount, const void *lpBits=NULL);
	BOOL Create(CSize size, WORD nBitCount, const void *lpBits=NULL);
	BOOL CreateCompatible(int width, int height, CDC *pDC=NULL, const void *lpBits=NULL);

	BOOL CopyFromObject(CBmpProc& souBmp, CRect *pDesRect=NULL, CRect *pSouRect=NULL);
	BOOL CopyFromHbmp(HBITMAP souBmp, CRect *pDesRect=NULL, CRect *pSouRect=NULL);
	BOOL CopyTran(CBmpProc& souBmp, COLORREF crColour=CBPCOLOR_WHITE, 
			CRect *pDesRect=NULL, CRect *pSouRect=NULL, int mode=0);
	BOOL CopyTran(CBmpProc& souBmp, COLORREF crColour=CBPCOLOR_WHITE, 
			int x=0, int y=0, CRect *pSouRect=NULL, int mode=0);
	BOOL CopyTranCenter(CBmpProc& souBmp, CRect* crArea=NULL, 
			COLORREF crColour=CBPCOLOR_WHITE, int mode=0);
	BOOL CopyTranTile(CBmpProc& souBmp, CRect* crArea=NULL, 
			COLORREF crColour=CBPCOLOR_WHITE, int mode=0);

	BOOL LoadFromFile(LPCTSTR lpszPathName);
	BOOL LoadFromResourceID(UINT uID, HINSTANCE hInst);
	BOOL LoadFromResourceName(LPCTSTR lpResourceName, HINSTANCE hInst);
	BOOL LoadFromObject(CBmpProc& souBmp, CRect *pSouRect=NULL);
	BOOL LoadFromHbmp(HBITMAP sou, CRect *pSouRect=NULL);
	BOOL LoadFromClipboard();

	BOOL Save(LPCTSTR lpszNewFileName=NULL, WORD nBitsPerPixel=0);
	BOOL SaveToClipboard();

	BOOL Draw(CDC& dc, const CRect* rcDst=NULL, const CRect* rcSrc=NULL);
	BOOL DrawTile(CDC& dc, CRect* crArea);
	BOOL DrawCenter(CDC& dc, CRect* crArea);
	BOOL DrawTransparentInRect(CDC& dc, COLORREF crColour=CBPCOLOR_WHITE, 
			CRect* crDest=NULL, CRect* crSou=NULL, int mode=0);
	BOOL DrawTransparentInPoint(CDC& dc, COLORREF crColour=CBPCOLOR_WHITE, 
			int x=0, int y=0, CRect* crSou=NULL, int mode=0);
	BOOL DrawTranCenter(CDC& dc, CRect* crArea, COLORREF crColour=CBPCOLOR_WHITE, int mode=0);
	BOOL DrawTranTile(CDC& dc, CRect* crArea, COLORREF crColour=CBPCOLOR_WHITE, int mode=0);

	BOOL Attach(HBITMAP hBmp);
	HBITMAP Detach();
	void Clear();
	HRGN ExportRegion(COLORREF cTranColor=CBPCOLOR_WHITE);
	BOOL Fill(COLORREF crColour=CBPCOLOR_WHITE, CRect *crArea=NULL);
	BOOL ReSetColor(COLORREF oldcolr, COLORREF newcolr, CRect *crArea=NULL);
		   
	// ���ز���������
	CBmpProc& operator=(const CBmpProc& ds);
	CBmpProc& operator=(const HBITMAP sou);
	BOOL operator==(const CBmpProc& ds);
	BOOL operator!=(const CBmpProc& ds);
	operator HBITMAP() const { return (HBITMAP)m_hObject; };


	// �����ݷ��ʺ���
	CSize Size() const;
	CRect Rect() const;
	int   Width() const;
	int   Height() const;
	int   ScanWidth() const;
	int   BitsPerPixel() const;

	BOOL  IsValid() const;

	//�������ڲ����ݵĽ��溯��
	int   GetImageType() const;
	DWORD GetInAddInfo() const;
	const CString& GetFileName() const;
	BOOL  SetFileName(LPSTR pSou);
	int   GetObject(LPBITMAP lpBitmap) const;

	HANDLE GetHDib() const;
	WORD GetVersion() const;

#ifdef _DEBUG
	static DWORD GetObjectCount() {return m_ObjectCount;};
#endif

protected:
	// ���ֺ����������޹أ�
	LPSTR FindDIBBits(LPSTR lpbi) const;
	DWORD DIBWidth(LPBITMAPINFOHEADER lpbi) const;
	DWORD DIBHeight(LPBITMAPINFOHEADER lpbi) const;
	WORD PaletteSize(LPBITMAPINFOHEADER lpbi) const;
	WORD DIBNumColors(LPBITMAPINFOHEADER lpbi) const;
	int	GetBitsPerPixel(LPBITMAPINFOHEADER lpbi) const;

	LPVOID CreateMemoryBlockFromHandle(HANDLE handle, DWORD dwCount=0);
	LPVOID CreateMemoryBlockFromAddress(LPVOID pSou, DWORD dwCount=0);

	BOOL IsValidBmpFile(CFile& file);
	LPBITMAPINFO GetBmpFileInfo(CFile& file);
	LPSTR GetBmpFileBits(CFile& file);
	BOOL IsValidDibFormat(WORD nBitsPerPixel);
	BOOL IsFileExist(LPCTSTR lpFileName);

	DWORD CalculateDIBFileSize(LPBITMAPINFOHEADER lpbi) const;
	DWORD CalculateDIBSize(LPBITMAPINFOHEADER lpbi) const;
	DWORD CalculateDIBitsOff(LPBITMAPINFOHEADER lpbi) const;
	DWORD CalculateDIBInfoSize(LPBITMAPINFOHEADER lpbi) const;
	DWORD CalculateDIBitsSize(LPBITMAPINFOHEADER lpbi) const;
	
	BOOL PasteAvailable();


	// �ڲ��������������йأ�
	BOOL SaveBaseSpecifyFileName(LPCTSTR fn);
	BOOL SaveBaseSpecifyFormat(WORD nBitsPerPixel);
	LPBITMAPINFO GetSpecifyFormatInfo(WORD newBitCount=0);
	BOOL ClearAndSetData(int type, DWORD addinfo, LPCTSTR lpFn, LPBITMAPINFO pInfo, HBITMAP hBmp);

public:
	void RotatedBitmap(RateDirection radians, COLORREF clrBack);
	void RotatedBitmap(RateDirection radians);
	enum			// λͼ�ĳ�ʼ��Դ���ͣ�
	{
		IT_NULL,		// ��λͼ����
		IT_DISKFILE,	// �ļ�λͼ
		IT_RESOURCE,	// ��Դλͼ
		IT_MEMORY,		// �ڴ�λͼ
		IT_CLIP,		// ������λͼ
		IT_CREATE		// ������λͼ
	};

private:
	// ���ݳ�Ա
	BOOL m_mark;		// �����Ч��־����TRUE-��������λͼ��
						// FALSE-������û��λͼ��
	int  m_type;		// ͼ�����ͣ�ֻ��ʹ������ö������ֵ
	DWORD   m_addInfo;	// ������Ϣ��
	CString	m_cFileName;// �ļ�·����
	LPBITMAPINFO  m_pInfo;	// DIB ��Ϣ�ṹ

#ifdef _DEBUG
	static DWORD m_ObjectCount;	// ���๹���ʵ��Ŀ�������������ʹ��
#endif
};

#endif /*__BMPPROC_H__*/
