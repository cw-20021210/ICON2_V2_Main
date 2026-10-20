/// @file   watermeter.h
/// @date   2019.07.10
/// @author PSM
/// @brief  유량센서 처리 관련 헤더 file

#ifdef FLOWMETER_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_FLOWMETER()	{	/*	1ms 타이머 감산			*/	\
								}
#define	TDEC_10ms_FLOWMETER()	{	/*	10ms 타이머 감산		*/	\
									TDEC(gu8_Timer_FlowSafety);	\
								}
#define	TDEC_100ms_FLOWMETER()	{	/*	100ms 타이머 감산		*/	\
									TDEC(gu16_TimerEffluent);	\
								}
#define	TDEC_1s_FLOWMETER()		{	/*	1s 타이머 감산			*/	\
								}
#define	TDEC_10s_FLOWMETER()	{	/*	10s 타이머 감산			*/	\
								}
#define	TDEC_1min_FLOWMETER()	{	/*	1min 타이머 감산		*/	\
									TDEC(gu8_TimerColdEff_FirstCup);	\
								}
#define	TDEC_10min_FLOWMETER()	{	/*	10min 타이머 감산		*/	\
								}
#define	TDEC_1h_FLOWMETER()		{	/*	1h 타이머 감산			*/	\
								}


/*==============================================================================
 Constant Definition
 ==============================================================================*/
#define Z_HOLESENSOR_NUM			(4)				//	8Pulse by 1Roatation (절반인 4Pulse)

#ifdef _DPP_FLOWMETER_
//#define Z_FlowMeter_Standard		(974)			//	초소형 센서
#define Z_FlowMeter_Standard_KOR		(1100)			//	초소형 센서		DPP 보정
//#define Z_FlowMeter_Standard		(1620)			//	초소형 센서		초소형 유량증대
//#define Z_FlowMeter_Standard		(1068)			//	초소형 센서	1/156/60 (156은 1LPM에 156Hz)	DPP 보정
#else
//#define Z_FlowMeter_Standard		(2874)			//	기존 유량센서
//#define Z_FlowMeter_Standard		(4166)			//	다빈치 유량센서 개선
//@030 #define Z_FlowMeter_Revise			(300)			//	커질수록 추출양 적어짐
//#define Z_FlowMeter_Revise			(117)			//@021	커질수록 추출양 적어짐
#define Z_FlowMeter_Revise			(117)			//@030(초기플러싱 추출량 적음으로 변경)	커질수록 추출양 적어짐
#define Z_FlowMeter_Standard_KOR	(3584+Z_FlowMeter_Revise)		//	다빈치 유량센서 개선 1LPM=46.5Hz => 1/46.5/60s
#define Z_FlowMeter_Standard_forHotWater_KOR	(3703+Z_FlowMeter_Revise)			//	다빈치 유량센서 개선	0.5LPM=22.5Hz => 0.5/22.5/60s. 실제는 다른 데이터라 보정하여 3703

#define Z_FlowMeter_Revise_JP		(30)			//	커질수록 추출양 적어짐
//#define Z_FlowMeter_Revise_JP		(90)			//@021	커질수록 추출양 적어짐
#define Z_FlowMeter_Standard_JP		(1984)			//	저유량 센서
#define Z_FlowMeter_Standard_forHotWater_JP	(2272)	//	저유량 센서

//#ifdef _FOR_JAPAN_90DEGREE_CONTINUE
//#define Z_FlowMeter_Standard_forHotWater_forJapan	(5568+Z_FlowMeter_Revise)			//
//#else
//#define Z_FlowMeter_Standard_forHotWater_forJapan	(4461+Z_FlowMeter_Revise)			//	다빈치 유량센서 개선	0.25=10.5Hz => 0.25/10.5/60s
//#endif
#endif

//#define Z_TimerFlushing				(3000)			//	<5min/100ms>

#define Z_TimerColdEff_FirstCup		(30)			//	냉수 30분 주기 첫잔 <30min/1min>

/*==============================================================================
 Variable Definition
 ==============================================================================*/
EXTERN volatile U8 F_FlowCheck;						//	유량계산 (타이머 기능 사용)
EXTERN U8 gu8_Timer_FlowSafety;						//	유량 안전타이머 (일정시간 이상 노이즈 및 클리어 처리)
EXTERN volatile U16 gu16_FlowMeterTCR_Now;			//	TCR 현재치
EXTERN U16 gu16_FlowMeterTCR_Before;				//	TCR 이전치
EXTERN U32 gu32_FlowMeterTCR_Sum;					//	TCR 합산 데이터
EXTERN U8 gu8_Counter_FlowMeterTCR;					//	TCR 누적 카운터
EXTERN U16 gu16_FlowMeterTCR_Array[Z_HOLESENSOR_NUM];		//	TCR 누적 배열
EXTERN U16 gu16_FlowDecide;							//	TCR 평균
EXTERN U16 gu16_Spec_FlowMeter;

EXTERN U8 gu8_TimerColdEff_FirstCup;				//	냉수 30분 주기 첫잔 <30min/1min>

EXTERN U8 gu8_FlowMeter_NumMax;


/*==============================================================================
 function Definition
 ==============================================================================*/
void FlowMeter_Initialize(void);
void FlowMeter_INT(void);
void FlowMeter_Decide(void);
void Effluent_Hz_Decide(void);
void FlowAmount_ForMonitor(void);

//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================

