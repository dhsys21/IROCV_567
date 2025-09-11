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
    //MakeGrid();
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::FormShow(TObject *Sender)
{
	this->BringToFront();

	OffsetEdit->Text = BaseForm->DefaultOffset[stage];
	LowOffsetEdit->Text = BaseForm->DefaultLowOffset[stage];
	OffsetEdit->Enabled = false;

    //* 판넬 만들기
    pBase->ControlStyle = pBase->ControlStyle << csOpaque; // 깜빡임 줄이기
	pBase->DisableAlign();

    MakePanel(BaseForm->lblLineNo->Caption);

    pBase->EnableAlign();
    pBase->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    this->stage = -1;
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::MakePanel(AnsiString type)
{
	int nx, ny, nw, nh;

    //* type 4 외에 다른 것은 코드 수정해야 함
	if(type == "4")
	{
		nx = 1462;
		ny = 4;
		nw = 30;
		nh = 20;

		int cnt = 0;
		for(int index = 0; index < MAXCHANNEL;){
			pch[index] = new TPanel(this);
			SetOption(pch[index], nx, ny, nw, nh-1, index);
			pch[index]->Caption = pch[index]->Hint;
			pch[index]->Color = clSkyBlue;
			pch[index]->ParentBackground = false;

			nx = nx + nw + 1;
			pstandard[index] = new TPanel(this);
            SetOption(pstandard[index], nx, ny, nw, nh, index);
            pstandard[index]->Color = clWhite;
            pstandard[index]->ParentBackground = false;

			ny = ny + nh;
			nx = pch[index]->Left;
			poffset[index] = new TPanel(this);
			SetOption(poffset[index], nx, ny, nw, nh, index);
			poffset[index]->Color = pnormal2->Color;
			poffset[index]->ParentBackground = false;
			poffset[index]->Caption = "0";
			poffset[index]->ParentBackground = false;

			nx = nx + nw + 1;
			pmeasure[index] = new TPanel(this);
			SetOption(pmeasure[index], nx, ny, nw, nh, index);
			pmeasure[index]->Color = pnormal3->Color;
			pmeasure[index]->ParentBackground = false;
			pmeasure[index]->Caption = "0.0";
			pmeasure[index]->ParentBackground = false;

            index += 1;
			nx = nx - 3 * nw - 3;
			ny = ny - nh;
			if(index % 4 == 0) nx -= 4;
            if((index - 1) % 4 == 1) nx -= 2;

			if(index % 24 == 0)
			{
				ny = ny + nh + nh + 1;
				nx = 1462;
				if( (index / 24) % 12 == 0) ny += 3;
			}

		}
	}
    else if(type == "3")
	{
		nx = 4;
//		nx = 1300;
		ny = 600;
//		ny = 4;
//		nw = 37;
		nw = 30;
//		nh = 25;
		nh = 20;

		int cnt = 0;
		for(int index = MAXCHANNEL; index >= 0;){
			pch[index] = new TPanel(this);
			SetOption(pch[index], nx, ny, nw, nh-1, index);
			pch[index]->Caption = pch[index]->Hint;
			pch[index]->Color = clSkyBlue;
			pch[index]->ParentBackground = false;

			nx = nx + nw + 1;
			pstandard[index] = new TPanel(this);
            SetOption(pstandard[index], nx, ny, nw, nh, index);
            pstandard[index]->Color = clWhite;
            pstandard[index]->ParentBackground = false;


			ny = ny + nh;
			nx = pch[index]->Left;
			poffset[index] = new TPanel(this);
			SetOption(poffset[index], nx, ny, nw, nh, index);
			poffset[index]->Color = pnormal2->Color;
			poffset[index]->ParentBackground = false;
			poffset[index]->Caption = "0";
			poffset[index]->ParentBackground = false;

			nx = nx + nw + 1;
			pmeasure[index] = new TPanel(this);
			SetOption(pmeasure[index], nx, ny, nw, nh, index);
			pmeasure[index]->Color = pnormal2->Color;
			pmeasure[index]->ParentBackground = false;
			pmeasure[index]->Caption = "0.00";
			pmeasure[index]->ParentBackground = false;


			nx = nx + nw + 3;
//			nx = nx - nw - 3;
			ny = ny - nh;
//			ny = ny + nh;
			if(index % 2 == 0) nx += 2;
//			if(index % 2 == 0) nx -= 2;
			if(index % 20 == 0)
			{
				ny = ny - nh - nh  - 1;
//				ny = ny + nh + nh + 1;
				nx = 4;
//				nx = 1300;
				if( (index / 20) % 10 == 0) ny -= 3;
//				if( (index / 20) % 10 == 0) ny += 3;
			}
			index -= 1;
		}
	}
	else if(type == "6")
	{
		nx = 1200;
		ny = 600;
		nw = 30;
		nh = 20;

		int cnt = 0;
		for(int index = 0; index < MAXCHANNEL;){
			pch[index] = new TPanel(this);
			SetOption(pch[index], nx, ny, nw, nh-1, index);
			pch[index]->Caption = pch[index]->Hint;
			pch[index]->Color = clSkyBlue;
			pch[index]->ParentBackground = false;

			nx = nx - nw - 1;
			pstandard[index] = new TPanel(this);
            SetOption(pstandard[index], nx, ny, nw, nh, index);
            pstandard[index]->Color = clWhite;
            pstandard[index]->ParentBackground = false;


			ny = ny + nh;
			nx = pch[index]->Left;
			poffset[index] = new TPanel(this);
			SetOption(poffset[index], nx, ny, nw, nh, index);
			poffset[index]->Color = pnormal2->Color;
			poffset[index]->ParentBackground = false;
			poffset[index]->Caption = "0";
			poffset[index]->ParentBackground = false;

			nx = nx - nw - 1;
			pmeasure[index] = new TPanel(this);
			SetOption(pmeasure[index], nx, ny, nw, nh, index);
			pmeasure[index]->Color = pnormal2->Color;
			pmeasure[index]->ParentBackground = false;
			pmeasure[index]->Caption = "0.00";
			pmeasure[index]->ParentBackground = false;


			index += 1;
			nx = nx - nw - 3;
			ny = ny - nh;
			if(index % 2 == 0) nx -= 2;
			if(index % 20 == 0)
			{
				ny = ny - nh - nh  - 1;
				nx = 1200;
				if( (index / 20) % 4 == 0) ny -= 3;
			}
		}
	}
    else if(type == "1" || type == "2")
	{
		nx = 4;
		ny = 600;
		nw = 30;
		nh = 20;

		int cnt = 0;
		for(int index = 0; index < MAXCHANNEL;){
			pch[index] = new TPanel(this);
			SetOption(pch[index], nx, ny, nw, nh-1, index);
			pch[index]->Caption = pch[index]->Hint;
			pch[index]->Color = clSkyBlue;
			pch[index]->ParentBackground = false;

			nx = nx + nw + 1;
			pstandard[index] = new TPanel(this);
            SetOption(pstandard[index], nx, ny, nw, nh, index);
            pstandard[index]->Color = clWhite;
            pstandard[index]->ParentBackground = false;


			ny = ny + nh;
			nx = pch[index]->Left;
			poffset[index] = new TPanel(this);
			SetOption(poffset[index], nx, ny, nw, nh, index);
			poffset[index]->Color = pnormal2->Color;
			poffset[index]->ParentBackground = false;
			poffset[index]->Caption = "0";
			poffset[index]->ParentBackground = false;

			nx = nx + nw + 1;
			pmeasure[index] = new TPanel(this);
			SetOption(pmeasure[index], nx, ny, nw, nh, index);
			pmeasure[index]->Color = pnormal2->Color;
			pmeasure[index]->ParentBackground = false;
			pmeasure[index]->Caption = "0.00";
			pmeasure[index]->ParentBackground = false;


			index += 1;
			nx = nx + nw + 3;
			ny = ny - nh;
			if(index % 2 == 0) nx += 2;
			if(index % 20 == 0)
			{
				ny = ny - nh - nh  - 1;
				nx = 4;
				if( (index / 20) % 4 == 0) ny -= 3;
			}
		}
	}
    else if(type == "5")
	{
		nx = 1200;
		ny = 600;
		nw = 30;
		nh = 20;

		int cnt = 0;
		for(int index = 0; index < MAXCHANNEL;){
			pch[index] = new TPanel(this);
			SetOption(pch[index], nx, ny, nw, nh-1, index);
			pch[index]->Caption = pch[index]->Hint;
			pch[index]->Color = clSkyBlue;
			pch[index]->ParentBackground = false;

			nx = nx - nw - 1;
			pstandard[index] = new TPanel(this);
            SetOption(pstandard[index], nx, ny, nw, nh, index);
            pstandard[index]->Color = clWhite;
            pstandard[index]->ParentBackground = false;


			ny = ny + nh;
			nx = pch[index]->Left;
			poffset[index] = new TPanel(this);
			SetOption(poffset[index], nx, ny, nw, nh, index);
			poffset[index]->Color = pnormal2->Color;
			poffset[index]->ParentBackground = false;
			poffset[index]->Caption = "0";
			poffset[index]->ParentBackground = false;

			nx = nx - nw - 1;
			pmeasure[index] = new TPanel(this);
			SetOption(pmeasure[index], nx, ny, nw, nh, index);
			pmeasure[index]->Color = pnormal2->Color;
			pmeasure[index]->ParentBackground = false;
			pmeasure[index]->Caption = "0.00";
			pmeasure[index]->ParentBackground = false;


			index += 1;
            nx = pch[index-1]->Left;
            ny = ny - 3 * nh - 1;
            //ny = ny - nh - nh  - 1;
			//nx = nx + nw + 3;
			//ny = ny - nh;
			//if(index % 2 == 0) nx += 2;
			if(index % 20 == 0)
			{
                nx = nx - 2 * nw - 3;
                ny = 600;
                //nx = nx + nw + 3;
				//nx = 4;
				//if( (index / 20) % 4 == 0) ny -= 3;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::SetOption(TPanel *pnl, int nx, int ny, int nw, int nh, int index)
{
	pnl->Parent = pBase;
	pnl->Left =  nx;
	pnl->Top = ny;
	pnl->Width = nw;
	pnl->Height = nh;
	pnl->Alignment = taCenter;
	pnl->Font->Size = 8;
	pnl->Font->Color = clBlack;
	pnl->ParentBackground=false;
	pnl->Color = clSkyBlue;
	pnl->OnClick = PanelDblClick;
	pnl->BevelInner = bvNone;
	pnl->BevelKind = bkNone;
	pnl->BevelOuter = bvNone;
	pnl->Tag = index;
	pnl->Hint = IntToStr((index/24)+1) + "-" + IntToStr((index+24) % 24 + 1);
	pnl->ShowHint = false;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Method
//---------------------------------------------------------------------------
void __fastcall TCaliForm::WriteCalibOffset()
{
	//TIniFile *ini;
    TMemIniFile *ini;
	AnsiString file;
	file = (AnsiString)BIN_PATH + "Caliboffset_" + IntToStr(this->stage) + ".cali";

	//ini = new TIniFile(file);
    ini = new TMemIniFile(file);

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

    ini->UpdateFile();
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
	for(int i = 0; i < MAXCHANNEL; ++i){
        str = IntToStr(i) + "," + pstandard[i]->Caption + "," + pmeasure[i]->Caption + "," + poffset[i]->Caption + "\r\n";
		FileWrite(file_handle, str.c_str(), str.Length());
	}
	FileClose(file_handle);
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::WriteCaliFile2(bool Data)
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
void __fastcall TCaliForm::InsertValueToPanel(int pos1, int pos2, double value, TColor clr)
{
    //* 보정은 트레이 이동해서 할 필요 없음. 픽스쳐 보드 채널갯수가 288개. 1번 찍고 2개채널에 값 입력.
    //* (0, 4) channel, (1,5) standard, (2,6) measure, (3,7) offset

    int index1 = pos1 - 1;
    int index2 = pos2 - 1;

    //* Measure
    pmeasure[index1]->Caption = FormatFloat("0.0", value);
//    if(index1 <= 288) pmeasure[index1]->Caption = FormatFloat("0.00", value);
//    else if(index1 > 288) pmeasure[index1 - 288]->Caption = FormatFloat("0.00", value);
    pmeasure[index2]->Caption = FormatFloat("0.0", value);
//    if(index2 <= 288) StringGrid1->Cells[2][index2] = FormatFloat("0.00", value);
//    else if(index2 > 288) StringGrid1->Cells[6][index2 - 288] = FormatFloat("0.00", value);

    //* Offset
	double offset1, offset2;
    offset1 = StringToDouble(pstandard[index1]->Caption, 0) - value;
    offset2 = StringToDouble(pstandard[index2]->Caption, 0) - value;

    poffset[index1]->Caption = FormatFloat("0.0", offset1);
    poffset[index2]->Caption = FormatFloat("0.0", offset2);

    //* Color
    clrMeasureArr[index1] = clr;
    clrMeasureArr[index2] = clr;
}
//---------------------------------------------------------------------------
void __fastcall TCaliForm::InsertValue(int pos1, int pos2, double value, TColor clr)
{
    //* 보정은 트레이 이동해서 할 필요 없음. 픽스쳐 보드 채널갯수가 288개. 1번 찍고 2개채널에 값 입력.
    //* (0, 4) channel, (1,5) standard, (2,6) measure, (3,7) offset

    int index1 = pos1;
    int index2 = pos2;

    //* Measure
    if(index1 <= 288) StringGrid1->Cells[2][index1] = FormatFloat("0.00", value);
    else if(index1 > 288) StringGrid1->Cells[6][index1 - 288] = FormatFloat("0.00", value);

    if(index2 <= 288) StringGrid1->Cells[2][index2] = FormatFloat("0.00", value);
    else if(index2 > 288) StringGrid1->Cells[6][index2 - 288] = FormatFloat("0.00", value);

    //* Offset
	double offset1, offset2;
	if(index1 <= 288)  {
		offset1 = StringToDouble(StringGrid1->Cells[1][index1], 0) - value;
		StringGrid1->Cells[3][index1] = FormatFloat("0.0", offset1);
	} else if(index1 > 288) {
		offset2 = StringToDouble(StringGrid1->Cells[5][index1 - 288], 0) - value;
		StringGrid1->Cells[7][index1 - 288] = FormatFloat("0.0", offset2);
	}

	if(index2 <= 288){
		offset1 = StringToDouble(StringGrid1->Cells[1][index2], 0) - value;
		StringGrid1->Cells[3][index2] = FormatFloat("0.0", offset1);
	} else if(index2 > 288){
		offset2 = StringToDouble(StringGrid1->Cells[5][index2 - 288], 0) - value;
        StringGrid1->Cells[7][index2 - 288] = FormatFloat("0.0", offset2);
	}

    //* Color
    clrMeasureArr[index1 - 1] = clr;
    clrMeasureArr[index2 - 1] = clr;

    //* StringGrid Refresh
    StringGrid1->Invalidate();
}
//---------------------------------------------------------------------------
// Method
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Panel Event
//---------------------------------------------------------------------------
void __fastcall TCaliForm::PanelDblClick(TObject *Sender)
{
	TPanel *pnl;
	pnl = (TPanel*)Sender;
	chEdit->Text = IntToStr(pnl->Tag+1);
	ManMeasureEdit->Text = pmeasure[pnl->Tag]->Caption;
	ManOffsetEdit->Text = poffset[pnl->Tag]->Caption;
	ManStandardEdit->Text = pstandard[pnl->Tag]->Caption;
}
//------------------------------------------------------------------
//---------------------------------------------------------------------------
// Button Event
//---------------------------------------------------------------------------
void __fastcall TCaliForm::btnInitClick(TObject *Sender)
{
	for(int pos = 0; pos <MAXCHANNEL; ++pos){
        clrMeasureArr[pos] = pnormal2->Color;
        clrOffsetArr[pos] = pnormal1->Color;

        if(pos <= 288){
            StringGrid1->Cells[2][pos] = "-";
            StringGrid1->Cells[3][pos] = "-";
        } else{
            StringGrid1->Cells[6][pos - 288] = "-";
            StringGrid1->Cells[7][pos - 288] = "-";
        }
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

    int boardch = 0, traypos = 0;;
    if(channel % 4 == 1 || channel % 4 == 2) traypos = 1;
    else if(channel % 4 == 3 || channel % 4 == 0) traypos = 2;
    boardch = BaseForm->nForm[stage]->chReverseMap[channel];// - ((traypos - 1) * 288);

    int pos1, pos2;
    pos1 = BaseForm->nForm[stage]->chMap[boardch];
    pos2 = BaseForm->nForm[stage]->chMap[boardch + 288];

    InsertValueToPanel(pos1, pos2, value, clr);
}
//---------------------------------------------------------------------------


