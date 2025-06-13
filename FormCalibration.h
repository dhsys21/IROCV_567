//---------------------------------------------------------------------------

#ifndef FormCalibrationH
#define FormCalibrationH
//---------------------------------------------------------------------------
#include "DEFINE.h"
#include "AdvSmoothButton.hpp"
#include "AdvSmoothToggleButton.hpp"
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.StdCtrls.hpp>
#include <map>
#include <Vcl.Forms.hpp>
#include <Buttons.hpp>
#include <Vcl.Imaging.jpeg.hpp>
//---------------------------------------------------------------------------
class TCaliForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *pstage;
	TPanel *Panel20;
	TPanel *pBase;
	TPanel *pnormal1;
	TPanel *pnormal2;
	TAdvSmoothButton *btnApply;
	TAdvSmoothButton *btnSave;
	TAdvSmoothButton *btnLoad;
	TAdvSmoothButton *btnInit;
	TGroupBox *GroupBox1;
	TPanel *Panel2;
	TAdvSmoothButton *btnProbeClose;
	TAdvSmoothButton *btnAuto1;
	TAdvSmoothButton *btnStop;
	TGroupBox *GroupBox4;
	TEdit *chEdit;
	TPanel *Panel37;
	TAdvSmoothButton *btnIr;
	TGroupBox *GroupBox2;
	TEdit *ManMeasureEdit;
	TPanel *Panel44;
	TEdit *ManStandardEdit;
	TPanel *Panel46;
	TAdvSmoothButton *btnMeasure;
	TAdvSmoothButton *btnStandard;
	TGroupBox *GroupBox6;
	TLabel *Label4;
	TPanel *Panel52;
	TEdit *OffsetEdit;
	TPanel *Panel42;
	TEdit *LowOffsetEdit;
	TAdvSmoothButton *ConfigBtn1;
	TSaveDialog *SaveDialog;
	TOpenDialog *OpenDialog1;
	TPanel *Panel1;
	TEdit *ManOffsetEdit;
	TAdvSmoothButton *btnOffset;
	TStringGrid *StringGrid1;
	TAdvSmoothButton *AdvSmoothButton1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnStopClick(TObject *Sender);
	void __fastcall btnAuto1Click(TObject *Sender);
	void __fastcall btnIrClick(TObject *Sender);
	void __fastcall pstageClick(TObject *Sender);
	void __fastcall btnLoadClick(TObject *Sender);
	void __fastcall btnSaveClick(TObject *Sender);
	void __fastcall btnInitClick(TObject *Sender);
	void __fastcall btnStandardClick(TObject *Sender);
	void __fastcall ConfigBtn1Click(TObject *Sender);
	void __fastcall btnApplyClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall btnProbeCloseClick(TObject *Sender);
	void __fastcall StringGrid1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall StringGrid1SelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall AdvSmoothButton1Click(TObject *Sender);


private:	// User declarations
	void __fastcall MakePanel(AnsiString type);
    void __fastcall MakeGrid();
    void __fastcall SetValues(int ch, int pos);
    int __fastcall SetPos(int channel);
    int __fastcall SetCh(int channel);
    int __fastcall GetChannel(int col, int row);
    void __fastcall InitColor();

    TColor clrMeasureArr[MAXCHANNEL];
    TColor clrOffsetArr[MAXCHANNEL];
public:		// User declarations
	__fastcall TCaliForm(TComponent* Owner);
    void __fastcall WriteCaliFile(bool Data);
	void __fastcall WriteCaliboffset();
	void __fastcall ReadCaliboffset();
    void __fastcall InsertValue(int channel, double value, TColor clr);

    int stage;
	double *measure;
	double *standard;
	double *offset;
};
//---------------------------------------------------------------------------
extern PACKAGE TCaliForm *CaliForm;
//---------------------------------------------------------------------------
#endif
