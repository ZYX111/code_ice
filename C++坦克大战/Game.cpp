// Game.cpp: implementation of the CGame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tank.h"
#include "Game.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGame::CGame()
{
	m_pMenuBuffer=NULL;
	m_bFreeze=FALSE;
	m_nBorderTime=0;
	m_nFreezeTime=0;
	m_nMenuPos=3;  //默认为返回游戏
	m_pDD=NULL;
	m_pddsFrontBuffer=NULL;
	m_pddsBackBuffer=NULL;
	pcClipper=NULL;

	m_pBackGroundBuffer=NULL;
	m_pBulletBuffer=NULL;
	m_pExplodeBuffer=NULL;
	m_pGeneralBuffer=NULL;
	m_pEggBuffer=NULL;
	m_pWaitBuffer=NULL;
	m_pGameOverBuffer=NULL;
	m_pBeginBuffer=NULL;


	m_pPlayerBuffer=NULL;
	m_pTankAppBuffer=NULL;
	m_pTankStateBuffer=NULL;
	m_pEnemyBuffer=NULL;
	m_pLevelBuffer=NULL;

	m_bSinglePlayer=FALSE;
	m_nEnemyCount=0;
	m_nLeftEnemy=0;	
	m_nGameLevel=0;

	m_nLeftPlayer=3;
	m_nRightPlayer=3;
	m_nLeftKills=0;
	m_nRightKills=0;
	m_nLeftScore=0;
	m_nRightScore=0;

	m_tankleft=m_tankright=NULL;

	m_bLeftBring=false;			//左边正在产生坦克
	m_bRightBring=false;
	m_hWnd=NULL;
	m_rcWnd.left=0;
	m_rcWnd.right=SCREEN_WIDTH;
	m_rcWnd.top=0;
	m_rcWnd.bottom=SCREEN_HEIGHT;
	m_nGameState=GAMEBEGIN;
	m_rcGeneral.SetRect(18 *TANKMAPCELL,37*TANKMAPCELL,22*TANKMAPCELL,40*TANKMAPCELL);

}

CGame::~CGame()
{
	m_TankMap.DestroyBmp();
	DestroyObject();
	DDDestroy();
}

BOOL CGame::DDinit(HWND hWnd)
{
	HRESULT hr;
	m_hWnd=hWnd;
	//create DirectDraw
	hr = DirectDrawCreate( NULL, &m_pDD, NULL );
	if( FAILED(hr) )
	{
//		AfxMessageBox( "Error Create DirectDraw\n" );
		return FALSE;
	}

	m_pDD->SetCooperativeLevel( m_hWnd, DDSCL_NORMAL );
	//create FrontBuffer
	DDSURFACEDESC	ddsd;
	ZeroMemory( &ddsd, sizeof(ddsd) );
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	hr = m_pDD->CreateSurface( &ddsd, &m_pddsFrontBuffer, NULL );
	if( FAILED(hr) )
	{
//		AfxMessageBox( "Error Create Front Buffer\n" );
		return FALSE;
	}
	//create BackBuffer
	m_pddsBackBuffer = DDCreateSurface( SCREEN_WIDTH, SCREEN_HEIGHT );
	if(!m_pddsBackBuffer)
	{
//		AfxMessageBox( "Error Create Back Buffer\n" );
		return FALSE;
	}
	//create Clipper
	if(FAILED(hr = m_pDD->CreateClipper(0, &pcClipper, NULL)))
	{
//		AfxMessageBox("Failed to create clipper.");
		return FALSE;		
	}

    if(FAILED(hr = pcClipper->SetHWnd(0, hWnd)))
    {
        pcClipper->Release();
//		AfxMessageBox("Failed to create primary surface.");
		return FALSE;		
    }

    if(FAILED(hr = m_pddsFrontBuffer->SetClipper(pcClipper)))
    {
        pcClipper->Release();
//		AfxMessageBox("Failed to create primary surface.");
		return FALSE;		
    }

	if( !LoadBitmaps() )
		return FALSE;
	return TRUE;
}

LPDIRECTDRAWSURFACE CGame::DDCreateSurface(int width, int height)
{
	DDSURFACEDESC	ddsd;
	ZeroMemory( &ddsd, sizeof(ddsd) );
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = width;
	ddsd.dwHeight = height;

	LPDIRECTDRAWSURFACE pdds;
	if( FAILED(m_pDD->CreateSurface( &ddsd, &pdds, NULL )) )
		return NULL;
	return pdds;
}

BOOL CGame::LoadBitmaps()
{
	
    m_pBeginBuffer = DDLoadBitmap("pic\\bmp\\Begin.bmp");
	if(!m_pBeginBuffer)
	{
//		AfxMessageBox("载入序幕图源失败");
		return FALSE;}
	//Egg
    m_pEggBuffer = DDLoadBitmap("pic\\bmp\\Egg.bmp");
	if(!m_pEggBuffer)
	{
//		AfxMessageBox("载入彩蛋图源失败");
		return FALSE;}
	DDSetColorKey( m_pEggBuffer, 0);

    m_pExplodeBuffer = DDLoadBitmap("pic\\bmp\\explode.bmp");
	if(!m_pExplodeBuffer)
	{
//		AfxMessageBox("载入爆炸效果图失败");
		return FALSE;}
	DDSetColorKey( m_pExplodeBuffer, RGB(0,0,255) );

    m_pGeneralBuffer = DDLoadBitmap("pic\\bmp\\General.bmp");
	if(!m_pGeneralBuffer)
	{
//		AfxMessageBox("载入大本营图失败");
		return FALSE;}
    DDSetColorKey( m_pGeneralBuffer, 0 );  

	m_pWaitBuffer = DDLoadBitmap("pic\\bmp\\Wait.bmp");
	if(!m_pWaitBuffer)
	{
//		AfxMessageBox("载入等待图失败");
		return FALSE;}
	DDSetColorKey( m_pWaitBuffer, 0 );

	m_pGameOverBuffer = DDLoadBitmap("pic\\bmp\\over.bmp");
	if(!m_pGameOverBuffer)
	{
//		AfxMessageBox("载入结束图失败");
		return FALSE;}
	DDSetColorKey( m_pGameOverBuffer, 0 );  

	m_pBulletBuffer = DDLoadBitmap("pic\\bmp\\Bullet.bmp");
	if(!m_pBulletBuffer)
	{
//		AfxMessageBox("载入子弹图失败");
		return FALSE;}
	DDSetColorKey( m_pBulletBuffer, 0 );
	//Map Bitmap
	LPDIRECTDRAWSURFACE pDDsTemp=NULL;
	pDDsTemp = DDLoadBitmap(IDB_SOIL);
	if(!pDDsTemp)
	{
//		AfxMessageBox("载入泥图源失败");
		return FALSE;}
	m_TankMap.SetSoilBmp(pDDsTemp);

	pDDsTemp = DDLoadBitmap(IDB_STONE);
	if(!pDDsTemp)
	{
//		AfxMessageBox("载入石图源失败");
		return FALSE;}
	m_TankMap.SetStoneBmp(pDDsTemp);

	pDDsTemp = DDLoadBitmap(IDB_STEEL);
	if(!pDDsTemp)
	{
//		AfxMessageBox("载入钢图源失败");
		return FALSE;}
	m_TankMap.SetSteelBmp(pDDsTemp);

	pDDsTemp = DDLoadBitmap(IDB_WATER);
	if(!pDDsTemp)
	{
//		AfxMessageBox("载入水图源失败");
		return FALSE;}
	m_TankMap.SetWaterBmp(pDDsTemp);

	pDDsTemp = DDLoadBitmap(IDB_ICE);
	if(!pDDsTemp)
	{
//		AfxMessageBox("载入冰图源失败");
		return FALSE;}
	DDSetColorKey( pDDsTemp, 0 );
	m_TankMap.SetIceBmp(pDDsTemp);

	pDDsTemp = DDLoadBitmap(IDB_TREE);
	if(!pDDsTemp)
	{
//		AfxMessageBox("载入树图源失败");
		return FALSE;}
	DDSetColorKey( pDDsTemp, 0 );
	m_TankMap.SetTreeBmp(pDDsTemp);
	//Tank Bitmap
	m_pPlayerBuffer = DDLoadBitmap("pic\\bmp\\TankPlayer.bmp");
	if(!m_pPlayerBuffer)
	{
//		AfxMessageBox("载入坦克图源失败");
		return FALSE;}
	DDSetColorKey( m_pPlayerBuffer, RGB(20,20,140) );

	m_pTankAppBuffer = DDLoadBitmap("pic\\bmp\\TankApp.bmp");
	if(!m_pTankAppBuffer)
	{
//		AfxMessageBox("载入坦克出现图源失败");
		return FALSE;}
	DDSetColorKey( m_pTankAppBuffer, 0 );

	m_pTankStateBuffer = DDLoadBitmap("pic\\bmp\\Tankstate.bmp");
	if(!m_pTankStateBuffer)
	{
//		AfxMessageBox("载入坦克状态图源失败");
		return FALSE;}

	m_pEnemyBuffer = DDLoadBitmap("pic\\bmp\\TankEnemy.bmp");
	if(!m_pEnemyBuffer)
	{
//		AfxMessageBox("载入敌方坦克图源失败");
		return FALSE;}
	DDSetColorKey( m_pEnemyBuffer, RGB(20,20,140) );
	return TRUE;
}

LPDIRECTDRAWSURFACE CGame::DDLoadBitmap(LPCSTR szBitmap, int dx, int dy)
{
    HBITMAP                 hbm;
    BITMAP                  bm;
    DDSURFACEDESC			ddsd;
    LPDIRECTDRAWSURFACE		pdds;

    //
    //  Try to load the bitmap as a resource, if that fails, try it as a file
    //
    hbm = (HBITMAP) LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, dx,
                              dy, LR_CREATEDIBSECTION);
    if (hbm == NULL)
        hbm = (HBITMAP) LoadImage(NULL, szBitmap, IMAGE_BITMAP, dx, dy,
                                  LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    if (hbm == NULL)
        return NULL;
    //
    // Get size of the bitmap
    //
    GetObject(hbm, sizeof(bm), &bm);
    //
    // Create a DirectDrawSurface for this bitmap
    //
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN ;
    ddsd.dwWidth = bm.bmWidth;
    ddsd.dwHeight = bm.bmHeight;
    if (m_pDD->CreateSurface(&ddsd, &pdds, NULL) != DD_OK)
	{
		DeleteObject(hbm);
        return NULL;
	}
    DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);
    DeleteObject(hbm);
    return pdds;
}

LPDIRECTDRAWSURFACE CGame::DDLoadBitmap(int id, int dx, int dy)
{
	HBITMAP                 hbm;
    BITMAP                  bm;
    DDSURFACEDESC			ddsd;
    LPDIRECTDRAWSURFACE		pdds;
    //
    //  Try to load the bitmap as a resource, if that fails, try it as a file
    //
	CBitmap bmp;
	if(bmp.LoadBitmap(id)==FALSE)
		return NULL;
	hbm=HBITMAP(bmp.GetSafeHandle());

    if (hbm == NULL)
        return NULL;
    //
    // Get size of the bitmap
    //
    GetObject(hbm, sizeof(bm), &bm);
    //
    // Create a DirectDrawSurface for this bitmap
    //
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth = bm.bmWidth;
    ddsd.dwHeight = bm.bmHeight;
    if (m_pDD->CreateSurface(&ddsd, &pdds, NULL) != DD_OK)
        return NULL;
    DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);
    DeleteObject(hbm);///???
    return pdds;
}

HRESULT CGame::DDCopyBitmap(LPDIRECTDRAWSURFACE pdds, HBITMAP hbm, int x, int y, int dx, int dy)
{
	    HDC                     hdcImage;
    HDC                     hdc;
    BITMAP                  bm;
    DDSURFACEDESC			ddsd;
    HRESULT                 hr;

    if (hbm == NULL || pdds == NULL)
        return E_FAIL;
    //
    // Make sure this surface is restored.
    //
    pdds->Restore();
    //
    // Select bitmap into a memoryDC so we can use it.
    //
    hdcImage = CreateCompatibleDC(NULL);
    if (!hdcImage)
        TRACE("createcompatible dc failed\n");
    SelectObject(hdcImage, hbm);
    //
    // Get size of the bitmap
    //
    GetObject(hbm, sizeof(bm), &bm);
    dx = dx == 0 ? bm.bmWidth : dx;     // Use the passed size, unless zero
    dy = dy == 0 ? bm.bmHeight : dy;
    //
    // Get size of surface.
    //
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    pdds->GetSurfaceDesc(&ddsd);

    if ((hr = pdds->GetDC(&hdc)) == DD_OK)
    {
        StretchBlt(hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, x, y,
                   dx, dy, SRCCOPY);
        pdds->ReleaseDC(hdc);
    }
    DeleteDC(hdcImage);
    return hr;

}

void CGame::DDDestroy()
{
	if(m_pDD)
	{
		if(m_pddsFrontBuffer)
		{
			m_pddsFrontBuffer->Release();
			m_pddsFrontBuffer=NULL;
			if(m_pddsBackBuffer)
			{
				m_pddsBackBuffer->Release();
				m_pddsBackBuffer=NULL;
			}
			if(pcClipper)
			{
				pcClipper->Release();
				pcClipper=NULL;
			}
			if(m_pBackGroundBuffer)
			{
				m_pBackGroundBuffer->Release();
				m_pBackGroundBuffer=NULL;
			}
			if(m_pMenuBuffer)
			{
				m_pMenuBuffer->Release();
				m_pMenuBuffer=NULL;
			}
			if(m_pBulletBuffer)
			{
				m_pBulletBuffer->Release();
				m_pBulletBuffer=NULL;
			}
			if(m_pExplodeBuffer)
			{
				m_pExplodeBuffer->Release();
				m_pExplodeBuffer=NULL;
			}
			if(m_pGeneralBuffer)
			{
				m_pGeneralBuffer->Release();
				m_pGeneralBuffer=NULL;
			}
			if(m_pPlayerBuffer)
			{
				m_pPlayerBuffer->Release();
				m_pPlayerBuffer=NULL;
			}
			if(m_pTankAppBuffer)
			{
				m_pTankAppBuffer->Release();
				m_pTankAppBuffer=NULL;
			}
			
			if(m_pTankStateBuffer)
			{
				m_pTankStateBuffer->Release();
				m_pTankStateBuffer=NULL;
			}
			if(m_pEnemyBuffer)
			{
				m_pEnemyBuffer->Release();
				m_pEnemyBuffer=NULL;
			}
			if(m_pWaitBuffer)
			{
				m_pWaitBuffer->Release();
				m_pWaitBuffer=NULL;
			}
			if(m_pGameOverBuffer)
			{
				m_pGameOverBuffer->Release();
				m_pGameOverBuffer=NULL;
			}
			if(m_pBeginBuffer)
			{
				m_pBeginBuffer->Release();
				m_pBeginBuffer=NULL;
			}
			if(m_pLevelBuffer)
			{
				m_pLevelBuffer->Release();
				m_pLevelBuffer=NULL;
			}
		}
		m_pDD->Release();
		m_pDD=NULL;
	}
}

HRESULT CGame::DDSetColorKey(LPDIRECTDRAWSURFACE pdds, COLORREF rgb)
{
	DDCOLORKEY              ddck;
    ddck.dwColorSpaceLowValue = DDColorMatch(pdds, rgb);
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
    return pdds->SetColorKey(DDCKEY_SRCBLT, &ddck);
}

DWORD CGame::DDColorMatch(LPDIRECTDRAWSURFACE pdds, COLORREF rgb)
{
	COLORREF                rgbT;
    HDC                     hdc;
    DWORD                   dw = CLR_INVALID;
    DDSURFACEDESC			ddsd;
    HRESULT                 hres;

    //
    //  Use GDI SetPixel to color match for us
    //
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
        rgbT = GetPixel(hdc, 0, 0);     // Save current pixel value
        SetPixel(hdc, 0, 0, rgb);       // Set our value
        pdds->ReleaseDC(hdc);
    }
    //
    // Now lock the surface so we can read back the converted color
    //
    ddsd.dwSize = sizeof(ddsd);
    while ((hres = pdds->Lock(NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING)
        ;
    if (hres == DD_OK)
    {
        dw = *(DWORD *) ddsd.lpSurface;                 // Get DWORD
        if (ddsd.ddpfPixelFormat.dwRGBBitCount < 32)
            dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount) - 1;  // Mask it to bpp
        pdds->Unlock(NULL);
    }
    //
    //  Now put the color that was there back.
    //
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
        SetPixel(hdc, 0, 0, rgbT);
        pdds->ReleaseDC(hdc);
    }
    return dw;
}

BOOL CGame::GameInit()
{
	//*init sound 
	if(!m_wavBang.Create(IDR_BANG))
		return FALSE;
	if(!m_wavDie.Create(IDR_DIE))
		return FALSE;
	if(!m_wavShoot.Create(IDR_SHOOT))
		return FALSE;
	if(!m_wavEgg.Create(IDR_EGG))
		return FALSE;
	if(!m_midiBKMusic.Create(IDR_MIDIBKMUSIC))
		return FALSE;
	return TRUE;
}

void CGame::SetWndRect(RECT &rc)
{
	m_rcWnd=rc;
}

void CGame::FlipScreen()
{
	HRESULT hr;
	hr = m_pddsFrontBuffer->Blt( &m_rcWnd, m_pddsBackBuffer,NULL, DDBLT_WAIT, NULL );

	if( hr == DDERR_SURFACELOST )
		ResetSurface();
}

void CGame::ResetSurface()
{
	m_pddsBackBuffer->Restore();
	m_pddsFrontBuffer->Restore();
	m_TankMap.ResetSurface();

	DDReLoadBitmap( m_TankMap.GetSoilBmp(),IDB_SOIL);
	DDReLoadBitmap( m_TankMap.GetStoneBmp(),IDB_STONE);
	DDReLoadBitmap( m_TankMap.GetSteelBmp(),IDB_STEEL);
	DDReLoadBitmap( m_TankMap.GetWaterBmp(),IDB_WATER);
	DDReLoadBitmap( m_TankMap.GetIceBmp(),IDB_ICE);
	DDReLoadBitmap( m_TankMap.GetTreeBmp(),IDB_TREE);


}

HRESULT CGame::DDReLoadBitmap(LPDIRECTDRAWSURFACE pdds, LPCSTR szBitmap)
{
    HBITMAP                 hbm;
    HRESULT                 hr;

    hbm = (HBITMAP) LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, 0,
                              0, LR_CREATEDIBSECTION);
    if (hbm == NULL)
        hbm = (HBITMAP) LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0,
                                  LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    if (hbm == NULL)
    {
        OutputDebugString("handle is null\n");
        return E_FAIL;
    }
    hr = DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);
    if (hr != DD_OK)
    {
        OutputDebugString("ddcopybitmap failed\n");
    }
    DeleteObject(hbm);
    return hr;

}

HRESULT CGame::DDReLoadBitmap(LPDIRECTDRAWSURFACE pdds, int id)
{
   HBITMAP                 hbm;
    HRESULT                 hr;

  	CBitmap bmp;
	if(bmp.LoadBitmap(id)==FALSE)
		return NULL;
	hbm=HBITMAP(bmp.GetSafeHandle());
    if (hbm == NULL)
    {
        OutputDebugString("handle is null\n");
        return E_FAIL;
    }
    hr = DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);
    if (hr != DD_OK)
    {
        OutputDebugString("ddcopybitmap failed\n");
    }
    DeleteObject(hbm);
    return hr;

}

void CGame::DDClear(RECT *prc, DWORD dwFillColor)
{
	DDBLTFX	ddbfx;
	ZeroMemory( &ddbfx, sizeof(ddbfx) );
	ddbfx.dwSize = sizeof(ddbfx);
	ddbfx.dwFillColor = dwFillColor;
	
	m_pddsBackBuffer->Blt( prc, NULL, NULL, 
		DDBLT_COLORFILL | DDBLT_WAIT, &ddbfx );
}

void CGame::DrawWait(int n)
{
	if(n>3)return;

	RECT rc;
	rc.left=0;
	rc.right=SCREEN_WIDTH;
	rc.top=0;
	rc.bottom=SCREEN_HEIGHT;	
	DDClear(&rc,RGB(0,0,0));
//取消背景图
//	m_pddsBackBuffer->BltFast(0,0, m_pBackGroundBuffer, &rc, DDBLTFAST_WAIT );
    
	//draw general
	rc.left=0;
	rc.right=m_rcGeneral.Width();
	rc.top=0;
	rc.bottom=m_rcGeneral.Height();
	m_pddsBackBuffer->BltFast(m_rcGeneral.left,m_rcGeneral.top,m_pGeneralBuffer,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);

	m_TankMap.DrawMap(m_pddsBackBuffer); 
	m_TankMap.DrawShield(m_pddsBackBuffer);   //对于可以遮蔽的物体
	
	rc.left=720-n*240;
	rc.right=rc.left+240;
	rc.top=0;
	rc.bottom=240;	

	m_pddsBackBuffer->BltFast(200,200, m_pWaitBuffer, &rc, DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY );

	FlipScreen();
}

void CGame::DrawGameOver()
{
	RECT rc;
	rc.left=0;
	rc.right=424;
	rc.top=0;
	rc.bottom=317;	
	m_pddsBackBuffer->BltFast(120,100, m_pGameOverBuffer, &rc,DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );
	FlipScreen();
}

void CGame::DrawWorldMap()
{
	CBullet * p;
	RECT rc;
	rc.left=0;
	rc.right=SCREEN_WIDTH;
	rc.top=0;
	rc.bottom=SCREEN_HEIGHT;	

//以下是背景图片（为了节约空间，略去）
//*	m_pddsBackBuffer->BltFast(0,0, m_pBackGroundBuffer, &rc, DDBLTFAST_WAIT );
    //将背景改为黑屏
	DDClear(&rc,RGB(0,0,0));
	//draw general
	rc.left=0;
	rc.right=m_rcGeneral.Width();
	rc.top=0;
	rc.bottom=m_rcGeneral.Height();
	m_pddsBackBuffer->BltFast(m_rcGeneral.left,m_rcGeneral.top,m_pGeneralBuffer,&rc,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
//此处判断一下是否老盖是否被改变	
	if(m_nBorderTime==1)
	{
		CellType temp;
		temp.cell=CT_SOIL;
		temp.mask=0;
		temp.defence=100;
		m_TankMap.SetHeadQuarterBorder(temp);
	}
	if(m_nBorderTime>0)m_nBorderTime--;
	m_TankMap.DrawMap(m_pddsBackBuffer); 
	CEgg * egg;
	POSITION prepos;
	POSITION position=m_Egg.GetHeadPosition();
	while(position)
	{
	   prepos=position;
	   egg=m_Egg.GetNext(position);
	   if(egg->m_nLife-- <= 0 )
	   {
		   delete egg;
		   m_Egg.RemoveAt(prepos);
		   break;
	   }
	   egg->DrawEgg(m_pddsBackBuffer);
	}
	CTank *tp=NULL;
	position=m_Player.GetHeadPosition();
	while(position)
	{
	   tp=m_Player.GetNext(position);
	   tp->DrawTank(m_pddsBackBuffer);
	}

	position=m_Enemy.GetHeadPosition();
	if(!position && m_nLeftEnemy==0)
	{
		if(m_nGameLevel > GAMETOTALLEVEL)
			m_nGameState=GAMEWIN;
		else
			m_nGameState=GAMENEXT;
		return;
	}
	while(position)
	{
	   tp=m_Enemy.GetNext(position);
	   tp->DrawTank(m_pddsBackBuffer);
	}

	position=m_BulletList.GetHeadPosition();
	while(position)
	{
	   p=m_BulletList.GetNext(position);
	   p->DrawBullet(m_pddsBackBuffer);
	}

	CExplode *te;
	position=m_Explode.GetHeadPosition();
	while(position)
	{
	   te=m_Explode.GetNext(position);
	   te->DrawExplode(m_pddsBackBuffer);
	}

	m_TankMap.DrawShield(m_pddsBackBuffer);
	FlipScreen();
}

void CGame::OutputText(int x, int y, LPCTSTR string,COLORREF rgb,CFont *font)
{
	HDC hdc;

	if( m_pddsBackBuffer && 
		m_pddsBackBuffer->GetDC(&hdc) == DD_OK )
	{
		HGDIOBJ  oldFont=SelectObject(hdc,font->GetSafeHandle());
		SetBkMode( hdc, TRANSPARENT );
		SetTextColor( hdc, rgb);
		TextOut( hdc, x, y, string, lstrlen(string) );
		SelectObject(hdc,oldFont);
		m_pddsBackBuffer->ReleaseDC( hdc );
	}
}

void CGame::Run()
{
	if(m_nGameState==GAMEACTIVE)
	{
		POSITION position,pre=NULL;
		position=m_BulletList.GetHeadPosition();
		while(position)
		{
			pre=position;
			CBullet * p=m_BulletList.GetNext(position);
			BOOL ret=p->BulletTrack();
			if(!ret && pre)
			{
				delete p;
				m_BulletList.RemoveAt(pre);
				break;
			}
		}
		int nEnemy=0;
		if(m_nFreezeTime<=0)
		{
			position=m_Enemy.GetHeadPosition();
			while(position)
			{
				CTank * p=m_Enemy.GetNext(position);
				p->TankMove();
				nEnemy++;
			}

			if(nEnemy < 4 && m_nLeftEnemy > 0)
			{
				for(int i=0;i<3;i++)
					BringTank(i+1);
			}
		}
		else m_nFreezeTime--;
		position=m_Explode.GetHeadPosition();
		while(position)
		{
			pre=position;
			CExplode * p=m_Explode.GetNext(position);
			BOOL ret=p->Burn();
			if(!ret && pre)
			{

				if(p->GetSort()==1) //1: 产生坦克时的闪动效果
				{
					int side=p->GetSide();
					if(side==1)
					{
						m_tankleft=new CTank;
						if(m_tankleft)
						{
							m_bLeftBring=false;
							m_tankleft->Init(this,15,38,m_pPlayerBuffer,m_pTankStateBuffer,0);
							m_Player.AddHead(m_tankleft);
							m_nLeftPlayer--;
						}	
					}
					else if(side==2)
					{
						m_tankright=new CTank;
						if(m_tankright)
						{
							m_bRightBring=false;
							m_tankright->Init(this,23,38,m_pPlayerBuffer,m_pTankStateBuffer,1);
							m_Player.AddTail(m_tankright);
							m_nRightPlayer--;
						}

					}
					else
					{

					}
				}
				delete p;
				m_Explode.RemoveAt(pre);
				break;
			}
		}
		if(m_tankleft==NULL && !m_bLeftBring && m_nLeftPlayer>0)
		{
				CExplode *p=new CExplode;
				if(p)
				{
					p->Init(15*TANKMAPCELL,38*TANKMAPCELL,5,8,m_pTankAppBuffer,32,32,1,1);
					m_Explode.AddTail(p);
					m_bLeftBring=true;
				}
		}
		if(!m_bSinglePlayer && m_tankright==NULL && !m_bRightBring && m_nRightPlayer>0)
		{
				CExplode *p=new CExplode;
				if(p)
				{
					p->Init(23*TANKMAPCELL,38*TANKMAPCELL,5,8,m_pTankAppBuffer,32,32,1,2);
					m_Explode.AddTail(p);
					m_bRightBring=true;
				}
		}
		if(!m_tankleft && !m_tankright && m_nLeftPlayer==0 && m_nRightPlayer==0 && !m_bLeftBring && !m_bRightBring)
			m_nGameState=GAMEOVER;
	}
}

void CGame::PlaySound(int sort)
{
	switch(sort)
	{
	case 0:
		m_wavBang.Play();
		break;
	case 1:
		m_wavDie.Play();
		break;
	case 2:
		m_wavShoot.Play();
	case 3:
		m_wavEgg.Play();
	}
}

void CGame::UpdateScreen()
{
	switch(m_nGameState)
	{
	case GAMEBEGIN:
		DrawBegin();
		break;
	case GAMEMENU:
		DrawSelectMenu();
		break;
	case GAMEACTIVE:
		DrawWorldMap();
		break;
	case GAMEWIN:
		DrawWin();
		break;
	case GAMEPAUSE:
		DrawPause();
		break;
	case GAMEOVER:
		DrawGameOver();
		break;
	}
}


BOOL CGame::Init(HWND hWnd)
{
	if(!DDinit(hWnd))
		return FALSE;
	if(!GameInit())
		return FALSE;
	return TRUE;	
}

void CGame::DestroyObject()
{
	POSITION  position;
	position=m_Explode.GetHeadPosition();
	while(position)
	{
	   CExplode *p=m_Explode.GetNext(position);
	   delete p;
	}
	m_Explode.RemoveAll();

	position=m_BulletList.GetHeadPosition();
	while(position)
	{
	   CBullet *p=m_BulletList.GetNext(position);
	   delete p;
	}
	m_BulletList.RemoveAll();

	position=m_Player.GetHeadPosition();
	while(position)
	{
		CTank *p=m_Player.GetNext(position);
		delete p;
	}
	m_Player.RemoveAll();
	m_tankleft=m_tankright=NULL;

	position=m_Enemy.GetHeadPosition();
	while(position)
	{
		CTank *p=m_Enemy.GetNext(position);
		delete p;
	}
	m_Enemy.RemoveAll();

	position=m_Egg.GetHeadPosition();
	while(position)
	{
		CEgg *p=m_Egg.GetNext(position);
		delete p;
	}
	m_Egg.RemoveAll();
	m_midiBKMusic.Stop();

}

BOOL CGame::StartGame(int level)
{
	if(level > GAMETOTALLEVEL)return FALSE;
	m_nGameLevel=level;
	m_nBorderTime=0;
	m_nFreezeTime=0;
	m_bFreeze=FALSE;

	if(m_bSinglePlayer)
		m_nEnemyCount=10;
	else
		m_nEnemyCount=20;
	m_nEnemyCount+=(level-1)*3;
	m_nLeftEnemy=m_nEnemyCount;
	
	m_bLeftBring=false;			//左边正在产生坦克
	m_bRightBring=false;

	m_nLeftKills=0;
	m_nRightKills=0;
//销毁除player以外的东西
	DestroyObjectForNewLevel();
//保存player的属性,但要调整坦克位置
   	if(m_tankleft!=NULL && m_nLeftPlayer>=0)
	{
			m_tankleft->SetTankPos(15,38);
            m_tankleft->SetDirection(UP);
			m_tankleft->SetTankLife(200);
			m_tankleft->SetRect();
	}
	if(!m_bSinglePlayer && m_tankright!=NULL && m_nRightPlayer>=0)
	{
			m_tankright->SetTankPos(23,38);
            m_tankright->SetDirection(UP);
			m_tankright->SetTankLife(200);
			m_tankright->SetRect();
	}       
	m_nGameState=GAMEACTIVE;
	m_midiBKMusic.Play(TRUE);
	m_midiBKMusic.SetVolume(3);
	return TRUE;
}

void CGame::BringTank(int pos)
{
	if(m_nLeftEnemy <= 0)return;
	CTank * pTank;
	int x,y;
	CRect rct;
	switch(pos)
	{
	case 1://在左边产生一辆坦克
		x=-2;y=0;
		do{
			x+=2;
			rct.SetRect(x*TANKMAPCELL,y*TANKMAPCELL,x*TANKMAPCELL+TANKWIDTH,y*TANKMAPCELL+TANKHEIGHT);
		}while(TankInRect(rct) && x<10);
		if(x==10)break;
		pTank=new CTank;
		if(pTank)
		{
			pTank->Init(this,x,y,m_pEnemyBuffer,m_pTankStateBuffer,rand()%9);
			pTank->SetDirection(DOWN);
			m_Enemy.AddTail(pTank);
			m_nLeftEnemy--;
		}
		break;
	case 2://在中间产生一辆坦克
		x=17;y=0;
		do{
			x+=2;
			rct.SetRect(x*TANKMAPCELL,y*TANKMAPCELL,x*TANKMAPCELL+TANKWIDTH,y*TANKMAPCELL+TANKHEIGHT);
		}while(TankInRect(rct) && x<28);
		if(x==28)break;
		pTank=new CTank;
		if(pTank)
		{
			pTank->Init(this,x,y,m_pEnemyBuffer,m_pTankStateBuffer,rand()%9);
			pTank->SetDirection(DOWN);
			m_Enemy.AddTail(pTank);
			m_nLeftEnemy--;
		}
		break;
	case 3://在右边产生一辆坦克
		x=40;y=0;
		do{
			x-=2;
			rct.SetRect(x*TANKMAPCELL,y*TANKMAPCELL,x*TANKMAPCELL+TANKWIDTH,y*TANKMAPCELL+TANKHEIGHT);
		}while(TankInRect(rct) && x>20);
		if(x==20)break;
		pTank=new CTank;
		if(pTank)
		{
			pTank->Init(this,x,y,m_pEnemyBuffer,m_pTankStateBuffer,rand()%9);
			pTank->SetDirection(DOWN);
			m_Enemy.AddTail(pTank);
			m_nLeftEnemy--;
		}
		break;
	}
}

BOOL CGame::TankInRect(CRect &rc)
{
	CTank *p;
	CRect rect;
	POSITION position=m_Player.GetHeadPosition();
	while(position)
	{
		p=m_Player.GetNext(position);
		BOOL ret=rect.IntersectRect(&rc,&(p->rect));
		if(ret)
			return TRUE;
	}
	position=m_Enemy.GetHeadPosition();
	while(position)
	{
		p=m_Enemy.GetNext(position);
		BOOL ret=rect.IntersectRect(&rc,&(p->rect));
		if(ret)
			return TRUE;
	}
	return FALSE;
}

void CGame::DestroyObjectForNewLevel()
{
	POSITION  position;
	position=m_Explode.GetHeadPosition();
	while(position)
	{
	   CExplode *p=m_Explode.GetNext(position);
	   delete p;
	}
	m_Explode.RemoveAll();

	position=m_BulletList.GetHeadPosition();
	while(position)
	{
	   CBullet *p=m_BulletList.GetNext(position);
	   delete p;
	}
	m_BulletList.RemoveAll();

	position=m_Enemy.GetHeadPosition();
	while(position)
	{
		CTank *p=m_Enemy.GetNext(position);
		delete p;
	}
	m_Enemy.RemoveAll();

	position=m_Egg.GetHeadPosition();
	while(position)
	{
		CEgg *p=m_Egg.GetNext(position);
		delete p;
	}
	m_Egg.RemoveAll();
	m_midiBKMusic.Stop();
}

void CGame::DrawBegin()
{
	static int xx=75;
	static float yy=390;
	static CString str[4];
	str[0].LoadString(IDS_STRING001);
	str[1].LoadString(IDS_STRING002);
	str[2].LoadString(IDS_STRING003);
	str[3].LoadString(IDS_STRING004);
	RECT rc;
	rc.left=0;
	rc.right=SCREEN_WIDTH;
	rc.top=0;
	rc.bottom=SCREEN_HEIGHT;	
	DDClear(&rc,RGB(0,0,0));
	rc.right=300;
	rc.bottom=194;	
	m_pddsBackBuffer->BltFast(150,200, m_pBeginBuffer, &rc, DDBLTFAST_WAIT );

	CFont font;
	font.CreateFont(20,10, 0, 0, FW_BOLD , 0, 0, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, FF_ROMAN, "楷体");

	for(int i=0;i<4;i++)
	{
		yy-=0.07;
		if(int(yy)<100)
		{
			yy=390;
			m_nGameState=GAMENEXT;
			if(m_pBeginBuffer)
			{
				m_pBeginBuffer->Release();
				m_pBeginBuffer=NULL;
			}
		}
		OutputText(xx,int(yy)+i*25,str[i],RGB(255,0,0),&font);
	}
	font.DeleteObject();
	rc.left=0;
	rc.right=SCREEN_WIDTH;
	rc.top=0;
	rc.bottom=200;	
	DDClear(&rc,RGB(0,0,0));
	rc.left=0;
	rc.right=SCREEN_WIDTH;
	rc.top=394;
	rc.bottom=SCREEN_HEIGHT;	
	DDClear(&rc,RGB(0,0,0));


	LPDIRECTDRAWSURFACE tempbuffer=NULL;
	tempbuffer=DDLoadBitmap("pic\\bmp\\select.bmp");

	rc.left=0;
	rc.right=rc.left+106;
	rc.top=0;
	rc.bottom=rc.top+48;
	if(tempbuffer)
		m_pddsBackBuffer->BltFast(120,500, tempbuffer, &rc, DDBLTFAST_WAIT );

	rc.left=0;
	rc.right=rc.left+106;
	rc.top=48;
	rc.bottom=rc.top+48;
	if(tempbuffer)
		m_pddsBackBuffer->BltFast(400,500, tempbuffer, &rc, DDBLTFAST_WAIT );
	if(tempbuffer)
	{
		tempbuffer->Release();
		tempbuffer=NULL;
	}	
	rc.left=96;
	rc.top=0;
	rc.right=128;
	rc.bottom=32;
	if(m_bSinglePlayer)
		m_pddsBackBuffer->BltFast( 80, 510, m_pPlayerBuffer, &rc, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );
	else
		m_pddsBackBuffer->BltFast( 360, 510, m_pPlayerBuffer, &rc, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );
    FlipScreen();
}

void CGame::DrawLevel(int level)
{
		if(level>GAMETOTALLEVEL)return;
		RECT rc;
		rc.left=0;
		rc.right=SCREEN_WIDTH;
		rc.top=0;
		rc.bottom=SCREEN_HEIGHT;	
		DDClear(&rc,RGB(0,0,0));
		m_pLevelBuffer=DDLoadBitmap("pic\\bmp\\level.bmp");
		if(!m_pLevelBuffer){AfxMessageBox("载入游戏关图失败");return;}
		DDSetColorKey(m_pLevelBuffer, RGB(0,0,255) );
		if(m_pLevelBuffer)
		{
			rc.left=0;
			rc.top=(level-1)*100;
			rc.right=rc.left+640;
			rc.bottom=rc.top+100;
			m_pddsBackBuffer->BltFast(0,250, m_pLevelBuffer, &rc, DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY );
			FlipScreen();
			m_pLevelBuffer->Release();
			m_pLevelBuffer=NULL;
		}
}

void CGame::DrawWin()
{
	CFont font;
	font.CreateFont(30,12, 0, 0, FW_BOLD , 0, 0, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, FF_ROMAN, "楷体");

	CString strTemp("你已经经过敌人的重重炮火，到达了胜利的彼岸!");
	OutputText(80,200,strTemp,RGB(255,0,255),&font);
	font.DeleteObject();

	font.CreateFont(50,30, 0, 0, FW_BOLD , 0, 0, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, FF_ROMAN, "楷体");
	strTemp="祝:";
	OutputText(300,250,strTemp,RGB(255,0,255),&font);
	font.DeleteObject();

	font.CreateFont(40,30, 0, 0, FW_BOLD , 0, 0, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, FF_ROMAN, "楷体");
	strTemp="新年快乐, 万事如意!";
    OutputText(80,350,strTemp,RGB(255,0,255),&font);
	font.DeleteObject();
	FlipScreen();
	
}

void CGame::DrawPause()
{
	CFont font;
	font.CreateFont(20,10, 0, 0, FW_BOLD , 0, 0, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY, FF_ROMAN, "楷体");

	CString strTemp("暂 停");
	OutputText(590,10,strTemp,RGB(255,255,0),&font);
	FlipScreen();
	font.DeleteObject();
}

BOOL CGame::DrawSelectMenu()
{

	
	CRect rc;
	rc.left=0;
	rc.right=rc.left+351;
	rc.top=0;
	rc.bottom=rc.top+240;
	if(m_pMenuBuffer)
		m_pddsBackBuffer->BltFast(150,200, m_pMenuBuffer, &rc, DDBLTFAST_WAIT |DDBLTFAST_SRCCOLORKEY);
	rc.left=96;
	rc.top=0;
	rc.right=128;
	rc.bottom=32;
	m_pddsBackBuffer->BltFast( 155, 212+(m_nMenuPos-1)*60, m_pPlayerBuffer, &rc, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );
	FlipScreen();
   return FALSE;
}
