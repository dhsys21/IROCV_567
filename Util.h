//---------------------------------------------------------------------------

#ifndef UtilH
#define UtilH
//---------------------------------------------------------------------------
#endif

#include <System.hpp>
#include <System.SysUtils.hpp>
//---------------------------------------------------------------------------
#include "define.h"

int __fastcall StringToInt(UnicodeString str, int def);
double __fastcall StringToDouble(UnicodeString str, double def);
int GetChMap(int stage, int trayPos, int index);
int GetChRMap(int stage, int trayPos, int index);
AnsiString GetChPosF(int stage, int index, AnsiString lineno);
AnsiString GetChPosF(int* chReverseMap, int index, AnsiString lineno);
AnsiString GetChPosR(int stage, int index, AnsiString lineno);
AnsiString GetChPosR(int* chReverseMap, int index, AnsiString lineno);
void OpenFolder(UnicodeString path);
AnsiString SetChannelHint2(int channel);
AnsiString SetChannelHint(int reversechannel);
int GetChannel(int trayPos, int index);
