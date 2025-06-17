//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "FormTotal.h"
#include "RVMO_main.h"
#include "FormMeasureInfo.h"
#include "Modplc.h"
#include "FormCalibration.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvSmoothButton"
#pragma link "AdvSmoothPanel"
#pragma resource "*.dfm"

TTotalForm *TotalForm;

//---------------------------------------------------------------------------
__fastcall TTotalForm::TTotalForm(TComponent* Owner)
	: TForm(Owner)
{
	remLimit = 50;
	Old_batch = "START";
	senCnt = 0;
	CurrentGrp = GrpMain;
	bconfig = false;
	sock = NULL;

    clAverageOver = cl_avgover->Color;
	clNoCell = cl_no->Color;
	clBadIr = cl_badir->Color;
	clCellError = cl_badocv->Color;
	clLine = cl_line->Color;
	clIrCheck = cl_ir->Color;
	clOcvCheck = cl_ocv->Color;
	clBothCheck = cl_irocv->Color;
	clMeasureFail = cl_ce->Color;
	clNo = clSilver;
	clYes = clrConInfo->Color;

    ReadchannelMapping();
	MakePanel(BaseForm->lblLineNo->Caption);
	start_delay_time = 0;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::FormShow(TObject *Sender)
{
	pdev[0] = pdev1;
	pdev[0]->ParentBackground = false;
	pdev[1] = pdev2;
	pdev[1]->ParentBackground = false;
	pdev[2] = pdev3;
	pdev[2]->ParentBackground = false;
	pdev[3] = pdev4;
	pdev[3]->ParentBackground = false;
	pdev[4] = pdev5;
	pdev[4]->ParentBackground = false;
	pdev[5] = pdev6;
	pdev[5]->ParentBackground = false;
	pdev[6] = pdev7;
	pdev[6]->ParentBackground = false;
	pdev[7] = pdev8;
	pdev[7]->ParentBackground = false;

	stage.init = true;
	m_bAuto = true;
	bLocal = false;

    this->ReadCaliboffset();                      //20171202 개별보정을 위해 추가
    ReadRemeasureInfo();
	ReadSystemInfo();

    btnMeasureInfoClick(this);
	Initialization();

	Timer_PLCConnect->Enabled = true;
	btnConnectIROCVClick(this);
	config.recontact = true;

	pback->Width = 620;
	this->Width = pback->Width + 10;
	this->Height = pback->Height;

	OldPLCStatus = "";
	PLCStatus = "";
	OldErrorCheckStatus = "";
	ErrorCheckStatus = "";
	OldIROCVStage = "";
	IROCVStage = "";

	pProcess[0] = pReady;
	pProcess[1] = pTrayIn;
	pProcess[2] = pBarcode;
	pProcess[3] = pProbeDown;
	pProcess[4] = pMeasure;
	pProcess[5] = pFinish;
	pProcess[6] = pProbeOpen;
	pProcess[7] = pTrayOut;
}
//---------------------------------------------------------------------------
// 구조체 초기화 : 트레이 정보, 재측정 정보
void __fastcall TTotalForm::InitCellDisplay()
{
	for(int i = 0; i < MAXCHANNEL; ++i){
		panel[i]->Color = clLine;
		panel[i]->ParentBackground = false;
		if(MeasureInfoForm->stage == this->Tag){
			MeasureInfoForm->DisplayIrValue(i, clLine, "");
			MeasureInfoForm->DisplayOcvValue(i, clLine, "");
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::InitTrayStruct()
{
	InitCellDisplay();
	memset(&tray, 0, sizeof(tray));
	memset(&retest, 0, sizeof(retest));

	this->WriteRemeasureInfo();
	tray.trayid = "start";

	for(int i = 0; i < MAXCHANNEL; ++i){
		tray.cell[i] = 1;	//CELL INFO
//		tray.cell_serial[i] = 1;
	}
	tray.first = true;

	pWork->Visible = false;
	pWork->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::Initialization()
{
	PLCInitialization();
	this->InitTrayStruct();

//    if(m_Auto)
	nSection = STEP_WAIT;

	nStep = 0;
    n_bMeasureStart = false;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::PLCInitialization()
{
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_TRAY_OUT, 0);
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_PROB_OPEN, 0);
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_PROB_CLOSE, 0);
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_ERROR, 0);

	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_MEASURING, 0);
    Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_COMPLETE1, 0);
    Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_COMPLETE2, 0);
    Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_TRAY_POS_MOVE, 0);

	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_NG_COUNT, 0);
	for(int i = 0; i < LINECOUNT; i++)
	{
		for(int j = 0; j < LINECOUNT; j++)
		{
			Mod_PLC->SetData(Mod_PLC->pc_Interface_Data, PC_D_IROCV_MEASURE_OK_NG + i, j, false);
		}
	}

	for(int i = 0; i < MAXCHANNEL; i++)
	{
		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_IR_VALUE + i, 0);
		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Ocv_Data, PC_D_IROCV_OCV_VALUE + i, 0);
	}

	WriteIRMINMAX();

	WritePLCLog("PLCInitialization", "IROCV TRAY OUT, IROCV PROBE OPEN, IROCV PROBE CLOSE = 0");
	OldPLCStatus = "";
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	WriteRemeasureInfo();
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnSaveConfigClick(TObject *Sender)
{
	if(MessageBox(Handle, L"Are you sure you want to save?", L"SAVE", MB_YESNO|MB_ICONQUESTION) == ID_YES){
		WriteSystemInfo();
		WriteRemeasureInfo();
        ReadSystemInfo();
//		pnlConfig->Visible = false;
	}
}
//---------------------------------------------------------------------------
//
// IR/OCV 검사기 통신 관련
//
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ClientConnect(TObject *Sender,
	  TCustomWinSocket *Socket)
{
	pConInfo->Font->Color = clrConInfo->Color;
	pConInfo->Caption = "IR/OCV is connected";
	sock = Socket;

	send.tx_mode = 0;  	// 초기화
	send.time_out = 0;
	send.re_send = 0;

	if(stage.arl == nLocal){
		this->CmdManualMod(true);
	}
	OldSenCmd = "NONE";
	SendTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ClientConnecting(TObject *Sender,
	  TCustomWinSocket *Socket)
{
	pConInfo->Font->Color = clRed;
	pConInfo->Caption = "Connection...";
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ClientError(TObject *Sender,
	  TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
	AnsiString str;
	str = "Connection failed";
	pConInfo->Caption = str;
	ErrorCode = 0;
	Socket->Close();
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ClientDisconnect(TObject *Sender,
	  TCustomWinSocket *Socket)
{
	pConInfo->Font->Color = clRed;
	pConInfo->Caption = "Connection failed.";
	ReContactTimer->Enabled = true;
	sock = NULL;
	//this->DisplayStatus(nNoAnswer);
}
//---------------------------------------------------------------------------
// 재접속 타이머
void __fastcall TTotalForm::ReContactTimerTimer(TObject *Sender)
{
		ReContactTimer->Enabled = false;
		if(config.recontact == true)
			Client->Active = true;
}
//---------------------------------------------------------------------------

void __fastcall TTotalForm::ClientRead(TObject *Sender,
	  TCustomWinSocket *Socket)
{
	AnsiString msg;
	AnsiString queue_msg;

	msg = Socket->ReceiveText();
	int stx =  msg.Pos((char)0x02);
	int etx = msg.Pos((char)0x03);

	if(etx > 0){
		while(etx > 0){
			if(stx == 1)queue_msg = msg.SubString(1, etx);
			else queue_msg = remainMsg + msg.SubString(1, etx);
			rxq.push(queue_msg.c_str());
			msg.Delete(1, etx);

			stx =  msg.Pos((char)0x02);
			etx =  msg.Pos((char)0x03);

			if(etx > 0)remainMsg = "";
			else remainMsg = msg;
		}
	}else{
		remainMsg = msg;
	}
}
//---------------------------------------------------------------------------

void __fastcall TTotalForm::rxTimerTimer(TObject *Sender)
{
	AnsiString RxStr;
	bool flag;

	if(rxq.empty() == false){	// 데이터가 있으면 처리
			RxStr = rxq.front().data();
			rxq.pop();
			SendMessage(BaseForm->nForm[Tag]->Handle, COMM_RECEIVE, 0, (LPARAM)&RxStr);
	}

}
//---------------------------------------------------------------------------


// 전송 타이머
void __fastcall TTotalForm::SendTimerTimer(TObject *Sender)
{
	if(q_cmd.empty() == false){
		SendTimer->Interval = 700;
		SendData(q_cmd.front().data(), q_param.front().data());
		q_cmd.pop();
		q_param.pop();
	}else{
		senCnt += 1;

		switch(send.tx_mode){
			case 0: // 일반 상태, 타임아웃 처리
				SendTimer->Interval = 10;
				if(senCnt < 32 && senCnt > 30){
					SendData("OUT");
				}else if(senCnt > 60) {
					senCnt = 0;
					SendData("SEN");
				}
				break;
			case 1: // 특정 메세지 전송
				SendTimer->Interval = 500;
				SendData(send.cmd, send.param);
				break;
			case 2:	// 응답일 경우
				SendTimer->Interval = 100;
				SendData(send.cmd, send.param);
				send.tx_mode = 0;
				break;
			case 3:	// 전송 후 다른 명령어 대기
                SendTimer->Interval = 300;
				SendData(send.cmd, send.param);
				send.tx_mode = 100;
			case 200:
				return;
			default:
				break;
		}
		send.time_out += 1;	// 재전송 횟수 및 타이머 시간 체크
		// 타임아웃 시간 설정 할것.
		if(send.time_out == 300){
			//this->DisplayStatus(nNoAnswer);
		}
	}
}
//---------------------------------------------------------------------------
// 측정정보보기 버튼
void __fastcall TTotalForm::InitMeasureForm()
{
	TColor clr, fontclr;
	AnsiString sir, socv;

	MeasureInfoForm->InitStruct();
	MeasureInfoForm->stage = this->Tag;
	MeasureInfoForm->display.cell = tray.cell;	// cell 정보
	MeasureInfoForm->display.orginal_value = tray.orginal_value;	// 보정전 값 - ir
	MeasureInfoForm->display.after_value = tray.after_value;		// 보정후 값 - ir
	MeasureInfoForm->display.ocv_value = tray.ocv_value;			// ocv
	MeasureInfoForm->display.measure_result = tray.measure_result;	// ir 결과
	//MeasureInfoForm->pstage->Caption = lblTitle->Caption;
	int clr_index = 0;
	for(int i=0; i<MAXCHANNEL; ++i){
		sir = "";
		clr_index = GetColorIndex(panel[i]->Color);
		if(clr_index == MeasureFail){
			switch(tray.measure_result[i]){
				case HI: sir = "HI"; break;
				case LO: sir = "LO"; break;
				case OV: sir = "OV"; break;
				case UN: sir = "UN"; break;
				case CE: sir = "CE"; break;
				case NA: sir = "NA"; break;
				case NO: sir = "NO"; break;
			}
		}
		this->SetProcessColor(i, clr_index, sir);
	}

//	MeasureInfoForm->pLocal->Visible = false;
//	MeasureInfoForm->grpRemeasure->Visible = false;
	MeasureInfoForm->GroupBox5->Visible = true;
	MeasureInfoForm->BringToFront();
	MeasureInfoForm->Visible = true;
}
void __fastcall TTotalForm::InitMeasureFormRemeasure()
{
	TColor clr, fontclr;
	AnsiString sir, socv;

	MeasureInfoForm->InitStruct();
	MeasureInfoForm->stage = this->Tag;
	MeasureInfoForm->display.cell = tray.cell;	// cell 정보
	MeasureInfoForm->display.orginal_value = tray.orginal_value;	// 보정전 값 - ir
	MeasureInfoForm->display.after_value = tray.after_value;		// 보정후 값 - ir
	MeasureInfoForm->display.ocv_value = tray.ocv_value;			// ocv
	MeasureInfoForm->display.measure_result = tray.measure_result;	// ir 결과
	//MeasureInfoForm->pstage->Caption = lblTitle->Caption;
	int clr_index = 0;
	for(int i=0; i<MAXCHANNEL; ++i){
		sir = "";
		clr_index = GetColorIndex(panel[i]->Color);
		if(clr_index == MeasureFail){
			switch(tray.measure_result[i]){
				case HI: sir = "HI"; break;
				case LO: sir = "LO"; break;
				case OV: sir = "OV"; break;
				case UN: sir = "UN"; break;
				case CE: sir = "CE"; break;
				case NA: sir = "NA"; break;
				case NO: sir = "NO"; break;
			}
		}
		this->SetProcessColor(i, clr_index, sir);
	}
	MeasureInfoForm->BringToFront();
	MeasureInfoForm->Visible = true;
}

// 재측정 정보 보기
void __fastcall TTotalForm::btnRemeasureInfoClick(TObject *Sender)
{
	RemeasureForm->stage            = this->Tag;
	RemeasureForm->acc_remeasure 	= acc_remeasure;
	RemeasureForm->acc_init 		= &acc_init;
	RemeasureForm->acc_cnt			= &acc_cnt;

	RemeasureForm->pstage->Caption	= lblTitle->Caption;
	RemeasureForm->Visible = true;
    RemeasureForm->Top = 50;
}
//---------------------------------------------------------------------------
// 센서 정보 보기

// 수동 전지 검사
void __fastcall TTotalForm::localTestClick(TObject *Sender)
{
	MeasureInfoForm->display.arl = nLocal;
	InitMeasureForm();
	MeasureInfoForm->pLocal->Visible = true;
	bLocal = true;
}
//---------------------------------------------------------------------------
void  __fastcall TTotalForm::OnInit()
{
	for(int i=0; i<MAXCHANNEL; ++i){
		panel[i]->Color = clLine;
		tray.ocv_value[i] = 0;
		tray.after_value[i] = 0;
		tray.orginal_value[i] = 0;
		MeasureInfoForm->DisplayIrValue(i, clLine, "-");
		MeasureInfoForm->DisplayOcvValue(i, clLine, "-");
	}
}
//---------------------------------------------------------------------------
// 통신 정보 보기 - 삭제할것
// 닫기 버튼
// 메인화면 버튼
// 전체 재측정
void __fastcall TTotalForm::RemeasureAllBtnClick(TObject *Sender)
{
//	retest.re_excute = false;
	//CmdBattHeight();
	//CmdAutoTest();     // 2017 09 11 herald
	InitTrayStruct();

//	LoadTrayInfo(tray.trayid);
	tray.cell_count = 0;
	for(int i = 0; i < MAXCHANNEL; i++){
		tray.cell[i] = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_TRAY_CELL_DATA + i);
		tray.cell_count += tray.cell[i];
	}

	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_PROB_CLOSE, 1);
	WritePLCLog("RemeasureAllBtnClick", "IROCV PROBE CLOSE = 1");

	retest.re_excute = false;
	nSection = STEP_MEASURE;
	nStep = 2;
	VisibleBox(GrpMain);
}
//---------------------------------------------------------------------------
// 개별 재측정
void __fastcall TTotalForm::RemeasureBtnClick(TObject *Sender)
{
	//InitTrayStruct();
	//InitCellDisplay();
//	tray.trayid = "start";			// 수정
	tray.rem_mode = 1;
	retest.re_index = 0;
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_PROB_CLOSE, 1);
	WritePLCLog("RemeasureBtnClick", "IROCV PROBE CLOSE = 1");

	retest.re_excute = true;
	nSection = STEP_MEASURE;
	nStep = 2;
	VisibleBox(GrpMain);
}
//---------------------------------------------------------------------------


// 트레이 보내기 - 재측정
void __fastcall TTotalForm::TrayOutBtnClick(TObject *Sender)
{
	//ims->WriteMsg_TrayFinish(m_Equip_Info, tray, retest);		// 수정
	CmdTrayOut();
	VisibleBox(GrpMain);
	nSection = STEP_FINISH;
	nStep = 0;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// 트레이 보내기 - 메인폼
// 검사시작 - 메인폼
void __fastcall TTotalForm::AlarmConfirmBtnClick(TObject *Sender)
{
	MainBtnClick(Sender);

}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnTrayOutClick(TObject *Sender)
{
	if(MessageBox(Handle, L"Are you sure you want to eject the tray?", L"", MB_YESNO|MB_ICONQUESTION) == ID_YES){
		for(int i = 0; i < MAXCHANNEL; i++) retest.cell[i] = '0';
		this->CmdTrayOut();
	}
}
//---------------------------------------------------------------------------

// 메인화면 패널 만들기
// 1,2 라인 / 3,4 라인 다름.
void __fastcall TTotalForm::MakePanel(AnsiString type)
{
	int nx, ny, nw, nh;
    nh = (pBase->Height-25)/LINECOUNT;
    nw = (pBase->Width-25)/LINECOUNT;
    int ch;
	if(type == "3") //* 왼쪽 위가 1번, 오른쪽 방향으로 1 -> 24
	{
		nx = 1;
		ny = 1;

		for(int index = 0; index < MAXCHANNEL;){
			panel[index] = new TPanel(this);
			panel[index]->Parent = pBase;
			panel[index]->Left =  nx;
			panel[index]->Top = ny;
			panel[index]->Width = nw;
			panel[index]->Height = nh;

			panel[index]->Color = pnormal1->Color;

			panel[index]->BevelInner = bvNone;
			panel[index]->BevelKind = bkNone;
			panel[index]->BevelOuter = bvNone;
			panel[index]->Tag = index;
	//		panel[index]->Caption = index;

			//panel[index]->Hint = IntToStr(index+1) + " (" + IntToStr((index/LINECOUNT)+1) + "-" + IntToStr((index%LINECOUNT)+1) + ")";
            //* 채널 위치 -> 릴레이가 12줄이므로 위치를 계산해야 함
            ch = chReverseMap[index + 1];
            if(ch >= 289) ch  = ch - 288;
            panel[index]->Hint = IntToStr(index + 1) + " : " + IntToStr((ch - 1)/LINECOUNT + 1) + "-" + IntToStr((ch - 1)%LINECOUNT + 1);

			panel[index]->ShowHint = true;
			panel[index]->OnMouseEnter =  ChInfoMouseEnter;
			panel[index]->OnMouseLeave =  ChInfoMouseLeave;

			index += 1;
			nx = nx + nw + 1;
			if(index % 2 == 0) nx += 1;
			if(index % (LINECOUNT / 4) == 0) nx += 1;
			if(index % LINECOUNT == 0)
			{
				ny = ny + nh + 1;
				nx = 1;
				if( (index / LINECOUNT) % (LINECOUNT / 4) == 0) ny += 2;
			}
		}
	}
    else if(type == "4") //* 오른쪽 위가 1번, 왼쪽 방향으로 24 <- 1
	{
		nx = pBase->Width - nw - 1;
		ny = 1;

		for(int index = 0; index < MAXCHANNEL;){
			panel[index] = new TPanel(this);
			panel[index]->Parent = pBase;
			panel[index]->Left =  nx;
			panel[index]->Top = ny;
			panel[index]->Width = nw;
			panel[index]->Height = nh;

			panel[index]->Color = pnormal1->Color;

			panel[index]->BevelInner = bvNone;
			panel[index]->BevelKind = bkNone;
			panel[index]->BevelOuter = bvNone;
			panel[index]->Tag = index;
	//		panel[index]->Caption = index;

			//panel[index]->Hint = IntToStr(index+1) + " (" + IntToStr((index/LINECOUNT)+1) + "-" + IntToStr((index%LINECOUNT)+1) + ")";
            ch = chReverseMap[index + 1];
            if(ch >= 289) ch  = ch - 288;
            panel[index]->Hint = "POS : " + IntToStr((ch - 1)/LINECOUNT + 1) + "-" + IntToStr((ch - 1)%LINECOUNT + 1);

			panel[index]->ShowHint = true;
			panel[index]->OnMouseEnter =  ChInfoMouseEnter;
			panel[index]->OnMouseLeave =  ChInfoMouseLeave;

			index += 1;
			nx = nx - nw - 1;
			if(index % 2 == 0) nx -= 1;
			if(index % (LINECOUNT / 4) == 0) nx -= 1;
			if(index % LINECOUNT == 0)
			{
				ny = ny + nh + 1;
				nx = pBase->Width - nw - 1;
				if( (index / LINECOUNT) % (LINECOUNT / 4) == 0) ny += 2;
			}
		}
	}
	else if(type == "1") //* 왼쪽 아래가 1번. 오른쪽 방향으로 1 -> 24
	{
		nx = 1;
		ny = pBase->Height - nh - 1;

		for(int index = 0; index < MAXCHANNEL;){
			panel[index] = new TPanel(this);
			panel[index]->Parent = pBase;
			panel[index]->Left =  nx;
			panel[index]->Top = ny;
			panel[index]->Width = nw;
			panel[index]->Height = nh;

			panel[index]->Color = pnormal1->Color;
			panel[index]->ParentBackground = false;

			panel[index]->BevelInner = bvNone;
			panel[index]->BevelKind = bkNone;
			panel[index]->BevelOuter = bvNone;
			panel[index]->Tag = index;
	//		panel[index]->Caption = index;

			//panel[index]->Hint = IntToStr(index+1) + " (" + IntToStr((index/LINECOUNT)+1) + "-" + IntToStr((index%LINECOUNT)+1) + ")";
            ch = chReverseMap[index + 1];
            if(ch >= 289) ch  = ch - 288;
            panel[index]->Hint = "POS : " + IntToStr((ch - 1)/LINECOUNT + 1) + "-" + IntToStr((ch - 1)%LINECOUNT + 1);

			panel[index]->ShowHint = true;
			panel[index]->OnMouseEnter =  ChInfoMouseEnter;
			panel[index]->OnMouseLeave =  ChInfoMouseLeave;

			index += 1;
			nx = nx + nw + 1;
			if(index % 2 == 0) nx += 1;
			if(index % (LINECOUNT / 4) == 0) nx += 1;
			if(index % LINECOUNT == 0)
			{
				ny = ny - nh - 1;
				nx = 1;
				if( (index / LINECOUNT) % (LINECOUNT / 4) == 0) ny -= 2;
			}
		}
	}
    else if(type == "2") //* 오른쪽 아래가 1번. 왼쪽 방향으로 24 <- 1
	{
		nx = pBase->Width - nw - 1;
		ny = pBase->Height - nh - 1;

		for(int index = 0; index < MAXCHANNEL;){
			panel[index] = new TPanel(this);
			panel[index]->Parent = pBase;
			panel[index]->Left =  nx;
			panel[index]->Top = ny;
			panel[index]->Width = nw;
			panel[index]->Height = nh;

			panel[index]->Color = pnormal1->Color;
			panel[index]->ParentBackground = false;

			panel[index]->BevelInner = bvNone;
			panel[index]->BevelKind = bkNone;
			panel[index]->BevelOuter = bvNone;
			panel[index]->Tag = index;
	//		panel[index]->Caption = index;

			//panel[index]->Hint = IntToStr(index+1) + " (" + IntToStr((index/LINECOUNT)+1) + "-" + IntToStr((index%LINECOUNT)+1) + ")";
            ch = chReverseMap[index + 1];
            if(ch >= 289) ch  = ch - 288;
            panel[index]->Hint = "POS : " + IntToStr((ch - 1)/LINECOUNT + 1) + "-" + IntToStr((ch - 1)%LINECOUNT + 1);

			panel[index]->ShowHint = true;
			panel[index]->OnMouseEnter =  ChInfoMouseEnter;
			panel[index]->OnMouseLeave =  ChInfoMouseLeave;

			index += 1;
			nx = nx - (nw + 1);
			if(index % 2 == 0) nx -= 1;
			if(index % (LINECOUNT / 4) == 0) nx -= 1;
			if(index % LINECOUNT == 0)
			{
				ny = ny - nh - 1;
				nx = pBase->Width - nw - 1;
				if( (index / LINECOUNT) % (LINECOUNT / 4) == 0) ny -= 2;
			}
		}
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TTotalForm::BadListDrawItem(TCustomListView *Sender,
	  TListItem *Item, TRect &Rect, TOwnerDrawState State)
{
	if(Item->Selected ) {
		BadList->Canvas->Brush->Color = clYellow;
	}
	else {
		BadList->Canvas->Brush->Color = clWindow;
	}
	BadList->Canvas->FillRect(Rect);

	BadList->Canvas->Font->Size = 8;
	BadList->Canvas->TextOut(Rect.Left + 5,Rect.Top,Item->Caption);
	int width = 0;
	for(int i=0; i<Item->SubItems->Count; i++) {
		if(i == 0){
			BadList->Canvas->Font->Color = clRed;
			BadList->Canvas->Font->Style = Font->Style << fsBold;

		}
		else{
			BadList->Canvas->Font->Color = clBlack;
		}

		width += BadList->Columns->Items[i]->Width;
		BadList->Canvas->TextOut(Rect.Left + width + 5,Rect.Top,Item->SubItems->Strings[i]);
	}

}
//---------------------------------------------------------------------------


void __fastcall TTotalForm::StatusTimerTimer(TObject *Sender)
{
	if(stage.now_status != stage.alarm_status){
		stage.now_status = stage.alarm_status;
		stage.alarm_cnt = 0;
	}

	stage.alarm_cnt += 1;
    if(stage.alarm_cnt >= 1500) stage.alarm_cnt = 0;
	switch (stage.alarm_status){
		case nVacancy	:
			stage.alarm_cnt = 0;
			break;
		case nIN:
			if(stage.alarm_cnt > 100){
				ErrorMsg(nRedEnd);
				stage.alarm_cnt = 0;
			}
			break;
		case nREADY:
			if(stage.alarm_cnt > 100){
				ErrorMsg(nReadyError);
				stage.alarm_cnt = 0;
			}
			break;
		case nRUN:
			if(stage.alarm_cnt > 120){
				ErrorMsg(nRunningError);
				stage.alarm_cnt = 0;
			}
			break;
		case nEND:
			if(stage.alarm_cnt > 100){
				ErrorMsg(nBlueEnd);
				stage.alarm_cnt = 0;
			}
			break;
		case nReameasure:
			if( (stage.alarm_cnt > 300) && (stage.alarm_cnt < 400) ){
				stage.alarm_cnt = 500;
			}
			break;
		case nFinish:
            if(stage.alarm_cnt > 100){
				ErrorMsg(nFinishError);
				stage.alarm_cnt = 0;
			}
			break;
		case nOpbox:
		case nEmergency :
		case nManual:
            stage.alarm_cnt = 0;
		case nNoAnswer:
			stage.alarm_cnt = 0;
			break;
		default:
			break;
	}

    if(GetPlcValue(PLC_D_IROCV_TRAY_IN) == 1) ShowPLCSignal(pnlTrayIn, true);
    else ShowPLCSignal(pnlTrayIn, false);

    if(GetPlcValue(PLC_D_IROCV_PROB_OPEN) == 1) ShowPLCSignal(pnlProbeOpen, true);
    else ShowPLCSignal(pnlProbeOpen, false);

    if(GetPlcValue(PLC_D_IROCV_PROB_CLOSE) == 1) ShowPLCSignal(pnlProbeClose, true);
    else ShowPLCSignal(pnlProbeClose, false);
}
//---------------------------------------------------------------------------

// 그룹박스 보여주기
void __fastcall TTotalForm::VisibleBox(TGroupBox *grp)
{
	if(grp->Visible == false){

/*		if(grp == GrpConfig){
			grp->Left = pMain->Left;
			grp->Top = pMain->Top;
			grp->Visible = true;
			grp->BringToFront();
			return;
		}
		else{
			grp->Left = GrpMain->Left;
			grp->Top = GrpMain->Top;
		}
*/
		grp->Left = GrpMain->Left;
		grp->Top = GrpMain->Top;

		if( (grp == GrpRemeasure) || (grp == GrpError) || (grp == GrpAlarm) ){
			if(grp->Visible == false)BaseForm->IncErrorCount();
			//Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_INTERFACE3_STATE_ERROR + (this->Tag * 100), 1);
		}
		//else Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_INTERFACE3_STATE_ERROR + (this->Tag * 100), 0);
		if( (CurrentGrp == GrpRemeasure) || (CurrentGrp == GrpError) || (CurrentGrp == GrpAlarm) ){
			BaseForm->DecErrorCount();
		}

		if(bconfig != true){
			if(CurrentGrp != NULL){
				CurrentGrp->Visible = false;
			}
			grp->Visible = true;
			OldGrp = CurrentGrp;
			CurrentGrp = grp;
		}
		else{
			OldGrp = grp;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::pTrayidDblClick(TObject *Sender)
{
	editTrayId->BringToFront();
	editTrayId->Text = "";
	editTrayId->Visible = true;
	editTrayId->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::editTrayIdKeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift)
{
//	UnicodeString str;
//	if(Key == VK_RETURN){
//		str = "[" + editTrayId->Text + "]" + " Are you sure you want inspection?";
//		if(MessageBox(Handle, str.c_str(), L"", MB_YESNO|MB_ICONQUESTION) == ID_YES){
//			send.tx_mode = 0;
//			plc_Barcode();
//			if(LoadTrayInfo(editTrayId->Text))
//			{
//				CmdBattHeight();
//			}
//			else ProcessError("Tray Information", "Error", "The tray information file does not exist.", "Check the barcode ID and file information.");
//		}
//		editTrayId->Visible = false;
//	}
//	if(Key == VK_ESCAPE){
//		editTrayId->Visible = false;
//	}
}
//---------------------------------------------------------------------------
// Common_comm.h
void __fastcall TTotalForm::OnReceiveStage(TMessage& Msg)
{
	AnsiString *msg, param;
	int cmd = 0;
	msg = (AnsiString*)Msg.LParam;
	int nvalue = 0;

//	if((stage.err == NO_ANSWER) && (GrpError->Visible)){
//		this->VisibleBox(OldGrp);
//	}

	try{

		if(msg->Trim().IsEmpty()){
			return;
		}
		WriteCommLog("RX", *msg);
		cmd = DataCheck(*msg, param); 	// cmd , check sum 확인

		send.time_out = 0;

        //if(!ims->m_bStart) return;      // 수정

		switch(cmd){
			case STA:
				send.tx_mode = 0;
				 break;
			case DEV:
				if(param.Pos(",1,") > 0)pdev1->Color = clBothCheck;
				else pdev1->Color = clSilver;
				if(param.Pos(",2,") > 0)pdev2->Color = clBothCheck;
				else pdev2->Color = clSilver;
				if(param.Pos(",3,") > 0)pdev3->Color = clBothCheck;
				else pdev3->Color = clSilver;
				if(param.Pos(",4,") > 0)pdev4->Color = clBothCheck;
				else pdev4->Color = clSilver;
				if(param.Pos(",11,") > 0)pdev5->Color = clBothCheck;
				else pdev5->Color = clSilver;
				if(param.Pos(",12,") > 0)pdev6->Color = clBothCheck;
				else pdev6->Color = clSilver;
				if(param.Pos(",13,") > 0)pdev7->Color = clBothCheck;
				else pdev7->Color = clSilver;
				if(param.Pos(",14,") > 0)pdev8->Color = clBothCheck;
				else pdev8->Color = clSilver;

				for(int i=0; i<8; ++i){
					if(pdev[i]->Color == clBothCheck)nvalue += 1;
				}

				if(nvalue < 2)ProcessError("MEASUREMENT", "ERROR","Measurement error", "");
				else{
					CmdAutoTest();
				}
				break;
			case BCR:
				if( (param.Pos("?") == 0) && (param != "NOREAD") ){ 	// 바코드 읽기 성공
					//pTrayid->Caption = param;
					editTrayId->Text = param;
					send.tx_mode = 0;
				}
				else{
					if(send.re_send < 2){
						send.re_send += 1;
						send.tx_mode = 3;
					}
					else{
						send.tx_mode = 0;
						send.re_send = 0;
						ErrorMsg(BARCODE_ERROR);
					}
				}
				break;
			case IDN:        // 버전
				pConInfo->Caption = param;
				send.tx_mode = 0;
				break;
			case RST:
				send.tx_mode = 0;
				ErrorMsg(RESET);
                Initialization(); // 2017 09 04 herald
				//SendData("STA");
				break;        // 모든 에러 해제
			case SIZ:
				send.tx_mode = 0;
				DisplayStatus(nREADY);
				break;        // BATT 사이즈 정보
			case AMS:
				pb->Position = 0;
				send.tx_mode = 200;
				tray.ams = true;
                tray.amf = false;
				DisplayStatus(nRUN);
				break;
			case AMF:        // 검사종료 알림
				send.tx_mode = 0;
				tray.ams = false;
				tray.amf = true;

				ResponseAutoTestFinish();
				break;
			case IR:        // IR 셀 검사
				if(pb->Position < pb->Max)pb->Position += 1;
				ProcessIr(param);	// 읽기 , 색깔 변화
				break;
			case OCV:        // OCV 셀 검사
				ProcessOcv(param);
				break;
			case STP:        // 강제 검사 종료
				send.tx_mode = 0;
				DisplayStatus(nEND);
				break;
			case FIN:       // 트레이 방출
				//this->DisplayStatus(nFinish);
				ModChange();
				send.tx_mode = 0;
				break;
			// 검사장치 송신에 대한 PC응답 메세지 처리
			case MAN: send.tx_mode = 0; break;
			case REM: send.tx_mode = 0; break;
			case EMS: send.tx_mode = 0; break;
			case SEN:        // 센서정보
				SensorInputProcess(param);
				break;
			case sOUT:
				SensorOutputProcess(param);
				break;
			case ERR:        // 검사장치 에러 발생
				ResponseError(param);
				OldSenCmd = "NONE";
				send.tx_mode = 0;
				break;
			case CLR:
				SendData("CLR");
				OldSenCmd = "NONE";
				VisibleBox(OldGrp);
				break;       // 에러 해제 통보
			case REC: send.tx_mode = 0; break;
			case LRM:
				send.tx_mode = 0;
				StageLocalRemeasure();
				break;
			default:
				this->WriteCommLog("ERR", "Undefined Command");
				send.tx_mode = 0;
				break;
		}
	}catch(...){
		this->WriteCommLog("ERR", "Except Error : " + *msg);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::StageLocalRemeasure(bool frm)
{
	// OP 박스 재측정 요청시
	SendData("LRM");
	
	if(GrpRemeasure->Visible == true){
		VisibleBox(GrpMain);

		if(retest.cnt_error > remLimit){
			retest.re_excute = false;	// 전체 재측정
		}
		else{
			retest.re_excute = true;	// 불량 재측정
		}
		CmdBattHeight();
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::CmdTestMode()
{
	if(retest.cnt_error > 0){
		tray.rem_mode = 0;
		retest.cnt_remeasure += 1;
		if(retest.cnt_remeasure == 1)acc_cnt += 1;

		if(retest.re_excute == false){
			CmdDeviceInfo();	// 전체 재측정
		}
		else{
			SetRemeasureList();	// 개별 재측정
		}
	}
	else{
		CmdDeviceInfo();	// IR/OCV 검사
	}
}
//---------------------------------------------------------------------------
// 개별 재측정
//* 트레이 위치에 따라 맵핑 다르게 적용
void __fastcall TTotalForm::RemeasureExcute()
{
	int i = retest.re_index;
	int ch = 0, boardch = 0;
	for(;i < MAXCHANNEL;++i){
		ch = i + 1;
		boardch = chReverseMap[ch] - (nTrayPos * 288);
		switch(retest.cell[i]){
			case '2':	// IR 불량
				this->MakeData(3, "IR*", FormatFloat("000", boardch));
				if(tray.ocv_value[i] < config.ocv_min || tray.ocv_value[i] > config.ocv_max){
					retest.cell[i] = '3';
				}else{
					retest.re_index = ++i;
				}
				return;
			case '3':	// OCV 불량
				this->MakeData(3, "OCV", FormatFloat("000", boardch));
				if(tray.after_value[i] < config.ir_min || tray.after_value[i] > config.ir_max){
					retest.cell[i] = '2';
				}
				retest.re_index = ++i;
				return;
			default:
				break;
		}
	}
	retest.re_excute = false;
	SetRemeasureList();
	CmdForceStop();
	WriteCommLog("IR/OCV STOP", "RemeasureExcute()");
}
//---------------------------------------------------------------------------
//* 예전 버전
void __fastcall TTotalForm::RemeasureExcute2()
{
	int i = retest.re_index;
	int value = 0;
	for(;i<MAXCHANNEL;++i){
		value = i + 1;
		value = chReverseMap[value];
		switch(retest.cell[i]){
			case '2':	// IR 불량
				this->MakeData(3, "IR*", FormatFloat("000", value));
				if(tray.ocv_value[i] < config.ocv_min || tray.ocv_value[i] > config.ocv_max){
					retest.cell[i] = '3';
				}else{
					retest.re_index = ++i;
				}
				return;
			case '3':	// OCV 불량
				this->MakeData(3, "OCV", FormatFloat("000", value));
				if(tray.after_value[i] < config.ir_min || tray.after_value[i] > config.ir_max){
					retest.cell[i] = '2';
				}
				retest.re_index = ++i;
				return;
			default:
				break;
		}
	}
	retest.re_excute = false;
	SetRemeasureList();
	CmdForceStop();
	WriteCommLog("IR/OCV STOP", "RemeasureExcute()");
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ModChange()
{
	if(stage.arl != stage.arl_reserve){
		stage.arl = stage.arl_reserve;
		switch(stage.arl){
			case nAuto:
				this->CmdManualMod(false);
				VisibleBox(GrpMain);
				break;
			case nRemote:
				this->CmdManualMod(false);
				break;
			case nLocal:
				this->CmdManualMod(true);
				stage.alarm_status = nManual;
				break;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::SensorInputProcess(AnsiString param)
{
	AnsiString cmd;
	cmd = param.SubString(1,3);
	param.Delete(1,3);

	unsigned char *ptrInput;
	ptrInput = (unsigned char *)&sensor;

	while(param.IsEmpty() == false){
		*ptrInput = (unsigned char)StrToInt("0x" + param.SubString(1,2));
		ptrInput++;
		param.Delete(1,2).Trim();
	}

	//DisplaySensorInfo();

	if(stage.init == true){
		lblStatus->Caption = cmd;
		InitEquipStatus(SensorState(cmd));
		OldSenCmd = cmd;
		stage.init = false;
	}
	else if(cmd != OldSenCmd){
		lblStatus->Caption = cmd;
		EquipStatus(SensorState(cmd));
		OldSenCmd = cmd;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::SensorOutputProcess(AnsiString param)
{
	unsigned char *ptrOutput;

	ptrOutput = (unsigned char *)&sensor_out;


	while(param.IsEmpty() == false){
		*ptrOutput = (unsigned char)StrToInt("0x" + param.SubString(1,2));
		ptrOutput++;
		param.Delete(1,2).Trim();
	}
	//DisplaySensorInfo();
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::EquipStatus(int cmd)
{
	double temp;
	switch(cmd)
	{
//		case ERR:
//			MakeData(2,"ERR");
//			break;
//		case ARV:
//			pb->Position = 0;
//			//DisplayStatus(nIN);
//			if(chkBypass->Checked == false){
//				plc_Barcode();
//				// tray id 확인    (PLC ) 추가
//				// cell 유/무 확인 (PLC )  추가
//				//LoadTrayInfo() 수정 필요
//				CmdBattHeight();
//				/*
//				if(LoadTrayInfo(Mod_PLC->GetString(Mod_PLC->plc_Interface_Data, PLC_INTERFACE3_TRAY_ID + (this->Tag * 100), 10)))
//				{
//					CmdBattHeight();
//				}
//				else ProcessError("Tray Information", "Error", "The tray information file does not exist.", "Check the barcode ID and file information.");
//				*/
//			}else{
//				CmdTrayOut();
//			}
//			break;
//		case RDY:
//			if(chkCycle->Checked == false)CmdTestMode();
//			else CmdForceStop();
//			break;
//		case STB:
//			if(chkCycle->Checked == true){
//				CmdTrayOut();
//			}
//			else if(tray.trayid != "start"){
//				// 1. 재측정이 발생했으면
//				if(retest.cnt_error > 0){
//					// 2. 자동 재측정을 사용하면 && 횟수가 지정 횟수 이하이면
//					if(config.remeasure_use && retest.cnt_remeasure < config.remeasure_cnt){
//						// 3.1 프로브 닫고
//						if(retest.cnt_error > remLimit)retest.re_excute = false;
//						else retest.re_excute = true;
//
//						CmdBattHeight();
//					}
//					else{
//						// 3.2 재측정 후 리스트를 SHOW 하거나 트레이를 배출한다
//						if(config.remeasure_bypass == false)AddRemeasureList();
//						else
//						{
//							double ngLimit = tray.cell_count / 3.0;
//							if(retest.cnt_error > ngLimit)
//							{
//								ProcessError("NG Cell", "Error", "There are more than 30% NG cells.", "Please check the specifications and cell status.");
//							}
//							else ims->WriteMsg_TrayFinish(m_Equip_Info, tray, retest);		// 수정
////							CmdTrayOut();
//						}
//					}
//				}else{
////					ims->WriteMsg_TrayFinish(m_Equip_Info, tray, retest);		// 수정
////					CmdTrayOut();
//				}
//			}
//			break;
		case HOM:
//			DisplayStatus(nVacancy);
//			VisibleBox(GrpMain);
			break;

		case MAN:
//			if(GrpLocal->Visible == false){
//				stage.arl = nLocal;
//				VisibleBox(GrpLocal);
//			}
//			DisplayStatus(nManual);
//			InitMeasureForm();
			break;
		case EMS:
			//DisplayStatus(nEmergency);
			VisibleBox(GrpMain);
			break;
		case LOC:
			//DisplayStatus(nOpbox);
			VisibleBox(GrpMain);
			break;
		case EMP:
			//DisplayStatus(nVacancy);
			break;
		case RST:
			CmdStart();
			OldSenCmd = "NONE";
			break;
		case IDL:
//			if( OldSenCmd != "ARV"){
//				CmdStart();
//			}
//			else{
//				DisplayStatus(nIdle);
//			}
			break;
		case BZY:
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::InitEquipStatus(int cmd)
{

	switch(cmd)
	{
		case RDY:
			//this->CmdForceStop();
			//this->DisplayStatus(nIN);
			//ProcessError("Tray In", "",  "Please select the following actions : ", "Inspection start or  eject tray");
			break;
		case ARV:
		case STB:
			//this->DisplayStatus(nIN);
			//ProcessError("Tray In", "",  "Please select the following actions : ", "Inspection start or  eject tray");
			break;
		case MAN:
			stage.alarm_status = nManual;
			stage.arl = nLocal;
			VisibleBox(GrpLocal);
			DisplayStatus(nManual);
			break;
		case BZY:
			//ProcessError("STAGE", "BUSY", "On stage is a work in progress", "");
			break;
		default:
			EquipStatus(cmd);
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnResetClick(TObject *Sender)
{
	if(MessageBox(Handle, L"Are you sure you want to reset?", L"RESET", MB_YESNO|MB_ICONQUESTION) == ID_YES){
		this->CmdReset();
		send.time_out = 0;
		OldSenCmd = "NONE";
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ChInfoMouseEnter(TObject *Sender)
{
	TPanel *pnl;
	pnl = (TPanel*)Sender;

	pPos->Caption = pnl->Hint;

	pIrValue->Caption = FormatFloat("0.00",tray.after_value[pnl->Tag]);
	pOcvValue->Caption = FormatFloat("0.0",tray.ocv_value[pnl->Tag]);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ChInfoMouseLeave(TObject *Sender)
{
	TPanel *pnl;
	pnl = (TPanel*)Sender;

	pPos->Caption = "";
	pIrValue->Caption = "";
	pOcvValue->Caption = "";
	
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::MainBtnClick(TObject *Sender)
{
	switch(stage.arl){
		case nAuto:
			VisibleBox(GrpMain);
			break;
		case nLocal:
			VisibleBox(GrpLocal);
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::chkBypassMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if(chkBypass->Checked && Button == mbLeft){
		if(MessageBox(Handle, L"Do yoy want change BYPASS mode?", L"BYPASS", MB_YESNO|MB_ICONQUESTION) == ID_NO){
			chkBypass->Checked = false;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnManualClick(TObject *Sender)
{
	stage.arl_reserve = nLocal;
	stage.arl = nLocal;
//    Timer_AutoInspection->Enabled = false;
	this->CmdManualMod(true);
	VisibleBox(GrpLocal);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnAutoClick(TObject *Sender)
{
	stage.arl_reserve = nAuto;
	stage.arl = nAuto;
	bLocal = false;
//    Timer_AutoInspection->Enabled = false;
    MeasureInfoForm->pLocal->Visible = false;
	this->CmdManualMod(false);
	VisibleBox(GrpMain);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::SetRemeasureList()
{
	bool brem = false;
	int remeasure_cnt = 0;
	double irMin, irMax, ocvMin, ocvMax;

	if(stage.arl == nAuto){
		retest.cnt_error = 0;

		if(tray.ir_avgAll_count == 0) tray.ir_avg = (config.ir_min + config.ir_max) / 2;
		else tray.ir_avg = tray.ir_avgAll / tray.ir_avgAll_count;

		if(tray.ocv_avgAll_count == 0) tray.ocv_avg = (config.ocv_min + config.ocv_max) / 2;
		else tray.ocv_avg = tray.ocv_avgAll / tray.ocv_avgAll_count;

		editIrAvg->Text = FormatFloat("0.00", tray.ir_avg);
		editOcvAvg->Text = FormatFloat("0.00", tray.ocv_avg);

		config.ir_range = BaseForm->StringToDouble(editIrRange->Text, 3);
		config.ocv_range = BaseForm->StringToDouble(editOcvRange->Text, 1000);

		if(tray.ir_avgAll_count < 2) tray.ir_sigma = 1;
		else tray.ir_sigma = GetSigma(tray.after_value, tray.ir_flag, tray.ir_avg, tray.ir_avgAll_count - 1);

		if(tray.ocv_avgAll_count < 2) tray.ocv_sigma = 1;
		else tray.ocv_sigma = GetSigma(tray.ocv_value, tray.ocv_flag, tray.ocv_avg, tray.ocv_avgAll_count - 1);

		irMin = tray.ir_avg - config.ir_range * tray.ir_sigma;
		irMax = tray.ir_avg + config.ir_range * tray.ir_sigma;

		ocvMin = tray.ocv_avg - config.ocv_range * tray.ocv_sigma;
		ocvMax = tray.ocv_avg + config.ocv_range * tray.ocv_sigma;
		 //* 색상도 바꿔야 함....
		for(int index = 0;index < MAXCHANNEL;++index){
			if(tray.cell[index] == 1){

				if(tray.after_value[index] < config.ir_min || tray.after_value[index] > config.ir_max)
				{
					retest.cell[index] = '2'; // ir spec ng
					SetProcessColor(index, AverageOver);
					retest.cnt_error += 1;
					remeasure_cnt += 1;
					if(tray.first) acc_remeasure[index] += 1;
				}
				else if((tray.after_value[index] >= config.ir_min && tray.after_value[index] <= config.ir_max)
					&& (tray.after_value[index] < irMin || tray.after_value[index] > irMax)){
					retest.cell[index] = '5'; // ir average ng
					SetProcessColor(index, AverageOver);
					retest.cnt_error += 1;
					remeasure_cnt += 1;
					if(tray.first) acc_remeasure[index] += 1;
				}
				else if(tray.ocv_value[index] < 100 || tray.ocv_value[index] > 4200)
				{
					if(retest.cell[index] != '2' || retest.cell[index] != '5'){
						retest.cell[index] = '3';  // ocv spec ng
						retest.cnt_error += 1;
						remeasure_cnt += 1;
						if(tray.first) acc_remeasure[index] += 1;
					}
				}
				else if((tray.ocv_value[index] >= 100 && tray.ocv_value[index] <= 4200)
					&& (tray.ocv_value[index] < ocvMin || tray.ocv_value[index] > ocvMax)){
					if(retest.cell[index] != '2' || retest.cell[index] != '5'){
						retest.cell[index] = '6'; // ocv average ng
						retest.cnt_error += 1;
						remeasure_cnt += 1;
						if(tray.first) acc_remeasure[index] += 1;
					}
				}else{
					retest.cell[index] = '0';
				}
			}
			else retest.cell[index] = '0';
		}

		tray.first = false;

		if((remeasure_cnt < remLimit) && (remeasure_cnt > 0)) brem = true;
		else brem= false;

		if(brem == false){
			CmdForceStop();     // Probe Open
		}
		else{
			tray.rem_mode = 1;
			retest.re_index = 0;
			RemeasureExcute();
		}
	} else {
		CmdForceStop();
	}

	WriteCommLog("IR/OCV STOP", "SetRemeasureList()");
}
//---------------------------------------------------------------------------
double __fastcall TTotalForm::GetSigma(float values[], bool flags[], double avg, int ncount)
{
	double sigma = 0.0, sum = 0.0;
	for(int nIndex = 0; nIndex < MAXCHANNEL; nIndex++)
	{
		if(flags[nIndex] == true){
			sum += (values[nIndex] - avg) * (values[nIndex] - avg);
		}
	}

	sigma = sqrt(sum / ncount);
    return sigma;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::SetRemeasureList2()
{
	bool brem = false;
	int remeasure_cnt = 0;

	if(stage.arl == nAuto){
		retest.cnt_error = 0;

		for(int index=0;index<MAXCHANNEL;++index){
			if(tray.cell[index] == 1){
				if(tray.after_value[index] < config.ir_min || tray.after_value[index] > config.ir_max){
					retest.cell[index] = '2';
					retest.cnt_error += 1;
					remeasure_cnt += 1;
					if(tray.first){
						acc_remeasure[index] += 1;
					}
				}
				else if(tray.ocv_value[index] < config.ocv_min || tray.ocv_value[index] > config.ocv_max){
					if(retest.cell[index] != '2'){
						retest.cell[index] = '3';
						retest.cnt_error += 1;
						remeasure_cnt += 1;
						if(tray.first)acc_remeasure[index] += 1;
					}
				}else{
					retest.cell[index] = '0';
				}
			}
			else retest.cell[index] = '0';
		}

        tray.first = false;

		if((remeasure_cnt < remLimit) && (remeasure_cnt > 0)) brem = true;
		else brem= false;

		if(brem == false){
			CmdForceStop();     // Probe Open
		}
		else{
			tray.rem_mode = 1;
			retest.re_index = 0;
			RemeasureExcute();
		}
	} else {
		CmdForceStop();
	}

	WriteCommLog("IR/OCV STOP", "SetRemeasureList()");
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ViewRemeasureList()
{
	// 1. 재측정이 발생했으면
	if(retest.cnt_error > 0){
		// 2. 자동 재측정을 사용하면 && 횟수가 지정 횟수 이하이면
		if(RemeasureChk->Checked == true && retest.cnt_remeasure < config.remeasure_cnt){

			if(retest.cnt_error > remLimit)retest.re_excute = false;
			else retest.re_excute = true;
			// 3.1 Remeasure 자동 시작
			if(retest.re_excute == true) RemeasureBtnClick(this);
			else if(retest.re_excute == false) RemeasureAllBtnClick(this);
		}
		else{
			// 3.2 재측정 후 리스트를 SHOW 하거나 트레이를 배출한다
			if(chkRemBypass->Checked == true) nStep = 3;
			else AddRemeasureList();
		}
	}
	else
		nStep = 3;

}
void __fastcall TTotalForm::AddRemeasureList()
{
	stage.alarm_status = nReameasure;
	TListItem	*ITEM;
	BadList->Items->BeginUpdate();
	BadList->Items->Clear();
	for(int i=0; i<MAXCHANNEL; ++i){
		switch(retest.cell[i]){
			case '2':
				ITEM = BadList->Items->Add();
				ITEM->Caption = BadList->Items->Count;
				ITEM->SubItems->Add(IntToStr(i+1) + " (" + IntToStr((i+16)/16) + "-" + IntToStr((i%16)+1)+ ")");
				ITEM->SubItems->Add(FormatFloat("0.00", tray.after_value[i]));
				ITEM->SubItems->Add(FormatFloat("0.0", tray.ocv_value[i]));
				ITEM->SubItems->Add("IR");
				ITEM->SubItems->Add(acc_remeasure[i]);
				break;
			case '3':
				ITEM = BadList->Items->Add();
				ITEM->Caption = BadList->Items->Count;
				ITEM->SubItems->Add(IntToStr(i+1) + " (" + IntToStr((i+16)/16) + "-" + IntToStr((i%16)+1)+ ")");
				ITEM->SubItems->Add(FormatFloat("0.00", tray.after_value[i]));
				ITEM->SubItems->Add(FormatFloat("0.0", tray.ocv_value[i]));
				ITEM->SubItems->Add("OCV");
				ITEM->SubItems->Add(acc_remeasure[i]);
				break;
		}
	}
	BadList->Items->EndUpdate();
	lblRemeasureTime->Caption = BadList->Items->Count;
	VisibleBox(GrpRemeasure);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::SetTrayID(AnsiString str_id)
{
//	tray.trayid = str_id.Trim();
//	if(tray.trayid.IsEmpty()){
//		tray.trayid = Now().FormatString("hhnnss");
//	}
//	DisplayTrayInfo();
//   //	pTrayid->Caption = tray.trayid;
//	editTrayId->Text = tray.trayid;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::WriteIROCVValue()
{
	// ir value 1 Word => Max Value = 65535
	for(int i = 0; i < MAXCHANNEL; i++)
	{
		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_IR_VALUE + i, FormatFloat("0000", (tray.after_value[i] * 100)) % (256 * 256));
	}

	for(int i = 0; i < MAXCHANNEL; i++)
	{
		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Ocv_Data, PC_D_IROCV_OCV_VALUE + i, FormatFloat("00000", (tray.ocv_value[i] * 10)) % (256 * 256));
	}

}
void __fastcall TTotalForm::BadInfomation()
{
	int ngCount = 0;
    NgCount = 0;
	for(int i = 0; i < LINECOUNT; ++i){
        int irocvNg = 0;
		for(int j = 0; j < LINECOUNT; j++)
		{
			if((tray.cell[(i * LINECOUNT) + j] == 1) && (panel[(i * LINECOUNT) + j]->Color == clCellError))
			{
				//Mod_PLC->SetData(Mod_PLC->pc_Interface_Data, PC_D_IROCV_MEASURE_OK_NG + i, j, true);
                //* ng -> true
                irocvNg |= 1 << j;
				ngCount++;
				NgCount++;
			}
			else if((tray.cell[(i * LINECOUNT) + j] == 1) && retest.cell[(i * LINECOUNT) + j] == '0')
			{
				//Mod_PLC->SetData(Mod_PLC->pc_Interface_Data, PC_D_IROCV_MEASURE_OK_NG + i, j, false);
			}
			else
			{
                //* ng -> true
                irocvNg |= 1 << j;
				//Mod_PLC->SetData(Mod_PLC->pc_Interface_Data, PC_D_IROCV_MEASURE_OK_NG + i, j, true);
				ngCount++;
			}
		}
        Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_MEASURE_OK_NG + (i * 2), irocvNg);
	}

	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_NG_COUNT, ngCount);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::WriteIRMINMAX()
{

	irMin = irEdit1->Text.ToDouble();
	irMax = irEdit2->Text.ToDouble();
	ocvMin = ocvEdit1->Text.ToDouble();
	ocvMax = ocvEdit2->Text.ToDouble();
	//* 2021 02 05

	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_IR_MIN, irMin);
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_IR_MAX, irMax);
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_OCV_MIN, ocvMin);
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_OCV_MAX, ocvMax);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::plc_Barcode()
{
//	InitTrayStruct();
//	SetTrayID("");

//	int add = this->Tag * 200;
//	plc->ReadData(8110+add, 10, "A8");

//	CmdBattHeight();						// 수정
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnMeasureInfoClick(TObject *Sender)
{
    InitMeasureForm();

	MeasureInfoForm->Left = 640;
	MeasureInfoForm->Top = 85;

    int ch;
	for(int i = 0; i < MAXCHANNEL; ++i){
        MeasureInfoForm->pir[i]->Caption = "-";
        MeasureInfoForm->pir[i]->ParentBackground = false;
        MeasureInfoForm->pocv[i]->Caption = "-";
        MeasureInfoForm->pocv[i]->Color = pnormal2->Color;

        //* 채널 위치 -> 릴레이가 12줄이므로 위치를 계산해야 함
        ch = chReverseMap[i + 1];
        if(ch >= 289) ch  = ch - 288;
        MeasureInfoForm->pir[i]->Hint = "POS : " + IntToStr((ch - 1)/LINECOUNT + 1) + "-" + IntToStr((ch - 1)%LINECOUNT + 1);
        MeasureInfoForm->pocv[i]->Hint = "POS : " + IntToStr((ch - 1)/LINECOUNT + 1) + "-" + IntToStr((ch - 1)%LINECOUNT + 1);
	}	// 모두 초기화
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TTotalForm::Timer_AutoInspectionTimer(TObject *Sender)
{
    if(ErrorCheck()) return;
	if(stage.arl == nAuto && Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_STAGE_AUTO_READY) == 0){
		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_STAGE_AUTO_READY, 1);
		IROCVStage = "IROCV STAGE AUTO READY = 1";
		WritePLCLog("IROCV STAGE AUTO/MANUAL", IROCVStage);
	}
	else if(stage.arl == nLocal && Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_STAGE_AUTO_READY) == 1){
		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_STAGE_AUTO_READY, 0);

		IROCVStage = "IROCV STAGE AUTO READY = 0";
		WritePLCLog("IROCV STAGE AUTO/MANUAL", IROCVStage);
	}

	/* 2021-08-23 comment for test

	{
		Panel_State->Color = clRed;
		Panel_State->Font->Color = clWhite;
		return;
	}
	else
	{
		Panel_State->Color = clWhite;
		Panel_State->Font->Color = clBlack;
	}
	*/
	switch(nSection)
	{
		case STEP_WAIT:
			AutoInspection_Wait();
			break;
		case STEP_MEASURE:
			AutoInspection_Measure();
			break;
		case STEP_FINISH:
			AutoInspection_Finish();
			break;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TTotalForm::ErrorCheck()
{
    if(!Client->Active)
	{
	  Panel_State->Caption = "IR/OCV Connection Fail.";
	   return true;
	}
    else {
        Panel_State->Caption = "";
		if(stage.alarm_status == nNoAnswer){
			DisplayStatus(nVacancy);
			DisplayProcess(sReady, "AutoInspection_Wait", " IR/OCV is ready... ");
		}
    }


	if(!Mod_PLC->ClientSocket_PC->Active && !Mod_PLC->ClientSocket_PLC->Active)
	{
		ErrorCheckStatus = "PLC - PC Connection Fail.";
		Panel_State->Caption = ErrorCheckStatus;
		if(OldErrorCheckStatus != ErrorCheckStatus) {
			OldErrorCheckStatus = ErrorCheckStatus;
			WritePLCLog("ErrorCheck", ErrorCheckStatus);
		}
		return true;
	}

	if(Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_ERROR))
	{
		ErrorCheckStatus = "PLC - Error!!";
		Panel_State->Caption = ErrorCheckStatus;
		if(OldErrorCheckStatus != ErrorCheckStatus) {
			OldErrorCheckStatus = ErrorCheckStatus;
			WritePLCLog("ErrorCheck", ErrorCheckStatus);
		}

        //* 2025 04 08 plc 연결이 안되면 아래 코드 때문에 수동측정도 못함.
//		CmdForceStop_Original();
//		Initialization();
		return true;
	}



	if(bLocal == true && Mod_PLC->GetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_STAGE_AUTO_READY) == 0)
	{
		Panel_State->Caption = "IR/OCV is not in AutoMode";
		return true;
	}

	return false;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::AutoInspection_Wait()
{
	AnsiString trayid;
	switch(nStep)
	{
		case 0: // Tray In Check // Remeasure Count
			if(Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_TRAY_IN))
			{
				if(chkBypass->Checked == true)
				{
					DisplayProcess(sTrayIn, "AutoInspection_Wait", "Bypass ...");
					CmdTrayOut();                                  // badinformation, writeresultfile, trayout
					nStep = 0;
					nSection = STEP_FINISH;
				}
				else
				{
					DisplayProcess(sTrayIn, "AutoInspection_Wait", "IR/OCV Tray In ...");

					PLCInitialization();
					InitTrayStruct();
					DisplayStatus(nREADY);
					nStep = 1;
				}
			}
			else {
				DisplayStatus(nVacancy);
				DisplayProcess(sReady, "AutoInspection_Wait", " IR/OCV is ready... ");
			}
			break;
		case 1: //BarCode
			m_dateTime = Now();
			trayid = Mod_PLC->GetString(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_TRAY_ID, 10);

			if(trayid != "")
			{
				DisplayProcess(sBarcode, "AutoInspection_Wait", "BCR OK ...(" + trayid + ")");

                pTrayid->Caption = trayid;
				editTrayId->Text = trayid;
				tray.trayid      = trayid;
				nStep = 2;
			}
			else
			{
				DisplayProcess(sBarcode, "AutoInspection_Wait", " BCR Error ... ", true);
				return;
			}
			break;
		case 2:
            DisplayStatus(nREADY);
			tray.cell_count = 0;
			for(int i = 0; i < LINECOUNT; i++)
            {
                for(int j = 0; j < LINECOUNT; j++)
                {
					tray.cell[i * LINECOUNT + j] = Mod_PLC->GetData(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_TRAY_CELL_DATA + (i * 2), j);
                    tray.cell_count += tray.cell[i * LINECOUNT + j];
                }
            }

			nStep = 3;
			break;
		case 3:    // CELL DATA
			start_delay_time++;
//			Panel_State->Caption = IntToStr(start_delay_time);
			if(tray.cell_count > 0 || chkCycle->Checked)
			{
                WriteTrayInfo();
				DisplayTrayInfo();

				if(start_delay_time > 1){

					DisplayProcess(sProbeDown, "AutoInspection_Wait", " PROBE IS CLOSED ... ");
					Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_PROB_CLOSE, 1);

                    WriteCommLog("AutoInspection_Wait", "PC_INTERFACE_PROB_CLOSE ...");
					WritePLCLog("AutoInspection_Wait", "PC_D_IROCV_PROB_CLOSE = 1");

					nSection = STEP_MEASURE;
					nStep = 0;
					nStepCount = 0;
					start_delay_time = 0;
				}
			}
			else
			{
				DisplayProcess(sBarcode, "AutoInspection_Wait", " NO CELL ... ", true);
				return;
			}
			break;
		default:
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::AutoInspection_Measure()
{
    //* Measuring
	if(tray.ams == true && tray.amf == false){
		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_MEASURING, 1);
		DisplayProcess(sMeasure, "AutoInspection_Measure", " Start measuring ... ");
	}
	else{
		Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_MEASURING, 0);
		DisplayProcess(sProbeDown, "AutoInspection_Measure", " Ready to measure ... ");
	}

	double plc_probe_close, plc_tray_in, plc_probe_open;
	switch(nStep)
	{
		case 0:
			plc_probe_close = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_PROB_CLOSE);
			plc_tray_in = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_TRAY_IN);
			if(plc_probe_close == 1 && plc_tray_in == 1)
			{
				DisplayStatus(nRUN);
                if(n_bMeasureStart == false)
				DisplayProcess(sProbeDown, "AutoInspection_Measure", "PLC - PROBE CLOSED");

				Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_PROB_CLOSE, 0);

				if(n_bMeasureStart == false)
				{
					DisplayProcess(sMeasure, "AutoInspection_Measure", "IR/OCV Measure Start");
					CmdAutoTest();     // ams -> amf -> if cell-error (< 10) then auto-remeasure
					n_bMeasureStart = true;
				}
				nStep = 3;

			}
			break;
		case 1:
			n_bMeasureStart = false;
			plc_probe_open = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_PROB_OPEN);
			if(stage.arl == nAuto && plc_probe_open == 1){
				ViewRemeasureList();
			}
            break;
		case 2:
			PLCStatus = " IR/OCV Remeasure ... ";
			Panel_State->Caption = PLCStatus;

            nTrayPos = GetTrayPos();
			plc_probe_close = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_PROB_CLOSE);
			plc_tray_in = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_TRAY_IN);

			DisplayStatus(nRUN);
			if(plc_probe_close == 1 && plc_tray_in == 1)
			{
				DisplayProcess(sProbeDown, "AutoInspection_Measure", "PLC - PROBE CLOSED");

				Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_PROB_CLOSE, 0);
				DisplayProcess(sMeasure, "AutoInspection_Measure", "IR/OCV Re-Measure Start");
				if(retest.re_excute) RemeasureExcute();  // partly remeasure
				else CmdAutoTest();       // all remeasure

				nStep = 3;
			}
			break;
		case 3:
            n_bMeasureStart = false;
			plc_probe_open = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_PROB_OPEN);
			if(stage.arl == nAuto && plc_probe_open == 1)
			{
				Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_PROB_OPEN, 0);

				DisplayProcess(sProbeOpen, "AutoInspection_Measure", " PLC - PROBE IS OPEN ... ");
				WriteCommLog("AutoInspection_Measure", "IR/OCV Finish... ");

				CmdTrayOut();                                  // badinformation, writeresultfile, trayout
				nStep = 0;
				nSection = STEP_FINISH;
			}
			break;
		default:
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::AutoInspection_Finish()
{
	Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_MEASURING, 0);

	double plc_tray_in;
	DisplayStatus(nFinish);
	switch(nStep)
	{
		case 0:
			plc_tray_in = Mod_PLC->GetDouble(Mod_PLC->plc_Interface_Data, PLC_D_IROCV_TRAY_IN);
			if(plc_tray_in == 0)
			{
				WriteCommLog("AutoInspection_Finish", "TRAY OUT");
				Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_TRAY_OUT, 0);
				Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_PROB_OPEN, 0);
				Mod_PLC->SetDouble(Mod_PLC->pc_Interface_Data, PC_D_IROCV_PROB_CLOSE, 0);

				DisplayProcess(sTrayOut, "AutoInspection_Finish", " IR/OCV Tray Out ... ");

                //* Tray ID 초기화
				pTrayid->Caption = "";
				editTrayId->Text = "";

				nSection = STEP_WAIT;
				nStep = 0;

			}
			break;
		default:
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ResultReportToPLC()
{
	WriteResultFile();
//    WriteOKNG();
	DeleteFile((AnsiString)DATA_PATH + tray.trayid + ".Tray");
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ReadchannelMapping()
{
	AnsiString str, FileName;
	int file_handle;

	FileName = (AnsiString)BIN_PATH + "mapping.csv";

	TStringList *data;
	data = new TStringList;

	if (FileExists(FileName)) {

		data->LoadFromFile(FileName);

		for (int i = 1; i <= MAXCHANNEL; ++i) {
			str = data->Strings[i];
			str.Delete(1, str.Pos(",")); // 채널
			chMap[i] = str.ToInt();
			chReverseMap[str.ToInt()] = i;
		}
	}
	else {
		data->Add("변경전, 변경후");
		for (int i = 1; i <= MAXCHANNEL; ++i) {
			chMap[i] = i;
            chReverseMap[i] = i;
			data->Add(IntToStr(i) + "," + IntToStr(i));
		}
		data->SaveToFile(FileName);

	}
	delete data;
}
// ---------------------------------------------------------------------------
void __fastcall TTotalForm::btnConfigClick(TObject *Sender)
{
	pnlConfig->Visible = !pnlConfig->Visible;
	pnlConfig->Left = 10;
	pnlConfig->Top = 50;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TTotalForm::btnPLCConnectClick(TObject *Sender)
{
	AnsiString plc_ip = editPLCIPAddress->Text;
	int port1 = editPLCPortPC->Text.ToInt();
	int port2 = editPLCPortPLC->Text.ToInt();
//	Mod_PLC->Connect(plc_ip, port1, port2);
}
//---------------------------------------------------------------------------

void __fastcall TTotalForm::btnPLCDisconnectClick(TObject *Sender)
{
	Mod_PLC->DisConnect();
}
//---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

void __fastcall TTotalForm::localCaliClick(TObject *Sender)
{
	CaliForm->stage = this->Tag;
	CaliForm->pstage->Caption = lblTitle->Caption;
	CaliForm->Visible = true;
	CaliForm->BringToFront();

    CaliForm->ReadCaliboffset();
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnCloseConnConfigClick(TObject *Sender)
{
	pnlConfig->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnConnectPLCClick(TObject *Sender)
{
    WriteSystemInfo();
    ReadSystemInfo();
	Mod_PLC->Connect(PLC_IPADDRESS, PLC_PLCPORT, PLC_PCPORT);
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnDisConnectPLCClick(TObject *Sender)
{
    Mod_PLC->DisConnect();
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::Timer_PLCConnectTimer(TObject *Sender)
{
    if(Mod_PLC->ClientSocket_PC->Active == false && Mod_PLC->ClientSocket_PLC->Active == false)
		Mod_PLC->Connect(PLC_IPADDRESS, PLC_PLCPORT, PLC_PCPORT);
    Timer_PLCConnect->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::btnConnectIROCVClick(TObject *Sender)
{
    if(Client->Active == false){
		config.recontact = true;
		this->ReContactTimerTimer(ReContactTimer);
	}
	else{
			Client->Active = false;
			config.recontact = true;
			this->ReContactTimerTimer(ReContactTimer);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::Button1Click(TObject *Sender)
{
	SetRemeasureList();
    CmdTrayOut();
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::pReadyClick(TObject *Sender)
{
    Form_Error->DisplayErrorMessage("IR/OCV NG ERROR",
										"There is too many ng cells. Please check it.",
										"Select [Tray Out] or [Restart]");
		Form_Error->Tag = this->Tag;
}
//---------------------------------------------------------------------------


void __fastcall TTotalForm::chkUseAverageClick(TObject *Sender)
{
    if(chkUseAverage->Checked == true) config.average_use = true;
    else config.average_use = false;
}
//---------------------------------------------------------------------------
void __fastcall TTotalForm::ShowPLCSignal(TAdvSmoothPanel *advPanel, bool bOn)
{
    if(bOn)
	{
		advPanel->Fill->Color = BaseForm->pon->Color;
		advPanel->Fill->ColorMirror = BaseForm->pon->Color;
		advPanel->Fill->ColorMirrorTo = BaseForm->pon->Color;
		advPanel->Fill->ColorTo = BaseForm->pon->Color;
	}else{
		advPanel->Fill->Color = BaseForm->poff->Color;
		advPanel->Fill->ColorMirror = BaseForm->poff->Color;
		advPanel->Fill->ColorMirrorTo = BaseForm->poff->Color;
		advPanel->Fill->ColorTo = BaseForm->poff->Color;
	}
}

