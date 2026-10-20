/// @file     communication.c
/// @date     2021.01.12
/// @author   PSM
/// @brief    통신처리 file
/*==========================================================================*/
/*		PRAGMA						   			  							*/
/*==========================================================================*/
#pragma interrupt INTST3 Line_IntUartTx					  	/*	Tx Interrupt	*/
#pragma interrupt INTSR3 Line_IntUartRx					 	/* 	Rx Interrupt	*/

#define LINEAUTOTEST_C
#include "include/_include.h"

//=============================================================================
//		Macro 정의
//=============================================================================
#define SERIAL_RX3EN()			{SRIF3 = 0;				/*	Int Request Flag Register clear	*/\
								SRMK3 = 0;				/*	Int Mask Register clear			*/\
								SetIORBit(SS1L, 0x08);	/*	RX1 Start						*/}

#define SERIAL_TX3EN()			{STIF3 = 0;				/*	Int Request Flag Register clear	*/\
								STMK3 = 0;				/*	Int Mask Register clear			*/\
								SetIORBit(SOE1L, 0x04);	/*	Enables TX1 output Serial Set	*/\
								SetIORBit(SS1L, 0x04);	/*	TX1 Start						*/}

/// @brief    통신처리 초기화
/// @param    void
/// @return   void
void LineAutoTest_Initialize(void)
{
	gu8_TimerLineError_Time = Z_TimerLineError_Time; 		/*	Communication error 2sec timer set */
	gu8_TimerLineRecInt_Check = Z_TimerLineRecInt_Check;    //	Rec. interrupot 30ms check
	gu8_TimerLineDataReceive = Z_TimerLineDataReceive;		//	RX수신 에러 타이머 10초
}

/// @brief    통신처리 제어
/// @param    void
/// @return   void
void LineAutoTestControl(void)
{
	Line_TXControl();									//	TX control
	Line_RXControl();									//	RX control

//	if ((F_TestMode_LineAuto == SET) && (F_LineTest_Complete == SET) && (gu8_TimerLineTest_Complete == 0))
//	{
//		SafetyLoop();
//	}
}

/// @brief
/// @param    void
/// @return   void
__interrupt void Line_IntUartRx(void)
{
	U8 recdata;
	U16 work;

	/****************************************************/
	gu8_TimerLineRecInt_Check = Z_TimerLineRecInt_Check;	//	30ms check

	if ((SSR13 & 0x0007) != 0)
	{ 												/*	Status register check */
		work = RXD3; 								/*	Buffer set data register */
		SIR13 = 0x0007; 							/*	error flag clr */
		Line_Uart_SFR_Refresh(); 					/*	SFR refresh */
		gu16_LineTestErrorCount3++;
		return;
	}

	work = RXD3; 									/*	Save RX data	 */
	recdata = (U8) (work & 0x00FF); 				/*	Set lower 8bit data  */

	Line_UartReceive(recdata); 						/*	Receive process */
}

/// @brief
/// @param    void
/// @return   void
__interrupt void Line_IntUartTx(void)
{
	/************************************************/
	if (((SSR12 & 0x0046) != 0x00) || (f_Tra_LineStart == 0))	//	Processing TX or End TX
	{
		return;											//		return
	}

	if (gu8_CounterLineTraDataCount < Z_LineDataLen)	//	Incomplete TX
	{
		TXD3 = gu8_Buffer_Line_TraData[gu8_CounterLineTraDataCount].byte;    //		TX
		if (gu8_Buffer_Line_TraData[gu8_CounterLineTraDataCount].byte == Z_Line_ETX)
		{
			f_Tra_LineStart = 0;						//		Clear start flag
		}
		else
		{
			gu8_CounterLineTraDataCount++;				//		Increase counter
		}
	}
	else
	{
		f_Tra_LineStart = 0;							//		Clear start flag
	}
}

/// @brief
/// @param    void
/// @return   void
void Line_UartReceive(U8 recdata)
{
	/****************************************************/
	if (f_LineRec_End == 1)
	{												//	Over than RX data length
		f_LineRec_End = 0;							//		RX complete flag set
		gu8_CounterLineRecvCount = 0;				//		Counter clear
		Line_Uart_SFR_Refresh();					//		SFR refresh
		SERIAL_RX3EN();								//		RX enable
//		gu8_TimerError_Time = Z_TimerError_Time;	//		RX error time set
		gu16_LineTestErrorCount1++;
		return;
	}
	if (recdata == Z_Line_STX)
	{
		gu8_CounterLineRecvCount = 0;				//		Counter clear
	}
	gu8_Buffer_Line_RecData[gu8_CounterLineRecvCount].byte = recdata;     	//	Set data buffer

//	if (gu8_CounterLineRecvCount > (Z_LineDataLen - 1))
	gu8_CounterLineRecvCount++;						// 	Increase counter
	if (recdata == Z_Line_ETX)
	{												// 	End data length
		f_LineRec_End = 1;							//		RX complete flag set
	}
//	else
//	{
//		gu8_CounterLineRecvCount++;						// 	Increase counter
//	}
}

/// @brief
/// @param    void
/// @return   void
void Line_TXControl(void)
{
	U8 i;

//	if ((F_LineRx_Complete == 1) && (F_TestMode_LineAuto == SET) && (gu8_TimerHeaterFeedBack_Delay == 0))
	if ((F_LineRx_Complete == 1) && (F_TestMode_LineAuto == SET))
	{													//	수신 완료

	}
	else
	{
		gu8_TimerLineTraWait = Z_TimerLineTraWait;		//		10ms timer set
	}

	if ((gu8_TimerLineTraWait == 0))
	{													//	TX data set flag & 100ms delay
		F_LineRx_Complete = 0;							//		수신완료 flag clr
		gu8_Line_TraData_CS = 0;
		memset(gu8_Buffer_Line_TraData, 0x00, sizeof(gu8_Buffer_Line_TraData));
//		for (i = 0 ; i < Z_LineDataLen ; i++)
//		{												//		버퍼 클리어
//			gu8_Buffer_Line_TraData[i].byte = 0x00;		//			버퍼 초기화
//		}

		Line_Send_Data();

		gu8_CounterLineTraDataCount = 0;				//		TX counter clear
		f_Tra_LineDataMake = 0;							//		TX data set flag clear
		f_Tra_LineStart = 1;							//		TX start flag set
		gu8_TimerLineTrahalt = Z_TimerLineTrahalt;		//		TX timer 50ms set

		SERIAL_TX3EN();									//		TX enable

		TXD3 = gu8_Buffer_Line_TraData[0].byte;			//		TX first data transmission
		gu8_CounterLineTraDataCount++;					//		Increase counter
	}

	if ((gu8_CounterLineTraDataCount >= Z_LineDataLen) && ((SSR12 & 0x0040) != 0x0040))
	{
//		SERIAL_TX2DIS();								//		TX disable
		f_Tra_LineStart = 0;							//		Clear start flag
	}

	if ((f_Tra_LineStart == 1) && (gu8_TimerLineTrahalt == 0))
	{       											//	TX processing & TX error
		gu8_CounterLineTraDataCount = 0;				//		Counter clear
//		gu8_Buffer_TraCrc = 0;							//		CRC buffer clear
		f_Tra_LineStart = 0;							//		Clear start flag
		gu8_TimerLineTrahalt = Z_TimerLineTrahalt;		//		TX timer 50ms set
		gu8_TimerLineTraWait = Z_TimerLineTraWait;		//		TX error timer set
		Line_Uart_SFR_Refresh();						//		SFR refresh
		gu16_LineTestErrorCount4++;
	}
}

/// @brief
/// @param    void
/// @return   void
void Line_RXControl(void)
{
	U8 result;
	U8 work;
	U8 i;
	U8 buf1 = 0;
	U8 buf2 = 0;

	if ((gu8_CounterLineRecvCount >= 1) && (gu8_TimerLineRecInt_Check == 0))
	{    												//	수신 중 RX interrupt 30ms check
		gu8_CounterLineRecvCount = 0;					//		Count clear
		gu8_TimerLineRecInt_Check = Z_TimerLineRecInt_Check;	//		Rec. interrupot 30ms check
	}

	if (f_LineRec_End == 1)
	{													//	Complete RX
		f_LineRec_End = 0;								//		RX complete flag clear

//		gu8_Buffer_LineRecCrc = 0;						//		CRC buffer clear
		gu8_Line_RecData_CS = 0;
		gu8_Line_RecData_CS_Calc = 0;
//		SERIAL_RX2DIS();								//		RX disable

		for (i = 0 ; i < (gu8_CounterLineRecvCount - 3) ; i++)
		{
			gu8_Line_RecData_CS_Calc ^= gu8_Buffer_Line_RecData[i].byte;
		}

//		DisAsciiNibble(&buf1, &gu8_Buffer_Line_RecData[gu8_CounterLineRecvCount-1]);
//		DisAsciiNibble(&buf2, &gu8_Buffer_Line_RecData[gu8_CounterLineRecvCount-2]);
//
//		XORCS = buf1 & 0x0F;
//		XORCS = (buf2 << 4) & 0xF0U;

		DisAsciiByte(&gu8_Buffer_Line_RecData[gu8_CounterLineRecvCount-3].byte, &gu8_Line_RecData_CS);

		if (gu8_Line_RecData_CS == gu8_Line_RecData_CS_Calc)
		{
			result = 1;
		}
		else
		{
			gu16_LineTestErrorCount2++;
		}

		F_LineRx_Complete = 1;

		if (result == 1)								// 		CRC OK
		{
//			F_LineRx_Complete = 1;
			Line_Data_Parsing(); 						//			RX data parsing
			gu8_TimerLineDataReceive = Z_TimerLineDataReceive;	//			RX수신 에러 타이머 10초
			F_LineRx_Exp = 1;							//			수신 경험
			gu8_CounterLineRXDError = 0;
			F_LineAutoTest_Commu_Error = CLEAR;
			gu8_TimerLineError_Time = Z_TimerLineError_Time;	//		RX error timer set
			F_NACK_Tx = CLEAR;
		}
		else
		{												//		CRC NG
			Line_Uart_SFR_Refresh();					//			SFR refresh
			gu16_LineTestErrorCount5++;
			F_NACK_Tx = SET;
		}
		SERIAL_RX3EN();									//		RX enable
	}

	if (gu8_TimerLineError_Time == 0)
	{													//	RX 2s error
		gu8_CounterLineRecvCount = 0;					//		Counter clear
		Line_Uart_SFR_Refresh();						//		SFR refresh
		SERIAL_RX3EN();									//		RX enable

		gu8_CounterLineRXDError++;
		gu16_LineTestErrorCount6++;
		gu8_TimerLineError_Time = Z_TimerLineError_Time;//		RX error timer set
	}

	if (gu8_CounterLineRXDError >= 5)
	{
		gu8_CounterLineRXDError = 5;
		F_LineAutoTest_Commu_Error = SET;
	}
}

/// @brief
/// @param    void
/// @return   void
void Line_Data_Parsing(void)
{
	U8 buf1, buf2, buf3, buf4 = 0;

	gu16_Line_RecData_CMD = 0;
	gu8_Line_RecData_Data1 = 0;
	gu8_Line_RecData_Data2 = 0;
	gu8_Line_RecData_Data3 = 0;
	gu8_Line_RecData_Data4 = 0;

	DisAsciiWord(&gu8_Buffer_Line_RecData[1].byte, &gu16_Line_RecData_CMD);

	if (gu8_CounterLineRecvCount > 14)
	{
		DisAsciiByte(&gu8_Buffer_Line_RecData[5].byte, &gu8_Line_RecData_Data1);
		DisAsciiByte(&gu8_Buffer_Line_RecData[7].byte, &gu8_Line_RecData_Data2);
		DisAsciiByte(&gu8_Buffer_Line_RecData[9].byte, &gu8_Line_RecData_Data3);
		DisAsciiByte(&gu8_Buffer_Line_RecData[11].byte, &gu8_Line_RecData_Data4);
	}
	else if (gu8_CounterLineRecvCount > 12)
	{
		DisAsciiByte(&gu8_Buffer_Line_RecData[5].byte, &gu8_Line_RecData_Data1);
		DisAsciiByte(&gu8_Buffer_Line_RecData[7].byte, &gu8_Line_RecData_Data2);
		DisAsciiByte(&gu8_Buffer_Line_RecData[9].byte, &gu8_Line_RecData_Data3);
	}
	else if (gu8_CounterLineRecvCount > 10)
	{
		DisAsciiByte(&gu8_Buffer_Line_RecData[5].byte, &gu8_Line_RecData_Data1);
		DisAsciiByte(&gu8_Buffer_Line_RecData[7].byte, &gu8_Line_RecData_Data2);
	}
	else if (gu8_CounterLineRecvCount > 8)
	{
		DisAsciiByte(&gu8_Buffer_Line_RecData[5].byte, &gu8_Line_RecData_Data1);
	}
	else
	{

	}

	Line_CMD_Check();
}

/// @brief
/// @param    void
/// @return   void
void Line_Send_Data(void)
{
	U8 i;
	U8 mu8_OPENSHORT_Error;
	U8 mu8_hi, mu8_lo;
	U16 mu16_dec2hex;
	/************************************************/
//	memset(gu8_Buffer_Line_TraData, '\0', sizeof(gu8_Buffer_Line_TraData));

	if (F_NACK_Tx == SET)
	{
		gu8_Buffer_Line_TraData[0].byte = Z_Line_STX;
		gu8_Buffer_Line_TraData[1].byte = '1';
		gu8_Buffer_Line_TraData[2].byte = '5';
		gu8_Buffer_Line_TraData[3].byte = gu8_Buffer_Line_RecData[1].byte;
		gu8_Buffer_Line_TraData[4].byte = gu8_Buffer_Line_RecData[2].byte;
		gu8_Buffer_Line_TraData[5].byte = gu8_Buffer_Line_RecData[3].byte;
		gu8_Buffer_Line_TraData[6].byte = gu8_Buffer_Line_RecData[4].byte;
		gu8_Line_TraData_CS = (U8)(gu8_Buffer_Line_TraData[0].byte ^ gu8_Buffer_Line_TraData[1].byte ^ gu8_Buffer_Line_TraData[2].byte
							^ gu8_Buffer_Line_TraData[3].byte ^ gu8_Buffer_Line_TraData[4].byte ^ gu8_Buffer_Line_TraData[5].byte
							^ gu8_Buffer_Line_TraData[6].byte);
		gu8_Buffer_Line_TraData[7].byte = Ascii(gu8_Line_TraData_CS, Z_asciihi);
		gu8_Buffer_Line_TraData[8].byte = Ascii(gu8_Line_TraData_CS, Z_asciilo);
		gu8_Buffer_Line_TraData[9].byte = Z_Line_ETX;
	}
	else
	{
		gu8_Buffer_Line_TraData[0].byte = Z_Line_STX;
		gu8_Buffer_Line_TraData[1].byte = '0';
		gu8_Buffer_Line_TraData[2].byte = '6';
		gu8_Buffer_Line_TraData[3].byte = gu8_Buffer_Line_RecData[1].byte;
		gu8_Buffer_Line_TraData[4].byte = gu8_Buffer_Line_RecData[2].byte;
		gu8_Buffer_Line_TraData[5].byte = gu8_Buffer_Line_RecData[3].byte;
		gu8_Buffer_Line_TraData[6].byte = gu8_Buffer_Line_RecData[4].byte;

		switch (gu16_Line_RecData_CMD)
		{
			default:
				gu8_Buffer_Line_TraData[0].byte = Z_Line_STX;
				gu8_Buffer_Line_TraData[1].byte = '1';
				gu8_Buffer_Line_TraData[2].byte = '5';
				gu8_Buffer_Line_TraData[3].byte = gu8_Buffer_Line_RecData[1].byte;
				gu8_Buffer_Line_TraData[4].byte = gu8_Buffer_Line_RecData[2].byte;
				gu8_Buffer_Line_TraData[5].byte = gu8_Buffer_Line_RecData[3].byte;
				gu8_Buffer_Line_TraData[6].byte = gu8_Buffer_Line_RecData[4].byte;
				gu8_LineTra_Length = 7;
				break;
			case LINETEST_TESTMODE:
				if (F_TestMode_LineAuto == CLEAR)
				{

				}
				else
				{
					gu8_Buffer_Line_TraData[7].byte = '3';
					gu8_Buffer_Line_TraData[8].byte = '0';
					gu8_LineTra_Length = 9;
				}
				break;
			case LINETEST_SETTING_CHECK:
				if (F_EEPROM_CommunicationError == SET)		//	EEPROM 통신에러일 경우
				{
					gu8_Buffer_Line_TraData[7].byte = '0';	//		비정상 데이터 송신
					gu8_Buffer_Line_TraData[8].byte = '0';	//		비정상 데이터 송신
				}
				else
				{
					gu8_Buffer_Line_TraData[7].byte = 'A';	//	EEPROM
					gu8_Buffer_Line_TraData[8].byte = '5';	//	EEPROM
				}
				gu8_Buffer_Line_TraData[9].byte = Ascii((U8)(gu16_Front_ProgramVer & 0x00FF), Z_asciihi);	//	Front Prgoram Version
				gu8_Buffer_Line_TraData[10].byte = Ascii((U8)(gu16_Front_ProgramVer & 0x00FF), Z_asciilo);	//	Front Prgoram Version
				gu8_Buffer_Line_TraData[11].byte = Ascii((U8)(PROGRAM_VERSION_NO & 0x00FF), Z_asciihi);	//	Prgoram Version
				gu8_Buffer_Line_TraData[12].byte = Ascii((U8)(PROGRAM_VERSION_NO & 0x00FF), Z_asciilo);	//	Prgoram Version
				gu8_Buffer_Line_TraData[13].byte = '0';
				gu8_Buffer_Line_TraData[14].byte = Ascii(gu8_ModelType_forNational, Z_asciilo);
				gu8_Buffer_Line_TraData[15].byte = '0';	//
				gu8_Buffer_Line_TraData[16].byte = Ascii(gu8_ModelType_forCHP, Z_asciilo);
				gu8_LineTra_Length = 19;
				break;
			case LINETEST_WIFI_CHECK:
				gu8_Buffer_Line_TraData[7].byte = '0';
				if (F_WIFI_Pairing_forLineTest == SET)
				{
					gu8_Buffer_Line_TraData[8].byte = '1';
				}
				else
				{
					gu8_Buffer_Line_TraData[8].byte = '0';
				}
				gu8_LineTra_Length = 9;
				break;
			case LINETEST_UV_FEEDBACK:
				mu16_dec2hex = DEC2HEX_16BIT(gu16_Front_UV_FB_forLineTest);
//				gu8_Buffer_Line_TraData[7].byte = '0';
//				gu8_Buffer_Line_TraData[8].byte = '0';
				gu8_Buffer_Line_TraData[7].byte = Ascii((U8)((mu16_dec2hex >> 8) & 0xFF), Z_asciihi);
				gu8_Buffer_Line_TraData[8].byte = Ascii((U8)((mu16_dec2hex >> 8) & 0xFF), Z_asciilo);
				gu8_Buffer_Line_TraData[9].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciihi);
				gu8_Buffer_Line_TraData[10].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciilo);
				gu8_LineTra_Length = 11;
				break;
			case LINETEST_FLOWMETER_CHECK:
				gu8_Buffer_Line_TraData[7].byte = Ascii((U8)((gu16_FlowDecide_Hz_forLineAuto/10) >> 8), Z_asciihi);	//
				gu8_Buffer_Line_TraData[8].byte = Ascii((U8)((gu16_FlowDecide_Hz_forLineAuto/10) >> 8), Z_asciilo);	//
				gu8_Buffer_Line_TraData[9].byte = Ascii((U8)(gu16_FlowDecide_Hz_forLineAuto/10), Z_asciihi);	//
				gu8_Buffer_Line_TraData[10].byte = Ascii((U8)(gu16_FlowDecide_Hz_forLineAuto/10), Z_asciilo);	//
				gu8_LineTra_Length = 11;
				break;
			case LINETEST_CURRENT:
				mu16_dec2hex = DEC2HEX_16BIT((U16)gu16_TEM_mA_Result);
				gu8_Buffer_Line_TraData[7].byte = '0';
				gu8_Buffer_Line_TraData[8].byte = '0';
				gu8_Buffer_Line_TraData[9].byte = Ascii((U8)((mu16_dec2hex >> 8) & 0xFF), Z_asciihi);
				gu8_Buffer_Line_TraData[10].byte = Ascii((U8)((mu16_dec2hex >> 8) & 0xFF), Z_asciilo);
				gu8_Buffer_Line_TraData[11].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciihi);
				gu8_Buffer_Line_TraData[12].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciilo);
				mu16_dec2hex = DEC2HEX_16BIT((U16)gu16_DC_mA_Result);
				gu8_Buffer_Line_TraData[13].byte = '0';
				gu8_Buffer_Line_TraData[14].byte = '0';
				gu8_Buffer_Line_TraData[15].byte = Ascii((U8)((mu16_dec2hex >> 8) & 0xFF), Z_asciihi);
				gu8_Buffer_Line_TraData[16].byte = Ascii((U8)((mu16_dec2hex >> 8) & 0xFF), Z_asciilo);
				gu8_Buffer_Line_TraData[17].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciihi);
				gu8_Buffer_Line_TraData[18].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciilo);
				if (ADC_RESULT_FAN_FB != 0)
				{
					mu16_dec2hex = DEC2HEX_16BIT((U16)(ADC_RESULT_FAN_FB+10));
				}
				else
				{
					mu16_dec2hex = 0;
				}
				gu8_Buffer_Line_TraData[19].byte = '0';
				gu8_Buffer_Line_TraData[20].byte = '0';
				gu8_Buffer_Line_TraData[21].byte = Ascii((U8)((mu16_dec2hex >> 8) & 0xFF), Z_asciihi);
				gu8_Buffer_Line_TraData[22].byte = Ascii((U8)((mu16_dec2hex >> 8) & 0xFF), Z_asciilo);
				gu8_Buffer_Line_TraData[23].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciihi);
				gu8_Buffer_Line_TraData[24].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciilo);
				gu8_LineTra_Length = 25;
				break;
//			case LINETEST_FAN_FEEDBACK:
//				mu16_dec2hex = DEC2HEX_16BIT(gu16_FAN_FB_forLineTest);	//
//				gu8_Buffer_Line_TraData[9].byte = Ascii((U8)((mu16_dec2hex >> 8) & 0xFF), Z_asciihi);
//				gu8_Buffer_Line_TraData[10].byte = Ascii((U8)((mu16_dec2hex >> 8) & 0xFF), Z_asciilo);
//				gu8_Buffer_Line_TraData[11].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciihi);
//				gu8_Buffer_Line_TraData[12].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciilo);
//				gu8_LineTra_Length = 13;
//				break;
			case LINETEST_TH_CHECK:
				mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_OutDoorAirSensor.TH_ADC_AVERAGE);     //	OPEN SHORT 체크
				if (mu8_OPENSHORT_Error == 0)
				{
					mu16_dec2hex = DEC2HEX_16BIT((U16)abs(gi16_OutDoorAirSensor.DEGREE));
					if (gi16_OutDoorAirSensor.DEGREE < 0)
					{
						gu8_Buffer_Line_TraData[7].byte = '1';	//	음수
					}
					else
					{
						gu8_Buffer_Line_TraData[7].byte = '0';	//	양수
					}
					gu8_Buffer_Line_TraData[8].byte = Ascii((U8)((mu16_dec2hex >> 8) & 0xFF), Z_asciilo);
					gu8_Buffer_Line_TraData[9].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciihi);
					gu8_Buffer_Line_TraData[10].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciilo);
				}
				else
				{
					gu8_Buffer_Line_TraData[7].byte = '9';
					gu8_Buffer_Line_TraData[8].byte = '9';
					gu8_Buffer_Line_TraData[9].byte = '9';
					gu8_Buffer_Line_TraData[10].byte = '9';
				}

				mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_ColdSensor.TH_ADC_AVERAGE);     //	OPEN SHORT 체크
				if (mu8_OPENSHORT_Error == 0)
				{
					mu16_dec2hex = DEC2HEX_16BIT((U16)abs(gi16_ColdSensor.DEGREE));
					if (gi16_ColdSensor.DEGREE < 0)
					{
						gu8_Buffer_Line_TraData[11].byte = '1';	//	음수
					}
					else
					{
						gu8_Buffer_Line_TraData[11].byte = '0';	//	양수
					}
					gu8_Buffer_Line_TraData[12].byte = Ascii((U8)((mu16_dec2hex >> 8) & 0xFF), Z_asciilo);
					gu8_Buffer_Line_TraData[13].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciihi);
					gu8_Buffer_Line_TraData[14].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciilo);
				}
				else
				{
					gu8_Buffer_Line_TraData[11].byte = '9';
					gu8_Buffer_Line_TraData[12].byte = '9';
					gu8_Buffer_Line_TraData[13].byte = '9';
					gu8_Buffer_Line_TraData[14].byte = '9';
				}

				mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_HotInSensor.TH_ADC_AVERAGE);     //	OPEN SHORT 체크
				if (mu8_OPENSHORT_Error == 0)
				{
					mu16_dec2hex = DEC2HEX_16BIT((U16)abs(gi16_HotInSensor.DEGREE));
					if (gi16_HotInSensor.DEGREE < 0)
					{
						gu8_Buffer_Line_TraData[15].byte = '1';	//	음수
					}
					else
					{
						gu8_Buffer_Line_TraData[15].byte = '0';	//	양수
					}
					gu8_Buffer_Line_TraData[16].byte = Ascii((U8)((mu16_dec2hex >> 8) & 0xFF), Z_asciilo);
					gu8_Buffer_Line_TraData[17].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciihi);
					gu8_Buffer_Line_TraData[18].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciilo);
				}
				else
				{
					gu8_Buffer_Line_TraData[15].byte = '9';
					gu8_Buffer_Line_TraData[16].byte = '9';
					gu8_Buffer_Line_TraData[17].byte = '9';
					gu8_Buffer_Line_TraData[18].byte = '9';
				}

				mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_HotOutSensor.TH_ADC_AVERAGE);     //	OPEN SHORT 체크
				if (mu8_OPENSHORT_Error == 0)
				{
					mu16_dec2hex = DEC2HEX_16BIT((U16)abs(gi16_HotOutSensor.DEGREE));
					if (gi16_HotOutSensor.DEGREE < 0)
					{
						gu8_Buffer_Line_TraData[19].byte = '1';	//	음수
					}
					else
					{
						gu8_Buffer_Line_TraData[19].byte = '0';	//	양수
					}
					gu8_Buffer_Line_TraData[20].byte = Ascii((U8)((mu16_dec2hex >> 8) & 0xFF), Z_asciilo);
					gu8_Buffer_Line_TraData[21].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciihi);
					gu8_Buffer_Line_TraData[22].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciilo);
				}
				else
				{
					gu8_Buffer_Line_TraData[19].byte = '9';
					gu8_Buffer_Line_TraData[20].byte = '9';
					gu8_Buffer_Line_TraData[21].byte = '9';
					gu8_Buffer_Line_TraData[22].byte = '9';
				}

				mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_HeatSinkSensor.TH_ADC_AVERAGE);     //	OPEN SHORT 체크
				if (mu8_OPENSHORT_Error == 0)
				{
					mu16_dec2hex = DEC2HEX_16BIT((U16)abs(gi16_HeatSinkSensor.DEGREE));
					if (gi16_HeatSinkSensor.DEGREE < 0)
					{
						gu8_Buffer_Line_TraData[23].byte = '1';	//	음수
					}
					else
					{
						gu8_Buffer_Line_TraData[23].byte = '0';	//	양수
					}
					gu8_Buffer_Line_TraData[24].byte = Ascii((U8)((mu16_dec2hex >> 8) & 0xFF), Z_asciilo);
					gu8_Buffer_Line_TraData[25].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciihi);
					gu8_Buffer_Line_TraData[26].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciilo);
				}
				else
				{
					gu8_Buffer_Line_TraData[23].byte = '9';
					gu8_Buffer_Line_TraData[24].byte = '9';
					gu8_Buffer_Line_TraData[25].byte = '9';
					gu8_Buffer_Line_TraData[26].byte = '9';
				}

				mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_HeaterSensor.TH_ADC_AVERAGE);     //	OPEN SHORT 체크
				if (mu8_OPENSHORT_Error == 0)
				{
					mu16_dec2hex = DEC2HEX_16BIT((U16)abs(gi16_HeaterSensor.DEGREE));
					if (gi16_HeaterSensor.DEGREE < 0)
					{
						gu8_Buffer_Line_TraData[27].byte = '1';	//	음수
					}
					else
					{
						gu8_Buffer_Line_TraData[27].byte = '0';	//	양수
					}
					gu8_Buffer_Line_TraData[28].byte = Ascii((U8)((mu16_dec2hex >> 8) & 0xFF), Z_asciilo);
					gu8_Buffer_Line_TraData[29].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciihi);
					gu8_Buffer_Line_TraData[30].byte = Ascii((U8)(mu16_dec2hex & 0xFF), Z_asciilo);
				}
				else
				{
					gu8_Buffer_Line_TraData[27].byte = '9';
					gu8_Buffer_Line_TraData[28].byte = '9';
					gu8_Buffer_Line_TraData[29].byte = '9';
					gu8_Buffer_Line_TraData[30].byte = '9';
				}

				gu8_LineTra_Length = 31;
				break;
			case LINETEST_LEVEL_SENSOR:
				break;
			case LINETEST_ETC_SENSOR:
				/*	탑 커버 리드 스위치	*/
				gu8_Buffer_Line_TraData[7].byte = '0';
				if (REED_SW_COVER_Ready == SET)
				{
					gu8_Buffer_Line_TraData[8].byte = '1';
				}
				else
				{
					gu8_Buffer_Line_TraData[8].byte = '0';
				}

				/*	필터 리드 스위치	*/
				gu8_Buffer_Line_TraData[9].byte = '0';
				if (REED_SW_FILTER_Ready == SET)
				{
					gu8_Buffer_Line_TraData[10].byte = '1';
				}
				else
				{
					gu8_Buffer_Line_TraData[10].byte = '0';
				}

				/*	유로 리드 스위치	*/
				gu8_Buffer_Line_TraData[11].byte = '0';
				if (REED_SW_WATERFLOW_Ready == SET)
				{
					gu8_Buffer_Line_TraData[12].byte = '1';
				}
				else
				{
					gu8_Buffer_Line_TraData[12].byte = '0';
				}
				gu8_LineTra_Length = 13;
				break;
			case LINETEST_HEATER_FEEDBACK:
				/*	상/하판 별도 출력	*/
//				gu8_Buffer_Line_TraData[7].byte = Ascii((U8)((gu16_Heater_FB_Relay_forLineTest >> 8) & 0xFF), Z_asciihi);
//				gu8_Buffer_Line_TraData[8].byte = Ascii((U8)((gu16_Heater_FB_Relay_forLineTest >> 8) & 0xFF), Z_asciilo);
//				gu8_Buffer_Line_TraData[9].byte = Ascii((U8)(gu16_Heater_FB_Relay_forLineTest & 0xFF), Z_asciihi);
//				gu8_Buffer_Line_TraData[10].byte = Ascii((U8)(gu16_Heater_FB_Relay_forLineTest & 0xFF), Z_asciilo);
//
//				gu8_Buffer_Line_TraData[11].byte = Ascii((U8)((gu16_Heater_FB_Triac_forLineTest >> 8) & 0xFF), Z_asciihi);
//				gu8_Buffer_Line_TraData[12].byte = Ascii((U8)((gu16_Heater_FB_Triac_forLineTest >> 8) & 0xFF), Z_asciilo);
//				gu8_Buffer_Line_TraData[13].byte = Ascii((U8)(gu16_Heater_FB_Triac_forLineTest & 0xFF), Z_asciihi);
//				gu8_Buffer_Line_TraData[14].byte = Ascii((U8)(gu16_Heater_FB_Triac_forLineTest & 0xFF), Z_asciilo);
//				gu8_LineTra_Length = 15;

				/*	상/하판 동시 출력	*/
				if (gu16_Heater_FB_forLineTest >= 100)
				{
					gu16_Heater_FB_forLineTest -= 100;	//	순간 전압 상승(미안정화)으로 100W 감산
				}
				gu8_Buffer_Line_TraData[7].byte = Ascii((U8)((gu16_Heater_FB_forLineTest >> 8) & 0xFF), Z_asciihi);
				gu8_Buffer_Line_TraData[8].byte = Ascii((U8)((gu16_Heater_FB_forLineTest >> 8) & 0xFF), Z_asciilo);
				gu8_Buffer_Line_TraData[9].byte = Ascii((U8)(gu16_Heater_FB_forLineTest & 0xFF), Z_asciihi);
				gu8_Buffer_Line_TraData[10].byte = Ascii((U8)(gu16_Heater_FB_forLineTest & 0xFF), Z_asciilo);
				gu8_LineTra_Length = 11;
				break;
			case LINETEST_ALL_OFF:
			case LINETEST_COLDIN_VALVE_ON:
			case LINETEST_COLDIN_VALVE_OFF:
			case LINETEST_HOTIN_VALVE_ON:
			case LINETEST_HOTIN_VALVE_OFF:
			case LINETEST_PURE_VALVE_ON:
			case LINETEST_PURE_VALVE_OFF:
			case LINETEST_COLDOUT_VALVE_ON:
			case LINETEST_COLDOUT_VALVE_OFF:
			case LINETEST_HOTOUT_VALVE_ON:
			case LINETEST_HOTOUT_VALVE_OFF:
			case LINETEST_1STNOS_VALVE_ON:
			case LINETEST_1STNOS_VALVE_OFF:
			case LINETEST_FAN_ON:
			case LINETEST_FAN_OFF:
			case LINETEST_STEPPINGMOTOR_OPEN:
			case LINETEST_STEPPINGMOTOR_CLOSE:
			case LINETEST_TEM_ON:
			case LINETEST_TEM_OFF:
				gu8_LineTra_Length = 7;
				break;
			case LINETEST_TEST_COMPLETE:
				gu8_LineTra_Length = 7;
				break;
//			case ORIENTEC_TEST_START:
//			case ORIENTEC_TEST_COLDSENSOR_LOW_ON:
//			case ORIENTEC_TEST_COLDSENSOR_LOW_OFF:
//			case ORIENTEC_TEST_COLDSENSOR_HIGH_ON:
//			case ORIENTEC_TEST_COLDSENSOR_HIGH_OFF:
//			case ORIENTEC_TEST_BUZZER_OUTPUT:
//				gu8_LineTra_Length = 7;
//				break;
		}

		for (i = 0 ; i < gu8_LineTra_Length ; i++)
		{
			gu8_Line_TraData_CS ^= gu8_Buffer_Line_TraData[i].byte;
		}

		gu8_Buffer_Line_TraData[gu8_LineTra_Length].byte = Ascii(gu8_Line_TraData_CS, Z_asciihi);
		gu8_Buffer_Line_TraData[(gu8_LineTra_Length+1)].byte = Ascii(gu8_Line_TraData_CS, Z_asciilo);
		gu8_Buffer_Line_TraData[(gu8_LineTra_Length+2)].byte = Z_Line_ETX;
	}

	f_Tra_LineDataMake = 1;									//	Complete TX data set
}

/// @brief
/// @param    void
/// @return   void
void Line_Uart_SFR_Refresh(void)
{
	U8 work;
	/****************************************************/
	R_UART3_Stop();
	R_UART3_Create();
	R_UART3_Start();
//	ST0 |= 0x00C0;										//	Stop UART2
//
//	work = (U8) (SE1L & 0x0C);			//	SE register check (Prohibit write)
//	if (work == 0)
//	{									//		SE register check
//		SMR12 = 0x0022;		//	CK10 Clock, UART Mode, TX End interrupt (TX2)
//		SCR12 = 0x8097U;	//	TX Only, No Parity, LSB First, 1 Stop Bit, 8 Data Bit (TX2)
//		SDR12 = 0x3200;	//	Baud rate 9600, (16MHz/16)/(25+1)/2=19230bps (TX2)
//
//		SMR13 = 0x0122;	//	CK10 Clock, S/W Trigger, Falling Edge(Start Bit), UART Mode (RX2)
//		SCR13 = 0x4097;	//	RX Only, No Parity, LSB First, 1 Stop Bit, 8 Data Bit (RX2)
//		SDR13 = 0x3200;	//	Baud rate 9600, (16MHz/16)/(25+1)/2=19230bps (RX2)
//
//		SIR12 = 0x0007U;							//	Error flag clear (TX2)
//		SIR13 = 0x0007U;							//	Error flag clear (RX2)
//
//		NFEN0 |= 0x04;
//
//		SRPR03 = 0;								//	Interrupt priority level = 1
//		SRPR13 = 1;								//	Interrupt priority level = 1
//		STPR03 = 0;								//	Interrupt priority level = 1
//		STPR13 = 1;								//	Interrupt priority level = 1
//	}
//
//	SS0 |= 0x00C0; /* enable UART2 receive and transmit */
}

/// @brief
/// @param    void
/// @return   void
void Line_CMD_Check(void)
{
	if (F_TestMode_LineAuto == CLEAR)
	{
		switch (gu16_Line_RecData_CMD)
		{
			case LINETEST_TESTMODE:
				if ((F_LineAuto_Ready == SET) && (F_LineTest_Complete == CLEAR))
				{
					F_TestMode_LineAuto = SET;
					F_LineFrontTest_1stNos_VV_Complete = SET;	//@023
					gu16_LineTest_LoadCMD.Word = 0x0000U;
					F_LineFrontTest_Operation.byte = 0x00U;
					Play_Voice(VOICE_BEEP_ON);
				}
				else
				{
//					Play_Voice(BUZZER_ERROR);
				}
				break;
			default:
				break;
		}
	}
	else
	{
		switch (gu16_Line_RecData_CMD)
		{
//			case LINETEST_MODEL_CHECK:
//				break;
			case LINETEST_SETTING_CHECK:
				break;
			case LINETEST_WIFI_CHECK:
				break;
			case LINETEST_FLOWMETER_CHECK:
				break;
			case LINETEST_CURRENT:
				break;
			case LINETEST_TH_CHECK:
				break;
			case LINETEST_LEVEL_SENSOR:
				break;
			case LINETEST_ETC_SENSOR:
				break;
			case LINETEST_HEATER_FEEDBACK:
//				if (F_LineTest_Heater == CLEAR)
//				{
//					F_LineTest_Heater = SET;
//					gu8_TimerHeaterFeedBack_Delay = Z_TimerHeaterFeedBack_Delay;
//					gu8_TimerLineTest_Heater = Z_TimerLineTest_Heater;
//				}
				break;
			case LINETEST_ALL_OFF:
				gu16_LineTest_LoadCMD.Word = 0x0000U;
				break;
//			case LINETEST_HEATER_ON:
//				F_LineTest_Heater = SET;
//				break;
//			case LINETEST_HEATER_OFF:
//				F_LineTest_Heater = CLEAR;
//				break;
			case LINETEST_HOTIN_VALVE_ON:
				F_LineTest_HotIn_VV = SET;
				break;
			case LINETEST_HOTIN_VALVE_OFF:
				F_LineTest_HotIn_VV = CLEAR;
				break;
			case LINETEST_PURE_VALVE_ON:
				F_LineTest_Pure_VV = SET;
				break;
			case LINETEST_PURE_VALVE_OFF:
				F_LineTest_Pure_VV = CLEAR;
				break;
			case LINETEST_COLDIN_VALVE_ON:
				F_LineTest_ColdIn_VV = SET;
				break;
			case LINETEST_COLDIN_VALVE_OFF:
				F_LineTest_ColdIn_VV = CLEAR;
				break;
			case LINETEST_COLDOUT_VALVE_ON:
				F_LineTest_ColdOut_VV = SET;
				break;
			case LINETEST_COLDOUT_VALVE_OFF:
				F_LineTest_ColdOut_VV = CLEAR;
				break;
			case LINETEST_HOTOUT_VALVE_ON:
				F_LineTest_HotOut_VV = SET;
				break;
			case LINETEST_HOTOUT_VALVE_OFF:
				F_LineTest_HotOut_VV = CLEAR;
				break;
			case LINETEST_1STNOS_VALVE_ON:
				F_LineTest_1stNos_VV = SET;
				break;
			case LINETEST_1STNOS_VALVE_OFF:
				F_LineTest_1stNos_VV = CLEAR;
				break;
			case LINETEST_FAN_ON:
				F_LineTest_Fan = SET;
				break;
			case LINETEST_FAN_OFF:
				F_LineTest_Fan = CLEAR;
				break;
			case LINETEST_TEM_ON:
				F_LineTest_TEM = SET;
				break;
			case LINETEST_TEM_OFF:
				F_LineTest_TEM = CLEAR;
				break;
			case LINETEST_STEPPINGMOTOR_OPEN:
				StepMotor_ZeroBack_Open();
				break;
			case LINETEST_STEPPINGMOTOR_CLOSE:
				StepMotor_ZeroBack_Close();
				break;
			case LINETEST_TEST_COMPLETE:
				F_TestMode_LineAuto = CLEAR;
				gu16_LineTest_LoadCMD.Word = 0x0000U;
				F_LineTest_Complete = SET;
				F_LineFrontTest_Cold_Select = SET;
//				gu8_TimerLineTest_Complete = Z_TimerLineTest_Complete;
				Play_Voice(VOICE_BEEP_OFF);
				break;
			default:
				break;
		}
	}
}
