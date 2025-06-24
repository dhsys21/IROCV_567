#ifndef RVMO_define
#define RVMO_define


#define APP_PATH					"D:\\IROCV576\\"
#define BIN_PATH			  		APP_PATH"Bin\\"
#define MSA_PATH			  		APP_PATH"Msa\\"
#define DATA_PATH					APP_PATH"Data\\"
#define LOG_PATH                    APP_PATH"Log\\"
#define NG_PATH                     APP_PATH"NGPP\\"
#define TRAY_PATH2                  "D:\\CELL_SERIAL\\Data\\"


//---------------------------------------------------------------------------
//	STEP ����
//---------------------------------------------------------------------------
#define STEP_WAIT					0
#define STEP_MEASURE				1
#define STEP_FINISH					2
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//	Channel ����
//---------------------------------------------------------------------------
const int MAXCHANNEL = 576;
const int CHANNELCOUNT = 288;
const int LINECOUNT	= 24;

const int SEND = 1;
const int RECEIVE = 2;
const int ETC = 3;

const char nAuto 	= 0;
const char nRemote 	= 1;
const char nLocal 	= 2;

const char nAllRetest 	= 3;
const char nCellRetest 	= 4;
const char nCalibration = 5;

const char nNoAnswer	= 0;
const char nIdle		= 1;
const char nVacancy		= 2;
const char nIN			= 3;
const char nREADY		= 4;
const char nRUN			= 5;
const char nEND			= 6;
const char nFinish 		= 7;
const char nManual		= 8;
const char nOpbox		= 9; 	// IMS �������� �� ���� ����
const char nEmergency 	= 10;


const char nBusy		= 11;
const char nReameasure 	= 12;

const char nRedEnd 		= 20;
const char nBlueEnd 	= 21;

const int nRunningError = 28;
const int nReadyError 	= 30;
const int nFinishError  = 32;
const int nDefaultError = 40;

// Process Status
const int sReady   		= 0;
const int sTrayIn  		= 1;
const int sBarcode 		= 2;
const int sProbeDown 	= 3;
const int sMeasure      = 4;
const int sFinish       = 5;
const int sProbeOpen    = 6;
const int sTrayOut      = 7;

#define		COMM_RECEIVE	WM_USER + 1003


const int IDN = 1;

const int SIZ = 2;		// �˻� ���� ��ɾ�
const int AMS = 3;
const int IR =  4;
const int OCV = 5;
const int AMF = 6;
const int STP = 7;
const int FIN = 8;
const int SLO = 9;
const int FST = 10;
const int BCR = 11;
const int DEV = 13;


const int SAV = 20;     // ���� ����
const int LOD = 21;

const int SEN = 40;
const int EMS = 41;
const int ERR = 42;
const int ARV = 43;
const int STB = 44;
const int MAN = 45;
const int LOC = 46;
const int EMP = 47;
const int BYP = 48;
const int RDY = 49;
const int DOR = 50;
const int BZY = 51;
const int STA = 53;

const int LRM = 60;
const int REC = 61;
const int RST = 62;		// ������� ����
const int REM = 63;
const int CLR = 64;
const int GSC = 65;
const int IDL = 66;
const int FRM = 67;
const int sOUT = 68;
const int HOM = 69;

const int GO = 71;
const int HI = 72;
const int LO = 73;
const int OV = 74;
const int UN = 75;
const int CE = 76;
const int NA = 77;
const int NO = 78;
//const int CE = 79; /// value error

// IMS ���� �޼���
const int RESET = 0;
const int EMERGENCY = 1;
const int PROBE_ERROR = 4;
const int RESTART = 5;
const int PROCESS_ERROR = 7;
const int AIR_ERROR = 14;
const int NO_ANSWER = 17;
const int ACOUNT = 24;
const int GPIB_ERROR = 50;
const int BARCODE_ERROR = 51;
// STAGE ���� �޼���
const int OPBOX = 101;



typedef struct{
    bool ams;
    bool amf;
    bool trayin;
	AnsiString trayid;
	int cell[MAXCHANNEL];
	AnsiString cell_type;
	AnsiString lotid;
	AnsiString cell_serial[MAXCHANNEL];
	int cell_count1;
    int cell_count2;
    bool pos1_complete;
    bool pos2_complete;
	int rem_mode;
	float orginal_value[MAXCHANNEL];
	float after_value[MAXCHANNEL];
	float ocv_value[MAXCHANNEL];
	float Cali_value[MAXCHANNEL];
	int measure_result[MAXCHANNEL];
	bool first;
	AnsiString arrive;
    AnsiString finish;
	AnsiString cell_model;
	AnsiString lot_number;

	double ir_range;
	double ir_sigma;
	double ir_avg;
	double ir_avgAll;
	int ir_avgAll_count;
	bool ir_flag[MAXCHANNEL];

	double ocv_range;
    double ocv_sigma;
	double ocv_avg;
	double ocv_avgAll;
	int ocv_avgAll_count;
	bool ocv_flag[MAXCHANNEL];
}TRAY_INFO;

typedef struct{
	int arl_reserve;
	int arl;
	bool init;
	int err;
	int alarm_status;
	int alarm_cnt;
	int now_status;
	double ir_offset[MAXCHANNEL];
}STAGE_INFO;

typedef struct{
	bool re_excute;
	int cnt_error;
	int cell[MAXCHANNEL];
	int cnt_remeasure;
	int re_index;
}REMEASURE;

typedef struct{
	bool recontact;
	double ir_min;
	double ir_max;
	double ir_range;
	double ocv_min;
	double ocv_max;
	double ocv_range;
	bool average_use;
	int remeasure_cnt;
    int remeasure_alarm_cnt;
	bool remeasure_use;
	bool remeasure_bypass;
}CONFIG;

#endif
