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
	TSaveDialog *SaveDialog;
	TOpenDialog *OpenDialog1;
	TStringGrid *StringGrid1;
	TGroupBox *GroupBox3;
	TAdvSmoothButton *AdvSmoothButton1;
	TAdvSmoothButton *btnSave;
	TAdvSmoothButton *btnLoad;
	TAdvSmoothButton *btnApply;
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
	TPanel *Panel1;
	TEdit *ManOffsetEdit;
	TAdvSmoothButton *btnOffset;
	TPanel *Panel36;
	TPanel *ppos;
	TGroupBox *GroupBox6;
	TLabel *Label4;
	TPanel *Panel52;
	TEdit *OffsetEdit;
	TPanel *Panel42;
	TEdit *LowOffsetEdit;
	TAdvSmoothButton *ConfigBtn1;
	TPanel *pnormal3;
	TTimer *probetimer;
	TPanel *pnormal41;
	TPanel *pnormal4;
	TPanel *pnormal31;
	TPanel *Panel3;
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
    void __fastcall PanelDblClick(TObject *Sender);
	void __fastcall probetimerTimer(TObject *Sender);

private:	// User declarations
	void __fastcall MakePanel(AnsiString type);
    void __fastcall SetOption(TPanel *pnl, int nx, int ny, int nw, int nh, int index);
    void __fastcall MakeGrid();
    void __fastcall SetValues(int ch, int pos);
    int __fastcall GetChannel(int col, int row);
    void __fastcall InitColor();

    void __fastcall ApplyClick(int nTag);
    void __fastcall ApplyClick2(int nTag);

    TPanel *pmeasure[MAXCHANNEL];
	TPanel *pstandard[MAXCHANNEL];
	TPanel *pch[MAXCHANNEL];
	TPanel *poffset[MAXCHANNEL];

    TColor clrMeasureArr[MAXCHANNEL];
    TColor clrOffsetArr[MAXCHANNEL];
public:		// User declarations
	__fastcall TCaliForm(TComponent* Owner);
    void __fastcall WriteCaliFile(bool Data);
    void __fastcall WriteCaliFile2(bool Data);
    void __fastcall LoadCaliFile();
    void __fastcall LoadCaliFile2();
    void __fastcall ApplyFile();
    void __fastcall ApplyFile2();
	void __fastcall WriteCalibOffset();
    void __fastcall WriteCalibOffset2();
	void __fastcall ReadCaliboffset();
    void __fastcall ReadCaliboffset2();
    void __fastcall InsertValueToGrid(int pos1, int pos2, double value, TColor clr);
    void __fastcall InsertValueToPanel(int pos1, int pos2, double value, TColor clr);

    int stage;
	double *measure;
	double *standard;
	double *offset;
};
//---------------------------------------------------------------------------
extern PACKAGE TCaliForm *CaliForm;
//---------------------------------------------------------------------------
#endif
