/*
 string str("hello");
 const char *strp=str.c_str();    stringתconst char*
*/
//#using "Microsoft.DirectX.DirectSound.dll"
//#using "Microsoft.DirectX.dll"

#include "../SoundTest/stdafx.h"
//#include "stdafx.h"
#include "stdlib.h"
#include "stdio.h"
#include <windows.h>
#include <conio.h>
#include <errno.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <time.h>
#include <string>
#include <msclr\marshal.h>

using namespace std;

#include "../include/qisr.h"
#include "../include/qtts.h"
#include "../include/msp_cmn.h"
#include "../include/msp_errors.h"



#ifdef _WIN64
#pragma comment(lib,"../lib/msc_x64.lib")//x64
#else
#pragma comment(lib,"../lib/msc.lib")//x86
#endif

#define DebugPrint(str_x,msg_y) fprintf(out_file,(str_x),(msg_y))

typedef int SR_DWORD;
typedef short int SR_WORD ;

//��Ƶͷ����ʽ
struct wave_pcm_hdr
{
	char            riff[4];                        // = "RIFF"
	SR_DWORD        size_8;                         // = FileSize - 8
	char            wave[4];                        // = "WAVE"
	char            fmt[4];                         // = "fmt "
	SR_DWORD        dwFmtSize;                      // = ��һ���ṹ��Ĵ�С : 16

	SR_WORD         format_tag;              // = PCM : 1
	SR_WORD         channels;                       // = ͨ���� : 1
	SR_DWORD        samples_per_sec;        // = ������ : 8000 | 6000 | 11025 | 16000
	SR_DWORD        avg_bytes_per_sec;      // = ÿ���ֽ��� : dwSamplesPerSec * wBitsPerSample / 8
	SR_WORD         block_align;            // = ÿ�������ֽ��� : wBitsPerSample / 8
	SR_WORD         bits_per_sample;         // = ����������: 8 | 16

	char            data[4];                        // = "data";
	SR_DWORD        data_size;                // = �����ݳ��� : FileSize - 44 
} ;

//Ĭ����Ƶͷ������
const struct wave_pcm_hdr default_pcmwavhdr = 
{
	{ 'R', 'I', 'F', 'F' },
	0,
	{'W', 'A', 'V', 'E'},
	{'f', 'm', 't', ' '},
	16,
	1,
	1,
	16000,
	32000,
	2,
	16,
	{'d', 'a', 't', 'a'},
	0  
};

namespace SoundTest {
	using namespace System;
	using namespace System::Runtime::InteropServices;
	using namespace System::Media;
	using namespace msclr::interop;
//	using namespace Microsoft::DirectX;
//	using namespace Microsoft::DirectX::DirectSound;
	ref class SoundType;

	public class XunFeiSDK{
	
	private: FILE* out_file;//���log�ļ�
			 string appid;
			 int ret;
			 string pcm_path;//�洢��Ƶ�ļ����ļ���
			 string user;
			 string password;
		     string voice_type;//��������
			 string volunm;//���� 0-10
			 string engin;//����
			 string voice_speed;//����0-10
			 
	public: XunFeiSDK()
			{
				DateTime nowTime = DateTime::Now;
				string nowTimes = (const char*)(Marshal::StringToHGlobalAnsi(nowTime.ToString("yyyy-MM-dd HH:mm:ss"))).ToPointer();
				fopen_s(&out_file,"log.txt","at+");
				if(out_file == NULL)
				{
					ret = -1;
					return;
				}
				fseek(out_file, 0, 2);
				fprintf(out_file,"begin Time:%s \n",nowTimes.c_str());

				appid = "53954218";
				user = "1018412834@qq.com";
				password = "abc123";
				pcm_path = "PCM_SPEED.pcm";
				voice_type = "xiaoyan";
				volunm = "7";
				voice_speed = "5";
				engin = "intp65";
			}
			 ~XunFeiSDK()
			{
				string nowTimes = (const char*)(Marshal::StringToHGlobalAnsi(DateTime::Now.ToString("yyyy-MM-dd HH:mm:ss"))).ToPointer();
				fprintf(out_file,"Time:%s end\n",nowTimes.c_str());
				fclose(out_file);
			}

	public: int status()
			{
				return ret;
			}

			bool Login()//��¼
			{
				string logins = "appid = " + appid + ",work_dir =   .  ";
				ret = MSPLogin(user.c_str(), password.c_str(), logins.c_str());
				if ( ret != MSP_SUCCESS )
				{
					fprintf(out_file,"MSPLogin failed , Error code %d.\n",ret);
					return false;
				}
				return true;
			}

			void Logout()
			{
				MSPLogout();//�˳���¼
			}

			int TextToSpeed(System::String^ Ssrc_text)//�ַ���ת��Ƶ����Ƶ�����PCM_SPEED.pcm��
			{
				#pragma region �ַ���ת��Ƶ
				struct wave_pcm_hdr pcmwavhdr = default_pcmwavhdr;
				const char* sess_id = NULL;
				unsigned int text_len = 0;
				char* audio_data = NULL;
				unsigned int audio_len = 0;
				int synth_status = MSP_TTS_FLAG_STILL_HAVE_DATA;
				FILE* fp = NULL;
				string params = "vcn=xiaoyan, spd = 50, vol = 50";//�����ɲο������ò����б�
				ret = -1;//ʧ��
				//��������
				params = "vcn=" + voice_type + ", spd = " + voice_speed + ", vol = " + volunm + ", ent = "+engin;
				const char* src_text = (const char*)(Marshal::StringToHGlobalAnsi(Ssrc_text)).ToPointer();

				pcm_path = "PCM_SPEED.pcm";

				fprintf(out_file,"begin to synth source = %s\n",src_text);
				if (NULL == src_text)
				{
					fprintf(out_file,"params is null!\n");
					return ret;
				}
				text_len = strlen(src_text);//��ȡ�ı�����
				
				fopen_s(&fp,pcm_path.c_str(),"wb");//��PCM�ļ�
				if (NULL == fp)
				{
					fprintf(out_file,"open PCM file %s error\n",pcm_path);
					return ret;
				}

				sess_id = QTTSSessionBegin(params.c_str(), &ret);//��ʼһ���Ự				
				if ( ret != MSP_SUCCESS )
				{
					fprintf(out_file,"QTTSSessionBegin: qtts begin session failed Error code %d.\n",ret);
					return ret;
				}
				fprintf(out_file,"sess_id = %s\n",sess_id);
				ret = QTTSTextPut(sess_id, src_text, text_len, NULL );//����txt��Ϣ
				if ( ret != MSP_SUCCESS )
				{
					fprintf(out_file,"QTTSTextPut: qtts put text failed Error code %d.\n",ret);
					QTTSSessionEnd(sess_id, "TextPutError");//�쳣������
					return ret;
				}
				fwrite(&pcmwavhdr, sizeof(pcmwavhdr) ,1, fp);//�ѿ�ʼ�ļ�д����ǰ��

				while (1)//ѭ����ȡ��Ƶ�ļ����洢 
				{
					const void *data = QTTSAudioGet(sess_id, &audio_len, &synth_status, &ret);
					if (NULL != data)
					{
						fwrite(data, audio_len, 1, fp);
						pcmwavhdr.data_size += audio_len;//����pcm���ݵĴ�С
					}
					if (synth_status == MSP_TTS_FLAG_DATA_END || ret != 0) 
						break;
				}//�ϳ�״̬synth_statusȡֵ�ɲο������ĵ�

				//����pcm�ļ�ͷ���ݵĴ�С
				pcmwavhdr.size_8 += pcmwavhdr.data_size + 36;

				//��������������д���ļ�ͷ��
				fseek(fp, 4, 0);
				fwrite(&pcmwavhdr.size_8,sizeof(pcmwavhdr.size_8), 1, fp);
				fseek(fp, 40, 0);
				fwrite(&pcmwavhdr.data_size,sizeof(pcmwavhdr.data_size), 1, fp);
				fclose(fp);

				ret = QTTSSessionEnd(sess_id, NULL);
				if ( ret != MSP_SUCCESS )
				{
					fprintf(out_file,"QTTSSessionEnd: qtts end failed Error code %d.\n",ret);
				}
				fprintf(out_file,"program end");
				return ret;
				#pragma endregion
			}

			System::String^ GetPcmName()//��ȡ��Ƶ�ļ�·��
			{
				return gcnew String(pcm_path.c_str());
			}
			
			int Play(System::String^ text)//������Ƶ�ļ�
			{
				if(text == "") return -1;
				SoundPlayer^ player = (gcnew SoundPlayer(text));//��Ƶ������
				player->SoundLocation = text;
				player->Load();
				player->Play();
				return 0;
			}

			int StartRecord()//��ʼ¼��
			{
			
			}

			int EndRecord()//����¼��
			{
			
			}

			System::String^ SpeedToText(System::String^ text)//����ת���֣����������ļ���������������Ϣ
			{
				System::String^ Sys_value = "No data return";
				const char* src_wav_filename = (const char*)(Marshal::StringToHGlobalAnsi(text)).ToPointer();
				//test = Marshal::PtrToStringAnsi((IntPtr)(char *)src_text);
				//return test;
				char rec_result[1024] = {0};//��ŷ��ؽ��
				const char *sessionID = NULL;
				FILE *f_pcm = NULL;//
				char *pPCM = NULL;//�����Ƶ�ļ�����
				int lastAudio = 0 ;
				int audStat = MSP_AUDIO_SAMPLE_CONTINUE ;
				int epStatus = MSP_EP_LOOKING_FOR_SPEECH;
				int recStatus = MSP_REC_STATUS_SUCCESS ;
				long pcmCount = 0;
				long pcmSize = 0;//��Ƶ�ļ���С
				int errCode = 10 ;
				string param = "sub=iat,auf=audio/L16;rate=16000,aue=speex-wb,ent=sms16k,rst=plain,rse=gb2312";

				fprintf(out_file,"Start iat...\n");
				sessionID = QISRSessionBegin(NULL, param.c_str(), &errCode);//��ʼһ·�Ự
				fopen_s(&f_pcm,src_wav_filename, "rb");
				if (NULL != f_pcm) {
					fseek(f_pcm, 0, SEEK_END);
					pcmSize = ftell(f_pcm);//��ȡ��Ƶ��С
					fseek(f_pcm, 0, SEEK_SET);
					pPCM = (char *)malloc(pcmSize);//�����ڴ�����Ƶ
					fread((void *)pPCM, pcmSize, 1, f_pcm);
					fclose(f_pcm);
					f_pcm = NULL;
				}//��ȡ��Ƶ�ļ�,����pPCM��
				else
				{
					fprintf(out_file,"media %s not found\n",src_wav_filename);
					return Sys_value;
				}

				while (1) {//��ʼ��������д��Ƶ����
				unsigned int len = 6400;
				int ret = 0;
				if (pcmSize < 12800) {
					len = pcmSize;
					lastAudio = 1;//��Ƶ����С��12800
				}
				audStat = MSP_AUDIO_SAMPLE_CONTINUE;//�к����Ƶ
				if (pcmCount == 0)
					audStat = MSP_AUDIO_SAMPLE_FIRST;
				if (len<=0)
				{
					break;
				}
				fprintf(out_file,"csid=%s,count=%d,aus=%d,",sessionID,pcmCount/len,audStat);
				ret = QISRAudioWrite(sessionID, (const void *)&pPCM[pcmCount], len, audStat, &epStatus, &recStatus);//д��Ƶ
				fprintf(out_file,"eps=%d,rss=%d,ret=%d\n",epStatus,recStatus,errCode);
				if (ret != 0)
				break;
				pcmCount += (long)len;
				pcmSize -= (long)len;
				if (recStatus == MSP_REC_STATUS_SUCCESS) {
					const char *rslt = QISRGetResult(sessionID, &recStatus, 0, &errCode);//������Ѿ���ʶ���������Ի�ȡ
					fprintf(out_file,"csid=%s,rss=%d,ret=%d\n",sessionID,recStatus,errCode);
					if (NULL != rslt)
						strcat_s(rec_result,rslt);
				}
				if (epStatus == MSP_EP_AFTER_SPEECH)
					break;
				Sleep(150);//ģ����˵��ʱ���϶
				}
				QISRAudioWrite(sessionID, (const void *)NULL, 0, MSP_AUDIO_SAMPLE_LAST, &epStatus, &recStatus);//д�����
				free(pPCM);
				pPCM = NULL;
				while (recStatus != MSP_REC_STATUS_COMPLETE && 0 == errCode) {
					const char *rslt = QISRGetResult(sessionID, &recStatus, 0, &errCode);//��ȡ���
					fprintf(out_file,"csid=%s,rss=%d,ret=%d\n",sessionID,recStatus,errCode);
					if (NULL != rslt)
					{
						strcat_s(rec_result,rslt);
					}
					Sleep(150);
				}
				QISRSessionEnd(sessionID, NULL);
				fprintf(out_file,"The result is: %s\n",rec_result);
				if(NULL != rec_result)//��Ϊ��ʱ������ȷֵ
				Sys_value = Marshal::PtrToStringAnsi((IntPtr)rec_result);//��ֵת��

				return Sys_value;
			}

			void set_tts_params(System::String^ e_voice_type , System::String^ e_engin , int e_volunm , int e_speed)
			{
				const char* src_text = (const char*)(Marshal::StringToHGlobalAnsi(e_voice_type)).ToPointer();
				voice_type = src_text;
				src_text = (const char*)(Marshal::StringToHGlobalAnsi(e_engin)).ToPointer();
				engin = src_text;
				ostringstream oss1;
				ostringstream oss2;
				oss1<<e_volunm;
				volunm = oss1.str();//����
				oss2<<e_speed;
				voice_speed = oss2.str();//����
			}
			
	};

	public ref class SoundType{
	public: System::String^ engin;//��������
			System::String^ voice_type;//˵������
			System::String^ voice;//��ʾ

			SoundType(System::String^ e_voice)//switch case ��֧��string������
			{
				voice = e_voice;

				if (e_voice == "��ͨ��Ů��") {engin = "intp65";voice_type = "xiaoyan";}
				else if(e_voice == "��ͨ������") {engin = "intp65";voice_type = "xiaoyu";}
				else if(e_voice == "Ӣ��Ů��") {engin = "intp65_en";voice_type = "Catherine";}
				else if(e_voice == "Ӣ������") {engin = "intp65_en";voice_type = "henry";}
				else if(e_voice == "����") {engin = "vivi21";voice_type = "vixm";}
				else if(e_voice == "̨�廰") {engin = "vivi21";voice_type = "vixl";}
				else if(e_voice == "�Ĵ���") {engin = "vivi21";voice_type = "vixr";}
				else if(e_voice == "������") {engin = "vivi21";voice_type = "vixyun";}
				else {engin = "intp65";voice_type = "xiaoyan";voice = "��ͨ��Ů��";}

			}
			SoundType()
			{
				engin = "intp65";voice_type = "xiaoyan";voice = "��ͨ��Ů��";
			}

			virtual System::String^ ToString() override//����ToString����
			{
				return voice;
			}

	};

}