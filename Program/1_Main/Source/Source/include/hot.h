/// @file   watermeter.h
/// @date   2019.07.10
/// @author PSM
/// @brief  유량센서 처리 관련 헤더 file

#ifdef HOT_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_HOT()			{	/*	1ms 타이머 감산			*/	\
								}
#define	TDEC_10ms_HOT()			{	/*	10ms 타이머 감산		*/	\
									TDEC(gu16_TimerHotStep_10ms);	\
									TDEC(gu8_TimerFirstHot_HeaterSensing);	\
									TDEC(gu8_TimerHeaterCheck);	\
									TDEC(gu8_Timer_High_Flow_w);	\
								}
#define	TDEC_100ms_HOT()		{	/*	100ms 타이머 감산		*/	\
									TDEC(gu8_TimerHotSafety);	\
									TDEC(gu8_TimerFlowControl_AfterEff);	\
									TDEC(gu8_TimerHeaterControl_AfterEff);	\
									TDEC(gu8_TimerHeaterSafety_AfterEff);	\
									TDEC(gu8_TimerFlowCheck_AfterEff);		\
									TDEC(gu8_TimerTargetFlow_Change_AfterEff);	\
									/*TDEC(gu8_Timer_ReCalibration);*/	\
									TDEC(gu8_TimerHotStep_100ms);		\
									TDEC(gu8_TimerHotTest_DegreeDisplay);	\
									TDEC(gu8_TimerMustHeating);	\
									/*TDEC(gu8_TimerHotEff_CautionVoice);*/	\
									TDEC(gu8_TimerVoiceOutput_HotCaution);	\
									TDEC(gu8_TimerHeaterLimit);	\
									TDEC(gu8_TimerHeaterLimitCheck);	\
									TDEC(gu8_TimerTargetFlow_Decrease);	\
								}
#define	TDEC_1s_HOT()			{	/*	1s 타이머 감산			*/	\
								}
#define	TDEC_10s_HOT()			{	/*	10s 타이머 감산			*/	\
								}
#define	TDEC_1min_HOT()			{	/*	1min 타이머 감산		*/	\
								}
#define	TDEC_10min_HOT()		{	/*	10min 타이머 감산		*/	\
								}
#define	TDEC_1h_HOT()			{	/*	1h 타이머 감산			*/	\
								}


/*==============================================================================
 Constant Definition
 ==============================================================================*/
 
 typedef enum
{
    HOT_TEMP_SETUP_KOR45_JPX = 1,
	HOT_TEMP_SETUP_KOR__50,
	HOT_TEMP_SETUP_KOR__55,
	HOT_TEMP_SETUP_KOR__60,
	HOT_TEMP_SETUP_KOR__65,
	HOT_TEMP_SETUP_KOR70_JP70,
	HOT_TEMP_SETUP_KOR__75,
	HOT_TEMP_SETUP_KOR__80,
	HOT_TEMP_SETUP_KOR85_JP80,
	HOT_TEMP_SETUP_KOR__90,
	HOT_TEMP_SETUP_KOR__95,
	HOT_TEMP_SETUP_KOR100_JP90,
	HOT_TEMP_SETUP_MY_HOT_TEMP,
}HOT_TEMP_LEVEL;
 
 
#ifdef _HOT_TEMP_4STEP_
//#define HOT_TEMP_SETUP_KOR100_JP90   	(0)			//
//#define HOT_TEMP_SETUP_KOR85_JP80    	(1)			//
//#define HOT_TEMP_SETUP_KOR70_JP70    	(2)			//
//#define HOT_TEMP_SETUP_KOR45_JPX    	(3)			//	일본향 없음
#else
//#define HOT_TEMP_SETUP_KOR45_JPX    	(2)
#define HOT_TEMP_SETUP_KOR70_JP70    	(1)
#define HOT_TEMP_SETUP_KOR100_JP90   	(0)
#endif

#define HEATER_FULL_WATT_KOR		(2700)				//	히터 FULL와트 이론 데이터
#define HEATER_RELAY_WATT_KOR		(1500)				//	히터 Relay와트 이론 데이터
#define HEATER_RELAY_Percent_KOR	(((U32)HEATER_RELAY_WATT_KOR*100)/HEATER_FULL_WATT_KOR)	//	Relay와 TRIAC %

#define HEATER_FULL_WATT_JP			(1400)				//	히터 FULL와트 이론 데이터
#define HEATER_RELAY_WATT_JP		(700)				//	히터 Relay와트 이론 데이터
#define HEATER_RELAY_Percent_JP		(((U32)HEATER_RELAY_WATT_JP*100)/HEATER_FULL_WATT_JP)	//	Relay와 TRIAC %
//#define HEATER_PWM_WATT			(HEATER_FULL_WATT_KOR-HEATER_RELAY_WATT_KOR)	//	히터 TRIAC와트 이론 데이터

enum {
	Z_HotStepNum_00,
	Z_HotStepNum_01,
	Z_HotStepNum_02,
	Z_HotStepNum_10,
	Z_HotStepNum_20,
	Z_HotStepNum_2F,
	Z_HotStepNum_31,
	Z_HotStepNum_32,
	Z_HotStepNum_33,
	Z_HotStepNum_34,
	Z_HotStepNum_35,
	Z_HotStepNum_36,
//	Z_HotStepNum_3E,
	Z_HotStepNum_Max
};

#define Z_HotStep_00			(0x00)
#define Z_HotStep_01			(0x01)
#define Z_HotStep_02			(0x02)
#define Z_HotStep_10			(0x10)
#define Z_HotStep_20			(0x20)
#define Z_HotStep_2F			(0x2F)
#define Z_HotStep_31			(0x31)
#define Z_HotStep_32			(0x32)
#define Z_HotStep_33			(0x33)
#define Z_HotStep_34			(0x34)
#define Z_HotStep_35			(0x35)
#define Z_HotStep_36			(0x36)
//#define Z_HotStep_3E			(0x3E)

#define Z_TimerHotSafety				(100)		//	<?s/100ms>
#define Z_TimerHotSafety_JP				(250)		//	<?s/100ms>

#define Z_TimerHotStep_10ms_100ms		(10)		//	<100ms/10ms>
#define Z_TimerHotStep_10ms_200ms		(20)		//	<200ms/10ms>
#define Z_TimerHotStep_10ms_500ms		(50)		//	<500ms/10ms>
#define Z_TimerHotStep_10ms_600ms		(60)		//	<600ms/10ms>

#define Z_TimerHotStep_100ms_2s			(20)		//	<2s/100ms>

#define Z_TimerFlowControl_AfterEff		(10)		//	<1s/100ms>
#define Z_TimerHeaterControl_AfterEff	(10)		//	<1000ms/100ms>
#define Z_TimerHeaterSafety_AfterEff	(7)			//	<700ms/100ms>

#define Z_TimerFlowCheck_AfterEff		(7)			//	<700ms/100ms>
#define Z_TimerFlowCheck_AfterEff_forJapan	(15)	//@002	일본향 저유량 구간에서는 Hz가 늦게 결정됨 <?s/100ms>
#define Z_TimerTargetFlow_Change_AfterEff	(30)	//	<3s/100ms>

#define Z_TimerHeaterTest				(150)		//	<1500ms/10ms>

#define Z_HEATER_ON_TIME 				(50)

#define	Z_Heater_On_Time_Stand			(600)		//	<600s/1s>
#define Z_Timer_OverHeating_Wait		(300)		//	<300s/1s>

#define Z_TimerFirstHot_HeaterSensing	(70)		//	<700ms/10ms>

#define Z_TimerHeaterCheck				(200)		//	<2000ms/10ms>

#define Z_TimerHotTest_DegreeDisplay	(30)		//	<3s/100ms>
#define	Z_TimerHotEff_CautionVoice		(200)		//	<20s/100ms>

#define Z_TimerVoiceOutput_HotCaution	(80)		//	<8s/100ms>
//20260310
//#define Z_TimerHeaterLimit				(40)		//	<4s/100ms>
#define Z_TimerHeaterLimit				(45)		//	<4s/100ms>
#define Z_TimerHeaterLimitCheck			(40)		//	추출 ?s 이내 히터 온도 급상승 체크하여 Limit

#define Z_TimerTargetFlow_Decrease_Start	(60)	//@007	추출 시작부터 온도 체크까지의 지연시간		<6s/100ms>
#define Z_TimerTargetFlow_Decrease_Ing	(20)		//@007	상기 지연시간 경과 부터 x초 주기로 온도체크		<2s/100ms>


#define Z_SELECT_TEMP_MAX				(5)

#define Z_HOT_TEMP_ASCENDING			(0)
#define Z_HOT_TEMP_DESCENDING			(1)

/*==============================================================================
 Variable Definition
 ==============================================================================*/
 #ifdef _EFF_Continue_Heater_OFF
extern U8 F_Eff_Continue_Heater_Off;
#endif
EXTERN U8 gu8_HotStepNum;							//	온수 스텝 Num

EXTERN U8 F_MilkEff_MoreHot;						//	분유 추출 시 히터 내부 온도가 너무 높은 경우 미가열

EXTERN U8 gu8_TimerHotSafety;						//	예열 최대시간 타이머	<?s/100ms>
EXTERN U16 gu16_TimerHotStep_10ms;					//	스텝 관련 타이머 <?ms/10ms>
EXTERN U8 gu8_TimerHotStep_100ms;					//	스텝 관련 타이머	<2s/100ms>

EXTERN U8 gu8_TimerFlowControl_AfterEff;			//	추출 시작 후 유량제어 타이머
EXTERN U8 gu8_TimerHeaterControl_AfterEff;			//	추출 시작 후 히터제어 타이머
EXTERN U8 gu8_TimerHeaterSafety_AfterEff;			//	추출 시작 후 히터 안전 타이머
EXTERN U8 gu8_TimerFlowCheck_AfterEff;				//	추출 시작 후 유량체크 타이머
EXTERN U8 gu8_TimerTargetFlow_Change_AfterEff;		//	추출 시작후 목표유량 변경 타이머 set

EXTERN U16 gu16_WATT_forTest;
EXTERN I16 gi16_FlowCheck;							//	입수압에 따라 감산 보정 (스텝by유량으로 체크)
EXTERN I16 gi16_HotIn_DegreeCheck;					//	온수입수온도에 따라 보정

EXTERN U16 gu16_TargetDegree;						//	온수 목표온도

EXTERN I16 gi16_HotInSensor_BackUp_forFlowDecide;	//	추출 전 유량 결정 후 입수 온도 ?도 이상 변화하면 목표유량 재설정

EXTERN U16 gu16_HeatingTime;

EXTERN U8 gu8_TimerFirstHot_HeaterSensing;			//	<500ms/10ms>

EXTERN I16 gi16_HeaterSensor_Before[3];

EXTERN U8 gu8_TimerHeaterCheck;						//	<500ms/10ms>

EXTERN U8 gu8_TimerHotTest_DegreeDisplay;			//	<3s/100ms>

EXTERN U8 F_FirstHotEff_Detect;						//	첫 째잔 감지
EXTERN U8 F_2ndHot_Effluent;						//	두번째 잔
EXTERN U8 F_2ndHot_Effluent_HeaterDown;				//	두번째 잔
EXTERN U8 F_LatentHeat_Clear;						//	잠열제거 시행여부
//EXTERN U8 F_MorePreHeating_forJapan;				//	두번째잔 더 가열 (일본향)

EXTERN U8 gu8_TimerMustHeating;						//	Must 히팅 타임 <?s/100ms>

EXTERN U16 gu16_HeaterOff_Hz;						//	잠열제거 시작 잔여Hz
EXTERN U8 F_PreHeatingVoice_forJapan;				//	(for일본향) 예열시간이 길 경우 음성맨트 출력 플래그
//EXTERN U8 gu8_TimerHotEff_CautionVoice;				//	온수 85도 이상 추출 주의하세요 음성 출력 주기타이머<10s/100ms>

EXTERN U8 gu8_TimerVoiceOutput_HotCaution;			//	이전 음성 종료 후 3초 뒤에 "고온수가 추출됩니다. 컵을 가까이해주세요" 음성이 약 5초 걸림 <8s/100ms>
EXTERN U8 F_VoiceOutput_HotCaution;

EXTERN I16 gi16_HeaterSensor_Before_forEff;

EXTERN U8 gu8_TimerHeaterLimitCheck;				//	<?s/100ms>	추출 ?s 이내 히터 온도 급상승 체크하여 Limit
EXTERN U8 gu8_TimerHeaterLimit;						//	<4s/100ms>
EXTERN U8 F_HeaterLimit;
EXTERN U8 F_Heater_Decrease;	

//cbr_hot_pro
EXTERN U8 gu8_Timer_High_Flow_w;
EXTERN U8 gu8_F_RelayOff_Delay;
EXTERN U8 gu8_F_Hot80_FirstEffluent_correct;
 //-------------------------------------------20260201--------------------------
EXTERN HOT_TEMP_LEVEL selected_hot_temp[Z_SELECT_TEMP_MAX];			//cbr_test
EXTERN U8 gu8_select_hot_temp_total_count;				//	app에서 설정한 온수 최대 갯수
EXTERN TYPE_WORD gu8_hot_temp_data_for_wifi;
EXTERN U8 gu8_hot_temp_default_data_for_wifi;
EXTERN U8 gu8_f_hot_temp_sorting;
//------------------------------------------------------------------------------

//@007	시작
EXTERN U8 gu8_TimerTargetFlow_Decrease;
EXTERN U8 gu8_CountDegree_Average;
EXTERN I16 gi16_HotOutSensor_DegreeSum;
EXTERN I16 gi16_HeaterSensor_DegreeSum;
EXTERN I16 gi16_HotOutSensor_DegreeAverage;
EXTERN I16 gi16_HeaterSensor_DegreeAverage;
EXTERN U8 gu8_FlowTarget_Decrease;
//@007	끝

/*==============================================================================
 function Definition
 ==============================================================================*/
void Hot_Initialize(void);							//	초기설정
void HotControl(void);								//	온수스텝 처리

U16 WATT_Ini(void);									//	출수 시 1초간 초기 WATT
U16 WATT_forTest(void);
U16 StartFlow_Decide(void);							//	유량 결정
void StartFlow_StepMotorControl(void);				//	초기유량에 따른 스텝모터 조기이동

void HotTemp_Setting(void);
void Update_DefaultHotTemp_If_Disabled(void);
void Update_CurrentHotTemp_If_Disabled(void);
//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================
