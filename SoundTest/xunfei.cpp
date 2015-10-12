/*
 string str("hello");
 const char *strp=str.c_str();    string转const char*
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

//音频头部格式
struct wave_pcm_hdr
{
	char            riff[4];                        // = "RIFF"
	SR_DWORD        size_8;                         // = FileSize - 8
	char            wave[4];                        // = "WAVE"
	char            fmt[4];                         // = "fmt "
	SR_DWORD        dwFmtSize;                      // = 下一个结构体的大小 : 16

	SR_WORD         format_tag;              // = PCM : 1
	SR_WORD         channels;                       // = 通道数 : 1
	SR_DWORD        samples_per_sec;        // = 采样率 : 8000 | 6000 | 11025 | 16000
	SR_DWORD        avg_bytes_per_sec;      // = 每秒字节数 : dwSamplesPerSec * wBitsPerSample / 8
	SR_WORD         block_align;            // = 每采样点字节数 : wBitsPerSample / 8
	SR_WORD         bits_per_sample;         // = 量化比特数: 8 | 16

	char            data[4];                        // = "data";
	SR_DWORD        data_size;                // = 纯数据长度 : FileSize - 44 
} ;

//默认音频头部数据
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
	
	private: FILE* out_file;//输出log文件
			 string appid;
			 int ret;
			 string pcm_path;//存储音频文件的文件名
			 string user;
			 string password;
		     string voice_type;//语言类型
			 string volunm;//音量 0-10
			 string engin;//引擎
			 string voice_speed;//语速0-10
			 
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

			bool Login()//登录
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
				MSPLogout();//退出登录
			}

			int TextToSpeed(System::String^ Ssrc_text)//字符串转音频，音频存放在PCM_SPEED.pcm下
			{
				#pragma region 字符串转音频
				struct wave_pcm_hdr pcmwavhdr = default_pcmwavhdr;
				const char* sess_id = NULL;
				unsigned int text_len = 0;
				char* audio_data = NULL;
				unsigned int audio_len = 0;
				int synth_status = MSP_TTS_FLAG_STILL_HAVE_DATA;
				FILE* fp = NULL;
				string params = "vcn=xiaoyan, spd = 50, vol = 50";//参数可参考可设置参数列表
				ret = -1;//失败
				//参数配置
				params = "vcn=" + voice_type + ", spd = " + voice_speed + ", vol = " + volunm + ", ent = "+engin;
				const char* src_text = (const char*)(Marshal::StringToHGlobalAnsi(Ssrc_text)).ToPointer();

				pcm_path = "PCM_SPEED.pcm";

				fprintf(out_file,"begin to synth source = %s\n",src_text);
				if (NULL == src_text)
				{
					fprintf(out_file,"params is null!\n");
					return ret;
				}
				text_len = strlen(src_text);//获取文本长度
				
				fopen_s(&fp,pcm_path.c_str(),"wb");//打开PCM文件
				if (NULL == fp)
				{
					fprintf(out_file,"open PCM file %s error\n",pcm_path);
					return ret;
				}

				sess_id = QTTSSessionBegin(params.c_str(), &ret);//开始一个会话				
				if ( ret != MSP_SUCCESS )
				{
					fprintf(out_file,"QTTSSessionBegin: qtts begin session failed Error code %d.\n",ret);
					return ret;
				}
				fprintf(out_file,"sess_id = %s\n",sess_id);
				ret = QTTSTextPut(sess_id, src_text, text_len, NULL );//发送txt信息
				if ( ret != MSP_SUCCESS )
				{
					fprintf(out_file,"QTTSTextPut: qtts put text failed Error code %d.\n",ret);
					QTTSSessionEnd(sess_id, "TextPutError");//异常，结束
					return ret;
				}
				fwrite(&pcmwavhdr, sizeof(pcmwavhdr) ,1, fp);//把开始文件写到最前面

				while (1)//循环读取音频文件并存储 
				{
					const void *data = QTTSAudioGet(sess_id, &audio_len, &synth_status, &ret);
					if (NULL != data)
					{
						fwrite(data, audio_len, 1, fp);
						pcmwavhdr.data_size += audio_len;//修正pcm数据的大小
					}
					if (synth_status == MSP_TTS_FLAG_DATA_END || ret != 0) 
						break;
				}//合成状态synth_status取值可参考开发文档

				//修正pcm文件头数据的大小
				pcmwavhdr.size_8 += pcmwavhdr.data_size + 36;

				//将修正过的数据写回文件头部
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

			System::String^ GetPcmName()//获取音频文件路径
			{
				return gcnew String(pcm_path.c_str());
			}
			
			int Play(System::String^ text)//播放音频文件
			{
				if(text == "") return -1;
				SoundPlayer^ player = (gcnew SoundPlayer(text));//音频播放器
				player->SoundLocation = text;
				player->Load();
				player->Play();
				return 0;
			}

			int StartRecord()//开始录音
			{
			
			}

			int EndRecord()//结束录音
			{
			
			}

			System::String^ SpeedToText(System::String^ text)//语音转文字，输入语音文件名，返回文字信息
			{
				System::String^ Sys_value = "No data return";
				const char* src_wav_filename = (const char*)(Marshal::StringToHGlobalAnsi(text)).ToPointer();
				//test = Marshal::PtrToStringAnsi((IntPtr)(char *)src_text);
				//return test;
				char rec_result[1024] = {0};//存放返回结果
				const char *sessionID = NULL;
				FILE *f_pcm = NULL;//
				char *pPCM = NULL;//存放音频文件缓存
				int lastAudio = 0 ;
				int audStat = MSP_AUDIO_SAMPLE_CONTINUE ;
				int epStatus = MSP_EP_LOOKING_FOR_SPEECH;
				int recStatus = MSP_REC_STATUS_SUCCESS ;
				long pcmCount = 0;
				long pcmSize = 0;//音频文件大小
				int errCode = 10 ;
				string param = "sub=iat,auf=audio/L16;rate=16000,aue=speex-wb,ent=sms16k,rst=plain,rse=gb2312";

				fprintf(out_file,"Start iat...\n");
				sessionID = QISRSessionBegin(NULL, param.c_str(), &errCode);//开始一路会话
				fopen_s(&f_pcm,src_wav_filename, "rb");
				if (NULL != f_pcm) {
					fseek(f_pcm, 0, SEEK_END);
					pcmSize = ftell(f_pcm);//获取音频大小
					fseek(f_pcm, 0, SEEK_SET);
					pPCM = (char *)malloc(pcmSize);//分配内存存放音频
					fread((void *)pPCM, pcmSize, 1, f_pcm);
					fclose(f_pcm);
					f_pcm = NULL;
				}//读取音频文件,读到pPCM中
				else
				{
					fprintf(out_file,"media %s not found\n",src_wav_filename);
					return Sys_value;
				}

				while (1) {//开始往服务器写音频数据
				unsigned int len = 6400;
				int ret = 0;
				if (pcmSize < 12800) {
					len = pcmSize;
					lastAudio = 1;//音频长度小于12800
				}
				audStat = MSP_AUDIO_SAMPLE_CONTINUE;//有后继音频
				if (pcmCount == 0)
					audStat = MSP_AUDIO_SAMPLE_FIRST;
				if (len<=0)
				{
					break;
				}
				fprintf(out_file,"csid=%s,count=%d,aus=%d,",sessionID,pcmCount/len,audStat);
				ret = QISRAudioWrite(sessionID, (const void *)&pPCM[pcmCount], len, audStat, &epStatus, &recStatus);//写音频
				fprintf(out_file,"eps=%d,rss=%d,ret=%d\n",epStatus,recStatus,errCode);
				if (ret != 0)
				break;
				pcmCount += (long)len;
				pcmSize -= (long)len;
				if (recStatus == MSP_REC_STATUS_SUCCESS) {
					const char *rslt = QISRGetResult(sessionID, &recStatus, 0, &errCode);//服务端已经有识别结果，可以获取
					fprintf(out_file,"csid=%s,rss=%d,ret=%d\n",sessionID,recStatus,errCode);
					if (NULL != rslt)
						strcat_s(rec_result,rslt);
				}
				if (epStatus == MSP_EP_AFTER_SPEECH)
					break;
				Sleep(150);//模拟人说话时间间隙
				}
				QISRAudioWrite(sessionID, (const void *)NULL, 0, MSP_AUDIO_SAMPLE_LAST, &epStatus, &recStatus);//写入结束
				free(pPCM);
				pPCM = NULL;
				while (recStatus != MSP_REC_STATUS_COMPLETE && 0 == errCode) {
					const char *rslt = QISRGetResult(sessionID, &recStatus, 0, &errCode);//获取结果
					fprintf(out_file,"csid=%s,rss=%d,ret=%d\n",sessionID,recStatus,errCode);
					if (NULL != rslt)
					{
						strcat_s(rec_result,rslt);
					}
					Sleep(150);
				}
				QISRSessionEnd(sessionID, NULL);
				fprintf(out_file,"The result is: %s\n",rec_result);
				if(NULL != rec_result)//不为空时返回正确值
				Sys_value = Marshal::PtrToStringAnsi((IntPtr)rec_result);//数值转换

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
				volunm = oss1.str();//音量
				oss2<<e_speed;
				voice_speed = oss2.str();//语速
			}
			
	};

	public ref class SoundType{
	public: System::String^ engin;//语音引擎
			System::String^ voice_type;//说话类型
			System::String^ voice;//显示

			SoundType(System::String^ e_voice)//switch case 不支持string的输入
			{
				voice = e_voice;

				if (e_voice == "普通话女声") {engin = "intp65";voice_type = "xiaoyan";}
				else if(e_voice == "普通话男声") {engin = "intp65";voice_type = "xiaoyu";}
				else if(e_voice == "英文女声") {engin = "intp65_en";voice_type = "Catherine";}
				else if(e_voice == "英文男声") {engin = "intp65_en";voice_type = "henry";}
				else if(e_voice == "粤语") {engin = "vivi21";voice_type = "vixm";}
				else if(e_voice == "台湾话") {engin = "vivi21";voice_type = "vixl";}
				else if(e_voice == "四川话") {engin = "vivi21";voice_type = "vixr";}
				else if(e_voice == "东北话") {engin = "vivi21";voice_type = "vixyun";}
				else {engin = "intp65";voice_type = "xiaoyan";voice = "普通话女声";}

			}
			SoundType()
			{
				engin = "intp65";voice_type = "xiaoyan";voice = "普通话女声";
			}

			virtual System::String^ ToString() override//重载ToString方法
			{
				return voice;
			}

	};

}