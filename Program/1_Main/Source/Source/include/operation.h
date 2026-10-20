/// @file   watermeter.h
/// @date   2019.07.10
/// @author PSM
/// @brief  유량센서 처리 관련 헤더 file

#ifndef _OPERATION_H_
#define _OPERATION_H_

#ifdef OPERATION_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_OPERATION()	{	/*	1ms 타이머 감산			*/	\
								}
#define	TDEC_10ms_OPERATION()	{	/*	10ms 타이머 감산		*/	\
									TDEC(gu16_TimerBootDisplay);	\
									TDEC(gu16_TimerVersionDisplay);	\
									TDEC(gu16_Timer_FrontTest_Seg);	\
									TDEC(gu16_TimerMoveDisplay);	\
									TDEC(gu8_TimerLineTest_Heater);	\
									/*TDEC(gu8_TimerHeaterFeedBack_Delay);*/	\
									TDEC(gu8_TimerReedSWCheck);	\
									TDEC(gu8_TimerReedSWCheck_B0);	\
									TDEC(gu8_TimerLineTest2_Heater);	\
									TDEC(gu8_TimerOperation_10ms);	\
								}
#define	TDEC_100ms_OPERATION()	{	/*	100ms 타이머 감산		*/	\
									TDEC(gu16_TimerOperation_100ms);		\
									TDEC(gu8_TimerCupSelect);	\
									TDEC(gu8_TimerWaterSelect);	\
									TDEC(gu8_TimerFCTSet);		\
									TDEC(gu8_TimerEffluentAmount_Display);	\
									TDEC(gu8_TimerOperation_100ms);	\
									TDEC(gu8_TimerHotLock_Display);	\
									TDEC(gu16_TimerUV_Oper);	\
									TDEC(gu8_TimerUV_Delay);	\
									TDEC(gu8_Timer_FrontTest_UV_FBCheck);	\
									TDEC(gu8_TimerFilterChange);	\
									TDEC(gu8_TimerWaterFlowChange);	\
									TDEC(gu8_TimerOFF_Flic);	\
									TDEC(gu8_TimerWiFiTest_Start);	\
									TDEC(gu8_TimerOverHeatDisplay);	\
									TDEC(gu8_TimerSoundChange);	\
									TDEC(gu8_TimerEffluent_After_2s);	\
									TDEC(gu16_Timer_Repeat_Delay);	\
									TDEC(gu16_Timer_Repeat_Delay_forMP);	\
									TDEC(gu8_TimerWifiErrorDisplay);	\
									TDEC(gu8_TimerFirstFlushing_AirPurge);	\
									TDEC(gu8_TimerLineTest_Heater_Delay);	\
									TDEC(gu8_TimerChangeDisplay);	\
									TDEC(gu8_TimerVoiceTest_forRBK);	\
									TDEC(gu16_TimerFCT_Incomplete_Display);/*@027*/\
									TDEC(gu16_TimerFCT_Incomplete_Display_forPASECO);/*@027*/\
									TDEC(gu8_TimerPIOff_Display);	/*@032*/\
									TDEC(gu8_Timer_AdFeedback_Mode_Exit);	\
								}
#define	TDEC_1s_OPERATION()		{	/*	1s 타이머 감산			*/	\
									TDEC(gu8_TimerPBATest_Join);	\
									TDEC(gu16_Timer_Repeat_Eff);	\
									TDEC(gu16_Timer_Repeat_Eff_forMP);	\
									TDEC(gu8_TimerHotWater_1CUP_EFF);	\
									TDEC(gu16_TimerKITMode);		\
									TDEC(gu16_TimerColdTank_Clear);	\
									TDEC(gu8_TimerLineTest2_Cooling);	\
									TDEC(gu8_Timer10sAfter_WifiConnect_forFlushing);	\
								}
#define	TDEC_10s_OPERATION()	{	/*	10s 타이머 감산			*/	\
								}
#define	TDEC_1min_OPERATION()	{	/*	1min 타이머 감산		*/	\
									TDEC(gu8_Timer_forDSC_RepeatEff_30min);	\
									TDEC(gu8_TimerSleepMode);	\
									TDEC(gu8_TimerHotWater_DegreeDisplay);	\
								}
#define	TDEC_10min_OPERATION()	{	/*	10min 타이머 감산		*/	\
									TDEC(gu8_TimerUV_6h);	\
									TDEC(gu16_TimerSilverCare);	\
								}
#define	TDEC_1h_OPERATION()		{	/*	1h 타이머 감산			*/	\
								}


/*==============================================================================
 Constant Definition
 ==============================================================================*/
#define MODEL_CP		(0)
#define MODEL_CHP		(1)

#define MODEL_KOREA		(0)
#define MODEL_JAPAN		(1)

enum {
	Z_OperationStepNum_00,
	Z_OperationStepNum_01,
	Z_OperationStepNum_02,
	Z_OperationStepNum_03,
	Z_OperationStepNum_04,
	Z_OperationStepNum_05,
	Z_OperationStepNum_10,
	Z_OperationStepNum_15,
	Z_OperationStepNum_16,
	Z_OperationStepNum_17,
	Z_OperationStepNum_18,
	Z_OperationStepNum_20,
	Z_OperationStepNum_30,
	Z_OperationStepNum_31,
	Z_OperationStepNum_40,
	Z_OperationStepNum_41,
	Z_OperationStepNum_42,
	Z_OperationStepNum_50,
	Z_OperationStepNum_70,
	Z_OperationStepNum_80,
	Z_OperationStepNum_90,
	Z_OperationStepNum_91,
	Z_OperationStepNum_92,
	Z_OperationStepNum_93,
	Z_OperationStepNum_A0,
	Z_OperationStepNum_B0,
	Z_OperationStepNum_C0,
	Z_OperationStepNum_D0,
	Z_OperationStepNum_D1,
	Z_OperationStepNum_E0,
	Z_OperationStepNum_E1,
	Z_OperationStepNum_F0,
	Z_OperationStepNum_F1,
	Z_OperationStepNum_Max
};

#define Z_OperationStep_00			(0x00)          //	부팅
#define Z_OperationStep_01			(0x01)          //	초기화 표시
#define Z_OperationStep_02			(0x02)          //	초기 플러싱 미완료
#define Z_OperationStep_03			(0x03)          //	초기 플러싱 중
#define Z_OperationStep_04			(0x04)          //	히터 물 채우기 및 전력체크
#define Z_OperationStep_05			(0x05)          //	냉수탱크 물 채우기
#define Z_OperationStep_10			(0x10)          //	대기
#define Z_OperationStep_15			(0x15)          //	ON 설정 표시
#define Z_OperationStep_16			(0x16)          //	냉각 OFF 표시
#define Z_OperationStep_17			(0x17)          //	무빙 ON/OFF 설정 표시
#define Z_OperationStep_18			(0x18)          //	OFF 설정 표시
#define Z_OperationStep_20			(0x20)          //	슬립모드
#define Z_OperationStep_30			(0x30)          //	UV케어
#define Z_OperationStep_31			(0x31)          //	정기 UV케어
#define Z_OperationStep_40			(0x40)          //	전체 잠금
#define Z_OperationStep_41			(0x41)          //	음성 타입
#define Z_OperationStep_42			(0x42)          //	관리 모드 설정
#define Z_OperationStep_50			(0x50)          //	추출 중
#define Z_OperationStep_70			(0x70)          //	FOTA
#define Z_OperationStep_80			(0x80)          //	스마트진단
#define Z_OperationStep_90			(0x90)          //	공장자동화 테스트
#define Z_OperationStep_91			(0x91)          //	PBA 테스트
#define Z_OperationStep_92			(0x92)          //	프론트 테스트
#define Z_OperationStep_93			(0x93)          //	공장 단축 테스트
#define Z_OperationStep_A0			(0xA0)          //	냉각시간 변경 스텝
#define Z_OperationStep_B0			(0xB0)          //	커버 열림
#define Z_OperationStep_C0			(0xC0)          //	냉수탱크 진공 해제
#define Z_OperationStep_D0			(0xD0)          //	TH 확인
#define Z_OperationStep_D1			(0xD1)          //	My용량 변경
#define Z_OperationStep_E0			(0xE0)          //	에러
#define Z_OperationStep_E1			(0xE1)          //	에러 이력
#define Z_OperationStep_F0			(0xF0)		//	살균키트 모드
#define Z_OperationStep_F1			(0xF1)		//	전압, 전류 피드백 확인 모드

#define Z_TimerWaterSelect			(70)			//	<7s/100ms>
#define Z_TimerCupSelect			(70)			//	<7s/100ms>

#define Z_TimerOperation_10ms_200ms	(20)
#define Z_TimerOperation_10ms_300ms	(30)
#define Z_TimerOperation_10ms_500ms	(50)

#define Z_TimerOperation_100ms_1s	(10)			//	<1s/100ms>
#define Z_TimerOperation_100ms_2s	(20)			//	<2s/100ms>
#define Z_TimerOperation_100ms_3s	(30)			//	<3s/100ms>
#define Z_TimerOperation_100ms_6s	(60)			//	<6s/100ms>
#define Z_TimerOperation_100ms_7s	(70)			//	<7s/100ms>
#define Z_TimerOperation_100ms_10s	(100)			//	<10s/100ms>
#define Z_TimerOperation_100ms_30s	(300)			//	<30s/100ms>
#define Z_TimerOperation_100ms_60s	(600)			//	<60s/100ms>
#define Z_TimerOperation_100ms_120s	(1200)			//	<120s/100ms>

#define Z_TimerBootDisplay			(200)			//	<?s/10ms>
#define Z_TimerVersionDisplay		(270)			//	<?s/10ms>
#define Z_TimerPBATestDisplay		(160)			//	<?s/10ms>

#define Z_TimerPBATest_Join			(30)			//	<30s/1s>

#define Z_TimerFCTSet			(10)				//	테스트모드 1, 2 Seg표시 타이머 	<1s/100ms>
#define Z_TimerFlrushing_After_ColdTankFill	(10)	//	플러싱 후 냉수탱크 물채우기 대기 	<1s/100ms>
#define Z_TimerFlrushing_After_HeaterFill	(10)	//	플러싱 후 히터 물채우기 대기 	<1s/100ms>

#define Z_TimerBuzzerDelay		(50)				//		<500ms/10ms>

#define Z_TimerEffluentAmount_Display	(20)		//	2초 점등		<2s/100ms>

#define Z_TimerKITMode			(300)				//	살균키트 모드 타이머	<300s/1s>

#define Z_Flushing_Litter		(3000+100)			//	3,000cc
#define Z_Flushing_Litter_FirstSet	(2000+100)		//@029	2,000cc	냉수탱크 향 제거를 위해 필터 플러싱 용량은 줄이고 냉수탱크 용량을 증대
#define Z_HeaterFill_Litter		(150)				//	150cc
//@029 #define Z_ColdTankFill_Litter	(1100+100)			//	1,100cc
//@033 #define Z_ColdTankFill_Litter	(4000+100)			//@029	4,000cc
#define Z_ColdTankFill_Litter_JP	(4000+100)			//@033	4,000cc(일본향)
#define Z_ColdTankFill_Litter_KR	(5000+100)			//@033	5,000cc(국내향)

#define Z_TimerHotLock_Display	(20)				//	<2s/100ms>

#define Z_Timer_FrontTest_Seg	(360)				//	8x3x150ms <3600ms/10ms>

#define Z_TimerUV_Oper			(30)				//	UV 동작시간			<3s/100ms>   
#define Z_TimerUV_Oper_15min	(9000)				//	슬립모드 UV 동작시간		<15min/100ms>  
#define Z_TimerUV_Delay			(70)				//	추출 후 UV 동작 지연시간	<7s/100ms>
#define Z_TimerUV_6h			(36)				//	슬립모드 UV 동작 주기	<6h/10min>  

#define Z_TimerSleepMode		(60)				//	<60min/1min>

#define Z_TimerMoveDisplay		(480)				//	<?s/100ms>
#define Z_TimerMoveDisplay_forTest		(700)		//	<?s/100ms>

//#define Z_TimerLineTest_Heater	(200)			//	<2000ms/10ms>
//#define Z_TimerLineTest_Heater	(50)				//	<500ms/10ms>
#define Z_TimerLineTest_Heater	(70)				//	<700ms/10ms>
//#define Z_TimerHeaterFeedBack_Delay	(250)		//	<2500ms/10ms>
//#define Z_TimerHeaterFeedBack_Delay	(100)		//	<1000ms/10ms>
#define Z_TimerLineTest_Heater_Delay	(20)		//	<2s/100ms>

#define Z_Timer_FrontTest_UV_FBCheck	(10)	//	<1s/100ms>

#define Z_TimerColdTank_Clear	(300)			//	<300s/1s>

#define Z_TimerFilterChange		(15)			//	<1.5s/100ms>
#define Z_TimerWaterFlowChange	(15)			//	<1.5s/100ms>

#define Z_TimerOFF_Flic			(20)			//	<2s/100ms>
#define Z_TimerWiFiTest_Start	(20)			//	<2s/100ms>

#define Z_TimerOverHeatDisplay	(30)			//	<3s/100ms>

#define Z_TimerSoundChange		(30)			//	<3s/100ms>

#define Z_TimerSilverCare		(288)			//	<48h/10min>

#define Z_TimerReedSWCheck		(50)			//	<500ms/10ms>

#define Z_TimerLineTest2_Cooling		(60)	//	<60s/1s>
//#define Z_TimerLineTest2_Heater			(50)	//	<500ms/10ms>

#define Z_TimerEffluent_After_2s	(20)		//	<2s/100ms>

#define Z_TimerWifiErrorDisplay		(50)		//	<5s/100ms>

#define Z_Timer10sAfter_WifiConnect_forFlushing		(10)	//	<10s/1s>

#define Z_TimerFirstFlushing_AirPurge		(50)	//	<5s/100ms>

#define Z_TimerChangeDisplay		(70)		//	<7s/100ms>

#define Z_TimerFCT_Incomplete_Display	(600)	//@027	<60s/100ms>

#define Z_TimerPIOff_Display		(30)		//@032	<3s/100ms>

    //-------------------------------------------20260223--------------------------JHKO
#define Z_FLUSHING_PROGRESS_OFF_FOR_WIFI			(0)			//	플러싱 안함
#define Z_FLUSHING_PROGRESS_ON_FOR_WIFI				(1)			//	플러싱 동작중(1회전송)
#define Z_FLUSHING_PROGRESS_GOING_FOR_WIFI			(2)			//	플러싱 시작(일시정지 시에도 유지)
#define Z_FLUSHING_PROGRESS_DONE_FOR_WIFI			(3)			//	플러싱 완료 상태(1회전송)

EXTERN U8 gu8_FlushingProgress_forWifi;				//	플러싱 상태 서버 전송위한 상태


#define Z_CUP_SIZE_ARRAY_ONE		(0)
#define Z_CUP_SIZE_ARRAY_TWO		(1)
#define Z_CUP_SIZE_ARRAY_THREE		(2)
#define Z_CUP_SIZE_ARRAY_CONTINUE	(3)
//#define Z_CUP_SIZE_ARRAY_FOUR		(3)
//#define Z_CUP_SIZE_ARRAY_CONTINUE	(4)

/*==============================================================================
 Variable Definition
 ==============================================================================*/
EXTERN U8 gu8_OperationStepNum;

EXTERN U16 gu16_TimerBootDisplay;
EXTERN U16 gu16_TimerVersionDisplay;

EXTERN U16 gu16_Flushing_Hz_forDisplay;
EXTERN U16 gu16_Flushing_TotalHz_forDisplay;

EXTERN U8 gu8_ModelType_forNational;
EXTERN U8 gu8_ModelType_forCHP; 

EXTERN U8 F_Boot_forBuzzer;

//EXTERN U8 gu8_Flushing_Percent;						//	플러싱 진행률 표시 [1%]
//EXTERN U8 gu8_HotFirstEff_Percent;					//	온수 첫회 추출 진행률 표시 [1%]

EXTERN U8 gu8_TimerPBATest_Join;					//	PBA 검사 진입 가능 타이머 	<30s/1s>

EXTERN U8 gu8_TimerFCTSet;							//	테스트모드 1, 2 Seg표시 타이머 	<1s/100ms>

EXTERN U16 gu16_Effluent_Count[3];					// 	냉수 : 0, 정수 : 1, 온수 : 2
EXTERN U16 gu16_Effluent_Amount[3];					// 	[1L단위] 냉수 : 0, 정수 : 1, 온수 : 2

EXTERN U8 gu8_TimerSleepMode;						//	<60min/1min>

/*	반복추출 내구시험 용	*/
EXTERN U8 F_Repeat_Eff;
EXTERN U16 gu16_Timer_Repeat_Eff;
EXTERN U16 gu16_Timer_Repeat_Delay;
EXTERN U8 F_Repeat_StepChange;
EXTERN U8 gu8_Counter_Repeat_Eff;

EXTERN U8 gu8_TimerHotWater_1CUP_EFF;
EXTERN U8 gu8_Timer_forDSC_RepeatEff_30min;
EXTERN U8 F_HotWater_1CUP_Eff;
/*	...		*/

/*	양산용 프로그램 출하테스트 반복 내구시험 용	*/
EXTERN U8 F_Repeat_Eff_Mode_forMP;				//@003	반복추출 모드 설정 flag
EXTERN U8 F_Repeat_Eff_forMP;					//@003	반복추출 시작 flag
EXTERN U16 gu16_Timer_Repeat_Eff_forMP;			//@003	추출 대기 딜레이
EXTERN U16 gu16_Timer_Repeat_Delay_forMP;		//@003	추출 전 선택 버튼 딜레이
EXTERN U8 F_Repeat_StepChange_forMP;			//@003
EXTERN U8 gu8_Counter_Repeat_Eff_forMP;			//@003
/*	...		*/


EXTERN U8 gu8_TimerEffluentAmount_Display;			//	추출 중도 정지 시, 얼마나 추출 되었는지 점멸 표시하기 위함

EXTERN U16 gu16_TimerKITMode;						//	살균키트 모드 타이머
EXTERN U8 F_KITMode;								//	살균키트 모드 진입 시 필터 교체 감지 set안되도록
EXTERN U8 F_KITMode_VoiceRinse;						//	살균키트 모드 중 헹굼이 시작됩니다 음성 출력 여부 플래그

EXTERN U8 gu8_TimerOperation_10ms;
EXTERN U8 gu8_TimerOperation_100ms;
EXTERN U16 gu16_TimerOperation_100ms;				//	<?/100ms>

EXTERN U8 gu8_TimerHotLock_Display;					//	<2s/100ms>

EXTERN U16 gu16_Timer_FrontTest_Seg;				//	<?s/100ms>

EXTERN U16 gu16_TimerUV_Oper;						//	<?s/100ms>
EXTERN U8 gu8_TimerUV_Delay;
EXTERN U8 gu8_TimerUV_6h;							//	<6h/10min>
EXTERN U8 F_UVCare_Oper;							//	추출 종료 후 UV캐어 동작 flag
EXTERN U8 F_UVCare_Start_forWIFI;

EXTERN U16 gu16_TimerMoveDisplay;					//	<?s/10ms>

EXTERN U8 F_LineTest_Heater;
EXTERN U8 gu8_TimerLineTest_Heater;					//	<?/10ms>
EXTERN U8 gu8_TimerLineTest_Heater_Delay;			//	3회 Retry Delay 	<2s/100ms>
//EXTERN U8 gu8_TimerHeaterFeedBack_Delay;			//	<?/10ms>
//EXTERN U8 gu8_TimerLineTest_HeaterTriac;
EXTERN U8 F_LineTest_UV_FBError;
EXTERN U8 REED_SW_COVER_Ready;
EXTERN U8 REED_SW_FILTER_Ready;
EXTERN U8 REED_SW_WATERFLOW_Ready;

EXTERN U8 gu8_Timer_FrontTest_UV_FBCheck;

EXTERN U8 F_FrontTest_UV_FBError;

EXTERN U8 F_FilterChange_Detect;
EXTERN U8 F_FaucetChange_Detect;

EXTERN U16 gu16_TimerColdTank_Clear;					//	<300s/1s>

EXTERN U8 gu8_TimerFilterChange;					//	<2s/100ms>
EXTERN U8 gu8_TimerWaterFlowChange;					//	<2s/100ms>
EXTERN U8 gu8_TimerOFF_Flic;						//	<2s/100ms>

EXTERN U8 gu8_TimerWiFiTest_Start;					//	<?s/100ms>
EXTERN U8 F_WiFi_Start;

EXTERN U8 F_CupSize_MySet;
EXTERN U16 gu16_CupSize_My;

EXTERN U8 gu8_TimerOverHeatDisplay;					//	<3s/100ms>

EXTERN U8 gu8_TimerSoundChange;						//	<3s/100ms>

EXTERN U16 gu16_TimerSilverCare;					//	<48h/10min>
EXTERN U8 F_SilverCare_Detect;						//	WIFI 실버케어 상태
EXTERN U8 F_SilverCare_Notice;						//	WIFI 실버케어 알림

EXTERN U8 gu8_TimerReedSWCheck;						//	<500ms/10ms>
EXTERN U8 gu8_TimerReedSWCheck_B0;

EXTERN U8 F_LineTest2_HeaterResult_OK;
EXTERN U8 F_LineTest2_HeaterResult_NG;
EXTERN U8 F_LineTest2_Purge;
EXTERN U8 F_LineTest2_Heater;
EXTERN U8 F_LineTest2_Valve;
EXTERN U8 F_LineTest2_Cooling;
EXTERN U8 F_LineTest2_CoolingResult_OK;
EXTERN U8 F_LineTest2_CoolingResult_NG;
EXTERN I16 gi16_LineTest2_HeatSinkSensor_Backup;

EXTERN U8 gu8_TimerLineTest2_Cooling;				//	<105s/1s>
EXTERN U8 gu8_TimerLineTest2_Heater;				//	<300ms/10ms>

EXTERN U16 gu16_HeartServiceTime;					//	하트서비스 시간 <1s>

EXTERN U8 gu8_TimerEffluent_After_2s;				//	추출시작 후 2초	<2s/100ms>

EXTERN U8 gu8_WifiError;
EXTERN U8 gu8_TimerWifiErrorDisplay;				//	<3s/100ms>

EXTERN U8 gu8_Timer10sAfter_WifiConnect_forFlushing;	//	플러싱 시작 후 10초 후에 와이파이 연결 시작 <10s/1s>
EXTERN U8 gu8_TimerHotWater_DegreeDisplay;			//	<60min/1min>

EXTERN U8 gu8_TimerFirstFlushing_AirPurge;			//	초기 설치 플러싱 시, 내부 잔여 질소로 인해 수압이 밀어내면서 유량센서가 빠르게 흘러 20%가 그냥 Pass되는 것을 방지하기 위함	<5s/100ms>

EXTERN U8 gu8_TimerChangeDisplay;					//	파우셋/필터교체 알람 점멸 표시, 추출 종료 후 7초 간 표시 유지 <7s/100ms>

EXTERN U8 gu8_TimerVoiceTest_forRBK;

EXTERN U16 gu16_TimerFCT_Incomplete_Display;			//@027
EXTERN U16 gu16_TimerFCT_Incomplete_Display_forPASECO;	//@027

EXTERN U8 gu8_TimerPIOff_Display;					//@032	PI제어 미설정 OFF표시 타이머 <3s/100ms>

EXTERN U8 F_WIFI_First_Connecting;					//@033	플러싱 중 wifi 최초 1회 자동 연결 시도 여부 Flag [Clear : 미시도 / Set : 1회 시도 완료]

//EXTERN CupSize_Data gu8_Cup_size_Array[5];
EXTERN CupSize_Data gu8_Cup_size_Array[4];
EXTERN U8 gu8_CupIndex;
EXTERN U8 gu8_F_CupSize_FineAdjust;
EXTERN U8 gu8_hot_water_select_index;		//cbr_test

EXTERN U8 gu8_Timer_AdFeedback_Mode_Exit;
EXTERN U8 F_EFF_Continue_Heater_OFF;  //JHKO
/*==============================================================================
 function Definition
 ==============================================================================*/
void Operation_Initialize(void);
void Operation(void);

void Ini_OperationStep_00(void);     				//	부팅
void Ini_OperationStep_01(void);     				//	초기화 표시
void Ini_OperationStep_02(void);     				//	초기 플러싱 미완료
void Ini_OperationStep_03(void);     				//	초기 플러싱 중
void Ini_OperationStep_04(void);     				//	히터 물 채우기 및 전력체크
void Ini_OperationStep_05(void);     				//	냉수탱크 물 채우기
void Ini_OperationStep_10(void);     				//	대기
void Ini_OperationStep_15(void);     				//	ON 설정 표시
void Ini_OperationStep_16(void);     				//	냉각 OFF 표시
void Ini_OperationStep_17(void);     				//	무빙 ON/OFF 설정 표시
void Ini_OperationStep_18(void);     				//	OFF 설정 표시
void Ini_OperationStep_20(void);     				//	슬립모드
void Ini_OperationStep_30(void);     				//	UV케어
void Ini_OperationStep_31(void);     				//	정기 UV케어
void Ini_OperationStep_40(void);     				//	전체 잠금
void Ini_OperationStep_41(void);     				//	음성 타입
void Ini_OperationStep_42(void);     				//	관리 모드 설정
void Ini_OperationStep_50(void);     				//	추출 중
void Ini_OperationStep_70(void);     				//	FOTA
void Ini_OperationStep_80(void);     				//	스마트진단
void Ini_OperationStep_90(void);     				//	공장자동화 테스트
void Ini_OperationStep_91(void);     				//	PBA 테스트
void Ini_OperationStep_92(void);     				//	프론트 테스트
void Ini_OperationStep_93(void);     				//	공장 단축 테스트
void Ini_OperationStep_A0(void);     				//	냉각시간 변경 스텝
void Ini_OperationStep_B0(void);     				//	커버 열림
void Ini_OperationStep_C0(void);     				//	냉수탱크 진공 해제
void Ini_OperationStep_D0(void);     				//	TH 확인
void Ini_OperationStep_D1(void);                	//	My용량 변경
void Ini_OperationStep_E0(void);     				//	에러
void Ini_OperationStep_E1(void);     				//	에러 이력
void Ini_OperationStep_F0(void);     				//	살균키트 모드
void Ini_OperationStep_F1(void);     				//	전압, 전류 피드백 확인 모드

void OperationStep_00(void);                 		//	부팅
void OperationStep_01(void);                 		//	초기화 표시
void OperationStep_02(void);                 		//	초기 플러싱 미완료
void OperationStep_03(void);                 		//	초기 플러싱 중
void OperationStep_04(void);                 		//	히터 물 채우기 및 전력체크
void OperationStep_05(void);                 		//	냉수탱크 물 채우기
void OperationStep_10(void);                 		//	대기
void OperationStep_15(void);                 		//	ON 설정 표시
void OperationStep_16(void);                 		//	냉각 OFF 표시
void OperationStep_17(void);                 		//	무빙 ON/OFF 설정 표시
void OperationStep_18(void);                 		//	OFF 설정 표시
void OperationStep_20(void);                 		//	슬립모드
void OperationStep_30(void);                 		//	UV케어
void OperationStep_31(void);                 		//	정기 UV케어
void OperationStep_40(void);                 		//	전체 잠금
void OperationStep_41(void);                 		//	음성 타입
void OperationStep_42(void);                 		//	관리 모드 설정
void OperationStep_50(void);                 		//	추출 중
void OperationStep_70(void);     					//	FOTA
void OperationStep_80(void);     					//	스마트진단
void OperationStep_90(void);     					//	공장자동화 테스트
void OperationStep_91(void);     					//	PBA 테스트
void OperationStep_92(void);     					//	프론트 테스트
void OperationStep_93(void);     					//	공장 단축 테스트
void OperationStep_A0(void);                 		//	냉각PI 타겟온도 변경
void OperationStep_B0(void);                 		//	커버 열림
void OperationStep_C0(void);						//	냉수탱크 진공 해제
void OperationStep_D0(void);                		//	TH 확인
void OperationStep_D1(void);                		//	My용량 변경
void OperationStep_E0(void);                		//	에러
void OperationStep_E1(void);                		//	에러 이력
void OperationStep_F0(void);                		//	살균키트 모드
void OperationStep_F1(void);                		//	전압, 전류 피드백 확인 모드

void OperationStepNoSet(U8 data);

void return_default_cup(void);
void Update_DefaultCupSize_If_Disabled(void);
void Update_CurrentCupSize_If_Disabled(void);

void return_default_hot_temp_setup(void);
//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================
#endif /* _OPERATION_H_ */