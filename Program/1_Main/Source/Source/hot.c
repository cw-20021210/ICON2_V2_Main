/// @file     heater.c
/// @date     2019.07.10
/// @author   PSM
/// @brief    온수 제어 관련 file
#define HOT_C
#include "include/_include.h"
#include "operation.h"

static void Ini_HotStep_00(void);					//	부팅
static void Ini_HotStep_01(void);					//	온수 첫잔 대기
static void Ini_HotStep_02(void);					//	온수 첫잔 추출 중
static void Ini_HotStep_10(void);					//	온수 미선택
static void Ini_HotStep_20(void);					//	온수 선택 대기
static void Ini_HotStep_2F(void);					//	스테핑 초기화 대기
static void Ini_HotStep_31(void);					//	(미사용)목표유량 결정 및 스테핑모터 무빙 대기
static void Ini_HotStep_32(void);					//	온수 선택 중 예열 (일본향)
static void Ini_HotStep_33(void);					//	온수 예열
static void Ini_HotStep_34(void);					//	온수 추출 전 추출밸브 먼저 OPEN
static void Ini_HotStep_35(void);					//	온수 추출 중
static void Ini_HotStep_36(void);					//	온수 추출 정지 시에 압 빼는 스텝

static void HotStep_00(void);						//	부팅
static void HotStep_01(void);						//	온수 첫잔 대기
static void HotStep_02(void);						//	온수 첫잔 추출 중
static void HotStep_10(void);						//	온수 미선택
static void HotStep_20(void);						//	온수 선택 대기
static void HotStep_2F(void);						//	스테핑 초기화 대기
static void HotStep_31(void);						//	(미사용)목표유량 결정 및 스테핑모터 무빙 대기
static void HotStep_32(void);						//	온수 선택 중 예열 (일본향)
static void HotStep_33(void);						//	온수 예열
static void HotStep_34(void);						//	온수 추출 전 추출밸브 먼저 OPEN
static void HotStep_35(void);						//	온수 추출 중
static void HotStep_36(void);						//	온수 추출 정지 시에 압 빼는 스텝

static void HotStepNoSet(U8 data);

static void BeforeEff_SteppingMotor_Check(void);

static void Flow_Rapidly_Decrease_Check(void);		//	유량 급격 감소 체크 (스팀 방지)
static void HotTemp100_FlowTargetDecrease_Check(void);	//@007	100도 유량감소 필요 체크

static void WATT_Calculation(void);					//	WATT 결정
static I32 WATT_Calculation_P(void);				//	WATT P제어 결정
static I32 WATT_Calculation_I(void);				//	WATT I제어 결정

#define Z_Flow_Rapidly_Decrease	(20)				//	0.2LPM

#define HEATER_EFFICIENCY 		(98)				//	히터효율 %

/*	Delta 20도부터 비례제어 	*/
#define Z_Di1_MILK					(600)			/*	WATT I성분 산출용 정수(i1)	[1/1000]*/	//	국내향 45도 전용
#define Z_Di1						(1000)			/*	WATT I성분 산출용 정수(i1)	[1/1000]*/

#define Z_Dp1_MILK					(20)			/*	WATT P성분 산출용 정수(p1)	[1/1000]*/	//	국내향 45도 전용
#define Z_Dp2_MILK					(80)			/*	WATT P성분 제한용 정수(p2)	[1/1000]*/	//	국내향 45도 전용
#define Z_Dp1						(30)			/*	WATT P성분 산출용 정수(p1)	[1/1000]*/
#define Z_Dp2						(150)			/*	WATT P성분 제한용 정수(p2)	[1/1000]*/

#define Z_PI_Control_LimitTemp		(Z_TEMP30)		//	30도 이상은 30도로 고정

#define Z_StepDown_forStepByFlowCheck_KOR	(1200)	//	3000스텝에서 1200스텝을 빼고 1800스텝에서 유량이 얼마나 들어오는지 체크 (※ 2100이 가장 많아 사용하는 구간)
#define Z_StepDown_forStepByFlowCheck_JP	(2000)	//

#define Z_PreHeating_Degree_forJAPAN	(Z_TEMP50)	//	온수선택 시 예열 목표 온도

Hot_StepTable Hot_Step[Z_HotStepNum_Max] =
{
	//	스텝 No		온수 입수 밸브		온수 출수 밸브		히터 릴레이		히터 PWM			Step Func.
	{Z_HotStep_00,	VALVE_OFF,		VALVE_OFF,		HEATER_OFF,		HEATER_OFF,		HotStep_00},	//	부팅
	{Z_HotStep_01,	VALVE_OFF,		VALVE_OFF,		HEATER_OFF,		HEATER_OFF,		HotStep_01},	//	온수 첫잔 대기
	{Z_HotStep_02,	VALVE_ON,		VALVE_ON,		HEATER_FIRST,		HEATER_FIRST,		HotStep_02},	//	온수 첫잔 추출 중
	{Z_HotStep_10,	VALVE_OFF,		VALVE_OFF,		HEATER_OFF,		HEATER_OFF,		HotStep_10},	//	온수 미선택
	{Z_HotStep_20,	VALVE_OFF,		VALVE_OFF,		HEATER_OFF,		HEATER_OFF,		HotStep_20},	//	온수 선택 대기
	{Z_HotStep_2F,	VALVE_OFF,		VALVE_OFF,		HEATER_OFF,		HEATER_OFF,		HotStep_2F},	//	스테피모터 영점 대기
	{Z_HotStep_31,	VALVE_OFF,		VALVE_OFF,		HEATER_OFF,		HEATER_OFF,		HotStep_31},	//	(미사용)목표유량 결정 및 스테핑모터 무빙 대기
	{Z_HotStep_32,	VALVE_OFF,		VALVE_OFF,		HEATER_OFF,		HEATER_ON,		HotStep_32},	//	온수 선택 중 예열 (일본향)
	{Z_HotStep_33,	VALVE_OFF,		VALVE_OFF,		HEATER_ON,		HEATER_PRE,		HotStep_33},	//	온수 예열 중
	{Z_HotStep_34,	VALVE_OFF,		VALVE_ON,		HEATER_ON,		HEATER_ON,		HotStep_34},	//	온수 추출 전 추출밸브 먼저 OPEN
	{Z_HotStep_35,	VALVE_ON,		VALVE_ON,		HEATER_ON,		HEATER_PWM,		HotStep_35},	//	온수 추출 중
	{Z_HotStep_36,	VALVE_OFF,		VALVE_ON,		HEATER_OFF,		HEATER_OFF,		HotStep_36},	//	온수 추출 정지 시에 압 빼는 스텝
};

const U8 hot_temp_setting_table[12] = 
{
	{HOT_TEMP_SETUP_KOR45_JPX},
	{HOT_TEMP_SETUP_KOR__50},
	{HOT_TEMP_SETUP_KOR__55},
	{HOT_TEMP_SETUP_KOR__60},
	{HOT_TEMP_SETUP_KOR__65},
	{HOT_TEMP_SETUP_KOR70_JP70},
	{HOT_TEMP_SETUP_KOR__75},
	{HOT_TEMP_SETUP_KOR__80},
	{HOT_TEMP_SETUP_KOR85_JP80},
	{HOT_TEMP_SETUP_KOR__90},
	{HOT_TEMP_SETUP_KOR__95},
	{HOT_TEMP_SETUP_KOR100_JP90},
};



/// @brief    Hot & Hot 동작 관련 Flag & 변수 초기화
/// @param    void
/// @return   void
void Hot_Initialize(void)
{
	F_FirstHotEffluent = CLEAR;           			// 	부팅후 온수 첫잔 Flag Set
	
	
	gu16_Heater_Di1 = Z_Di1;
	gu16_Heater_Dp1 = Z_Dp1;
	gu16_Heater_Dp2 = Z_Dp2;

//	PSM_Dp1 = Z_Dp1;
//	PSM_Dp2 = Z_Dp2;

//	gu16_FlowDecide_W_HotEffMax = 90;				//	0.9LPM

	Ini_HotStep_00();
}

/// @brief
/// @param    void
/// @return   void
void HotControl(void)
{
	U16 mu16_EffStart_Temp = 0;
	U32 temp1;
	I16 temp2, temp3, temp4, temp5;
	I16 mi16_Buf = 0;

//#ifdef _HOT_TEST_
//	F_HotWater_100Degree = SET;
//#endif

	if (gu16_StepCount_BeforeEff != 0)
	{
		gi16_FlowCheck = (I16)(((U32)gu16_StepCount_BeforeEff*100)/(gu16_FlowDecide_W_BeforeEff*35)/5)-23;	//	입수압에 따라 감산(-) 보정 (스텝by유량으로 체크) (※ 실험치)
	}
	gi16_HotIn_DegreeCheck = (Z_TEMP25-gi16_HotInSensor.DEGREE)/10;	//	온수입수온도에 따라 DI 보정 (※ 실험치)
	if (gi16_HotIn_DegreeCheck < -10)				//	-10% 미만인 경우 (※ 실험치)
	{
		gi16_HotIn_DegreeCheck = -10;				//		-10%로 제한 (※ 실험치)
	}

	if (gi16_HotInSensor.DEGREE < Z_TEMP25)			//	국내향 100/85도 추출시작온도 보정
	{
		mi16_Buf = ((I16)Z_TEMP25 - gi16_HotInSensor.DEGREE)/3;
	}
	else
	{
		mi16_Buf = 0;
	}

	if ((F_LatentHeat_Clear == SET) && (gi16_HeaterSensor.DEGREE <= Z_TEMP80))
	{
		F_LatentHeat_Clear = CLEAR;
	}

	if (gu8_HotStepNum < Z_HotStepNum_Max)			//	스텝 허용 범위
	{
//@020		gu16_HeaterWATT_forCal = WATT_Ini();		//@008

		if (gu8_ModelType_forNational == MODEL_KOREA)	//	국내향
		{
			switch (gu8_HotTempSetup)				//	설정온도별 데이터 설정
			{
#ifndef _HOT_TEMP_4STEP_
				case HOT_TEMP_SETUP_KOR100_JP90:
				default:
					/*	목표온도 결정	*/
					//mu16_EffStart_Temp = Z_TEMP65;	//JHKO_260310
					mu16_EffStart_Temp = Z_TEMP80;	//
					gu16_TargetDegree = Z_TEMP90;	//
					gu16_FlowTarget_W = StartFlow_Decide();
					/*	추출시작 온도 결정 : 기준온도 30도 ± 히터전력 */
					gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/10);	//	10와트당 1도 증가 또는 감소
					/*	I값 초기치 계산 : (초기전류치-x) / (최대전력) + (히터전력) - 스텝바이유량	+ 입수온도	*/
					gu16_HeaterWATT_forCal = WATT_Ini();
					gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-100) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
					gu16_Heater_Dp1 = Z_Dp1;
					gu16_Heater_Dp2 = Z_Dp2;
					break;
				case HOT_TEMP_SETUP_KOR70_JP70:				//		차
					/*	목표온도 결정	*/
					mu16_EffStart_Temp = Z_TEMP55;	//
					gu16_TargetDegree = Z_TEMP74;		//			목표온도
					gu16_FlowTarget_W = StartFlow_Decide();
					/*	추출시작 온도 결정 : 기준온도 30도 ± 히터전력 */
					gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/10);	//	10와트당 1도 증가 또는 감소
					/*	I값 초기치 계산 : (초기전류치-x) / (최대전력) + (히터전력) - 스텝바이유량	+ 입수온도	*/
					gu16_HeaterWATT_forCal = WATT_Ini();
					gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-200) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
					gu16_Heater_Dp1 = Z_Dp1;
					gu16_Heater_Dp2 = Z_Dp2;
					break;
				case HOT_TEMP_SETUP_KOR45_JPX:				//		분유
					/*	목표온도 결정	*/
					mu16_EffStart_Temp = Z_TEMP35;
					gu16_TargetDegree = Z_TEMP46;
					gu16_FlowTarget_W = StartFlow_Decide();
					/*	추출시작 온도 결정 : 기준온도 30도 ± 히터전력 */
					gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/10);	//	10와트당 1도 증가 또는 감소
					/*	I값 초기치 계산 : (초기전류치-x) / (최대전력-릴레이전력) + (히터전력) - 스텝바이유량	+ 입수온도	*/
					if (gi16_HeaterSensor.DEGREE > (I16)(gu16_TargetDegree+Z_TEMP10))
					{
						gi16_Heater_Di_ini = 0;
					}
					else
					{
						gu16_HeaterWATT_forCal = WATT_Ini();
						gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-100) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
						
					}
					gu16_Heater_Dp1 = Z_Dp1_MILK;
					gu16_Heater_Dp2 = Z_Dp2_MILK;
					break;
#else
				case HOT_TEMP_SETUP_KOR100_JP90:		//	국내향 100도
				case HOT_TEMP_SETUP_KOR__95:
                		case HOT_TEMP_SETUP_KOR__90:
				default:
					 // mu16_EffStart_Temp = Z_TEMP84 + mi16_Buf;
					 
					 /*
                    if(gi16_HotInSensor.DEGREE < Z_TEMP15)
                    {
                        mu16_EffStart_Temp = Z_TEMP78;
                    }
                    else
                    {
                        mu16_EffStart_Temp = Z_TEMP82;
                    }
					gu16_TargetDegree = Z_TEMP98;	//
					gu16_FlowTarget_W = StartFlow_Decide();

					///	추출시작 온도 결정 : 기준온도 30도 ± 히터전력 
					if (gu16_HeaterFull_WATT > HEATER_FULL_WATT_KOR)
					{
						gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/5);	//	6와트당(나누기6) 1도 증가 또는 감소
						if (gi16_EffStartTargetTemp < Z_TEMP74)
						{
							gi16_EffStartTargetTemp = Z_TEMP74;
						}
					}
					else
					{
						gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/10);	//	10와트당(나누기10) 1도 증가 또는 감소
					}
					///	I값 초기치 계산 : (초기전류치-x) / (최대전력) + (히터전력) - 스텝바이유량	+ 입수온도	
					gu16_HeaterWATT_forCal = WATT_Ini();
					gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-0) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
					gu16_Heater_Dp1 = Z_Dp1;
					gu16_Heater_Dp2 = Z_Dp2;
					break;
				
				*/
				
					/*	목표온도 결정	*/
					
					
//					mu16_EffStart_Temp = Z_TEMP80;
					//mu16_EffStart_Temp = Z_TEMP78;		//@001
//					mu16_EffStart_Temp = Z_TEMP78+mi16_Buf;	//
//					if (mu16_EffStart_Temp > Z_TEMP90)
//					{
//						mu16_EffStart_Temp = Z_TEMP90;
//					}
					 if(gi16_HotInSensor.DEGREE < Z_TEMP15)
			                 {
			                        //mu16_EffStart_Temp = Z_TEMP78;
					       //mu16_EffStart_Temp = Z_TEMP82;
					       if(F_2ndHot_Effluent_HeaterDown==CLEAR)
					       {
					         mu16_EffStart_Temp = Z_TEMP85;   //79do under
						
					       
					       }
					       else
					       {mu16_EffStart_Temp = Z_TEMP78;}
			                 }
			                 else
			                 {
			                        //mu16_EffStart_Temp = Z_TEMP82;
						mu16_EffStart_Temp = Z_TEMP84;
					 }
					gu16_TargetDegree = Z_TEMP98;	//
					gu16_FlowTarget_W = StartFlow_Decide();
					//	추출시작 온도 결정 : 기준온도 30도 ± 히터전력
					if (gu16_HeaterFull_WATT > HEATER_FULL_WATT_KOR)
					{
						gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/5);	//	6와트당(나누기6) 1도 증가 또는 감소
						//gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/6);	//	6와트당(나누기6) 1도 증가 또는 감소
						if (gi16_EffStartTargetTemp < Z_TEMP74)
						{
							gi16_EffStartTargetTemp = Z_TEMP74;
						}
					}
					else
					{
						gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/10);	//	10와트당(나누기10) 1도 증가 또는 감소
					}
					//	I값 초기치 계산 : (초기전류치-x) / (최대전력) + (히터전력) - 스텝바이유량	+ 입수온도	
//@008					gu16_HeaterWATT_forCal = WATT_Ini();
					gu16_HeaterWATT_forCal = WATT_Ini();	//@020
					//gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-0) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
					
					if (gu8_HotStep == Z_HotStep_35)
					{
						if(gi16_HotInSensor.DEGREE < Z_TEMP15)
						{
						   ///gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal+200) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
						   	if(F_2ndHot_Effluent_HeaterDown==CLEAR)
						       {
							 if(gi16_HeaterSensor.DEGREE >= Z_TEMP96)
							 {
						          gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal+250) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
							 }
							 else
							 {
						          gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal+50) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
							 }
						       }
						       else
						       { 
						         gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal+180) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
						       }
						}
						else if(gi16_HotInSensor.DEGREE < Z_TEMP25)
						{
						gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal+150) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
						}
						else
						{
						gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal+100) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
						}
					}
					else
					{
						gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-0) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
					}
					gu16_Heater_Dp1 = Z_Dp1;
					gu16_Heater_Dp2 = Z_Dp2;
					
					
					
					break;
					
					
				case HOT_TEMP_SETUP_KOR85_JP80:		//	국내향 85도
				        
					//	목표온도 결정	
					//mu16_EffStart_Temp = Z_TEMP76+mi16_Buf;	//
					mu16_EffStart_Temp = Z_TEMP82+mi16_Buf;	
					//gu16_TargetDegree = Z_TEMP94;
					gu16_TargetDegree = Z_TEMP95;//
					gu16_FlowTarget_W = StartFlow_Decide();	//
					//	추출시작 온도 결정 : 기준온도 30도 ± 히터전력
					gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/10);	//	10와트당 1도 증가 또는 감소
					if (gi16_EffStartTargetTemp < Z_TEMP64)
					{
						gi16_EffStartTargetTemp = Z_TEMP64;
					}
					//	I값 초기치 계산 : (초기전류치-x) / (최대전력) + (히터전력) - 스텝바이유량	+ 입수온도
//@008					gu16_HeaterWATT_forCal = WATT_Ini();
					gu16_HeaterWATT_forCal = WATT_Ini();	//@020
					gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-100) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
					gu16_Heater_Dp1 = Z_Dp1;
					gu16_Heater_Dp2 = Z_Dp2;
							
					
					break;
					
				case HOT_TEMP_SETUP_KOR__80:		//	80도    
					 //mu16_EffStart_Temp = Z_TEMP75;	//
					 mu16_EffStart_Temp = Z_TEMP80;	//
					// mu16_EffStart_Temp = Z_TEMP70;		//cbr_test 2025-11-20
					//mu16_EffStart_Temp = Z_TEMP68;		//cbr_test 2025-11-20
			                    gu16_TargetDegree = Z_TEMP90;
			                   // gu16_TargetDegree = Z_TEMP78;				//cbr_test 2025-11-20
			                    gu16_FlowTarget_W = StartFlow_Decide();	//

			                    if(gu8_F_Hot80_FirstEffluent_correct == CLEAR)
			                    {
			                        if((gi16_HotInSensor.DEGREE < Z_TEMP15) && (gi16_HotOutSensor.DEGREE < Z_TEMP15))
			                        {
			                            gu8_F_Hot80_FirstEffluent_correct = SET;	
			                        }
			                    }
			                    if(gu8_F_Hot80_FirstEffluent_correct == SET)
			                    {
			                        gu16_TargetDegree = Z_TEMP91;
			                    }

					/*	추출시작 온도 결정 : 기준온도 30도 ± 히터전력 */
					gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/10);	//	10와트당 1도 증가 또는 감소
					/*	I값 초기치 계산 : (초기전류치-x) / (최대전력) + (히터전력) - 스텝바이유량	+ 입수온도	*/
					gu16_HeaterWATT_forCal = WATT_Ini();
					gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-100) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
					gu16_Heater_Dp1 = Z_Dp1;
					gu16_Heater_Dp2 = Z_Dp2;
					break;

				case HOT_TEMP_SETUP_KOR__75:		//	국내향 75도
					/*	목표온도 결정	*/
					/*
					if(gi16_HotInSensor.DEGREE > Z_TEMP25)
					{
						mu16_EffStart_Temp = Z_TEMP60;		//cbr_test 2025-11-20
						gu16_TargetDegree = Z_TEMP70;		//cbr_test 2025-11-21
					}
					else
					{
						mu16_EffStart_Temp = Z_TEMP65;		//cbr_test 2025-11-20
						gu16_TargetDegree = Z_TEMP73;		//cbr_test 2025-11-20
					}
					*/
					    mu16_EffStart_Temp = Z_TEMP73;	//
					    gu16_TargetDegree = Z_TEMP78;	
			                    gu16_FlowTarget_W = StartFlow_Decide();

			//@cjh              if(gi16_HotInSensor.DEGREE < Z_TEMP16)
			//@cjh              {
			//@cjh              	gu16_FlowTarget_W = gu16_FlowTarget_W - 4;
			//@cjh              }

					    /*	추출시작 온도 결정 : 기준온도 30도 ± 히터전력 */
					    gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/10);	//	10와트당 1도 증가 또는 감소

			                    /*  I값 초기치 계산 : (초기전류치-x) / (최대전력) + (히터전력) - 스텝바이유량  + 입수온도  */
			                    gu16_HeaterWATT_forCal = WATT_Ini();
			//@cjh        		gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal+200) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
			                    gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal+200) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck - mi16_Buf ;//
			                    gu16_Heater_Dp1 = Z_Dp1;
			                    gu16_Heater_Dp2 = Z_Dp2;
			                    break;
					
					
					
					
					
				case HOT_TEMP_SETUP_KOR70_JP70:		//	국내향 70도
					/*	목표온도 결정	*/
					//mu16_EffStart_Temp = Z_TEMP59;	//
					//gu16_TargetDegree = Z_TEMP74;		//			목표온도
					
					 //if((gi16_HotInSensor.DEGREE < Z_TEMP15) && (gu8_Heater_PowerLevel == Z_HEATER_POWER_LEVEL_LOW))	//2026-01-05
					 if(gi16_HotInSensor.DEGREE < Z_TEMP15)
					
			                    {
			                        //mu16_EffStart_Temp = Z_TEMP60;
			                        //gu16_TargetDegree = Z_TEMP70;       //2026-01-05
						mu16_EffStart_Temp = Z_TEMP60;  //
                        			gu16_TargetDegree = Z_TEMP75;       //          목표온도
			                    }
			                    else
			                    {           /*
							if(gi16_HotInSensor.DEGREE >= Z_TEMP25)
							{
								mu16_EffStart_Temp = Z_TEMP58;
								gu16_TargetDegree = Z_TEMP67;
							}
							else
							{
								mu16_EffStart_Temp = Z_TEMP60;  //cbr_test 2025-09-18
								gu16_TargetDegree = Z_TEMP68;       //
							} 
							*/
							mu16_EffStart_Temp = Z_TEMP59;  //
                        				gu16_TargetDegree = Z_TEMP74;       // 
			                    }
					
					
					gu16_FlowTarget_W = StartFlow_Decide();
					gu16_FlowTarget_W = gu16_FlowTarget_W - 7;
					/*	추출시작 온도 결정 : 기준온도 30도 ± 히터전력 */
					//gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/10);	//	10와트당 1도 증가 또는 감소
					/*	I값 초기치 계산 : (초기전류치-x) / (최대전력) + (히터전력) - 스텝바이유량	+ 입수온도	*/
//@008					gu16_HeaterWATT_forCal = WATT_Ini();
					gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/10);    //  10와트당 1도 증가 또는 감소
					//if ((gi16_EffStartTargetTemp < Z_TEMP63)&&(gi16_HotInSensor.DEGREE < Z_TEMP25))
					//{
					//	gi16_EffStartTargetTemp = Z_TEMP63;		//cbr_test 2025-09-18
					//}
					gu16_HeaterWATT_forCal = WATT_Ini();	//@020
					//gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-200) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
					if(gi16_HotInSensor.DEGREE < Z_TEMP15)
			                {
			                        gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal+200) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;
			                        //gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-200) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;
			                }
			                else
			                {
			                        gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-200) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
			                }
					
					
					gu16_Heater_Dp1 = Z_Dp1;
					gu16_Heater_Dp2 = Z_Dp2;
					break;
					
				case HOT_TEMP_SETUP_KOR__65:        //  65도(우선 45도랑 같게)
			                    /*  목표온도 결정 */
					    /*
			                    if((gi16_HotInSensor.DEGREE >= Z_TEMP25) && (gu8_Heater_PowerLevel == Z_HEATER_POWER_LEVEL_HIGH))
			                    //if((gi16_HotInSensor.DEGREE >= Z_TEMP25) ||(gu8_Heater_PowerLevel == Z_HEATER_POWER_LEVEL_HIGH))		//cbr 2025-12-04
								
			                    {
			                        mu16_EffStart_Temp = Z_TEMP50;
			                    }
			                    else
			                    {
			                        mu16_EffStart_Temp = Z_TEMP62;
			                        
			                        // mu16_EffStart_Temp = Z_TEMP50;		//cbr_test 2025-11-19
			                    }

			                    //  전압에 따른, Target 온도 변경
			                    if(gu8_Heater_PowerLevel == Z_HEATER_POWER_LEVEL_LOW)
			                    {
			                        if(gi16_HotInSensor.DEGREE <= Z_TEMP15)
			                        {
			                            //gu16_TargetDegree = Z_TEMP73;
						    if(F_2ndHot_Effluent_HeaterDown == CLEAR)  gu16_TargetDegree = Z_TEMP80;
						    else gu16_TargetDegree = Z_TEMP69;
			                        }
			                        else
			                        {
			                            gu16_TargetDegree = Z_TEMP75;
			                        }
			                    }
			                    else if(gu8_Heater_PowerLevel == Z_HEATER_POWER_LEVEL_RATED)
			                    {
			                        //gu16_TargetDegree = Z_TEMP69;
			                        // gu16_TargetDegree = Z_TEMP66;		//cbr_test 2025-11-19
						if(F_2ndHot_Effluent_HeaterDown == CLEAR)  gu16_TargetDegree = Z_TEMP80;
						else gu16_TargetDegree = Z_TEMP69;
			                    }
			                    else
			                    {
			                        gu16_TargetDegree = Z_TEMP63;
			                    }
					    if(gi16_HotInSensor.DEGREE >= Z_TEMP25)		//cbr_test 2025-11-20
					    {
						gu16_TargetDegree -= Z_TEMP5;
					    }

			                    gu16_FlowTarget_W = StartFlow_Decide();
			                    //  추출시작 온도 결정 : 기준온도 30도 ± 히터전력 
			                    gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/10);    //  10와트당 1도 증가 또는 감소
			                    //  I값 초기치 계산 : (초기전류치-x) / (최대전력-릴레이전력) + (히터전력) - 스텝바이유량  + 입수온도  

			                    gu16_HeaterWATT_forCal = WATT_Ini();
			                    gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal - 200) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
					    */
					     if(gi16_HotInSensor.DEGREE < Z_TEMP15)
					
			                    {
			                        //mu16_EffStart_Temp = Z_TEMP55;
			                        //gu16_TargetDegree = Z_TEMP65;       //2026-01-05
						
						if (F_2ndHot_Effluent_HeaterDown == CLEAR)
						{
							mu16_EffStart_Temp = Z_TEMP70;
			                                gu16_TargetDegree = Z_TEMP80;       //2026-01-05
						}
						else
						{
							mu16_EffStart_Temp = Z_TEMP55;
			                                 gu16_TargetDegree = Z_TEMP65;       //2026-01-05
							
						}
						
			                    }
			                    else
			                    {          
							mu16_EffStart_Temp = Z_TEMP59;  //
                        				gu16_TargetDegree = Z_TEMP74;       // 
			                    }
					
					
					gu16_FlowTarget_W = StartFlow_Decide();
					gu16_FlowTarget_W = gu16_FlowTarget_W - 7;
					/*	추출시작 온도 결정 : 기준온도 30도 ± 히터전력 */
					//gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/10);	//	10와트당 1도 증가 또는 감소
					/*	I값 초기치 계산 : (초기전류치-x) / (최대전력) + (히터전력) - 스텝바이유량	+ 입수온도	*/
//@008					gu16_HeaterWATT_forCal = WATT_Ini();
					gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/10);    //  10와트당 1도 증가 또는 감소
					//if ((gi16_EffStartTargetTemp < Z_TEMP63)&&(gi16_HotInSensor.DEGREE < Z_TEMP25))
					//{
					//	gi16_EffStartTargetTemp = Z_TEMP63;		//cbr_test 2025-09-18
					//}
					gu16_HeaterWATT_forCal = WATT_Ini();	//@020
					//gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-200) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
					if(gi16_HotInSensor.DEGREE < Z_TEMP15)
			                {
			                        gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal+100) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;
			                        //gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-200) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;
			                }
			                else
			                {
			                        gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-200) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
			                }
					    
					    
					    gu16_Heater_Dp1 = Z_Dp1;
					    gu16_Heater_Dp2 = Z_Dp2;
//					if ((gu8_HotStep == Z_HotStep_35) && (gu8_TimerFlowCheck_AfterEff == 0)	// 	45도 추출 시작 후 목표유량만큼 현재유량이 안나올 경우 목표유량 값 갱신하여 PI제어 용이하게
//					&& (gu16_FlowTarget_W > gu16_FlowDecide_W) && (gi16_StepCount >= 2800))
//					{
//						gu16_FlowTarget_W = gu16_FlowDecide_W;	//
//					}
					break;

				case HOT_TEMP_SETUP_KOR__60:		//	60도
					/*	목표온도 결정	*/
					mu16_EffStart_Temp = Z_TEMP55;

					if(gi16_HotInSensor.DEGREE < Z_TEMP15)						//	입수온 10도
					{
//@cjh241217			gu16_TargetDegree = Z_TEMP62;
						//gu16_TargetDegree = Z_TEMP63;	//@cjh241217
						//gu16_TargetDegree = Z_TEMP60;							//cbr 2025-12-04
						gu16_TargetDegree = Z_TEMP68;	
					}
					//else if((gi16_HotInSensor.DEGREE >= Z_TEMP15) && (gi16_HotInSensor.DEGREE < Z_TEMP25))
					//{															//	입수온 20도
					//	gu16_TargetDegree = Z_TEMP60;
					//}
					else														//	입수온 30도
					{
						gu16_TargetDegree = Z_TEMP62;
					}
                    
					gu16_FlowTarget_W = StartFlow_Decide();
			                if(gi16_HotInSensor.DEGREE < Z_TEMP15)
			                {
			                         //gu16_FlowTarget_W = gu16_FlowTarget_W - 6;
			                    	gu16_FlowTarget_W = gu16_FlowTarget_W - 4;	//@cjh241217
			                }
					/*	추출시작 온도 결정 : 기준온도 30도 ± 히터전력 */
					gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/10);	//	10와트당 1도 증가 또는 감소
					/*	I값 초기치 계산 : (초기전류치-x) / (최대전력-릴레이전력) + (히터전력) - 스텝바이유량	+ 입수온도	*/

					gu16_HeaterWATT_forCal = WATT_Ini();

					//	입수 고온인경우 초기 열량 변경
					if(gi16_HotInSensor.DEGREE >= Z_TEMP25)
					{
						gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal - 100) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
					}
					else
					{
						gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal - 400) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
					}

		                        gu16_Heater_Dp1 = Z_Dp1;
		                        gu16_Heater_Dp2 = Z_Dp2;
                    break;

                case HOT_TEMP_SETUP_KOR__55:        //  55도(우선 45도랑 같게)
                    /*  목표온도 결정 */
                    // mu16_EffStart_Temp = Z_TEMP52;
                    mu16_EffStart_Temp = Z_TEMP45;

					//	전압에 따른, Target 온도 변경
					if(gu8_Heater_PowerLevel == Z_HEATER_POWER_LEVEL_LOW)
					{
						gu16_TargetDegree = Z_TEMP56;
					}
					else if(gu8_Heater_PowerLevel == Z_HEATER_POWER_LEVEL_RATED)
					{
						if(gi16_HotInSensor.DEGREE < Z_TEMP15)
						{
							gu16_TargetDegree = Z_TEMP58;
						}
						else
						{
							gu16_TargetDegree = Z_TEMP56;
						}
					}
					else
					{
						if(gi16_HotInSensor.DEGREE >= Z_TEMP26)
						{
							gu16_TargetDegree = Z_TEMP54;
						}
						else
						{
							gu16_TargetDegree = Z_TEMP56;
						}
					}
					gu16_FlowTarget_W = StartFlow_Decide();
					/*	추출시작 온도 결정 : 기준온도 30도 ± 히터전력 */
					gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/10);	//	10와트당 1도 증가 또는 감소
					/*	I값 초기치 계산 : (초기전류치-x) / (최대전력-릴레이전력) + (히터전력) - 스텝바이유량	+ 입수온도	*/

		                    gu16_HeaterWATT_forCal = WATT_Ini();
		                    //gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal - 0) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
							gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal +100) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//  JHKO

					gu16_Heater_Dp1 = Z_Dp1_MILK;
					gu16_Heater_Dp2 = Z_Dp2_MILK;
					break;

				case HOT_TEMP_SETUP_KOR__50:		//	50도
					/*	목표온도 결정	*/
					/*	목표온도 결정	*/

					mu16_EffStart_Temp = Z_TEMP45;						//	42도로 하면..예열이 너무 빨리 도달하여 입수 고온인경우는 45로 셋팅
					// mu16_EffStart_Temp = Z_TEMP38;						//	42도로 하면..예열이 너무 빨리 도달하여 입수 고온인경우는 45로 셋팅
					//mu16_EffStart_Temp = Z_TEMP40;						//	42도로 하면..예열이 너무 빨리 도달하여 입수 고온인경우는 45로 셋팅
					 gu16_TargetDegree = Z_TEMP51;
					 /*
					if((gi16_HotInSensor.DEGREE > Z_TEMP25)		//cbr_test 2025-11-20
					&& (gu8_Heater_PowerLevel != Z_HEATER_POWER_LEVEL_LOW))		//cbr_test 2025-12-04
					{
						gu16_TargetDegree = Z_TEMP45;
					}
					else
					{
						gu16_TargetDegree = Z_TEMP50;
					}
					*/
					gu16_FlowTarget_W = StartFlow_Decide();
					/*	추출시작 온도 결정 : 기준온도 30도 ± 히터전력 */
					gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/10);	//	10와트당 1도 증가 또는 감소
					/*	I값 초기치 계산 : (초기전류치-x) / (최대전력-릴레이전력) + (히터전력) - 스텝바이유량	+ 입수온도	*/

					gu16_HeaterWATT_forCal = WATT_Ini();
					gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-0) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//

			                 gu16_Heater_Dp1 = Z_Dp1_MILK;
			                 gu16_Heater_Dp2 = Z_Dp2_MILK;

//					if ((gu8_HotStep == Z_HotStep_35) && (gu8_TimerFlowCheck_AfterEff == 0)	// 	45도 추출 시작 후 목표유량만큼 현재유량이 안나올 경우 목표유량 값 갱신하여 PI제어 용이하게
//					&& (gu16_FlowTarget_W > gu16_FlowDecide_W) && (gi16_StepCount >= 2800))
//					{
//						gu16_FlowTarget_W = gu16_FlowDecide_W;	//
//					}

					break;	
					
				case HOT_TEMP_SETUP_KOR45_JPX:		//	국내향 45도
					/*	목표온도 결정	*/
					mu16_EffStart_Temp = Z_TEMP37;
					gu16_TargetDegree = Z_TEMP44;
					//gu16_TargetDegree = Z_TEMP46;
					gu16_FlowTarget_W = StartFlow_Decide();
					/*	추출시작 온도 결정 : 기준온도 30도 ± 히터전력 */
					gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/10);	//	10와트당 1도 증가 또는 감소
					/*	I값 초기치 계산 : (초기전류치-x) / (최대전력-릴레이전력) + (히터전력) - 스텝바이유량	+ 입수온도	*/

			                if((gi16_HotInSensor.DEGREE < Z_TEMP15)  && (F_FirstHotEff_Detect == SET))
			                   
			                {
			                        gu16_HeaterWATT_forCal = WATT_Ini();
			                        //gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-500) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
			                        
									gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal+150) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
			                        // gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-0) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
			                }
			                else
			                {
			                        gu16_HeaterWATT_forCal = WATT_Ini();
			     //                   gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-0) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
                        gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-0) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
			    
			                }
					gu16_Heater_Dp1 = Z_Dp1_MILK;
					gu16_Heater_Dp2 = Z_Dp2_MILK;
					if ((gu8_HotStep == Z_HotStep_35) && (gu8_TimerFlowCheck_AfterEff == 0)	// 	45도 추출 시작 후 목표유량만큼 현재유량이 안나올 경우 목표유량 값 갱신하여 PI제어 용이하게
					&& (gu16_FlowTarget_W > gu16_FlowDecide_W) && (gi16_StepCount >= 2800))
					{
						gu16_FlowTarget_W = gu16_FlowDecide_W;	//
					}
					break;
#endif
			}

			if (gi16_EffStartTargetTemp > Z_TEMP90)
			{
				gi16_EffStartTargetTemp = Z_TEMP90;
			}
		}

		else										//	일본향
		{
			switch (gu8_HotTempSetup)				//	설정온도별 데이터 설정
			{
				case HOT_TEMP_SETUP_KOR100_JP90:	//	일본향 90도
				default:
					/*	목표온도 결정	*/
					mu16_EffStart_Temp = Z_TEMP83;	//
					gu16_TargetDegree = Z_TEMP93;	//
					gu16_FlowTarget_W = StartFlow_Decide();
					/*	추출시작 온도 결정 : 기준온도 30도 ± 히터전력 */
					gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_JP-gu16_HeaterFull_WATT)/10);	//	10와트당 1도 증가 또는 감소
					if (gi16_EffStartTargetTemp > Z_TEMP88)
					{
						gi16_EffStartTargetTemp = Z_TEMP88;
					}
					/*	I값 초기치 계산 : (초기전류치-x) / (최대전력) + (히터전력) - 스텝바이유량	+ 입수온도	*/
//@008					gu16_HeaterWATT_forCal = WATT_Ini();
					gu16_HeaterWATT_forCal = WATT_Ini();	//@020
					gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-0) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_JP-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
					gu16_Heater_Dp1 = Z_Dp1;
					gu16_Heater_Dp2 = Z_Dp2*3/5;
					break;
				case HOT_TEMP_SETUP_KOR85_JP80:		//	일본향 80도
					/*	목표온도 결정	*/
					mu16_EffStart_Temp = Z_TEMP68;	//
					gu16_TargetDegree = Z_TEMP82;	//			목표온도
					gu16_FlowTarget_W = StartFlow_Decide();
					/*	추출시작 온도 결정 : 기준온도 30도 ± 히터전력 */
					gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_JP-gu16_HeaterFull_WATT)/10);	//	10와트당 1도 증가 또는 감소
					/*	I값 초기치 계산 : (초기전류치-x) / (최대전력) + (히터전력) - 스텝바이유량	+ 입수온도	*/
//@008					gu16_HeaterWATT_forCal = WATT_Ini();
					gu16_HeaterWATT_forCal = WATT_Ini();	//@020
					gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-0) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_JP-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
					gu16_Heater_Dp1 = Z_Dp1;
					gu16_Heater_Dp2 = Z_Dp2;
					break;
				case HOT_TEMP_SETUP_KOR70_JP70:		//	일본향 70도
					/*	목표온도 결정	*/
					mu16_EffStart_Temp = Z_TEMP54;
					gu16_TargetDegree = Z_TEMP70;
					gu16_FlowTarget_W = StartFlow_Decide();
					/*	추출시작 온도 결정 : 기준온도 30도 ± 히터전력 */
					gi16_EffStartTargetTemp = mu16_EffStart_Temp+((I16)((I16)HEATER_FULL_WATT_JP-gu16_HeaterFull_WATT)/10);	//	10와트당 1도 증가 또는 감소
					/*	I값 초기치 계산 : (초기전류치-x) / (최대전력-릴레이전력) + (히터전력) - 스텝바이유량	+ 입수온도	*/
//@008					gu16_HeaterWATT_forCal = WATT_Ini();
					gu16_HeaterWATT_forCal = WATT_Ini();	//@020
					gi16_Heater_Di_ini = (I16)((U32)(gu16_HeaterWATT_forCal-0) * 100 / gu16_HeaterFull_WATT)+((I16)((I16)HEATER_FULL_WATT_JP-gu16_HeaterFull_WATT)/100)-gi16_FlowCheck+gi16_HotIn_DegreeCheck;//
					gu16_Heater_Dp1 = Z_Dp1;
					gu16_Heater_Dp2 = Z_Dp2;
					break;
			}

			if (gi16_EffStartTargetTemp > Z_TEMP94)
			{
				gi16_EffStartTargetTemp = Z_TEMP94;
			}
		}

		if (gi16_Heater_Di_ini > 100)				//	최대치 제한
		{
			gi16_Heater_Di_ini = 100;
		}
		else if (gi16_Heater_Di_ini < 0)			//	최소치 제한
		{
			gi16_Heater_Di_ini = 0;
		}
		else
		{

		}

#ifdef _FOR_DEVELOP_
		if (gi16_Heater_Di_ini_Test != 0) gi16_Heater_Di_ini = gi16_Heater_Di_ini_Test;
#endif

		(Hot_Step[gu8_HotStepNum].exe)();			//	해당 스텝 함수 실행

		gu8_HotInValve_CMD = Hot_Step[gu8_HotStepNum].HOTIN_V;	//	입수밸브 출력 설정
		gu8_HotOutValve_CMD = Hot_Step[gu8_HotStepNum].HOTOUT_V;	//	출수밸브 출력 설정

		if ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR45_JPX) && (gu8_ModelType_forNational == MODEL_KOREA) && (gu8_HotStep == Z_HotStep_33))	//	분유는 예열 중에 Heater Relay OFF
		{
			gu8_HeaterRelay_CMD = HEATER_OFF;
		}
		else
		{
			gu8_HeaterRelay_CMD = Hot_Step[gu8_HotStepNum].HEATER_RELAYCMD;	//	Heater Relay 출력 설정
		}
		gu8_HeaterTriac_CMD = Hot_Step[gu8_HotStepNum].HEATER_PWMCMD;	//	Heater PWM 출력 설정
	}
	else											//	스텝 허용 범위 외
	{
		SafetyLoop();								//		안전정지 루프
	}
}

/// @brief    온수 초기 스텝
/// @param    void
/// @return   void
static void HotStep_00(void)
{
	if (gu8_ModelType_forCHP == MODEL_CHP)
	{
		Ini_HotStep_01();
	}
}
static void Ini_HotStep_00(void)
{
	HotStepNoSet(Z_HotStepNum_00);
}

/// @brief    첫잔 대기
/// @param    void
/// @return   void
static void HotStep_01(void)
{
	if ((F_FirstHotEffluent == CLEAR) && (gu8_WaterSelect == HOT_WATER) && (gu8_OperationStep == Z_OperationStep_50) && (gu8_Stepping_ZeroBack == 0))
	{
		Ini_HotStep_02();
	}
	else if (gu8_OperationStep == Z_OperationStep_04)
	{
		Ini_HotStep_02();
	}
	else if (F_FirstHotEffluent == SET)
	{
		Ini_HotStep_10();
	}
	else if (gu8_ModelType_forCHP == MODEL_CP)
	{
		Ini_HotStep_00();
	}
	else
	{

	}
}
static void Ini_HotStep_01(void)
{
	HotStepNoSet(Z_HotStepNum_01);
}

/// @brief    온수 첫잔
/// @param    void
/// @return   void
static void HotStep_02(void)
{
	if ((F_FirstHotEff_HeaterSensing == CLEAR) && (gu16_Effluent_Hz < (gu16_FirstHotEff_Hz/2)))
	{
		F_FirstHotEff_HeaterSensing = SET;
		gu8_TimerFirstHot_HeaterSensing = Z_TimerFirstHot_HeaterSensing;
		gu16_FlowDecide_W_HotEffMax = gu16_FlowDecide_W;
		if (gu8_ModelType_forNational == MODEL_KOREA)
		{
			FlowControl_StepTarget((3000-Z_StepDown_forStepByFlowCheck_KOR));
		}
		else
		{
			FlowControl_StepTarget((3000-Z_StepDown_forStepByFlowCheck_JP));
		}
	}

	if (gu16_Effluent_Hz == 0)
	{
		F_FirstHotEffluent = SET;
		StepMotor_ZeroBack_Open();
		Ini_HotStep_10();
	}
	else if ((gu8_OperationStep_hi == 0x00) && (F_Heater_Fill == SET))
	{
		StepMotor_ZeroBack_Open();
		Ini_HotStep_10();
	}
	else if ((gu8_OperationStep != Z_OperationStep_04) && (gu8_OperationStep != Z_OperationStep_50))
	{
		StepMotor_ZeroBack_Open();
		Ini_HotStep_01();
	}
	else
	{

	}
}
static void Ini_HotStep_02(void)
{
	gu8_TimerFirstHotDisplay = Z_TimerFirstHotDisplay;
	FlowControl_StepTarget(2800);
	F_FirstHotEff_HeaterSensing = CLEAR;
	HotStepNoSet(Z_HotStepNum_02);
	if (gu16_CupSize == CUPSIZE_CONTINUE)
	{
		#if 0
		gu16_CupSize = gu8_DefaultCupSize;
		#endif
		return_default_cup();	
	}

	if (gu8_OperationStep_hi != 0x00)				//	전원 인가 후 온수 첫잔 (플러싱 중 제외. 플러싱 중에는 운전상태에서 Hz Set함)
	{
		gu16_Effluent_Hz = (U16)((U32)Z_HeaterFill_Litter * 10000 / gu16_FlowMeter_Standard);
	}

	if (gu8_ModelType_forNational == MODEL_KOREA)
	{
		gu8_TimerFlowCheck_AfterEff = Z_TimerFlowCheck_AfterEff;		//	추출 시작 후 유량체크 타이머 set
	}
	else
	{
		gu8_TimerFlowCheck_AfterEff = Z_TimerFlowCheck_AfterEff_forJapan;		//	추출 시작 후 유량체크 타이머 set
	}

	gu16_Effluent_Hz_forSegDisplay = gu16_Effluent_Hz;
	gu16_Effluent_CupLitter_forSegDisplay = 150;
	gu16_FirstHotEff_Hz = (U16)((U32)Z_HeaterFill_Litter * 10000 / gu16_FlowMeter_Standard);

	gu8_FlowTarget_Decrease = 0;					//@007
}

/// @brief    온수 미선택
/// @param    void
/// @return   void
static void HotStep_10(void)
{
	if (gu8_WaterSelect == HOT_WATER)				//	온수 선택 일 경우
	{
		Ini_HotStep_20();							//		온수 선택 대기로 이행
	}
//	else if ((gu8_OperationStep != Z_OperationStep_50) && (F_StartFlow_ReCalibration == SET))	//	유량 재설정
//	{
//		Ini_HotStep_3E();							//		유량 재설정으로 이행
//	}
	else if ((F_FirstHotEffluent == CLEAR) || (F_Heater_Fill == CLEAR))
	{
		Ini_HotStep_01();
	}
//	else if ((gu8_ModelType_forNational == MODEL_JAPAN) && (KEY_HOTWATER_LEVEL == HIGH) && (gi16_HeaterSensor.DEGREE < Z_PreHeating_Degree_forJAPAN))
//	{
//		Ini_HotStep_32();
//	}
	else
	{

	}
}
static void Ini_HotStep_10(void)
{
	F_VoiceOutput_HotCaution = CLEAR;
	F_Flow_Rapidly_Decrease = CLEAR;
	HotStepNoSet(Z_HotStepNum_10);
}

/// @brief    온수 선택 대기
/// @param    void
/// @return   void
static void HotStep_20(void)
{
	if (gu8_WaterSelect != HOT_WATER)				//	온수 선택이 아니면
	{
		Ini_HotStep_10();							//		온수 미선택 이행
	}
	else if (gu8_OperationStep == Z_OperationStep_50)	//	추출 중 인 경우
	{
		BeforeEff_SteppingMotor_Check();
	}
	else if ((gu8_ModelType_forNational == MODEL_JAPAN) && (gi16_HeaterSensor.DEGREE < Z_PreHeating_Degree_forJAPAN))
	{
		Ini_HotStep_32();
	}
	else if (F_FirstHotEffluent == CLEAR)
	{
		Ini_HotStep_01();
	}
	else
	{

	}
}
static void Ini_HotStep_20(void)
{
	F_VoiceOutput_HotCaution = CLEAR;
	F_Flow_Rapidly_Decrease = CLEAR;
	F_MilkEff_MoreHot = CLEAR;
	F_HeaterPWM_SingleControl = CLEAR;
	
	
      
        gu8_F_Hot80_FirstEffluent_correct = CLEAR;
    
	HotStepNoSet(Z_HotStepNum_20);
}

/// @brief    스테핑모터 초기화 대기
/// @param    void
/// @return   void
static void HotStep_2F(void)
{
	if (gu8_Stepping_ZeroBack == 0)					//	초기화 완료 인 경우
	{
		Ini_HotStep_33();							//		온수 선택 대기로 이행
	}
	else if ((gu8_WaterSelect != HOT_WATER) || (gu8_OperationStep != Z_OperationStep_50))	//@019	추출 중이 아닌 경우
	{
		Ini_HotStep_20();							//@019		온수 선택 대기로 이행
	}
	else											//@019
	{

	}
}
static void Ini_HotStep_2F(void)
{
	Effluent_Hz_Decide();							//	추출 유량 결정
	HotStepNoSet(Z_HotStepNum_2F);
}

/// @brief    목표유량 결정 및 스테핑모터 무빙 대기
/// @param    void
/// @return   void
static void HotStep_31(void)
{
	Ini_HotStep_20();								//	미사용 스텝

//	if ((gu8_WaterSelect != HOT_WATER) || (gu8_OperationStep != Z_OperationStep_50))	//	추출 중이 아닌 경우
//	{
//		Ini_HotStep_20();							//		온수 선택 대기로 이행
//	}
//	else if (((gu16_StepMovingPulse == 0) && (gu16_StepMovingPulse_Backup == 0)) || (gu8_TimerHotStep_100ms == 0))	//	스테핑모터 동작이 종료 and 2초 time out
//	{
//		Ini_HotStep_33();							//		예열
//	}
//	else
//	{
//
//	}
}
static void Ini_HotStep_31(void)
{
	HotStepNoSet(Z_HotStepNum_31);
//	gu16_FlowTarget_W = StartFlow_Decide();			//	추출 목표유량 결정
	if (gu16_FlowTarget_W != 0)						//	유량 결과값이 있는 경우
	{
		StartFlow_StepMotorControl();				//		스테핑모터 제어
	}
	gu8_TimerHotStep_100ms = Z_TimerHotStep_100ms_2s;
}

/// @brief    온수 선택 중 예열 (일본향)
/// @param    void
/// @return   void
static void HotStep_32(void)
{
	if (gu8_ModelType_forNational != MODEL_JAPAN)
	{
		Ini_HotStep_20();
	}
	else if (gi16_HeaterSensor.DEGREE >= Z_PreHeating_Degree_forJAPAN)
	{
		Ini_HotStep_20();
	}
	else if (gu8_WaterSelect != HOT_WATER)			//	온수 선택이 아니면
	{
		Ini_HotStep_10();							//		온수 미선택 이행
	}
	else if (gu8_OperationStep == Z_OperationStep_50)	//	추출 중 인 경우
	{
		BeforeEff_SteppingMotor_Check();
	}
	else if (F_FirstHotEffluent == CLEAR)
	{
		Ini_HotStep_01();
	}
	else
	{

	}
}
static void Ini_HotStep_32(void)
{
	HotStepNoSet(Z_HotStepNum_32);
}

/// @brief    예열 중
/// @param    void
/// @return   void
U8 u8_Test_HotStep_33;
static void HotStep_33(void)
{
	U8 i = 0;
	U8 HotIncrease = 0;
	U16 temp_buf = 0;

	if ((gi16_HeaterSensor_Before[0] > gi16_HeaterSensor_Before[1])
		&& (gi16_HeaterSensor_Before[0] > gi16_HeaterSensor_Before[2])
		&& (gi16_HeaterSensor_Before[1] > gi16_HeaterSensor_Before[2]))
	{
		HotIncrease = SET;
	}

//	if (((gu8_HotTempSetup == HOT_TEMP_SETUP_70) && (F_Temp70_HeaterDegree_Up_Need == CLEAR)) || (gu8_HotTempSetup == HOT_TEMP_SETUP_45))
	if ((gu8_ModelType_forNational == MODEL_KOREA) && ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR70_JP70) || ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR45_JPX) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__50))))
	{
		HotIncrease = SET;
	}

	if (F_2ndHot_Effluent == SET)
	{
		//if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)
		 if ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)  || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__95) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__90))
		{
//			if (gi16_HeaterSensor.DEGREE >= (gu16_TargetDegree-Z_TEMP10))
			if (gu8_ModelType_forNational == MODEL_KOREA)
			{
				if (gi16_HeaterSensor.DEGREE >= (gi16_EffStartTargetTemp+((gu16_TargetDegree-gi16_EffStartTargetTemp)/2)))
				{
					F_2ndHot_Effluent = CLEAR;
				}
			}
			else									//	일본향
			{
				if (gi16_HeaterSensor.DEGREE >= (gu16_TargetDegree-Z_TEMP1))
				{
					F_2ndHot_Effluent = CLEAR;
				}
			}
		}
		else
		{
//			if (gi16_HeaterSensor.DEGREE >= (gu16_TargetDegree-Z_TEMP10))
			if (gi16_HeaterSensor.DEGREE >= (gi16_EffStartTargetTemp+((gu16_TargetDegree-gi16_EffStartTargetTemp)/2)))
			{
				F_2ndHot_Effluent = CLEAR;
			}
		}
	}
//	if (((gi16_EffStartTargetTemp2 != 0) && (gi16_HeaterSensor.DEGREE < gi16_EffStartTargetTemp2)) || (gi16_HeaterSensor.DEGREE < gi16_EffStartTargetTemp))	//	히터센서 온도가 추출시작 온도보다 낮은경우
	if ((HotIncrease == CLEAR) || (gi16_HeaterSensor.DEGREE < (gi16_EffStartTargetTemp+temp_buf)) //	히터센서 온도가 추출시작 온도보다 낮은경우
	|| (F_2ndHot_Effluent == SET) || (gu8_TimerMustHeating != 0))
	{
		gu16_TimerHotStep_10ms = Z_TimerHotStep_10ms_100ms;			//	온도 달성 시간
	}

	if ((gu8_WaterSelect != HOT_WATER) || (gu8_OperationStep != Z_OperationStep_50))	//	추출 중이 아닌 경우
	
		
	{
		Ini_HotStep_20();							//		온수 선택 대기로 이행
	}
#ifndef _HOT_TEMP_4STEP_
	else if (gi16_HeaterSensor.DEGREE >= Z_TEMP96)
	{
		Ini_HotStep_34();
	}
#else
	 /*else if ((gu8_ModelType_forNational == MODEL_KOREA)
            	  && ((gu8_HotTempSetup != HOT_TEMP_SETUP_KOR100_JP90)  || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__95) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__90))
                   && (gi16_HeaterSensor.DEGREE >= Z_TEMP96))	
		   */
	 else if ((gu8_ModelType_forNational == MODEL_KOREA) && (gu16_TargetDegree != Z_TEMP98)  && (gi16_HeaterSensor.DEGREE >= Z_TEMP96))	  
	{
		Ini_HotStep_34();
		u8_Test_HotStep_33=1;
	}
	else if ((gu8_ModelType_forNational == MODEL_JAPAN)
			&& (gu8_HotTempSetup != HOT_TEMP_SETUP_KOR100_JP90) && (gi16_HeaterSensor.DEGREE >= Z_TEMP97))
	{
		Ini_HotStep_34();
	}
#ifndef _HeaterOff_OFF_
/*
	 else if (((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)|| (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__95) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__90))&& (gi16_HeaterSensor.DEGREE >= Z_TEMP95) && (F_2ndHot_Effluent_HeaterDown ==  CLEAR))
	 {
		Ini_HotStep_34();
		u8_Test_HotStep_33=2;
	}
	 */
	  else if (((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)  || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__95) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__90))
            && (gi16_HeaterSensor.DEGREE >= Z_TEMP98_5))
			            
#else
	else if ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90) && (gi16_HeaterSensor.DEGREE >= Z_TEMP98_5))
#endif
	{
		Ini_HotStep_34();
		u8_Test_HotStep_33=3;
	}
#endif
	//else if ((gu8_TimerHotSafety == 0) || (gu16_TimerHotStep_10ms == 0) || (gi16_HeaterSensor.DEGREE >= (gu16_TargetDegree+Z_TEMP10)))
	//else if ((gu8_TimerHotSafety == 0) || (gu16_TimerHotStep_10ms == 0) || (gi16_HeaterSensor.DEGREE >= (gu16_TargetDegree+Z_TEMP20)))	//cbr_test 2025-09-18 잔열로 +10도 상태면 예열 생략되어 다음잔 온도 낮음.
	/*else if ((gu8_HotTempSetup != HOT_TEMP_SETUP_KOR100_JP90 &&
         gu8_HotTempSetup != HOT_TEMP_SETUP_KOR__95 &&
         gu8_HotTempSetup != HOT_TEMP_SETUP_KOR__90)&&
	 ((gu8_TimerHotSafety == 0) || (gu16_TimerHotStep_10ms == 0) || (gi16_HeaterSensor.DEGREE >= (gu16_TargetDegree+Z_TEMP10))))*/
	else if ((gu8_TimerHotSafety == 0) || (gu16_TimerHotStep_10ms == 0) || (gi16_HeaterSensor.DEGREE >= (gu16_TargetDegree+Z_TEMP10)))
	{
		Ini_HotStep_34();
		u8_Test_HotStep_33=4;
	}
	else
	{

	}

	if (F_100ms_for_HeaterDegreeSave == SET)		//	온도 상승 체크 주기
	{
		F_100ms_for_HeaterDegreeSave = CLEAR;
//		gi16_HeaterSensor_Before[3] = gi16_HeaterSensor_Before[2];
		gi16_HeaterSensor_Before[2] = gi16_HeaterSensor_Before[1];
		gi16_HeaterSensor_Before[1] = gi16_HeaterSensor_Before[0];
		gi16_HeaterSensor_Before[0] = gi16_HeaterSensor.DEGREE;
	}
}
static void Ini_HotStep_33(void)
{
//	I8 mi8_HeaterCheck = 0;
//	I16 mi16_HeaterDegreeCheck = 0;
//	I16 mi16_HotInDegreeCheck = 0;
//	I16 mi16_TempBuf = 0;
	I8 mi8_Buf = 0;
	u8_Test_HotStep_33=0;
#ifdef _FOR_DEVELOP_
	gu16_HeatingTime = 0;
#endif
	Effluent_Hz_Decide();					//	추출 유량 결정

	F_2ndHot_Effluent = CLEAR;
	F_2ndHot_Effluent_HeaterDown = CLEAR;
//	if ((gi16_HeaterSensor.DEGREE >= (gi16_EffStartTargetTemp-Z_TEMP5)) && (gu8_HotTempSetup != HOT_TEMP_SETUP_KOR45_JPX))	//	둘쨰 잔
	//if ((gi16_HeaterSensor.DEGREE >= (gi16_EffStartTargetTemp-Z_TEMP12)) && (gu8_HotTempSetup != HOT_TEMP_SETUP_KOR45_JPX))	//	둘쨰 잔
	if ((gi16_HeaterSensor.DEGREE >= (gi16_EffStartTargetTemp-Z_TEMP12))
	&& ((gu8_HotTempSetup != HOT_TEMP_SETUP_KOR45_JPX)
	&& (gu8_HotTempSetup != HOT_TEMP_SETUP_KOR__50)))	//	Second Cup
	{
		F_2ndHot_Effluent = SET;
		F_2ndHot_Effluent_HeaterDown = SET;
	}

//	F_Temp70_HeaterDegree_Up_Need = CLEAR;
//	if ((gu8_HotTempSetup == HOT_TEMP_SETUP_70)
//		&& (gi16_HeaterSensor.DEGREE < (gu16_TargetDegree - Z_TEMP5)))
//	{
//		F_Temp70_HeaterDegree_Up_Need = SET;
//	}

	HotStepNoSet(Z_HotStepNum_33);
//	F_MorePreHeating_forJapan = CLEAR;

	if (gu8_ModelType_forNational == MODEL_KOREA)
	{
		switch (gu8_HotTempSetup)	//	설정온도별 예열
		{
			case HOT_TEMP_SETUP_KOR100_JP90:
#ifdef _HOT_TEMP_4STEP:
			case HOT_TEMP_SETUP_KOR85_JP80:
#endif
			case HOT_TEMP_SETUP_KOR__95:
		        case HOT_TEMP_SETUP_KOR__90:
		        case HOT_TEMP_SETUP_KOR85_JP80:
		        case HOT_TEMP_SETUP_KOR__80:
			default:
				gu8_TimerHotSafety = Z_TimerHotSafety;			//	예열 최대 시간
				if (F_LatentHeat_Clear == SET)					//	잠열제거 했다면
				{
					F_LatentHeat_Clear = CLEAR;					//220405
					mi8_Buf = (I8)((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/100);	//	100와트당 0.1초 증가 또는 감소
					if (mi8_Buf >= -22)
					{
						gu8_TimerMustHeating = (U8)(22+mi8_Buf);
					}
					else
					{
						gu8_TimerMustHeating = 22;
					}
				}
				else
				{
					gu8_TimerMustHeating = 3;
					
				}
				//cbr_hot_pro 2025-09-02 11:09:38
				if((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__80 ) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR85_JP80))		//cbr_test 2025-09-16
				{
					// if(gi16_HeaterSensor.DEGREE >= Z_TEMP93)		//cbr_test 2025-12-03 10도 고전력 85도 오버히팅 개선 (다른온도 확인?)
					// // if((gi16_HeaterSensor.DEGREE >= (I16)gu16_TargetDegree + Z_TEMP5)		//cbr_test 2025-12-03 10도 고전력 85도 오버히팅 개선 (다른온도 확인?)
					// // && (gi16_HotOutSensor.DEGREE >= (I16)gu16_TargetDegree + Z_TEMP5))		//2025-12-03 히터잔열로 예열생략 시 온도미달 개선
					// {
					// 	gu8_TimerHotSafety = 0;
					// }
					// else
					// {
					// 	gu8_TimerHotSafety = Z_TimerHotSafety;			//	예열 최대 시간
					// }
					if((gi16_HotInSensor.DEGREE <= Z_TEMP15 ) && (gi16_HeaterSensor.DEGREE <= Z_TEMP50 ))
					{						
						F_FirstHotEff_Detect = SET;					//@cjh241217
					}												//@cjh241217
					else{											//@cjh241217
						F_FirstHotEff_Detect = CLEAR;				//@cjh241217
					}												//@cjh241217
				}                   								//@cjh241217
				else
				{													//cbr_test 2025-12-03
					// gu8_TimerHotSafety = Z_TimerHotSafety;			//	예열 최대 시간
				}
				
				
				break;
				
			case HOT_TEMP_SETUP_KOR__75:
	 			//if (gi16_HeaterSensor.DEGREE >= (I16)gu16_TargetDegree)
				if (gi16_HeaterSensor.DEGREE >= (I16)gu16_TargetDegree + Z_TEMP7 )
				{
					gu8_TimerHotSafety = 0;
				}
				else
				{
					gu8_TimerHotSafety = Z_TimerHotSafety;			//	예열 최대 시간
				}

				if(gi16_HeaterSensor.DEGREE < Z_TEMP50 )
				{
					F_FirstHotEff_Detect = SET;
				}
				else
				{
					F_FirstHotEff_Detect = CLEAR;
				}
				break;	
			
				
				
			case HOT_TEMP_SETUP_KOR70_JP70:
				if (gi16_HeaterSensor.DEGREE >= (I16)gu16_TargetDegree)
				{
					gu8_TimerHotSafety = 0;
				}
				else
				{
					gu8_TimerHotSafety = Z_TimerHotSafety;			//	예열 최대 시간
				}
				break;
				
			case HOT_TEMP_SETUP_KOR__65:								//	65도
				if (gi16_HeaterSensor.DEGREE >= (I16)gu16_TargetDegree + Z_TEMP5)
				{
					gu8_TimerHotSafety = 0;
				}
				else
				{
					gu8_TimerHotSafety = Z_TimerHotSafety;			//	예열 최대 시간
				}
				break;
			case HOT_TEMP_SETUP_KOR__60:								//	65도
				//if (gi16_HeaterSensor.DEGREE >= (I16)gu16_TargetDegree)
			        if (gi16_HeaterSensor.DEGREE >= (I16)gu16_TargetDegree + Z_TEMP2 )
				{
					gu8_TimerHotSafety = 0;
				}
				else
				{
					gu8_TimerHotSafety = Z_TimerHotSafety;			//	예열 최대 시간
				}

				if(gi16_HeaterSensor.DEGREE < Z_TEMP40 ){
					F_FirstHotEff_Detect = SET;
				}
				else{
					F_FirstHotEff_Detect = CLEAR;
				}
				break;
			case HOT_TEMP_SETUP_KOR__55:								//	55도
				if (gi16_HeaterSensor.DEGREE >= (I16)gu16_TargetDegree)
				{
					gu8_TimerHotSafety = 0;
				}
				else
				{
					gu8_TimerHotSafety = Z_TimerHotSafety;			//	예열 최대 시간
				}
				break;
			case HOT_TEMP_SETUP_KOR__50:								//	50도
//				if (gi16_HeaterSensor.DEGREE >= (I16)(gu16_TargetDegree+Z_TEMP5))
				if (gi16_HeaterSensor.DEGREE >= (I16)(gu16_TargetDegree))
				{
					gu8_TimerHotSafety = 0;
				}
				else
				{
					gu8_TimerHotSafety = Z_TimerHotSafety;			//	예열 최대 시간
				}
				//	시작	(45도 잠열 히터 OFF 경우 33,34스텝으로 인해 Relay이가 잠깐 켜졌다 꺼지는 현상 방지)
				if ((gu8_ModelType_forNational == MODEL_KOREA)
				&& (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__50)
				&& (gi16_HeaterSensor.DEGREE >= Z_TEMP65))		//	국내향 고온 추출 후 분유 추출 시
				{
					F_MilkEff_MoreHot = SET;					//		미가열 플래그 set
				}
				else
				{
					F_MilkEff_MoreHot = CLEAR;
				}
				//	끝
				if(gi16_HeaterSensor.DEGREE < Z_TEMP35 ){
					F_FirstHotEff_Detect = SET;
				}
				else{
					F_FirstHotEff_Detect = CLEAR;
				}
				break;
				
			case HOT_TEMP_SETUP_KOR45_JPX:
//				//if (gi16_HeaterSensor.DEGREE >= (I16)(gu16_TargetDegree-Z_TEMP5))
				if (gi16_HeaterSensor.DEGREE >= (I16)(gu16_TargetDegree+Z_TEMP5))
				//if (gi16_HeaterSensor.DEGREE >= (I16)(gu16_TargetDegree+Z_TEMP3))
				{
					gu8_TimerHotSafety = 0;
				}
				else
				{
					gu8_TimerHotSafety = Z_TimerHotSafety;			//	예열 최대 시간
					gu8_TimerMustHeating = 15;
				}

				//@004	시작	(45도 잠열 히터 OFF 경우 33,34스텝으로 인해 Relay이가 잠깐 켜졌다 꺼지는 현상 방지)
				if ((gu8_ModelType_forNational == MODEL_KOREA)
				&& (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR45_JPX)
				&& (gi16_HeaterSensor.DEGREE >= Z_TEMP65))		//	국내향 고온 추출 후 분유 추출 시
				{
					F_MilkEff_MoreHot = SET;					//		미가열 플래그 set
				}
				else
				{
					F_MilkEff_MoreHot = CLEAR;
				}
				//@004	끝
				//cbr_hot_pro 2025-09-02 11:13:15
				if(gi16_HeaterSensor.DEGREE < Z_TEMP30 ){
					F_FirstHotEff_Detect = SET;
				}
				else{
					F_FirstHotEff_Detect = CLEAR;
				}
				
				break;
		}
	}
	else							//	일본향
	{
		switch (gu8_HotTempSetup)	//	설정온도별 예열
		{
			case HOT_TEMP_SETUP_KOR100_JP90:
			default:
				gu8_TimerHotSafety = Z_TimerHotSafety_JP;			//	예열 최대 시간
//				if (gi16_HeaterSensor.DEGREE >= (gi16_EffStartTargetTemp-Z_TEMP10))
//				{
//					F_MorePreHeating_forJapan = SET;
//				}
//				if (gi16_HeaterSensor.DEGREE <= Z_TEMP40)
//				{
					gu8_TimerHotSafety = Z_TimerHotSafety_JP;			//	예열 최대 시간
//				}
//				else
//				{
//					gu8_TimerHotSafety = (U8)((gu16_TargetDegree - gi16_HeaterSensor.DEGREE + Z_TEMP10) / 2);			//	예열 최대 시간
//				}
//				gu8_TimerMustHeating = 13;
				mi8_Buf = (I8)((I8)13 + ((I8)((I16)((I16)HEATER_FULL_WATT_JP-gu16_HeaterFull_WATT)/50)));	//	50와트당 0.1초 증가 또는 감소
				if (mi8_Buf < 13)
				{
					gu8_TimerMustHeating = 13;
				}
				else
				{
					gu8_TimerMustHeating = (U8)mi8_Buf;
				}
				break;
			case HOT_TEMP_SETUP_KOR85_JP80:
//				if (gi16_HeaterSensor.DEGREE >= (I16)gu16_TargetDegree)
//				{
//					gu8_TimerHotSafety = 0;
//				}
//				else
//				{
					gu8_TimerHotSafety = Z_TimerHotSafety_JP;			//	예열 최대 시간
//				}
				break;
			case HOT_TEMP_SETUP_KOR70_JP70:
//				if (gi16_HeaterSensor.DEGREE >= (I16)gu16_TargetDegree)
//				{
//					gu8_TimerHotSafety = 0;
//				}
//				else
//				{
					gu8_TimerHotSafety = Z_TimerHotSafety_JP;			//	예열 최대 시간
//				}
				break;
		}
//		gu8_TimerMustHeating = 10;
	}

//	if (gu8_ModelType_forNational == MODEL_KOREA)
//	{
//		mi8_HeaterCheck = (I8)((I16)((I16)HEATER_FULL_WATT_KOR-gu16_HeaterFull_WATT)/10);
//	}
//	else
//	{
//		mi8_HeaterCheck = (I8)((I16)((I16)HEATER_FULL_WATT_JP-gu16_HeaterFull_WATT)/10);
//	}
//	mi16_HotInDegreeCheck = gi16_HotInSensor.DEGREE - Z_TEMP25;
//	if (mi16_HotInDegreeCheck > 150)
//	{
//		mi16_HotInDegreeCheck = 150;
//	}
//	else if (mi16_HotInDegreeCheck < 0)
//	{
//		mi16_HotInDegreeCheck = 0;
//	}
//	else
//	{
//
//	}
//	mi16_HotInDegreeCheck /= 2;

	gu16_HeaterWATT = 0;							//	히터 전력 초기화
//	gu16_FlowTarget_W = StartFlow_Decide();			//	추출 목표유량 결정
	StartFlow_StepMotorControl();					//		스테핑모터 제어

	gu8_TimerHeaterCheck = Z_TimerHeaterCheck;
	gu16_TimerHotStep_10ms = Z_TimerHotStep_10ms_100ms;			//	온도 달성 시간

//	if (gi16_HeaterSensor.DEGREE <= Z_TEMP50)
//	{
//		F_FirstHotEff_Detect = SET;						//		첫 째잔 감지
//	}
//	else
//	{
//		F_FirstHotEff_Detect = CLEAR;
//	}

	gi16_HeaterSensor_Before[0] = 0;
	gi16_HeaterSensor_Before[1] = 0;
	gi16_HeaterSensor_Before[2] = 0;

#ifdef _FOR_WQA_
	gu8_TimerHotSafety = 5;
#endif
}

/// @brief    온수 추출 전 추출밸브 먼저 OPEN
/// @param    void
/// @return   void
static void HotStep_34(void)
{
	if ((gu8_WaterSelect != HOT_WATER) || (gu8_OperationStep != Z_OperationStep_50))	//	추출 중이 아닌 경우
	{
		Ini_HotStep_20();							//		온수 선택 대기로 이행
	}
	else if (gu16_TimerHotStep_10ms == 0)
	{
		Ini_HotStep_35();
	}
	else
	{

	}
}
static void Ini_HotStep_34(void)
{
	gu16_TimerHotStep_10ms = Z_TimerHotStep_10ms_100ms;
	HotStepNoSet(Z_HotStepNum_34);
}

/// @brief    온수 추출 스텝
/// @param    void
/// @return   void
static void HotStep_35(void)
{
	Flow_Rapidly_Decrease_Check();					//	유량 급격 감소 체크

//	if ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)
//	&& (gu8_TimerHotEff_CautionVoice == 0))
//	{
//		gu8_TimerHotEff_CautionVoice = Z_TimerHotEff_CautionVoice;
//		Play_Voice(VOICE_HOTWATER_EFFLUENT_A);
//	}
	/*	첫잔 가열 상승에 따른 히터 Limit 시작 */
	if (F_100ms_for_HeaterDegreeSave == SET)		//	온도 상승 체크 주기
	{
		F_100ms_for_HeaterDegreeSave = CLEAR;
		//if ((gu8_ModelType_forNational == MODEL_KOREA) && (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)
		//	&& (gu16_HeatingTime > 40) && (gu8_TimerHeaterLimitCheck != 0))		//	100도 설정온도 추출 중 4초 지난 경우에 히터온도가 이전온도보다 1도 이상 급격히 상승하는 경우
		if ((gu8_ModelType_forNational == MODEL_KOREA) && ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)  || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__95) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__90))
			&& (gu16_HeatingTime > 40) && (gu8_TimerHeaterLimitCheck != 0))		//	100도 설정온도 추출 중 4초 지난 경우에 히터온도가 이전온도보다 1도 이상 급격히 상승하는 경우
		
		{
			//20260310
			//if (gi16_HeaterSensor.DEGREE > (gi16_HeaterSensor_Before_forEff + Z_TEMP1))	//히터 온도가 이전 히터온도 _1도 초과인 경우
			if (gi16_HeaterSensor.DEGREE > (gi16_HeaterSensor_Before_forEff + Z_TEMP2))	//히터 온도가 이전 히터온도 _2도 초과인 경우
			{
				if (F_HeaterLimit == CLEAR)
				{
					F_HeaterLimit = SET;			//	히터 제한
					gu8_TimerHeaterLimit = Z_TimerHeaterLimit; //	히터 제한 타이머 4s Set
				}
			}
		}
		gi16_HeaterSensor_Before_forEff = gi16_HeaterSensor.DEGREE;

		//if ((gu8_ModelType_forNational == MODEL_KOREA) && (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90))	//@007
		if ((gu8_ModelType_forNational == MODEL_KOREA)
				&& ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)  || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__95) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__90)))
		
		
		{
			HotTemp100_FlowTargetDecrease_Check();	//@007		설정온도 목표 온도 미도달 체크 (미도달시 목표 유량 감소)
		}
		//cbr_hot_pro 2025-09-02 11:16:50
		else if((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__55)|| (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__65))
		{
			HotTemp100_FlowTargetDecrease_Check();
		}
		else
		{

        	}
	}
	/*	끝	*/

	/* 75도 제어 조건 (시작) */
	    if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__75)
	    {
	    	if(gi16_HotInSensor.DEGREE > Z_TEMP25 )
		{
	//@cjh241217   if((gi16_HeaterSensor.DEGREE >= Z_TEMP85) || (gi16_HotOutSensor.DEGREE >= Z_TEMP80 )){
	    		if((gi16_HeaterSensor.DEGREE >= Z_TEMP83) || (gi16_HotOutSensor.DEGREE >= Z_TEMP78 ))
			{		//@cjh241217
	    			F_Heater_Decrease = SET;
	    		}
	    		else{
	    			F_Heater_Decrease = CLEAR;
	    		}
	    	}
	    }
    /* 75도 제어 조건 (끝) */
    
           /* 65도 제어 조건 (시작) */
	    if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__65)
	    {
	    	if(gi16_HotInSensor.DEGREE < Z_TEMP15 )
		{
	//@cjh241217   if((gi16_HeaterSensor.DEGREE >= Z_TEMP85) || (gi16_HotOutSensor.DEGREE >= Z_TEMP80 )){
	    		//if((gi16_HeaterSensor.DEGREE >= Z_TEMP83) || (gi16_HotOutSensor.DEGREE >= Z_TEMP78 ))
			if((gi16_HeaterSensor.DEGREE >= Z_TEMP75) || (gi16_HotOutSensor.DEGREE >= Z_TEMP75 ))
			{		//@cjh241217
	    			F_Heater_Decrease = SET;
	    		}
	    		else{
	    			F_Heater_Decrease = CLEAR;
	    		}
	    	}
		else
		{
	//@cjh241217   if((gi16_HeaterSensor.DEGREE >= Z_TEMP85) || (gi16_HotOutSensor.DEGREE >= Z_TEMP80 )){
	    		//if((gi16_HeaterSensor.DEGREE >= Z_TEMP83) || (gi16_HotOutSensor.DEGREE >= Z_TEMP78 ))
			if((gi16_HeaterSensor.DEGREE >= Z_TEMP73) || (gi16_HotOutSensor.DEGREE >= Z_TEMP73 ))
			{		//@cjh241217
	    			F_Heater_Decrease = SET;
	    		}
	    		else{
	    			F_Heater_Decrease = CLEAR;
	    		}
	    	}
		
	    }
    /* 65도 제어 조건 (끝) */
    
    
	     if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__60){
    	 if(gi16_StepCount >= 2700){
//@cjh241217 if(gi16_HeaterSensor.DEGREE >= Z_TEMP70){
    		 if(gi16_HeaterSensor.DEGREE >= Z_TEMP68){		//@cjh241217
    			 F_Heater_Decrease = SET;
    		 }
//@CJH241217 else if(gi16_HeaterSensor.DEGREE <= Z_TEMP66){
    		 else if(gi16_HeaterSensor.DEGREE <= Z_TEMP63){ //@cjh241217
    			 F_Heater_Decrease = CLEAR;
    		 }
    		 else{

    		 }
    	 }
     }
     /* 60도 제어 조건 (끝) */
     
      if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__55)
      {
	      /*
    	 if(gi16_StepCount >= 2700){
//@cjh241217 if(gi16_HeaterSensor.DEGREE >= Z_TEMP70){
    		 if(gi16_HeaterSensor.DEGREE >= Z_TEMP58){		//@cjh241217
    			 F_Heater_Decrease = SET;
    		 }
//@CJH241217 else if(gi16_HeaterSensor.DEGREE <= Z_TEMP66){
    		 else if(gi16_HeaterSensor.DEGREE <= Z_TEMP55){ //@cjh241217
    			 F_Heater_Decrease = CLEAR;
    		 }
    		 else{

    		 }
    	 }*/
	 if(gi16_HotInSensor.DEGREE > Z_TEMP25){
     		 if((gi16_HeaterSensor.DEGREE >= Z_TEMP68) && (gu16_HotOut_Degree_Ave > gu16_HotOut_ClearMilkHot)){
      			F_MilkEff_MoreHot = SET;					//		미가열 플래그 set
      		 }
    	  }
    	  else if(gi16_HotInSensor.DEGREE > Z_TEMP15){
//@cjh241217  if((gi16_HeaterSensor.DEGREE >= Z_TEMP59) && (gu16_HotOut_Degree_Ave > gu16_HotOut_ClearMilkHot)){
    		  if((gi16_HeaterSensor.DEGREE >= Z_TEMP62)){	//@cjh241217
     			F_MilkEff_MoreHot = SET;					//		미가열 플래그 set
     		 }
    	  }
    	  else{
//@cjh241217	if(gi16_HeaterSensor.DEGREE >= Z_TEMP58){
    		  if(gi16_HeaterSensor.DEGREE >= Z_TEMP61){		//@cjh241217
    			 F_MilkEff_MoreHot = SET;					//		미가열 플래그 set
    		 }
    	  }
      }
     /* 55도 제어 조건 (끝) */
	
	
     /* 50도 제어 조건 (시작) */
      if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__50)
      {
    	  if(gi16_HotInSensor.DEGREE > Z_TEMP25){
     		 if((gi16_HeaterSensor.DEGREE >= Z_TEMP64) && (gu16_HotOut_Degree_Ave > gu16_HotOut_ClearMilkHot)){
      			F_MilkEff_MoreHot = SET;					//		미가열 플래그 set
      		 }
    	  }
    	  else if(gi16_HotInSensor.DEGREE > Z_TEMP15){
//@cjh241217  if((gi16_HeaterSensor.DEGREE >= Z_TEMP59) && (gu16_HotOut_Degree_Ave > gu16_HotOut_ClearMilkHot)){
    		  if((gi16_HeaterSensor.DEGREE >= Z_TEMP58)){	//@cjh241217
     			F_MilkEff_MoreHot = SET;					//		미가열 플래그 set
     		 }
    	  }
    	  else{
//@cjh241217	if(gi16_HeaterSensor.DEGREE >= Z_TEMP58){
    		  if(gi16_HeaterSensor.DEGREE >= Z_TEMP57){		//@cjh241217
    			 F_MilkEff_MoreHot = SET;					//		미가열 플래그 set
    		 }
    	  }
      }
      /* 50도 제어 조건 (끝) */
	
	if ((F_VoiceOutput_HotCaution == SET) && (gu8_TimerVoiceOutput_HotCaution == 0))
	{
		Play_Voice(VOICE_HOTWATER_EFFLUENT_A);
		F_VoiceOutput_HotCaution = CLEAR;
	}
        /*
	if (gi16_HotOutSensor.DEGREE >= (I16)gu16_TargetDegree)	//	이미 타겟온도보다 높은 경우
	{
		gu8_TimerHeaterControl_AfterEff = 0;		//		추출 후 히터 제어 지연시간 CLEAR
	}
	
	
	else											//	타겟온도보다 낮은 경우
	{
		if (((I16)gu16_TargetDegree - gi16_HotOutSensor.DEGREE) < Z_TEMP20)	//		20도 이내에 들어온 경우
		{
			gu8_TimerHeaterControl_AfterEff = 0;	//		추출 후 히터 제어 지연시간 CLEAR
		}
	}
	*/
	 if (gi16_HotOutSensor.DEGREE >= (I16)gu16_TargetDegree)	//	이미 타겟온도보다 높은 경우
	//if ((gi16_HotOutSensor.DEGREE >= (I16)gu16_TargetDegree)
	//|| (gi16_HeaterSensor.DEGREE >= (I16)gu16_TargetDegree))	//	이미 타겟온도보다 높은 경우
	{
		gu8_TimerHeaterControl_AfterEff = 0;		//		추출 후 히터 제어 지연시간 CLEAR
	}
	else											//	타겟온도보다 낮은 경우
	{
		if ((((I16)gu16_TargetDegree - gi16_HotOutSensor.DEGREE) < Z_TEMP20))	//		20도 이내에 들어온 경우
		//|| (((I16)gu16_TargetDegree - gi16_HeaterSensor.DEGREE) < Z_TEMP20))
		{
			gu8_TimerHeaterControl_AfterEff = 0;	//		추출 후 히터 제어 지연시간 CLEAR
		}
	}
	if (gu8_TimerHeaterControl_AfterEff != 0)		//	x초간 초기 열량으로 고정
	{
		gu16_HeaterWATT = WATT_Ini();				//		초기 열량 계산
	}
	else											//	지연시간 경과 후
	{
		gu16_WATT_forTest = WATT_forTest();			//		필요 WATT계산 모니터용
		if ((gu8_TimerFlowCheck_AfterEff == 0) && (gu16_FlowDecide_W < 10))	//		유량이 0.1LPM 미만 시
		{
			gu16_HeaterWATT = 0;					//			히터 전력값 초기화
		}
		else										//		유량 정상
		{
			if (F_100ms_for_HeaterControl == SET)	//			100ms마다 히터 제어
			{
				F_100ms_for_HeaterControl = CLEAR;	//				100ms Flag CLEAR
				WATT_Calculation();					//				히터 전력값 계산
			}
//			if (F_200ms_for_HeaterControl == SET)	//			100ms마다 히터 제어
//			{
//				F_200ms_for_HeaterControl = CLEAR;	//				100ms Flag CLEAR
//				WATT_Calculation();					//				히터 전력값 계산
//			}
		}
	}

	if ((gu8_WaterSelect != HOT_WATER) || (gu8_OperationStep != Z_OperationStep_50))	//	추출 중이 아닌 경우
	{
//		Ini_HotStep_20();							//		온수선택 대기로 이행
		if (gu8_ModelType_forNational == MODEL_KOREA)
		{
			if ((gi16_StepCount <= 1000) || (gi16_StepCount >= 2800))
			//if((gi16_StepCount <= 500) || (gi16_StepCount >= 2800))
			{
				StepMotor_ZeroBack_Open();
			}
		}
		else
		{
			if ((gi16_StepCount <= 500) || (gi16_StepCount >= 2800))
			{
				StepMotor_ZeroBack_Open();
			}
		}
		Ini_HotStep_36();							//		압빼는 스텝
	}
}
static void Ini_HotStep_35(void)
{
	U8 i = 0;

	gi16_HeaterSensor_Before_forEff = gi16_HeaterSensor.DEGREE;
	F_HeaterLimit = CLEAR;
	
	//cbr_hot_pro
    	F_Heater_Decrease = CLEAR;
	
	HotStepNoSet(Z_HotStepNum_35);
	Effluent_Hz_Decide();							//	추출유량 결정
	gu8_TimerFlowControl_AfterEff = Z_TimerFlowControl_AfterEff;	//	추출 시작 후 유량제어 타이머 set
//	if (gu8_ModelType_forNational == MODEL_KOREA)
//	{
		gu8_TimerHeaterControl_AfterEff = Z_TimerHeaterControl_AfterEff;//	추출 시작 후 히터제어 타이머 set
//	}
//	else
//	{
//		gu8_TimerHeaterControl_AfterEff = Z_TimerHeaterControl_AfterEff_forJapan;//	추출 시작 후 히터제어 타이머 set
//	}
	gu8_TimerHeaterSafety_AfterEff = Z_TimerHeaterSafety_AfterEff;
	if (gu8_ModelType_forNational == MODEL_KOREA)
	{
		gu8_TimerFlowCheck_AfterEff = Z_TimerFlowCheck_AfterEff;		//	추출 시작 후 유량체크 타이머 set
	}
	else
	{
		gu8_TimerFlowCheck_AfterEff = Z_TimerFlowCheck_AfterEff_forJapan;		//	추출 시작 후 유량체크 타이머 set
	}
	gu8_TimerTargetFlow_Change_AfterEff = Z_TimerTargetFlow_Change_AfterEff;	//	추출 시작후 목표유량 변경 타이머 set
	
	
	
	
	gu8_TimerHeaterLimitCheck = Z_TimerHeaterLimitCheck;			//	?s 이내 히터 온도 급상승 체크하여 Limit
	
#ifdef _FOR_DEVELOP_
	gu16_HotEffluentTime = 0;
	gu8_Counter_HeaterOFF = 0;
#endif

	gu16_HeaterWATT = WATT_Ini();					//	초기 WATT 계산
	gi32_Heater_P = 0;								//	P값 초기화

	gi32_Heater_I = (U32)gu16_HeaterFull_WATT * gi16_Heater_Di_ini / 100;	//		I값 초기 계산

	for (i = 0 ; i < 10 ; i++)
	{
		gu16_Flow_Buf[i] = 0;
	}

	gu32_HotOut_Degree_Total = 0;					//		데이터 clr
	gu16_HotOut_Degree_Cnt = 0;						//		데이터 clr


	if ((gu8_ModelType_forNational == MODEL_KOREA) &&
	//cbr_hot_pro
	// && (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR45_JPX)
	// && (gi16_HeaterSensor.DEGREE >= Z_TEMP65))		//	국내향 고온 추출 후 분유 추출 시
	//( ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR45_JPX) && (gi16_HeaterSensor.DEGREE >= Z_TEMP60)) || ( (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__50) && (gi16_HeaterSensor.DEGREE >= Z_TEMP65))))
	( ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR45_JPX) && (gi16_HeaterSensor.DEGREE >= Z_TEMP60)) 
       || ( (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__50) && (gi16_HeaterSensor.DEGREE >= Z_TEMP65))
       || ( (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__55) && (gi16_HeaterSensor.DEGREE >= Z_TEMP68)))                                                                                            )
	
	{
		F_MilkEff_MoreHot = SET;					//		미가열 플래그 set
	}
	else
	{
		F_MilkEff_MoreHot = CLEAR;
	}

	if (F_PreHeatingVoice_forJapan == SET)
	{
		Play_Voice(VOICE_HOTWATER_EFFLUENT_A);
	}

//	gu8_TimerHotEff_CautionVoice = Z_TimerHotEff_CautionVoice;
//220405	F_LatentHeat_Clear = CLEAR;

	if ((F_VoiceOutput_HotCaution == SET) && (gu8_TimerVoiceOutput_HotCaution == 0))
	{
		Play_Voice(VOICE_HOTWATER_EFFLUENT_A);
		F_VoiceOutput_HotCaution = CLEAR;
	}

	gi16_HotOutSensor_DegreeAverage = 0;	//@007
	gi16_HeaterSensor_DegreeAverage = 0;	//@007
	gi16_HotOutSensor_DegreeSum = 0;		//@007
	gi16_HeaterSensor_DegreeSum = 0;		//@007
	gu8_CountDegree_Average= 0;				//@007
	gu8_FlowTarget_Decrease = 0;			//@007
	gu8_TimerTargetFlow_Decrease = Z_TimerTargetFlow_Decrease_Start;	//@007
	
	gu8_F_RelayOff_Delay = CLEAR;
}

/// @brief    추출 후 압뺴기 스텝 (입수 먼저 닫고 100ms 후 출수 닫음)
/// @param    void
/// @return   void
static void HotStep_36(void)
{
	if (gu16_TimerHotStep_10ms == 0)				//	100ms 경과 후
	{
		Ini_HotStep_20();							//		온수대기 이행
	}
}
static void Ini_HotStep_36(void)
{
#ifdef _HOT_TEST_
	gu8_TimerHotTest_DegreeDisplay = Z_TimerHotTest_DegreeDisplay;
#else
	if (F_HotWater_DegreeDisplay == SET)
	{
		gu8_TimerHotTest_DegreeDisplay = Z_TimerHotTest_DegreeDisplay;
	}
#endif
//cbr_hot_pro
#ifdef _EFF_Continue_Heater_OFF
    	F_Eff_Continue_Heater_Off = CLEAR;                  //  연속 추출시 히터 Off Flag Clear (히터 정상 동작 지시)
#endif
	gu16_TimerHotStep_10ms = Z_TimerHotStep_10ms_100ms;
	HotStepNoSet(Z_HotStepNum_36);
}

///// @brief    유량 리캘리브레이션
///// @param    void
///// @return   void
//static void HotStep_3E(void)
//{
//	if (gu8_Timer_ReCalibration == 0)				//	리캘리브레이션 타이머 경과 후
//	{
//		F_StartFlow_ReCalibration = CLEAR;			//		리캘리브레이션 필요 flag clear
//		StepMotor_ZeroBack_Open();					//		초기화
//		Ini_HotStep_10();
//	}
//}
//static void Ini_HotStep_3E(void)
//{
//	HotStepNoSet(Z_HotStepNum_3E);
//	gu8_Timer_ReCalibration = Z_Timer_ReCalibration;	//	리캘리브레이션 타이머 set
////	FlowControl_StepMove(STEP_MOTER_CLOSE, Z_StepDown_forStepByFlowCheck);	//	스텝 by 유량 체크를 위함 (3000step 풀 오픈은 데이터가 정확하지 않음)
//	FlowControl_StepTarget((3000-Z_StepDown_forStepByFlowCheck_KOR));
//}

/// @brief    온수스텝 No set
/// @param    void
/// @return   void
static void HotStepNoSet(U8 data)
{
	static const U8 DT_HotNumber[] =
	{
		Z_HotStep_00,
		Z_HotStep_01,
		Z_HotStep_02,
		Z_HotStep_10,
		Z_HotStep_20,
		Z_HotStep_2F,
		Z_HotStep_31,
		Z_HotStep_32,
		Z_HotStep_33,
		Z_HotStep_34,
		Z_HotStep_35,
		Z_HotStep_36,
//		Z_HotStep_3E,
	};
	/************************************************/
	gu8_HotStepNum = data; 							/*	일련 번호 set*/

	gu8_HotStep = DT_HotNumber[gu8_HotStepNum];		/*	상태 mode No. set*/
}

/// @brief    온수 유량 확정
/// @param    void
/// @return   void

U16 Test1_mu16_ret, Test2_mu16_ret;
U32 Test3_HeaterFull_WATT;
U32 Test3_Delta_temp;
U16 StartFlow_Decide(void)
{
	U32 mu32_temp1 = 0;
	U32 mu32_temp2 = 0;
	U32 mu32_temp3 = 0;
	U32 mu32_temp4 = 0;
	U32 mu32_temp5 = 0;
	U16 mu16_ret = 0;
	
	static U8 mu8_FlowDown_Save = 0;
	U8 mu8_FlowDown = 0;
	U16 buf = 0;
        Test1_mu16_ret=0, Test2_mu16_ret=0;
	Test3_Delta_temp=0;
	Test3_HeaterFull_WATT=0;
	if (gu16_HeaterFull_WATT != 0)					//	히터 최대 전력이 0이 아닌 경우 (안전처리)
	{
		mu32_temp5 = gu16_HeaterFull_WATT;			//		최대 전력치 저장
	}
	else											//	안전처리
	{
		if (gu8_ModelType_forNational == MODEL_KOREA)
		{
			mu32_temp5 = HEATER_FULL_WATT_KOR;			//		최대 전력치 저장
		}
		else
		{
			mu32_temp5 = HEATER_FULL_WATT_JP;			//		최대 전력치 저장
		}
	}

	mu32_temp1 = (U32)HEATER_EFFICIENCY * 860;		//	히터 효율 x (WATT to Kcal)
	mu32_temp2 = (U32)(gu16_TargetDegree - gi16_HotInSensor.DEGREE);	//	목표온도 - 입수온도센서 Delta
	gi16_HotInSensor_BackUp_forFlowDecide = gi16_HotInSensor.DEGREE;
	mu32_temp3 = (mu32_temp5 * 10) / mu32_temp2;	//	최대전력치 / 온도 Delta
	mu32_temp2 = mu32_temp1 * mu32_temp3;			//
	mu32_temp4 = mu32_temp2 / 60 / 1000;			//	초기 목표유량 확정	[0.01LPM]

	mu16_ret = (U16)mu32_temp4;
	Test1_mu16_ret = mu16_ret;
	
	
	Test3_Delta_temp=(U32)(gu16_TargetDegree - gi16_HotInSensor.DEGREE);
	Test3_HeaterFull_WATT = mu32_temp5;
//	/* [200227 추가]
//	 * 신뢰성 시료 3대에서 198V 10℃에서 온수온도가 1~3도 미달 되는 내용이 있어서 추가. 200227 서재식님께 발신 메일 참고
//	 * 디버깅 결과, 위 계산 치 유량으로는 열량 한계가 있음
//	 * 2120W~2720W 비례적으로 0.00~0.05LPM 목표유량 감산
//	 */
//	if (HEATER_FULL_WATT_KOR > gu16_HeaterFull_WATT)	//	상기 주석 확인
//	{
//		mu32_temp1 = HEATER_FULL_WATT_KOR - gu16_HeaterFull_WATT;
//		mu32_temp1 *= 100;
//		mu32_temp2 = mu32_temp1 / 600;
//		mu32_temp3 = mu32_temp2 * 5;
//		mu32_temp3 = mu32_temp3 / 100;
//		if (mu32_temp3 > 5)							//	맥시멈 제한
//		{
//			mu32_temp3 = 5;
//		}
//		mu16_ret = (U16)(mu16_ret - (U16)mu32_temp3);
//	}

	/*	전잔 온수 온도 안나올 경우 시작유량 감소	*/
//	if (gu8_HotTempSetup == HOT_TEMP_SETUP_90)
//	{
//		if ((gu16_HotOut_Degree_Ave_Coffee != 0) && (gu16_HotOut_Degree_Ave_Coffee < Z_TEMP87) && (F_FirstHotEff_Detect == CLEAR))
//		{
//			mu8_FlowDown = (U8)((Z_TEMP87 - gu16_HotOut_Degree_Ave_Coffee) / 10);
//			if (mu8_FlowDown > 5)					//		0.5LPM 제한
//			{
//				mu8_FlowDown = 5;
//			}
////			mu8_FlowDown_Save += mu8_FlowDown;
////			if (mu8_FlowDown_Save > 8)				//		0.1LPM 제한
////			{
////				mu8_FlowDown_Save = 8;
////			}
//
//			mu16_ret -= mu8_FlowDown;
//		}
//		else
//		{
//			mu8_FlowDown = 0;
//			mu8_FlowDown_Save = 0;
//		}
//	}

//	if ((gu16_FlowDecide_W_HotEffMax != 0) && (mu16_ret > gu16_FlowDecide_W_HotEffMax))
//	{
//		mu16_ret = gu16_FlowDecide_W_HotEffMax;
//	}

//	if (mu16_ret > 	gu16_FlowDecide_W_Ave_Pure_Standard)
//	{
//		mu16_ret = 	gu16_FlowDecide_W_Ave_Pure_Standard;
//	}

	/*	일본향 90도 반컵 유량 증대 계산	*/
//#ifndef _FOR_JAPAN_90DEGREE_CONTINUE
//	if ((gu8_ModelType_forNational == MODEL_JAPAN)
//	&& (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)
//	&& (gu8_CupSize == CUPSIZE_HALFCUP))			//	일본향의 경우 반컵 추출유량 증대를 위하여
//	{
//		buf = (U16)((U32)(115) * 10000 / gu16_FlowMeter_Standard);	//
//		if (gu16_Effluent_Hz_Amount < buf)
//		{
//			mu16_ret += (22 + ((gi16_HotInSensor.DEGREE-Z_TEMP20)/10));
//		}
//	}
//#endif
	/*	끝	*/

#ifdef _HEATER_CURRENT_CHECK_
	mu16_ret += 20;
#endif

	if (mu16_ret > 100)
	{
		mu16_ret = 100;
	}
	
	/*
	if (gu8_HotStep == Z_HotStep_35)					//@007
	{
		if ((gu8_FlowTarget_Decrease != 0) && (mu16_ret > gu8_FlowTarget_Decrease))	//@007	순간온수 100도 유량감소
		{
			mu16_ret -= gu8_FlowTarget_Decrease;		//@007
		}
	}
	*/
	if ((gu8_HotStep >= Z_HotStep_33) || (gu8_HotStep <= Z_HotStep_35))	//추출전후 타겟유량 가변안되도록	
	//if (gu8_HotStep == Z_HotStep_35)
	{
		// if ((gu8_FlowTarget_Decrease != 0) && (mu16_ret > gu8_FlowTarget_Decrease))	//	순간온수 100도 유량감소
		//// {
		// 	mu16_ret -= gu8_FlowTarget_Decrease;		
		// }
		/*//cbr_hot_pro*/
		if((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__55) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__65))
		//if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__65)	//cbr_test 2025-11-20
        	{
		            //if((gu16_TargetDegree > gi16_HotOutSensor_DegreeAverage) || (gu16_TargetDegree > gi16_HeaterSensor_DegreeAverage))
			    if(gu16_TargetDegree > gi16_HotOutSensor_DegreeAverage)
		            {
		                if(gu8_Timer_High_Flow_w == CLEAR)
		                {
		                    gu8_Timer_High_Flow_w = 10;

		                    if(gu8_FlowTarget_Decrease <= 20)
		                    {
		                        if(gu16_FlowTarget_W >= 53)
		                        {
		                            gu8_FlowTarget_Decrease ++;
		                        }
		                    }
		                }
		            }
		            
		            if(gu8_FlowTarget_Decrease != 0)
		            {
		                mu16_ret -= gu8_FlowTarget_Decrease;

		                if((gi16_HotOutSensor_DegreeAverage >= (gu16_TargetDegree + Z_TEMP2)) || (gi16_HeaterSensor_DegreeAverage >= (gu16_TargetDegree + Z_TEMP2)))
		                {
		                    gu8_FlowTarget_Decrease = 0;
		                }
			    }
		}
		else if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__60)
		// if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__60)
		{
			if(gi16_HotInSensor.DEGREE > Z_TEMP15)
			{
				
		                mu16_ret += 2;
		                /* 기존 유량 0.8LPM → 0.82LPM으로 변경(증가) 250123 CH.PARK */
				if (mu16_ret > 82)
				{
					mu16_ret = 82;
				}
			}
			else
			{
				if(F_FirstHotEff_Detect == SET)
				{
					mu16_ret -= 5;

					if (mu16_ret > 55)
					{
						mu16_ret = 55;
					}
				}
				else
				{
					if (mu16_ret > 63)
					{
						mu16_ret = 63;
					}
				}
			}
		}
		else if((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__95)|| (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__90))
		{         
				//origal
				/*
				if(mu16_ret <= 40)
				{
				 	mu16_ret = 40;
				}
		                if ((gu8_FlowTarget_Decrease != 0) && (mu16_ret > gu8_FlowTarget_Decrease))	//	순간온수 100도 유량감소
		                {
		                    mu16_ret -= gu8_FlowTarget_Decrease;
		                }
		                */
				
				//Pro
				/*
				 if(gi16_HotInSensor.DEGREE <= Z_TEMP15 )
			{
                if(F_FirstHotEff_Detect == SET)
                {
                    mu16_ret = mu16_ret - 2;

                    if(mu16_ret <= 40)
                    {
                        mu16_ret = 40;
                    }
                }
                else
                {
                    if ((gu8_FlowTarget_Decrease != 0) && (mu16_ret > gu8_FlowTarget_Decrease))	//	순간온수 100도 유량감소
                    {
                        mu16_ret -= gu8_FlowTarget_Decrease;
                    }
                }
            }
            else
            {
                mu16_ret = mu16_ret - 2;

                if(mu16_ret <= 40)
                {
                    mu16_ret = 40;
                }
                // if ((gu8_FlowTarget_Decrease != 0) && (mu16_ret > gu8_FlowTarget_Decrease))	//	순간온수 100도 유량감소
                // {
                //     mu16_ret -= gu8_FlowTarget_Decrease;
                // }
            }
				*/
				
			
				
				
				if(gi16_HotInSensor.DEGREE <= Z_TEMP15 )
				//if(gi16_HotInSensor.DEGREE <= Z_TEMP25 )
				{
			                //if(F_FirstHotEff_Detect == SET)
					if(F_2ndHot_Effluent_HeaterDown == SET)
			                {
			                    //mu16_ret = mu16_ret - 2;
						mu16_ret = mu16_ret - 3;
			                    if(mu16_ret <= 40)
			                    {
			                        mu16_ret = 40;
			                    }
			                }
			                else
			                {   
			                    //if ((gu8_FlowTarget_Decrease != 0) && (mu16_ret > gu8_FlowTarget_Decrease))	//	순간온수 100도 유량감소
			                    //{
			                    //    mu16_ret -= (gu8_FlowTarget_Decrease+5);
			                    //}
					    //else 
					    //{
						    
						 mu16_ret -= 6;   
					    //}
			                }
		            	}
		                else
		               {
			              
			                   // mu16_ret = mu16_ret - 2;
					   mu16_ret = mu16_ret - 3;
					   //mu16_ret = mu16_ret - 5; //////20260403
						
			                    if(mu16_ret <= 40)
			                    {
			                        mu16_ret = 40;
			                    }
			               
		               }
		}
					    
		else if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__75)		//@cjh
		{														//@cjh
			if(gi16_HotInSensor.DEGREE <= Z_TEMP15 )
			{
				mu16_ret -= 8;
			}

			if(gi16_HotInSensor.DEGREE > Z_TEMP25 )
			{
				if((gi16_HeaterSensor.DEGREE >= Z_TEMP82) || (gi16_HotOutSensor.DEGREE >= Z_TEMP80 ))
				{
//@cjh241217		mu16_ret += 2;
					mu16_ret += 3;		//@cjh241217
				}
			}

			if(gi16_HotInSensor.DEGREE <= Z_TEMP25 )
			{
				if(mu16_ret > 65)
				{								//@cjh
					mu16_ret = 65;								//@cjh
				}
			}
			else
			{
				if(mu16_ret > 73)
				{								//@cjh
					mu16_ret = 73;								//@cjh
				}
			}

			if(F_FirstHotEff_Detect == SET)
			{					//@cjh
//@cjh241217	mu16_ret -= 3;									//@cjh
				mu16_ret -= 7;									//@cjh241217
			}
		}														//@cjh
		else if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__50)
		{
			if(gi16_HeaterSensor.DEGREE >= Z_TEMP59)
			{
				mu16_ret += 2;
			}

			if(gi16_HotInSensor.DEGREE <= Z_TEMP15 )
			{
//@cjh241217	if(mu16_ret > 80)
				if(mu16_ret > 55)											//@cjh241217
				{
//@cjh241217		mu16_ret = 80;										//@cjh
					mu16_ret = 55; //0.55LPM											//@cjh241217
				}

                /* 온수 첫잔인 경우 */
				if(F_FirstHotEff_Detect == SET)
				{							//@cjh241217
					// mu16_ret -= 5;										//@cjh241217

		                    /* 0.2 기존 0.05LPM 유량감소 → 변경 0.07LPM 유량감소 250123 CH.PARK */
		                    mu16_ret -= 7;

		                    /* 유량 하한치 설정 0.5LPM 250123 CH.PARK */
		                    if(mu16_ret < 50)
		                    {
		                        mu16_ret = 50;      // 0.5LPM
		                    }
				}															//@cjh241217
				else{														//@cjh241217
																			//@cjh241217
				}
			}
			else if(gi16_HotInSensor.DEGREE <= Z_TEMP25)
            		{
				if(mu16_ret > 58)										//@cjh241217
				{														//@cjh241217
					mu16_ret = 58;										//@cjh241217
				}														//@cjh241217

				if(F_FirstHotEff_Detect == SET){						//@cjh241217
					mu16_ret -= 5;										//@cjh241217
				}														//@cjh241217
				else{													//@cjh241217
																		//@cjh241217
				}														//@cjh241217
			}
			else
            		{
				 if(mu16_ret > 72)
				 {
				 	mu16_ret = 72;									//@cjh
				 }
			}

		}
		else if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__80)
		{			//@cjh241217
			if(F_FirstHotEff_Detect == SET)
			{							//@cjh241217
				// mu16_ret -= 4;											//@cjh241217
				
		                if((gi16_HotInSensor.DEGREE <= Z_TEMP25) && (gi16_HotInSensor.DEGREE >= Z_TEMP15))
		                
		                {
		                    /* 기존 유량 0.04LPM 감소 → 0.06LPM 감소로 수정 250123 CH.PARK */
		                    mu16_ret -= 8;	 
		                }
		                else
		                {
		                    /* 기존 유량 0.04LPM 감소 → 0.06LPM 감소로 수정 250123 CH.PARK */
		                    mu16_ret -= 6;	                    
		                }
			}															//@cjh241217
			else
			{														//@cjh241217
				   mu16_ret -= 10;														//@cjh241217
			}															//@cjh241217
		}	
	         else if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR85_JP80)
	         {			
		 
		 	if(F_FirstHotEff_Detect == SET)
		            {
		                mu16_ret -= 3;
		            }	
		
	        }	
	        else if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR45_JPX)
	        {			//@cjh241217
	            //if((gi16_HotInSensor.DEGREE < Z_TEMP25))
	            if(gi16_HotInSensor.DEGREE < Z_TEMP15)
				// || (gu8_Heater_PowerLevel != Z_HEATER_POWER_LEVEL_HIGH))
	            {
	                 if(mu16_ret >= 70)
	                 {
	                     mu16_ret = 70;
	                 }
	               
	            }
				
	        }
		else if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR70_JP70)		//cbr_test 2025-10-01
	        {
	            if(gi16_HotInSensor.DEGREE < Z_TEMP15)
				// && (gu8_Heater_PowerLevel == Z_HEATER_POWER_LEVEL_LOW))		//cbr_test 2025-11-25 10도 정격 온도 과함.
	            {
					// if(F_FirstHotEff_Detect == SET)	//cbr_test 2025-12-03
					// {
					// 	mu16_ret -= 3;
					// }	
					// mu16_ret += 5;
					// mu16_ret -= 5;
	                if(mu16_ret <= 40)
	                {
	                    mu16_ret = 40;
	                }
					// if(mu16_ret >= 70)
					// {
					// 	mu16_ret = 70;
					// }
	            }
				else
				{
					if(mu16_ret >= 65)
					{
						mu16_ret = 65;
					}
				}
	        }																																																	//@cjh241217
		else
		{
	            if ((gu8_FlowTarget_Decrease != 0) && (mu16_ret > gu8_FlowTarget_Decrease))	//@007	¼ø°?¿?¼? 100?? ??·®°¨¼?
	            {
	                mu16_ret -= gu8_FlowTarget_Decrease;		//@007
	            }
		}
			/*//cbr_hot_pro*/
	}
	Test2_mu16_ret =mu16_ret; 
	return (mu16_ret);
}

/// @brief    추출 시작 시점 스테핑모터 위치 결정
/// @param    void
/// @return   void
I16 Test_mi16_Target_Step;
void StartFlow_StepMotorControl(void)
{
	U16 mu16_Flow_Target_Before_Delta = 0;
	I16 mi16_Close_Step = 0;
	I16 mi16_Target_Step = 0;
	I16 mi16_Target_Step_LowLimit = 0;
	I16 mi16_Target_Step_HighLimit = 0;
	Test_mi16_Target_Step=0;

	if ((gu8_ModelType_forNational == MODEL_KOREA) && (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR45_JPX))
	{
		FlowControl_StepTarget(2800);
	}
	else if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__50)
	{
		FlowControl_StepTarget(2800);
	}
	else if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__55)
	{
		FlowControl_StepTarget(2800);
	}
	else if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__60)
	{
		FlowControl_StepTarget(2600);
	}
	else if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__75)
	{
		FlowControl_StepTarget(2200);
	}
	else if (gu16_StepCount_BeforeEff == 0)			//	이전 추출 스텝 데이터가 없는 경우
	{
		FlowControl_StepTarget(3000-Z_StepDown_forStepByFlowCheck_KOR);
	}

	else											//	이전 추출 스텝 데이터가 있는 경우
	{
		if (gu8_ModelType_forNational == MODEL_KOREA)
		{
			mi16_Target_Step_HighLimit = 2600;
			mi16_Target_Step_LowLimit = 1400;
			/*
			if((gi16_HotInSensor.DEGREE < Z_TEMP25) && (gu8_Heater_PowerLevel != Z_HEATER_POWER_LEVEL_HIGH))
			{
				mi16_Target_Step_LowLimit = 100;		//cbr_test 2025-11-28 저온 고유량 시 왈칵
			}
			else
			{
				mi16_Target_Step_LowLimit = 1400;
			}
			*/
		}
		else										//	일본향
		{
			mi16_Target_Step_HighLimit = 2600;
			mi16_Target_Step_LowLimit = 500;
		}


		if (gu16_FlowTarget_W > gu16_FlowDecide_W_BeforeEff)	//		목표유량이 이전추출 유량보다 높은경우
		{
			mu16_Flow_Target_Before_Delta = gu16_FlowTarget_W - gu16_FlowDecide_W_BeforeEff;
			mi16_Close_Step = (I16)((I16)(ZERO_BACK-gu16_StepCount_BeforeEff) - (30*mu16_Flow_Target_Before_Delta));	//		CLOSE할 스텝 연산
			mi16_Target_Step = ZERO_BACK-mi16_Close_Step;
		}
		else
		{
			mu16_Flow_Target_Before_Delta = gu16_FlowDecide_W_BeforeEff - gu16_FlowTarget_W;
			mi16_Close_Step = (I16)((I16)(ZERO_BACK-gu16_StepCount_BeforeEff) + (30*mu16_Flow_Target_Before_Delta));	//		CLOSE할 스텝 연산
			mi16_Target_Step = ZERO_BACK-mi16_Close_Step;
		}

		if (mi16_Target_Step < mi16_Target_Step_LowLimit)			//		최소치 제한
		{
			mi16_Target_Step = mi16_Target_Step_LowLimit;
		}
		else if (mi16_Target_Step > mi16_Target_Step_HighLimit)
		{
			mi16_Target_Step = mi16_Target_Step_HighLimit;
		}
		else
		{

		}
		Test_mi16_Target_Step = mi16_Target_Step;
		FlowControl_StepTarget(mi16_Target_Step);  
	}
}

/// @brief    초기 전류치 계산
/// @param    void
/// @return   void
U16 WATT_Ini(void)
{
	U32 mu32_work1 = 0;
	U32 mu32_work2 = 0;
	U32 mu32_work3 = 0;
	U32 mu32_work4 = 0;
	U16 mu16_ret = 0;

	mu32_work2 = (U32)gu16_TargetDegree - gi16_HotInSensor.DEGREE;	//	목표온도 - 입수온도. Delta T	[1/10℃]
	mu32_work3 = mu32_work2 * 10 / HEATER_EFFICIENCY;		//	Delta T x 히터효율
	mu32_work1 = (U32)gu16_FlowTarget_W * 60;			//	목표유량 LPM 계산
	mu32_work4 = mu32_work1 * mu32_work3;				//	유량 x 온도. kcal 계산
	mu32_work1 = mu32_work4 / 860;					//	kcal → WATT

	mu16_ret = (U16)(mu32_work1 * 10);				//	[1W]

//	if ((gu8_ModelType_forNational == MODEL_JAPAN) && (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90))
//	{
//		mu16_ret = gu16_HeaterFull_WATT;
//	}

	return (mu16_ret);
}

/// @brief    필요전력이 얼마인지 모니터링용
/// @param    void
/// @return   void
U16 WATT_forTest(void)
{
	U32 mu32_work1 = 0;
	U32 mu32_work2 = 0;
	U32 mu32_work3 = 0;
	U32 mu32_work4 = 0;
	U16 mu16_ret = 0;

	mu32_work2 = (U32)gu16_TargetDegree - gi16_HotInSensor.DEGREE;	//	목표온도 - 입수온도. Delta T	[1/10℃]
	mu32_work3 = mu32_work2 * 10 / HEATER_EFFICIENCY;			//	Delta T x 히터효율
	mu32_work1 = (U32)gu16_FlowDecide_W * 60;					//  LPM 계산
	mu32_work4 = mu32_work1 * mu32_work3;						//	유량 x 온도. kcal 계산
	mu32_work1 = mu32_work4 / 860;								//	kcal → WATT

	mu16_ret = (U16)(mu32_work1 * 10);							//	[1W]

	return (mu16_ret);
}

///// @brief
///// @param    void
///// @return   void
//U16 HeaterCurrent_Decide(void)
//{
//	U32 mu32_work1 = 0;
//	U16 mu16_worr2 = 0;
//	U32 mu32_work3 = 0;
//	U32 mu32_work4 = 0;
//	U16 ret = 0;
//
//	if (gu16_TargetDegree > gi16_HotOutSensor)
//	{
//		mu32_work1 = (U32)gu16_TargetDegree - gi16_HotOutSensor.DEGREE;
//		if (mu32_work1 > 200)
//		{
//			mu32_work1 = 200;
//		}
//		mu16_worr2 = mu32_work1 / 10;
//		ret = gu16_HeaterWATT + mu16_worr2;
//		if (ret > gu16_HeaterFull_WATT)
//		{
//			ret = gu16_HeaterFull_WATT;
//		}
//	}
//	else
//	{
//		mu32_work1 = (U32)gi16_HotOutSensor.DEGREE - gu16_TargetDegree;
//		if (mu32_work1 > 200)
//		{
//			mu32_work1 = 200;
//		}
//		mu16_worr2 = mu32_work1 / 10;
//		ret = gu16_HeaterWATT - mu16_worr2;
//		if (ret > gu16_HeaterFull_WATT)				//	오버플로우
//		{
//			ret = 0;
//		}
//	}
//
//	return (ret);
//}

/// @brief    유량 급격 감소 체크 (스팀 방지)
/// @param    void
/// @return   void
static void Flow_Rapidly_Decrease_Check(void)
{
	U8 i = 0;

	if (gu8_TimerFlowCheck_AfterEff == 0)
	{
		for (i = 0 ; i < 10 ; i++)
		{
			if ((gu16_Flow_Buf[i] >= Z_Flow_Rapidly_Decrease)
			&& (gu16_FlowDecide_W >= Z_Flow_Rapidly_Decrease)
			&& (gu16_FlowDecide_W < (gu16_FlowTarget_W-Z_Flow_Rapidly_Decrease))
			&& (gu16_FlowDecide_W < (gu16_Flow_Buf[i]-Z_Flow_Rapidly_Decrease)))	//		타겟유량이랑 차이가 나고 0.2리터 이상 급감 하는 경우
			{
				F_Flow_Rapidly_Decrease = SET;	//		유량 급감소 확정
				break;
			}
			else
			{
				if (gi16_HeaterSensor.DEGREE < Z_TEMP90)	//		히터온도가 90도 미만으로 떨어질 경우
				{
					F_Flow_Rapidly_Decrease = CLEAR;	//			유량 급감소 해지
				}
			}
		}
	}

	for (i = 1 ; i < 10 ; i++)
	{
		gu16_Flow_Buf[i] = gu16_Flow_Buf[i-1];
	}
	gu16_Flow_Buf[0] = gu16_FlowDecide_W;
}

/// @brief    100도 유량감소 필요 체크 (온수 추출 시 100ms마다 호출 됨) @007
/// @param    void
/// @return   void
static void HotTemp100_FlowTargetDecrease_Check(void)
{
	if (gu8_TimerTargetFlow_Decrease == 0)			//	유량 변경 후 지연시간 도달 시
	{
		gi16_HotOutSensor_DegreeSum += gi16_HotOutSensor.DEGREE;
		gi16_HeaterSensor_DegreeSum += gi16_HeaterSensor.DEGREE;

		if (++gu8_CountDegree_Average >= 10)		//		온도 평균 카운트 횟수 도달 시
		{
			gi16_HotOutSensor_DegreeAverage = gi16_HotOutSensor_DegreeSum / gu8_CountDegree_Average;
			gi16_HeaterSensor_DegreeAverage = gi16_HeaterSensor_DegreeSum / gu8_CountDegree_Average;
			gi16_HotOutSensor_DegreeSum = 0;
			gi16_HeaterSensor_DegreeSum = 0;
			gu8_CountDegree_Average = 0;

			if (((abs(gi16_HotOutSensor_DegreeAverage - gi16_HotOutSensor.DEGREE)) <= Z_TEMP1_5)
			&& ((abs(gi16_HeaterSensor_DegreeAverage - gi16_HeaterSensor.DEGREE)) <= Z_TEMP1_5))	//	온도 안정화 체크
			{       /*
				if (gi16_HotOutSensor_DegreeAverage < (gu16_TargetDegree-Z_TEMP1_5))	//	온수 출수온도 평균이 타겟 온도에 X℃ 도달 못하는 경우
				{
					gu8_FlowTarget_Decrease++;
					gu8_TimerTargetFlow_Decrease = Z_TimerTargetFlow_Decrease_Ing;
				}
				*/
				 if((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)  || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__95) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__90))
                		{
					//if ((gi16_HotOutSensor_DegreeAverage < (gu16_TargetDegree-Z_TEMP1_5))	//	온수 출수온도 평균이 타겟 온도에 X℃ 도달 못하는 경우
					//|| (gi16_HeaterSensor_DegreeAverage < (gu16_TargetDegree-Z_TEMP1_5)))	//	온수 출수온도 평균이 타겟 온도에 X℃ 도달 못하는 경우
					if (gi16_HotOutSensor_DegreeAverage < (gu16_TargetDegree-Z_TEMP1_5))	//	온수 출수온도 평균이 타겟 온도에 X℃ 도달 못하는 경우
					{
						gu8_FlowTarget_Decrease++;
						gu8_TimerTargetFlow_Decrease = Z_TimerTargetFlow_Decrease_Ing;
					}
				}
				
				
			}
		}
	}
}

/// @brief    WATT 계산
/// @param    void
/// @return   void
static void WATT_Calculation(void)
{
	U16 mu16_buff = 0;
	/************************************************/
	gi32_Heater_P = WATT_Calculation_P();			//	P값 계산
	gi32_Heater_I = WATT_Calculation_I();			//	I값 계산

	/*	P+I 값 설정	*/
	gi32_Heater_P_I = gi32_Heater_P + gi32_Heater_I;//	P+I

	if (gi32_Heater_P_I >= gu16_HeaterFull_WATT)	//	결정값이 최대출력값 보다 높을 경우
	{
		gi32_Heater_P_I = gu16_HeaterFull_WATT;		//		최대값 제한
	}
	else if (gi32_Heater_P_I < 0)					//	이상 계산일 경우
	{
		gi32_Heater_P_I = 0;						//		최소값 제한
	}
	else
	{

	}

	if (gi32_Heater_P_I < 0)						//	PI 값이 마이너스인 경우
	{
		gu16_HeaterWATT = 0;						//		전력값 0으로 고정
	}
	else											//	양수인 경우
	{
		gu16_HeaterWATT = (U16)gi32_Heater_P_I;		//		PI값으로 갱신
	}
	
	/*//cbr_hot_pro*/
    if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__55)
    {
        if((gu8_Heater_PowerLevel == Z_HEATER_POWER_LEVEL_HIGH) || (gu8_Heater_PowerLevel == Z_HEATER_POWER_LEVEL_RATED)
                || ((gu8_Heater_PowerLevel == Z_HEATER_POWER_LEVEL_LOW) && (gi16_HotInSensor.DEGREE > Z_TEMP18)))
        {
            if((gu8_Heater_PowerLevel == Z_HEATER_POWER_LEVEL_HIGH) && (gi16_HotInSensor.DEGREE > Z_TEMP25))
            {
                if(gi16_HeaterSensor.DEGREE >= Z_TEMP66)
                {
                    gu8_F_RelayOff_Delay = SET;
                }
                if(gu8_F_RelayOff_Delay == SET)
                {
                    mu16_buff = (U16)(((U32)gu16_HeaterWATT * (U32)40) / 100);
                    gu16_HeaterWATT = gu16_HeaterWATT - mu16_buff;

                    if(gi16_HeaterSensor.DEGREE <= Z_TEMP61)
                    {
                        gu8_F_RelayOff_Delay = CLEAR;
                    }
                }
            }
            else
            {
                if(gi16_HeaterSensor.DEGREE >= Z_TEMP66)
                {
                    gu8_F_RelayOff_Delay = SET;
                }

				if(gu8_F_RelayOff_Delay == SET)
				{
					if(gi16_HeaterSensor.DEGREE <= Z_TEMP61)
					{
						gu8_F_RelayOff_Delay = CLEAR;
					}
				}
			}
		}
	}
//	else if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__60)
//	{
//		if(gi16_HotInSensor.DEGREE < Z_TEMP15)		//	입수온 저온, 고전압일 경우....타겟온도는 같지만....전체적으로 열량 낮춰 제어
//		{
//			if(gu8_Heater_PowerLevel == Z_HEATER_POWER_LEVEL_HIGH)
//			{
//				mu16_buff = (U16)(((U32)gu16_HeaterWATT * (U32)8) / 100);
//				gu16_HeaterWATT = gu16_HeaterWATT - mu16_buff;
//			}
//		}
//	}
	else if(gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__65)
	{
		if(gi16_HotInSensor.DEGREE <= Z_TEMP15)
		{
			if(gu8_Heater_PowerLevel != Z_HEATER_POWER_LEVEL_LOW)
			{
				if(gi16_HeaterSensor.DEGREE >= Z_TEMP79)
				//if(gi16_HeaterSensor.DEGREE >= Z_TEMP76)		//cbr 2025-12-04
				{
					gu16_HeaterWATT = gu16_HeaterRELAY_WATT;
				}
			}
		}
		else if(gi16_HotInSensor.DEGREE > Z_TEMP25)
		{
			// if(gi16_HeaterSensor.DEGREE >= Z_TEMP65)		//30도는 릴레이만 켜도 과열
			// {
			// 	gu16_HeaterWATT = gu16_HeaterRELAY_WATT;
			// }
			if(gu8_Heater_PowerLevel == Z_HEATER_POWER_LEVEL_HIGH)
			{
				if(gi16_HeaterSensor.DEGREE >= Z_TEMP79)
				{
					gu16_HeaterWATT = 0;
				}
			}
		}
		else
		{
			if(gi16_HeaterSensor.DEGREE >= Z_TEMP74)
			// if(gi16_HeaterSensor.DEGREE >= Z_TEMP68)
			{
				gu16_HeaterWATT = gu16_HeaterRELAY_WATT;
			}
			else
			{

			}
		}
	}
	else
	{

    }/*//cbr_hot_pro*/
}

/// @brief    히터 P값 연산
/// @param    void
/// @return   void
static I32 WATT_Calculation_P(void)
{
	I16	mi16_diff;									//	Tset'－Tout buffer
	I32	mi32_DP;									//	DP buffer
	I32	mi32_max, mi32_min;
	I32	mi32_max_work;								//	max buf

	/*	P 값 설정	*/
	mi16_diff = (I16)gu16_TargetDegree - gi16_HotOutSensor.DEGREE;		//		Tset - Tout				[1/10:℃]
	//mi16_diff = (I16)gu16_TargetDegree - gi16_HeaterSensor.DEGREE;
	if (mi16_diff >= Z_PI_Control_LimitTemp)
	{												//		차이가 30.0℃ 이상 차이 있는 경우
		mi16_diff = Z_PI_Control_LimitTemp;			//			max(30.0℃)의 제한
	}
	else if (mi16_diff <= -Z_PI_Control_LimitTemp)
	{												//		차이가 -30.0℃ 이하인 경우
		mi16_diff = -Z_PI_Control_LimitTemp;		//			min(-30.0℃)의 제한
	}
	else
	{
		/* ... */
	}

//	if ((gu8_ModelType_forNational == MODEL_KOREA)
//	&& (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)
//	&& (gu16_TargetDegree <= (gi16_HotOutSensor.DEGREE+Z_TEMP2)))	//	100도 고온수 추출 중 타겟 온도보다 2도 넘는 경우 DP 두배 (100도 부근에서 정체되면서 I값이 너무 감소되는 것을 방지하면서 P로 보상)
//	{
//		gu16_Heater_Dp1 = gu16_Heater_Dp1*2;
//	}

	mi32_DP = ((I32)gu16_Heater_Dp1 * (I32)mi16_diff) / 10;	//		DP＝dp1×(DTset'－DTout)	[1W]

	/************************************************/
	mi32_max_work = (I32)gu16_HeaterFull_WATT;		//
	mi32_max = (mi32_max_work * gu16_Heater_Dp2) / 1000;	//	MAX치 = max × Dp2 	[1W]
	mi32_min = -mi32_max;							//	MIN치 = max × Dp2	[1W]

	if ((gu16_FlowTarget_W-20) > gu16_FlowDecide_W)	//	목표유량이 현재유량보다 높은 경우
	{
		mi32_max = 0;
	}

	if (mi32_DP < mi32_min)
	{												//	최소치보다 작은 경우
		mi32_DP = mi32_min;							//		DP의 MIN 제한
	}
	else if (mi32_DP > mi32_max)
	{												//	최대치보다 큰 경우
		mi32_DP = mi32_max;							//		DP의 MAX 제한
	}
	else
	{
		/* ... */
	}

	return (mi32_DP);								//	제한후의 DP의 설정			[1W]
}

/// @brief    히터 I값 연산
/// @param    void
/// @return   void
static I32 WATT_Calculation_I(void)
{
	I16	mi16_diff;									//	Tset' - Tout buffer
	I32	mi32_DIn;									//	DI buffer
	I32	mi32_DIn2;									//	DI buffer
	I32	mi32_max, mi32_min;
	I32	mi32_DIDP;									//	DI+DP buffer
	U16 mu16_Flow_Diff;

	if (gu16_FlowTarget_W > gu16_FlowDecide_W)		//	목표유량이 현재유량보다 높은 경우
	{
		mu16_Flow_Diff = gu16_FlowTarget_W - gu16_FlowDecide_W;	//		유량 차 계산
	}
	else
	{
		mu16_Flow_Diff = gu16_FlowDecide_W - gu16_FlowTarget_W;	//		유량 차 계산
	}

	if (mu16_Flow_Diff > 20)						//	유량차이가 0.2LPM이상인 경우
	{
		mu16_Flow_Diff = 20;						//		0.2LPM으로 제한
	}

	//if ((gu8_ModelType_forNational == MODEL_KOREA) && (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR45_JPX))	//	분유 인 경우
	 if ((gu8_ModelType_forNational == MODEL_KOREA) && ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR45_JPX) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__50))) //  분유 인 경우
	{
		gu16_Heater_Di1 = Z_Di1_MILK - (mu16_Flow_Diff * Z_Di1_MILK / 30);	//		분유 데이터로 계산 (현재유량과 목표유량이 차이나는 경우 i정수 가변)
	}
	else											//	상기 외
	{
		gu16_Heater_Di1 = Z_Di1 - (mu16_Flow_Diff * Z_Di1 / 30);	//		일반 데이터로 계산 (현재유량과 목표유량이 차이나는 경우 i정수 가변)
		//if ((gu8_ModelType_forNational == MODEL_KOREA)
		//&& (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)
		//&& (gu16_TargetDegree <= (gi16_HotOutSensor.DEGREE+Z_TEMP2)))	//	100도 고온수 추출 중 타겟 온도보다 2도 넘는 경우 DI 절반 (100도 부근에서 정체되면서 I값이 너무 감소되는 것을 방지)
		if ((gu8_ModelType_forNational == MODEL_KOREA)
        && ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90) 
		|| (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__95)
		|| (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__90)
		|| (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR85_JP80))			//cbr_test 2025-09-17
		&& ((gu16_TargetDegree <= (gi16_HotOutSensor.DEGREE+Z_TEMP2))
		|| (gu16_TargetDegree <= (gi16_HeaterSensor.DEGREE+Z_TEMP2))))	//	100도 고온수 추출 중 타겟 온도보다 2도 넘는 경우 DI 절반 (100도 부근에서 정체되면서 I값이 너무 감소되는 것을 방지)	//cbr_test 2025-09-15
		{
			gu16_Heater_Di1 = gu16_Heater_Di1/2;
		}
	}

//	if ((gu8_ModelType_forNational == MODEL_JAPAN)
//	&& (gu16_TargetDegree <= (gi16_HotOutSensor.DEGREE+Z_TEMP1)))	//	일본향 온수 추출온도가 타겟온도보다 높은경우 I값 개입이 너무 많아 I값 하강 하향
//	{
////		gu16_Heater_Di1 = gu16_Heater_Di1/4;
//		gu16_Heater_Di1 = gu16_Heater_Di1/2;
//	}

#ifdef _FOR_DEVELOP_
	if (PSM_gu16_Heater_Di1 != 0) gu16_Heater_Di1 = PSM_gu16_Heater_Di1;
#endif

	/*	I 값 설정	*/
	mi16_diff = (I16)gu16_TargetDegree - gi16_HotOutSensor.DEGREE;	//		Tset - Tout				[1/10:℃]
	if (mi16_diff > Z_PI_Control_LimitTemp)			//		30℃ 이상 차이 있는 경우
	{
		mi16_diff = Z_PI_Control_LimitTemp;			//			MAX(30℃)의 제한
	}
//#ifdef _FOR_JAPAN_90DEGREE_CONTINUE
	if (gu8_ModelType_forNational == MODEL_JAPAN)
	{
		if (mi16_diff < Z_TEMP_Minus1)			//
		{
			mi16_diff = Z_TEMP_Minus1;			//
		}
	}
//#endif
//	mi32_DIn = gi32_Heater_I + (((I32) gu16_Heater_Di1 * (I32) mi16_diff) / (gu16_Heater_Di1*10));	//	I값 누적
	mi32_DIn2 = (I32)(((I32)gu16_Heater_Di1 * (I32) mi16_diff) / 10000);	//	I값 누적
	mi32_DIn = gi32_Heater_I + mi32_DIn2;	//	I값 누적

	if (mi32_DIn > gu16_HeaterFull_WATT)			//	I값 최대치 제한
	{
		mi32_DIn = gu16_HeaterFull_WATT;
	}

	mi32_max = gu16_HeaterFull_WATT;				//	MAX치 = [1W]
	//if ((gu8_ModelType_forNational == MODEL_KOREA) && (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR45_JPX))
	 if ((gu8_ModelType_forNational == MODEL_KOREA) && ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR45_JPX) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__50) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__60)))

	{
		mi32_min = 0;
	}
	else
	{
		mi32_min = gu16_HeaterRELAY_WATT;				//	MIN치 = [1W]
	}

	mi32_DIDP = mi32_DIn + gi32_Heater_P;			//	DI＋DP						[1/1000:Kcal/min]
	if (mi32_DIDP <= mi32_min)						//	최소값보다 작은 경우
	{
		mi32_DIn = mi32_min - gi32_Heater_P;		//		DI의 MIN 제한(DI+DP)
	}
	else if (mi32_DIDP >= mi32_max)					//	최대값보다 큰 경우
	{
		mi32_DIn = mi32_max;
	}
	else
	{

	}

	return (mi32_DIn);									//	DI의 설정							[1/1000:Kcal/min]
}

/// @brief
/// @param    void
/// @return   void
static void BeforeEff_SteppingMotor_Check(void)
{
	I16 mi16_TempBuf = 0;

	if (gu8_Stepping_ZeroBack != 0)					//			스테핑모터 초기화 중인 경우
	{
		Ini_HotStep_2F();							//				스테핑모터 대기로 이행
	}
	else
	{
		Ini_HotStep_33();							//
	}

	if (gu8_ModelType_forNational == MODEL_KOREA)
	{
		mi16_TempBuf = Z_TEMP3;
	}
	else											//	일본향
	{
		mi16_TempBuf = Z_TEMP0_5;
	}

	F_PreHeatingVoice_forJapan = CLEAR;
	gu8_HotPreHeatingStep = 0;						//	에열 표시 단계 clr
	if ((gi16_EffStartTargetTemp-mi16_TempBuf) >= gi16_HeaterSensor.DEGREE)	//	예열필요 표시 판단
	{
		gu16_HotEffStart_Standard = gi16_EffStartTargetTemp;
		gu16_HotEffStart_First = gi16_HeaterSensor.DEGREE;
		gu16_HotEffStart_DeltaT = gi16_EffStartTargetTemp - gi16_HeaterSensor.DEGREE;
		if (gu8_ModelType_forNational == MODEL_KOREA)
		{
			//if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)
			if ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)  || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__95) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__90))
			{
#ifndef _KOREA_NEW_VOICE_
				Play_Voice(VOICE_HOTWATER_EFFLUENT_A);
#else
				Play_Voice(VOICE_HIGH_HOTTEMP_EFFLUENT_1);
				gu8_TimerVoiceOutput_HotCaution = Z_TimerVoiceOutput_HotCaution;
				F_VoiceOutput_HotCaution = SET;
#endif
			}
			else
			{
				Play_Voice(VOICE_HOTWATER_EFFLUENT_A);
			}
		}
		else										//	일본향은 예열시간이 길기에 음성 별도
		{
			F_PreHeatingVoice_forJapan = SET;
			Play_Voice(VOICE_PRE_HEATING);
		}
	}
	else
	{
		gu16_HotEffStart_Standard = 0;
		gu16_HotEffStart_DeltaT = 0;
		gu16_HotEffStart_First = 0;

		if (gu8_ModelType_forNational == MODEL_KOREA)
		{
			//if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)
			if ((gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__95) || (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__90))
			{
#ifndef _KOREA_NEW_VOICE_
				Play_Voice(VOICE_HOTWATER_EFFLUENT_A);	//
#else
				Play_Voice(VOICE_HIGH_HOTTEMP_EFFLUENT_1);	//
				gu8_TimerVoiceOutput_HotCaution = Z_TimerVoiceOutput_HotCaution;
				F_VoiceOutput_HotCaution = SET;
#endif
			}
			else
			{
				Play_Voice(VOICE_HOTWATER_EFFLUENT_A);
			}
		}
		else										//	일본향
		{
			Play_Voice(VOICE_HOTWATER_EFFLUENT_A);
		}
	}
}

/// @brief    디폴트 온수 온도가 비활성화되었을 때 활성화된 다른 온도로 변경
/// @param    void
/// @return   void
void Update_DefaultHotTemp_If_Disabled(void)
{
	U8 mu8_f_no_match = 0;
	U8 min_temp_index = 0;
	U8 min_temp_value = 0;
	U8 k = 0;
	
	// selected_hot_temp[] 배열이 업데이트되어 있어야 하므로 HotTemp_Setting() 호출 후에 이 함수를 호출해야 함
	
	// WiFi 제어인 경우만, gu8_hot_temp_default_data_for_wifi와 매칭 확인
	if(gu8_OperationStep >= Z_OperationStep_10)
	{
		// gu8_hot_temp_default_data_for_wifi는 hot_temp_setting_table의 인덱스(1~12)
		// hot_temp_setting_table[gu8_hot_temp_default_data_for_wifi - 1]의 값과 매칭되는 온도 찾기
		if((gu8_hot_temp_default_data_for_wifi >= 1) && (gu8_hot_temp_default_data_for_wifi <= 12))
		{
			U8 default_temp_value = hot_temp_setting_table[gu8_hot_temp_default_data_for_wifi - 1];
			for(k = 0; k < gu8_select_hot_temp_total_count; k++)
			{
				if(selected_hot_temp[k] == default_temp_value)
				{
					gu8_DefaultHotTempSetup = k;
					mu8_f_no_match = 1; // 1 = 매칭됨, 0 = 매칭 안 됨
					break;
				}
			}
		}
		
		// 매칭이 안된 경우 현재 선택된 온수온도를 디폴트로 변경
		if(mu8_f_no_match == 0)
		{
			// 현재 선택된 온수온도(gu8_HotTempSetup)가 selected_hot_temp[] 배열에 있는지 확인
			for(k = 0; k < gu8_select_hot_temp_total_count; k++)
			{
				if(selected_hot_temp[k] == gu8_HotTempSetup)
				{
					gu8_DefaultHotTempSetup = k;
					mu8_f_no_match = 1;  // 매칭됨
					break;
				}
			}
		}
	}
	else
	{
		// 전원 재인가시에는 EEPROM 데이터 사용
		// gu8_DefaultHotTempSetup이 유효한 범위인지 확인
		if(gu8_DefaultHotTempSetup < gu8_select_hot_temp_total_count)
		{
			// 유효한 범위이면 그대로 유지
			return;
		}
	}
	
	// 디폴트 온도가 비활성화되어 범위를 벗어났거나 매칭되지 않았으면 가장 낮은 온도로 변경
	if(gu8_DefaultHotTempSetup >= gu8_select_hot_temp_total_count || mu8_f_no_match == 0)
	{
		if(gu8_select_hot_temp_total_count > 0)
		{
			min_temp_index = 0;
			min_temp_value = selected_hot_temp[0];
			k = 0;
			
			// 가장 낮은 온도 찾기
			for(k = 1; k < gu8_select_hot_temp_total_count; k++)
			{
				if(selected_hot_temp[k] < min_temp_value)
				{
					min_temp_value = selected_hot_temp[k];
					min_temp_index = k;
				}
			}
			
			gu8_DefaultHotTempSetup = min_temp_index;
		}
		else
		{
			// 활성화된 온도가 없으면 기본값으로
			gu8_DefaultHotTempSetup = 0;
		}
	}
}

/// @brief
/// @param    void
/// @return   void
void HotTemp_Setting(void)
{
	U8 i = 0;
	U8 j = 0;
	U8 count = 0;
	U16 mu16_amount_data = 0;

	for(i = 0; i < Z_SELECT_TEMP_MAX; i++)
	{
		selected_hot_temp[i] = 0;
	}
	for(j = 0; j < (HOT_TEMP_SETUP_KOR100_JP90 + 1); j++)
	{
		mu16_amount_data = (gu8_hot_temp_data_for_wifi.word >> j) & 0x0001;			//	shift 시켜서 데이터 set되어 있는 용량 서칭

		if(mu16_amount_data == SET)
		{
			selected_hot_temp[count] = hot_temp_setting_table[j];
			count++;
		}
	}

	gu8_select_hot_temp_total_count = (U8)(count);
	
	// 디폴트 온도 업데이트
	Update_DefaultHotTemp_If_Disabled();
}

/// @brief    현재 선택된 온수 온도가 비활성화되었을 때 활성화된 다른 온도로 변경
/// @param    void
/// @return   void
void Update_CurrentHotTemp_If_Disabled(void)
{
	U8 k = 0;
	U8 found = 0;
	U8 min_temp_index = 0;
	U8 min_temp_value = 0;
	
	// selected_hot_temp[] 배열이 업데이트되어 있어야 하므로 HotTemp_Setting() 호출 후에 이 함수를 호출해야 함
	// 현재 선택된 온수 온도(gu8_HotTempSetup)가 selected_hot_temp[] 배열에 있는지 확인
	for(k = 0; k < gu8_select_hot_temp_total_count; k++)
	{
		if(selected_hot_temp[k] == gu8_HotTempSetup)
		{
			// 현재 선택된 온도가 활성화되어 있음
			gu8_hot_water_select_index = k;
			found = 1;
			break;
		}
	}
	
	// 현재 선택된 온도가 비활성화되었거나 범위를 벗어났으면 활성화된 온도로 변경
	if(found == 0)
	{
		if(gu8_select_hot_temp_total_count > 0)
		{
			// 가장 낮은 온도 찾기
			min_temp_index = 0;
			min_temp_value = selected_hot_temp[0];
			
			for(k = 1; k < gu8_select_hot_temp_total_count; k++)
			{
				if(selected_hot_temp[k] < min_temp_value)
				{
					min_temp_value = selected_hot_temp[k];
					min_temp_index = k;
				}
			}
			
			// 가장 낮은 온도로 설정
			gu8_hot_water_select_index = min_temp_index;
			gu8_HotTempSetup = selected_hot_temp[min_temp_index];
		}
		else
		{
			// 활성화된 온도가 없으면 기본값으로
			gu8_hot_water_select_index = 0;
		}
	}
}
