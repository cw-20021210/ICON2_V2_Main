/// @file     communication.c
/// @date     2021.01.12
/// @author   PSM
/// @brief    통신처리 file
/*==========================================================================*/
/*		PRAGMA						   			  							*/
/*==========================================================================*/
#pragma interrupt INTST2 IntUartTx					  	/*	Tx Interrupt	*/
#pragma interrupt INTSR2 IntUartRx					 	/* 	Rx Interrupt	*/

#define COMMUNICATION_C
#include "include/_include.h"

//=============================================================================
//		Macro 정의
//=============================================================================
#define SERIAL_RX2EN()			{SRIF2 = 0;				/*	Int Request Flag Register clear	*/\
								SRMK2 = 0;				/*	Int Mask Register clear			*/\
								SetIORBit(SS1L, 0x02);	/*	RX1 Start						*/}

#define SERIAL_TX2EN()			{STIF2 = 0;				/*	Int Request Flag Register clear	*/\
								STMK2 = 0;				/*	Int Mask Register clear			*/\
								SetIORBit(SOE1L, 0x01);	/*	Enables TX1 output Serial Set	*/\
								SetIORBit(SS1L, 0x01);	/*	TX1 Start						*/}

/// @brief    통신처리 초기화
/// @param    void
/// @return   void
void Communication_Initialize(void)
{
	SERIAL_TX2EN();									//		TX enable
	SERIAL_RX2EN();									//		RX enable
	gu8_TimerError_Time = Z_TimerError_Time; 		/*	Communication error 2sec timer set */
	gu8_TimerRecInt_Check = Z_TimerRecInt_Check;    //	Rec. interrupot 30ms check
	gu8_TimerDataReceive = Z_TimerDataReceive;		//	RX수신 에러 타이머 10초
}

/// @brief    통신처리 제어
/// @param    void
/// @return   void
void CommunicationControl(void)
{
	TXControl();									//	TX control
	RXControl();									//	RX control
}

/// @brief    통신 Rx 처리
/// @param    void
/// @return   void
__interrupt void IntUartRx(void)
{
	U8 recdata;
	U16 work;

	/****************************************************/
	gu8_TimerRecInt_Check = Z_TimerRecInt_Check;	//	30ms check

	if ((SSR11 & 0x0007) != 0)
	{ 												/*	Status register check */
		work = RXD2; 								/*	Buffer set data register */
		SIR11 = 0x0007; 							/*	error flag clr */
		gu8_CounterRXDError++;
		Uart_SFR_Refresh(); 						/*	SFR refresh */
		return;
	}

	work = RXD2; 									/*	Save RX data	 */
	recdata = (U8) (work & 0x00FF); 				/*	Set lower 8bit data  */

	UartReceive(recdata); 							/*	Receive process */
}

/// @brief    통신 Tx 처리
/// @param    void
/// @return   void
__interrupt void IntUartTx(void)
{
	/************************************************/
	if ((SSR10 & 0x0046) != 0x00)					//	Processing TX
	{
		gu8_CounterTXDError++;
		return;										//		return
	}

#ifndef _FRONT_COMMU_CHANGE_
	if (gu8_CounterTraDataCount < Z_DataLen)		//	Incomplete TX
#else
	if (gu8_CounterTraDataCount < Z_DataLen_Tx)		//	Incomplete TX
#endif
	{
		TXD2 = gu8_Buffer_MCU_TraData[gu8_CounterTraDataCount].byte;     //		TX
		gu8_CounterTraDataCount++;					//		Increase counter
	}
}

/// @brief    수신 처리
/// @param    void
/// @return   void
void UartReceive(U8 recdata)
{
	/****************************************************/
	if (f_Rec_End == 1)								//	Over than RX data length
	{
		f_Rec_End = 0;								//		RX complete flag set
		gu8_CounterRecvCount = 0;					//		Counter clear
		Uart_SFR_Refresh();							//		SFR refresh
		return;
	}
	gu8_Buffer_MCU_RecData[gu8_CounterRecvCount].byte = recdata;	//	Set data buffer

	gu8_CounterRecvCount++;							// 	Increase counter
#ifndef _FRONT_COMMU_CHANGE_
	if (gu8_CounterRecvCount > (Z_DataLen - 1))
#else
	if (gu8_CounterRecvCount > (Z_DataLen_Rx - 1))
#endif
	{												// 	End data length
		f_Rec_End = 1;								//		RX complete flag set
	}
}

/// @brief
/// @param    void
/// @return   void
void TXControl(void)
{
	U8 i;
	U8 F_TraOnce = 0;

//	if (F_Rx_Complete == SET)
//	{													//	수신 완료
//
//	}
//	else
//	{
//		gu8_TimerTraWait = Z_TimerTraWait;				//		10ms timer set
//	}

//	if ((F_CommunicationCMD == SET) && (f_Tra_Start == 0))	//	즉시 전송 커맨드 및 송신중이 아닌 경우
//	{
//		F_CommunicationCMD = CLEAR;						//		flag clear
//		gu8_TimerTraWait = 0;							//		송신 타이머 초기화 하여 즉시 전송
//	}

//	if (F_CommunicationCMD == CLEAR)
//	{
//		gu8_TimerTraWait_Short = Z_TimerTraWait_Short;
//	}
//	else
//	{
//		if ((gu8_TimerTraWait_Short == 0) && (f_Tra_Start == 0))
//		{
//			F_CommunicationCMD = CLEAR;
//			gu8_TimerTraWait_Short = Z_TimerTraWait_Short;
//			F_TraOnce = SET;
//		}
//	}

	if (F_CommunicationCMD == SET)
	{
		if ((gu8_TimerTraWait_Short == 0) && (f_Tra_Start == 0))
		{
			gu8_TimerTraWait_Short = Z_TimerTraWait_Short;
			F_TraOnce = SET;
			gu8_TimerTraWait = Z_TimerTraWait;				//		timer set
			F_CommunicationCMD = CLEAR;
		}
	}

	if ((gu8_TimerTraWait == 0) || (F_TraOnce == SET))
	{													//	TX data set flag
		F_Rx_Complete = 0;								//		수신완료 flag clr
		gu8_TimerTraWait = Z_TimerTraWait;				//		10ms timer set
#ifndef _FRONT_COMMU_CHANGE_
		for (i = 0 ; i < Z_DataLen ; i++)
#else
		for (i = 0 ; i < Z_DataLen_Tx ; i++)
#endif
		{												//		버퍼 클리어
			gu8_Buffer_MCU_TraData[i].byte = 0x00;		//			버퍼 초기화
		}

		Send_Data();									//		송신 data set

		gu8_CounterTraDataCount = 0;					//		TX counter clear
		f_Tra_DataMake = 0;								//		TX data set flag clear
		f_Tra_Start = 1;								//		TX start flag set
		gu8_TimerTrahalt = Z_TimerTrahalt;				//		TX timer 50ms set
		gu32_FrontCommunication_Tx_Count++;

		SERIAL_TX2EN();									//		TX enable

		TXD2 = gu8_Buffer_MCU_TraData[0].byte;			//		TX first data transmission
		gu8_CounterTraDataCount++;						//		Increase counter
	}

#ifndef _FRONT_COMMU_CHANGE_
	if ((gu8_CounterTraDataCount >= Z_DataLen) && ((SSR10 & 0x0040) != 0x0040))
#else
	if ((gu8_CounterTraDataCount >= Z_DataLen_Tx) && ((SSR10 & 0x0040) != 0x0040))
#endif
	{
//		SERIAL_TX2DIS();								//		TX disable
		f_Tra_Start = 0;								//		Clear start flag
	}

	if ((f_Tra_Start == 1) && (gu8_TimerTrahalt == 0))
	{       											//	TX processing & TX error
		gu8_CounterTraDataCount = 0;					//		Counter clear
		gu8_Buffer_TraCrc = 0;							//		CRC buffer clear
		f_Tra_Start = 0;								//		Clear start flag
		gu8_TimerTrahalt = Z_TimerTrahalt;				//		TX timer 50ms set
		gu8_TimerTraWait = Z_TimerTraWait;				//		TX error timer set
		Uart_SFR_Refresh();								//		SFR refresh
	}
}

/// @brief
/// @param    void
/// @return   void
void RXControl(void)
{
	U8 result;
	U8 work;
	U8 i;

	if ((gu8_CounterRecvCount >= 1) && (gu8_TimerRecInt_Check == 0))	//	수신 중 RX interrupt 30ms check
	{
		gu8_CounterRecvCount = 0;						//		Count clear
		gu8_TimerRecInt_Check = Z_TimerRecInt_Check;	//		Rec. interrupot 30ms check
	}

	if (f_Rec_End == 1)									//	Complete RX
	{
		f_Rec_End = 0;									//		RX complete flag clear
		gu8_CounterRecvCount = 0;						//		Counter clear
		gu8_Buffer_RecCrc = 0;							//		CRC buffer clear
		gu32_FrontCommunication_Rx_Count++;
//		SERIAL_RX2DIS();								//		RX disable

#ifndef _FRONT_COMMU_CHANGE_
		for (i = 0 ; i < (Z_DataLen - 2) ; i++)			//		CRC calculation (D1~D14)
#else
		for (i = 0 ; i < (Z_DataLen_Rx - 2) ; i++)			//		CRC calculation (D1~D14)
#endif
		{
			work = (U8) (((U16) (gu8_Buffer_RecCrc >> 8) ^ gu8_Buffer_MCU_RecData[i].byte) & 0x00ffU);
			gu8_Buffer_RecCrc = (gu8_Buffer_RecCrc << 8) ^ crc_table1[work];    //			CRC buffer set
		}

		result = Crc_Check();							//		CRC check

		if (result == 1)
		{												// 		CRC OK
			F_Rx_Complete = 1;
			Data_Parsing(); 							//			RX data parsing
			gu8_TimerDataReceive = Z_TimerDataReceive;	//			RX수신 에러 타이머 10초
			F_Rx_Exp = 1;								//			수신 경험
			gu8_CounterRXDError = 0;
			F_Front_Communication_Error = CLEAR;
			gu8_TimerError_Time = Z_TimerError_Time;	//		RX error timer set
		}
		else
		{												//		CRC NG
			Uart_SFR_Refresh();							//			SFR refresh
		}
	}

	if (gu8_TimerError_Time == 0)						//	RX 2s error
	{
		gu8_Key_Short.byte = 0x00;						//		Key data clear
		gu8_DigitalInput_Key1.Level.byte = 0x00;        //		Key data clear
		gu8_DigitalInput_Key1.First = 0x00;             //      Key data clear
		gu8_DigitalInput_Key1.OffEdge.byte = 0x00;      //      Key data clear
		gu8_DigitalInput_Key1.OnEdge.byte = 0x00;       //      Key data clear
//		gu8_DigitalInput_Key2.Level.byte = 0x00;        //      Key data clear
//		gu8_DigitalInput_Key2.First = 0x00;             //      Key data clear
//		gu8_DigitalInput_Key2.OffEdge.byte = 0x00;      //      Key data clear
//		gu8_DigitalInput_Key2.OnEdge.byte = 0x00;       //      Key data clear
		gu8_CounterRecvCount = 0;						//		Counter clear
		Uart_SFR_Refresh();								//		SFR refresh

		gu8_CounterRXDError++;
		gu8_TimerError_Time = Z_TimerError_Time;		//		RX error timer set
	}

	if (gu8_CounterRXDError >= 5)						//	수신 타이머 경과 5회 이상인 경우
	{
		gu8_CounterRXDError = 5;						//		5회 고정
		F_Front_Communication_Error = SET;				//		에러 플래그 set
	}
}

/// @brief
/// @param    void
/// @return   void
U8 Crc_Check(void)
{
	U16 work1, work2;
	U8 ret;

	//******	CRC buffer set		******//
#ifndef _FRONT_COMMU_CHANGE_
	work1 = ((U16) gu8_Buffer_MCU_RecData[Z_DataLen - 2].byte & 0x00FF);
	work2 = ((U16) gu8_Buffer_MCU_RecData[Z_DataLen - 1].byte & 0x00FF) << 8;
#else
	work1 = ((U16) gu8_Buffer_MCU_RecData[Z_DataLen_Rx - 2].byte & 0x00FF);
	work2 = ((U16) gu8_Buffer_MCU_RecData[Z_DataLen_Rx - 1].byte & 0x00FF) << 8;
#endif

	if (gu8_Buffer_RecCrc == (work1 | work2))
	{												//	CRC OK
		ret = 1;
	}
	else
	{												//	CRC NG
		ret = 0;
	}
	return (ret);
}

/// @brief
/// @param    void
/// @return   void
void Data_Parsing(void)
{
	gu8_Front_OperationStep = gu8_Buffer_MCU_RecData[3].byte;

	ThreeMatchModule(&gu8_Model_ThreeMatchData, gu8_Buffer_MCU_RecData[4].byte);
#ifdef _FRONT_JIG_PROGRAM_
	if (gu8_CountModelDecide < 2)
#else
	if (gu8_CountModelDecide < 5)
#endif
	{
		gu8_CountModelDecide++;
	}
	else
	{
		if (MODEL_DECIDE == SET)
		{
			gu8_ModelType_forCHP = MODEL_CHP;
		}
		else
		{
			gu8_ModelType_forCHP = MODEL_CP;
		}
		F_ModelDecide_Complete = SET;
	}

#ifdef _FOR_DEVELOP_
	if (gu8_ModelType_forCHP_Test != 0)
	{
		if (gu8_ModelType_forCHP_Test == 2)
		{
			gu8_ModelType_forCHP = MODEL_CP;
		}
		else
		{
			gu8_ModelType_forCHP = MODEL_CHP;
		}
	}
#endif

	KEY_HOTWATER_SHORT = gu8_Buffer_MCU_RecData[5].Bit.b0;
	KEY_PUREWATER_SHORT = gu8_Buffer_MCU_RecData[5].Bit.b1;
	KEY_COLDWATER_SHORT = gu8_Buffer_MCU_RecData[5].Bit.b2;
	KEY_AMOUNT_SHORT = gu8_Buffer_MCU_RecData[5].Bit.b3;
	KEY_EFFLUENT_SHORT = gu8_Buffer_MCU_RecData[5].Bit.b4;
//	if (gu8_Buffer_MCU_RecData[5].byte != 0)
//	{
//		NOP();
//	}

	OneMatchModule(&gu8_DigitalInput_Key1, gu8_Buffer_MCU_RecData[6].byte);

	gu16_Front_UV_FB = (U16)gu8_Buffer_MCU_RecData[8].byte;
	gu16_Front_UV_FB |= (U16)(gu8_Buffer_MCU_RecData[9].byte << 8);
	if (F_TestMode_LineFront == SET)
	{
		if ((gu8_UV_CMD == SET) && (gu16_Front_UV_FB > gu16_Front_UV_FB_forLineTest))
		{
			gu16_Front_UV_FB_forLineTest = gu16_Front_UV_FB;
		}
	}
#ifdef _FRONT_JIG_PROGRAM_
	if (gu8_UV_CMD == SET)
	{
		if ((gu8_Timer_FrontTest_UV_FBCheck == 0) && (gu16_Front_UV_FB < 10))
		{
			F_FrontTest_UV_FBError = SET;
		}
	}
	else
	{
		if (gu16_Front_UV_FB >= 10)				//
		{
			F_FrontTest_UV_FBError = SET;
		}
	}
#endif

#ifdef _FRONT_COMMU_CHANGE_
	gu16_Front_ProgramVer = (U16)gu8_Buffer_MCU_RecData[10].byte;
	gu16_Front_ProgramVer |= (U16)(gu8_Buffer_MCU_RecData[11].byte << 8);
#else
	gu16_Front_ProgramVer = (U16)gu8_Buffer_MCU_RecData[38].byte;
	gu16_Front_ProgramVer |= (U16)(gu8_Buffer_MCU_RecData[39].byte << 8);
#endif

//	gu8_CommuPacketType = Z_Commu_NormalData;
}

/// @brief
/// @param    void
/// @return   void
void Send_Data(void)
{
	/************************************************/
	gu8_Buffer_TraCrc = 0;										//	CRC clear

	gu8_Buffer_MCU_TraData[0].byte = Z_STX;						//	STX
	gu8_Buffer_MCU_TraData[1].byte = Z_Main_Address;			//	메인 Address
	gu8_Buffer_MCU_TraData[2].byte = Z_NormalData;				//	통상데이터
	gu8_Buffer_MCU_TraData[3].byte = gu8_OperationStep;			//	운전상태

	gu8_Buffer_MCU_TraData[4].byte = DisplayControl_Data[Z_SEGMENT].Display_CMD;
	gu8_Buffer_MCU_TraData[5].byte = SEG_OXX;
	gu8_Buffer_MCU_TraData[6].byte = SEG_XOX;
	gu8_Buffer_MCU_TraData[7].byte = SEG_XXO;

	gu8_Buffer_MCU_TraData[8].byte = DisplayControl_Data[Z_LED_EFFLUENT].Display_CMD;
	gu8_Buffer_MCU_TraData[9].byte = DisplayControl_Data[Z_LED_mL].Display_CMD;
	gu8_Buffer_MCU_TraData[10].byte = DisplayControl_Data[Z_LED_FILTER_CHANGE].Display_CMD;
	gu8_Buffer_MCU_TraData[11].byte = DisplayControl_Data[Z_LED_FAUCET].Display_CMD;
	gu8_Buffer_MCU_TraData[12].byte = DisplayControl_Data[Z_LED_2CUP].Display_CMD;
	gu8_Buffer_MCU_TraData[13].byte = DisplayControl_Data[Z_LED_1CUP].Display_CMD;
	gu8_Buffer_MCU_TraData[14].byte = DisplayControl_Data[Z_LED_HALFCUP].Display_CMD;
	gu8_Buffer_MCU_TraData[15].byte = DisplayControl_Data[Z_LED_STE].Display_CMD;
	gu8_Buffer_MCU_TraData[16].byte = DisplayControl_Data[Z_LED_DEGREE].Display_CMD;
	gu8_Buffer_MCU_TraData[17].byte = DisplayControl_Data[Z_LED_AMOUNT].Display_CMD;
	gu8_Buffer_MCU_TraData[18].byte = DisplayControl_Data[Z_LED_COLD_BLUE].Display_CMD;
	gu8_Buffer_MCU_TraData[19].byte = DisplayControl_Data[Z_LED_COLD_WHITE].Display_CMD;
	gu8_Buffer_MCU_TraData[20].byte = DisplayControl_Data[Z_LED_COLD].Display_CMD;
	gu8_Buffer_MCU_TraData[21].byte = DisplayControl_Data[Z_LED_PURE].Display_CMD;
	gu8_Buffer_MCU_TraData[22].byte = DisplayControl_Data[Z_LED_HOT].Display_CMD;
	gu8_Buffer_MCU_TraData[23].byte = DisplayControl_Data[Z_LED_HOTLOCK].Display_CMD;
	gu8_Buffer_MCU_TraData[24].byte = DisplayControl_Data[Z_LED_WIFI].Display_CMD;
	gu8_Buffer_MCU_TraData[25].byte = DisplayControl_Data[Z_LED_UV1].Display_CMD;
	gu8_Buffer_MCU_TraData[26].byte = DisplayControl_Data[Z_LED_UV2].Display_CMD;
	gu8_Buffer_MCU_TraData[27].byte = DisplayControl_Data[Z_LED_UV3].Display_CMD;
	gu8_Buffer_MCU_TraData[28].byte = DisplayControl_Data[Z_LED_UV4].Display_CMD;
	gu8_Buffer_MCU_TraData[29].byte = DisplayControl_Data[Z_LED_UV5].Display_CMD;
	gu8_Buffer_MCU_TraData[30].byte = DisplayControl_Data[Z_LED_CON].Display_CMD;

	gu8_Buffer_MCU_TraData[31].Bit.b0 = F_FOTA_Display;
	gu8_Buffer_MCU_TraData[31].Bit.b1 = gu8_UV_CMD;

	gu8_Buffer_MCU_TraData[32].byte = gu8_Fade_Select;
	gu8_Buffer_MCU_TraData[33].byte = gu8_ON_DimmingPercent;
	gu8_Buffer_MCU_TraData[34].byte = gu8_DimmingPercent;
	gu8_Buffer_MCU_TraData[35].byte = gu8_DimmingPercent_Low;

	gu8_Buffer_MCU_TraData[38].byte = (U8)(PROGRAM_VERSION_NO & 0xFFU);
	gu8_Buffer_MCU_TraData[39].byte = (U8)((PROGRAM_VERSION_NO >> 8) & 0xFFU);

	gu8_Buffer_MCU_TraData[40].byte = Z_ETX;
	TraCrcCheck();										//	Data CRC calculation
	gu8_Buffer_MCU_TraData[41].byte = (U8) (gu8_Buffer_TraCrc & 0x00FFU);				//	CRC1 set
	gu8_Buffer_MCU_TraData[42].byte = (U8) ((gu8_Buffer_TraCrc & 0xFF00U) >> 8);

	f_Tra_DataMake = 1;									//	Complete TX data set
}

/// @brief
/// @param    void
/// @return   void
void TraCrcCheck(void)
{
	U16 i;
	U16 work;
	//******	CRC data set (D1-D14)	******//
#ifndef _FRONT_COMMU_CHANGE_
	for (i = 0 ; i < (Z_DataLen - 2) ; i++)
#else
	for (i = 0 ; i < (Z_DataLen_Tx - 2) ; i++)
#endif
	{			//	CRC데이터 계산
		work = ((U16) (gu8_Buffer_TraCrc >> 8) ^ gu8_Buffer_MCU_TraData[i].byte) & 0x00ffU;
		gu8_Buffer_TraCrc = (gu8_Buffer_TraCrc << 8) ^ crc_table1[work];
	}
}

/// @brief
/// @param    void
/// @return   void
void Uart_SFR_Refresh(void)
{
	U8 work;
	/****************************************************/
	R_UART2_Stop();
	R_UART2_Create();
	R_UART2_Start();

	SERIAL_TX2EN();									//		TX enable
	SERIAL_RX2EN();									//		RX enable
}
