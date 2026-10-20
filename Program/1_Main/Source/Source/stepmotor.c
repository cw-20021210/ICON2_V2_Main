/// @file     stepmotor.c
/// @date     2019.07.10
/// @author   PSM
/// @brief    Step Motor 동작 제어 file
#define STEPMOTOR_C
#include "include/_include.h"

#define Z_SteppingMotor_TorqueUp_Count	(5)			//	PPS조절 카운트
#define Z_OpenClose_Min		(8)						//	스테핑모터 최저 제어

CI8 au8Step_Open[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };	//	OPEN 스텝 루틴
CI8 au8Step_Close[8] = { 8, 7, 6, 5, 4, 3, 2, 1 };	//	CLOSE 스텝 루틴


/// @brief    부팅후 Step Motor 위치 초기화
/// @param    void
/// @return   void
void StepMotor_Initialize(void)
{

}

/// @brief    유량 제어 함수
/// @param    void
/// @return   void
void FlowControl(void)
{
	U8 mu8_StepControl_Value = 0;					//	스텝 제한
	U8 mu8_StepDelta = 0;							//	목표유량/현재유량 대비 스텝 제한

	/*	목표유량 변경	*/
//	if (F_3s_for_FlowControl == SET)				//	목표유량 변경 Flag (3초마다) (히터 전력이 낮은데도 온도가 너무 높은경우만 해당)
//	{
//		F_3s_for_FlowControl = CLEAR;				//		Flag Clear
//		if ((gu8_WaterSelect == HOT_WATER) && (gu8_HotStep == Z_HotStep_35))	//		온수상태이고 온수추출중인 경우
//		{
//#ifdef _HOT_TEMP_4STEP_
//			if (((gu8_HotTempSetup == HOT_TEMP_SETUP_90) || (gu8_HotTempSetup == HOT_TEMP_SETUP_80) || (gu8_HotTempSetup == HOT_TEMP_SETUP_70))	//		COFFEE 및 TEA만
//#else
//			if (((gu8_HotTempSetup == HOT_TEMP_SETUP_90) || (gu8_HotTempSetup == HOT_TEMP_SETUP_70))	//		COFFEE 및 TEA만
//#endif
//				&& (gu16_HeaterWATT < gu16_HeaterRELAY_WATT) && (gi16_HotOutSensor.DEGREE > gu16_TargetDegree))	//		히터 지시전력이 RELAY WATT보다 낮은데 목표온도보다 높은경우
//			{
//				if (gu16_FlowTarget_W < 80)			//				목표유량이 0.8LPM보다 낮은 경우 (실험치)
//				{
//					gu16_FlowTarget_W++;			//				목표유량 증가 (3초마다 0.1LPM씩)
//				}
//			}
//		}
//	}
	/*	...		*/
	if (((F_TestMode_PBA == SET) && (F_PBATest_SteppingMotor == SET))	//	PBA Test and 스테핑모터검사 중인 경우
		|| ((F_TestMode_LineFront == SET) && (F_LineFrontTest_SteppingMotor_Oper == SET))	//	라인 Test
		|| (F_SmartCheck_SteppingMotor == SET))
	{
		F_PBATest_SteppingMotor = CLEAR;
		if ((gu16_StepMovingPulse == 0) || (gi16_StepCount < 1500) || (gi16_StepCount >= 2800))		//		스텝 제어 완료인 경우
		{
			gu8_Stepping_ZeroBack = 0;				//			제로백 Flag Clear
			if (gi16_StepCount >= 1500)				//			스테핑모터가 열려있는 경우
			{
//				StepMotor_ZeroBack_Close();			//				닫힘 초기화
				FlowControl_StepTarget(1400);
			}
			else									//			스테핑모터가 닫혀있는 경우
			{
				StepMotor_ZeroBack_Open();			//				열림 초기화
			}
		}
	}
	else											//	일반제어
	{
		if (F_100ms_for_FlowControl == SET)			//		100ms마다 유량제어
		{
			F_100ms_for_FlowControl = CLEAR;		//			100ms 타이머 flag clear
			if ((gu8_HotStep == Z_HotStep_35) && (gu8_TimerFlowControl_AfterEff != 0) && (gu16_FlowDecide_W > gu16_FlowTarget_W))
			{
				gu8_TimerFlowControl_AfterEff = 0;
			}
			if (((gu8_WaterSelect == HOT_WATER) && (gu8_HotStep == Z_HotStep_35) && (gu8_TimerFlowControl_AfterEff == 0)	//	온수추출 중 and 유량제어 시작 타이머 경과 and 스텝제어 완료 and 목표유량과 현재유량이 다른경우
//				&& (gu16_StepMovingPulse == 0) && (gu16_FlowTarget_W != 0) && (abs(gu16_FlowDecide_W-gu16_FlowTarget_W) > 1)))
				&& (gu16_StepMovingPulse == 0) && (gu16_FlowTarget_W != 0) && (abs(gu16_FlowDecide_W-gu16_FlowTarget_W) >= 1)))
			{
				if ((gu8_ModelType_forNational == MODEL_KOREA)
				&& (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR45_JPX)
				&& (gi16_HotInSensor.DEGREE > Z_TEMP8)
				&& (gu16_FlowTarget_W >= gu16_FlowDecide_W))	//	분유는 유량이 많거나 입수온도가 낮을 때에만 유량컨트롤
				{

				}
				else if (gu16_FlowDecide_W > gu16_FlowTarget_W)	//		현재유량이 더 큰 경우
				{
					mu8_StepControl_Value = (U8)((gu16_FlowDecide_W - gu16_FlowTarget_W) / 4);	//			스텝제한치 계산

					if (mu8_StepControl_Value > 4)	//			스텝제한치가 4보다 큰 경우
					{
						mu8_StepControl_Value = 4;	//				4로 제한
					}

					if ((gu16_FlowDecide_W - gu16_FlowTarget_W) >= 10)	//			유량 Delta로 스텝제한치 계산
					{
						mu8_StepDelta = 10;			//			10보다 큰경우 10으로 제한
					}
					else
					{
						mu8_StepDelta = (U8)(gu16_FlowDecide_W - gu16_FlowTarget_W);	//			유량Delta
					}

					gu16_OpenCloseStep_Value = mu8_StepDelta + (5*mu8_StepControl_Value);	//			닫을 스텝 구함

					if (gu16_OpenCloseStep_Value < Z_OpenClose_Min)		//			닫을 스텝이 4보다 작은 경우 (실험치, 4보다 낮으면 반응속도가 너무 느림)
					{
						gu16_OpenCloseStep_Value = Z_OpenClose_Min;		//				최소치 4로 제한
					}

					if (gi16_StepCount > 0)			//			닫을 스텝이 있을 경우
					{
						FlowControl_StepMove(STEP_MOTER_CLOSE, gu16_OpenCloseStep_Value);	//			스텝 Set
					}
				}
				else								//			목표유량이 더 큰 경우
				{
					mu8_StepControl_Value = (U8)((gu16_FlowTarget_W - gu16_FlowDecide_W) / 4);

					if (mu8_StepControl_Value > 4)
					{
						mu8_StepControl_Value = 4;
					}

					if ((gu16_FlowTarget_W - gu16_FlowDecide_W) >= 10)
					{
						mu8_StepDelta = 10;
					}
					else
					{
						mu8_StepDelta = (U8)(gu16_FlowTarget_W - gu16_FlowDecide_W);
					}

					gu16_OpenCloseStep_Value = mu8_StepDelta + (5*mu8_StepControl_Value);

					if (gu16_OpenCloseStep_Value < Z_OpenClose_Min)//
					{
						gu16_OpenCloseStep_Value = Z_OpenClose_Min;
					}

					if (gi16_StepCount < 2800)		//	더 열 스텝이 있을 경우 (※ 3000이 초기화이나, 스테핑모터 특성곡선 상 2800정도가 가장 최대열림치)
					{
						FlowControl_StepMove(STEP_MOTER_OPEN, gu16_OpenCloseStep_Value);
					}
				}
			}
		}

		/* 스텝 by 유량 계산	*/
		if ((gu8_WaterSelect == HOT_WATER)
			&& ((gu8_HotStep == Z_HotStep_02) || ((gu8_TimerFlowCheck_AfterEff == 0) && (gu8_HotStep == Z_HotStep_35)))	//	온수상태 and 유량체크타이머 경과 and 추출 중인 경우
			/*|| (gu8_HotStep == Z_HotStep_02)*/ /*|| (gu8_HotStep == Z_HotStep_3E)*/)		//	재캘리브레이션 또는 초기드레인
		{
			if (gi16_StepCount >= 2800)
			{
				if (gu16_FlowDecide_W_HotEffMax < gu16_FlowDecide_W)
				{
					gu16_FlowDecide_W_HotEffMax = gu16_FlowDecide_W;
				}
			}
			if ((gi16_StepCount >= 0) && (gi16_StepCount <= 2800)
				&& (gu16_FlowDecide_W_BeforeEff != gu16_FlowDecide_W) && (gu16_FlowDecide_W >= 10))	//@002		스텝이 0이 아니고 현재유량과 이전 유량이 다른 경우
			{
				gu16_StepCount_BeforeEff = (U16)gi16_StepCount;	//			스텝치 저장
				if (gu16_StepCount_BeforeEff > 2800)			//			스텝치가 3000보다 큰 경우
				{
					gu16_StepCount_BeforeEff = 2800;			//				3000 제한
				}
				gu16_FlowDecide_W_BeforeEff = gu16_FlowDecide_W;//			현재유량 저장
			}
		}
	}
}

/// @brief    유량 제어 함수
/// @param    mu8OpenClose : 유량 제어시 유로 Open or Close 방향
///           mu16Pulse     : 유량 제어 모터 이동 Pulse
/// @return   void
void FlowControl_StepMove(U8 mu8OpenClose, U16 mu16Pulse)
{
	if (gu8_Stepping_ZeroBack == 0)					//	스텝초기화 flag clear인 경우
	{
		gu8_StepMotorWay = mu8OpenClose;				//		스테핑모터 방향
		gu16_StepMovingPulse = mu16Pulse;			//		스텝 치
		gu8_Pps_Control = 0;							//		PPS 세팅
		gu8_Step_Cycle = 0;							//		스텝 루틴 초기화
	}
	else											//	스테핑모터 초기화 중인 경우
	{
		gu8_StepMotorWay_Backup = mu8OpenClose;		//		데이터 back up (초기화 완료 후 이동)
		gu16_StepMovingPulse_Backup = mu16Pulse;	//		데이터 back up (초기화 완료 후 이동)
	}
}

/// @brief    스텝 목표 제어 함수
/// @param    mu16Pulse_Target : 현재 스텝 대비 목표 스텝 계산 및 지시
/// @return   void
void FlowControl_StepTarget(U16 mu16Pulse_Target)
{
	if (mu16Pulse_Target > 2800)
	{
		mu16Pulse_Target = 2800;
	}

	if (mu16Pulse_Target >= gi16_StepCount)
	{
		gu16_OpenCloseStep_Value = mu16Pulse_Target-gi16_StepCount;
		if (gu16_OpenCloseStep_Value < Z_OpenClose_Min)    //
		{
			gu16_OpenCloseStep_Value = Z_OpenClose_Min;
		}

		FlowControl_StepMove(STEP_MOTER_OPEN, gu16_OpenCloseStep_Value);
	}
	else
	{
		gu16_OpenCloseStep_Value = gi16_StepCount-mu16Pulse_Target;
		if (gu16_OpenCloseStep_Value < Z_OpenClose_Min)    //
		{
			gu16_OpenCloseStep_Value = Z_OpenClose_Min;
		}

		FlowControl_StepMove(STEP_MOTER_CLOSE, gu16_OpenCloseStep_Value);
	}
}

/// @brief    Step Motor 위치 변경(Move) 함수(1ms)
/// @param    void
/// @return   void
void StepMotorMove(void)
{
	if ((gu8_Stepping_ZeroBack == 0) && (gu16_StepMovingPulse_Backup != 0))	//	스테핑모터 초기화 완료 후 back up 데이터 있을 경우
	{
		gu8_StepMotorWay = gu8_StepMotorWay_Backup;             //		스테핑모터 방향
		gu16_StepMovingPulse = gu16_StepMovingPulse_Backup;   //		스텝 치
		gu8_StepMotorWay_Backup = 0;                           //
		gu16_StepMovingPulse_Backup = 0;                      //
	}

	if (gu16_StepMovingPulse != 0)					// 	Step Motor의 이동값이 있는 경우
	{
		if (++gu8_Pps_Control < Z_SteppingMotor_TorqueUp_Count)   // 		PPS 조절 (5회까지는 2ms마다 제어, 그 이후 1ms마다 제어. 토크를 높이기 위함)
		{
			gu8Move_PPS = 2;						//			2ms마다 제어
		}
		else
		{
			gu8_Pps_Control = Z_SteppingMotor_TorqueUp_Count + 1;
			gu8Move_PPS = 1;						//			1ms마다 제어
		}

		if (++gu8Cnt_PPS >= gu8Move_PPS)			//		PPS 성립 시
		{
			gu8Cnt_PPS = 0;							//			카운트 초기화

			gu16_StepMovingPulse--;					//			움직일 스텝 감소
			StepMotor_Control();    				//			스테핑모터 제어

			if (++gu8_Step_Cycle >= 8)				//			8 Cycle 루틴 초과인 경우
			{
				gu8_Step_Cycle = 0;					//				루틴카운터 초기화
			}
		}
	}
	else											// 	Step Motor의 이동값이 없는 경우
	{
		gu16_StepMovingPulse = 0;					//		데이터 초기화
		gu8_Pps_Control = 0;							//		데이터 초기화
		gu8_Step_Cycle = 0;							//		데이터 초기화
		gu8_StepMotorWay = STEP_MOTER_OPEN;			//		OPEN으로 셋팅

		if (gu8_Stepping_ZeroBack == 1)				//		OPEN 초기화 지시 있는 경우
		{
			gu8_Stepping_ZeroBack = 0;				//			Flag Clear
			gi16_StepCount = ZERO_BACK;				//			스텝치 최대 OPEN치로 set
		}
		else if (gu8_Stepping_ZeroBack == 2)          //		CLOSE 초기화 지시 있는 경우
		{
			gu8_Stepping_ZeroBack = 0;                //			Flag Clear
			gi16_StepCount = 0;                     //			스텝치 최대 CLOSE치로 set
		}
		else
		{
			if (gi16_StepCount > 3000)
			{
				gi16_StepCount = 3000;
			}
		}

		StepMotor_Stop();							//		스텝제어 정지 (여자 OFF)
	}
}

/// @brief    Step Motor 제어 함수
/// @param    void
/// @return   void
void StepMotor_Control(void)
{
	U8 mu8Step_Order = 0;

	if (gu8_StepMotorWay == STEP_MOTER_CLOSE)		//	CLOSE 지시 인 경우
	{
		mu8Step_Order = au8Step_Close[gu8_Step_Cycle];	//		CLOSE 루틴
	}
	else											//	OPEN 지시
	{
		mu8Step_Order = au8Step_Open[gu8_Step_Cycle];	//		OPEN 루틴
	}

	if (gu8_StepMotorWay == STEP_MOTER_CLOSE)
	{
		gi16_StepCount--;
	}
	else
	{
		if (gi16_StepCount < 3000)
		{
			gi16_StepCount++;
		}
	}

	switch (mu8Step_Order)
	{
		case 1:
			P_STEPMOTOR1 = 0;
			P_STEPMOTOR2 = 0;
			P_STEPMOTOR3 = 0;
			P_STEPMOTOR4 = 1;
			break;
		case 2:
			P_STEPMOTOR1 = 1;
			P_STEPMOTOR2 = 0;
			P_STEPMOTOR3 = 0;
			P_STEPMOTOR4 = 1;
			break;
		case 3:
			P_STEPMOTOR1 = 1;
			P_STEPMOTOR2 = 0;
			P_STEPMOTOR3 = 0;
			P_STEPMOTOR4 = 0;
			break;
		case 4:
			P_STEPMOTOR1 = 1;
			P_STEPMOTOR2 = 1;
			P_STEPMOTOR3 = 0;
			P_STEPMOTOR4 = 0;
			break;
		case 5:
			P_STEPMOTOR1 = 0;
			P_STEPMOTOR2 = 1;
			P_STEPMOTOR3 = 0;
			P_STEPMOTOR4 = 0;
			break;
		case 6:
			P_STEPMOTOR1 = 0;
			P_STEPMOTOR2 = 1;
			P_STEPMOTOR3 = 1;
			P_STEPMOTOR4 = 0;
			break;
		case 7:
			P_STEPMOTOR1 = 0;
			P_STEPMOTOR2 = 0;
			P_STEPMOTOR3 = 1;
			P_STEPMOTOR4 = 0;
			break;
		case 8:
			P_STEPMOTOR1 = 0;
			P_STEPMOTOR2 = 0;
			P_STEPMOTOR3 = 1;
			P_STEPMOTOR4 = 1;
			break;
		default:
			P_STEPMOTOR1 = 0;
			P_STEPMOTOR2 = 0;
			P_STEPMOTOR3 = 0;
			P_STEPMOTOR4 = 0;
			break;
	}
}

/// @brief    Tray Step Motor Stop Function
/// @param    void
/// @return   void
void StepMotor_Stop(void)
{
	P_STEPMOTOR1 = 0;
	P_STEPMOTOR2 = 0;
	P_STEPMOTOR3 = 0;
	P_STEPMOTOR4 = 0;
}

/// @brief    Step Motor Open Position Zero Back
/// @param    void
/// @return   void
void StepMotor_ZeroBack_Open(void)
{
	FlowControl_StepMove(STEP_MOTER_OPEN, ZERO_BACK);
	gu8_Stepping_ZeroBack = 1;
}

/// @brief    Step Motor Close Position Zero Back
/// @param    void
/// @return   void
void StepMotor_ZeroBack_Close(void)
{
	FlowControl_StepMove(STEP_MOTER_CLOSE, ZERO_BACK);
	gu8_Stepping_ZeroBack = 2;
}
