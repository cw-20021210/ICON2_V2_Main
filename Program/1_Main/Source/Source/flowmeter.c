/// @file     watermeter.c
/// @date     2019.07.10
/// @author   PSM
/// @brief    유량센서 동작 제어 file
#define FLOWMETER_C
#include "include/_include.h"

#define TIMER_INPUT_COUNT_MINIMUM		(20)		//	타이머 카운트 최소치

#define Z_Timer_FlowSafety				(40)		//	<400ms/10ms>

typedef struct
{
	U8 Hz;
	U16 Standard;
} FlowMeter_Table;

#ifdef _DPP_FLOWMETER_
#define FlowMeter_NumMax	(1)
/*	유량센서 데이터시트 상 Hz별 정수값 지정 (Hz구간별 계산식 상이)	*/
FlowMeter_Table FlowMeter_Hz_Table[FlowMeter_NumMax] =
{
	{156, 	Z_FlowMeter_Standard_KOR},
};
#else
//#define FlowMeter_NumMax	(2)
///*	유량센서 데이터시트 상 Hz별 정수값 지정 (Hz구간별 계산식 상이)	*/
//FlowMeter_Table FlowMeter_Hz_Table[FlowMeter_NumMax] =
//{
//	{30, 	(Z_FlowMeter_Standard+Z_FlowMeter_Revise)},
//	{46, 	Z_FlowMeter_Standard},
//};
//#define FlowMeter_NumMax	(5)
///*	유량센서 데이터시트 상 Hz별 정수값 지정 (Hz구간별 계산식 상이)	*/
//FlowMeter_Table FlowMeter_Hz_Table[FlowMeter_NumMax] =
//{
//#ifdef _FOR_JAPAN_90DEGREE_CONTINUE
//	{6, 	Z_FlowMeter_Standard_forHotWater_forJapan},	//
//#else
//	{10, 	Z_FlowMeter_Standard_forHotWater_forJapan},	// 테이블 전체 디버깅
//#endif
//	{22, 	Z_FlowMeter_Standard_forHotWater},		//
//	{46, 	Z_FlowMeter_Standard},					//
//	{70, 	(3546+Z_FlowMeter_Revise)},							//	1.5LPM=70.5Hz => 1.5/70.5/60s
//	{94, 	(3527+Z_FlowMeter_Revise)},							//	2LPM=94.5Hz => 2/94.5/60s
//};
//#endif

#define FlowMeter_NumMax	(5)
/*	유량센서 데이터시트 상 Hz별 정수값 지정 (Hz구간별 계산식 상이)	*/
FlowMeter_Table FlowMeter_Hz_Table[FlowMeter_NumMax] =
{
	{10, 	(4461+Z_FlowMeter_Revise)},				// 테이블 전체 디버깅
	{22, 	Z_FlowMeter_Standard_forHotWater_KOR},	//
	{46, 	Z_FlowMeter_Standard_KOR},				//
	{70, 	(3546+Z_FlowMeter_Revise)},				//	1.5LPM=70.5Hz => 1.5/70.5/60s
	{94, 	(3527+Z_FlowMeter_Revise)},				//	2LPM=94.5Hz => 2/94.5/60s
};
#endif

#define FlowMeter_NumMax_Japan	(9)
/*	유량센서 데이터시트 상 Hz별 정수값 지정 (Hz구간별 계산식 상이)	*/
FlowMeter_Table FlowMeter_Hz_Table_Japan[FlowMeter_NumMax_Japan] =	// 저유량센서 적용 시 설계 필요
{
	{10, 	(2500+Z_FlowMeter_Revise_JP)},
	{22, 	(Z_FlowMeter_Standard_forHotWater_JP+Z_FlowMeter_Revise_JP)},
//	{10, 	(2500)},				//@021
//	{22, 	(Z_FlowMeter_Standard_forHotWater_JP)},	//@021
	{30, 	(2222+Z_FlowMeter_Revise_JP)},
	{72, 	(2083+Z_FlowMeter_Revise_JP)},
	{84, 	(Z_FlowMeter_Standard_JP+Z_FlowMeter_Revise_JP)},
	{93, 	(1971+Z_FlowMeter_Revise_JP)},
	{101, 	(1980+Z_FlowMeter_Revise_JP)},
	{110, 	(1969+Z_FlowMeter_Revise_JP)},
	{118, 	(1977+Z_FlowMeter_Revise_JP)},
};

/// @brief    유량센서 처리 초기화
/// @param    void
/// @return   void
void FlowMeter_Initialize(void)
{
	gu8_Timer_FlowSafety = Z_Timer_FlowSafety;		//

	gu16_Effluent_Hz = 0;                			// 	정량 추출동작 Hz(추출동작시 Flow Meter를 통해 Hz 체크)
	gu16_TimerEffluent = 0;              			// 	연속 추출시 연속 추출 시간 Timer

//	gu8_FlowMeter_NumMax = FlowMeter_NumMax;
	if (gu8_ModelType_forNational == MODEL_KOREA)
	{
		gu8_FlowMeter_NumMax = FlowMeter_NumMax;
	}
	else
	{
		gu8_FlowMeter_NumMax = FlowMeter_NumMax_Japan;
	}

//	if (gu8_ModelType_forNational == MODEL_KOREA)
//	{
//		gu16_FlowMeter_Standard = Z_FlowMeter_Standard;
//#ifdef _DPP_FLOWMETER_
//		gu16_FlowMeter_Standard_forHotWater = Z_FlowMeter_Standard;
//	//	gu16_FlowMeter_Standard_forHotWater_forJapan = Z_FlowMeter_Standard;
//#else
//		gu16_FlowMeter_Standard_forHotWater = Z_FlowMeter_Standard_forHotWater;
//	//	gu16_FlowMeter_Standard_forHotWater_forJapan = Z_FlowMeter_Standard_forHotWater_forJapan;
//#endif
//		gu16_Spec_FlowMeter = (U16)((U32)166666 / Z_FlowMeter_Standard);	//		유량센서 spec 기준 데이터 처리
//	}
//	else
//	{
//		gu16_FlowMeter_Standard = Z_FlowMeter_Standard_Japan;
//		gu16_FlowMeter_Standard_forHotWater = Z_FlowMeter_Standard_forHotWater_Japan;
//		gu16_Spec_FlowMeter = (U16)((U32)166666 / Z_FlowMeter_Standard_Japan);	//		유량센서 spec 기준 데이터 처리
//	}
}

/// @brief    Flow meter interrupt
/// @param    void
/// @return   void
void FlowMeter_INT(void)
{
//	EI();
	F_FlowCheck = SET;								//	인터럽트 발생 후 FlowCheck 필요 확정

#ifdef _FOR_FOREVER_EFF_
		if (F_FirstFlushing == SET)
		{
			gu16_TimerEffluent = 3000;
		}
#endif

#ifdef _FOR_PURE_EFF_30s_REPEAT_
		if (F_FirstFlushing == SET)
		{
			gu16_TimerEffluent = 3000;
		}
#endif

#ifdef _FOR_HEATER_ENDURANCE_TEST_
		if (F_Heater_Endurance_Test == SET)
		{
			gu16_Effluent_Hz = 1000;
		}
#endif


	if (gu8_OperationStep == Z_OperationStep_02)	//	플러싱 대기↔플러싱 중을 왔다갔다하면 02 스텝에서 잔여 추출량떄문에 Hz가 빠지는 경우가 있어서 제외 처리
	{

	}
	else if (gu8_OperationStep == Z_OperationStep_03)	//	플러싱 질소 퍼지를 위해 별도 처리함
	{
		if (F_First_Set == CLEAR)
		{
			if ((gu8_TimerFirstFlushing_AirPurge != 0) && (F_FirstFlushing_AirPurge == CLEAR))	//	초기 설치 플러싱 시, 내부 잔여 질소로 인해 수압이 밀어내면서 유량센서가 빠르게 흘러 20%가 그냥 Pass되는 것을 방지하기 위함
			{

			}
			else
			{
#ifndef _Flowmeter_Fake
				TDEC(gu16_Effluent_Hz);
				TDEC(gu16_Flushing_Hz_forDisplay);
#endif
			}
		}
		else
		{
#ifndef _Flowmeter_Fake
				TDEC(gu16_Effluent_Hz);
				TDEC(gu16_Flushing_Hz_forDisplay);
#endif
		}
	}
	else
	{
#ifndef _Flowmeter_Fake
				TDEC(gu16_Effluent_Hz);
				TDEC(gu16_Flushing_Hz_forDisplay);
#endif
	}

	if (gu8_OperationStep_hi == 0x50)				//	추출 중인 경우
	{
//		gu16_Effluent_Hz_Amount++;
		gu16_Effluent_Hz_Amount_forWIFI++;
	}

	gu16_FlowMeterTCR_Now = (U16)P_FLOWMETER_TDR;				//
}

/// @brief    유량계산
/// @param    void
/// @return   void
void FlowMeter_Decide(void)
{
	U8 mu8_i = 0;
	U16 mu16_buf = 0;
	U16 mu16_buf1 = 0;
	U16 mu16_buf2 = 0;
	U8 mu8_flag_Flow_NoDecide = 0;
	U8 i = 0;
	U8 mu8_Hz_buf = 0;
	U16 mu16_Standard_buf = 0;
	U16 mu16_Standard_diff = 0;
	U16 mu16_Standard_Calc = 0;
	FlowMeter_Table *Pointer_FlowMeter_Hz_Table = FlowMeter_Hz_Table;

#ifdef _FOR_DEVELOP_
	if (gu16_FlowDecide_Hz_Test != 0)
	{
		gu16_FlowDecide_Hz = gu16_FlowDecide_Hz_Test;
		return;
	}
	if (gu16_FlowDecide_W_Test != 0)
	{
		gu16_FlowDecide_W = gu16_FlowDecide_W_Test;
		return;
	}
	else
#endif
	// ※ Timer는 프리스케일 주파수가 125kHz로써  65536카운터가 약 524ms임 (최대치) 최소치는 8ns
	if (gu8_Timer_FlowSafety == 0) 					//	300ms(3.3Hz) 초과 시 (노이즈로 판단)
	{
		gu8_Timer_FlowSafety = Z_Timer_FlowSafety;					//		Timer SET
		gu16_FlowMeterTCR_Now = 0;					//		현재 데이터 Clear
		gu16_FlowMeterTCR_Before = gu16_FlowMeterTCR_Now;     //		이전 데이터 Clear
		gu8_Counter_FlowMeterTCR = 0;				//		카운터 Clear
		gu32_FlowMeterTCR_Sum = 0;					//		합산데이터 Clear
		gu16_FlowDecide = 0;						//		평균값 Clear
		gu16_FlowDecide_Hz = 0;						//		Hz 데아터 Clear
		gu16_FlowDecide_W = 0;						//		유량 데이터 Clear

		for (mu8_i = 0; mu8_i < Z_HOLESENSOR_NUM; mu8_i++)		//		배열 초기화
		{
			gu16_FlowMeterTCR_Array[mu8_i] = 0;
		}
	}
	else
	{
		if (F_FlowCheck == SET)						//		Interrupt 발생이 있으면
		{
			F_FlowCheck = CLEAR;					//			Flag Clear

			if (gu8_ModelType_forNational == MODEL_KOREA)
			{
				gu8_FlowMeter_NumMax = FlowMeter_NumMax;
				Pointer_FlowMeter_Hz_Table = FlowMeter_Hz_Table;
			}
			else
			{
				gu8_FlowMeter_NumMax = FlowMeter_NumMax_Japan;
				Pointer_FlowMeter_Hz_Table = FlowMeter_Hz_Table_Japan;
			}
//			if (gu16_FlowMeterTCR_Before >= gu16_FlowMeterTCR_Now) {     //		이전데이터가 현재데이터보다 크면 (Non-Underflow)
//				buf = gu16_FlowMeterTCR_Before - gu16_FlowMeterTCR_Now;  //			차이값 계산
//			} else {								//		현재데이터가 이전데이터보다 크면 (Underflow)
//				buf = (U16) (gu16_FlowMeterTCR_Before + (65535 - gu16_FlowMeterTCR_Now));     //			차이값 계산
//			}
			mu16_buf = gu16_FlowMeterTCR_Now;

			// ※ Timer는 프리스케일 주파수가 125kHz로써  65536카운터가 약 524ms임 (최대치) 최소치는 8ns
//			if (buf > 782){     //		782 Count 초과만 계산. 782Count = 6.2ms 160Hz이상이어서  1.6LPM (유량센서 Spec. 0.25~1.4LPM) 하한치는 상위 300ms 타이머로 안전처리
			if (mu16_buf > TIMER_INPUT_COUNT_MINIMUM) 	//		(상기 주석(> 782)이 맞는 설계이나, 라인검사에서 질소테스트는 엄청빠른 Hz이므로 제한 여유 (20은 하기 Hz계산시에 16비트 오버플로우 되지 않게))
			{
				gu16_FlowMeterTCR_Array[gu8_Counter_FlowMeterTCR] = mu16_buf;     //		배열에 저장
//				gu16_FlowMeterTCR_Before = gu16_FlowMeterTCR_Now;     //		이전데이터 현재데이터로 저장
				gu8_Counter_FlowMeterTCR++;			//		카운터 증가
				if (gu8_Counter_FlowMeterTCR >= Z_HOLESENSOR_NUM)//		카운터 Max일 시
				{
					gu8_Counter_FlowMeterTCR = 0;	//			Clear
				}
				for (mu8_i = 0; mu8_i < Z_HOLESENSOR_NUM; mu8_i++)	//		단속 평균이 아닌 연속 평균
				{
					gu32_FlowMeterTCR_Sum += gu16_FlowMeterTCR_Array[mu8_i];		//		합산
					if (gu16_FlowMeterTCR_Array[mu8_i] == 0) //		데이터 중 하나라도 0데이터가 있는 경우, 즉 최초 6회 측정 후부터 계산
					{
						mu8_flag_Flow_NoDecide = 1;			//			유량결정 안함
					}
				}
				if (mu8_flag_Flow_NoDecide != 1)			//			유량결정 확정인 경우
				{
					gu16_FlowDecide = (U16) (gu32_FlowMeterTCR_Sum / Z_HOLESENSOR_NUM);		//		데이터 평균
					gu32_FlowMeterTCR_Sum = 0;

					if (gu16_FlowDecide > TIMER_INPUT_COUNT_MINIMUM)	//		유효범위 인 경우
					{
						gu16_FlowDecide_Hz = (U16) (1250000 / gu16_FlowDecide);     //		Hz 계산  125Khz / 평균값	[1/10Hz]
						if ((F_TestMode_LineFront == SET) || (F_TestMode_LineAuto == SET))	//		공장 검사 중인 경우
						{
							if (gu16_FlowDecide_Hz_forLineAuto < gu16_FlowDecide_Hz)
							{
								gu16_FlowDecide_Hz_forLineAuto = gu16_FlowDecide_Hz;
							}
						}

						/*	유량센서 스펙 데이터 찾기 시작	*/
						if (((gu8_OperationStep_hi == 0x00) || (gu8_OperationStep_hi == 0x50)) && (gu8_TimerEffluent_After_2s == 0))
						{
//							for (i = (FlowMeter_NumMax-1) ; i >= 0 ; i--)
							for (i = (U8)((gu8_FlowMeter_NumMax-1)) ; i >= 0 ; i--)
							{
//								if ((gu16_FlowDecide_Hz/10) >= FlowMeter_Hz_Table[i].Hz)	//	현재 Hz별 스펙 정수값 찾기
								if ((gu16_FlowDecide_Hz/10) >= Pointer_FlowMeter_Hz_Table[i].Hz)	//	현재 Hz별 스펙 정수값 찾기
								{
//									if (i == (FlowMeter_NumMax-1))	//	마지막 배열까지 온 경우
									if (i == (gu8_FlowMeter_NumMax-1))	//	마지막 배열까지 온 경우
									{
										if (gu8_WaterSelect == HOT_WATER)
										{
											gu16_FlowMeter_Standard_forHotWater = Pointer_FlowMeter_Hz_Table[i].Standard;
										}
										else
										{
//											gu16_FlowMeter_Standard = FlowMeter_Hz_Table[i].Standard;
											gu16_FlowMeter_Standard = Pointer_FlowMeter_Hz_Table[i].Standard;
										}
									}
									else			//	스펙 찾은 경우
									{
//										mu8_Hz_buf = (U8)(abs(FlowMeter_Hz_Table[i+1].Hz - FlowMeter_Hz_Table[i].Hz));
//										mu16_Standard_buf = abs(FlowMeter_Hz_Table[i+1].Standard - FlowMeter_Hz_Table[i].Standard);	//	스펙 데이터 중간 값(편차) 구하기
										mu8_Hz_buf = (U8)(abs(Pointer_FlowMeter_Hz_Table[i+1].Hz - Pointer_FlowMeter_Hz_Table[i].Hz));
										mu16_Standard_buf = abs(Pointer_FlowMeter_Hz_Table[i+1].Standard - Pointer_FlowMeter_Hz_Table[i].Standard);	//	스펙 데이터 중간 값(편차) 구하기
										mu16_Standard_diff = (mu16_Standard_buf * 10) / mu8_Hz_buf;
										mu16_buf1 = (gu16_FlowDecide_Hz/10) - Pointer_FlowMeter_Hz_Table[i].Hz;	//	현재 Hz와 스펙 Hz 델타 구하기
										mu16_buf2 = (mu16_buf1 * mu16_Standard_diff) / 10;

//										if (FlowMeter_Hz_Table[i].Standard >= mu16_buf2)
										if (Pointer_FlowMeter_Hz_Table[i].Standard >= mu16_buf2)
										{
											if (gu8_WaterSelect == HOT_WATER)
											{
													gu16_FlowMeter_Standard_forHotWater = Pointer_FlowMeter_Hz_Table[i].Standard - mu16_buf2;
													gu16_Spec_FlowMeter = (U16)((U32)166666 / gu16_FlowMeter_Standard_forHotWater);		//		유량센서 spec 기준 데이터 처리
											}
											else
											{
//												gu16_FlowMeter_Standard = FlowMeter_Hz_Table[i].Standard - mu16_buf2;
												gu16_FlowMeter_Standard = Pointer_FlowMeter_Hz_Table[i].Standard - mu16_buf2;
												gu16_Spec_FlowMeter = (U16)((U32)166666 / gu16_FlowMeter_Standard);		//		유량센서 spec 기준 데이터 처리
											}
										}
										else
										{
											if (gu8_ModelType_forNational == MODEL_KOREA)
											{
												gu16_FlowMeter_Standard = Z_FlowMeter_Standard_KOR;
#ifdef _DPP_FLOWMETER_
												gu16_FlowMeter_Standard_forHotWater = Z_FlowMeter_Standard_KOR;
#else
												gu16_FlowMeter_Standard_forHotWater = Z_FlowMeter_Standard_forHotWater_KOR;
#endif
												gu16_Spec_FlowMeter = (U16)((U32)166666 / gu16_FlowMeter_Standard);		//		유량센서 spec 기준 데이터 처리
											}
											else
											{
												gu16_FlowMeter_Standard = Z_FlowMeter_Standard_JP;
#ifdef _DPP_FLOWMETER_
												gu16_FlowMeter_Standard_forHotWater = Z_FlowMeter_Standard_JP;
#else
												gu16_FlowMeter_Standard_forHotWater = Z_FlowMeter_Standard_forHotWater_JP;
#endif
												gu16_Spec_FlowMeter = (U16)((U32)166666 / gu16_FlowMeter_Standard);		//		유량센서 spec 기준 데이터 처리
											}
										}
									}
									break;
								}
								else if (i == 0)
								{
									if (gu8_WaterSelect == HOT_WATER)
									{
//#ifdef _FOR_JAPAN_90DEGREE_CONTINUE
//										if (gu8_ModelType_forNational == MODEL_JAPAN)
//#else
//										if ((gu8_ModelType_forNational == MODEL_JAPAN) && ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR85_JP80) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR70_JP70)))
//#endif
//										{
////											gu16_FlowMeter_Standard_forHotWater_forJapan = FlowMeter_Hz_Table[i].Standard;
//											gu16_FlowMeter_Standard_forHotWater_forJapan = Pointer_FlowMeter_Hz_Table[i].Standard;
//											gu16_Spec_FlowMeter = (U16)((U32)166666 / gu16_FlowMeter_Standard_forHotWater_forJapan);		//		유량센서 spec 기준 데이터 처리
//										}
//										else
//										{
//											gu16_FlowMeter_Standard_forHotWater = FlowMeter_Hz_Table[i].Standard;
											gu16_FlowMeter_Standard_forHotWater = Pointer_FlowMeter_Hz_Table[i].Standard;
											gu16_Spec_FlowMeter = (U16)((U32)166666 / gu16_FlowMeter_Standard_forHotWater);		//		유량센서 spec 기준 데이터 처리
//										}
									}
									else
									{
//										gu16_FlowMeter_Standard = FlowMeter_Hz_Table[i].Standard;
										gu16_FlowMeter_Standard = Pointer_FlowMeter_Hz_Table[i].Standard;
										gu16_Spec_FlowMeter = (U16)((U32)166666 / gu16_FlowMeter_Standard);		//		유량센서 spec 기준 데이터 처리
									}
									break;
								}
								else
								{

								}
							}
						}
						/*	유량센서 스펙 데이터 찾기 끝	*/
						if (gu8_WaterSelect == HOT_WATER)
						{
//#ifdef _FOR_JAPAN_90DEGREE_CONTINUE
//							if (gu8_ModelType_forNational == MODEL_JAPAN)
//#else
//							if ((gu8_ModelType_forNational == MODEL_JAPAN) && ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR85_JP80) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR70_JP70)))
//#endif
//							{
//								mu16_Standard_Calc = gu16_FlowMeter_Standard_forHotWater_forJapan;
//							}
//							else
//							{
								mu16_Standard_Calc = gu16_FlowMeter_Standard_forHotWater;
//							}
						}
						else
						{
							mu16_Standard_Calc = gu16_FlowMeter_Standard;
						}
						gu16_FlowDecide_W = (U16) ((U32) gu16_FlowDecide_Hz * mu16_Standard_Calc * 6 / 100000);     //	유량값 계산 [1/100LPM]
					}
				}
			}
			gu8_Timer_FlowSafety = Z_Timer_FlowSafety;				//	[300ms/10ms]
		}
	}

	if (F_100ms_for_FlowMeter_Average == SET)		//	100ms 마다 유량 평균치 계산
	{
		F_100ms_for_FlowMeter_Average = CLEAR;		//		flag clear
		if (gu8_OperationStep_hi == 0x50)			//		추출중인 경우
		{
			gu32_FlowDecide_W_Ave_Sum += gu16_FlowDecide_W;	//			유량 데이터 누적
			gu16_FlowDecide_W_Ave_Cnt++;			//		누적카운터 증가
			gu16_FlowDecide_W_Ave = (U16) (gu32_FlowDecide_W_Ave_Sum / gu16_FlowDecide_W_Ave_Cnt);	//			평균유량 계산
		}
		else
		{
//			gu16_FlowDecide_W_Ave = 0;
			gu32_FlowDecide_W_Ave_Sum = 0;			//		누적데이터 clear
			gu16_FlowDecide_W_Ave_Cnt = 0;			//		누적카운터 clear
		}
	}
}

/// @brief    추출유량 결정
/// @param    물 종류
/// @return   void
void Effluent_Hz_Decide(void)
{
//	if (gu8_HotStep == Z_HotStep_02)
//	{
//		gu16_Effluent_Hz = (U16)((U32)(150) * 10000 / gu16_FlowMeter_Standard);    //
//		gu16_FirstHotEff_Hz = gu16_Effluent_Hz;
//	}
//	else

	#if 0
	if (gu16_CupSize == CUPSIZE_CONTINUE)		//		연속추출인 경우
	#endif
	if(gu16_CupSize == CUPSIZE_CONTINUE)
	{
		gu16_TimerEffluent = 1800;					//			3분 set
		gu16_Effluent_Hz = 0;
	}
	else											//		연속 추출 외
	{
//		gu16_Effluent_Hz = CupSizeArray[mu8_waterkind][gu8_CupSize];    //		물 종류 및 컵사이즈별 데이터 set
#ifdef _FOR_CONDENSATION_TEST_
		gu16_Effluent_CupLitter = 1000;
#else
		switch (gu16_CupSize)
		{
			case CUPSIZE_HALFCUP:
				//if ((gu8_WaterSelect == HOT_WATER) && ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR85_JP80)))
				if ((gu8_WaterSelect == HOT_WATER) && ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__95) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__90)
						|| (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR85_JP80)))
				{
//@030					gu16_Effluent_CupLitter = 118;
//					gu16_Effluent_CupLitter = 115;	//@030
					gu16_Effluent_CupLitter = 120;	//@021
				}
				else
				{
					if (gu8_WaterSelect == COLD_WATER)
					{
						if (gi16_OutDoorAirSensor.DEGREE >= Z_TEMP30)
						{
							if (gu8_TimerColdEff_FirstCup == 0)
							{
								gu16_Effluent_CupLitter = 125;
							}
							else
							{
								gu16_Effluent_CupLitter = 120;
							}
						}
						else
						{
							gu16_Effluent_CupLitter = 120;
						}
					}
					else
					{
						gu16_Effluent_CupLitter = 120;
					}
				}
				break;
			case CUPSIZE_1CUP:
				if (gu8_ModelType_forNational == MODEL_KOREA)
				{
					gu16_Effluent_CupLitter = 250;
				}
				else
				{
					gu16_Effluent_CupLitter = 180;
				}
				break;
			case CUPSIZE_2CUP:
				if (gu8_ModelType_forNational == MODEL_KOREA)
				{
					gu16_Effluent_CupLitter = 500;
				}
				else
				{
					gu16_Effluent_CupLitter = 300;
				}
				break;
			case CUPSIZE_MY:
				gu16_Effluent_CupLitter = gu16_CupSize_My;
				break;
			default:
				if(gu8_F_CupSize_FineAdjust == SET)
				{
					gu16_Effluent_CupLitter = gu16_CupSize;
				}
				else
				{
					gu16_Effluent_Hz = 0;
				}
				break;
		}
#endif
		gu16_TimerEffluent = 0;
		if (gu8_WaterSelect == HOT_WATER)
		{
			gu16_Effluent_Hz = (U16)((U32)(gu16_Effluent_CupLitter) * 10000 / gu16_FlowMeter_Standard_forHotWater);	//
		}
		else
		{
			gu16_Effluent_Hz = (U16)((U32)(gu16_Effluent_CupLitter) * 10000 / gu16_FlowMeter_Standard);	//
		}
		if ((gu8_WaterSelect == HOT_WATER) && (gu16_CupSize == CUPSIZE_HALFCUP))
		{
			gu16_Effluent_CupLitter_forSegDisplay = 120;
		}
		else
		{
			gu16_Effluent_CupLitter_forSegDisplay = gu16_Effluent_CupLitter;
		}
		gu16_Effluent_Hz_forSegDisplay = gu16_Effluent_Hz;
	}
}

/// @brief    추출유량 적산
/// @param    물 종류
/// @return   void
void FlowAmount_ForMonitor(void)
{
//	U16 Standard_1L = 0;
//
//	Standard_1L = (U16)((U32)10000000 / gu16_FlowMeter_Standard);	//	1000cc의 Hz 계산
//
//	if (gu16_Effluent_Hz_FilterModule1 > Standard_1L)				//	1L
//	{
//		gu16_Effluent_Hz_FilterModule1 = 0;
//		if (gu16_Effluent_1L_FilterModule1 < 65535)
//		{
//			gu16_Effluent_1L_FilterModule1++;
//			if ((gu16_Effluent_1L_FilterModule1 == 65535) || ((gu16_Effluent_1L_FilterModule1 % 5) == 0))	//	EEPROM 수명으로 5리터마다 저장
//			{
//				gu8_FilterModule1_EEPROM_Low = (U8)(gu16_Effluent_1L_FilterModule1 & 0x00FF);
//				gu8_FilterModule1_EEPROM_High = (U8)((gu16_Effluent_1L_FilterModule1 >> 8) & 0x00FF);
//			}
//		}
//	}
//
//	if (gu16_Effluent_Hz_FilterModule2 > Standard_1L)				//	1L
//	{
//		gu16_Effluent_Hz_FilterModule2 = 0;
//		if (gu16_Effluent_1L_FilterModule2 < 65535)
//		{
//			gu16_Effluent_1L_FilterModule2++;
//			if ((gu16_Effluent_1L_FilterModule2 == 65535) || ((gu16_Effluent_1L_FilterModule2 % 5) == 0))	//	EEPROM 수명으로 5리터마다 저장
//			{
//				gu8_FilterModule2_EEPROM_Low = (U8)(gu16_Effluent_1L_FilterModule2 & 0x00FF);
//				gu8_FilterModule2_EEPROM_High = (U8)((gu16_Effluent_1L_FilterModule2 >> 8) & 0x00FF);
//			}
//		}
//	}
}
