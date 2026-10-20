/// @file   watermeter.h
/// @date   2019.07.10
/// @author PSM
/// @brief  유량센서 처리 관련 헤더 file

#ifdef COMMAND_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_COMMAND()		{	/*	1ms 타이머 감산			*/	\
								}
#define	TDEC_10ms_COMMAND()		{	/*	10ms 타이머 감산		*/	\
								}
#define	TDEC_100ms_COMMAND()	{	/*	100ms 타이머 감산		*/	\
								}
#define	TDEC_1s_COMMAND()		{	/*	1s 타이머 감산			*/	\
								}
#define	TDEC_10s_COMMAND()		{	/*	10s 타이머 감산			*/	\
								}
#define	TDEC_1min_COMMAND()		{	/*	1min 타이머 감산		*/	\
									TDEC(gu8_TimerTestMode);	\
								}
#define	TDEC_10min_COMMAND()	{	/*	10min 타이머 감산		*/	\
								}
#define	TDEC_1h_COMMAND()		{	/*	1h 타이머 감산			*/	\
								}


/*==============================================================================
 Constant Definition
 ==============================================================================*/
#define	Z_endcode			(0xff)						//
#define	Z_eepEndAddress		(0xFF)						/*	EEPROM의 최종 Address*/
#define	Z_eepB121Data		(0x55)						/*	SerialCommand[B1+20]에 대응한, EEPROM 체크용의 Data*/
#define ZT_TestMode			(30)

#define Z_asciihi		(0)					/*	ASCII 변환 상위4bit 지정*/
#define Z_asciilo		(1)					/*	ASCII 변환 하위4bit 지정*/


/*==============================================================================
 Variable Definition
 ==============================================================================*/
#define Z_VerNo		(0x00)								//

EXTERN const U8 gu8_RomNum[4]							//
#ifdef	GLOBAL_VARIABLE_DEFINE							//
 = {													//	프로그램 식별 CODE
	0x03,												//		분류 No
	0x10,												//		기구종류,형식No(상위)
	0x17,												//		형식No(중위/하위)
	Z_VerNo												// 		프로그램 Ver
}														//
#endif													//
;														//

//EXTERN U8		gu8_SerialNo[6];							//	제조번호(6Btye DATA)

EXTERN BYTE_BIT F_Command;								//	SERIAL COMMAND 관계 FLAG
	#define	f_F7cmd			(F_Command.Bit.b2)				//		F7 CMD 수신 FLAG [0:F7CMD무효,1:F7CMD유효]
	#define	f_50cmd			(F_Command.Bit.b1)				//		F50 CMD 수신 FLAG [0:50CMD미수신,1:50CMD수신]

//EXTERN U8	D_SerialF3Address;							/*	시리얼 F3Command EEPROM Address*/
//EXTERN U8	DE_Serial[2];								/*	시리얼읽기 Data*/

EXTERN BYTE_BIT	F_TestOutput[1];						/*	TEST MODE시 출력지시용 FLAG*/
	#define	f_BuzzerOutput			(F_TestOutput[0].Bit.b0)	/*		9140: 부저*/
	#define	f_DisplayTest1			(F_TestOutput[0].Bit.b1)	/*		91B0: Display 검사*/
	#define	f_DisplayTest2			(F_TestOutput[0].Bit.b2)	/*		91B1: Display 검사*/
	#define	f_DisplayTest3			(F_TestOutput[0].Bit.b3)	/*		91B2: Display 검사*/
	#define	f_TxPinTest				(F_TestOutput[0].Bit.b4)	/*		9160: Tx Pin 검사*/

EXTERN U8 gu8_TimerDisplayTest;
EXTERN U8 gu8_CounterDisplayTest;

EXTERN U8 gu8_SerData[50];
EXTERN U8 gu8_TimerTestMode;

/*==============================================================================
 function Definition
 ==============================================================================*/
void Command_Initialize(void);
U8 Command(void);									// 표준 SERIAL COMMAND 처리


//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================
