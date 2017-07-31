// Game.h: interface for the CGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAME_H__57350DA2_B866_449D_9135_643C2C305326__INCLUDED_)
#define AFX_GAME_H__57350DA2_B866_449D_9135_643C2C305326__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TankMap.h"
#include "Tank1.h"
#include "explode.h"
#include "directsound.h"
#include <afxtempl.h>
#include "Egg.h"

#include "Midi.h"

class CBullet;
//##ModelId=3EBA499B0330
class CGame  
{
public:
	//objects
	//##ModelId=3EBA499C010C
	CList<CBullet *,CBullet *> m_BulletList;
	//##ModelId=3EBA499C00FD
	CList<CTank *,CTank *>m_Player;
	//##ModelId=3EBA499C00EE
    CList<CTank *,CTank *>m_Enemy;
	//##ModelId=3EBA499C00E1
    CList<CExplode *,CExplode *>m_Explode;
	//##ModelId=3EBA499C00DA
	CList<CEgg *,CEgg *>m_Egg;

	//##ModelId=3EBA499C00BB
	CTank*	m_tankleft;
	//##ModelId=3EBA499C00B0
	CTank*	m_tankright;
	
	//surfaces
	//##ModelId=3EBA499C00AB
	LPDIRECTDRAWSURFACE     m_pEggBuffer;
	//##ModelId=3EBA499C00A1
	LPDIRECTDRAWSURFACE     m_pBulletBuffer;
	//##ModelId=3EBA499C009C
	LPDIRECTDRAWSURFACE     m_pExplodeBuffer;
	//##ModelId=3EBA499C0091
	LPDIRECTDRAWSURFACE     m_pTankAppBuffer;
	//##ModelId=3EBA499C008C
	LPDIRECTDRAWSURFACE     m_pPlayerBuffer;
	//##ModelId=3EBA499C0082
	LPDIRECTDRAWSURFACE     m_pTankStateBuffer;
	//##ModelId=3EBA499C007D
	LPDIRECTDRAWSURFACE     m_pEnemyBuffer;
	//##ModelId=3EBA499C0072
	LPDIRECTDRAWSURFACE     m_pBeginBuffer;
	//##ModelId=3EBA499C006D
	LPDIRECTDRAWSURFACE     m_pMenuBuffer;
	//##ModelId=3EBA499C0064
	LPDIRECTDRAWSURFACE     m_pLevelBuffer;//专用于报关画面
	//##ModelId=3EBA499C005F
	LPDIRECTDRAWSURFACE     m_pBackGroundBuffer;

	//Sounds
	//##ModelId=3EBA499C0055
	CDirectSound m_wavBang;
	//##ModelId=3EBA499C0050
	CDirectSound m_wavDie;
	//##ModelId=3EBA499C0045
	CDirectSound m_wavShoot;
	//##ModelId=3EBA499C0040
	CDirectSound m_wavEgg;
	//单双人标志变量
	//##ModelId=3EBA499C002F
	BOOL m_bSinglePlayer;
	//##ModelId=3EBA499C002E
	int m_nLeftPlayer;
	//##ModelId=3EBA499C0020
	int m_nRightPlayer;

	//##ModelId=3EBA499C001F
	int m_nLeftKills;
	//##ModelId=3EBA499C0010
	int m_nRightKills;
	//##ModelId=3EBA499C000F
	int m_nLeftScore;
	//##ModelId=3EBA499C0001
	int m_nRightScore;

	//##ModelId=3EBA499C0000
	int m_nEnemyCount;			//当前关敌人总数
	//##ModelId=3EBA499B03D9
	int m_nLeftEnemy;			//当前关没有出现的坦克数
	//##ModelId=3EBA499B03D8
	int m_nGameLevel;

	//游戏状态标志位
	//##ModelId=3EBA499B03C9
	int m_nMenuPos ;                  //1-4 分别对应四个选项

	//##ModelId=3EBA499B03C8
	int m_nBorderTime;                //指定改变老盖时间 
	//##ModelId=3EBA499B03BA
	int m_nFreezeTime;               //指定敌人坦克是否被定时以及定时时间
	//##ModelId=3EBA499B03B9
	BOOL m_bFreeze;                  //标定player是否被定时
	//##ModelId=3EBA499B03AC
	CMIDI	m_midiBKMusic;			//背景音乐
public:
	//##ModelId=3EBA499C03BB
	BOOL DrawSelectMenu();
	//##ModelId=3EBA499C03BA
	void DrawPause();
	//##ModelId=3EBA499C03B9
	void DrawWin();
	//##ModelId=3EBA499C03AB
	void DrawLevel(int level);
	//##ModelId=3EBA499B03A9
	bool m_bLeftBring;			//左边正在产生坦克
	//##ModelId=3EBA499B039E
	bool m_bRightBring;

	//##ModelId=3EBA499C03AA
	void DrawBegin();
	//##ModelId=3EBA499C03A9
	void DestroyObjectForNewLevel();
	//##ModelId=3EBA499C0399
	BOOL TankInRect(CRect& rc);
	//##ModelId=3EBA499C038A
	void BringTank(int pos=1);
	//##ModelId=3EBA499C036C
	BOOL StartGame(int level);
	//##ModelId=3EBA499C036B
	void DestroyObject();
	//##ModelId=3EBA499C035C
	BOOL Init(HWND hWnd);
	//##ModelId=3EBA499C035B
	void UpdateScreen();
	//##ModelId=3EBA499C034C
	void PlaySound(int sort=0);
	//##ModelId=3EBA499C034B
	void Run();

	//##ModelId=3EBA499C02FE
	void OutputText( int x, int y, LPCTSTR string,COLORREF rgb,CFont * font );
	//##ModelId=3EBA499C02FD
	void DrawWorldMap();
	//##ModelId=3EBA499C02DF
	void DrawWait(int n);
	//##ModelId=3EBA499C02DE
	void DrawGameOver();
	//##ModelId=3EBA499C02BF
	void DDClear( RECT* prc, DWORD dwFillColor );
	//##ModelId=3EBA499C0282
	HRESULT DDReLoadBitmap(LPDIRECTDRAWSURFACE pdds,int id);
	//##ModelId=3EBA499C029F
	HRESULT DDReLoadBitmap(LPDIRECTDRAWSURFACE pdds, LPCSTR szBitmap);
	//##ModelId=3EBA499C0281
	void ResetSurface();
	//##ModelId=3EBA499C0280
	void FlipScreen();
	//##ModelId=3EBA499C0271
	void SetWndRect(RECT &rc);
	//##ModelId=3EBA499C0261
	BOOL GameInit();

	//##ModelId=3EBA499C0242
	DWORD DDColorMatch(LPDIRECTDRAWSURFACE pdds, COLORREF rgb);
	//##ModelId=3EBA499C0223
	HRESULT DDSetColorKey(LPDIRECTDRAWSURFACE pdds, COLORREF rgb);
	//##ModelId=3EBA499C0222
	void DDDestroy();
	//##ModelId=3EBA499C01C5
	HRESULT DDCopyBitmap( LPDIRECTDRAWSURFACE pdds, HBITMAP hbm, int x, int y, int dx, int dy);
	//##ModelId=3EBA499C0158
	LPDIRECTDRAWSURFACE DDLoadBitmap(int id,int dx=0,int dy=0);
	//##ModelId=3EBA499C0196
	LPDIRECTDRAWSURFACE DDLoadBitmap( LPCSTR szBitmap, int dx=0, int dy=0);
	//##ModelId=3EBA499C0157
	BOOL LoadBitmaps();
	//##ModelId=3EBA499C0138
	LPDIRECTDRAWSURFACE DDCreateSurface( int width, int height );
	//##ModelId=3EBA499C0129
	BOOL DDinit(HWND hWnd);

	//##ModelId=3EBA499C0128
	GameState GetGameState()const{return m_nGameState;}
	//##ModelId=3EBA499C011B
	void SetGameState(GameState state){m_nGameState=state;}

	//##ModelId=3EBA499C011A
	CGame();
	//##ModelId=3EBA499C0119
	virtual ~CGame();

	//##ModelId=3EBA499B039B
	CTankMap m_TankMap;
	//##ModelId=3EBA499B0390
	HWND m_hWnd;
	//##ModelId=3EBA499B038F
	CRect m_rcGeneral;
private:
	//##ModelId=3EBA499B038C
	RECT m_rcWnd;
	//##ModelId=3EBA499B0386
	GameState m_nGameState;
	
	//##ModelId=3EBA499B0381
	LPDIRECTDRAW			m_pDD;
	//##ModelId=3EBA499B037C
	LPDIRECTDRAWSURFACE		m_pddsFrontBuffer;
	//##ModelId=3EBA499B0377
	LPDIRECTDRAWSURFACE		m_pddsBackBuffer;
	//##ModelId=3EBA499B0372
	LPDIRECTDRAWCLIPPER		pcClipper;

	//##ModelId=3EBA499B036D
	LPDIRECTDRAWSURFACE     m_pGeneralBuffer; 
	//##ModelId=3EBA499B0362
	LPDIRECTDRAWSURFACE     m_pWaitBuffer; 
	//##ModelId=3EBA499B035D
	LPDIRECTDRAWSURFACE		m_pGameOverBuffer;

};

#endif // !defined(AFX_GAME_H__57350DA2_B866_449D_9135_643C2C305326__INCLUDED_)
