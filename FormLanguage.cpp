//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RVMO_main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm_Language *Form_Language;
//---------------------------------------------------------------------------
__fastcall TForm_Language::TForm_Language(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TForm_Language::LanguageChange(int LangIndex)
{
    TValueListEditor *vle;
    if(LangIndex == 0) vle = VLE_ENGLISH;
    else if(LangIndex == 1) vle = VLE_KOREAN;

	//-------------------------------------------------------------------------
	// 				COMPONENT
	//-------------------------------------------------------------------------
	for(int i = 0; i < BaseForm->FormCnt; i++){
        BaseForm->nForm[i]->btnSaveConnConfig->Caption = vle->Values["SAVE"];
        BaseForm->nForm[i]->btnCloseConnConfig->Caption = vle->Values["CANCEL"];
        BaseForm->nForm[i]->btnConfig->Caption = vle->Values["CONFIG"];
        BaseForm->nForm[i]->btnManual->Caption = vle->Values["MANUAL"];
        BaseForm->nForm[i]->btnAuto->Caption = vle->Values["AUTO"];
        BaseForm->nForm[i]->btnReset->Caption = vle->Values["RESET"];
        BaseForm->nForm[i]->Panel6->Caption = vle->Values["TRAYID"];
        BaseForm->nForm[i]->Panel3->Caption = vle->Values["STATUS"];
        BaseForm->nForm[i]->Panel20->Caption = vle->Values["PROCESS"];
        BaseForm->nForm[i]->Panel9->Caption = vle->Values["CHANNEL"];
        BaseForm->nForm[i]->btnTrayOut->Caption = vle->Values["TRAYOUT"];
        BaseForm->nForm[i]->btnRemeasureInfo->Caption = vle->Values["REMEAINFO"];
    }

    //-------------------------------------------------------------------------
	// 				MESSAGE
	//-------------------------------------------------------------------------
	msgRbt = vle->Values["msgRBT"];
    msgRst = vle->Values["msgRST"];
    msgTooManyNG = vle->Values["msgTooManyNG"];
}
//---------------------------------------------------------------------------
