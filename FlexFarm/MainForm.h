//---------------------------------------------------------------------------

#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.WinXCtrls.hpp>
#include <System.Net.HttpClient.hpp>
#include <System.Net.HttpClientComponent.hpp>
#include <System.Net.URLClient.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.ObjectScope.hpp>
#include <REST.Client.hpp>
#include <REST.Types.hpp>

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TPopupMenu *PopupMenu1;
	TMenuItem *Save1;
	TMenuItem *Load1;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TIdTCPClient *client;
	TMainMenu *MainMenu1;
	TMenuItem *Styles1;
	TMenuItem *Glow1;
	TMenuItem *Windows10SlateGray1;
	TMenuItem *Carbon1;
	TMenuItem *CharcoalDarkSlate1;
	TMenuItem *Auric1;
	TMenuItem *Carbon2;
	TMenuItem *CharcoalDarkSlate2;
	TMenuItem *CobaltXEMedia1;
	TMenuItem *CyanDusk1;
	TMenuItem *CyanNight1;
	TMenuItem *EmeraldLightSlate1;
	TMenuItem *Glossy1;
	TMenuItem *Glow2;
	TMenuItem *GoldenGraphite1;
	TMenuItem *IcebergClassico1;
	TMenuItem *Light1;
	TMenuItem *Luna1;
	TMenuItem *MetropolisUI1;
	TMenuItem *Obsidian1;
	TMenuItem *OnyxBlue1;
	TMenuItem *SapphireKamri1;
	TMenuItem *RubyGraphite1;
	TMenuItem *Silver1;
	TMenuItem *Sky1;
	TMenuItem *SlateClassicco1;
	TMenuItem *SmokeyQuartzKamri1;
	TMenuItem *abletLight1;
	TMenuItem *abletDark1;
	TMenuItem *urquosieGray1;
	TMenuItem *Windows101;
	TMenuItem *Windows10Blue1;
	TMenuItem *Windows10Dark1;
	TMenuItem *Windows10Green1;
	TMenuItem *Windows10Purple1;
	TMenuItem *Windows10SlateGray2;
	TMenuItem *MetropolisUIBlack1;
	TMenuItem *MetropolisUIBlue1;
	TMenuItem *MetropolisUIGreen1;
	TMenuItem *MetropolisUIGreen2;
	TMenuItem *About1;
	TMenuItem *Social1;
	TMenuItem *Update1;
	TMenuItem *Youtube1;
	TMenuItem *Discord1;
	TTimer *Timer1;
	TRESTRequest *RESTRequest1;
	TRESTClient *RESTClient1;
	TRESTResponse *RESTResponse1;
	TMainMenu *MainMenu2;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TPanel *Panel1;
	TButton *Button2;
	TCheckBox *CheckBox1;
	TEdit *Edit1;
	TEdit *Edit2;
	TListBox *ListBox1;
	TButton *Button1;
	TPageControl *PageControl2;
	TTabSheet *Control;
	TCheckBox *CheckBox3;
	TCheckBox *CheckBox2;
	TEdit *Edit3;
	TEdit *Edit4;
	TButton *Button8;
	TButton *Button5;
	TButton *Button6;
	TButton *Button4;
	TButton *Button3;
	TCheckBox *CheckBox7;
	TGroupBox *GroupBox1;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *Label12;
	TButton *Button33;
	TCheckBox *CheckBox11;
	TTabSheet *TabSheet3;
	TLabel *Label1;
	TButton *Button9;
	TButton *Button10;
	TButton *Button11;
	TButton *Button12;
	TButton *Button13;
	TButton *Button14;
	TButton *Button15;
	TButton *Button16;
	TButton *Button17;
	TButton *Button18;
	TButton *Button19;
	TButton *Button20;
	TButton *Button31;
	TButton *Button30;
	TButton *Button29;
	TButton *Button28;
	TButton *Button27;
	TButton *Button25;
	TButton *Button26;
	TButton *Button23;
	TButton *Button24;
	TButton *Button22;
	TButton *Button7;
	TButton *Button21;
	TCheckBox *CheckBox4;
	TEdit *Edit5;
	TCheckBox *CheckBox6;
	TCheckBox *CheckBox5;
	TCheckBox *CheckBox10;
	TCheckBox *CheckBox12;
	TTabSheet *TabSheet5;
	TLabel *Label13;
	TLabel *Label14;
	TEdit *Edit10;
	TEdit *Edit11;
	TCheckBox *CheckBox9;
	TCheckBox *CheckBox8;
	TTabSheet *TabSheet6;
	TLabel *Label15;
	TButton *Button34;
	TEdit *Edit12;
	TMemo *Memo1;
	TTabSheet *TabSheet2;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *Edit6;
	TButton *Button32;
	TSearchBox *SearchBox1;
	TListView *ListView1;
	TTabSheet *TabSheet4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TEdit *Edit7;
	TEdit *Edit8;
	TEdit *Edit9;
	TButton *Button35;
	TLabel *Label16;
	TEdit *Edit13;
	TLabel *Label17;
	TButton *Button36;
	TButton *Button37;
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall CheckBox3Click(TObject *Sender);
	void __fastcall Button8Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall CheckBox2Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall CheckBox4Click(TObject *Sender);
	void __fastcall Button15Click(TObject *Sender);
	void __fastcall Button14Click(TObject *Sender);
	void __fastcall Button9Click(TObject *Sender);
	void __fastcall Button10Click(TObject *Sender);
	void __fastcall Button24Click(TObject *Sender);
	void __fastcall Button13Click(TObject *Sender);
	void __fastcall Button16Click(TObject *Sender);
	void __fastcall Button12Click(TObject *Sender);
	void __fastcall Button11Click(TObject *Sender);
	void __fastcall Button29Click(TObject *Sender);
	void __fastcall Button22Click(TObject *Sender);
	void __fastcall Button21Click(TObject *Sender);
	void __fastcall Button18Click(TObject *Sender);
	void __fastcall Button17Click(TObject *Sender);
	void __fastcall Button26Click(TObject *Sender);
	void __fastcall Button25Click(TObject *Sender);
	void __fastcall Button20Click(TObject *Sender);
	void __fastcall Button19Click(TObject *Sender);
	void __fastcall Button30Click(TObject *Sender);
	void __fastcall Button27Click(TObject *Sender);
	void __fastcall Button23Click(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall Button28Click(TObject *Sender);
	void __fastcall Button31Click(TObject *Sender);
	void __fastcall Edit3Change(TObject *Sender);
	void __fastcall PageControl1Exit(TObject *Sender);
	void __fastcall Edit5Change(TObject *Sender);
	void __fastcall Load1Click(TObject *Sender);
	void __fastcall Save1Click(TObject *Sender);
	void __fastcall CheckBox5Click(TObject *Sender);
	void __fastcall CheckBox6Click(TObject *Sender);
	void __fastcall SearchBox1InvokeSearch(TObject *Sender);
	void __fastcall Button32Click(TObject *Sender);
	void __fastcall Style(TObject *Sender);
	void __fastcall ListBox1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall ListBox1KeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CheckBox7Click(TObject *Sender);
	void __fastcall Edit7Change(TObject *Sender);
	void __fastcall Edit8Change(TObject *Sender);
	void __fastcall Edit9Change(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall Button33Click(TObject *Sender);
	void __fastcall Button34Click(TObject *Sender);
	void __fastcall CheckBox8Click(TObject *Sender);
	void __fastcall Edit10Change(TObject *Sender);
	void __fastcall Youtube1Click(TObject *Sender);
	void __fastcall Discord1Click(TObject *Sender);
	void __fastcall Update1Click(TObject *Sender);
	void __fastcall CheckBox10Click(TObject *Sender);
	void __fastcall CheckBox11Click(TObject *Sender);
	void __fastcall Button35Click(TObject *Sender);
	void __fastcall Edit13Change(TObject *Sender);
	void __fastcall Button36Click(TObject *Sender);
	void __fastcall Button37Click(TObject *Sender);



private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);

    void itemchange();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
