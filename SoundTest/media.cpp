// audio2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "media.h"
//#include "audio2Dlg.h"
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
//	using namespace std;


/////////////////////////////////////////////////////////////////////////////
// CAudio2Dlg dialog
namespace SoundTest {
CAudio2Dlg::CAudio2Dlg()
{
	bEnding=FALSE;
	dwDataLength=0;

}

/////////////////////////////////////////////////////////////////////////////
// CAudio2Dlg message handlers

BOOL CAudio2Dlg::OnInitDialog()
{
////	CDialog::OnInitDialog();
//
//	// Add "About..." menu item to system menu.
//
//	// IDM_ABOUTBOX must be in the system command range.
//	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
//	ASSERT(IDM_ABOUTBOX < 0xF000);
//
//	CMenu* pSysMenu = GetSystemMenu(FALSE);
//	if (pSysMenu != NULL)
//	{
//		CString strAboutMenu;
//		strAboutMenu.LoadString(IDS_ABOUTBOX);
//		if (!strAboutMenu.IsEmpty())
//		{
//			pSysMenu->AppendMenu(MF_SEPARATOR);
//			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
//		}
//	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
//	SetIcon(m_hIcon, TRUE);			// Set big icon
//	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//allocate memory for wave header
	pWaveHdr1=reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
	pWaveHdr2=reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
	
	//allocate memory for save buffer
	pSaveBuffer = reinterpret_cast<PBYTE>(malloc(1));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAudio2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{

}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAudio2Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



void CAudio2Dlg::OnRecStart() 
{//allocate buffer memory
	pBuffer1=(PBYTE)malloc(INP_BUFFER_SIZE);
	pBuffer2=(PBYTE)malloc(INP_BUFFER_SIZE);
	if (!pBuffer1 || !pBuffer2) {
		if (pBuffer1) free(pBuffer1);
		if (pBuffer2) free(pBuffer2);
		MessageBeep(MB_ICONEXCLAMATION);
		//AfxMessageBox("Memory erro!");
		return ;
	}
	
	//open waveform audo for input
	
	waveform.wFormatTag=WAVE_FORMAT_PCM;
	waveform.nChannels=1;
	waveform.nSamplesPerSec=11025;
	waveform.nAvgBytesPerSec=11025;
	waveform.nBlockAlign=1;
	waveform.wBitsPerSample=8;
	waveform.cbSize=0;
	
	
	if (waveInOpen(&hWaveIn,WAVE_MAPPER,&waveform,0,NULL,CALLBACK_WINDOW)) {//(DWORD)this->m_hWnd
		free(pBuffer1);
		free(pBuffer2);
		MessageBeep(MB_ICONEXCLAMATION);
//		AfxMessageBox("Audio can not be open!");
	}
	pWaveHdr1->lpData=(LPTSTR)pBuffer1;
	pWaveHdr1->dwBufferLength=INP_BUFFER_SIZE;
	pWaveHdr1->dwBytesRecorded=0;
	pWaveHdr1->dwUser=0;
	pWaveHdr1->dwFlags=0;
	pWaveHdr1->dwLoops=1;
	pWaveHdr1->lpNext=NULL;
	pWaveHdr1->reserved=0;
	
	waveInPrepareHeader(hWaveIn,pWaveHdr1,sizeof(WAVEHDR));
	
	pWaveHdr2->lpData=(LPTSTR)pBuffer2;
	pWaveHdr2->dwBufferLength=INP_BUFFER_SIZE;
	pWaveHdr2->dwBytesRecorded=0;
	pWaveHdr2->dwUser=0;
	pWaveHdr2->dwFlags=0;
	pWaveHdr2->dwLoops=1;
	pWaveHdr2->lpNext=NULL;
	pWaveHdr2->reserved=0;
	
	waveInPrepareHeader(hWaveIn,pWaveHdr2,sizeof(WAVEHDR));
	
	//////////////////////////////////////////////////////////////////////////
	pSaveBuffer = (PBYTE)realloc (pSaveBuffer, 1) ;
	// Add the buffers
	
	waveInAddBuffer (hWaveIn, pWaveHdr1, sizeof (WAVEHDR)) ;
	waveInAddBuffer (hWaveIn, pWaveHdr2, sizeof (WAVEHDR)) ;
	
	// Begin sampling
	bEnding=FALSE;
	dwDataLength = 0 ;
	waveInStart (hWaveIn) ;

	
}



void CAudio2Dlg::OnRecStop() 
{bEnding=TRUE;

	waveInReset(hWaveIn);
	
	
}
void CAudio2Dlg::OnMM_WIM_OPEN(UINT wParam, LONG lParam) 
{
	// TODO: Add your message handler code here and/or call default
//		SetTimer(1,100,NULL);
	
}

#define debug 1
#ifndef debug//Ð´ÈëWAV explain by LC 2014.06.11 16:17
void CAudio2Dlg::OnMM_WIM_DATA(UINT wParam, LONG lParam) 
{
	// TODO: Add your message handler code here and/or call default
	// Reallocate save buffer memory
	
	//////////////////////////////////////////////////////////////////////////
	
	pNewBuffer = (PBYTE)realloc (pSaveBuffer, dwDataLength +
		((PWAVEHDR) lParam)->dwBytesRecorded) ;
	
	if (pNewBuffer == NULL)
	{
		waveInClose (hWaveIn) ;
		MessageBeep (MB_ICONEXCLAMATION) ;
		AfxMessageBox("erro memory");
		return ;
	}
	
	pSaveBuffer = pNewBuffer ;
	//////////////////////////////////////////////////////////////////////////
	
	CopyMemory (pSaveBuffer + dwDataLength, ((PWAVEHDR) lParam)->lpData,
		((PWAVEHDR) lParam)->dwBytesRecorded) ;
	
	dwDataLength += ((PWAVEHDR) lParam)->dwBytesRecorded ;
	
	if (bEnding)
	{
		waveInClose (hWaveIn) ;
		return ;
	}
	//////////////////////////////////////////////////////////////////////////
	CFile m_file;
    CFileException fileException;
    CString m_csFileName= "F:\\audio.wav";
	m_file.Open(m_csFileName,CFile::modeCreate|CFile::modeReadWrite, &fileException);
	/*if (m_file.Open(m_csFileName,CFile::modeCreate|CFile::modeReadWrite, &fileException))
	{MessageBox("open file");
}*/
	DWORD m_WaveHeaderSize = 38;
	DWORD m_WaveFormatSize = 18;
	m_file.SeekToBegin();
    m_file.Write("RIFF",4);
    //unsigned int Sec=(sizeof  + m_WaveHeaderSize);
	unsigned int Sec=(sizeof pSaveBuffer + m_WaveHeaderSize);
	m_file.Write(&Sec,sizeof(Sec));
	m_file.Write("WAVE",4);
	m_file.Write("fmt ",4);
	m_file.Write(&m_WaveFormatSize,sizeof(m_WaveFormatSize));
	
	m_file.Write(&waveform.wFormatTag,sizeof(waveform.wFormatTag));
	m_file.Write(&waveform.nChannels,sizeof(waveform.nChannels));
	m_file.Write(&waveform.nSamplesPerSec,sizeof(waveform.nSamplesPerSec));
	m_file.Write(&waveform.nAvgBytesPerSec,sizeof(waveform.nAvgBytesPerSec));
	m_file.Write(&waveform.nBlockAlign,sizeof(waveform.nBlockAlign));
	m_file.Write(&waveform.wBitsPerSample,sizeof(waveform.wBitsPerSample));
	m_file.Write(&waveform.cbSize,sizeof(waveform.cbSize));
	m_file.Write("data",4);
	m_file.Write(&dwDataLength,sizeof(dwDataLength));
	
	m_file.Write(pSaveBuffer,dwDataLength);
	m_file.Seek(dwDataLength,CFile::begin);
	m_file.Close();		
	
	// Send out a new buffer
	
	waveInAddBuffer (hWaveIn, (PWAVEHDR) lParam, sizeof (WAVEHDR)) ;
	return ;
	
	
}
#endif

void CAudio2Dlg::OnMM_WIM_CLOSE(UINT wParam, LONG lParam) 
{
	KillTimer(1);
	if (0==dwDataLength) {
		return;
	}
	waveInUnprepareHeader (hWaveIn, pWaveHdr1, sizeof (WAVEHDR)) ;
	waveInUnprepareHeader (hWaveIn, pWaveHdr2, sizeof (WAVEHDR)) ;
	
	free (pBuffer1) ;
	free (pBuffer2) ;
	return ;
	
}



void CAudio2Dlg::OnPlayStart() 
{
	
	//open waveform audio for output
	waveform.wFormatTag		=	WAVE_FORMAT_PCM;
	waveform.nChannels		=	1;
	waveform.nSamplesPerSec	=11025;
	waveform.nAvgBytesPerSec=11025;
	waveform.nBlockAlign	=1;
	waveform.wBitsPerSample	=8;
	waveform.cbSize			=0;
	
	
	if (waveOutOpen(&hWaveOut,WAVE_MAPPER,&waveform,0,NULL,CALLBACK_WINDOW)) {//(DWORD)this->m_hWnd
		MessageBeep(MB_ICONEXCLAMATION);
		//AfxMessageBox("Audio output erro");
	}
	
	
	return ;
	
}


void CAudio2Dlg::OnMM_WOM_OPEN(UINT wParam, LONG lParam){

	// Set up header
	
	pWaveHdr1->lpData          = (LPTSTR)pSaveBuffer ;
	pWaveHdr1->dwBufferLength  = dwDataLength ;
	pWaveHdr1->dwBytesRecorded = 0 ;
	pWaveHdr1->dwUser          = 0 ;
	pWaveHdr1->dwFlags         = WHDR_BEGINLOOP | WHDR_ENDLOOP ;
	pWaveHdr1->dwLoops         = 1;
	pWaveHdr1->lpNext          = NULL ;
	pWaveHdr1->reserved        = 0 ;
	
	// Prepare and write
	
	waveOutPrepareHeader (hWaveOut, pWaveHdr1, sizeof (WAVEHDR)) ;
	waveOutWrite (hWaveOut, pWaveHdr1, sizeof (WAVEHDR)) ;

	
}

void CAudio2Dlg::OnMM_WOM_DONE(UINT wParam, LONG lParam)
{
	
	waveOutUnprepareHeader (hWaveOut, pWaveHdr1, sizeof (WAVEHDR)) ;
	waveOutClose (hWaveOut) ;
	
	
	return  ;
	
}

}