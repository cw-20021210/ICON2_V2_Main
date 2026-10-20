/// @file   communication.h
/// @date   2021.01.12
/// @author PSM
/// @brief  통신 처리 file

#ifdef LINEAUTOTEST_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_LINEAUTOTEST()		{	/*	1ms 타이머 감산		*/	\
										TDEC(gu8_TimerLineRecWait);		\
										TDEC(gu8_TimerLineTraWait);		\
										TDEC(gu8_TimerLineTrahalt);		\
										TDEC(gu8_TimerLineRecInt_Check);	\
									}
#define	TDEC_10ms_LINEAUTOTEST()	{	/*	10ms 타이머 감산		*/	\
										TDEC(gu8_TimerLineError_Time);		\
									}
#define	TDEC_100ms_LINEAUTOTEST()	{	/*	100ms 타이머 감산		*/	\
										TDEC(gu8_TimerLineDataReceive);	\
										TDEC(gu8_TimerLineTest_Complete);	\
										TDEC(gu8_TimerLineFrontTest_EffLimit);	\
										TDEC(gu8_TimerFrontTest_1stNosDisplay);	\
									}
#define	TDEC_1s_LINEAUTOTEST()		{	/*	1s 타이머 감산			*/	\
									}
#define	TDEC_10s_LINEAUTOTEST()		{	/*	10s 타이머 감산		*/	\
									}
#define	TDEC_1min_LINEAUTOTEST()	{	/*	1min 타이머 감산		*/	\
									}
#define	TDEC_10min_LINEAUTOTEST()	{	/*	10min 타이머 감산		*/	\
									}
#define	TDEC_1h_LINEAUTOTEST()		{	/*	1h 타이머 감산			*/	\
									}


/*==============================================================================
 Constant Definition
 ==============================================================================*/
#define Z_LineDataLen				(100)						/* Packet data  */
#define Z_TimerLineError_Time		(200)						/* 통신 미수신 에러 */
#define Z_TimerLineRecInt_Check		(30)						//	Rec. Interrupt 30ms check

#define Z_TimerLineRecWait			(20)						/* 수신대기 Timer 20ms */
#define Z_TimerLineTraWait			(10)						/* 송신대기 Timer 10ms */
#define Z_TimerLineTrahalt			(50)						//	Tx 정지체크 Timer 50ms
#define Z_TimerLineDataReceive		(100)						//	통신 미수신 10초 <10s/100ms>

#define Z_Line_STX					(0x01)
#define Z_Line_ETX					(0x04)

/*	라인테스트 CMD	*/
#define LINETEST_TESTMODE			(0x2000)
//#define LINETEST_MODEL_CHECK		(0x2001)
#define LINETEST_SETTING_CHECK		(0x3000)
#define LINETEST_WIFI_CHECK			(0x3010)
#define LINETEST_UV_FEEDBACK		(0x3011)
#define LINETEST_FLOWMETER_CHECK	(0x3012)
#define LINETEST_CURRENT			(0x3013)
//#define LINETEST_FAN_FEEDBACK		(0x3014)
#define LINETEST_TH_CHECK			(0x3020)
#define LINETEST_LEVEL_SENSOR		(0x3030)
#define LINETEST_ETC_SENSOR			(0x3040)
#define LINETEST_HEATER_FEEDBACK	(0x3050)
#define LINETEST_ALL_OFF			(0x5000)
//#define LINETEST_HEATER_ON			(0x5010)
//#define LINETEST_HEATER_OFF			(0x5011)
#define LINETEST_COLDIN_VALVE_ON	(0x5102)
#define LINETEST_COLDIN_VALVE_OFF	(0x5103)
#define LINETEST_HOTIN_VALVE_ON		(0x5104)
#define LINETEST_HOTIN_VALVE_OFF	(0x5105)
#define LINETEST_PURE_VALVE_ON		(0x5106)
#define LINETEST_PURE_VALVE_OFF		(0x5107)
#define LINETEST_COLDOUT_VALVE_ON	(0x5108)
#define LINETEST_COLDOUT_VALVE_OFF	(0x5109)
#define LINETEST_HOTOUT_VALVE_ON	(0x510A)
#define LINETEST_HOTOUT_VALVE_OFF	(0x510B)
#define LINETEST_1STNOS_VALVE_ON	(0x5110)
#define LINETEST_1STNOS_VALVE_OFF	(0x5111)
//#define LINETEST_EXC_VALVE_ON		(0x5126)
//#define LINETEST_EXC_VALVE_OFF		(0x5127)
//#define LINETEST_INOSENSE_VALVE_ON	(0x5128)
//#define LINETEST_INOSENSE_VALVE_OFF	(0x5129)
//#define LINETEST_HOTAIR_VALVE_ON	(0x512A)
//#define LINETEST_HOTAIR_VALVE_OFF	(0x512B)
//#define LINETEST_COLDAIR_VALVE_ON	(0x512C)
//#define LINETEST_COLDAIR_VALVE_OFF	(0x512D)
#define LINETEST_FAN_ON				(0x5200)
#define LINETEST_FAN_OFF			(0x5201)
#define LINETEST_STEPPINGMOTOR_OPEN		(0x5202)
#define LINETEST_STEPPINGMOTOR_CLOSE	(0x5203)
//#define LINETEST_PUMP_ON			(0x5300)
//#define LINETEST_PUMP_OFF			(0x5301)
#define LINETEST_TEM_ON				(0x9900)
#define LINETEST_TEM_OFF			(0x9901)
#define LINETEST_TEST_COMPLETE		(0xFF00)

//#define ORIENTEC_TEST_START					(0xEC00)
//#define ORIENTEC_TEST_COLDSENSOR_LOW_ON		(0xEE01)
//#define ORIENTEC_TEST_COLDSENSOR_LOW_OFF	(0xEE02)
//#define ORIENTEC_TEST_COLDSENSOR_HIGH_ON	(0xEE03)
//#define ORIENTEC_TEST_COLDSENSOR_HIGH_OFF	(0xEE04)
//#define ORIENTEC_TEST_BUZZER_OUTPUT			(0xEE05)

#define Z_TimerLineTest_Complete	(10)

//@028 #define Z_TimerLineFrontTest_EffLimit	(15)
#define Z_TimerLineFrontTest_EffLimit	(10)		//@028
#define Z_TimerFrontTest_1stNosDisplay	(20)		//	<2s/100ms>

/*==============================================================================
 Variable Definition
 ==============================================================================*/
EXTERN U8 gu8_TimerLineError_Time;								/* 	2초 미수신 타이머 <2s/10ms>	*/
EXTERN U8 gu8_CounterLineRXDError;
EXTERN U8 F_LineAutoTest_Commu_Error;
EXTERN U8 gu8_TimerLineDataReceive;								//	통신 미수신 10초	<10s/100ms>
EXTERN BYTE_BIT gu8_LineUartState;							//	통신상태 변수
	#define	f_LineRec_End			(gu8_LineUartState.Bit.b1)	//		수신 끝
	#define	f_LineRec_Start			(gu8_LineUartState.Bit.b2)	//		수신 시작
	#define	f_Tra_LineDataMake		(gu8_LineUartState.Bit.b3)	//		송신 데이터 생성
	#define	f_Tra_LineStart			(gu8_LineUartState.Bit.b4)	//		송신 시작

EXTERN U8 gu8_CounterLineRecvCount;								/* 	수신 데이터 길이 카운터	*/
EXTERN U8 gu8_CounterLineTraDataCount;								/* 	송신 데이터 카운터	*/
EXTERN BYTE_BIT	gu8_Buffer_Line_RecData[Z_LineDataLen];				/* 	수신 데이터 버퍼	*/
EXTERN BYTE_BIT	gu8_Buffer_Line_TraData[Z_LineDataLen];				/* 	송신 데이터 버퍼	*/

EXTERN WORD_BIT gu16_LineTest_LoadCMD;
	#define F_LineTest_SteppingMotor	(gu16_LineTest_LoadCMD.Bit.b0)
	#define F_LineTest_HotIn_VV		(gu16_LineTest_LoadCMD.Bit.b1)
	#define F_LineTest_Pure_VV		(gu16_LineTest_LoadCMD.Bit.b2)
	#define F_LineTest_ColdOut_VV	(gu16_LineTest_LoadCMD.Bit.b3)
	#define F_LineTest_HotOut_VV	(gu16_LineTest_LoadCMD.Bit.b4)
	#define F_LineTest_1stNos_VV	(gu16_LineTest_LoadCMD.Bit.b5)
	#define F_LineTest_ColdIn_VV	(gu16_LineTest_LoadCMD.Bit.b6)
	#define F_LineTest_Fan			(gu16_LineTest_LoadCMD.Bit.b7)
	#define F_LineTest_TEM			(gu16_LineTest_LoadCMD.Bit.b8)

EXTERN BYTE_BIT F_LineFrontTest_Complete;
	#define F_LineFrontTest_Amount_Complete		(F_LineFrontTest_Complete.Bit.b0)
	#define F_LineFrontTest_Hot_Complete		(F_LineFrontTest_Complete.Bit.b1)
	#define F_LineFrontTest_Cold_Complete		(F_LineFrontTest_Complete.Bit.b2)
	#define F_LineFrontTest_Pure_Complete		(F_LineFrontTest_Complete.Bit.b3)
	#define F_LineFrontTest_Effluent_Complete	(F_LineFrontTest_Complete.Bit.b4)
	#define	F_LineFrontTest_1stNos_VV_Complete	(F_LineFrontTest_Complete.Bit.b5)

EXTERN BYTE_BIT F_LineFrontTest_Operation;
	#define F_LineFrontTest_SteppingMotor_Oper	(F_LineFrontTest_Operation.Bit.b0)
	#define F_LineFrontTest_Effluent_Oper	(F_LineFrontTest_Operation.Bit.b4)
	#define F_LineFrontTest_Amount_Select 	(F_LineFrontTest_Operation.Bit.b5)

EXTERN U8 F_LineFrontTest_1stNos_VV;		//@023	파세코 수동검사에서 노스밸브 검사를 위한 플래그

EXTERN U8 gu8_TimerLineFrontTest_EffLimit;			//	저유량센서 고속 회전(열발생)으로 추출 X초 제한	<1.5s/100ms>
EXTERN U8 gu8_TimerFrontTest_1stNosDisplay;	//@023	Nos밸브 Open/Close 상태 디스플레이

EXTERN U8 gu8_LineFrontTest_Amount;

EXTERN U8 F_LineFrontTest_Hot_Select;
EXTERN U8 F_LineFrontTest_Cold_Select;
EXTERN U8 F_LineFrontTest_Pure_Select;

EXTERN U8 F_LineAuto_Ready;

EXTERN U8 gu8_LineTra_Length;

EXTERN U8 gu8_Line_RecData_CS_Calc;
EXTERN U8 gu8_Line_RecData_CS;
EXTERN U8 gu8_Line_TraData_CS;
EXTERN U16 gu16_Line_RecData_CMD;
EXTERN U8 gu8_Line_RecData_Data1;
EXTERN U8 gu8_Line_RecData_Data2;
EXTERN U8 gu8_Line_RecData_Data3;
EXTERN U8 gu8_Line_RecData_Data4;

EXTERN U16 gu8_TimerLineRecWait;									/* 	수신 대기 타이머		<20ms/1ms> */
EXTERN U16 gu8_TimerLineTraWait;									/* 	송신 대기 타이머		<20ms/1ms> */

EXTERN U8 gu8_TimerLineTrahalt;						//	Tx 체크		<50ms/1ms>
EXTERN U8 gu8_TimerLineRecInt_Check;				//	Rx 체크		<30ms/1ms>

EXTERN U8 F_LineRx_Complete;						//	수신성공 flag
EXTERN U8 F_LineRx_Exp;								//	수신 경험

EXTERN U8 F_NACK_Tx;

EXTERN U8 F_LineTest_Complete;
EXTERN U8 gu8_TimerLineTest_Complete;				//	<1s/100ms>

EXTERN U16 gu16_Heater_FB_Relay_forLineTest;
EXTERN U16 gu16_Heater_FB_Triac_forLineTest;
EXTERN U16 gu16_Heater_FB_forLineTest;

EXTERN U8 F_WIFI_Pairing_forLineTest;

EXTERN U16 gu16_LineTestErrorCount1;
EXTERN U16 gu16_LineTestErrorCount2;
EXTERN U16 gu16_LineTestErrorCount3;
EXTERN U16 gu16_LineTestErrorCount4;
EXTERN U16 gu16_LineTestErrorCount5;
EXTERN U16 gu16_LineTestErrorCount6;

EXTERN U8 gu8_HeaterRetry;

/*==============================================================================
 function Definition
 ==============================================================================*/
void LineAutoTest_Initialize(void);
void LineAutoTestControl(void);

void Line_UartInitial(void);						//	통신초기화
void Line_UartReceive(U8 recdata);					//	Rx처리 함수 (인터럽트 내)
void Line_UartControl(void);						//	통신처리 함수
void Line_TXControl(void);							//	Tx처리함수
void Line_RXControl(void);							//	Rx처리함수
void Line_TraCrcCheck(void);						//	Tx CRC 체크
void Line_Data_Parsing(void);						//	수신데이터 분석
void Line_Send_Data(void);							//	송신 데이터 생성
void Line_Uart_SFR_Refresh(void);					//	통신관련 SFR refresh
__interrupt void Line_IntUartTx(void);				//	Tx interrupt
__interrupt void Line_IntUartRx(void);				//	Rx interrupt

void Line_CMD_Check(void);

//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================

/*===========================================================================*/
#undef	EXTERN
#undef	GLOBAL_VARIABLE_DEFINE
/*===========================================================================*/
