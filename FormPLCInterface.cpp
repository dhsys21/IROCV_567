#pragma link "AdvSmoothPanel"
#pragma link "AdvSmoothButton"
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RVMO_main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_PLCInterface *Form_PLCInterface;
//---------------------------------------------------------------------------
__fastcall TForm_PLCInterface::TForm_PLCInterface(TComponent* Owner)
	: TForm(Owner)
{
	SetListViewPLC();
	SetListViewPC();

	heart_bit_plc = 0;
	heart_bit_pc = 0;

	heart_bit_plc_old = 0;
	heart_bit_pc_old = 0;

	heart_bit_plc_count = 0;
	heart_bit_pc_count = 0;

	for(int nIndex = 0; nIndex < 1; nIndex++){
		auto_manual_plc[nIndex] = 0;
		auto_manual_plc_old[nIndex] = 0;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::SetListViewPLC()
{
    const int count = chkAllData->Checked ? MAXCHANNEL : LINECOUNT;

    ListView_PLC->Items->BeginUpdate();
    try
    {
        ListView_PLC->Items->Clear();

        // PLC - IR/OCV
        AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_HEART_BEAT), "PLC HEART BEAT");
        AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_AUTO_MANUAL), "PLC AUTO MANUAL");
        AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_ERROR), "PLC ERROR");

        AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_TRAY_IN), "TRAY IN");
        AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_PROB_OPEN), "PROB OPEN");
        AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_PROB_CLOSE), "PROB CLOSE");
        AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_TRAY_POS), "TRAY POS");
        AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_PLC_AUTOMODE), "PLC AUTO MODE");
        AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_TRAY_ID), "IN TRAY BCR DATA");

        // CELL INFO => 1 : YES, 0 : NO
        //* 16 bit * 36
        for(int i = 0; i < 36; i++)
        {
            AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_TRAY_CELL_DATA + i), "TRAY CELL DATA #" + IntToStr(i + 1));
        }

        if(chkAllData->Checked == false){
            for(int i = 0; i < LINECOUNT; i++)
                AddListView(ListView_PLC, "D" + IntToStr(PLC_D_CELL_SERIAL_NUM + PLC_D_IROCV_CELL_SERIAL + (i * 10 * LINECOUNT)), "CELL SERIAL #" + IntToStr(i * LINECOUNT + 1));
        }else{
            for(int i = 0; i < MAXCHANNEL; i++)
                AddListView(ListView_PLC, "D" + IntToStr(PLC_D_CELL_SERIAL_NUM + PLC_D_IROCV_CELL_SERIAL + (i * 10)), "CELL SERIAL #" + IntToStr(i + 1));
        }
    }
    __finally
    {
        ListView_PLC->Items->EndUpdate();
    }

    // 보통 EndUpdate로 충분하지만, 즉시 화면 반영이 필요하면:
    ListView_PLC->Invalidate();
    ListView_PLC->Update();
}
//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::SetListViewPC()
{
    const int count = chkAllData->Checked ? MAXCHANNEL : LINECOUNT;

    ListView_PC->Items->BeginUpdate();
    try
    {
        ListView_PC->Items->Clear();

        // PC - IR/OCV
        AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_HEART_BEAT), "PC HEART BEAT");
        AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_ERROR), "PC ERROR");
        AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_STAGE_AUTO_READY), "STAGE AUTO READY");

        AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_TRAY_OUT), "TRAY OUT");
        AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_PROB_OPEN), "PROB OPEN");
        AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_PROB_CLOSE), "PROB CLOSE");
        AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_MEASURING), "MEASURING");
        AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_COMPLETE1), "COMPLETE1");
        AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_COMPLETE2), "COMPLETE2");
        AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_TRAY_POS_MOVE), "TRAY POS MOVE");

        AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_NG_COUNT), "IROCV NG COUNT");
        AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_IR_MIN), "IROCV IR MIN.");
        AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_IR_MAX), "IROCV IR MAX.");
        AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_OCV_MIN), "IROCV OCV MIN.");
        AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_OCV_MAX), "IROCV OCV MAX.");
        AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_NG_ALARM), "NG ALARM");
        AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_DATA_WRITE), "DATA WRITE COMPLETE");
        AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_RESTART), "IROCV RESTART");

        //* 16 bit * 36
        for(int i = 0; i < 36; i++)
            AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_MEASURE_OK_NG + i), "IR/OCV OK/NG DATA #" + IntToStr(i + 1));

        if(chkAllData->Checked == false){
            for(int i = 0; i < LINECOUNT; i++)
                AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_IR + PC_D_IROCV_IR_VALUE + i * LINECOUNT), "IR VALUE #" + IntToStr(i * LINECOUNT + 1));

            for(int i = 0; i < LINECOUNT; i++)
                AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_OCV + PC_D_IROCV_OCV_VALUE + i  * LINECOUNT), "OCV VALUE #" + IntToStr(i * LINECOUNT + 1));
        } else{
            for(int i = 0; i < MAXCHANNEL; i++)
                AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_IR + PC_D_IROCV_IR_VALUE + i), "IR VALUE #" + IntToStr(i + 1));

            for(int i = 0; i < MAXCHANNEL; i++)
                AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_OCV + PC_D_IROCV_OCV_VALUE + i), "OCV VALUE #" + IntToStr(i + 1));
        }
    }
    __finally
    {
        ListView_PC->Items->EndUpdate();
    }

    // 보통 EndUpdate로 충분하지만, 즉시 화면 반영이 필요하면:
    ListView_PC->Invalidate();
    ListView_PC->Update();
}
//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::AddListView(TListView *list, AnsiString address, AnsiString name)
{
	tempListItem = list->Items->Add();
	tempListItem->Caption = address;
	tempListItem->SubItems->Add(name);
	tempListItem->SubItems->Add("");
}
//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::FormShow(TObject *Sender)
{
	Timer_Update->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::FormClose(TObject *Sender, TCloseAction &Action)
{
	Timer_Update->Enabled = false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::Timer_UpdateTimer(TObject *Sender)
{
	int index;

    Label4->Caption = "CELL INFO. : ";
	if(Mod_PLC->ClientSocket_PLC->Active)	//	PLC 상태
	{
		// PLC - IR/OCV
		index = 0;
        ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_HEART_BEAT);
		ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_AUTO_MANUAL);
		ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_ERROR);
		ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_TRAY_IN);
		ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_PROB_OPEN);
		ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_PROB_CLOSE);
        ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_TRAY_POS);
        ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_PLC_AUTOMODE);
		ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetString(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_TRAY_ID, 10);

		AnsiString cell_info;
        //* 16 bit * 36
		for(int i = 0; i < 36; i++)
		{
			cell_info = "";
			for(int j = 0; j < 16; j++)
			{
				cell_info += Mod_PLC->GetData(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_TRAY_CELL_DATA + i, j);
			}
			ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = cell_info;
			Label4->Caption += Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_TRAY_CELL_DATA + i);
		}

        if(chkAllData->Checked == false){
            for(int i = 0; i < LINECOUNT; i++)
                ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetCellSrial(PLC_D_IROCV_CELL_SERIAL, i * LINECOUNT, 10);
        } else{
            for(int i = 0; i < MAXCHANNEL; i++)
                ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetCellSrial(PLC_D_IROCV_CELL_SERIAL, i, 10);
        }
	}

	if(Mod_PLC->ClientSocket_PC->Active)	//	PC 상태
	{
		// PC - IR/OCV
		index = 0;
		ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_HEART_BEAT);
		ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_ERROR);
		ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_STAGE_AUTO_READY);
		ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_TRAY_OUT);
		ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_PROB_OPEN);
		ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_PROB_CLOSE);
		ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_MEASURING);
		ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_COMPLETE1);
        ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_COMPLETE2);
        ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_TRAY_POS_MOVE);

		ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_NG_COUNT);
		ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_IR_MIN);
		ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_IR_MAX);
		ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_OCV_MIN);
		ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_OCV_MAX);
		ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_NG_ALARM);
        ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_DATA_WRITE);
        ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_RESTART);

		AnsiString okng_bin;
        //* 16 bit * 36
		for(int i = 0; i < 36; i++)
		{
			okng_bin = "";
			for(int j = 0; j < 16; j++)
				okng_bin += Mod_PLC->GetData(Mod_PLC->pc_Interface_Data, PC_D_IROCV_MEASURE_OK_NG + i, j);

			ListView_PC->Items->Item[index++]->SubItems->Strings[1] = okng_bin;
		}

        if(chkAllData->Checked == false){
            // IROCV RESULT IR VALUE
            for(int i = 0; i < LINECOUNT; i++)
                ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetIrValue(PC_D_IROCV_IR_VALUE, i * LINECOUNT);

            // IROCV RESULT OCV VALUE
            for(int i = 0; i < LINECOUNT; i++)
                ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetOcvValue(PC_D_IROCV_OCV_VALUE, i * LINECOUNT);
        } else{
            // IROCV RESULT IR VALUE
            for(int i = 0; i < MAXCHANNEL; i++)
                ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetIrValue(PC_D_IROCV_IR_VALUE, i);

            // IROCV RESULT OCV VALUE
            for(int i = 0; i < MAXCHANNEL; i++)
                ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetOcvValue(PC_D_IROCV_OCV_VALUE, i);
        }
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::AdvSmoothButton_CloseClick(TObject *Sender)
{
    Timer_Update->Enabled = false;
	this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::GroupBox_PLC_PCDblClick(TObject *Sender)
{
    Panel1->Visible = !Panel1->Visible;
}
//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::btnWriteValueClick(TObject *Sender)
{
	int address = cbAddress->Text.ToIntDef(45000);
	int value = editValue->Text.ToIntDef(1);
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, address - 38000, value);
}
//---------------------------------------------------------------------------

void __fastcall TForm_PLCInterface::btnWriteNgValueClick(TObject *Sender)
{
	AnsiString strIrocvNg = editIrOcvNg->Text;
	vector<int> ngchannels = BaseForm->StringToVector(strIrocvNg);
    int stageno = 0;
    //* 해당 채널 IR/OCV 값을 spec보다 크거나 작게
    int ngCount = 0;
    //* 16 bit * 36
	for(int i = 0; i < 36; ++i){
        int irocvNg = 0;
		for(int j = 0; j < 16; j++)
		{
			int nChannel = i * 16 + j + 1;
			if(find(ngchannels.begin(), ngchannels.end(), nChannel) != ngchannels.end())
			{
                //* ng -> true
                irocvNg |= 1 << j;
				ngCount++;
                BaseForm->nForm[stageno]->tray.after_value[nChannel - 1] = 999;
                BaseForm->nForm[stageno]->tray.ocv_value[nChannel - 1] = 0;
			}
            else
            {
                BaseForm->nForm[stageno]->tray.after_value[nChannel - 1] = 45.0;
                BaseForm->nForm[stageno]->tray.ocv_value[nChannel - 1] = 2200;
            }
		}
        Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_MEASURE_OK_NG + i, irocvNg);
	}

	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_NG_COUNT, ngCount);

    //*
    BaseForm->nForm[stageno]->tray.first = true;
    BaseForm->nForm[stageno]->SetRemeasureList(1);
    BaseForm->nForm[stageno]->tray.first = true;
    BaseForm->nForm[stageno]->SetRemeasureList(2);
    BaseForm->nForm[stageno]->WriteRemeasureInfo();
}
//---------------------------------------------------------------------------

void __fastcall TForm_PLCInterface::btnWriteIrOcvValueClick(TObject *Sender)
{
	Mod_PLC->PLC_Write_Result = true;
	double ir_base = StringToDouble(editIR->Text, 1);
	double ocv_base = StringToDouble(editOCV->Text, 1);

	// ir value 2 Word
	// 2 Word :  value / (65536 / 2) => 윗 주소에 쓰기, value % (65536 /2 ) => 아래 주소에 쓰기 // herald 2017 11 30
	for(int i = 0; i < MAXCHANNEL; i++)
	{
        int32_t ir_int;
        if(ir_base == 999) ir_int = static_cast<int32_t>(ir_base * 10) + i;  // signed 32-bit int
        else ir_int = static_cast<int32_t>(ir_base * 100.0) + i;  // signed 32-bit int
        Mod_PLC->SetIrValue(PC_D_IROCV_IR_VALUE, i, ir_int);
	}

	for(int i = 0; i < MAXCHANNEL; i++)
	{
		int32_t ocv_int = static_cast<int32_t>(ocv_base * 10.0) + i;  // signed 32-bit int
        Mod_PLC->SetOcvValue(PC_D_IROCV_OCV_VALUE, i, ocv_int);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm_PLCInterface::chkAllDataClick(TObject *Sender)
{
    SetListViewPLC();
	SetListViewPC();
}
//---------------------------------------------------------------------------

