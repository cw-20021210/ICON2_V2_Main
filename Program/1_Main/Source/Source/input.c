/// @file     input.c
/// @date     2019.07.10
/// @author   PSM
/// @brief    입력 처리 file
#define INPUT_C
#include "include/_include.h"


/// @brief    디지털 입력 초기화
/// @param    void
/// @return   void
void Input_Initialize(void)
{

}

/// @brief    디지털 입력 제어
/// @param    void
/// @return   void
void InputControl(void)
{
	BYTE_BIT mu8_inputdata;

	if (F_10ms_for_InputDigital ==  SET)			//	10ms 주기
	{
		F_10ms_for_InputDigital = CLEAR;

		mu8_inputdata.Bit.b0 = !P_REED_SW_COVER;
		mu8_inputdata.Bit.b1 = !P_REED_SW_FILTER;
		mu8_inputdata.Bit.b2 = !P_REED_SW_WATERFLOW;
		mu8_inputdata.Bit.b3 = P_MODEL_NATIONAL;

		ThreeMatchModule(&gu8_DigitalInput, mu8_inputdata.byte);	//	디지털 입력 3회 일치 모듈

#ifdef _FOR_DEVELOP_
		if (INPUT_REED_SW_COVER_Test != 0) INPUT_REED_SW_COVER = INPUT_REED_SW_COVER_Test;
		if (INPUT_REED_SW_FILTER_Test != 0) INPUT_REED_SW_FILTER = INPUT_REED_SW_FILTER_Test;
		if (INPUT_REED_SW_WATERFLOW_Test != 0) INPUT_REED_SW_FAUCET = INPUT_REED_SW_WATERFLOW_Test;
#endif
	}
}

/// @brief    기종 확정
/// @param    void
/// @return   void
void Model_Decide(void)
{
	switch (INPUT_MODEL_NATIONAL)					//	국가 판단
	{
		case SET:									//
			gu8_ModelType_forNational = MODEL_JAPAN;
			break;
		case CLEAR:									//
			gu8_ModelType_forNational = MODEL_KOREA;
			break;
		default:
			break;
	}
#ifdef _JAPAN_MODEL_
	gu8_ModelType_forNational = MODEL_JAPAN;
#endif

	if (gu8_ModelType_forNational == MODEL_JAPAN)	//	일본향
	{
		gu16_HeaterFull_WATT = HEATER_FULL_WATT_JP;		//	히터 최대출력 WATT 초기치 저장
		gu16_HeaterRELAY_WATT = HEATER_RELAY_WATT_JP;

		gu16_FlowMeter_Standard = Z_FlowMeter_Standard_JP;
		gu16_FlowMeter_Standard_forHotWater = Z_FlowMeter_Standard_forHotWater_JP;
		gu16_Spec_FlowMeter = (U16)((U32)166666 / Z_FlowMeter_Standard_JP);	//		유량센서 spec 기준 데이터 처리

		F_HotLock = SET;
	}
	else											//	국내향
	{
		gu16_HeaterFull_WATT = HEATER_FULL_WATT_KOR;	//	히터 최대출력 WATT 초기치 저장
		gu16_HeaterRELAY_WATT = HEATER_RELAY_WATT_KOR;

		gu16_FlowMeter_Standard = Z_FlowMeter_Standard_KOR;
#ifdef _DPP_FLOWMETER_
		gu16_FlowMeter_Standard_forHotWater = Z_FlowMeter_Standard_KOR;
	//	gu16_FlowMeter_Standard_forHotWater_forJapan = Z_FlowMeter_Standard;
#else
		gu16_FlowMeter_Standard_forHotWater = Z_FlowMeter_Standard_forHotWater_KOR;
	//	gu16_FlowMeter_Standard_forHotWater_forJapan = Z_FlowMeter_Standard_forHotWater_forJapan;
#endif
		gu16_Spec_FlowMeter = (U16)((U32)166666 / Z_FlowMeter_Standard_KOR);	//		유량센서 spec 기준 데이터 처리
	}
}

/// @brief    3회 일치 모듈
/// @param    void
/// @return   void
void ThreeMatchModule(ITTI3* p, U8 data)
{
	p->Third = p->Second;
	p->Second = p->First;
	p->First = data;

	p->OnEdge.byte = (U8) ((~p->Level.byte) & (p->First) & (p->Second) & (p->Third));
	p->OffEdge.byte = (U8) ((p->Level.byte) & (~p->First) & (~p->Second) & (~p->Third));
	p->Level.byte = (U8) (((p->First | p->Second | p->Third) & p->Level.byte) | p->OnEdge.byte);
}


/// @brief    1회 일치 모듈
/// @param    void
/// @return   void
void OneMatchModule(ITTI1* p, U8 data)
{
	p->First = data;

	p->OnEdge.byte = (U8) ((~p->Level.byte) & (p->First));
	p->OffEdge.byte = (U8) ((p->Level.byte) & (~p->First));
	p->Level.byte = (U8) (((p->First) & p->Level.byte) | p->OnEdge.byte);
}
