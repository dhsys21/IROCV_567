//---------------------------------------------------------------------------

#pragma hdrstop

#include "Util.h"
#include "RVMO_main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
int __fastcall StringToInt(UnicodeString str, int def)
{
	int iVal;
	if(TryStrToInt(str, iVal) == true) return iVal;
	else return def;
}
//---------------------------------------------------------------------------
double __fastcall StringToDouble(UnicodeString str, double def)
{
	double dVal;
	if(TryStrToFloat(str, dVal) == true) return dVal;
	else return def;
}
//---------------------------------------------------------------------------
// Channel Mapping
int GetChMap(int stage, int trayPos, int index)
{
    int offset = (trayPos - 1) * (MAXCHANNEL / 2);
    return BaseForm->nForm[stage]->chMap[offset + index + 1];
}
//---------------------------------------------------------------------------
int GetChannel(int trayPos, int index)
{
    int offset = (trayPos - 1) * CHANNELCOUNT;
    return offset + index + 1;
}
//---------------------------------------------------------------------------
AnsiString SetChannelHint(int reversechannel)
{
    AnsiString hint = "";
    //* chReverseMap을 사용해서 계산
    if(reversechannel >= 289) reversechannel  = reversechannel - 288;
    int board_no = (reversechannel - 1) / LINECOUNT + 1;
    //int channel_no = (reversechannel - 1) % LINECOUNT + 1;
    int channel_no = 24 - (reversechannel - 1) % LINECOUNT;
    hint = IntToStr(board_no) + " - " + IntToStr(channel_no);
    return hint;
}
AnsiString SetChannelHint2(int channel)
{
    AnsiString hint = "";

    //* 채널번호를 1-576으로 해서 계산
    int group = (channel % 24) + 1;
    int channel_no = channel / 24 + 1;

    int board_no;
    if (group % 4 == 3 || group % 4 == 0)
        board_no = group / 2;
    else if(group % 4 == 1)
        board_no = (group + 1) / 2;
    else if(group % 4 == 2)
        board_no = (group + 1) / 2 + 1;

    hint = IntToStr(board_no) + " - " + IntToStr(channel_no);
    return hint;
}
//---------------------------------------------------------------------------
// Reverse Channel Mapping
int GetChRMap(int stage, int trayPos, int index)
{
    int offset = (trayPos - 1) * (MAXCHANNEL / 2);
    return BaseForm->nForm[stage]->chReverseMap[offset + index + 1];
}
//---------------------------------------------------------------------------
// Get Channel Position Front
AnsiString GetChPosF(int stage, int index, AnsiString lineno)
{
    int rch = BaseForm->nForm[stage]->chReverseMap[index + 1];
    if(rch >= 289) rch  = rch - 288;
    return IntToStr((rch - 1) / LINECOUNT + 1);
}
//---------------------------------------------------------------------------
AnsiString GetChPosF(int* chReverseMap, int index, AnsiString lineno)
{
    int rch = chReverseMap[index + 1];
    if(rch >= 289) rch  = rch - 288;
    return IntToStr((rch - 1) / LINECOUNT + 1);
}
//---------------------------------------------------------------------------
// Get Channel Position Rear
AnsiString GetChPosR(int stage, int index, AnsiString lineno)
{
    int rch = BaseForm->nForm[stage]->chReverseMap[index + 1];
    if(rch >= 289) rch  = rch - 288;
    return IntToStr((rch - 1) % LINECOUNT + 1);
}
//---------------------------------------------------------------------------
AnsiString GetChPosR(int* chReverseMap, int index, AnsiString lineno)
{
    int rch = chReverseMap[index + 1];
    if(rch >= 289) rch  = rch - 288;
    return IntToStr((rch - 1) % LINECOUNT + 1);
}
//---------------------------------------------------------------------------
void OpenFolder(UnicodeString path)
{
	 ShellExecute(NULL, L"open", path.c_str(), NULL, NULL, SW_SHOW);
}
//---------------------------------------------------------------------------
