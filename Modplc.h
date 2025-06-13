//---------------------------------------------------------------------------

#ifndef ModPLCH
#define ModPLCH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <System.Win.ScktComp.hpp>
#include "Define.h"

#include <deque>

//---------------------------------------------------------------------------
//	����̽� �ڵ�
//---------------------------------------------------------------------------
const int DEVCODE_M			=	0x90;		//	���� ������
const int DEVCODE_L			=	0x92;	  	//	��ġ ������
const int DEVCODE_D			=	0xA8;		//	������ ��������
const int DEVCODE_W			=	0xB4;		//	��ũ ��������
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//	Index ����
//---------------------------------------------------------------------------
const int PLC_INDEX_INTERFACE					=		1;


const int PC_INDEX_INTERFACE					=		11;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//	���� ����
//---------------------------------------------------------------------------
const int PLC_D_INTERFACE_START_DEV_NUM	 			=	44000;
const int PLC_D_INTERFACE_LEN 						= 	100;

const int PC_D_INTERFACE_START_DEV_NUM1				=	45000;
const int PC_D_INTERFACE_LEN1	 					= 	700;
const int PC_D_INTERFACE_START_DEV_NUM2				=	46000;
const int PC_D_INTERFACE_LEN2	 					= 	600;
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//	PLC - PC Interface
//---------------------------------------------------------------------------
// PLC - IR/OCV
const int PLC_D_HEART_BEAT				   			=	0;
const int PLC_D_IROCV_AUTO_MANUAL	                =   1;
const int PLC_D_IROCV_ERROR    	  			        =   2;
const int PLC_D_IROCV_TRAY_IN   	  		        =   3;
const int PLC_D_IROCV_PROB_OPEN 	  		        =   4;
const int PLC_D_IROCV_PROB_CLOSE    	  	        =   5;
const int PLC_D_IROCV_TRAY_POS                      =   6;

const int PLC_D_IROCV_TRAY_ID   	  		        =   10;
// TRAY INFO     256
const int PLC_D_IROCV_TRAY_CELL_DATA                = 	30;

//---------------------------------------------------------------------------
//	PLC - PC Interface
//---------------------------------------------------------------------------
// PC - IR/OCV
const int PC_D_HEART_BEAT			  				=	0;
const int PC_D_IROCV_STAGE_AUTO_READY     	    	=   1;
const int PC_D_IROCV_ERROR    	  			    	=   2;
const int PC_D_IROCV_TRAY_OUT    	  	   	    	=   3;
const int PC_D_IROCV_PROB_OPEN   	  		    	=   4;
const int PC_D_IROCV_PROB_CLOSE    	  	        	=   5;
const int PC_D_IROCV_MEASURING                   	=   6;
const int PC_D_IROCV_COMPLETE1		                =   7;
const int PC_D_IROCV_COMPLETE2		                =   8;
const int PC_D_IROCV_TRAY_POS_MOVE	                =   9;

const int PC_D_IROCV_NG_COUNT						=   10;
const int PC_D_IROCV_IR_MIN							=   11;
const int PC_D_IROCV_IR_MAX							=   13;
const int PC_D_IROCV_OCV_MIN						=   15;
const int PC_D_IROCV_OCV_MAX						=   17;
const int PC_D_IROCV_NG_ALARM                       =   19;

// OK/NG - D45030
const int PC_D_IROCV_MEASURE_OK_NG			   		=	30;

const int PC_D_IROCV_IR_VALUE                   	=   100;
const int PC_D_IROCV_OCV_VALUE                   	=   0;
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//	PLC
//---------------------------------------------------------------------------
typedef struct
{
	unsigned char SubHeader[2];			//	���� ���
	unsigned char NetNum;		 		//	��Ʈ��ũ ��ȣ
	unsigned char PlcNum;	   			//	PLC ��ȣ
	unsigned char ReqIONum[2];			//	IO ��ȣ
	unsigned char ReqOfficeNum;	  		//	�� ��ȣ
	unsigned char ReqDataLen[2];		//  �䱸 ������ ����(CPU ���� Ÿ�̸� ~ ����̽� ����)
	unsigned char CpuTime[2];			//	CPU ���� Ÿ�̸�
	unsigned char Command[2];			//	Ŀ�ǵ�
	unsigned char SubCommand[2];		//	���� Ŀ�ǵ� (0 = ��Ʈ-16����, ����-1���� / 1 = ��Ʈ-1����)
	unsigned char StartDevNum[3];		//	���� ����̽�
	unsigned char DevCode;  			//	����̽� �ڵ�
	unsigned char DevLen[2];			//	����̽� ����
} PLC_DATA;
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//	PC
//---------------------------------------------------------------------------
typedef struct
{
	unsigned char SubHeader[2];					//	���� ���
	unsigned char NetNum;						//	��Ʈ��ũ ��ȣ
	unsigned char PlcNum;	    				//	PLC ��ȣ
	unsigned char ReqIONum[2];					//	IO ��ȣ
	unsigned char ReqOfficeNum;	  				//	�� ��ȣ
	unsigned char ReqDataLen[2];				//  �䱸 ������ ����(CPU ���� Ÿ�̸� ~ ����̽� ����)
	unsigned char CpuTime[2];					//	CPU ���� Ÿ�̸�
	unsigned char Command[2];					//	Ŀ�ǵ�
	unsigned char SubCommand[2];				//	���� Ŀ�ǵ�
	unsigned char StartDevNum[3];				//	���� ����̽�
	unsigned char DevCode;  					//	����̽� �ڵ�
	unsigned char DevLen[2];					//	����̽� ����
} PC_DATA;
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
class TMod_PLC : public TDataModule
{
__published:	// IDE-managed Components
	TTimer *Timer_Read;
	TClientSocket *ClientSocket_PC;
	TTimer *Timer_PC_AutoConnect;
	TTimer *Timer_PC_WriteMsg;
	TClientSocket *ClientSocket_PLC;
	TTimer *Timer_PLC_AutoConnect;
	TTimer *Timer_PLC_WriteMsg;
	void __fastcall ClientSocket_PCConnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocket_PCDisconnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocket_PCError(TObject *Sender, TCustomWinSocket *Socket,
          TErrorEvent ErrorEvent, int &ErrorCode);
	void __fastcall ClientSocket_PCRead(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall Timer_PC_AutoConnectTimer(TObject *Sender);
	void __fastcall Timer_PC_WriteMsgTimer(TObject *Sender);
	void __fastcall ClientSocket_PLCConnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocket_PLCDisconnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocket_PLCError(TObject *Sender, TCustomWinSocket *Socket,
          TErrorEvent ErrorEvent, int &ErrorCode);
	void __fastcall ClientSocket_PLCRead(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall Timer_PLC_WriteMsgTimer(TObject *Sender);
	void __fastcall Timer_PLC_AutoConnectTimer(TObject *Sender);

private:	// User declarations
    bool bClose;
//---------------------------------------------------------------------------
//	PLC
//---------------------------------------------------------------------------
	void __fastcall PLC_Initialization();
	void __fastcall PLC_DataChange(int subCommand, int address, int devCode, int devLen);
	void __fastcall PLC_Recv_Interface();

	PLC_DATA plc_Data;
	AnsiString plc_Read, plc_Read_Temp;
	bool plc_ReadFlag;
	int plc_ReadCount, plc_index;
	AnsiString plc_Interface;

//---------------------------------------------------------------------------
//	PC
//---------------------------------------------------------------------------
	void __fastcall PC_Initialization();
	void __fastcall PC_DataChange(int subCommand, int address, int devCode, int devLen);

	PC_DATA pc_Data;
	AnsiString pc_Read, pc_Read_Temp;
	bool pc_ReadFlag;
	int pc_ReadCount, pc_index;

//---------------------------------------------------------------------------
//	Tray Info
//---------------------------------------------------------------------------
	void __fastcall SaveTrayInfo(AnsiString trayID);
    AnsiString m_trayID;
	int m_slotNum, m_slotNumTemp;
public:		// User declarations
	__fastcall TMod_PLC(TComponent* Owner);

	void __fastcall Connect(AnsiString ip, int port1, int port2);
	void __fastcall DisConnect();

	void __fastcall SetData(unsigned char (*data)[2], int column, int num, bool flag);
	void __fastcall SetDouble(unsigned char (*data)[2], int column, double value);
	void __fastcall SetString(unsigned char (*data)[2], int column, AnsiString msg);

	int __fastcall GetData(unsigned char (*data)[2], int column, int num);
	double __fastcall GetDouble(unsigned char (*data)[2], int column);
	AnsiString __fastcall GetString(unsigned char (*data)[2], int column, int count);

	unsigned char plc_Interface_Data[PLC_D_INTERFACE_LEN][2];
	unsigned char pc_Interface_Data[PC_D_INTERFACE_LEN1][2];
	unsigned char pc_Interface_Ocv_Data[PC_D_INTERFACE_LEN2][2];
};
//---------------------------------------------------------------------------
extern PACKAGE TMod_PLC *Mod_PLC;
//---------------------------------------------------------------------------
#endif
