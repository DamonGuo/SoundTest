#pragma once

#using "SoundRecord.dll"
#include "xunfei.cpp"

namespace SoundTest {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace std;
	using namespace VoiceRecord;
	using namespace System::Threading;
	//using namespace System::Runtime::InteropServices;
	/// <summary>
	/// Form1 ժҪ
	///
	/// ����: ������Ĵ�������ƣ�����Ҫ����
	///          ����������������� .resx �ļ��������й���Դ���������ߵ�
	///          ����Դ�ļ��������ԡ�����
	///          �������������˴���Ĺ���
	///          ���ػ���Դ��ȷ������
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{

	private: System::Windows::Forms::Button^  speak_btn;
	private: System::Windows::Forms::Label^  status_lab;
	private: System::Windows::Forms::TextBox^  result_box;
	private: System::Windows::Forms::ComboBox^  voice_type;
	private: System::Windows::Forms::TrackBar^  volunm_bar;
	private: System::Windows::Forms::Label^  volunm_lab;
	private: System::Windows::Forms::TrackBar^  speed_bar;
	private: System::Windows::Forms::Label^  speed_lab;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  tts_status_lab;

	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: �ڴ˴���ӹ��캯������
			//
			this->Text = Application::StartupPath;
			//voice_types = gcnew ArrayList();
			voice_type->Items->Add(gcnew SoundType("��ͨ��Ů��"));
			voice_type->Items->Add(gcnew SoundType("��ͨ������"));
			voice_type->Items->Add(gcnew SoundType("Ӣ��Ů��"));
			voice_type->Items->Add(gcnew SoundType("Ӣ������"));
			voice_type->Items->Add(gcnew SoundType("����"));
			voice_type->Items->Add(gcnew SoundType("̨�廰"));
			voice_type->Items->Add(gcnew SoundType("�Ĵ���"));
			voice_type->Items->Add(gcnew SoundType("������"));
			voice_type->SelectedIndex = 0;//Ĭ�ϵ�һ��
		}

	protected:
		/// <summary>
		/// ������������ʹ�õ���Դ��
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::TextBox^  txt_speak;

	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Button^  play_tts_btn;
	private: System::ComponentModel::IContainer^  components;

	protected: 

	private:
		/// <summary>
		/// ����������������
		/// </summary>
	private: static XunFeiSDK* xunfei;
	private: Thread^ xunfei_thread;
			 static int end_flag;
			 static String^ end_result;
			 ArrayList^ voice_types;
	private: static SoundRecord^ recorder;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����֧������ķ��� - ��Ҫ
		/// ʹ�ô���༭���޸Ĵ˷��������ݡ�
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::Windows::Forms::Timer^  timer1;
			this->txt_speak = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->play_tts_btn = (gcnew System::Windows::Forms::Button());
			this->speak_btn = (gcnew System::Windows::Forms::Button());
			this->status_lab = (gcnew System::Windows::Forms::Label());
			this->result_box = (gcnew System::Windows::Forms::TextBox());
			this->voice_type = (gcnew System::Windows::Forms::ComboBox());
			this->volunm_bar = (gcnew System::Windows::Forms::TrackBar());
			this->volunm_lab = (gcnew System::Windows::Forms::Label());
			this->speed_bar = (gcnew System::Windows::Forms::TrackBar());
			this->speed_lab = (gcnew System::Windows::Forms::Label());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->tts_status_lab = (gcnew System::Windows::Forms::Label());
			timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->volunm_bar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->speed_bar))->BeginInit();
			this->panel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// timer1
			// 
			timer1->Enabled = true;
			timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// txt_speak
			// 
			this->txt_speak->Font = (gcnew System::Drawing::Font(L"��Բ", 10.5F));
			this->txt_speak->Location = System::Drawing::Point(15, 231);
			this->txt_speak->Name = L"txt_speak";
			this->txt_speak->Size = System::Drawing::Size(243, 23);
			this->txt_speak->TabIndex = 1;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"��Բ", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label5->Location = System::Drawing::Point(12, 214);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(119, 14);
			this->label5->TabIndex = 10;
			this->label5->Text = L"����Ҫת��������";
			// 
			// play_tts_btn
			// 
			this->play_tts_btn->Font = (gcnew System::Drawing::Font(L"��Բ", 10.5F));
			this->play_tts_btn->Location = System::Drawing::Point(80, 260);
			this->play_tts_btn->Name = L"play_tts_btn";
			this->play_tts_btn->Size = System::Drawing::Size(75, 23);
			this->play_tts_btn->TabIndex = 11;
			this->play_tts_btn->Text = L"����";
			this->play_tts_btn->UseVisualStyleBackColor = true;
			this->play_tts_btn->Click += gcnew System::EventHandler(this, &Form1::play_tts_btn_Click);
			// 
			// speak_btn
			// 
			this->speak_btn->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->speak_btn->Cursor = System::Windows::Forms::Cursors::Hand;
			this->speak_btn->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->speak_btn->Font = (gcnew System::Drawing::Font(L"��Բ", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->speak_btn->Location = System::Drawing::Point(115, 131);
			this->speak_btn->Name = L"speak_btn";
			this->speak_btn->Size = System::Drawing::Size(117, 76);
			this->speak_btn->TabIndex = 0;
			this->speak_btn->Text = L"�밴ס˵��";
			this->speak_btn->UseVisualStyleBackColor = true;
			this->speak_btn->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::speak_btn_MouseDown);
			this->speak_btn->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::speak_btn_MouseUp);
			// 
			// status_lab
			// 
			this->status_lab->AutoSize = true;
			this->status_lab->Font = (gcnew System::Drawing::Font(L"��Բ", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->status_lab->Location = System::Drawing::Point(149, 223);
			this->status_lab->Name = L"status_lab";
			this->status_lab->Size = System::Drawing::Size(49, 14);
			this->status_lab->TabIndex = 16;
			this->status_lab->Text = L"status";
			// 
			// result_box
			// 
			this->result_box->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->result_box->Font = (gcnew System::Drawing::Font(L"��Բ", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->result_box->Location = System::Drawing::Point(55, 57);
			this->result_box->Name = L"result_box";
			this->result_box->Size = System::Drawing::Size(237, 25);
			this->result_box->TabIndex = 17;
			// 
			// voice_type
			// 
			this->voice_type->Font = (gcnew System::Drawing::Font(L"��Բ", 10.5F));
			this->voice_type->FormattingEnabled = true;
			this->voice_type->Location = System::Drawing::Point(96, 41);
			this->voice_type->Name = L"voice_type";
			this->voice_type->Size = System::Drawing::Size(121, 22);
			this->voice_type->TabIndex = 18;
			// 
			// volunm_bar
			// 
			this->volunm_bar->AllowDrop = true;
			this->volunm_bar->Location = System::Drawing::Point(15, 72);
			this->volunm_bar->Name = L"volunm_bar";
			this->volunm_bar->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->volunm_bar->Size = System::Drawing::Size(45, 104);
			this->volunm_bar->TabIndex = 19;
			this->volunm_bar->Value = 5;
			this->volunm_bar->Scroll += gcnew System::EventHandler(this, &Form1::volunm_bar_Scroll);
			// 
			// volunm_lab
			// 
			this->volunm_lab->AutoSize = true;
			this->volunm_lab->Font = (gcnew System::Drawing::Font(L"��Բ", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->volunm_lab->Location = System::Drawing::Point(12, 179);
			this->volunm_lab->Name = L"volunm_lab";
			this->volunm_lab->Size = System::Drawing::Size(42, 14);
			this->volunm_lab->TabIndex = 20;
			this->volunm_lab->Text = L"���� ";
			// 
			// speed_bar
			// 
			this->speed_bar->Location = System::Drawing::Point(110, 72);
			this->speed_bar->Name = L"speed_bar";
			this->speed_bar->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->speed_bar->Size = System::Drawing::Size(45, 104);
			this->speed_bar->TabIndex = 21;
			this->speed_bar->Value = 5;
			this->speed_bar->Scroll += gcnew System::EventHandler(this, &Form1::speed_bar_Scroll);
			// 
			// speed_lab
			// 
			this->speed_lab->AutoSize = true;
			this->speed_lab->Font = (gcnew System::Drawing::Font(L"��Բ", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->speed_lab->Location = System::Drawing::Point(107, 179);
			this->speed_lab->Name = L"speed_lab";
			this->speed_lab->Size = System::Drawing::Size(35, 14);
			this->speed_lab->TabIndex = 22;
			this->speed_lab->Text = L"�ٶ�";
			// 
			// panel1
			// 
			this->panel1->AllowDrop = true;
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel1->Controls->Add(this->label2);
			this->panel1->Controls->Add(this->result_box);
			this->panel1->Controls->Add(this->speak_btn);
			this->panel1->Controls->Add(this->status_lab);
			this->panel1->Dock = System::Windows::Forms::DockStyle::Right;
			this->panel1->Location = System::Drawing::Point(319, 0);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(336, 341);
			this->panel1->TabIndex = 23;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"��Բ", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label2->Location = System::Drawing::Point(13, 8);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(77, 14);
			this->label2->TabIndex = 18;
			this->label2->Text = L"����ת����";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"��Բ", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(77, 14);
			this->label1->TabIndex = 24;
			this->label1->Text = L"����ת����";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"��Բ", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label3->Location = System::Drawing::Point(12, 44);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(63, 14);
			this->label3->TabIndex = 25;
			this->label3->Text = L"ѡ������";
			// 
			// tts_status_lab
			// 
			this->tts_status_lab->AutoSize = true;
			this->tts_status_lab->Font = (gcnew System::Drawing::Font(L"��Բ", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->tts_status_lab->Location = System::Drawing::Point(93, 299);
			this->tts_status_lab->Name = L"tts_status_lab";
			this->tts_status_lab->Size = System::Drawing::Size(49, 14);
			this->tts_status_lab->TabIndex = 26;
			this->tts_status_lab->Text = L"status";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(655, 341);
			this->Controls->Add(this->tts_status_lab);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->speed_lab);
			this->Controls->Add(this->speed_bar);
			this->Controls->Add(this->volunm_lab);
			this->Controls->Add(this->volunm_bar);
			this->Controls->Add(this->voice_type);
			this->Controls->Add(this->play_tts_btn);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->txt_speak);
			this->DoubleBuffered = true;
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Ŀ¼";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->volunm_bar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->speed_bar))->EndInit();
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

#pragma region �ؼ���������
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
				 xunfei = (new XunFeiSDK());
				 end_flag = 0;
				 if(-1 == xunfei->status())
			  {
				  MessageBox::Show("��ʼ��ʧ��");
				  this->Close();//�رմ���
				  return;				
			  }
				 if(!(xunfei->Login()))
			  {
				  MessageBox::Show("��¼ʧ��");
				  this->Close();//�رմ���
				  return;
			  }
				 volunm_lab->Text = "���� " + volunm_bar->Value;
				 speed_lab->Text = "�ٶ� " + speed_bar->Value;
			 }
	private: System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
				 xunfei->Logout();//�ǳ�
				 delete xunfei;//�����ͷŲŻ������������
				 delete recorder;
			 }

	private: System::Void play_tts_btn_Click(System::Object^  sender, System::EventArgs^  e) {
				// tts_status_lab->Text = "��ת�����ٲ�������";
				 set_xunfei_param();//��������
				 if(-1 == xunfei->TextToSpeed(txt_speak->Text))
				 {
					 MessageBox::Show("ת��ʧ��");
				 }
				 else
				 {
					 xunfei->Play(xunfei->GetPcmName());
				 }		 
			 }

	private: System::Void speak_btn_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 StartRecord();//��ʼ¼���߳�
				 status_lab->Text = "¼����.....";
			 }

	private: System::Void speak_btn_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 status_lab->Text = "����¼����ת����...";
				 xunfei_thread = (gcnew Thread(gcnew ThreadStart(EndRecord)));
				 xunfei_thread->Start();
			 }
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
				 if(1 == end_flag)
				 {
					 end_flag = 0;
					 result_box->Text = end_result;
					 status_lab->Text = "ת������";
				 }
			 }
	private: System::Void volunm_bar_Scroll(System::Object^  sender, System::EventArgs^  e) {
				 volunm_lab->Text = "���� " + volunm_bar->Value;
			 }
	private: System::Void speed_bar_Scroll(System::Object^  sender, System::EventArgs^  e) {
				 speed_lab->Text = "�ٶ� " + speed_bar->Value;
			 }
#pragma endregion 

#pragma region �Զ��庯��
	private: void set_xunfei_param()//Ѷ��������������
			 {
				 SoundType^ sound_type;

				 sound_type = (SoundType^)(voice_type->SelectedItem);//��ȡѡ�еĶ���
				 xunfei->set_tts_params(sound_type->voice_type , sound_type->engin , volunm_bar->Value , speed_bar->Value);		
			 }
	private: static void StartRecord()
			 {
				 recorder = (gcnew SoundRecord());
				 recorder->SetFileName("record.wav");
				 recorder->RecStart();	//��ʼ¼��		  
			 }

	private:static void EndRecord()
			{
				//	String text;
				recorder->RecStop();
				delete recorder;
				end_result = xunfei->SpeedToText("record.wav");//¼����������ʾ����ת�����					
				end_flag = 1;
			}
#pragma endregion 
	};

}

