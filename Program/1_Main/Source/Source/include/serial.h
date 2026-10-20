/// @file   serial.h
/// @date   2019.07.10
/// @author PSM
/// @brief  Serial UART통신 동작 관련 헤더 file

#ifdef SERIAL_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_SERIAL()		{	/*	1ms 타이머 감산			*/	\
									TDEC(gu8_TimerSerTrNo);		\
									TDEC(gu8_TimerSerTrIntvl);	\
								}
#define	TDEC_10ms_SERIAL()		{	/*	10ms 타이머 감산		*/	\
								}
#define	TDEC_100ms_SERIAL()		{	/*	100ms 타이머 감산		*/	\
									TDEC(gu8_TimerSerCmuSts);	\
								}
#define	TDEC_1s_SERIAL()		{	/*	1s 타이머 감산			*/	\
								}
#define	TDEC_10s_SERIAL()		{	/*	10s 타이머 감산			*/	\
								}
#define	TDEC_1min_SERIAL()		{	/*	1min 타이머 감산		*/	\
								}
#define	TDEC_10min_SERIAL()		{	/*	10min 타이머 감산		*/	\
								}
#define	TDEC_1h_SERIAL()		{	/*	1h 타이머 감산			*/	\
								}


/*==============================================================================
 Constant Definition
 ==============================================================================*/
#define Z_recbufmax		(32)						/*	SERIAL 수신 버퍼 최대수	*/


/*==============================================================================
	Variable Definition
==============================================================================*/
EXTERN	U8	gu8_TimerSerTrNo;						/*	SERIAL 송신금지 TIMER					<20ms/1ms>	*/
EXTERN	U8	gu8_TimerSerTrIntvl;					/*	SERIAL CHARACTER 송신간격금지 TIMER		<3ms/1ms> 	*/
EXTERN	U8	gu8_TimerSerCmuSts;						/*	SERIAL 통신상태 계속 TIMER				<5s/100ms>	*/
EXTERN	union	adress_para	gu16_access;			/*	ADDRESS DATA*/
EXTERN	union	adress_para	gu16_addr;				/*	ADDRESS DATA [50/51 command용]*/
EXTERN	U8	gu8_BufferSerData[Z_recbufmax];			/*	SERIAL 수신 BUFFER*/
EXTERN	U8	gu8_DataMany;							/*	[DATA1~DATAn] 송신 DATA수*/
EXTERN	U8	gu8_CMDMany;							/*	[STX~CMD3] 송신 DATA수*/
EXTERN	U8	gu8_R_eepSerial;

EXTERN U8 gu8_Serial;								/*	SERIAL 통신			*/
EXTERN U8 gu8_SerControl;							/*	SERIAL 송수신 상태 	*/
EXTERN U8 gu8_CounterSerData;						/*	SERIAL 통신 COUNTER */
EXTERN U8 gu8_BufferChecksum;						/*	CHECKSUM 가산용 BUFFER */
EXTERN U8 gu8_DataNum[256];							/*	SERIAL 송수신용 DATA BUFFER */
EXTERN U8 F_ascii;									/*	ASCII 상위하위 지정 FLAG */

EXTERN U8 gu8_B1cmdAns;

/*==============================================================================
	Function Definition
==============================================================================*/
void Serial_Initialize(void);						/*	표준 SERIAL 통신:초기 설정		*/
void SerialControl(void);							/*	표준 SERIAL 통신:통신 상태 천이 처리*/
void R_Serial_Interrupt(void);

/*=============================================================================*/
#undef	EXTERN
#undef	GLOBAL_VARIABLE_DEFINE
/*=============================================================================*/
