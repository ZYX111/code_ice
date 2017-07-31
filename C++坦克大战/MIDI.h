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


// Midi.h : main header file for the MIDI application
//

// This class is based on the DirectX sample "mstream".

#ifndef MIDI_h
#define MIDI_h

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <mmsystem.h>
#pragma message("linking with multimedia library")
#pragma comment(lib, "winmm.lib")

#include <vector>
using namespace std;


// This message is sent to the controlling window, if the volume changes in
// another way than explicitly set by the owner of the CMIDI object.
// WPARAM	the pointer to the MIDI object
// LPARAM	lo-word: the number of the channel that changed volume
//			hi-word: the new volume in percent
#define	WM_MIDI_VOLUMECHANGED	WM_USER+23


#define MIDI_CTRLCHANGE			((BYTE)0xB0)		// + ctrlr + value
#define MIDI_PRGMCHANGE			((BYTE)0xC0)		// + new patch
#define MIDI_CHANPRESS			((BYTE)0xD0)		// + pressure (1 byte)

#define MIDICTRL_VOLUME			((BYTE)0x07)

#define MIDIEVENT_CHANNEL(dw)	(dw & 0x0000000F)
#define MIDIEVENT_TYPE(dw)		(dw & 0x000000F0)
#define MIDIEVENT_DATA1(dw)		((dw & 0x0000FF00) >> 8)
#define MIDIEVENT_VOLUME(dw)	((dw & 0x007F0000) >> 16)

#define MIDI_SYSEX				((BYTE)0xF0)		// SysEx begin
#define MIDI_SYSEXEND			((BYTE)0xF7)		// SysEx end
#define MIDI_META				((BYTE)0xFF)		// Meta event begin
#define MIDI_META_TEMPO			((BYTE)0x51)		// Tempo change
#define MIDI_META_EOT			((BYTE)0x2F)		// End-of-track


// flags for the ConvertToBuffer() method
#define CONVERTF_RESET				0x00000001
#define CONVERTF_STATUS_DONE		0x00000001
#define CONVERTF_STATUS_STUCK		0x00000002
#define CONVERTF_STATUS_GOTEVENT	0x00000004

// Return values from the ConvertToBuffer() method
#define CONVERTERR_NOERROR		0		// No error occured
#define CONVERTERR_CORRUPT		-101	// The input file is corrupt
// The converter has already encountered a corrupt file and cannot convert any
// more of this file -- must reset the converter
#define CONVERTERR_STUCK		-102
#define CONVERTERR_DONE			-103	// Converter is done
#define CONVERTERR_BUFFERFULL	-104	// The buffer is full
#define CONVERTERR_METASKIP		-105	// Skipping unknown meta event

#define STATUS_KILLCALLBACK		100		// Signals that the callback should die
#define STATUS_CALLBACKDEAD		200		// Signals callback is done processing
#define STATUS_WAITINGFOREND	300		// Callback's waiting for buffers to play

// Description of a track
//
//##ModelId=3EBA49A1033C
struct TRACK
{
	//##ModelId=3EBA49A10350
	DWORD	fdwTrack;		// Track's flags
	//##ModelId=3EBA49A1034F
	DWORD	dwTrackLength;	// Total bytes in track
	//##ModelId=3EBA49A1034E
	LPBYTE	pTrackStart;	// -> start of track data buffer
	//##ModelId=3EBA49A1034D
	LPBYTE	pTrackCurrent;	// -> next byte to read in buffer
	//##ModelId=3EBA49A1034C
	DWORD	tkNextEventDue;	// Absolute time of next event in track
	//##ModelId=3EBA49A1034B
	BYTE	byRunningStatus;// Running status from last channel msg

	//##ModelId=3EBA49A10351
	TRACK()
		: fdwTrack(0)
		, dwTrackLength(0)
		, pTrackStart(0)
		, pTrackCurrent(0)
		, tkNextEventDue(0)
		, byRunningStatus(0)
	{
	}
};

#define ITS_F_ENDOFTRK		0x00000001


// This structure is used to pass information to the ConvertToBuffer()
// system and then internally by that function to send information about the
// target stream buffer and current state of the conversion process to internal
// lower level conversion routines.
//##ModelId=3EBA499E00DA
struct CONVERTINFO
{
	//##ModelId=3EBA499E00EF
    MIDIHDR	mhBuffer;			// Standard Windows stream buffer header
	//##ModelId=3EBA499E00EC
    DWORD	dwStartOffset;		// Start offset from mhStreamBuffer.lpStart
	//##ModelId=3EBA499E00EB
    DWORD	dwMaxLength;		// Max length to convert on this pass
	//##ModelId=3EBA499E00EA
    DWORD	dwBytesRecorded;
	//##ModelId=3EBA499E00DC
    DWORD	tkStart;
	//##ModelId=3EBA499E00DB
    BOOL	bTimesUp;

	//##ModelId=3EBA499E00F2
	CONVERTINFO()
		: dwStartOffset(0)
		, dwMaxLength(0)
		, dwBytesRecorded(0)
		, tkStart(0)
		, bTimesUp(FALSE)
	{
		memset(&mhBuffer, 0, sizeof(MIDIHDR));
	}
};

// Temporary event structure which stores event data until we're ready to
// dump it into a stream buffer
//##ModelId=3EBA49A201C5
struct TEMPEVENT
{
	//##ModelId=3EBA49A201D7
	DWORD	tkEvent;		// Absolute time of event
	//##ModelId=3EBA49A201D6
	BYTE	byShortData[4];	// Event type and parameters if channel msg
	//##ModelId=3EBA49A201D5
	DWORD	dwEventLength;	// Length of data which follows if meta or sysex
	//##ModelId=3EBA49A201D4
	LPBYTE	pLongData;		// -> Event data if applicable
};

//##ModelId=3EBA499D032C
class CMIDI  
{
protected:
	typedef vector<TRACK>		TrackArray_t;
	typedef vector<DWORD>		VolumeArray_t;
	typedef vector<CONVERTINFO>	ConvertArray_t;

	enum {
		NUM_CHANNELS = 16,		// 16 volume channels
		VOLUME_INIT = 100,		// 100% volume by default
		NUM_STREAM_BUFFERS = 2,
		OUT_BUFFER_SIZE = 1024,	// Max stream buffer size in bytes
		DEBUG_CALLBACK_TIMEOUT = 2000,
		VOLUME_MIN = 0,
		VOLUME_MAX = 127		// == 100%
	};

public:
	//##ModelId=3EBA499E00AF
	CMIDI();
	//##ModelId=3EBA499E00AE
	virtual ~CMIDI();

	//##ModelId=3EBA499E0090
	BOOL	Create(LPVOID pSoundData, DWORD dwSize, CWnd * pParent = 0);
	//##ModelId=3EBA499E009E
	BOOL	Create(LPCTSTR pszResID, CWnd * pParent = 0);
	//##ModelId=3EBA499E00AB
	BOOL	Create(UINT uResID, CWnd * pParent = 0);

	//##ModelId=3EBA499E008E
	BOOL	Play(BOOL bInfinite = FALSE);
	//##ModelId=3EBA499E008C
	BOOL	Stop(BOOL bReOpen = TRUE);
	//##ModelId=3EBA499E0083
	BOOL	IsPlaying() const { return m_bPlaying; }

	//##ModelId=3EBA499E0082
	BOOL	Pause();
	//##ModelId=3EBA499E0081
	BOOL	Continue();
	//##ModelId=3EBA499E0080
	BOOL	IsPaused() const { return m_bPaused; }

		// Set playback position back to the start
	//##ModelId=3EBA499E007F
	BOOL	Rewind();

		// Get the number of volume channels
	//##ModelId=3EBA499E007E
	DWORD	GetChannelCount() const;

		// Set the volume of a channel in percent. Channels are from 0 to (GetChannelCount()-1)
	//##ModelId=3EBA499E0072
	void	SetChannelVolume(DWORD channel, DWORD percent);

		// Get the volume of a channel in percent
	//##ModelId=3EBA499E0070
	DWORD	GetChannelVolume(DWORD channel) const;
	
		// Set the volume for all channels in percent
	//##ModelId=3EBA499E006E
	void	SetVolume(DWORD percent);

		// Get the average volume for all channels
	//##ModelId=3EBA499E006D
	DWORD	GetVolume() const;

		// Set the tempo of the playback. Default: 100%
	//##ModelId=3EBA499E0060
	void	SetTempo(DWORD percent);

		// Get the current tempo in percent (usually 100)
	//##ModelId=3EBA499E005F
	DWORD	GetTempo() const;

		// You can (un)set an infinite loop during playback.
		// Note that "Play()" resets this setting!
	//##ModelId=3EBA499E005D
	void	SetInfinitePlay(BOOL bSet = TRUE);

protected:	// implementation
		// This function converts MIDI data from the track buffers.
	//##ModelId=3EBA499E0050
	int		ConvertToBuffer(DWORD dwFlags, CONVERTINFO * lpciInfo);

		// Fills in the event struct with the next event from the track
	//##ModelId=3EBA499E0042
	BOOL	GetTrackEvent(TRACK * ptsTrack, TEMPEVENT * pteTemp);

		// Retrieve the next byte from the track buffer, refilling the buffer from
		// disk if necessary.
	//##ModelId=3EBA499E003F
	BOOL	GetTrackByte(TRACK * ptsTrack, LPBYTE lpbyByte) {
				if( DWORD(ptsTrack->pTrackCurrent - ptsTrack->pTrackStart) == ptsTrack->dwTrackLength )
					return FALSE;
				*lpbyByte = *ptsTrack->pTrackCurrent++;
				return TRUE;
			}

		// Attempts to parse a variable length DWORD from the given track.
	//##ModelId=3EBA499E0032
	BOOL	GetTrackVDWord(TRACK * ptsTrack, LPDWORD lpdw);

		// Put the given event into the given stream buffer at the given location.
	//##ModelId=3EBA499E002F
	int		AddEventToStreamBuffer( TEMPEVENT * pteTemp, CONVERTINFO * lpciInfo );

		// Opens a MIDI stream. Then it goes about converting the data into a midiStream buffer for playback.
	//##ModelId=3EBA499E002E
	BOOL	StreamBufferSetup();

	//##ModelId=3EBA499E0023
	void	FreeBuffers();


protected:	// error handling
		// The default implementation writes the error message in the
		// debuggers output window. Override if you want a different
		// behavior.
	//##ModelId=3EBA499E0021
	virtual void	MidiError(MMRESULT Result);

		// Failure in converting track into stream.
		// The default implementation displays the offset and the total
		// number of bytes of the failed track and the error message in
		// the debuggers output window. 
	//##ModelId=3EBA499E0013
	virtual void	TrackError(TRACK *, LPSTR ErrMsg);


protected:	// overridables
		// NOTE THAT, IF YOU OVERRIDE ONE OF THESE METHODS, YOU MUST CALL
		// THE BASE CLASS IMPLEMENTATION TOO!

		// called when a MIDI output device is opened
	//##ModelId=3EBA499E0012
	virtual void	OnMidiOutOpen();
	
		// called when the MIDI output device is closed
	//##ModelId=3EBA499E0011
	virtual void	OnMidiOutClose();

		// called when the specified system-exclusive or stream buffer
		// has been played and is being returned to the application
	//##ModelId=3EBA499E000F
	virtual void	OnMidiOutDone(MIDIHDR &);

		// called when a MEVT_F_CALLBACK event is reached in the MIDI output stream
	//##ModelId=3EBA499E0002
	virtual void	OnMidiOutPositionCB(MIDIHDR &, MIDIEVENT &);


private:	// callback procedure
		// This procedure calls the overridables above.
	//##ModelId=3EBA499D03D8
	static void	CALLBACK	MidiProc(HMIDIOUT, UINT, DWORD, DWORD, DWORD);


protected:	// data members
	//##ModelId=3EBA499D03C9
	DWORD			m_dwSoundSize;
	//##ModelId=3EBA499D03C8
	LPVOID			m_pSoundData;
	//##ModelId=3EBA499D03BB
	DWORD			m_dwFormat;
	//##ModelId=3EBA499D03BA
	DWORD			m_dwTrackCount;
	//##ModelId=3EBA499D03B9
	DWORD			m_dwTimeDivision;
	//##ModelId=3EBA499D03AF
	BOOL			m_bPlaying;
	//##ModelId=3EBA499D03AC
	HMIDISTRM		m_hStream;
	//##ModelId=3EBA499D03A9
	DWORD			m_dwProgressBytes;
	//##ModelId=3EBA499D039A
	BOOL			m_bLooped;
	//##ModelId=3EBA499D0399
	DWORD			m_tkCurrentTime;
	//##ModelId=3EBA499D038C
	DWORD			m_dwBufferTickLength;
	//##ModelId=3EBA499D038B
	DWORD			m_dwCurrentTempo;
	//##ModelId=3EBA499D038A
	DWORD			m_dwTempoMultiplier;
	//##ModelId=3EBA499D037C
	BOOL			m_bInsertTempo;
	//##ModelId=3EBA499D037B
	BOOL			m_bBuffersPrepared;
	//##ModelId=3EBA499D037A
	int				m_nCurrentBuffer;
	//##ModelId=3EBA499D036C
	UINT			m_uMIDIDeviceID;
	//##ModelId=3EBA499D036B
	int				m_nEmptyBuffers;
	//##ModelId=3EBA499D0360
	BOOL			m_bPaused;
	//##ModelId=3EBA499D035F
	UINT			m_uCallbackStatus;
	//##ModelId=3EBA499D035E
	HANDLE			m_hBufferReturnEvent;
	//##ModelId=3EBA499D035B
	CWnd *			m_pWndParent;
	//##ModelId=3EBA499D0355
	TrackArray_t	m_Tracks;
	//##ModelId=3EBA499D0350
	VolumeArray_t	m_Volumes;
	//##ModelId=3EBA499D034B
	ConvertArray_t	m_StreamBuffers;

	// data members especially for ConvertToBuffer()
	//##ModelId=3EBA499D0345
    TRACK *			m_ptsTrack;
	//##ModelId=3EBA499D0340
	TRACK *			m_ptsFound;
	//##ModelId=3EBA499D033D
    DWORD			m_dwStatus;
	//##ModelId=3EBA499D033C
    DWORD			m_tkNext;
	//##ModelId=3EBA499D0332
	DWORD			m_dwMallocBlocks;
	//##ModelId=3EBA499D032F
    TEMPEVENT		m_teTemp;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // MIDI_h
