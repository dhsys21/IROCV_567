﻿#pragma link "AdvSmoothButton"
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
	// PLC - IR/OCV
	AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_HEART_BEAT), "PLC HEART BEAT");
	AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_AUTO_MANUAL), "PLC AUTO MANUAL");
	AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_ERROR), "PLC ERROR");

	AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_TRAY_IN), "TRAY IN");
	AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_PROB_OPEN), "PROB OPEN");
	AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_PROB_CLOSE), "PROB CLOSE");
    AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_TRAY_POS), "TRAY POS");
    AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_COMPLETE), "READ COMPLETE");
	AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_TRAY_ID), "IN TRAY BCR DATA");

    //* Cell Serial Data
    AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_CELL_SERIAL_START), "CELL SERIAL WRITE START");
	AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_CELL_SERIAL_COMP), "CELL SERIAL WRITE COMPLETE");
    AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_CELL_SERIAL_COUNT), "CELL SERIAL COUNT");

	// CELL INFO => 1 : YES, 0 : NO
	for(int i = 0; i < LINECOUNT; i++)
	{
		AddListView(ListView_PLC, "D" + IntToStr(PLC_D_INTERFACE_START_DEV_NUM + PLC_D_IROCV_TRAY_CELL_DATA + (i * 2)), "TRAY CELL DATA #" + IntToStr(i + 1));
	}

    //* CELL SERIAL DATA
    AddListView(ListView_PLC, "D" + IntToStr(PLC_D_CELL_SERIAL_NUM + PLC_D_IROCV_CELL_SERIAL_TRAYID), "CELL SERIAL TRAY ID");

    for(int i = 0; i < LINECOUNT; i++)
       AddListView(ListView_PLC, "D" + IntToStr(PLC_D_CELL_SERIAL_NUM + PLC_D_IROCV_CELL_SERIAL + (i * 10 * LINECOUNT)), "CELL SERIAL #" + IntToStr(i * LINECOUNT + 1));
}
//---------------------------------------------------------------------------
void __fastcall TForm_PLCInterface::SetListViewPC()
{
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

    // CELL SERIAL
    AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_CELL_SERIAL_START), "CELL SERIAL READ START");
	AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_CELL_SERIAL_COMP), "CELL SERIAL READ COMPLETE");
    AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_CELLID_BYPASS), "CELL ID BYPASS");

	for(int i = 0; i < LINECOUNT; i++)
		AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_MEASURE_OK_NG + (i * 2)), "IR/OCV OK/NG DATA #" + IntToStr(i + 1));

//	for(int i = 0; i < MAXCHANNEL; i++)
//		AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM1 + PC_D_IROCV_IR_VALUE + i), "IR VALUE #" + IntToStr(i + 1));
//
//	for(int i = 0; i < MAXCHANNEL; i++)
//		AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_START_DEV_NUM2 + PC_D_IROCV_OCV_VALUE + i), "OCV VALUE #" + IntToStr(i + 1));
    for(int i = 0; i < LINECOUNT; i++)
		AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_IR + PC_D_IROCV_IR_VALUE + (i * 2 * LINECOUNT)), "IR VALUE #" + IntToStr(i * LINECOUNT + 1));

	for(int i = 0; i < LINECOUNT; i++)
		AddListView(ListView_PC, "D" + IntToStr(PC_D_INTERFACE_OCV + PC_D_IROCV_OCV_VALUE + (i * 2 * LINECOUNT)), "OCV VALUE #" + IntToStr(i * LINECOUNT + 1));
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
		ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetString(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_TRAY_ID, 10);

        //* CELL SERIAL
        ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetPlcValue(PLC_D_IROCV_CELL_SERIAL_START);
		ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetPlcValue(PLC_D_IROCV_CELL_SERIAL_COMP);
		ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetPlcValue(PLC_D_IROCV_CELL_SERIAL_COUNT);

		AnsiString cell_info;
		for(int i = 0; i < LINECOUNT; i++)
		{
			cell_info = "";
			for(int j = 0; j < LINECOUNT; j++)
			{
				cell_info += Mod_PLC->GetData(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_TRAY_CELL_DATA + (i * 2), j);
			}
			ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = cell_info;
			Label4->Caption += Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_TRAY_CELL_DATA + (i * 2));
		}

        //* CELL SERIAL
        ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetCellSrialTrayId(PLC_D_IROCV_CELL_SERIAL_TRAYID, 10);
        for(int i = 0; i < LINECOUNT; i++)
            ListView_PLC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetCellSrial(PLC_D_IROCV_CELL_SERIAL, i * LINECOUNT, 10);
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

        //* CELL SERIAL
        ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_CELL_SERIAL_START);
        ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_CELL_SERIAL_COMP);
        ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_CELLID_BYPASS);

		AnsiString okng_bin;
		for(int i = 0; i < LINECOUNT; i++)
		{
			okng_bin = "";
			for(int j = 0; j < LINECOUNT; j++)
				okng_bin += Mod_PLC->GetData(Mod_PLC->pc_Interface_Data, PC_D_IROCV_MEASURE_OK_NG + (i * 2), j);

			ListView_PC->Items->Item[index++]->SubItems->Strings[1] = okng_bin;
		}

		// IROCV RESULT IR VALUE
//		for(int i = 0; i < MAXCHANNEL; i++){
//			ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_IR_VALUE + i);
//		}
        for(int i = 0; i < LINECOUNT; i++)
        	ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetIrValue(PC_D_IROCV_IR_VALUE, i * LINECOUNT);

		// IROCV RESULT OCV VALUE
//		for(int i = 0; i < MAXCHANNEL; i++){
//			ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Ocv_Data, PC_D_IROCV_OCV_VALUE + i);
//		}
        for(int i = 0; i < LINECOUNT; i++)
        	ListView_PC->Items->Item[index++]->SubItems->Strings[1] = Mod_PLC->GetOcvValue(PC_D_IROCV_OCV_VALUE, i * LINECOUNT);
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
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, address - 45000, value);
}
//---------------------------------------------------------------------------

void __fastcall TForm_PLCInterface::btnWriteNgValueClick(TObject *Sender)
{
	AnsiString strIrocvNg = editIrOcvNg->Text;
	vector<int> ngchannels = BaseForm->StringToVector(strIrocvNg);

    int ngCount = 0;
	for(int i = 0; i < LINECOUNT; ++i){
        int irocvNg = 0;
		for(int j = 0; j < LINECOUNT; j++)
		{
			int nChannel = i * LINECOUNT + j + 1;
			if(find(ngchannels.begin(), ngchannels.end(), nChannel) != ngchannels.end())
			{
                //* ng -> true
                irocvNg |= 1 << j;
				ngCount++;
			}
		}
        Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_MEASURE_OK_NG + (i * 2), irocvNg);
	}

	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_NG_COUNT, ngCount);
}
//---------------------------------------------------------------------------

void __fastcall TForm_PLCInterface::btnWriteIrOcvValueClick(TObject *Sender)
{
	double ir_base = StringToDouble(editIR->Text, 1);
	double ocv_base = StringToDouble(editOCV->Text, 1);

	// ir value 1 Word
	for(int i = 0; i < MAXCHANNEL; i++)
	{
		double ir = ir_base + (i / 100.0);
		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_IR_VALUE + i, ir);
	}

	for(int i = 0; i < MAXCHANNEL; i++)
	{
		double ocv = ocv_base + (i / 10.0);
		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Ocv_Data, PC_D_IROCV_OCV_VALUE + i, ocv);
	}
}
//---------------------------------------------------------------------------

