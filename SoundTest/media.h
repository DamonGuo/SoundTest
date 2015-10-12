// audio2Dlg.h : header file
//
#include "stdafx.h"
#include <windows.h>
#include <mmsystem.h>
#include "resource.h"
#include <string>
#include <fstream>
#include <iostream>
#include "stdlib.h"
#include "stdio.h"

#pragma comment(lib,"WINMM.LIB")
#if !defined(AFX_AUDIO2DLG_H__ED9BB19A_81F5_4EAC_A39A_8EF723702621__INCLUDED_)
#define AFX_AUDIO2DLG_H__ED9BB19A_81F5_4EAC_A39A_8EF723702621__INCLUDED_

#ifndef afx_msg
#define afx_msg         // intentional placeholder
#endif

/////////////////////////////////////////////////////////////////////////////
// CAudio2Dlg dialog
#define  INP_BUFFER_SIZE 16384
namespace SoundTest {
	using namespace std;

	public class CAudio
	{
		// Construction
	public:
		//CAudio2Dlg();	// standard constructor
		enum { IDD = IDD_AUDIO2_DIALOG };

	protected:


		// Implementation
	protected:
		BOOL bEnding;
		DWORD dwDataLength;
		HWAVEIN hWaveIn;
		HWAVEOUT hWaveOut;
		PBYTE pBuffer1,pBuffer2,pSaveBuffer,pNewBuffer;
		PWAVEHDR pWaveHdr1,pWaveHdr2;
		WAVEFORMATEX waveform;
		// Generated message map functions
		//{{AFX_MSG(CAudio2Dlg)
		//virtual BOOL OnInitDialog();
		//afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
		//afx_msg void OnPaint();
		//afx_msg HCURSOR OnQueryDragIcon();
		//afx_msg void OnMM_WIM_OPEN(UINT wParam,LONG lParam);
		//afx_msg void OnMM_WIM_DATA(UINT wParam,LONG lParam);
		////afx_msg void OnMM_WIM_CLOSE(UINT wParam,LONG lParam);
		//afx_msg void OnMM_WOM_OPEN(UINT wParam,LONG lParam);
		//afx_msg void OnMM_WOM_DONE(UINT wParam,LONG lParam);
		//afx_msg void OnMM_WOM_CLOSE(UINT wParam,LONG lParam);
		//afx_msg void OnRecStart();
		//afx_msg void OnRecStop();
		//afx_msg void OnPlayStart();
		//}}AFX_MSG
		//	DECLARE_MESSAGE_MAP();


	public:
		CAudio()
		{
			bEnding=FALSE;
			dwDataLength=0;

		}

		/////////////////////////////////////////////////////////////////////////////
		// CAudio2Dlg message handlers

		BOOL OnInitDialog()
		{
			// TODO: Add extra initialization here
			//allocate memory for wave header
			pWaveHdr1=reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));
			pWaveHdr2=reinterpret_cast<PWAVEHDR>(malloc(sizeof(WAVEHDR)));

			//allocate memory for save buffer
			pSaveBuffer = reinterpret_cast<PBYTE>(malloc(1));

			return TRUE;  // return TRUE  unless you set the focus to a control
		}

		// The system calls this to obtain the cursor to display while the user drags
		//  the minimized window.
	/*	HCURSOR OnQueryDragIcon()
		{
			return (HCURSOR) m_hIcon;
		}*/

		void OnRecStart() 
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
			pWaveHdr1->lpData=(LPSTR)pBuffer1;
			pWaveHdr1->dwBufferLength=INP_BUFFER_SIZE;
			pWaveHdr1->dwBytesRecorded=0;
			pWaveHdr1->dwUser=0;
			pWaveHdr1->dwFlags=0;
			pWaveHdr1->dwLoops=1;
			pWaveHdr1->lpNext=NULL;
			pWaveHdr1->reserved=0;

			waveInPrepareHeader(hWaveIn,pWaveHdr1,sizeof(WAVEHDR));

			pWaveHdr2->lpData=(LPSTR)pBuffer2;
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



		void OnRecStop() 
		{bEnding=TRUE;

		waveInReset(hWaveIn);


		}
		void OnMM_WIM_OPEN(UINT wParam, LONG lParam) 
		{
			// TODO: Add your message handler code here and/or call default
//					SetTimer(1,100,NULL);

		}

//#define debug 1
//#ifndef debug//Ð´ÈëWAV explain by LC 2014.06.11 16:17
		int write_wav(const char *wav_path, PWAVEHDR lParam) 
		{
			// TODO: Add your message handler code here and/or call default
			// Reallocate save buffer memory

			//////////////////////////////////////////////////////////////////////////
			int ret = 0;
			pNewBuffer = (PBYTE)realloc (pSaveBuffer, dwDataLength +
				((PWAVEHDR) lParam)->dwBytesRecorded) ;

			if (pNewBuffer == NULL)
			{
				waveInClose (hWaveIn) ;
			//	MessageBeep (MB_ICONEXCLAMATION) ;
			//	AfxMessageBox("erro memory");
				return ret = -1;
			}

			pSaveBuffer = pNewBuffer ;
			//////////////////////////////////////////////////////////////////////////

			CopyMemory (pSaveBuffer + dwDataLength, ((PWAVEHDR) lParam)->lpData,
				((PWAVEHDR) lParam)->dwBytesRecorded) ;

			dwDataLength += ((PWAVEHDR) lParam)->dwBytesRecorded ;

			if (bEnding)
			{
				waveInClose (hWaveIn) ;
				return ret = -1;;
			}
			//////////////////////////////////////////////////////////////////////////
			FILE* m_file = NULL;
			fopen_s(&m_file,"wav_path","wb");
			if(m_file == NULL)
			{
				return ret = -1;
			}

			DWORD m_WaveHeaderSize = 38;
			DWORD m_WaveFormatSize = 18;
			fseek(m_file, 0, SEEK_SET);
			fwrite("RIFF",4,1,m_file);
			//unsigned int Sec=(sizeof  + m_WaveHeaderSize);
			unsigned int Sec=(sizeof pSaveBuffer + m_WaveHeaderSize);
			fwrite(&Sec,sizeof(Sec),1,m_file);
			fwrite("WAVE",4,1,m_file);
			fwrite("fmt ",4,1,m_file);
			fwrite(&m_WaveFormatSize,sizeof(m_WaveFormatSize),1,m_file);

			fwrite(&waveform.wFormatTag,sizeof(waveform.wFormatTag),1,m_file);
			fwrite(&waveform.nChannels,sizeof(waveform.nChannels),1,m_file);
			fwrite(&waveform.nSamplesPerSec,sizeof(waveform.nSamplesPerSec),1,m_file);
			fwrite(&waveform.nAvgBytesPerSec,sizeof(waveform.nAvgBytesPerSec),1,m_file);
			fwrite(&waveform.nBlockAlign,sizeof(waveform.nBlockAlign),1,m_file);
			fwrite(&waveform.wBitsPerSample,sizeof(waveform.wBitsPerSample),1,m_file);
			fwrite(&waveform.cbSize,sizeof(waveform.cbSize),1,m_file);
			fwrite("data",4,1,m_file);
			fwrite(&dwDataLength,sizeof(dwDataLength),1,m_file);

			fwrite(pSaveBuffer,dwDataLength,1,m_file);
			fseek(m_file,dwDataLength,SEEK_SET);
			fclose(m_file);		

			// Send out a new buffer

			waveInAddBuffer (hWaveIn, (PWAVEHDR) lParam, sizeof (WAVEHDR)) ;
			return ret = 0;


		}
//#endif

		void OnMM_WIM_CLOSE(UINT wParam, LONG lParam) 
		{
			//KillTimer(1);
			if (0==dwDataLength) {
				return;
			}
			waveInUnprepareHeader (hWaveIn, pWaveHdr1, sizeof (WAVEHDR)) ;
			waveInUnprepareHeader (hWaveIn, pWaveHdr2, sizeof (WAVEHDR)) ;

			free (pBuffer1) ;
			free (pBuffer2) ;
			return ;

		}



		void OnPlayStart() 
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


		void OnMM_WOM_OPEN(UINT wParam, LONG lParam){

			// Set up header

			pWaveHdr1->lpData          = (LPSTR)pSaveBuffer ;
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

		void OnMM_WOM_DONE(UINT wParam, LONG lParam)
		{

			waveOutUnprepareHeader (hWaveOut, pWaveHdr1, sizeof (WAVEHDR)) ;
			waveOutClose (hWaveOut) ;


			return  ;

		}


	};
}
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUDIO2DLG_H__ED9BB19A_81F5_4EAC_A39A_8EF723702621__INCLUDED_)
