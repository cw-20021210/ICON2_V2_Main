/// @file   cooler.h
/// @date   2019.07.10
/// @author PSM
/// @brief  냉각 동작 제어 관련 헤더 file

#ifdef COLD_C
	#define 	EXTERN
	#define 	GLOBAL_VARIABLE_DEFINE
#else
	#define 	EXTERN extern
#endif

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_COLD()			{	/*	1ms 타이머 감산			*/	\
								}
#define	TDEC_10ms_COLD()		{	/*	10ms 타이머 감산		*/	\
									TDEC(gu8_TimerColdEffStep_10ms);	\
									TDEC(gu8_TimerPressureClear_ColdVV_Delay);	\
								}
#define	TDEC_100ms_COLD()		{	/*	100ms 타이머 감산		*/	\
									TDEC(gu8_TimerCoolingStep_100ms);	\
									TDEC(gu8_TimerCoolingStep_100ms_2);	\
									TDEC(gu8_TimerCoolingStep_100ms_3);	\
									TDEC(gu8_TimerCoolingStep_100ms_4);	\
									TDEC(gu8_TimerPressureClear_ColdVV);	\
									TDEC(gu8_TimerDisplayEcoMode);	\
									TDEC(gu8_TimerDisplayCoolingChange);	\
									TDEC(gu8_TimerDisplayFanChange);	\
								}
#define	TDEC_1s_COLD()			{	/*	1s 타이머 감산			*/	\
									TDEC(gu16_TimerCoolingRapid);	\
									TDEC(gu16_TimerCoolingStep_1s);	\
									TDEC(gu8_TimerCooling_PID);	\
									TDEC(gu16_TimerForced_RapidCooling);	\
									TDEC(gu16_Timer_CoolingRapid_12V);	\
								}
#define	TDEC_10s_COLD()			{	/*	10s 타이머 감산			*/	\
								}
#define	TDEC_1min_COLD()		{	/*	1min 타이머 감산		*/	\
									TDEC(gu16_TimerColdEcoMode_24Hour);	\
									TDEC(gu16_TimerColdEcoMode_14Hour);	\
									TDEC(gu16_TimerColdEcoMode_10Hour);	\
									TDEC(gu16_TimerCoolingRapid_6Hour);	/*@018*/\
								}
#define	TDEC_10min_COLD()		{	/*	10min 타이머 감산		*/	\
								}
#define	TDEC_1h_COLD()			{	/*	1h 타이머 감산			*/	\
								}


/*==============================================================================
 Constant Definition
 ==============================================================================*/
enum {
	Z_ColdEffStepNum_00,
	Z_ColdEffStepNum_01,
	Z_ColdEffStepNum_02,
	Z_ColdEffStepNum_10,
	Z_ColdEffStepNum_20,
	Z_ColdEffStepNum_21,
	Z_ColdEffStepNum_22,
	Z_ColdEffStepNum_23,
	Z_ColdEffStepNum_B0,
	Z_ColdEffStepNum_C0,
	Z_ColdEffStepNum_E0,
	Z_ColdEffStepNum_Max
};

#define Z_ColdEffStep_00			(0x00)
#define Z_ColdEffStep_01			(0x01)
#define Z_ColdEffStep_02			(0x02)
#define Z_ColdEffStep_10			(0x10)
#define Z_ColdEffStep_20			(0x20)
#define Z_ColdEffStep_21			(0x21)
#define Z_ColdEffStep_22			(0x22)
#define Z_ColdEffStep_23			(0x23)
#define Z_ColdEffStep_B0			(0xB0)
#define Z_ColdEffStep_C0			(0xC0)
#define Z_ColdEffStep_E0			(0xE0)



enum {
	Z_CoolingStepNum_00,
	Z_CoolingStepNum_10,
	Z_CoolingStepNum_20,
	Z_CoolingStepNum_21,
	Z_CoolingStepNum_22,
	Z_CoolingStepNum_23,
	Z_CoolingStepNum_24,
	Z_CoolingStepNum_30,
	Z_CoolingStepNum_40,
	Z_CoolingStepNum_41,
	Z_CoolingStepNum_E0,
	Z_CoolingStepNum_Max
};

#define Z_CoolingStep_00			(0x00)
#define Z_CoolingStep_10			(0x10)
#define Z_CoolingStep_20			(0x20)
#define Z_CoolingStep_21			(0x21)
#define Z_CoolingStep_22			(0x22)
#define Z_CoolingStep_23			(0x23)
#define Z_CoolingStep_24			(0x24)
#define Z_CoolingStep_30			(0x30)
#define Z_CoolingStep_40			(0x40)
#define Z_CoolingStep_41			(0x41)
#define Z_CoolingStep_E0			(0xE0)

#define COLD_COMPLETE_DEGREE		(Z_TEMP1)		//	냉각 완료 온도
#define RETAIN_JOIN_DEGREE		(Z_TEMP3_5)		//	급속모드 중 유지모드 진입 온도
#define RAPID_COMPLETE_DEGREE		(Z_TEMP6_5)		//	급속모드 완료 온도
//#define RAPID_OPERATE_DEGREE		(Z_TEMP7_5)		//	급속모드 진입 온도
#define RAPID_OPERATE_DEGREE		(Z_TEMP7_2)		//	급속모드 진입 온도
#define	COLD_REOPERATE_DEGREE		(Z_TEMP2_5)		//	냉각 종료 중 냉각 재가동 온도

#define Z_TimerColdEffStep_10ms_100ms	(10)		//	<100ms/10ms>
#define Z_TimerCoolingStep_100ms_3s		(30)		//	<3s/100ms>

//#define Z_TimerCoolingRapid_Out33Degree		(3600)	//	<?s/1s>
//#define Z_TimerCoolingRapid_Out30Degree		(3000)	//	<?s/1s>
//#define Z_TimerCoolingRapid_Out12Degree		(800)	//	<?s/1s>
//#define Z_TimerCoolingRapid_Out6Degree		(200)	//	<?s/1s>
#define Z_TimerCoolingRapid_Out33Degree		(4500)	//	<?s/1s>
#define Z_TimerCoolingRapid_Out30Degree		(3000)	//	<?s/1s>
#define Z_TimerCoolingRapid_Out20Degree		(780)	//	<?s/1s>
#define Z_TimerCoolingRapid_Out12Degree		(180)	//	<?s/1s>
#define Z_TimerCoolingRapid_Out6Degree		(200)	//	<?s/1s>




#define Z_TimerPressureClear_ColdVV	(30)			//	<3s/100ms>
#define Z_TimerPressureClear_ColdVV_Delay	(20)	//	<200ms/10ms>

#define Z_TimerCoolingRapid_6Hour	(360)			//	<360h/1h>

#define Z_TimerCoolingStep_1s_600s	(600)			//	<600s/1s>

#define Z_TimerColdEcoMode_24Hour	(1440)			//	24시간 <1440min/1min>
#define Z_TimerColdEcoMode_14Hour	(840)			//	14시간 <840min/1min>
#define Z_TimerColdEcoMode_10Hour	(600)			//	10시간 <600min/1min>

#define Z_TimerDisplayEcoMode		(30)			//@016
#define Z_TimerDisplayCoolingChange	(30)           	//@016
#define Z_TimerDisplayFanChange		(30)           	//@016

#define Z_TimerCooling_PID 			(120)			//	단위 초
#define Z_TIMER_COOLINGRAPID_12V	(300)

/*==============================================================================
 Variable Definition
 ==============================================================================*/
EXTERN U8 gu8_ColdEffStep;							//
	#define gu8_ColdEffStep_hi	(gu8_ColdEffStep & 0xf0)
	#define gu8_ColdEffStep_lo	(gu8_ColdEffStep & 0x0f)

EXTERN U8 gu8_CoolingStep;
	#define gu8_CoolingStep_hi	(gu8_CoolingStep & 0xf0)
	#define gu8_CoolingStep_lo	(gu8_CoolingStep & 0x0f)

EXTERN U8 gu8_CoolingStep_Before;

EXTERN U8 gu8_ColdEffStepNum;						//
EXTERN U8 gu8_ColdEffStepNum_Before;				//	냉각스텝 Num Before

EXTERN U8 gu8_CoolingStepNum;						//
EXTERN U8 gu8_CoolingStepNum_Before;

#ifdef _FOR_DEVELOP_
EXTERN U16 gu16_ColdTimeData;						//	냉각 시견 변경
#endif

EXTERN U8 gu8_TimerColdEffStep_10ms;				//	<?/10ms>

EXTERN U16 gu16_TimerCoolingRapid;					//	급속모드 추가냉각 시간 <?/1s>
EXTERN U8 F_CoolingRapid_Complete;					//	급속모드 완료 온도 감지 플래그

EXTERN U8 gu8_TimerCoolingStep_100ms;				//	<?/100ms>
EXTERN U8 gu8_TimerCoolingStep_100ms_2;				//	<?/100ms>
EXTERN U8 gu8_TimerCoolingStep_100ms_3;				//	<?/100ms>
EXTERN U8 gu8_TimerCoolingStep_100ms_4;				//	<?/100ms>

EXTERN U16 gu16_TimerCoolingStep_1s;				//	<?/1s>

EXTERN U8 gu8_TimerPressureClear_ColdVV;			//	잔압제거 동작 타이머 <3s/100ms>
EXTERN U8 gu8_TimerPressureClear_ColdVV_Delay;		//	잔압제거 NOS밸브 선CLOSE 후 냉수밸브OPEN 딜레이 타임 <200ms/10ms>

EXTERN U16 gu16_TimerCoolingRapid_6Hour;			//	<360min/1min>
EXTERN U16 gu16_TimerColdEcoMode_24Hour;			//	24시간 <1440min/1min>
EXTERN U16 gu16_TimerColdEcoMode_14Hour;			//	14시간 <840min/1min>
EXTERN U16 gu16_TimerColdEcoMode_10Hour;			//	10시간 <600min/1min>

EXTERN U8 F_CoolingEcoMode_Disable;					//@011

EXTERN U8 gu8_TimerDisplayEcoMode;					//@016
EXTERN U8 gu8_TimerDisplayCoolingChange;            //@016
EXTERN U8 gu8_TimerDisplayFanChange;                //@016

EXTERN U8 F_ColdRetainMode_VoltMin;					//@017	냉각 유지모드에서 냉수온도 3도 미만 감지 시 최저전압 가동

EXTERN I16 gi16_TEM_Volt_P;							//@022	냉각 P제어 <1/10V>
EXTERN I16 gi16_TEM_Volt_I;                         //@022	냉각 I제어 <1/10V>
EXTERN I16 gi16_TEM_Volt_P_I;                       //@022	P+I <1/10V>
EXTERN U16 gu16_TEM_Volt;                           //@022	최종 제어 값  <1/10V>
EXTERN U16 gu16_Cooling_Dp1;                        //@022
EXTERN U16 gu16_Cooling_Dp2;                        //@022
EXTERN U16 gu16_Cooling_Di1;                        //@022

EXTERN U8 gu8_TimerCooling_PID;						//@022	<?s/1s>

EXTERN U16 gu16_Cooling_TargetDegree;               //@022	냉각 PI제어 타겟 온도	<1/10℃>
EXTERN U8 gu8_Cooling_TargetDegree_forEEPROM;		//@022
EXTERN U16 gu16_ColdSensor_DEGREE_Backup;           //@022
EXTERN U16 gu16_OutDoorSensor_DEGREE_Backup;        //@022
EXTERN U16 gu16_HeatSinkSensor_DEGREE_Backup;       //@022

EXTERN U16 gu16_TimerForced_RapidCooling;			//@022	냉수 추출 후 유지모드에서 강제 급속모드 전압 동작 타이머	<?s/1s>
EXTERN U16 gu16_CountColdEff_Sec;					//@022	냉수 추출 시간 카운트 <1s>

EXTERN U16 gu16_Timer_CoolingRapid_12V;				//	급속모드 진입 시, 12V 5분 출력 타이머
/*==============================================================================
 function Definition
 ==============================================================================*/
void Cold_Initialize(void);
void ColdControl(void);

//=============================================================================
#undef 	EXTERN
#undef 	GLOBAL_VARIABLE_DEFINE
//=============================================================================

