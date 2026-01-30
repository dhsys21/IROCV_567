#include "FormTotal.h"
#include "RVMO_main.h"

// 환경설정 파일 저장 / 읽기
void __fastcall TTotalForm::WriteSystemInfo()
{
	TIniFile *ini;

	AnsiString file;
	file = (AnsiString)BIN_PATH + "SystemInfo_"+ IntToStr(this->Tag) + ".inf";

	ini = new TIniFile(file);

	ini->WriteBool("MAIN", "AUTO_CHECK", RemeasureChk->Checked);
	ini->WriteInteger("MAIN", "REMEASURE", RemeasureEdit->Text.ToIntDef(0));
	ini->WriteBool("MAIN", "REM_BYPASS", chkRemBypass->Checked);
    ini->WriteInteger("MAIN", "REMEASURE_ALARM_COUNT", editRemeasureAlarmCount->Text.ToIntDef(3));
    ini->WriteString("NG_ALARM_COUNT", "COUNT", editNgAlarmCount->Text);

	ini->WriteString("IROCV_PLC", "IP", editPLCIPAddress->Text);
	ini->WriteString("IROCV_PLC", "PORT1", editPLCPortPC->Text);
	ini->WriteString("IROCV_PLC", "PORT2", editPLCPortPLC->Text);

	ini->WriteString("IROCV", "IP", editIROCVIPAddress->Text);
	ini->WriteString("IROCV", "PORT", editIROCVPort->Text);

    ini->WriteString("CELL_INFO", "CELL_MODEL", editCellModel->Text);
	ini->WriteString("CELL_INFO", "LOT_NUMBER", editLotNumber->Text);

	config.remeasure_use = RemeasureChk->Checked;
	config.remeasure_cnt = RemeasureEdit->Text.ToIntDef(1);
	config.remeasure_alarm_cnt = editRemeasureAlarmCount->Text.ToIntDef(3);
    RemeasureForm->pcolor2->Caption = config.remeasure_alarm_cnt;
	config.remeasure_bypass = chkRemBypass->Checked;

    ini->WriteString("CELLINFO", "MODELNAME", editModelName->Text);
    ini->WriteString("PASSWORD", "PWD", editPwd->Text);

	//* 최대/최소 대신에 평균값 +/- 범위로 변경
	//* 2022 11 07
	ini->WriteFloat("MAIN", "IR_RANGE", StringToDouble(editIrRange->Text, 5));
	ini->WriteFloat("MAIN", "OCV_RANGE", StringToDouble(editOcvRange->Text, 1000));

	ini->WriteFloat("MAIN", "IR1", StringToDouble(irEdit1->Text, 12));
	ini->WriteFloat("MAIN", "IR2", StringToDouble(irEdit2->Text, 20));
	ini->WriteFloat("MAIN", "OCV1", StringToDouble(ocvEdit1->Text, 1000));
	ini->WriteFloat("MAIN", "OCV2", StringToDouble(ocvEdit2->Text, 3000));

	config.ir_range = StringToDouble(editIrRange->Text, 5);
	config.ocv_range = StringToDouble(editOcvRange->Text, 1000);

	config.ir_min = StringToDouble(irEdit1->Text, 12);
	config.ir_max = StringToDouble(irEdit2->Text, 5);
	config.ocv_min = StringToDouble(ocvEdit1->Text, 1000);
	config.ocv_max = StringToDouble(ocvEdit2->Text, 3000);

	delete ini;
}
//---------------------------------------------------------------------------
bool __fastcall TTotalForm::ReadSystemInfo()
{
	TIniFile *ini;

	AnsiString file;
	file = (AnsiString)BIN_PATH + "SystemInfo_"+ IntToStr(this->Tag) + ".inf";

	ini = new TIniFile(file);

	config.remeasure_use = ini->ReadBool("MAIN", "AUTO_CHECK", true);
	config.remeasure_cnt = ini->ReadInteger("MAIN", "REMEASURE", 1);
    config.remeasure_alarm_cnt = ini->ReadInteger("MAIN", "REMEASURE_ALARM_COUNT", 3);
	config.remeasure_bypass = 	ini->ReadBool("MAIN", "REM_BYPASS", false);

	//* IR SPEC (min/max 로 한번검사 + average +/-로 한번 더 검사)
	config.average_use = ini->ReadBool("MAIN", "USE_AVERAGE", false);
	config.ir_min = ini->ReadFloat("MAIN", "IR1", 10);
	config.ir_max = ini->ReadFloat("MAIN", "IR2", 40);
	config.ir_range = ini->ReadFloat("MAIN", "IR_RANGE", 5);

	irEdit1->Text = config.ir_min;
	irEdit2->Text = config.ir_max;
	editIrRange->Text = config.ir_range;

    pnlIRSpec->Caption = "IR : " + FormatFloat("0.0", config.ir_min)  + " ~ " + FormatFloat("0.0", config.ir_max);

	//* OCV SPEC (min/max 로 한번검사 + average +/-로 한번 더 검사)
	config.ocv_min = ini->ReadFloat("MAIN", "OCV1", 500);
	config.ocv_max = ini->ReadFloat("MAIN", "OCV2", 3000);
	config.ocv_range = ini->ReadFloat("MAIN", "OCV_RANGE", 1000);

	ocvEdit1->Text = config.ocv_min;
	ocvEdit2->Text = config.ocv_max;
	editOcvRange->Text = config.ocv_range;

    pnlOCVSpec->Caption = "OCV : " + FormatFloat("0.0", config.ocv_min) + " ~ " + FormatFloat("0.0", config.ocv_max);

    editModelName->Text = ini->ReadString("CELLINFO", "MODELNAME", "20PQ");
    editPwd->Text = ini->ReadString("PASSWORD", "PWD", "0000");

	editPLCIPAddress->Text = ini->ReadString("IROCV_PLC", "IP", "17.91.80.220");
	editPLCPortPC->Text = ini->ReadString("IROCV_PLC", "PORT1", "5007");
	editPLCPortPLC->Text = ini->ReadString("IROCV_PLC", "PORT2", "5008");

    PLC_IPADDRESS = editPLCIPAddress->Text;
	PLC_PCPORT = editPLCPortPC->Text.ToIntDef(5007);
	PLC_PLCPORT = editPLCPortPLC->Text.ToIntDef(5008);

	editIROCVIPAddress->Text = ini->ReadString("IROCV", "IP", "192.168.250.202");
	editIROCVPort->Text = ini->ReadString("IROCV", "PORT", "45000");

	Client->Host = editIROCVIPAddress->Text;
    Client->Port = editIROCVPort->Text.ToIntDef(45000);

	editCellModel->Text = ini->ReadString("CELL_INFO", "CELL_MODEL", "-");
	editLotNumber->Text = ini->ReadString("CELL_INFO", "LOT_NUMBER", "-");

    editNgAlarmCount->Text = ini->ReadString("NG_ALARM_COUNT", "COUNT", "20");
	RemeasureChk->Checked = config.remeasure_use;
	RemeasureEdit->Text = config.remeasure_cnt;
	editRemeasureAlarmCount->Text = config.remeasure_alarm_cnt;
	RemeasureForm->pcolor2->Caption = config.remeasure_alarm_cnt;
	chkRemBypass->Checked = config.remeasure_bypass;

	delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::VisibleSpec(bool bUseAverage)
{
	if(bUseAverage){
		grpIrSpec->Visible = true;
		grpOcvSpec->Visible = false;
		grpIrAvg->Visible = true;
		grpOcvAvg->Visible = true;
	}
	else{
		grpIrSpec->Visible = true;
		grpOcvSpec->Visible = true;
		grpIrAvg->Visible = false;
		grpOcvAvg->Visible = false;
	}
}
//---------------------------------------------------------------------------
// 재측정 정보 읽고 쓰기
void __fastcall TTotalForm::ReadRemeasureInfo()
{
	TIniFile *ini;
	ini = new TIniFile((AnsiString)BIN_PATH + "RemeasureInfo.inf");

	AnsiString strNg, strTotalUse, strConsNg, strPrevNg;
	AnsiString title = "REMEASURE" + IntToStr(this->Tag);

	int posNg = 0, posTotalNg = 0, posConsNg = 0, posPrevNg = 0;
	strNg = ini->ReadString(title, "NG", "-1");
    strTotalUse = ini->ReadString(title, "TOTAL_USE", "-1");
    strConsNg = ini->ReadString(title, "CONSECUTIVE_NG", "-1" );
    strPrevNg = ini->ReadString(title, "PREV_NG", "-1");

	int nRemeasureAlarmCount = 0;
	config.remeasure_alarm_cnt = ini->ReadInteger("MAIN", "REMEASURE_ALARM_COUNT", 3);
    editRemeasureAlarmCount->Text = config.remeasure_alarm_cnt;
	RemeasureForm->pcolor2->Caption = config.remeasure_alarm_cnt;

    //* 총 누적 불량
	if(strNg == "-1"){	// 파일이 존재하지 않으면
		for(int index = 0; index < MAXCHANNEL; ++index)
			acc_remeasure[index] = 0; 	// 모두 0으로
	}
	else{
		for(int index = 0; index < MAXCHANNEL; ++index){
			posNg = strNg.Pos("_");
			acc_remeasure[index] = strNg.SubString(1, posNg - 1).ToIntDef(0);
			strNg.Delete(1, posNg);
		}
	}

	//* 총 측정 횟수
	if(strTotalUse == "-1"){	// 파일이 존재하지 않으면
		for(int index = 0; index < MAXCHANNEL; ++index)
			acc_totaluse[index] = 0; 	// 모두 0으로
	} else{
		for(int index = 0; index < MAXCHANNEL; ++index){
			posTotalNg = strTotalUse.Pos("_");
			acc_totaluse[index] = strTotalUse.SubString(1, posTotalNg - 1).ToIntDef(0);
			strTotalUse.Delete(1, posTotalNg);
		}
	}

    //* 이전 측정에서 불량여부
    if(strPrevNg == "-1"){
        for(int index = 0; index < MAXCHANNEL; ++index)
            acc_prevng[index] = 0;
    } else{
        for(int index = 0; index < MAXCHANNEL; ++index){
            posPrevNg = strPrevNg.Pos("_");
            acc_prevng[index] = strPrevNg.SubString(1, posPrevNg - 1).ToIntDef(0);
            strPrevNg.Delete(1, posPrevNg);
        }
    }

    //* 연속불량 횟수
	if(strConsNg == "-1"){	// 파일이 존재하지 않으면
		for(int index = 0; index < MAXCHANNEL; ++index)
			acc_consng[index] = 0; 	// 모두 0으로
	} else{
		for(int index = 0; index < MAXCHANNEL; ++index){
			posConsNg = strConsNg.Pos("_");
			acc_consng[index] = strConsNg.SubString(1, posConsNg - 1).ToIntDef(0);
			if(acc_consng[index] >= config.remeasure_alarm_cnt)
				nRemeasureAlarmCount++;
			strConsNg.Delete(1, posConsNg);
		}
	}
    //* 연속불량이 일정횟수 이상이면 에러발생
    RemeasureAlarm(nRemeasureAlarmCount);

	acc_init = 	 ini->ReadString(title, "ACCMULATE_DAY", Now().FormatString("yyyy. m. d. hh:nn"));
	acc_cnt = ini->ReadInteger(title, "ACC_CNT", 0);

	delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::WriteRemeasureInfo()	// Tray가 Vacancy 상태일때 기록
{
	TIniFile *ini;
	ini = new TIniFile((AnsiString)BIN_PATH + "RemeasureInfo.inf");

	AnsiString strNg, strTotalUse, strConsNg, strPrevNg;
	AnsiString title = "REMEASURE" + IntToStr(this->Tag);
	strNg = "";
    strTotalUse = "";
    strConsNg = "";
    strPrevNg = "";
	int nRemeasureAlarmCount = 0;

	for(int index = 0; index < MAXCHANNEL; ++index){
		strNg = strNg + acc_remeasure[index] + "_";
        strTotalUse = strTotalUse + acc_totaluse[index] + "_";
        strConsNg = strConsNg + acc_consng[index] + "_";
        strPrevNg = strPrevNg + acc_prevng[index] + "_";

		if(acc_consng[index] >= config.remeasure_alarm_cnt)
			nRemeasureAlarmCount++;
	}
	RemeasureAlarm(nRemeasureAlarmCount);

    ini->WriteInteger(title, "REMEASURE_ALARM_COUNT", editRemeasureAlarmCount->Text.ToIntDef(3));
    ini->WriteString(title, "TOTAL_USE", strTotalUse);
	ini->WriteString(title, "NG", strNg);
    ini->WriteString(title, "CONSECUTIVE_NG", strConsNg);
    ini->WriteString(title, "PREV_NG", strPrevNg);
	ini->WriteString(title, "ACCMULATE_DAY", acc_init);
	ini->WriteInteger(title, "ACC_CNT", acc_cnt); //* 이 버전에서는 안쓰는 변수

	delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::RemeasureAlarm(int remeasure_alarm_count)
{
	if(remeasure_alarm_count > 0) {
		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data,  PC_D_IROCV_NG_ALARM, 1);
		btnRemeasureInfo->Color = clRed;
		lblRemeasureAlarmCheck->Visible = true;
	}
	else{
        Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data,  PC_D_IROCV_NG_ALARM, 0);
		btnRemeasureInfo->Color = clWhite;
		lblRemeasureAlarmCheck->Visible = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::WriteCommLog(AnsiString Type, AnsiString Msg)
{
	AnsiString str, dir;
	int file_handle;

	dir = (AnsiString)LOG_PATH + Now().FormatString("yyyymmdd") + "\\";
	ForceDirectories((AnsiString)dir);

	str = dir + "STAGE" + FormatFloat("000", this->Tag+1) + "_" + Now().FormatString("yymmdd-hh") + ".log";

	if(FileExists(str))
		file_handle = FileOpen(str, fmOpenWrite);
	else{
		file_handle = FileCreate(str);
	}

	FileSeek(file_handle, 0, 2);

	str = Now().FormatString("yyyy-mm-dd hh:nn:ss ") +Type +  "\t" + Msg + "\n";
	FileWrite(file_handle, str.c_str(), str.Length());

	FileClose(file_handle);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::WritePLCLog(AnsiString Type, AnsiString Msg)
{
	AnsiString str, dir;
	int file_handle;

	dir = (AnsiString)LOG_PATH + Now().FormatString("yyyymmdd") + "\\";
	ForceDirectories((AnsiString)dir);

	str = dir + "STAGE" + FormatFloat("000", this->Tag+1) + "_PLC_" + Now().FormatString("yymmdd-hh") + ".log";

	if(FileExists(str))
		file_handle = FileOpen(str, fmOpenWrite);
	else{
		file_handle = FileCreate(str);
	}

	FileSeek(file_handle, 0, 2);

	str = Now().FormatString("yyyy-mm-dd hh:nn:ss ") +Type +  "\t" + Msg + "\n";
	FileWrite(file_handle, str.c_str(), str.Length());

	FileClose(file_handle);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::WriteResultFile()
{
	int file_handle;
	AnsiString filename;
	AnsiString dir;
	AnsiString cell, cell_id, ir, ocv, ch, ok_ng;

	dir = (AnsiString)DATA_PATH + Now().FormatString("yyyymmdd") + "\\";
	ForceDirectories((AnsiString)dir);

	filename =  dir + tray.trayid +  "-" + Now().FormatString("yymmddhhnnss") + ".csv";

	if(FileExists(filename)){
		DeleteFile(filename);
	}

	file_handle = FileCreate(filename);
	FileSeek(file_handle, 0, 0);

	AnsiString file;
	file = "Tray ID," + tray.trayid + "\n";
    file = "TRAY POSITION,ALL\r\n";
    file = file + "ARRIVE TIME," + m_dateTime.FormatString("yyyy/mm/dd hh:nn:ss") + "\r\n";
	file = file + "FINISH TIME," + Now().FormatString("yyyy/mm/dd hh:nn:ss") + "\r\n";
	file = file + "IR Min.," + FormatFloat("0.0", config.ir_min) + ",IR Max.," + FormatFloat("0.0", config.ir_max) + "\n";
	file = file + "OCV Min.," + FormatFloat("0.0", config.ocv_min) + ",OCV Max.," + FormatFloat("0.0", config.ocv_max) + "\n";
	file = file + "CH,CELL,CELL_ID,IR,OCV,RESULT\n";

	for(int i = 0; i < MAXCHANNEL; ++i){
		ch = IntToStr(i+1);

		cell_id = tray.cell_serial[i];
		ir = FormatFloat("0.00", tray.after_value[i]);
		ocv = FormatFloat("0.0", tray.ocv_value[i]);

		if(tray.cell[i] == 1)
		{
			if(retest.cell[i] == 0) ok_ng = "OK";
			else if(retest.cell[i] == 2) ok_ng = "IR SPEC NG";
            else if(retest.cell[i] == 5) ok_ng = "IR Avg. NG";
			else if(retest.cell[i] == 3) ok_ng = "OCV SPEC NG";
            else if(retest.cell[i] == 6) ok_ng = "OCV Avg. NG";

			cell = "O";
		}
		else if(tray.cell[i] == 0)
		{
			if(panel[i]->Color == clCellError) ok_ng = "NG(No Cell)";
			else ok_ng = "No Cell";

			cell = "X";
		}

		file = file + ch + "," + cell + "," + cell_id + ", " + ir + "," + ocv + "," + ok_ng +"\n";
	}
	FileWrite(file_handle, file.c_str(), file.Length());
	FileClose(file_handle);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::WriteResultFile(int traypos)
{
	int file_handle;
	AnsiString filename;
	AnsiString dir;
	AnsiString cell, cell_id, ir, ocv, ch, ok_ng;

	dir = (AnsiString)DATA_PATH + Now().FormatString("yyyymmdd") + "\\";
	ForceDirectories((AnsiString)dir);

	filename =  dir + tray.trayid + "_TP" + IntToStr(traypos) + ".csv";

	if(FileExists(filename)){
		DeleteFile(filename);
	}

	file_handle = FileCreate(filename);
	FileSeek(file_handle, 0, 0);

	AnsiString file;
	file = "Tray ID," + tray.trayid + "\n";
    file = "TRAY POSITION," + IntToStr(traypos) + "\r\n";
    file = file + "ARRIVE TIME," + m_dateTime.FormatString("yyyy/mm/dd hh:nn:ss") + "\r\n";
	file = file + "FINISH TIME," + Now().FormatString("yyyy/mm/dd hh:nn:ss") + "\r\n";
	file = file + "IR Min.," + FormatFloat("0.0", config.ir_min) + ",IR Max.," + FormatFloat("0.0", config.ir_max) + "\n";
	file = file + "OCV Min.," + FormatFloat("0.0", config.ocv_min) + ",OCV Max.," + FormatFloat("0.0", config.ocv_max) + "\n";
	file = file + "CH,CELL,CELL_ID,IR,OCV,RESULT\n";

    int channel;
	for(int i = 0; i < MAXCHANNEL; ++i){
        channel = i;
		cell_id = tray.cell_serial[i];
		ir = FormatFloat("0.00", tray.after_value[channel]);
		ocv = FormatFloat("0.0", tray.ocv_value[channel]);

		if(tray.cell[channel] == 1)
		{
			if(retest.cell[channel] == 0) ok_ng = "OK";
			else if(retest.cell[channel] == 2) ok_ng = "IR SPEC NG";
            else if(retest.cell[channel] == 5) ok_ng = "IR Avg. NG";
			else if(retest.cell[channel] == 3) ok_ng = "OCV SPEC NG";
            else if(retest.cell[channel] == 6) ok_ng = "OCV Avg. NG";

			cell = "O";
		}
		else if(tray.cell[channel] == 0)
		{
			if(panel[channel]->Color == clCellError) ok_ng = "NG(No Cell)";
			else ok_ng = "No Cell";

			cell = "X";
		}

		file = file + IntToStr(channel + 1) + "," + cell + "," + cell_id + ", " + ir + "," + ocv + "," + ok_ng +"\n";
	}
	FileWrite(file_handle, file.c_str(), file.Length());
	FileClose(file_handle);
}
//---------------------------------------------------------------------------
bool __fastcall TTotalForm::ReadResultFile(int traypos)
{
    bool bExistFile = false;
	AnsiString filename;
    AnsiString dir = (AnsiString)DATA_PATH + Now().FormatString("yyyymmdd") + "\\";
    filename = dir + tray.trayid + "_TP" + IntToStr(traypos) + ".csv";

    if (!FileExists(filename)) {
        WritePLCLog("ReadResultFile","File is not exist: " + filename);
        return false;
    }
    else
        bExistFile = true;

    TStringList *lines = new TStringList;
    try {
        lines->LoadFromFile(filename);

        // CSV에서 데이터 시작 인덱스: 헤더는 6줄 (TRAY ID ~ CH,CELL...) 이후부터 데이터
        int startLine = 6;
        int idx = 0;
        int channel;
        AnsiString ir, ocv;
        for (int i = startLine; i < lines->Count; i++) {
            AnsiString line = lines->Strings[i];
            if (line.Trim().IsEmpty()) continue;

            // CH,CELL,CELL_ID,IR,OCV,RESULT
            TStringList *cols = new TStringList;
            cols->Delimiter = ',';
            cols->StrictDelimiter = true;
            cols->DelimitedText = line;

            if (cols->Count >= 6) {
                // IR, OCV는 3, 4번째 열
                //if(MeasureInfoForm->pocv[channel]->Caption.Pos("-") > 1){
                    channel = StringToInt(cols->Strings[0].Trim(), 1) - 1;
                    ir = cols->Strings[3].Trim();
                    ocv = cols->Strings[4].Trim();
                	MeasureInfoForm->pir[channel]->Caption = ir;
                	MeasureInfoForm->pocv[channel]->Caption = ocv;

                    tray.after_value[channel] = StringToDouble(ir, 0);
					tray.ocv_value[channel] = StringToDouble(ocv, 0);

                    if(tray.after_value[channel] < config.ir_min && tray.after_value[channel] > config.ir_max)
                        retest.cell[channel] = 2;
                    if(tray.ocv_value[channel] < config.ocv_min && tray.ocv_value[channel] > config.ocv_max)
                        if(retest.cell[channel] != 2) retest.cell[channel] == 3;
                    else
                        retest.cell[channel] = 0;

                //}
                idx++;
            }
            delete cols;
        }
    }
    __finally {
        delete lines;
    }
    return true;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ErrorLog()
{
	AnsiString str, dir;
	int file_handle;

	dir = (AnsiString)LOG_PATH + Now().FormatString("yyyymmdd") + "\\";
	ForceDirectories((AnsiString)dir);

	str = dir + "ERROR" + FormatFloat("000", this->Tag+1) + "_" + Now().FormatString("yymmdd-hh") + ".log";

	if(FileExists(str))
		file_handle = FileOpen(str, fmOpenWrite);
	else{
		file_handle = FileCreate(str);
	}

	FileSeek(file_handle, 0, 2);

	str = Now().FormatString("yyyy-mm-dd hh:nn:ss ") + error1->Caption + ", " + error3->Caption + ", " + error4->Caption + "\n";
	FileWrite(file_handle, str.c_str(), str.Length());

	FileClose(file_handle);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ErrorLog(AnsiString msg)
{
	AnsiString str, dir;
	int file_handle;

	dir = (AnsiString)LOG_PATH + Now().FormatString("yyyymmdd") + "\\";
	ForceDirectories((AnsiString)dir);

	str = dir + "ERROR" + FormatFloat("000", this->Tag+1) + "_" + Now().FormatString("yymmdd") + ".log";

	if(FileExists(str))
		file_handle = FileOpen(str, fmOpenWrite);
	else{
		file_handle = FileCreate(str);
	}

	FileSeek(file_handle, 0, 2);

	str = Now().FormatString("yyyy-mm-dd hh:nn:ss ") + msg + "\n";
	FileWrite(file_handle, str.c_str(), str.Length());

	FileClose(file_handle);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ReadCaliboffset()                         //20171202 개별보정을 위해 추가
{
	TIniFile *ini;
	ini = new TIniFile((AnsiString)BIN_PATH + "Caliboffset_" + IntToStr(this->Tag) + ".cali");

	for(int index = 0; index < MAXCHANNEL; ++index){     							//20171202 개별보정을 위해 추가
		stage.ir_offset[index] = ini->ReadFloat("IR OFFSET", IntToStr(index+1), 0.0);
	}

	delete ini;
}
//---------------------------------------------------------------------------
int __fastcall TTotalForm::ReadCellSerial()
{
    int nCellSerial = 0;
    for(int i = 0; i < MAXCHANNEL; i++){
        tray.cell_serial[i] = Mod_PLC->GetCellSrial(PLC_D_IROCV_CELL_SERIAL, i, 10);
        if(tray.cell_serial[i].IsEmpty() == false) nCellSerial++;
    }
    return nCellSerial;
}
//---------------------------------------------------------------------------
