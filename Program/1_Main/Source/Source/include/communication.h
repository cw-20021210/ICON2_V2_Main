/// @file   communication.h
/// @date   2021.01.12
/// @author PSM
/// @brief  통신 처리 file

#ifdef COMMUNICATION_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_COMMUNICATION()	{	/*	1ms 타이머 감산		*/	\
										TDEC(gu8_TimerRecWait);		\
										TDEC(gu8_TimerTraWait);		\
										TDEC(gu8_TimerTrahalt);		\
										TDEC(gu8_TimerRecInt_Check);	\
										TDEC(gu8_TimerTraWait_Short);	\
									}
#define	TDEC_10ms_COMMUNICATION()	{	/*	10ms 타이머 감산		*/	\
										TDEC(gu8_TimerError_Time);		\
									}
#define	TDEC_100ms_COMMUNICATION()	{	/*	100ms 타이머 감산		*/	\
										TDEC(gu8_TimerDataReceive);	\
									}
#define	TDEC_1s_COMMUNICATION()		{	/*	1s 타이머 감산			*/	\
									}
#define	TDEC_10s_COMMUNICATION()	{	/*	10s 타이머 감산		*/	\
									}
#define	TDEC_1min_COMMUNICATION()	{	/*	1min 타이머 감산		*/	\
									}
#define	TDEC_10min_COMMUNICATION()	{	/*	10min 타이머 감산		*/	\
									}
#define	TDEC_1h_COMMUNICATION()		{	/*	1h 타이머 감산			*/	\
									}


/*==============================================================================
 Constant Definition
 ==============================================================================*/
#ifndef _FRONT_COMMU_CHANGE_
#define Z_DataLen				(43)						/* Packet data  */
#else
#define Z_DataLen_Tx			(43)						/* Packet data  */
#define Z_DataLen_Rx			(15)						/* Packet data  */
#endif
#ifdef _FRONT_TEST_
#define Z_TimerError_Time		(100)						/* 우림 지그는 1초로 빠르게 통신 미수신 에러 */
#else
#define Z_TimerError_Time		(200)						/* 통신 미수신 에러 */
#endif
#define Z_TimerRecInt_Check		(30)						//	Rec. Interrupt 30ms check

#define Z_TimerRecWait			(20)						/* 수신대기 Timer 20ms */
//#define Z_TimerTraWait			(10)						/* 송신대기 Timer 10ms */
#define Z_TimerTraWait			(100)						/* 송신대기 Timer 200ms */
#define Z_TimerTrahalt			(50)						//	Tx 정지체크 Timer 50ms
#define Z_TimerDataReceive		(100)						//	통신 미수신 10초 <10s/100ms>

#define Z_STX					(0x02)
#define Z_ETX					(0x03)
#define Z_Main_Address			(0x10)

#define Z_NormalData			(0x11)						//	통상 데이터

//#define Z_Commu_NormalData		(1)

#define Z_TimerTraWait_Short	(50)						//	<?ms/1ms>


/*==============================================================================
 Variable Definition
 ==============================================================================*/
EXTERN U8 gu8_TimerError_Time;								/* 	2초 미수신 타이머 <2s/10ms>	*/
EXTERN U8 gu8_CounterRXDError;
EXTERN U8 gu8_CounterTXDError;
EXTERN U8 F_Front_Communication_Error;
EXTERN U8 gu8_TimerDataReceive;								//	통신 미수신 10초	<10s/100ms>
EXTERN BYTE_BIT gu8_UartState;							//	통신상태 변수
	#define	f_Rec_End			(gu8_UartState.Bit.b1)	//		수신 끝
	#define	f_Rec_Start			(gu8_UartState.Bit.b2)	//		수신 시작
	#define	f_Tra_DataMake		(gu8_UartState.Bit.b3)	//		송신 데이터 생성
	#define	f_Tra_Start			(gu8_UartState.Bit.b4)	//		송신 시작

EXTERN U8 gu8_CounterRecvCount;						/* 	수신 데이터 길이 카운터	*/
EXTERN U8 gu8_CounterTraDataCount;					/* 	송신 데이터 카운터	*/
EXTERN U16 gu8_Buffer_RecCrc;						/* 	수신CRC DATA 		*/
EXTERN U16 gu8_Buffer_TraCrc;						/* 	송신CRC DATA 		*/
#ifndef _FRONT_COMMU_CHANGE_
EXTERN BYTE_BIT	gu8_Buffer_MCU_RecData[Z_DataLen+2];	/* 	수신 데이터 버퍼	*/
EXTERN BYTE_BIT	gu8_Buffer_MCU_TraData[Z_DataLen+2];	/* 	송신 데이터 버퍼	*/
#else
EXTERN BYTE_BIT	gu8_Buffer_MCU_RecData[Z_DataLen_Rx+2];	/* 	수신 데이터 버퍼	*/
EXTERN BYTE_BIT	gu8_Buffer_MCU_TraData[Z_DataLen_Tx+2];	/* 	송신 데이터 버퍼	*/
#endif

EXTERN U16 gu8_TimerRecWait;						/* 	수신 대기 타이머		<20ms/1ms> */
EXTERN U16 gu8_TimerTraWait;						/* 	송신 대기 타이머		<20ms/1ms> */

EXTERN U8 gu8_TimerTrahalt;							//	Tx 체크		<50ms/1ms>
EXTERN U8 gu8_TimerRecInt_Check;					//	Rx 체크		<30ms/1ms>

EXTERN U8 F_Rx_Complete;							//	수신성공 flag
EXTERN U8 F_Rx_Exp;									//	수신 경험

//EXTERN U8 gu8_CommuPacketType;

EXTERN U8 gu8_Front_OperationStep;
EXTERN U16 gu16_Front_ProgramVer;

EXTERN U16 gu16_Front_UV_FB;						//	에러는 없는건가?
EXTERN U16 gu16_Front_UV_FB_forLineTest;
EXTERN U16 gu16_FAN_FB_forLineTest;

EXTERN U8 gu8_TimerTraWait_Short;

EXTERN BYTE_BIT gu8_Key_Short;
	#define KEY_HOTWATER_SHORT 			(gu8_Key_Short.Bit.b0)
	#define KEY_PUREWATER_SHORT			(gu8_Key_Short.Bit.b1)
	#define KEY_COLDWATER_SHORT			(gu8_Key_Short.Bit.b2)
	#define KEY_AMOUNT_SHORT			(gu8_Key_Short.Bit.b3)
	#define KEY_EFFLUENT_SHORT 			(gu8_Key_Short.Bit.b4)

//EXTERN BYTE_BIT gu8_Key_Long;
//	#define KEY_HOTWATER_LONG			(gu8_Key_Long.Bit.b0)
//	#define KEY_COLDWATER_LONG			(gu8_Key_Long.Bit.b1)
//	#define KEY_PUREWATER_LONG			(gu8_Key_Long.Bit.b2)
//	#define KEY_AMOUNT_LONG				(gu8_Key_Long.Bit.b3)
//	#define KEY_HOTLOCK_LONG			(gu8_Key_Long.Bit.b4)
//	#define KEY_ECONOMY_LONG			(gu8_Key_Long.Bit.b5)
//
//EXTERN BYTE_BIT gu8_Key_SpecialLong1;
//	#define KEY_SILENT_LONG				(gu8_Key_SpecialLong1.Bit.b0)
//	#define KEY_FRONT_TEST_LONG			(gu8_Key_SpecialLong1.Bit.b1)
//	#define KEY_NORMAL_TEST_LONG		(gu8_Key_SpecialLong1.Bit.b2)
//	#define KEY_PBA_TEST_LONG			(gu8_Key_SpecialLong1.Bit.b3)
//	#define KEY_EEPROM_CLEAR_LONG		(gu8_Key_SpecialLong1.Bit.b4)
//	#define KEY_ERROR_HISTORY_LONG		(gu8_Key_SpecialLong1.Bit.b5)
//	#define KEY_ERROR_HIS_CLEAR_LONG	(gu8_Key_SpecialLong1.Bit.b6)
//	#define KEY_ALTITUDE_MODE_LONG		(gu8_Key_SpecialLong1.Bit.b7)
//
//EXTERN BYTE_BIT gu8_Key_SpecialLong2;
//	#define KEY_HOTTANK_CLEAR_LONG		(gu8_Key_SpecialLong2.Bit.b0)

//EXTERN BYTE_BIT gu8_Key_Level;
//	#define KEY_HOTWATER_LEVEL			(gu8_Key_Level.Bit.b0)
//	#define KEY_COLDWATER_LEVEL			(gu8_Key_Level.Bit.b1)
//	#define KEY_PUREWATER_LEVEL			(gu8_Key_Level.Bit.b2)
//	#define KEY_AMOUNT_LEVEL			(gu8_Key_Level.Bit.b3)
//	#define KEY_HOTLOCK_LEVEL			(gu8_Key_Level.Bit.b4)
//	#define KEY_ECONOMY_LEVEL			(gu8_Key_Level.Bit.b5)
//	#define KEY_COLD_FAUSET_LEVEL		(gu8_Key_Level.Bit.b6)
//	#define KEY_HOT_FAUSET_LEVEL		(gu8_Key_Level.Bit.b7)

EXTERN U8 F_CommunicationCMD;						//	즉시 통신 flag

EXTERN ITTI1 gu8_DigitalInput_Key1;					//	디지털 입력 데이터
	#define KEY_HOTWATER_LEVEL			(gu8_DigitalInput_Key1.Level.Bit.b0)
	#define KEY_HOTWATER_LOW_EDGE		(gu8_DigitalInput_Key1.OffEdge.Bit.b0)
	#define KEY_HOTWATER_HIGH_EDGE		(gu8_DigitalInput_Key1.OnEdge.Bit.b0)

	#define KEY_COLDWATER_LEVEL			(gu8_DigitalInput_Key1.Level.Bit.b1)
	#define KEY_COLDWATER_LOW_EDGE		(gu8_DigitalInput_Key1.OffEdge.Bit.b1)
	#define KEY_COLDWATER_HIGH_EDGE		(gu8_DigitalInput_Key1.OnEdge.Bit.b1)

	#define KEY_PUREWATER_LEVEL			(gu8_DigitalInput_Key1.Level.Bit.b2)
	#define KEY_PUREWATER_LOW_EDGE		(gu8_DigitalInput_Key1.OffEdge.Bit.b2)
	#define KEY_PUREWATER_HIGH_EDGE		(gu8_DigitalInput_Key1.OnEdge.Bit.b2)

	#define KEY_AMOUNT_LEVEL			(gu8_DigitalInput_Key1.Level.Bit.b3)
	#define KEY_AMOUNT_LOW_EDGE			(gu8_DigitalInput_Key1.OffEdge.Bit.b3)
	#define KEY_AMOUNT_HIGH_EDGE		(gu8_DigitalInput_Key1.OnEdge.Bit.b3)

	#define KEY_EFFLUENT_LEVEL			(gu8_DigitalInput_Key1.Level.Bit.b4)
	#define KEY_EFFLUENT_LOW_EDGE		(gu8_DigitalInput_Key1.OffEdge.Bit.b4)
	#define KEY_EFFLUENT_HIGH_EDGE		(gu8_DigitalInput_Key1.OnEdge.Bit.b4)

EXTERN ITTI3 gu8_Model_ThreeMatchData;						//
	#define MODEL_DECIDE				(gu8_Model_ThreeMatchData.Level.Bit.b0)

EXTERN U8 gu8_CountModelDecide;
EXTERN U8 F_ModelDecide_Complete;

EXTERN U32 gu32_FrontCommunication_Tx_Count;
EXTERN U32 gu32_FrontCommunication_Rx_Count;

/*===========================================================================*/
/*		CRC TABLE						     								 */
/*===========================================================================*/
EXTERN const U16 crc_table1[256]
#ifdef	GLOBAL_VARIABLE_DEFINE
= {
	0x0000U,0x1021U,0x2042U,0x3063U,0x4084U,0x50a5U,0x60c6U,0x70e7U,
	0x8108U,0x9129U,0xa14aU,0xb16bU,0xc18cU,0xd1adU,0xe1ceU,0xf1efU,
	0x1231U,0x0210U,0x3273U,0x2252U,0x52b5U,0x4294U,0x72f7U,0x62d6U,
	0x9339U,0x8318U,0xb37bU,0xa35aU,0xd3bdU,0xc39cU,0xf3ffU,0xe3deU,
	0x2462U,0x3443U,0x0420U,0x1401U,0x64e6U,0x74c7U,0x44a4U,0x5485U,
	0xa56aU,0xb54bU,0x8528U,0x9509U,0xe5eeU,0xf5cfU,0xc5acU,0xd58dU,
	0x3653U,0x2672U,0x1611U,0x0630U,0x76d7U,0x66f6U,0x5695U,0x46b4U,
	0xb75bU,0xa77aU,0x9719U,0x8738U,0xf7dfU,0xe7feU,0xd79dU,0xc7bcU,
	0x48c4U,0x58e5U,0x6886U,0x78a7U,0x0840U,0x1861U,0x2802U,0x3823U,
	0xc9ccU,0xd9edU,0xe98eU,0xf9afU,0x8948U,0x9969U,0xa90aU,0xb92bU,
	0x5af5U,0x4ad4U,0x7ab7U,0x6a96U,0x1a71U,0x0a50U,0x3a33U,0x2a12U,
	0xdbfdU,0xcbdcU,0xfbbfU,0xeb9eU,0x9b79U,0x8b58U,0xbb3bU,0xab1aU,
	0x6ca6U,0x7c87U,0x4ce4U,0x5cc5U,0x2c22U,0x3c03U,0x0c60U,0x1c41U,
	0xedaeU,0xfd8fU,0xcdecU,0xddcdU,0xad2aU,0xbd0bU,0x8d68U,0x9d49U,
	0x7e97U,0x6eb6U,0x5ed5U,0x4ef4U,0x3e13U,0x2e32U,0x1e51U,0x0e70U,
	0xff9fU,0xefbeU,0xdfddU,0xcffcU,0xbf1bU,0xaf3aU,0x9f59U,0x8f78U,
	0x9188U,0x81a9U,0xb1caU,0xa1ebU,0xd10cU,0xc12dU,0xf14eU,0xe16fU,
	0x1080U,0x00a1U,0x30c2U,0x20e3U,0x5004U,0x4025U,0x7046U,0x6067U,
	0x83b9U,0x9398U,0xa3fbU,0xb3daU,0xc33dU,0xd31cU,0xe37fU,0xf35eU,
	0x02b1U,0x1290U,0x22f3U,0x32d2U,0x4235U,0x5214U,0x6277U,0x7256U,
	0xb5eaU,0xa5cbU,0x95a8U,0x8589U,0xf56eU,0xe54fU,0xd52cU,0xc50dU,
	0x34e2U,0x24c3U,0x14a0U,0x0481U,0x7466U,0x6447U,0x5424U,0x4405U,
	0xa7dbU,0xb7faU,0x8799U,0x97b8U,0xe75fU,0xf77eU,0xc71dU,0xd73cU,
	0x26d3U,0x36f2U,0x0691U,0x16b0U,0x6657U,0x7676U,0x4615U,0x5634U,
	0xd94cU,0xc96dU,0xf90eU,0xe92fU,0x99c8U,0x89e9U,0xb98aU,0xa9abU,
	0x5844U,0x4865U,0x7806U,0x6827U,0x18c0U,0x08e1U,0x3882U,0x28a3U,
	0xcb7dU,0xdb5cU,0xeb3fU,0xfb1eU,0x8bf9U,0x9bd8U,0xabbbU,0xbb9aU,
	0x4a75U,0x5a54U,0x6a37U,0x7a16U,0x0af1U,0x1ad0U,0x2ab3U,0x3a92U,
	0xfd2eU,0xed0fU,0xdd6cU,0xcd4dU,0xbdaaU,0xad8bU,0x9de8U,0x8dc9U,
	0x7c26U,0x6c07U,0x5c64U,0x4c45U,0x3ca2U,0x2c83U,0x1ce0U,0x0cc1U,
	0xef1fU,0xff3eU,0xcf5dU,0xdf7cU,0xaf9bU,0xbfbaU,0x8fd9U,0x9ff8U,
	0x6e17U,0x7e36U,0x4e55U,0x5e74U,0x2e93U,0x3eb2U,0x0ed1U,0x1ef0U
}
#endif
;

/*==============================================================================
 function Definition
 ==============================================================================*/
void Communication_Initialize(void);
void CommunicationControl(void);

void UartInitial(void);							//	통신초기화
void UartReceive(U8 recdata);					//	Rx처리 함수 (인터럽트 내)
void UartControl(void);							//	통신처리 함수
void TXControl(void);							//	Tx처리함수
void RXControl(void);							//	Rx처리함수
void TraCrcCheck(void);							//	Tx CRC 체크
void Data_Parsing(void);						//	수신데이터 분석
U8 Crc_Check(void);								//	CRC 체크
void Send_Data(void);							//	통상 데이터 생성
void Uart_SFR_Refresh(void);					//	통신관련 SFR refresh
__interrupt void IntUartTx(void);				//	Tx interrupt
__interrupt void IntUartRx(void);				//	Rx interrupt

//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================



/*===========================================================================*/
#undef	EXTERN
#undef	GLOBAL_VARIABLE_DEFINE
/*===========================================================================*/
