/// @file     pure.c
/// @date     2019.07.10
/// @author   PSM
/// @brief    추출 제어 관련 file
#define PURE_C
#include "include/_include.h"

static void Ini_PureStep_00(void);					//	부팅
static void Ini_PureStep_01(void);					//	필터 플러싱 대기
static void Ini_PureStep_02(void);					//	필터 플러싱 중
static void Ini_PureStep_10(void);					//	정수 미선택
static void Ini_PureStep_20(void);					//	정수 선택 (미추출)
static void Ini_PureStep_22(void);					//	추출 중

static void PureStep_00(void);						//	부팅
static void PureStep_01(void);						//
static void PureStep_02(void);						//
static void PureStep_10(void);						//	정수 미선택
static void PureStep_20(void);						//	정수 선택 (미추출)
static void PureStep_22(void);						//	추출 중

static void PureStepNoSet(U8 data);

static void Pure_WaterFlow_Average_Calculation(void);

Pure_StepTable Pure_Step[Z_PureStepNum_Max] =
{
	//	스텝 No			정수 밸브			Step Func.
	{Z_PureStep_00,		VALVE_OFF,		PureStep_00},	//	부팅
	{Z_PureStep_01,		VALVE_OFF,		PureStep_01},	//
	{Z_PureStep_02,		VALVE_ON,		PureStep_02},	//
	{Z_PureStep_10,		VALVE_OFF,		PureStep_10},	//	정수 미선택
	{Z_PureStep_20,   	VALVE_OFF,		PureStep_20},	//	정수 선택 (미추출)
	{Z_PureStep_22,		VALVE_ON,		PureStep_22},	//	추출 중
//	{Z_PureStep_B0,		VALVE_ON,		PureStep_B0},	//	내부 잔압 제거
};


/// @brief    추출 시퀀스 초기화
/// @param    void
/// @return   void
void Pure_Initialize(void)
{
	F_Pure_First = CLEAR;							//	부팅 후 첫회 추출 이후 인 경우 (유량변화 20%이상 체크를 위함)
	gu16_FlowDecide_W_Ave_Pure_Standard = 100;		//	1LPM

	Ini_PureStep_00();
}

/// @brief
/// @param    void
/// @return   void
void PureControl(void)
{
	if (gu8_PureStepNum < Z_PureStepNum_Max)		//	스텝 허용범위 이내
	{
		(Pure_Step[gu8_PureStepNum].exe)();			//	해당 스텝 함수 실행

//		if ((gu8_PureStep != Z_PureStep_B0) && (gu8_OperationStep == Z_OperationStep_B0) && (INPUT_REED_SW_COVER == CLEAR))
//		{
//			Ini_PureStep_B0();
//		}

		gu8_PureValve_CMD = Pure_Step[gu8_PureStepNum].PURE_V;
	}
	else											//	스텝 허용 범위 외
	{
		SafetyLoop();								//		안전정지 루프
	}
}

/// @brief    부팅
/// @param    void
/// @return   void
static void PureStep_00(void)
{
	if (F_FirstFlushing == SET)
	{
		Ini_PureStep_10();
	}
	else
	{
		Ini_PureStep_01();
	}
}
static void Ini_PureStep_00(void)
{
	PureStepNoSet(Z_PureStepNum_00);
}

/// @brief    플러싱 대기
/// @param    void
/// @return   void
static void PureStep_01(void)
{
	if (gu8_OperationStep == Z_OperationStep_03)
	{
		Ini_PureStep_02();
	}
	else if (F_FirstFlushing == SET)
	{
		Ini_PureStep_10();
	}
	else
	{

	}
}
static void Ini_PureStep_01(void)
{
	PureStepNoSet(Z_PureStepNum_01);
}

/// @brief
/// @param    void
/// @return   void
static void PureStep_02(void)
{
	if (gu8_OperationStep != Z_OperationStep_03)
	{
		if (F_FirstFlushing == SET)
		{
			Ini_PureStep_10();
		}
		else
		{
			Ini_PureStep_01();
		}
	}
}
static void Ini_PureStep_02(void)
{
	PureStepNoSet(Z_PureStepNum_02);
}

/// @brief    정수 미선택
/// @param    void
/// @return   void
static void PureStep_10(void)
{
	if (gu8_WaterSelect == PURE_WATER)				//	정수 선택의 경우
	{
		Ini_PureStep_20();							//		정수선택 미추출
	}
	else if (F_FirstFlushing == CLEAR)
	{
		Ini_PureStep_01();
	}
	else
	{

	}
}
static void Ini_PureStep_10(void)
{
	PureStepNoSet(Z_PureStepNum_10);
}

/// @brief    정수선택 (미추출)
/// @param    void
/// @return   void
static void PureStep_20(void)
{
	if (gu8_OperationStep == Z_OperationStep_50)	//	 추출 중 인경우
	{
		Ini_PureStep_22();						//		정수 선택 (추출중)으로 이행
	}
	else if (gu8_WaterSelect != PURE_WATER)	//	정수 미선택의 경우
	{
		Ini_PureStep_10();						//		정수 미선택으로 이행
	}
	else if (F_FirstFlushing == CLEAR)
	{
		Ini_PureStep_01();
	}
	else
	{

	}
}
static void Ini_PureStep_20(void)
{
	PureStepNoSet(Z_PureStepNum_20);
}

/// @brief    정수 추출 중
/// @param    void
/// @return   void
static void PureStep_22(void)
{
	if (gu8_OperationStep != Z_OperationStep_50)
	{
		Ini_PureStep_20();							//		정수선택 이행
		if (gu8_Timer_PureWaterFlow_Average_Start == 0)	//	정수 추출 후 3초가 지난 뒤(유량계산이 안정화 된 뒤)부터 유량 재계산 확정
		{
			Pure_WaterFlow_Average_Calculation();	//		평균유량 기준값 결정 및 재세팅 필요 판단
		}
	}
	else if (gu8_WaterSelect != PURE_WATER)		//	정수 미선택의 경우
	{
		Ini_PureStep_10();							//		정수 미선택으로 이행
	}
	else
	{

	}
}
static void Ini_PureStep_22(void)
{
	PureStepNoSet(Z_PureStepNum_22);
	gu8_Timer_PureWaterFlow_Average_Start = Z_Timer_PureWaterFlow_Average_Start;	//	정수 추출 후 3초가 지난 뒤(유량계산이 안정화 된 뒤)부터 유량 재계산 확정
	Effluent_Hz_Decide();				//	정수추출 유량 선택
}

/// @brief    정수 스텝 설정
/// @param    void
/// @return   void
static void PureStepNoSet(U8 data)
{
	static const U8 DT_PureNumber[] =
	{
		Z_PureStep_00,
		Z_PureStep_01,
		Z_PureStep_02,
		Z_PureStep_10,
		Z_PureStep_20,
		Z_PureStep_22,
	};
	/************************************************/
	gu8_PureStepNum = data; 						/*	일련 번호 set*/

	gu8_PureStep = DT_PureNumber[gu8_PureStepNum];	/*	상태 mode No. set*/
}

/// @brief    정수 평균유량 계산
/// @param    void
/// @return   void
static void Pure_WaterFlow_Average_Calculation(void)
{
	U16 mu16_FirstFlowDelta = 0;

	if (F_Pure_First == SET)						//		부팅 후 첫회 추출 이후 인 경우 (유량변화 20%이상 체크)
	{
		mu16_FirstFlowDelta = abs(gu16_FlowDecide_W_Ave_Pure_Standard - gu16_FlowDecide_W_Ave);	//		첫회 추출 평균유량과 현재 추출 평균유량 Delta
		if (mu16_FirstFlowDelta > (gu16_FlowDecide_W_Ave_Pure_Standard - ((gu16_FlowDecide_W_Ave_Pure_Standard * 8) / 10)))	//		20% 초과 차이나는 경우
		{
//			F_StartFlow_ReCalibration = SET;		//			유량 Recalibration 필요
			gu16_FlowDecide_W_Ave_Pure_Standard = gu16_FlowDecide_W_Ave;	//		기준 유량 재설정
		}
	}

	if (F_Pure_First == CLEAR)						//		최초추출
	{
		F_Pure_First = SET;							//			flag set
		gu16_FlowDecide_W_Ave_Pure_Standard = gu16_FlowDecide_W_Ave;	//			평균유량 기준유량으로 저장
	}
}
