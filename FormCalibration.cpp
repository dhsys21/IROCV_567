//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FormCalibration.h"
#include "Util.h"
#include "RVMO_main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvSmoothButton"
#pragma link "AdvSmoothToggleButton"
#pragma link "AdvSmoothTabPager"
#pragma link "AdvSmoothButton"
#pragma resource "*.dfm"
TCaliForm *CaliForm;
//---------------------------------------------------------------------------
__fastcall TCaliForm::TCaliForm(TComponent* Owner)
	: TForm(Owner)
{
	stage = -1;

	this->Left = 140;
	this->Top = 50;
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::FormCreate(TObject *Sender)
{
    InitColor();
    MakeGrid();
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::FormShow(TObject *Sender)
{
	this->BringToFront();

	OffsetEdit->Text = BaseForm->DefaultOffset[stage];
	LowOffsetEdit->Text = BaseForm->DefaultLowOffset[stage];
	OffsetEdit->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    this->stage = -1;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Method
//---------------------------------------------------------------------------
void __fastcall TCaliForm::WriteCalibOffset()
{
	TIniFile *ini;
	AnsiString file;
	file = (AnsiString)BIN_PATH + "Caliboffset_" + IntToStr(this->stage) + ".cali";

	ini = new TIniFile(file);

	for(int index = 1; index <= MAXCHANNEL; ++index)
	{
        if(index <= 288){
            ini->WriteFloat("STANDARD", IntToStr(index), StringToDouble(StringGrid1->Cells[1][index], 0));
            ini->WriteFloat("MEASURE", IntToStr(index), StringToDouble(StringGrid1->Cells[2][index], 0));
            ini->WriteFloat("IR OFFSET", IntToStr(index), StringToDouble(StringGrid1->Cells[3][index], 0));
        } else{
            ini->WriteFloat("STANDARD", IntToStr(index), StringToDouble(StringGrid1->Cells[5][index - 288], 0));
            ini->WriteFloat("MEASURE", IntToStr(index), StringToDouble(StringGrid1->Cells[6][index - 288], 0));
            ini->WriteFloat("IR OFFSET", IntToStr(index), StringToDouble(StringGrid1->Cells[7][index - 288], 0));
        }
	}

	delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::ReadCaliboffset()
{
	TIniFile *ini;
	ini = new TIniFile((AnsiString)BIN_PATH + "Caliboffset_" + IntToStr(this->stage) + ".cali");

	for(int index = 1; index <= MAXCHANNEL; ++index){
        if(index <= 288){
            StringGrid1->Cells[1][index] = ini->ReadFloat("STANDARD", IntToStr(index), 0.0);
            StringGrid1->Cells[2][index] = ini->ReadFloat("MEASURE", IntToStr(index), 0.0);
            StringGrid1->Cells[3][index] = ini->ReadFloat("IR OFFSET", IntToStr(index), 0.0);
        } else{
            StringGrid1->Cells[5][index - 288] = ini->ReadFloat("STANDARD", IntToStr(index), 0.0);
            StringGrid1->Cells[6][index - 288] = ini->ReadFloat("MEASURE", IntToStr(index), 0.0);
            StringGrid1->Cells[7][index - 288] = ini->ReadFloat("IR OFFSET", IntToStr(index), 0.0);
        }
	}

	delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::WriteCaliFile(bool Data)
{
	AnsiString str, FileName;
	int file_handle;

	if(SaveDialog->Execute() == false){
		return;
	}
	FileName = SaveDialog->FileName ;
	if(FileExists(FileName))DeleteFile(FileName);
	file_handle = FileCreate(FileName);
	FileSeek(file_handle, 0, 0);
	str = "Channel,STANDARD,MEASURE,OFFSET\r\n";
	FileWrite(file_handle, str.c_str(), str.Length());
	for(int i = 1; i <= MAXCHANNEL; ++i){
        if(i <= 288)
        	str = IntToStr(i) + "," + StringGrid1->Cells[1][i] + "," + StringGrid1->Cells[2][i] + ","+ StringGrid1->Cells[3][i] + "\r\n";
        else
            str = IntToStr(i) + "," + StringGrid1->Cells[5][i] + "," + StringGrid1->Cells[6][i] + ","+ StringGrid1->Cells[7][i] + "\r\n";
		FileWrite(file_handle, str.c_str(), str.Length());
	}
	FileClose(file_handle);
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::SetValues(int ch, int pos)
{
    chEdit->Text = pos == 0 ? IntToStr(ch) : IntToStr(ch + 288);
    ManStandardEdit->Text = StringGrid1->Cells[pos + 1][ch];
    ManMeasureEdit->Text = StringGrid1->Cells[pos + 2][ch];
    ManOffsetEdit->Text = StringGrid1->Cells[pos + 3][ch];

    int channel = BaseForm->nForm[stage]->chReverseMap[pos == 0 ? ch : ch + 288];
    if(channel >= 289) channel = channel - 288;
    ppos->Caption = IntToStr((channel - 1)/LINECOUNT + 1) + "-" + IntToStr((channel - 1)%LINECOUNT + 1);
}
//---------------------------------------------------------------------------
int __fastcall TCaliForm::GetChannel(int col, int row)
{
    int channel;

    if(col < 4) channel = row;
    else channel = row + 288;

    return channel;
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::InitColor()
{
    for(int i = 0; i < MAXCHANNEL; i++){
        clrMeasureArr[i] = pnormal2->Color;
        clrOffsetArr[i] = pnormal1->Color;
    }
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::InsertValue(int channel, double value, TColor clr)
{
    //* 0(4) channel, 1(5) standard, 2(6) measure, 3(7) offset

    //* Measure
    if(channel <= 288) StringGrid1->Cells[2][channel - 1] = FormatFloat("0.00", value);
    else StringGrid1->Cells[6][channel - 1 - 288] = FormatFloat("0.00", value);
    //* Offset
    if(channel <= 288) StringGrid1->Cells[3][channel - 1] = StringToDouble(StringGrid1->Cells[1][channel - 1], 0) - value;
    else StringGrid1->Cells[7][channel - 1 - 288] = StringToDouble(StringGrid1->Cells[5][channel - 1 - 288], 0) - value;
    //* Color
    clrMeasureArr[channel - 1] = clr;

    //* StringGrid Refresh
    StringGrid1->Invalidate();
}
//---------------------------------------------------------------------------
// Method
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// Button Event
//---------------------------------------------------------------------------
void __fastcall TCaliForm::btnInitClick(TObject *Sender)
{
	for(int pos = 0; pos <MAXCHANNEL; ++pos){
        clrMeasureArr[pos] = pnormal2->Color;
        clrOffsetArr[pos] = pnormal1->Color;
	}

    //* StringGrid Refresh
    StringGrid1->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::btnStopClick(TObject *Sender)
{
	BaseForm->nForm[stage]->CmdForceStop();
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::btnAuto1Click(TObject *Sender)
{
	BaseForm->nForm[stage]->InitTrayStruct(1);
    BaseForm->nForm[stage]->InitTrayStruct(2);
	for(int i = 1; i <= MAXCHANNEL; i++)
	{
        if(i <= 288){
            StringGrid1->Cells[2][i] = "-";
            StringGrid1->Cells[3][i] = "-";
        } else{
            StringGrid1->Cells[6][i - 288] = "-";
            StringGrid1->Cells[7][i - 288] = "-";
        }
	}

	BaseForm->nForm[stage]->CmdAutoTest();
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::btnIrClick(TObject *Sender)
{
	int ch;
	ch = chEdit->Text.ToInt();
    clrMeasureArr[ch-1] = pnormal2->Color;

	BaseForm->nForm[stage]->CmdIRCell(FormatFloat("000", ch));
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::btnStandardClick(TObject *Sender)
{
	TAdvSmoothToggleButton *btn;
	btn = (TAdvSmoothToggleButton*)Sender;

	int channel = StringToInt(chEdit->Text, 1);
    if(channel > 288) channel = channel - 288;
	switch(btn->Tag){
		case 1:
            StringGrid1->Cells[1][channel] = ManStandardEdit->Text;
            StringGrid1->Cells[5][channel] = ManStandardEdit->Text;
			break;
		case 2:
			StringGrid1->Cells[2][channel] = ManMeasureEdit->Text;
            StringGrid1->Cells[6][channel] = ManMeasureEdit->Text;
			break;
		case 3:
			StringGrid1->Cells[3][channel] = ManOffsetEdit->Text;
            StringGrid1->Cells[7][channel] = ManOffsetEdit->Text;
			break;
		default: break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::btnSaveClick(TObject *Sender)
{
	WriteCaliFile(true);
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::btnLoadClick(TObject *Sender)
{
	AnsiString str;
	TStringList *list = new TStringList;
	if(OpenDialog1->Execute()){
		list->LoadFromFile(OpenDialog1->FileName);
		try{
            for(int index = 1; index <= MAXCHANNEL; ++index){

                str = list->Strings[index];
                str.Delete(1, str.Pos(","));

                //* Standard
                if(index <= 288) StringGrid1->Cells[1][index] = str.SubString(1, str.Pos(",")-1);
                else StringGrid1->Cells[5][index - 288] = str.SubString(1, str.Pos(",")-1);
                str.Delete(1, str.Pos(","));

                //* Measure
                if(index <= 288) StringGrid1->Cells[2][index] = str.SubString(1, str.Pos(",")-1);
                else StringGrid1->Cells[6][index - 288] = str.SubString(1, str.Pos(",")-1);
                str.Delete(1, str.Pos(","));

                //* Offset
                if(index <= 288) StringGrid1->Cells[3][index] = str.Trim();
                else StringGrid1->Cells[7][index - 288] = str.Trim();
            }
        }
		catch(...){
			MessageBox(Handle, L"Is the wrong type of file.", L"", MB_OK|MB_ICONERROR);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::btnApplyClick(TObject *Sender)
{
	for(int i = 1; i <= MAXCHANNEL; i++)
	{
        if(i <= 288) BaseForm->nForm[stage]->stage.ir_offset[i] =  StringToDouble(StringGrid1->Cells[3][i], 0);
        else BaseForm->nForm[stage]->stage.ir_offset[i] =  StringToDouble(StringGrid1->Cells[7][i - 288], 0);
	}
	WriteCalibOffset();
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::ConfigBtn1Click(TObject *Sender)
{
	try{
		BaseForm->DefaultOffset[stage] = OffsetEdit->Text.ToDouble();
		BaseForm->DefaultLowOffset[stage] = LowOffsetEdit->Text.ToDouble();
		BaseForm->WriteDefaultOffset();

	}
	catch(...){
		MessageBox(Handle, L"Please check the offset value.",L"", MB_OK|MB_ICONWARNING);
	}
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::btnProbeCloseClick(TObject *Sender)
{
    Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_PROB_CLOSE, 1);
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::pstageClick(TObject *Sender)
{
	OffsetEdit->Enabled = true;
}
//---------------------------------------------------------------------------
// Button Event
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
// String Grid
//---------------------------------------------------------------------------
void __fastcall TCaliForm::MakeGrid()
{
    StringGrid1->ColCount = 8;   // 열 4개
	StringGrid1->RowCount = (MAXCHANNEL / 2) + 1;   // 데이터 1줄 + 헤더 1줄

    // 고정행과 고정열
    StringGrid1->FixedRows = 1;
    StringGrid1->FixedCols = 0;

    // 행 높이 설정
    StringGrid1->DefaultRowHeight = 20;     // 일반 행 높이
    StringGrid1->RowHeights[0] = 24;        // 제목행 높이

    // 열 너비 설정 (예시)
    StringGrid1->ColWidths[0] = 138;  // "채널번호"
    StringGrid1->ColWidths[1] = 165;  // "STANDARD"
    StringGrid1->ColWidths[2] = 165;  // "MEASURE"
    StringGrid1->ColWidths[3] = 165;  // "OFFSET"

    StringGrid1->ColWidths[4] = 138;  // "채널번호"
    StringGrid1->ColWidths[5] = 165;  // "STANDARD"
    StringGrid1->ColWidths[6] = 165;  // "MEASURE"
    StringGrid1->ColWidths[7] = 165;  // "OFFSET"

    //* 제목
    StringGrid1->Cells[0][0] = "채널번호";
    StringGrid1->Cells[1][0] = "STANDARD";
    StringGrid1->Cells[2][0] = "MEASURE";
    StringGrid1->Cells[3][0] = "OFFSET";
    StringGrid1->Cells[4][0] = "채널번호";
    StringGrid1->Cells[5][0] = "STANDARD";
    StringGrid1->Cells[6][0] = "MEASURE";
    StringGrid1->Cells[7][0] = "OFFSET";

    //* 채널 번호
    for(int i = 1; i <= MAXCHANNEL; i++){
        StringGrid1->Cells[0][i] = IntToStr(i);//"CH " + IntToStr(i);
        StringGrid1->Cells[4][i] = IntToStr(i + 288);//"CH " + IntToStr(i);
    }
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::StringGrid1DrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{
    int channel;
    channel = GetChannel(ACol, ARow);
    TCanvas *canvas = StringGrid1->Canvas;

    //* 가운데 정렬을 위한 텍스트 사이즈 측정
    UnicodeString text = StringGrid1->Cells[ACol][ARow];
    int textWidth = canvas->TextWidth(text);
    int textHeight = canvas->TextHeight(text);

    int x = Rect.Left + (Rect.Width() - textWidth) / 2;
    int y = Rect.Top + (Rect.Height() - textHeight) / 2;

    //* Title, 채널번호 회색
    if (ARow == 0 || ACol == 0 || ACol == 4)
    {
        canvas->Font->Style = TFontStyles() << fsBold; // 굵게
        canvas->Font->Size = 10; // 글자 크기 설정
        StringGrid1->Canvas->Brush->Color = (TColor)0x00C3C3C3;
    }
    else if(ACol == 2 || ACol == 6)
    {
        double value = StringToDouble(text, 0.0);
        if(value == 0) clrMeasureArr[channel - 1] = BaseForm->nForm[stage]->cl_ce->Color;

        canvas->Brush->Color = clrMeasureArr[channel - 1];
    }
    else if(ACol == 3 || ACol == 7)
    {
        canvas->Brush->Color = clrOffsetArr[channel - 1];
    }
    else{
        double value = 0.0;
        value = StringToDouble(text, 0);
    }

    //* 셀 그리기
    StringGrid1->Canvas->FillRect(Rect);
    canvas->TextOut(x, y, text);
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::StringGrid1SelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect)
{
    int Ch = 0;
    if (ARow == 0) {
        CanSelect = false; // 선택 안 되게 할 수도 있음
    }
    else{
        if(ACol > 3) SetValues(ARow, 4);
        else SetValues(ARow, 0);
    }
}

//---------------------------------------------------------------------------
// String Grid
//---------------------------------------------------------------------------
void __fastcall TCaliForm::AdvSmoothButton1Click(TObject *Sender)
{
    TColor clr;
    double value;
    int channel;
    channel = StringToInt(chEdit->Text, 1);
    value = StringToDouble(ManMeasureEdit->Text, 0.0);

    if (value == 0) {
        clr = BaseForm->nForm[stage]->cl_ce->Color;;
    }
    else clr = BaseForm->nForm[stage]->cl_ir->Color;;

    InsertValue(channel, value, clr);
}
//---------------------------------------------------------------------------

