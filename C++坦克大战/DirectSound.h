/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998 by Jörg König
// All rights reserved
//
// This file is part of the completely free tetris clone "CGTetris".
//
// This is free software.
// You may redistribute it by any means providing it is not sold for profit
// without the authors written consent.
//
// No warrantee of any kind, expressed or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc., and
// I'll try to keep a version up to date.  I can be reached as follows:
//    J.Koenig@adg.de                 (company site)
//    Joerg.Koenig@rhein-neckar.de    (private site)
/////////////////////////////////////////////////////////////////////////////


// DirectSound.h: interface for the CDirectSound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRECTSOUND_H__A20FE86F_118F_11D2_9AB3_0060B0CDC13E__INCLUDED_)
#define AFX_DIRECTSOUND_H__A20FE86F_118F_11D2_9AB3_0060B0CDC13E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <mmsystem.h>
#include <dsound.h>

#pragma message("linking with Microsoft's DirectSound library ...")
#pragma comment(lib, "dsound.lib")

//##ModelId=3EBA49A20138
class CDirectSound  
{
public:		// construction/destruction
	//##ModelId=3EBA49A2017C
	CDirectSound();
	//##ModelId=3EBA49A2017B
	virtual ~CDirectSound();
	
	// If the "pWnd" paramter is NULL, then AfxGetApp()->GetMainWnd() will be used.
	//##ModelId=3EBA49A2016D
	BOOL	Create(LPCTSTR pszResource, CWnd * pWnd = 0);
	//##ModelId=3EBA49A20170
	BOOL	Create(UINT uResourceID, CWnd * pWnd = 0) {
				return Create(MAKEINTRESOURCE(uResourceID), pWnd);
			}
	// Alternativly you can specify the sound by yourself
	// Note that the class does not copy the entire data ! Instead
	// a pointer to the given data will be stored !
	// You can load an entire WAV file into memory and then call this
	// Create() method.
	//##ModelId=3EBA49A20178
	BOOL	Create(LPVOID pSoundData, CWnd * pWnd = 0);

public:		// operations
	//##ModelId=3EBA49A2016C
	BOOL			IsValid() const;
	//##ModelId=3EBA49A20169
	void			Play(DWORD dwStartPosition = 0, BOOL bLoop = FALSE);
	//##ModelId=3EBA49A20168
	void			Stop();
	//##ModelId=3EBA49A20167
	void			Pause();
	//##ModelId=3EBA49A20162
	void			Continue();
	//##ModelId=3EBA49A20160
	CDirectSound &	EnableSound(BOOL bEnable = TRUE) {
						m_bEnabled = bEnable;
						if( ! bEnable )
							Stop();
						return * this;
					}
	//##ModelId=3EBA49A2015F
	BOOL			IsEnabled() const { return m_bEnabled; }

protected:	// implementation
	//##ModelId=3EBA49A2015C
	BOOL SetSoundData(LPVOID pSoundData, DWORD dwSoundSize);
	//##ModelId=3EBA49A2015A
	BOOL CreateSoundBuffer(WAVEFORMATEX * pcmwf);
	//##ModelId=3EBA49A20150
	BOOL GetWaveData(void * pRes, WAVEFORMATEX * & pWaveHeader, void * & pbWaveData, DWORD & cbWaveSize);

private:	// data member
	//##ModelId=3EBA49A2014F
	LPVOID m_pTheSound;
	//##ModelId=3EBA49A2014E
	DWORD m_dwTheSound;
	//##ModelId=3EBA49A2014B
	LPDIRECTSOUNDBUFFER m_pDsb;
	//##ModelId=3EBA49A20148
	BOOL m_bEnabled;
	//##ModelId=3EBA49A2013C
	static LPDIRECTSOUND m_lpDirectSound;
	//##ModelId=3EBA49A20139
	static DWORD m_dwInstances;
};

#endif // !defined(AFX_DIRECTSOUND_H__A20FE86F_118F_11D2_9AB3_0060B0CDC13E__INCLUDED_)
