/// @file     display.c
/// @date     2019.07.10
/// @author   PSM
/// @brief    표시 제어 관련 file
#define DISPLAY_C
#include "include/_include.h"

#define Z_SOURCE_NUM 		(3)						//	Source 갯수

//#define Z_Fade_Select			(10)				//	Fade Timer 100ms
#define Z_Fade_Select			(8)					//	Fade Timer 80ms
#define Z_ON_DimmingPercent		(100)				//	ON Dimming
#define Z_DimmingPercent		(20)				//	약하게 ON
#define Z_DimmingPercent_Low	(10)				//	더 약하게

static void Operation_Display(void);

static void LED_EFFLUENT(void);
static void LED_HOT(void);
static void LED_PURE(void);
static void LED_COLD(void);
static void LED_AMOUNT(void);
static void LED_CUP(void);
static void LED_COLD_TEMP(void);
static void LED_WIFI(void);
static void LED_FILTER_CHANGE(void);
static void LED_FAUCET(void);
static void LED_ML(void);
static void LED_DEGREE(void);
static void LED_STE(void);
static void LED_UV(void);
static void LED_HOTLOCK(void);

static void SegDataSet(void);
static void SegDataClear(void);

/// @brief    표시부 초기화
/// @param    void
/// @return   void
void Display_Initialize(void)
{
	gu8_Fade_Select = Z_Fade_Select;				//	Fade Timer 초기값
	gu8_ON_DimmingPercent = Z_ON_DimmingPercent;
	gu8_DimmingPercent = Z_DimmingPercent;			//	Dimming Percent 초기값
	gu8_DimmingPercent_Low = Z_DimmingPercent_Low;	//	Dimming Percent 초기값
}

/// @brief    표시부 제어
/// @param    void
/// @return   void
void DisplayControl(void)
{
	U8 i;

	for (i = 0 ; i < DisplayNum_Max ; i++)
	{
		DisplayControl_Data[i].Display_CMD = LED_OFF;	//		표시 CMD 초기화
	}

	SegDataClear();
	Operation_Display();							//	운전상태에 따른 표시 지시

	for (i = 0 ; i < DisplayNum_Max ; i++)
	{
		if (DisplayControl_Data[i].Display_CMD != DisplayControl_Data[i].Display_CMD_Before)	//	표시 상태가 변하는 경우
		{
			F_CommunicationCMD = SET;				//		통신 즉시 송신
		}
		DisplayControl_Data[i].Display_CMD_Before = DisplayControl_Data[i].Display_CMD;		//	이전 지시 저장
	}
}

/// @brief    Data Clear
/// @param    void
/// @return   void
static void SegDataClear(void)
{
	U8 mu8_i;

	for (mu8_i = 0 ; mu8_i < Z_SOURCE_NUM ; mu8_i++)	//	Seg 데이터 갯수에 따라
	{
		gu8_SegData[mu8_i].byte = 0x00;					//		clr
	}
}

/// @brief    운전스텝 표시Data 처리
/// @param    void
/// @return   void
static void Operation_Display(void)
{
	SegDataSet();									//	7-Segment 데이터 set

	LED_EFFLUENT();
	LED_HOT();
	LED_PURE();
	LED_COLD();
	LED_AMOUNT();
	if ((F_TestMode_LineFront == SET) && (gu16_TimerFCT_Incomplete_Display != 0))	//@027
	{
		return;	//@027
	}
	LED_CUP();
	LED_COLD_TEMP();
	LED_WIFI();
	LED_FILTER_CHANGE();
	LED_FAUCET();
	LED_ML();
	LED_DEGREE();
	LED_STE();
	LED_UV();
	LED_HOTLOCK();
}

/// @brief
/// @param    void
/// @return   void
static void SegDataSet(void)
{
	U16 mu16_Cup_buf = 0;
	U16 mu16_Seg_buf = 0;
	static I16 mi16_Seg_buf_forSEGTH = 0;
	U8 mu8_OPENSHORT_Error = 0;
	U8 mu8_TotalPercent = 0;
	U8 temp;
	U8 mu8_buf = 0;
	U16 mu16_temp = 0;
	U16 mu16_FrontTest = 0;
	
	static U8 mu8_AdFeedback_State = 0;      // 상태 (0:VOL 텍스트, 1:VOL 값, 2:TEM 텍스트, 3:TEM 값)    
    	U16 mu16_TemValue = 0;   // TEM 값
    	U16 mu16_VolValue = 0;  // VOL 값
    	U8 mu8_Thousands = 0;
    	U8 mu8_Hundreds = 0;
    	U8 mu8_Tens = 0;
    	U8 mu8_Ones = 0;
	

	switch (DisplayControl_Data[Z_SEGMENT].Operation_CMD)	//	7-Segment 지시에 따라
	{
		case SEG_OFF:								//		OFF 지시
			break;
		case SEG_ON_FLIC:							//		`ON` 점멸 지시
			SEG_OXX = DT_7seg[Z_NUM_O];
			SEG_XOX = DT_7seg[Z_NUM_n];
			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			break;
		case SEG_OFF_FLIC:							//		`OFF` 점멸 지시
			SEG_OXX = DT_7seg[Z_NUM_O];
			SEG_XOX = DT_7seg[Z_NUM_F];
			SEG_XXO = DT_7seg[Z_NUM_F];
			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			break;
		case SEG_OFF_DISP:							//		`OFF` 점등
			SEG_OXX = DT_7seg[Z_NUM_O];
			SEG_XOX = DT_7seg[Z_NUM_F];
			SEG_XXO = DT_7seg[Z_NUM_F];
			if (gu8_TimerOFF_Flic != 0)
			{
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			}
			else
			{
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			}
			break;
		case SEG_ONOFF_MOVE:
			if (F_SleepMode_Display == SET)
			{
				if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*11))
				{
					SEG_XXO = DT_7seg[Z_NUM_S];
				}
				else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*10))
				{
					SEG_XOX = DT_7seg[Z_NUM_S];
					SEG_XXO = DT_7seg[Z_NUM_L];
				}
				else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*9))
				{
					SEG_OXX = DT_7seg[Z_NUM_S];
					SEG_XOX = DT_7seg[Z_NUM_L];
					SEG_XXO = DT_7seg[Z_NUM_E];
				}
				else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*8))
				{
					SEG_OXX = DT_7seg[Z_NUM_L];
					SEG_XOX = DT_7seg[Z_NUM_E];
					SEG_XXO = DT_7seg[Z_NUM_E];
				}
				else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*7))
				{
					SEG_OXX = DT_7seg[Z_NUM_E];
					SEG_XOX = DT_7seg[Z_NUM_E];
					SEG_XXO = DT_7seg[Z_NUM_P];
				}
				else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*6))
				{
					SEG_OXX = DT_7seg[Z_NUM_E];
					SEG_XOX = DT_7seg[Z_NUM_P];
					SEG_XXO = DT_7seg[Z_NUM_BAR];
				}
				else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*5))
				{
					SEG_OXX = DT_7seg[Z_NUM_P];
					SEG_XOX = DT_7seg[Z_NUM_BAR];
					SEG_XXO = DT_7seg[Z_NUM_O];
				}
				else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*4))
				{
					SEG_OXX = DT_7seg[Z_NUM_BAR];
					SEG_XOX = DT_7seg[Z_NUM_O];
					if (F_SleepMode == SET)
					{
						SEG_XXO = DT_7seg[Z_NUM_n];
					}
					else
					{
						SEG_XXO = DT_7seg[Z_NUM_F];
					}
				}
				else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*3))
				{
					SEG_OXX = DT_7seg[Z_NUM_O];
					if (F_SleepMode == SET)
					{
						SEG_XOX = DT_7seg[Z_NUM_n];
					}
					else
					{
						SEG_XOX = DT_7seg[Z_NUM_F];
						SEG_XXO = DT_7seg[Z_NUM_F];
					}
				}
				else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*2))
				{
					if (F_SleepMode == SET)
					{
						SEG_OXX = DT_7seg[Z_NUM_n];
					}
					else
					{
						SEG_OXX = DT_7seg[Z_NUM_F];
						SEG_XOX = DT_7seg[Z_NUM_F];
					}
				}
				else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*1))
				{
					if (F_SleepMode == SET)
					{

					}
					else
					{
						SEG_OXX = DT_7seg[Z_NUM_F];
					}
				}
				else
				{

				}
			}
			else if (F_ColdOff_Display == SET)
			{
				if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*11))
				{
					SEG_XXO = DT_7seg[Z_NUM_C];
				}
				else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*10))
				{
					SEG_XOX = DT_7seg[Z_NUM_C];
					SEG_XXO = DT_7seg[Z_NUM_o];
				}
				else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*9))
				{
					SEG_OXX = DT_7seg[Z_NUM_C];
					SEG_XOX = DT_7seg[Z_NUM_o];
					SEG_XXO = DT_7seg[Z_NUM_L];
				}
				else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*8))
				{
					SEG_OXX = DT_7seg[Z_NUM_o];
					SEG_XOX = DT_7seg[Z_NUM_L];
					SEG_XXO = DT_7seg[Z_NUM_d];
				}
				else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*7))
				{
					SEG_OXX = DT_7seg[Z_NUM_L];
					SEG_XOX = DT_7seg[Z_NUM_d];
					SEG_XXO = DT_7seg[Z_NUM_BAR];
				}
				else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*6))
				{
					SEG_OXX = DT_7seg[Z_NUM_d];
					SEG_XOX = DT_7seg[Z_NUM_BAR];
					SEG_XXO = DT_7seg[Z_NUM_O];
				}
				else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*5))
				{
					SEG_OXX = DT_7seg[Z_NUM_BAR];
					SEG_XOX = DT_7seg[Z_NUM_O];
					if (F_ColdOff == CLEAR)
					{
						SEG_XXO = DT_7seg[Z_NUM_n];
					}
					else
					{
						SEG_XXO = DT_7seg[Z_NUM_F];
					}
				}
				else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*4))
				{
					SEG_OXX = DT_7seg[Z_NUM_O];
					if (F_ColdOff == CLEAR)
					{
						SEG_XOX = DT_7seg[Z_NUM_n];
					}
					else
					{
						SEG_XOX = DT_7seg[Z_NUM_F];
						SEG_XXO = DT_7seg[Z_NUM_F];
					}
				}
				else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*3))
				{
					if (F_ColdOff == CLEAR)
					{
						SEG_OXX = DT_7seg[Z_NUM_n];
					}
					else
					{
						SEG_OXX = DT_7seg[Z_NUM_F];
						SEG_XOX = DT_7seg[Z_NUM_F];
					}
				}
				else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay/12)*2))
				{
					if (F_ColdOff == CLEAR)
					{

					}
					else
					{
						SEG_OXX = DT_7seg[Z_NUM_F];
					}
				}
				else
				{

				}
			}

			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			break;
		case SEG_ERR_FLIC:						//		에러표시
			SEG_OXX = DT_7seg[Z_NUM_E];
			SEG_XOX = DT_7seg[(gu8_ErrorCode & 0xf0)>>4];
			SEG_XXO = DT_7seg[(gu8_ErrorCode & 0x0f)];
			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			break;
		case SEG_FLUSHING:							//	플러싱 중
//			SEG_XOX = DT_7seg[Z_NUM_F];
//			SEG_XXO = DT_7seg[Z_NUM_L];
//			mu8_TotalPercent = (U8)((U32)gu16_Flushing_Hz_forDisplay * 100 / gu16_Flushing_TotalHz_forDisplay);	//	100~0
			mu8_buf = (U8)((U32)gu16_Flushing_Hz_forDisplay * 100 / gu16_Flushing_TotalHz_forDisplay);	//	0~100
			if (mu8_buf <= 100)
			{
				if (gu8_ModelType_forNational == MODEL_KOREA)	//	국내향은 100~0
				{
					mu8_TotalPercent = mu8_buf;		//	100~0
				}
				else								//	일본향은 0~100
				{
					mu8_TotalPercent = (U8)(100-mu8_buf);	//	0~100
				}
			}
			if (gu8_TimerFlicker == 0)
			{
				gu8_TimerFlicker = Z_TimerFlicker;
			}

			/*	1% 단위 표시 시작 	*/
			if (mu8_TotalPercent >= 100)
			{
				SEG_OXX = DT_7seg[(mu8_TotalPercent / 100) % 10];
			}
			if (mu8_TotalPercent >= 10)
			{
				SEG_XOX = DT_7seg[(mu8_TotalPercent / 10) % 10];
			}
			SEG_XXO = DT_7seg[mu8_TotalPercent % 10];
			if (gu8_ModelType_forNational == MODEL_JAPAN)		//	일본향 WIFI LED를 %로 활용
			{
				DisplayControl_Data[Z_LED_WIFI].Display_CMD = LED_ON;
			}
			/*	1% 단위 표시 끝	*/

			/*	써클 한칸씩 채워지는 플러싱 표시	시작	*/
//			if (mu8_TotalPercent <= 10)
//			{
//				if (gu8_TimerFlicker <= 50)
//				{
//					SEG_OXX = DT_7seg[Z_NUM_SEG_a];
//				}
//			}
//			else if (mu8_TotalPercent <= 20)
//			{
//				SEG_OXX = DT_7seg[Z_NUM_SEG_a];
//				if (gu8_TimerFlicker <= 50)
//				{
//					SEG_XOX = DT_7seg[Z_NUM_SEG_a];
//				}
//			}
//			else if (mu8_TotalPercent <= 30)
//			{
//				SEG_OXX = DT_7seg[Z_NUM_SEG_a];
//				SEG_XOX = DT_7seg[Z_NUM_SEG_a];
//				if (gu8_TimerFlicker <= 50)
//				{
//					SEG_XXO = DT_7seg[Z_NUM_SEG_a];
//				}
//			}
//			else if (mu8_TotalPercent <= 40)
//			{
//				SEG_OXX = DT_7seg[Z_NUM_SEG_a];
//				SEG_XOX = DT_7seg[Z_NUM_SEG_a];
//				SEG_XXO = DT_7seg[Z_NUM_SEG_a];
//				if (gu8_TimerFlicker <= 50)
//				{
//					SEG_XXO |= DT_7seg[Z_NUM_SEG_b];
//				}
//			}
//			else if (mu8_TotalPercent <= 50)
//			{
//				SEG_OXX = DT_7seg[Z_NUM_SEG_a];
//				SEG_XOX = DT_7seg[Z_NUM_SEG_a];
//				SEG_XXO = DT_7seg[Z_NUM_SEG_a];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_b];
//				if (gu8_TimerFlicker <= 50)
//				{
//					SEG_XXO |= DT_7seg[Z_NUM_SEG_c];
//				}
//			}
//			else if (mu8_TotalPercent <= 60)
//			{
//				SEG_OXX = DT_7seg[Z_NUM_SEG_a];
//				SEG_XOX = DT_7seg[Z_NUM_SEG_a];
//				SEG_XXO = DT_7seg[Z_NUM_SEG_a];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_b];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_c];
//				if (gu8_TimerFlicker <= 50)
//				{
//					SEG_XXO |= DT_7seg[Z_NUM_SEG_d];
//				}
//			}
//			else if (mu8_TotalPercent <= 70)
//			{
//				SEG_OXX = DT_7seg[Z_NUM_SEG_a];
//				SEG_XOX = DT_7seg[Z_NUM_SEG_a];
//				SEG_XXO = DT_7seg[Z_NUM_SEG_a];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_b];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_c];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_d];
//				if (gu8_TimerFlicker <= 50)
//				{
//					SEG_XOX |= DT_7seg[Z_NUM_SEG_d];
//				}
//			}
//			else if (mu8_TotalPercent <= 80)
//			{
//				SEG_OXX = DT_7seg[Z_NUM_SEG_a];
//				SEG_XOX = DT_7seg[Z_NUM_SEG_a];
//				SEG_XXO = DT_7seg[Z_NUM_SEG_a];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_b];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_c];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_d];
//				SEG_XOX |= DT_7seg[Z_NUM_SEG_d];
//				if (gu8_TimerFlicker <= 50)
//				{
//					SEG_OXX |= DT_7seg[Z_NUM_SEG_d];
//				}
//			}
//			else if (mu8_TotalPercent <= 90)
//			{
//				SEG_OXX = DT_7seg[Z_NUM_SEG_a];
//				SEG_XOX = DT_7seg[Z_NUM_SEG_a];
//				SEG_XXO = DT_7seg[Z_NUM_SEG_a];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_b];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_c];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_d];
//				SEG_XOX |= DT_7seg[Z_NUM_SEG_d];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_d];
//				if (gu8_TimerFlicker <= 50)
//				{
//					SEG_OXX |= DT_7seg[Z_NUM_SEG_e];
//				}
//			}
//			else if (mu8_TotalPercent <= 100)
//			{
//				SEG_OXX = DT_7seg[Z_NUM_SEG_a];
//				SEG_XOX = DT_7seg[Z_NUM_SEG_a];
//				SEG_XXO = DT_7seg[Z_NUM_SEG_a];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_b];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_c];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_d];
//				SEG_XOX |= DT_7seg[Z_NUM_SEG_d];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_d];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_e];
//				if (gu8_TimerFlicker <= 50)
//				{
//					SEG_OXX |= DT_7seg[Z_NUM_SEG_f];
//				}
//			}
//			else
//			{
//
//			}
			/*	써클 한칸씩 채워지는 플러싱 표시	끝	*/

			/*	써클 한칸씩 지워지는 플러싱 표시	시작 */
//			if (mu8_TotalPercent <= 10)
//			{
//				if (gu8_TimerFlicker <= 50)
//				{
//					SEG_OXX = DT_7seg[Z_NUM_SEG_a];
//				}
//				SEG_XOX = DT_7seg[Z_NUM_SEG_a];
//				SEG_XXO = DT_7seg[Z_NUM_SEG_a];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_b];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_c];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_d];
//				SEG_XOX |= DT_7seg[Z_NUM_SEG_d];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_d];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_e];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_f];
//			}
//			else if (mu8_TotalPercent <= 20)
//			{
//				if (gu8_TimerFlicker <= 50)
//				{
//					SEG_XOX = DT_7seg[Z_NUM_SEG_a];
//				}
//				SEG_XXO = DT_7seg[Z_NUM_SEG_a];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_b];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_c];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_d];
//				SEG_XOX |= DT_7seg[Z_NUM_SEG_d];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_d];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_e];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_f];
//			}
//			else if (mu8_TotalPercent <= 30)
//			{
//				if (gu8_TimerFlicker <= 50)
//				{
//					SEG_XXO = DT_7seg[Z_NUM_SEG_a];
//				}
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_b];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_c];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_d];
//				SEG_XOX |= DT_7seg[Z_NUM_SEG_d];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_d];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_e];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_f];
//			}
//			else if (mu8_TotalPercent <= 40)
//			{
//				if (gu8_TimerFlicker <= 50)
//				{
//					SEG_XXO |= DT_7seg[Z_NUM_SEG_b];
//				}
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_c];
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_d];
//				SEG_XOX |= DT_7seg[Z_NUM_SEG_d];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_d];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_e];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_f];
//			}
//			else if (mu8_TotalPercent <= 50)
//			{
//				if (gu8_TimerFlicker <= 50)
//				{
//					SEG_XXO |= DT_7seg[Z_NUM_SEG_c];
//				}
//				SEG_XXO |= DT_7seg[Z_NUM_SEG_d];
//				SEG_XOX |= DT_7seg[Z_NUM_SEG_d];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_d];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_e];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_f];
//			}
//			else if (mu8_TotalPercent <= 60)
//			{
//				if (gu8_TimerFlicker <= 50)
//				{
//					SEG_XXO |= DT_7seg[Z_NUM_SEG_d];
//				}
//				SEG_XOX |= DT_7seg[Z_NUM_SEG_d];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_d];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_e];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_f];
//			}
//			else if (mu8_TotalPercent <= 70)
//			{
//				if (gu8_TimerFlicker <= 50)
//				{
//					SEG_XOX |= DT_7seg[Z_NUM_SEG_d];
//				}
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_d];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_e];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_f];
//			}
//			else if (mu8_TotalPercent <= 80)
//			{
//				if (gu8_TimerFlicker <= 50)
//				{
//					SEG_OXX |= DT_7seg[Z_NUM_SEG_d];
//				}
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_e];
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_f];
//			}
//			else if (mu8_TotalPercent <= 90)
//			{
//				if (gu8_TimerFlicker <= 50)
//				{
//					SEG_OXX |= DT_7seg[Z_NUM_SEG_e];
//				}
//				SEG_OXX |= DT_7seg[Z_NUM_SEG_f];
//			}
//			else if (mu8_TotalPercent <= 100)
//			{
//				if (gu8_TimerFlicker <= 50)
//				{
//					SEG_OXX |= DT_7seg[Z_NUM_SEG_f];
//				}
//			}
//			else
//			{
//
//			}
			/*	써클 한칸씩 지워지는 플러싱 표시	끝 */
			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			break;
//		case SEG_CoL:
//			SEG_OXX = DT_7seg[Z_NUM_C];
//			SEG_XOX = DT_7seg[Z_NUM_o];
//			SEG_XXO = DT_7seg[Z_NUM_L];
//			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
//			break;
		case SEG_TOPCOVER:
#ifndef _AFTER_LPP_IMDCHANGE_
			if (INPUT_REED_SW_FAUCET == CLEAR)
			{
				SEG_OXX = DT_7seg[Z_NUM_F];
				SEG_XOX = DT_7seg[Z_NUM_L];
				SEG_XXO = DT_7seg[Z_NUM_o];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			}
#endif
			break;
		case SEG_ERR_HIS:							//		에러이력
			if (gu8_TimerErrorHistory == 0)			//			표시 타이머 경과 시
			{
				gu8_TimerErrorHistory = Z_TimerErrorHistory;
				if (++gu8_CounterErrorHistory >= Z_ErrorHistory)
				{
					gu8_CounterErrorHistory = 0;
				}
			}
			if (gu8_TimerErrorHistory < (Z_TimerErrorHistory-10))	//	이력 표시 타이머 중, 1초는 SEG OFF를 하여 순차 구분
			{
				if (gu8_ErrorHistory[gu8_CounterErrorHistory] == 0x00)
				{
					SEG_OXX = DT_7seg[Z_NUM_E];
					SEG_XOX = DT_7seg[Z_NUM_BAR];
					SEG_XXO = DT_7seg[Z_NUM_BAR];
				}
				else
				{
					SEG_OXX = DT_7seg[Z_NUM_E];
					SEG_XOX = DT_7seg[(gu8_ErrorHistory[gu8_CounterErrorHistory] & 0xf0) >> 4];
					SEG_XXO = DT_7seg[(gu8_ErrorHistory[gu8_CounterErrorHistory] & 0x0f)];
				}
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			}
			else
			{
				SEG_OXX = DT_7seg[gu8_CounterErrorHistory+1];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			}
			break;
		case SEG_VER:
			if (F_TestMode_PBA == SET)
			{
				if (gu16_TimerVersionDisplay <= (Z_TimerPBATestDisplay/2))	//	테스트모드 1 표시
				{
					SEG_OXX = DT_7seg[Z_NUM_t];
					SEG_XOX = DT_7seg[Z_NUM_E];
					SEG_XXO = DT_7seg[Z_NUM_1];
				}
				else								//	기종 표시
				{
					if (INPUT_MODEL_NATIONAL == MODEL_KOREA)
					{
						SEG_XOX = DT_7seg[Z_NUM_K];
						SEG_XXO = DT_7seg[Z_NUM_r];
					}
					else
					{
						SEG_XOX = DT_7seg[Z_NUM_J];
						SEG_XXO = DT_7seg[Z_NUM_P];
					}
				}
			}
			else
			{
				if (gu16_TimerVersionDisplay <= 50)		//			?초 이내 버전표시
				{
#ifdef _FOR_DEVELOP_
					SEG_OXX = DT_7seg[(gu16_Front_ProgramVer & 0x0F00) >> 8];	//	DPP or LPP 단계 표시
#endif
					SEG_XOX = DT_7seg[(gu16_Front_ProgramVer & 0x00F0) >> 4];
					SEG_XXO = DT_7seg[(gu16_Front_ProgramVer & 0x000F)];

				}
				if (gu16_TimerVersionDisplay <= 100)				//			소등
				{

				}
				else if (gu16_TimerVersionDisplay <= 150)		//			?초 이내 버전표시
				{
#ifdef _FOR_DEVELOP_
					SEG_OXX = DT_7seg[(PROGRAM_VERSION_NO & 0x0F00) >> 8];	//	DPP or LPP 단계 표시
#endif
					SEG_XOX = DT_7seg[(PROGRAM_VERSION_NO & 0x00F0) >> 4];
					SEG_XXO = DT_7seg[(PROGRAM_VERSION_NO & 0x000F)];
				}
				else
				{
					if (gu8_ModelType_forNational == MODEL_KOREA)
					{
						if (gu8_ModelType_forCHP == MODEL_CHP)
						{
							SEG_OXX = DT_7seg[Z_NUM_C];
							SEG_XOX = DT_7seg[Z_NUM_H];
							SEG_XXO = DT_7seg[Z_NUM_P];
						}
						else
						{
							SEG_OXX = DT_7seg[Z_NUM_C];
							SEG_XOX = DT_7seg[Z_NUM_P];
						}
					}
					else							//	일본향
					{
						SEG_OXX = DT_7seg[Z_NUM_J];
						SEG_XOX = DT_7seg[Z_NUM_P];
					}
				}
			}
			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			break;
		case SEG_BOOT:							//		부팅 표시
//			temp = (U8)((Z_TimerBootDisplay - gu16_TimerBootDisplay));
//			if (temp <= 50)
//			{
//
//			}
//			else if (temp <= 100)
//			{
//				SEG_OXX = DT_7seg[Z_NUM_8];
//			}
//			else if (temp <= 150)
//			{
//				SEG_OXX = DT_7seg[Z_NUM_8];
//				SEG_XOX = DT_7seg[Z_NUM_8];
//			}
//			else
//			{
//				SEG_OXX = DT_7seg[Z_NUM_8];
//				SEG_XOX = DT_7seg[Z_NUM_8];
//				SEG_XXO = DT_7seg[Z_NUM_8];
//			}
//			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;

			SEG_OXX = DT_7seg[Z_NUM_8];
			SEG_XOX = DT_7seg[Z_NUM_8];
			SEG_XXO = DT_7seg[Z_NUM_8];
			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			break;
		case SEG_EFF:								//	추출 표시
			if ((gu8_WaterSelect == HOT_WATER) && (gu8_HotStep >= Z_HotStep_20) && (gu8_HotStep < Z_HotStep_35) && (gu8_HotStep != Z_HotStep_02))		//	예열 중일 경우 선택량 표시
			{
				if (gu16_HotEffStart_DeltaT != 0)
				{
					if (gu16_HotEffStart_Standard >= gu16_HotEffStart_First)
					{
						mu16_temp = (gu16_HotEffStart_Standard - gu16_HotEffStart_First)/4;	//	예열표시 4로 나누어 => 1단계 ~50% / 2단계 ~75% / 3단계 ~100%
					}
					if ((gu16_HotEffStart_First+(mu16_temp*2)) > gi16_HeaterSensor.DEGREE)
					{
						if (gu8_HotPreHeatingStep == 0)
						{
							gu8_HotPreHeatingStep = 1;
						}
					}
					else if ((gu16_HotEffStart_First+(mu16_temp*3)) > gi16_HeaterSensor.DEGREE)
					{
						if (gu8_HotPreHeatingStep == 1)
						{
							gu8_HotPreHeatingStep = 2;
						}
					}
					else
					{
						if (gu8_HotPreHeatingStep == 2)
						{
							gu8_HotPreHeatingStep = 3;
						}
					}

					switch (gu8_HotPreHeatingStep)
					{
						case 1:
						default:
							SEG_OXX = DT_7seg[Z_NUM_SEG_d];
							SEG_XOX = DT_7seg[Z_NUM_SEG_d];
							SEG_XXO = DT_7seg[Z_NUM_SEG_d];
							break;
						case 2:
							SEG_OXX = (U8)(DT_7seg[Z_NUM_SEG_d] | DT_7seg[Z_NUM_SEG_g]);
							SEG_XOX = (U8)(DT_7seg[Z_NUM_SEG_d] | DT_7seg[Z_NUM_SEG_g]);
							SEG_XXO = (U8)(DT_7seg[Z_NUM_SEG_d] | DT_7seg[Z_NUM_SEG_g]);
							break;
						case 3:
							SEG_OXX = (U8)(DT_7seg[Z_NUM_SEG_d] | DT_7seg[Z_NUM_SEG_g] | DT_7seg[Z_NUM_SEG_a]);
							SEG_XOX = (U8)(DT_7seg[Z_NUM_SEG_d] | DT_7seg[Z_NUM_SEG_g] | DT_7seg[Z_NUM_SEG_a]);
							SEG_XXO = (U8)(DT_7seg[Z_NUM_SEG_d] | DT_7seg[Z_NUM_SEG_g] | DT_7seg[Z_NUM_SEG_a]);
							break;
					}
#ifndef _HOT_TEMP_4STEP_
					if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR85_JP80)
					{
						SEG_XOX = DT_7seg[Z_NUM_8];
						SEG_XXO = DT_7seg[Z_NUM_5];
					}
					else if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR70_JP70)
					{
						SEG_XOX = DT_7seg[Z_NUM_7];
						SEG_XXO = DT_7seg[Z_NUM_0];
					}
					else
					{
						SEG_XOX = DT_7seg[Z_NUM_4];
						SEG_XXO = DT_7seg[Z_NUM_5];
					}
#else
//					if (gu8_ModelType_forNational == MODEL_KOREA)
//					{
//						if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)
//						{
//							SEG_OXX = DT_7seg[Z_NUM_1];
//							SEG_XOX = DT_7seg[Z_NUM_0];
//							SEG_XXO = DT_7seg[Z_NUM_0];
//						}
//						else if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR85_JP80)
//						{
//							SEG_XOX = DT_7seg[Z_NUM_8];
//							SEG_XXO = DT_7seg[Z_NUM_5];
//						}
//						else if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR70_JP70)
//						{
//							SEG_XOX = DT_7seg[Z_NUM_7];
//							SEG_XXO = DT_7seg[Z_NUM_0];
//						}
//						else
//						{
//							SEG_XOX = DT_7seg[Z_NUM_4];
//							SEG_XXO = DT_7seg[Z_NUM_5];
//						}
//					}
//					else							//	일본향
//					{
//						if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)
//						{
//							SEG_XOX = DT_7seg[Z_NUM_9];
//							SEG_XXO = DT_7seg[Z_NUM_0];
//						}
//						else if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR85_JP80)
//						{
//							SEG_XOX = DT_7seg[Z_NUM_8];
//							SEG_XXO = DT_7seg[Z_NUM_0];
//						}
//						else
//						{
//							SEG_XOX = DT_7seg[Z_NUM_7];
//							SEG_XXO = DT_7seg[Z_NUM_0];
//						}
//					}
#endif
//#ifndef _FRONT_LED_UP_
//					DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_DIMM;
//#else
//					DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_ON;
//#endif
				}
				else								//	온수 예열 중, 예열 표시 짧을 경우 0ml로 표시
				{
					SEG_XXO = DT_7seg[Z_NUM_0];
#ifndef _FRONT_LED_UP_
				DisplayControl_Data[Z_LED_mL].Display_CMD = LED_DIMM;
#else
				DisplayControl_Data[Z_LED_mL].Display_CMD = LED_ON;
#endif
//					if (gu8_CupSize == CUPSIZE_HALFCUP)
//					{
//						SEG_OXX = DT_7seg[Z_NUM_1];
//						SEG_XOX = DT_7seg[Z_NUM_2];
//						SEG_XXO = DT_7seg[Z_NUM_0];
//					}
//					else if (gu8_CupSize == CUPSIZE_1CUP)
//					{
//						SEG_OXX = DT_7seg[Z_NUM_2];
//						SEG_XOX = DT_7seg[Z_NUM_5];
//						SEG_XXO = DT_7seg[Z_NUM_0];
//					}
//					else if (gu8_CupSize == CUPSIZE_2CUP)
//					{
//						SEG_OXX = DT_7seg[Z_NUM_5];
//						SEG_XOX = DT_7seg[Z_NUM_0];
//						SEG_XXO = DT_7seg[Z_NUM_0];
//					}
//					else if (gu8_CupSize == CUPSIZE_CONTINUE)     //			연속추출
//					{
//						SEG_OXX = DT_7seg[Z_NUM_SEG_a] | DT_7seg[Z_NUM_SEG_d] | DT_7seg[Z_NUM_SEG_e] | DT_7seg[Z_NUM_SEG_f];
//						SEG_XOX = DT_7seg[Z_NUM_SEG_a] | DT_7seg[Z_NUM_SEG_d];
//						SEG_XXO = DT_7seg[Z_NUM_SEG_a] | DT_7seg[Z_NUM_SEG_b] | DT_7seg[Z_NUM_SEG_c] | DT_7seg[Z_NUM_SEG_d];
//					}
//					else
//					{
//
//					}
				}
			}
			else if (gu16_CupSize == CUPSIZE_CONTINUE)	//	 연속추출 (Rotation 표시)
			{
#ifdef _HOT_TEST_
				if (gu8_WaterSelect == HOT_WATER)
				{
					if (gu8_TimerSEG_TH_Limit == 0)		//
					{
						gu8_TimerSEG_TH_Limit = Z_TimerSEG_TH_Limit;
						if (F_HotWater_DegreeDisplay_TH == CLEAR)
						{
							mi16_Seg_buf_forSEGTH =	gi16_HotOutSensor.DEGREE;	//	[1/10℃]
						}
						else
						{
							mi16_Seg_buf_forSEGTH =	gi16_HeaterSensor.DEGREE;	//	[1/10℃]
						}
					}

					if (mi16_Seg_buf_forSEGTH < 0)
					{
						SEG_OXX = DT_7seg[Z_NUM_BAR];
					}
					else if (mi16_Seg_buf_forSEGTH >= 100)
					{
//						SEG_OXX = DT_7seg[(mi16_Seg_buf_forSEGTH / 100) % 10];
						SEG_OXX = DT_7seg[mi16_Seg_buf_forSEGTH / 100];
					}

					if (abs(mi16_Seg_buf_forSEGTH) >= 10)
					{
						SEG_XOX = DT_7seg[(abs(mi16_Seg_buf_forSEGTH) / 10) % 10];
					}
					SEG_XXO = DT_7seg[abs(mi16_Seg_buf_forSEGTH) % 10];
#ifndef _FRONT_LED_UP_
					DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_DIMM;
#else
					DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_ON;
#endif
				}
#else
				if (gu8_TimerContSeg == 0)
				{
					gu8_TimerContSeg = Z_TimerContSeg;
					gu8_CounterContSeg++;
					if (gu8_CounterContSeg >= 20)
					{
						gu8_CounterContSeg = 0;
					}
				}
				SEG_OXX = DT_Cont_OXX[gu8_CounterContSeg];
				SEG_XOX = DT_Cont_XOX[gu8_CounterContSeg];
				SEG_XXO = DT_Cont_XXO[gu8_CounterContSeg];
#endif
			}
			else									//		예열중 외
			{
//				if (gu8_CupSize == CUPSIZE_HALFCUP)
//				{
//					mu16_Cup_buf = 120;
//				}
//				else if (gu8_CupSize == CUPSIZE_1CUP)
//				{
//					mu16_Cup_buf = 250;
//				}
//				else if (gu8_CupSize == CUPSIZE_2CUP)
//				{
//					mu16_Cup_buf = 500;
//				}
//				else
//				{
//
//				}

#ifdef _HOT_TEST_
				if (gu8_WaterSelect == HOT_WATER)
				{
					if (gu8_TimerSEG_TH_Limit == 0)		//
					{
						gu8_TimerSEG_TH_Limit = Z_TimerSEG_TH_Limit;
						if (F_HotWater_DegreeDisplay_TH == CLEAR)
						{
							mi16_Seg_buf_forSEGTH =	gi16_HotOutSensor.DEGREE;	//	[1/10℃]
						}
						else
						{
							mi16_Seg_buf_forSEGTH =	gi16_HeaterSensor.DEGREE;	//	[1/10℃]
						}
					}

					if (mi16_Seg_buf_forSEGTH < 0)
					{
						SEG_OXX = DT_7seg[Z_NUM_BAR];
					}
					else if (mi16_Seg_buf_forSEGTH >= 100)
					{
//						SEG_OXX = DT_7seg[(mi16_Seg_buf_forSEGTH / 100) % 10];
						SEG_OXX = DT_7seg[mi16_Seg_buf_forSEGTH / 100];
					}

					if (abs(mi16_Seg_buf_forSEGTH) >= 10)
					{
						SEG_XOX = DT_7seg[(abs(mi16_Seg_buf_forSEGTH) / 10) % 10];
					}
					SEG_XXO = DT_7seg[abs(mi16_Seg_buf_forSEGTH) % 10];
#ifndef _FRONT_LED_UP_
					DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_DIMM;
#else
					DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_ON;
#endif
				}
				else
				{
#else
				if ((gu8_WaterSelect == HOT_WATER) && (F_HotWater_DegreeDisplay == SET))
				{
					if (gu8_TimerSEG_TH_Limit == 0)		//
					{
						gu8_TimerSEG_TH_Limit = Z_TimerSEG_TH_Limit;
						if (F_HotWater_DegreeDisplay_TH == CLEAR)
						{
							mi16_Seg_buf_forSEGTH =	gi16_HotOutSensor.DEGREE/10;	//	[1/1℃]
						}
						else
						{
							mi16_Seg_buf_forSEGTH =	gi16_HeaterSensor.DEGREE/10;	//	[1/1℃]
						}
					}

					if (mi16_Seg_buf_forSEGTH < 0)
					{
						SEG_OXX = DT_7seg[Z_NUM_BAR];
					}
					else if (mi16_Seg_buf_forSEGTH >= 100)
					{
						SEG_OXX = DT_7seg[(mi16_Seg_buf_forSEGTH / 100) % 10];
					}

					if (abs(mi16_Seg_buf_forSEGTH) >= 10)
					{
						SEG_XOX = DT_7seg[(abs(mi16_Seg_buf_forSEGTH) / 10) % 10];
					}
					SEG_XXO = DT_7seg[abs(mi16_Seg_buf_forSEGTH) % 10];
#ifndef _FRONT_LED_UP_
					DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_DIMM;
#else
					DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_ON;
#endif
				}
				else
				{
#endif
					mu16_Cup_buf = gu16_Effluent_CupLitter_forSegDisplay;

					if (gu16_Effluent_Hz > 5)			//	잔여 유량이 5Hz 미만 남았을 경우 Cup 그대로 표시 (하기 else)
					{
						if (gu16_Effluent_Hz_forSegDisplay >= gu16_Effluent_Hz)
						{
							mu16_Seg_buf = (U16)((mu16_Cup_buf*(((U32)((U32)gu16_Effluent_Hz_forSegDisplay-gu16_Effluent_Hz) * 100) / gu16_Effluent_Hz_forSegDisplay))/100);	//	추출 된 유량 계산
						}
					}
					else
					{
						mu16_Seg_buf = mu16_Cup_buf;
					}
					gu16_Effluent_Amount_SegDisplay = mu16_Seg_buf;	//	추출 된 물양

					if (mu16_Seg_buf >= 100)
					{
						SEG_OXX = DT_7seg[(mu16_Seg_buf/100)%10];
					}
					if (mu16_Seg_buf >= 10)
					{
						SEG_XOX = DT_7seg[(mu16_Seg_buf/10)%10];
					}
					SEG_XXO = DT_7seg[Z_NUM_0];
#ifndef _FRONT_LED_UP_
				DisplayControl_Data[Z_LED_mL].Display_CMD = LED_DIMM;
#else
				DisplayControl_Data[Z_LED_mL].Display_CMD = LED_ON;
#endif
#ifndef _HOT_TEST_
				}
#endif
#ifdef _HOT_TEST_
				}
#endif
			}
			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			break;
		case SEG_SOUND:
			SEG_OXX = DT_7seg[Z_NUM_BAR];
			SEG_XOX = DT_7seg[gu8_Sound_Type];
			SEG_XXO = DT_7seg[Z_NUM_BAR];
			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			break;
		case SEG_STE:
			SEG_OXX = DT_7seg[Z_NUM_S];
			SEG_XOX = DT_7seg[Z_NUM_t];
			SEG_XXO = DT_7seg[Z_NUM_E];
			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			break;
		case SEG_MYSET:
			if (gu16_CupSize_My >= 100)
			{
				SEG_OXX = DT_7seg[(gu16_CupSize_My/100)%10];
			}
			if (gu16_CupSize_My >= 10)
			{
				SEG_XOX = DT_7seg[(gu16_CupSize_My/10)%10];
			}
			SEG_XXO = DT_7seg[Z_NUM_0];
#ifndef _FRONT_LED_UP_
				DisplayControl_Data[Z_LED_mL].Display_CMD = LED_DIMM;
#else
				DisplayControl_Data[Z_LED_mL].Display_CMD = LED_ON;
#endif
			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			break;
		case SEG_ML:								//		컵 설정 표시
#ifdef _FOR_CONDENSATION_TEST_
			if (gu16_TimerMoveDisplay == 0)
			{
				gu16_TimerMoveDisplay = Z_TimerMoveDisplay_forTest;
			}

			if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*13))
			{
				SEG_XXO = DT_7seg[Z_NUM_t];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*12))
			{
				SEG_XOX = DT_7seg[Z_NUM_t];
				SEG_XXO = DT_7seg[Z_NUM_E];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*11))
			{
				SEG_OXX = DT_7seg[Z_NUM_t];
				SEG_XOX = DT_7seg[Z_NUM_E];
				SEG_XXO = DT_7seg[Z_NUM_S];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*10))
			{
				SEG_OXX = DT_7seg[Z_NUM_E];
				SEG_XOX = DT_7seg[Z_NUM_S];
				SEG_XXO = DT_7seg[Z_NUM_t];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*9))
			{
				SEG_OXX = DT_7seg[Z_NUM_S];
				SEG_XOX = DT_7seg[Z_NUM_t];
				SEG_XXO = DT_7seg[Z_NUM_BAR];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*8))
			{
				SEG_OXX = DT_7seg[Z_NUM_t];
				SEG_XOX = DT_7seg[Z_NUM_BAR];
				SEG_XXO = DT_7seg[Z_NUM_C];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*7))
			{
				SEG_OXX = DT_7seg[Z_NUM_BAR];
				SEG_XOX = DT_7seg[Z_NUM_C];
				SEG_XXO = DT_7seg[Z_NUM_o];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*6))
			{
				SEG_OXX = DT_7seg[Z_NUM_C];
				SEG_XOX = DT_7seg[Z_NUM_o];
				SEG_XXO = DT_7seg[Z_NUM_n];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*5))
			{
				SEG_OXX = DT_7seg[Z_NUM_o];
				SEG_XOX = DT_7seg[Z_NUM_n];
				SEG_XXO = DT_7seg[Z_NUM_d];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*4))
			{
				SEG_OXX = DT_7seg[Z_NUM_n];
				SEG_XOX = DT_7seg[Z_NUM_d];
				SEG_XXO = DT_7seg[Z_NUM_E];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*3))
			{
				SEG_OXX = DT_7seg[Z_NUM_d];
				SEG_XOX = DT_7seg[Z_NUM_E];
				SEG_XXO = DT_7seg[Z_NUM_n];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*2))
			{
				SEG_OXX = DT_7seg[Z_NUM_E];
				SEG_XOX = DT_7seg[Z_NUM_n];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*1))
			{
				SEG_OXX = DT_7seg[Z_NUM_n];
			}
			else
			{

			}
			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			return;
#endif
#ifdef _FOR_REPEAT_EFF_ENDURANCE_TEST_
			if (gu16_TimerMoveDisplay == 0)
			{
				gu16_TimerMoveDisplay = Z_TimerMoveDisplay_forTest;
			}

			if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*13))
			{
				SEG_XXO = DT_7seg[Z_NUM_t];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*12))
			{
				SEG_XOX = DT_7seg[Z_NUM_t];
				SEG_XXO = DT_7seg[Z_NUM_E];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*11))
			{
				SEG_OXX = DT_7seg[Z_NUM_t];
				SEG_XOX = DT_7seg[Z_NUM_E];
				SEG_XXO = DT_7seg[Z_NUM_S];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*10))
			{
				SEG_OXX = DT_7seg[Z_NUM_E];
				SEG_XOX = DT_7seg[Z_NUM_S];
				SEG_XXO = DT_7seg[Z_NUM_t];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*9))
			{
				SEG_OXX = DT_7seg[Z_NUM_S];
				SEG_XOX = DT_7seg[Z_NUM_t];
				SEG_XXO = DT_7seg[Z_NUM_BAR];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*8))
			{
				SEG_OXX = DT_7seg[Z_NUM_t];
				SEG_XOX = DT_7seg[Z_NUM_BAR];
				SEG_XXO = DT_7seg[Z_NUM_r];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*7))
			{
				SEG_OXX = DT_7seg[Z_NUM_BAR];
				SEG_XOX = DT_7seg[Z_NUM_r];
				SEG_XXO = DT_7seg[Z_NUM_E];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*6))
			{
				SEG_OXX = DT_7seg[Z_NUM_r];
				SEG_XOX = DT_7seg[Z_NUM_E];
				SEG_XXO = DT_7seg[Z_NUM_P];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*5))
			{
				SEG_OXX = DT_7seg[Z_NUM_E];
				SEG_XOX = DT_7seg[Z_NUM_P];
				SEG_XXO = DT_7seg[Z_NUM_E];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*4))
			{
				SEG_OXX = DT_7seg[Z_NUM_P];
				SEG_XOX = DT_7seg[Z_NUM_E];
				SEG_XXO = DT_7seg[Z_NUM_A];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*3))
			{
				SEG_OXX = DT_7seg[Z_NUM_E];
				SEG_XOX = DT_7seg[Z_NUM_A];
				SEG_XXO = DT_7seg[Z_NUM_t];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*2))
			{
				SEG_OXX = DT_7seg[Z_NUM_A];
				SEG_XOX = DT_7seg[Z_NUM_t];
			}
			else if (gu16_TimerMoveDisplay > ((Z_TimerMoveDisplay_forTest/14)*1))
			{
				SEG_OXX = DT_7seg[Z_NUM_t];
			}
			else
			{

			}
			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			return;
#endif
#ifdef _COLD_TEST_DEGREE_DISPLAY_
			if (gu8_Timer_TEM_Volt_Display != 0)
			{
				SEG_XOX = DT_7seg[gu8_PSM_TEM_Volt / 10];
				SEG_XXO = DT_7seg[gu8_PSM_TEM_Volt % 10];
			}
			else
			{
				if (gu8_Timer_SEG_TH == 0)				//			TH데이터 타이머
				{
					gu8_Timer_SEG_TH = Z_Timer_SEG_TH;
					if (gu8_Counter_SEG_TH == 2)		//			TH데이터 카운터에 따라
					{
						gu8_Counter_SEG_TH = 0;
					}
					else
					{
						gu8_Counter_SEG_TH = 2;
					}
				}

				if (gu8_Timer_SEG_TH >= 30)				//			4초 중 1초는 TH종류 표시
				{
					switch (gu8_Counter_SEG_TH)
					{
						case 0:
							SEG_OXX = DT_7seg[Z_NUM_C];
							SEG_XOX = DT_7seg[Z_NUM_o];
							SEG_XXO = DT_7seg[Z_NUM_L];
							break;
						case 2:
							SEG_OXX = DT_7seg[Z_NUM_O];
							SEG_XOX = DT_7seg[Z_NUM_u];
							SEG_XXO = DT_7seg[Z_NUM_t];
							break;
						default:
							break;
					}
				}
				else								//			4초 중 3초는 TH데이터 표시
				{
					mu8_OPENSHORT_Error = TH_Open_Short_Check(TH_DataArray[gu8_Counter_SEG_TH]->TH_ADC_AVERAGE);
					if (mu8_OPENSHORT_Error == 0)			//	정상상태
					{
						if (gu8_TimerSEG_TH_Limit == 0)		//
						{
							gu8_TimerSEG_TH_Limit = Z_TimerSEG_TH_Limit;
							mi16_Seg_buf_forSEGTH =	TH_DataArray[gu8_Counter_SEG_TH]->DEGREE;
						}

						if (mi16_Seg_buf_forSEGTH < 0)
						{
							SEG_OXX = DT_7seg[Z_NUM_BAR];
						}
						else if (mi16_Seg_buf_forSEGTH >= 100)
						{
							SEG_OXX = DT_7seg[(mi16_Seg_buf_forSEGTH / 100) % 10];
						}

						if (abs(mi16_Seg_buf_forSEGTH) >= 10)
						{
							SEG_XOX = DT_7seg[abs(mi16_Seg_buf_forSEGTH) / 10 % 10];
						}
						SEG_XXO = DT_7seg[abs(mi16_Seg_buf_forSEGTH) % 10];
#ifndef _FRONT_LED_UP_
					DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_DIMM;
#else
					DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_ON;
#endif
					}
					else									//	OPEN or SHORT
					{
						SEG_OXX = DT_7seg[Z_NUM_BAR];
						SEG_XOX = DT_7seg[Z_NUM_BAR];
						SEG_XXO = DT_7seg[Z_NUM_BAR];
					}
				}
			}
			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
#else
			if (gu8_TimerOverHeatDisplay != 0)
			{
				SEG_OXX = DT_7seg[Z_NUM_H];
				SEG_XOX = DT_7seg[Z_NUM_o];
				SEG_XXO = DT_7seg[Z_NUM_t];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			}
#ifdef _HOT_TEST_
			else if (gu8_TimerHotTest_DegreeDisplay != 0)
			{
				if (gu16_HotOut_Degree_Ave >= 100)
				{
//					SEG_OXX = DT_7seg[(gu16_HotOut_Degree_Ave / 100) % 10];
					SEG_OXX = DT_7seg[(gu16_HotOut_Degree_Ave / 100)];
				}
				if (gu16_HotOut_Degree_Ave >= 10)
				{
					SEG_XOX = DT_7seg[(gu16_HotOut_Degree_Ave / 10) % 10];
				}
				SEG_XXO = DT_7seg[gu16_HotOut_Degree_Ave % 10];
#ifndef _FRONT_LED_UP_
					DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_DIMM;
#else
					DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_ON;
#endif
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			}
#else
			else if (gu8_TimerWifiErrorDisplay != 0)
			{
				SEG_OXX = DT_7seg[Z_NUM_A];
				SEG_XOX = DT_7seg[Z_NUM_P];
				SEG_XXO = DT_7seg[gu8_WifiError % 10];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			}
			else if ((gu8_WaterSelect == HOT_WATER) && (F_HotWater_DegreeDisplay == SET) && (gu8_TimerHotTest_DegreeDisplay != 0))	//	추출 종료 후 평균온도 표시
			{
				if (gu16_HotOut_Degree_Ave >= 1000)
				{
					SEG_OXX = DT_7seg[(gu16_HotOut_Degree_Ave / 1000) % 10];
				}
				if (gu16_HotOut_Degree_Ave >= 100)
				{
					SEG_XOX = DT_7seg[(gu16_HotOut_Degree_Ave / 100) % 10];
				}
				SEG_XXO = DT_7seg[(gu16_HotOut_Degree_Ave / 10) % 10];
#ifndef _FRONT_LED_UP_
					DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_DIMM;
#else
					DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_ON;
#endif
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			}
#endif
			else if (gu8_TimerMySetFilker != 0)
			{
				if (gu16_CupSize_My >= 100)
				{
					SEG_OXX = DT_7seg[(gu16_CupSize_My / 100) % 10];
				}
				if (gu16_CupSize_My >= 10)
				{
					SEG_XOX = DT_7seg[(gu16_CupSize_My / 10) % 10];
				}
				SEG_XXO = DT_7seg[gu16_CupSize_My % 10];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
				DisplayControl_Data[Z_LED_mL].Display_CMD = LED_FADE_F;
			}
			else if (gu8_TimerEffluentAmount_Display != 0)	//		타이머 설정값이 있을 시
			{
				if (gu16_Effluent_Amount_SegDisplay >= 100)
				{
					SEG_OXX = DT_7seg[(gu16_Effluent_Amount_SegDisplay/100)%10];	//
				}
				if (gu16_Effluent_Amount_SegDisplay >= 10)
				{
					SEG_XOX = DT_7seg[(gu16_Effluent_Amount_SegDisplay/10)%10];
				}
				SEG_XXO = DT_7seg[Z_NUM_0];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
#ifndef _FRONT_LED_UP_
				DisplayControl_Data[Z_LED_mL].Display_CMD = LED_DIMM;
#else
				DisplayControl_Data[Z_LED_mL].Display_CMD = LED_ON;
#endif
			}
			else
			{
				if ((gu8_WaterSelect == HOT_WATER) && (F_HotDegreeDisplay == SET))
				{
					if (F_FirstHotEffluent == SET)
					{
#ifndef _HOT_TEMP_4STEP_
						if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR85_JP80)
						{
							SEG_XOX = DT_7seg[Z_NUM_8];
							SEG_XXO = DT_7seg[Z_NUM_5];
						}
						else if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR70_JP70)
						{
							SEG_XOX = DT_7seg[Z_NUM_7];
							SEG_XXO = DT_7seg[Z_NUM_0];
						}
						else
						{
							SEG_XOX = DT_7seg[Z_NUM_4];
							SEG_XXO = DT_7seg[Z_NUM_5];
						}
#else
							if (gu8_ModelType_forNational == MODEL_KOREA)
						{
							if (gu8_HotTempSetup == HOT_TEMP_SETUP_MY_HOT_TEMP)
							{
								if(gu8_My_HotTemp == 1)
								{
									SEG_XOX = DT_7seg[Z_NUM_4];
									SEG_XXO = DT_7seg[Z_NUM_5];
								}
								else if(gu8_My_HotTemp == 2)
								{
									SEG_XOX = DT_7seg[Z_NUM_5];
									SEG_XXO = DT_7seg[Z_NUM_0];
								}
								else if(gu8_My_HotTemp == 3)
								{
									SEG_XOX = DT_7seg[Z_NUM_5];
									SEG_XXO = DT_7seg[Z_NUM_5];
								}
								else if(gu8_My_HotTemp == 4)
								{
									SEG_XOX = DT_7seg[Z_NUM_6];
									SEG_XXO = DT_7seg[Z_NUM_0];
								}
								else if(gu8_My_HotTemp == 5)
								{
									SEG_XOX = DT_7seg[Z_NUM_6];
									SEG_XXO = DT_7seg[Z_NUM_5];
								}
								else if(gu8_My_HotTemp == 6)
								{
									SEG_XOX = DT_7seg[Z_NUM_7];
									SEG_XXO = DT_7seg[Z_NUM_0];
								}
								else if(gu8_My_HotTemp == 7)
								{
									SEG_XOX = DT_7seg[Z_NUM_7];
									SEG_XXO = DT_7seg[Z_NUM_5];
								}
								else if(gu8_My_HotTemp == 8)
								{
									SEG_XOX = DT_7seg[Z_NUM_8];
									SEG_XXO = DT_7seg[Z_NUM_0];
								}
								else if(gu8_My_HotTemp == 9)
								{
									SEG_XOX = DT_7seg[Z_NUM_8];
									SEG_XXO = DT_7seg[Z_NUM_5];
								}
								else if(gu8_My_HotTemp == 10)
								{
									SEG_XOX = DT_7seg[Z_NUM_9];
									SEG_XXO = DT_7seg[Z_NUM_0];
								}
								else if(gu8_My_HotTemp == 11)
								{
									SEG_XOX = DT_7seg[Z_NUM_9];
									SEG_XXO = DT_7seg[Z_NUM_0];
								}
								else if(gu8_My_HotTemp == 12)
								{
									SEG_XOX = DT_7seg[Z_NUM_9];
									SEG_XXO = DT_7seg[Z_NUM_5];
								}								
								else if(gu8_My_HotTemp == 13)
								{
									SEG_OXX = DT_7seg[Z_NUM_1];
									SEG_XOX = DT_7seg[Z_NUM_0];
									SEG_XXO = DT_7seg[Z_NUM_0];									
								}		
								else
								{
									//	占쏙옙占쏙옙처占쏙옙占쏙옙...占쏙옙占쏙옙占쏙옙占쏙옙 占쌘드에占쏙옙..
								}																																																																																						
							}
							else if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)
							{
								SEG_OXX = DT_7seg[Z_NUM_1];
								SEG_XOX = DT_7seg[Z_NUM_0];
								SEG_XXO = DT_7seg[Z_NUM_0];
							}
							else if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__95)
							{
								SEG_XOX = DT_7seg[Z_NUM_9];
								SEG_XXO = DT_7seg[Z_NUM_5];
							}
							else if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__90)
							{
								SEG_XOX = DT_7seg[Z_NUM_9];
								SEG_XXO = DT_7seg[Z_NUM_0];
							}
							else if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR85_JP80)
							{
								SEG_XOX = DT_7seg[Z_NUM_8];
								SEG_XXO = DT_7seg[Z_NUM_5];
							}
							else if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__80)
							{
								SEG_XOX = DT_7seg[Z_NUM_8];
								SEG_XXO = DT_7seg[Z_NUM_0];
							}
							else if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__75)
							{
								SEG_XOX = DT_7seg[Z_NUM_7];
								SEG_XXO = DT_7seg[Z_NUM_5];
							}
							else if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR70_JP70)
							{
								SEG_XOX = DT_7seg[Z_NUM_7];
								SEG_XXO = DT_7seg[Z_NUM_0];
							}
							else if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__65)
							{
								SEG_XOX = DT_7seg[Z_NUM_6];
								SEG_XXO = DT_7seg[Z_NUM_5];
							}
							else if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__60)
							{
								SEG_XOX = DT_7seg[Z_NUM_6];
								SEG_XXO = DT_7seg[Z_NUM_0];
							}
							else if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__55)
							{
								SEG_XOX = DT_7seg[Z_NUM_5];
								SEG_XXO = DT_7seg[Z_NUM_5];
							}
							else if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR__50)
							{
								SEG_XOX = DT_7seg[Z_NUM_5];
								SEG_XXO = DT_7seg[Z_NUM_0];
							}
							else
							{
								SEG_XOX = DT_7seg[Z_NUM_4];
								SEG_XXO = DT_7seg[Z_NUM_5];
							}
						}
						else							//	占싹븝옙占쏙옙
						{
							if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR100_JP90)
							{
								SEG_XOX = DT_7seg[Z_NUM_9];
								SEG_XXO = DT_7seg[Z_NUM_0];
							}
							else if (gu8_HotTempSetup == HOT_TEMP_SETUP_KOR85_JP80)
							{
								SEG_XOX = DT_7seg[Z_NUM_8];
								SEG_XXO = DT_7seg[Z_NUM_0];
							}
							else
							{
								SEG_XOX = DT_7seg[Z_NUM_7];
								SEG_XXO = DT_7seg[Z_NUM_0];
							}
						}
					}
					else
					{
						SEG_XOX = DT_7seg[Z_NUM_BAR];
						SEG_XXO = DT_7seg[Z_NUM_BAR];
					}
#endif
					DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
#ifndef _FRONT_LED_UP_
					DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_DIMM;
#else
					DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_ON;
#endif
				}
				else
				{
					if (gu8_TimerSleepMode != 0)
					{
						#if 0
						if (gu16_CupSize == CUPSIZE_HALFCUP)
						{
							SEG_OXX = DT_7seg[Z_NUM_1];
							SEG_XOX = DT_7seg[Z_NUM_2];
							SEG_XXO = DT_7seg[Z_NUM_0];
#ifndef _FRONT_LED_UP_
				DisplayControl_Data[Z_LED_mL].Display_CMD = LED_DIMM;
#else
				DisplayControl_Data[Z_LED_mL].Display_CMD = LED_ON;
#endif
						}
						else if (gu16_CupSize == CUPSIZE_1CUP)
						{
							if (gu8_ModelType_forNational == MODEL_KOREA)
							{
								SEG_OXX = DT_7seg[Z_NUM_2];
								SEG_XOX = DT_7seg[Z_NUM_5];
								SEG_XXO = DT_7seg[Z_NUM_0];
							}
							else
							{
								SEG_OXX = DT_7seg[Z_NUM_1];
								SEG_XOX = DT_7seg[Z_NUM_8];
								SEG_XXO = DT_7seg[Z_NUM_0];
							}
#ifndef _FRONT_LED_UP_
				DisplayControl_Data[Z_LED_mL].Display_CMD = LED_DIMM;
#else
				DisplayControl_Data[Z_LED_mL].Display_CMD = LED_ON;
#endif
						}
						else if (gu16_CupSize == CUPSIZE_2CUP)
						{
							if (gu8_ModelType_forNational == MODEL_KOREA)
							{
								SEG_OXX = DT_7seg[Z_NUM_5];
								SEG_XOX = DT_7seg[Z_NUM_0];
								SEG_XXO = DT_7seg[Z_NUM_0];
							}
							else
							{
								SEG_OXX = DT_7seg[Z_NUM_3];
								SEG_XOX = DT_7seg[Z_NUM_0];
								SEG_XXO = DT_7seg[Z_NUM_0];
							}
#ifndef _FRONT_LED_UP_
				DisplayControl_Data[Z_LED_mL].Display_CMD = LED_DIMM;
#else
				DisplayControl_Data[Z_LED_mL].Display_CMD = LED_ON;
#endif
						}
						#endif
						
						if (gu16_CupSize == CUPSIZE_ONE_BOTTLE)
						{
							SEG_OXX = LED_OFF;
							SEG_XOX = DT_7seg[Z_NUM_1];
							SEG_XXO = DT_7seg[Z_NUM_L];
						}
						else if (gu16_CupSize == CUPSIZE_CONTINUE)    //			연속추출
						//if (gu16_CupSize == CUPSIZE_CONTINUE)    //			연속추출
						{
							SEG_OXX = (U8)(DT_7seg[Z_NUM_SEG_a] | DT_7seg[Z_NUM_SEG_d] | DT_7seg[Z_NUM_SEG_e] | DT_7seg[Z_NUM_SEG_f]);
							SEG_XOX = (U8)(DT_7seg[Z_NUM_SEG_a] | DT_7seg[Z_NUM_SEG_d]);
							SEG_XXO = (U8)(DT_7seg[Z_NUM_SEG_a] | DT_7seg[Z_NUM_SEG_b] | DT_7seg[Z_NUM_SEG_c] | DT_7seg[Z_NUM_SEG_d]);
						}
						//else if (gu16_CupSize == CUPSIZE_MY)
						else 
						{
							//if (gu16_CupSize_My >= 100)
							if (gu16_CupSize >= 100)
							{
								//SEG_OXX = DT_7seg[(gu16_CupSize_My / 100) % 10];
								SEG_OXX = DT_7seg[(gu16_CupSize / 100) % 10];
							}
							//if (gu16_CupSize_My >= 10)
							if (gu16_CupSize >= 10)
							{
								//SEG_XOX = DT_7seg[(gu16_CupSize_My / 10) % 10];
								SEG_XOX = DT_7seg[(gu16_CupSize / 10) % 10];
							}
							//SEG_XXO = DT_7seg[gu16_CupSize_My % 10];
							SEG_XXO = DT_7seg[gu16_CupSize % 10];
#ifndef _FRONT_LED_UP_
							DisplayControl_Data[Z_LED_mL].Display_CMD = LED_DIMM;
#else
							DisplayControl_Data[Z_LED_mL].Display_CMD = LED_ON;
#endif
						}
						//else
						//{

						//}
#ifdef _AFTER_LPP_IMDCHANGE_
						if (gu8_TimerDefaultCupSize_Set_LEDFlic != 0)
						{
							DisplayControl_Data[Z_LED_mL].Display_CMD = LED_FADE_F;
							DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
						}
						else
						{
#endif
							DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
#ifdef _AFTER_LPP_IMDCHANGE_
						}
#endif
					}
				}
			}
#endif
			break;
		case SEG_FL_FLIC:						//		초기플러싱 필요 표시
			SEG_XOX = DT_7seg[Z_NUM_F];
			SEG_XXO = DT_7seg[Z_NUM_L];
			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			break;
		case SEG_COLDTIME:							//		냉각시간변경
//#ifdef _FOR_DEVELOP_
			if (gu16_Cooling_TargetDegree >= 10)							//@022
			{                                                               //@022
				SEG_XOX = DT_7seg[(gu16_Cooling_TargetDegree / 10) % 10];   //@022
			}                                                               //@022
			SEG_XXO = DT_7seg[gu16_Cooling_TargetDegree % 10];              //@022
			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;            //@022

//			if ((gu16_ColdTimeData/60) >= 100)
//			{
//				SEG_OXX = DT_7seg[((gu16_ColdTimeData/60) / 100) % 10];
//			}
//			if ((gu16_ColdTimeData/60) >= 10)
//			{
//				SEG_XOX = DT_7seg[((gu16_ColdTimeData/60) / 10) % 10];
//			}
//			SEG_XXO = DT_7seg[(gu16_ColdTimeData/60) % 10];
//			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
//#endif
			break;
		case SEG_TH:								//		TH데이터 표시
			if (gu8_TimerPIOff_Display != 0)		//@032
			{										//@032
				SEG_OXX = DT_7seg[Z_NUM_O];         //@032
				SEG_XOX = DT_7seg[Z_NUM_F];         //@032
				SEG_XXO = DT_7seg[Z_NUM_F];         //@032
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;	//@032
			} else 									//@032
			if (gu8_TimerDisplayEcoMode != 0)                               //@016
			{                                                               //@016
				if (F_CoolingEcoMode_Disable == SET)                        //@016
				{                                                           //@016
					SEG_XOX = DT_7seg[Z_NUM_d];                             //@016
					SEG_XXO = DT_7seg[Z_NUM_I];                             //@016
				}                                                           //@016
				else                                                        //@016
				{                                                           //@016
					SEG_XOX = DT_7seg[Z_NUM_E];                             //@016
					SEG_XXO = DT_7seg[Z_NUM_n];                             //@016
				}                                                           //@016
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;    //@016
			}                                                               //@016
			else if (gu8_TimerDisplayCoolingChange != 0)                    //@016
			{                                                               //@016
				if (F_Cooling_Up == SET)                                    //@016
				{                                                           //@016
					SEG_XOX = DT_7seg[Z_NUM_U];                             //@016
					SEG_XXO = DT_7seg[Z_NUM_P];                             //@016
				}                                                           //@016
				else                                                        //@016
				{                                                           //@016
					SEG_XOX = DT_7seg[Z_NUM_d];                             //@016
					SEG_XXO = DT_7seg[Z_NUM_o];                             //@016
				}                                                           //@016
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;    //@016
			}                                                               //@016
			else if (gu8_TimerDisplayFanChange != 0)                        //@016
			{                                                               //@016
				if (F_FAN_NoiseClear == SET)                                //@016
				{                                                           //@016
					SEG_XOX = DT_7seg[Z_NUM_1];                             //@016
					SEG_XXO = DT_7seg[Z_NUM_8];                             //@016
				}                                                           //@016
				else                                                        //@016
				{                                                           //@016
					SEG_XOX = DT_7seg[Z_NUM_2];                             //@016
					SEG_XXO = DT_7seg[Z_NUM_1];                             //@016
				}                                                           //@016
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;    //@016
			}                                                               //@016
			else                                                            //@016
			{																//@016
				if (gu8_Timer_SEG_TH == 0)				//			TH데이터 타이머
				{
					gu8_Timer_SEG_TH = Z_Timer_SEG_TH;
					if (++gu8_Counter_SEG_TH >= 6)		//			TH데이터 카운터에 따라
					{
						gu8_Counter_SEG_TH = 0;
					}
				}

				if (gu8_Timer_SEG_TH >= 30)				//			4초 중 1초는 TH종류 표시
				{
					switch (gu8_Counter_SEG_TH)
					{
						case 0:
							SEG_OXX = DT_7seg[Z_NUM_C];
							SEG_XOX = DT_7seg[Z_NUM_o];
							SEG_XXO = DT_7seg[Z_NUM_L];
							break;
						case 1:
							SEG_OXX = DT_7seg[Z_NUM_S];
							SEG_XOX = DT_7seg[Z_NUM_I];
							SEG_XXO = DT_7seg[Z_NUM_n];
							break;
						case 2:
							SEG_OXX = DT_7seg[Z_NUM_O];
							SEG_XOX = DT_7seg[Z_NUM_u];
							SEG_XXO = DT_7seg[Z_NUM_t];
							break;
						case 3:
							SEG_OXX = DT_7seg[Z_NUM_H];
							SEG_XOX = DT_7seg[Z_NUM_I];
							SEG_XXO = DT_7seg[Z_NUM_n];
							break;
						case 4:
							SEG_OXX = DT_7seg[Z_NUM_H];
							SEG_XOX = DT_7seg[Z_NUM_u];
							SEG_XXO = DT_7seg[Z_NUM_t];
							break;
						case 5:
							SEG_OXX = DT_7seg[Z_NUM_H];
							SEG_XOX = DT_7seg[Z_NUM_E];
							SEG_XXO = DT_7seg[Z_NUM_A];
							break;
						default:
							break;
					}
				}
				else									//			4초 중 3초는 TH데이터 표시
				{
					mu8_OPENSHORT_Error = TH_Open_Short_Check(TH_DataArray[gu8_Counter_SEG_TH]->TH_ADC_AVERAGE);
					if (mu8_OPENSHORT_Error == 0)			//	정상상태
					{
						if (gu8_TimerSEG_TH_Limit == 0)		//
						{
							gu8_TimerSEG_TH_Limit = Z_TimerSEG_TH_Limit;
							mi16_Seg_buf_forSEGTH = TH_DataArray[gu8_Counter_SEG_TH]->DEGREE;
						}

						if (mi16_Seg_buf_forSEGTH < 0)
						{
							SEG_OXX = DT_7seg[Z_NUM_BAR];
						}
						else if (mi16_Seg_buf_forSEGTH >= 100)
						{
							SEG_OXX = DT_7seg[(mi16_Seg_buf_forSEGTH / 100) % 10];
						}

						if (abs(mi16_Seg_buf_forSEGTH) >= 10)
						{
							SEG_XOX = DT_7seg[(abs(mi16_Seg_buf_forSEGTH) / 10) % 10];
						}
						SEG_XXO = DT_7seg[abs(mi16_Seg_buf_forSEGTH) % 10];
#ifndef _FRONT_LED_UP_
						DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_DIMM;
#else
						DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_ON;
#endif
					}
					else								//	OPEN or SHORT
					{
						SEG_OXX = DT_7seg[Z_NUM_BAR];
						SEG_XOX = DT_7seg[Z_NUM_BAR];
						SEG_XXO = DT_7seg[Z_NUM_BAR];
					}
				}
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			}
			break;
		case SEG_MONITOR:							//		추출 횟수 및 추출량 표시
			if (gu8_Timer_SEG_MONITOR == 0)			//			타이머
			{
				gu8_Timer_SEG_MONITOR = Z_Timer_SEG_MONITOR;
				if (++gu8_Counter_SEG_MONITOR >= 3)	//			물 종류 카운터에 따라
				{
					gu8_Counter_SEG_MONITOR = 0;
				}
			}

			if (gu8_Timer_SEG_MONITOR >= 60)		//			7초 중 1초는 물 종류 표시
			{
				switch (gu8_Counter_SEG_MONITOR)
				{
					case 1:
						SEG_OXX = DT_7seg[Z_NUM_C];
						SEG_XOX = DT_7seg[Z_NUM_o];
						SEG_XXO = DT_7seg[Z_NUM_L];
						break;
					case 2:
						SEG_OXX = DT_7seg[Z_NUM_H];
						SEG_XOX = DT_7seg[Z_NUM_o];
						SEG_XXO = DT_7seg[Z_NUM_t];
						break;
					case 0:
						SEG_OXX = DT_7seg[Z_NUM_A];
						SEG_XOX = DT_7seg[Z_NUM_b];
						SEG_XXO = DT_7seg[Z_NUM_E];
						break;
					default:
						break;
				}
			}
			else if (gu8_Timer_SEG_MONITOR >= 30)	//			7초 중 3~6초는 추출 횟수
			{
				if (gu16_Effluent_Count[gu8_Counter_SEG_MONITOR] >= 100)
				{
					SEG_OXX = DT_7seg[(gu16_Effluent_Count[gu8_Counter_SEG_MONITOR] / 100) % 10];
				}
				if (gu16_Effluent_Count[gu8_Counter_SEG_MONITOR] >= 10)
				{
					SEG_XOX = DT_7seg[(gu16_Effluent_Count[gu8_Counter_SEG_MONITOR] / 10) % 10];
				}
				SEG_XXO = DT_7seg[gu16_Effluent_Count[gu8_Counter_SEG_MONITOR] % 10];
			}
			else									//		0~2초는 추출량. 1L 단위 기준
			{
				if (gu16_Effluent_Amount[gu8_Counter_SEG_MONITOR] >= 100)
				{
					SEG_OXX = DT_7seg[(gu16_Effluent_Amount[gu8_Counter_SEG_MONITOR] / 100) % 10];
				}
				if (gu16_Effluent_Amount[gu8_Counter_SEG_MONITOR] >= 10)
				{
					SEG_XOX = DT_7seg[(gu16_Effluent_Amount[gu8_Counter_SEG_MONITOR] / 10) % 10];
				}
				SEG_XXO = DT_7seg[gu16_Effluent_Amount[gu8_Counter_SEG_MONITOR] % 10];
			}
			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			break;
		case SEG_FRONTTEST:
			/*	제성용	*/
//			if (PSM5 == CLEAR)
//			{
//				SEG_OXX = DT_7seg[Z_NUM_E];
//				SEG_XOX = DT_7seg[Z_NUM_4];
//				SEG_XXO = DT_7seg[Z_NUM_4];
//			}
//			else
//			{
//				SEG_OXX = DT_7seg[Z_NUM_C];
//				SEG_XOX = DT_7seg[Z_NUM_H];
//				SEG_XXO = DT_7seg[Z_NUM_P];
//			}
//			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
//			return;
			/*	제성용 끝	*/
			if (F_FrontTest_EffluentButton == SET)
			{
				if (gu16_Timer_FrontTest_Seg != 0)
				{
					if (Z_Timer_FrontTest_Seg >= gu16_Timer_FrontTest_Seg)
					{
						mu16_temp = Z_Timer_FrontTest_Seg - gu16_Timer_FrontTest_Seg;
					}
					mu16_FrontTest = mu16_temp / (Z_Timer_FrontTest_Seg/24);
					if (mu16_FrontTest < 8)
					{
						SEG_OXX = DT_FrontTest[mu16_FrontTest];
						SEG_XOX = DT_7seg[Z_NUM_8];
						SEG_XXO = DT_7seg[Z_NUM_8];
					}

					if ((mu16_FrontTest >= 8) && (mu16_FrontTest < 16))
					{
						SEG_XOX = DT_FrontTest[(mu16_FrontTest-8)];
						SEG_XXO = DT_7seg[Z_NUM_8];
					}

					if (mu16_FrontTest >= 16)
					{
						SEG_XXO = DT_FrontTest[(mu16_FrontTest-16)];
					}
				}
				else
				{
					if (gu8_ModelType_forCHP == MODEL_CHP)
					{
						SEG_OXX = DT_7seg[Z_NUM_C];
						SEG_XOX = DT_7seg[Z_NUM_H];
						SEG_XXO = DT_7seg[Z_NUM_P];
					}
					else
					{
						SEG_OXX = DT_7seg[Z_NUM_C];
						SEG_XOX = DT_7seg[Z_NUM_P];
					}
				}
			}
			else
			{
				SEG_OXX = DT_7seg[Z_NUM_8];
				SEG_XOX = DT_7seg[Z_NUM_8];
				SEG_XXO = DT_7seg[Z_NUM_8];
			}
			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			break;
		case SEG_LINETEST:
			if (F_TestMode_LineAuto == CLEAR)
			{
				if (gu16_TimerFCT_Incomplete_Display != 0)	//@027
				{											//@027
					if (F_LineFrontTest_Amount_Complete == CLEAR)	//@027
					{										//@027
						SEG_OXX = DT_7seg[Z_NUM_b];       	//@027
						SEG_XOX = DT_7seg[Z_NUM_t];         //@027
						SEG_XXO = DT_7seg[Z_NUM_n];         //@027
					}										//@027
					else if (F_LineFrontTest_Hot_Complete == CLEAR)	//@027
					{										//@027
						if (gu8_ModelType_forCHP == MODEL_CHP)//@027
						{									//@027
							SEG_OXX = DT_7seg[Z_NUM_H];     //@027
							SEG_XOX = DT_7seg[Z_NUM_o];     //@027
							SEG_XXO = DT_7seg[Z_NUM_t];     //@027
						}									//@027
						else								//@027
						{									//@027
							SEG_OXX = DT_7seg[Z_NUM_b];     //@027
							SEG_XOX = DT_7seg[Z_NUM_t];     //@027
							SEG_XXO = DT_7seg[Z_NUM_n];     //@027
						}									//@027
					}										//@027
					else if (F_LineFrontTest_Cold_Complete == CLEAR)	//@027
					{										//@027
						SEG_OXX = DT_7seg[Z_NUM_C];       	//@027
						SEG_XOX = DT_7seg[Z_NUM_o];         //@027
						SEG_XXO = DT_7seg[Z_NUM_L];         //@027
					}										//@027
					else if (F_LineFrontTest_Pure_Complete == CLEAR)	//@027
					{										//@027
						SEG_OXX = DT_7seg[Z_NUM_P];       	//@027
						SEG_XOX = DT_7seg[Z_NUM_u];         //@027
						SEG_XXO = DT_7seg[Z_NUM_r];         //@027
					}										//@027
					else if ((REED_SW_COVER_Ready == CLEAR) || (INPUT_REED_SW_COVER == SET))	//@027
					{										//@027
						SEG_OXX = DT_7seg[Z_NUM_t];       	//@027
						SEG_XOX = DT_7seg[Z_NUM_o];         //@027
						SEG_XXO = DT_7seg[Z_NUM_P];         //@027
					}										//@027
					else if ((REED_SW_WATERFLOW_Ready == CLEAR) || (INPUT_REED_SW_FAUCET == CLEAR))	//@027
					{										//@027
						SEG_OXX = DT_7seg[Z_NUM_U];       	//@027
						SEG_XOX = DT_7seg[Z_NUM_r];         //@027
						SEG_XXO = DT_7seg[Z_NUM_o];         //@027
					}										//@027
					else if ((REED_SW_FILTER_Ready == CLEAR) || (INPUT_REED_SW_FILTER == CLEAR))	//@027
					{										//@027
						SEG_OXX = DT_7seg[Z_NUM_F];       	//@027
						SEG_XOX = DT_7seg[Z_NUM_I];         //@027
						SEG_XXO = DT_7seg[Z_NUM_L];         //@027
					}										//@027
					else if ((F_WIFI_Pairing_forLineTest == CLEAR) && (gu16_TimerFCT_Incomplete_Display_forPASECO != 0))	//@027
					{										//@027
						SEG_OXX = DT_7seg[Z_NUM_I];       	//@027
						SEG_XOX = DT_7seg[Z_NUM_F];         //@027
						SEG_XXO = DT_7seg[Z_NUM_I];         //@027
					}										//@027
					else if (gu16_FlowDecide_Hz_forLineAuto == 0)	//@027
					{										//@027
						SEG_OXX = DT_7seg[Z_NUM_F];       	//@027
						SEG_XOX = DT_7seg[Z_NUM_L];         //@027
						SEG_XXO = DT_7seg[Z_NUM_o];         //@027
					}										//@027
					else if ((F_LineFrontTest_1stNos_VV_Complete == CLEAR) && (gu16_TimerFCT_Incomplete_Display_forPASECO != 0))	//@027
					{										//@027
						SEG_OXX = DT_7seg[Z_NUM_n];       	//@027
						SEG_XOX = DT_7seg[Z_NUM_o];         //@027
						SEG_XXO = DT_7seg[Z_NUM_S];         //@027
					}										//@027
					else									//@027
					{                                       //@027
                                                            //@027
					}                                       //@027
					DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;	  //@027
					return;									//@027
				}											//@027
				else if (gu8_TimerFrontTest_1stNosDisplay != 0)	//@023
				{											//@023
					if (F_LineFrontTest_1stNos_VV == SET)   //@023
					{                                       //@023
						SEG_OXX = DT_7seg[Z_NUM_BAR];       //@023
						SEG_XOX = DT_7seg[Z_NUM_C];         //@023
						SEG_XXO = DT_7seg[Z_NUM_L];         //@023
					}                                       //@023
					else                                    //@023
					{                                       //@023
						SEG_OXX = DT_7seg[Z_NUM_BAR];       //@023
						SEG_XOX = DT_7seg[Z_NUM_O];         //@023
						SEG_XXO = DT_7seg[Z_NUM_P];         //@023
					}                                       //@023
				}                                           //@023
				else if (F_LineFrontTest_Effluent_Oper == CLEAR)
				{
					SEG_OXX = DT_7seg[Z_NUM_8];
					SEG_XOX = DT_7seg[Z_NUM_8];
					SEG_XXO = DT_7seg[Z_NUM_8];
				}
				else
				{
//					mu16_Seg_buf = gu16_FlowDecide_Hz / 10;
					if (gu8_TimerLineTest_SegUpdate == 0)
					{
						gu8_TimerLineTest_SegUpdate = Z_TimerLineTest_SegUpdate;
						if (gu8_ModelType_forNational == MODEL_KOREA)
						{
							gu16_LineTest_Seg = gu16_FlowDecide_Hz / 10;
						}
						else						//	일본향
						{
							gu16_LineTest_Seg = gu16_FlowDecide_Hz / 10 / 2;	//	일본향은 저유량센서이므로 더 높은 Hz가 입력되어 나누기2
						}
					}

					if (gu16_LineTest_Seg > 999)
					{
						gu16_LineTest_Seg = 999;
					}

					if (gu16_LineTest_Seg >= 100)
					{
						SEG_OXX = DT_7seg[(gu16_LineTest_Seg / 100) % 10];
					}
					if (gu16_LineTest_Seg >= 10)
					{
						SEG_XOX = DT_7seg[(gu16_LineTest_Seg / 10) % 10];
					}
					SEG_XXO = DT_7seg[gu16_LineTest_Seg % 10];
				}
			}
			else
			{
				if (gu8_ModelType_forNational == MODEL_KOREA)
				{
					if (gu8_ModelType_forCHP == MODEL_CHP)
					{
						SEG_OXX = DT_7seg[Z_NUM_C];
						SEG_XOX = DT_7seg[Z_NUM_H];
						SEG_XXO = DT_7seg[Z_NUM_P];
					}
					else
					{
						SEG_OXX = DT_7seg[Z_NUM_C];
						SEG_XOX = DT_7seg[Z_NUM_P];
					}
				}
				else								//	일본향
				{
					SEG_OXX = DT_7seg[Z_NUM_J];
					SEG_XOX = DT_7seg[Z_NUM_P];
				}
			}
			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			break;
		case SEG_PBATEST:
			if (gu8_TimerFCTSet != 0)				//			테스트모드1,2 전환 표시 타이머 중인 경우
			{
				if (F_TestMode_PBA_2 == SET)		//				테스트모드2
				{
					SEG_OXX = DT_7seg[Z_NUM_t];
					SEG_XOX = DT_7seg[Z_NUM_E];
					SEG_XXO = DT_7seg[Z_NUM_2];
				}
				else								//				테스트모드1
				{
					SEG_OXX = DT_7seg[Z_NUM_t];
					SEG_XOX = DT_7seg[Z_NUM_E];
					SEG_XXO = DT_7seg[Z_NUM_1];
				}
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			}
			else if (gu8_TimerPBATest_HotLoadDisplay != 0)
			{
				SEG_OXX = DT_7seg[Z_NUM_H];
				SEG_XOX = DT_7seg[Z_NUM_o];
				SEG_XXO = DT_7seg[gu8_PBATest_HotLoad];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			}
			else if (gu8_TimerPBATest_ColdLoadDisplay != 0)
			{
				SEG_OXX = DT_7seg[Z_NUM_C];
				SEG_XOX = DT_7seg[Z_NUM_o];
				SEG_XXO = DT_7seg[gu8_PBATest_ColdLoad];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			}
			else if (gu8_TimerPBATest_PureLoadDisplay != 0)
			{
				SEG_OXX = DT_7seg[Z_NUM_P];
				SEG_XOX = DT_7seg[Z_NUM_u];
				SEG_XXO = DT_7seg[gu8_PBATest_PureLoad];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			}
			else if (gu8_ErrorCode != 0)
			{
				SEG_OXX = DT_7seg[Z_NUM_E];
				SEG_XOX = DT_7seg[(gu8_ErrorCode & 0xf0) >> 4];
				SEG_XXO = DT_7seg[(gu8_ErrorCode & 0x0f)];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			}
			else if (gu8_TimerPBATest_FanDisplay != 0)
			{
				SEG_OXX = DT_7seg[Z_NUM_F];
				SEG_XOX = DT_7seg[Z_NUM_A];
				SEG_XXO = DT_7seg[gu8_PBATest_FanTest];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			}
			else if (F_PBATest_FanFeedBack_Error == SET)
			{
				SEG_OXX = DT_7seg[Z_NUM_F];
				SEG_XOX = DT_7seg[Z_NUM_A];
				SEG_XXO = DT_7seg[Z_NUM_n];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			}
			else if (F_HeaterFeedBack_Error == SET)
			{
				SEG_OXX = DT_7seg[Z_NUM_H];
				SEG_XOX = DT_7seg[Z_NUM_E];
				SEG_XXO = DT_7seg[Z_NUM_A];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			}
			else if (F_TEMFeedBack_Error == SET)
			{
				SEG_XOX = DT_7seg[Z_NUM_t];
				SEG_XXO = DT_7seg[Z_NUM_E];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			}
			else if (F_24VFeedBack_Error == SET)
			{
				SEG_XOX = DT_7seg[Z_NUM_2];
				SEG_XXO = DT_7seg[Z_NUM_4];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			}
			else if (INPUT_REED_SW_COVER == SET)
			{
				SEG_OXX = DT_7seg[Z_NUM_SEG_a];
				SEG_XOX = DT_7seg[Z_NUM_SEG_a];
				SEG_XXO = DT_7seg[Z_NUM_SEG_a];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			}
			else if (INPUT_REED_SW_FILTER == SET)
			{
				SEG_OXX = DT_7seg[Z_NUM_SEG_g];
				SEG_XOX = DT_7seg[Z_NUM_SEG_g];
				SEG_XXO = DT_7seg[Z_NUM_SEG_g];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			}
			else if (INPUT_REED_SW_FAUCET == SET)
			{
				SEG_OXX = DT_7seg[Z_NUM_SEG_d];
				SEG_XOX = DT_7seg[Z_NUM_SEG_d];
				SEG_XXO = DT_7seg[Z_NUM_SEG_d];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			}
//			else if ((gu16_FlowDecide_Hz >= 200) && (gu16_FlowDecide_Hz <= 1000))
//			{
//				SEG_OXX = DT_7seg[Z_NUM_F];
//				SEG_XOX = DT_7seg[Z_NUM_L];
//				SEG_XXO = DT_7seg[Z_NUM_O];
//				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
//			}
			break;
		case SEG_LINETEST2:
			if (gu8_ErrorCode != 0)
			{
				SEG_OXX = DT_7seg[Z_NUM_E];
				SEG_XOX = DT_7seg[(gu8_ErrorCode & 0xf0) >> 4];
				SEG_XXO = DT_7seg[(gu8_ErrorCode & 0x0f)];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
			}
			else if (F_LineTest2_HeaterResult_NG == SET)
			{
				SEG_OXX = DT_7seg[Z_NUM_BAR];
				SEG_XOX = DT_7seg[Z_NUM_BAR];
				SEG_XXO = DT_7seg[Z_NUM_BAR];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			}
			else if (F_LineTest2_HeaterResult_OK == SET)
			{
				SEG_OXX = DT_7seg[Z_NUM_O];
				SEG_XOX = DT_7seg[Z_NUM_O];
				SEG_XXO = DT_7seg[Z_NUM_O];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			}
			else
			{
				SEG_OXX = DT_7seg[Z_NUM_t];
				SEG_XOX = DT_7seg[Z_NUM_E];
				SEG_XXO = DT_7seg[Z_NUM_2];
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			}
			break;
//		case SEG_COLDTANK:
//			SEG_OXX = DT_7seg[Z_NUM_t];
//			SEG_XOX = DT_7seg[Z_NUM_A];
//			SEG_XXO = DT_7seg[Z_NUM_n];
//			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_FADE_F;
//			break;
		case SEG_MANAGE:
			if (F_SelfService == SET)
			{
				SEG_OXX = DT_7seg[Z_NUM_S];
				SEG_XOX = DT_7seg[Z_NUM_E];
			}
			else
			{
				SEG_OXX = DT_7seg[Z_NUM_C];
				SEG_XOX = DT_7seg[Z_NUM_O];
			}
			DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
			break;
		
			
		case SEG_AD_FEEDBACK:
			
			// 타이머가 0이 되면 상태 전환
			if (gu8_Timer_AdFeedback_Loading != 0)
			{
				if(gu8_Timer_AdFeedback_Loading >= 20)
				{
					SEG_OXX = DT_7seg[Z_NUM_BAR];
				}
				else if(gu8_Timer_AdFeedback_Loading >= 10)
				{
					SEG_OXX = DT_7seg[Z_NUM_BAR];
					SEG_XOX = DT_7seg[Z_NUM_BAR];
				}
				else
				{
					// 켜짐
					SEG_OXX = DT_7seg[Z_NUM_BAR];
					SEG_XOX = DT_7seg[Z_NUM_BAR];
					SEG_XXO = DT_7seg[Z_NUM_BAR];
				}
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;

				gu8_Timer_Smps_Feedback_Display = Z_TIMER_SMPS_FEEDBACK_DISPLAY;
			}
			else
			{
				#ifdef SMPS_VOLTAGE_AND_TEM_CURRENT_DISPLAY
				// 1초 주기로 상태 전환 (0 → 1 → 2 → 3 → 0)
				if(gu8_Timer_Smps_Feedback_Display == 0)
				{
					gu8_Timer_Smps_Feedback_Display = Z_TIMER_SMPS_FEEDBACK_DISPLAY;

					mu8_AdFeedback_State++;
					if (mu8_AdFeedback_State >= 4)
					{
						mu8_AdFeedback_State = 0;
					}
					
				}
				else
				{

				}

				// 상태에 따라 표시
				switch (mu8_AdFeedback_State)
				{
					case 0:  // VOL 텍스트 표시
						SEG_OXX = DT_7seg[Z_NUM_U];  // V (V가 없으면 다른 문자 사용)
						SEG_XOX = DT_7seg[Z_NUM_O];  // O
						SEG_XXO = DT_7seg[Z_NUM_L];  // L
						break;
						
					case 1:  // VOL 값 표시

						mu16_VolValue = calculate_smps_voltage_feedback(ADC_RESULT_VOLTAGE_FB);  // VOL 값
						mu8_Thousands = (U8)((mu16_VolValue / 1000) % 10);
						mu8_Hundreds = (U8)((mu16_VolValue / 100) % 10);
						mu8_Tens = (U8)((mu16_VolValue / 10) % 10);
						
						if (mu16_VolValue >= 1000)
						{
							SEG_OXX = DT_7seg[mu8_Thousands];
						}
						else
						{
							SEG_OXX = DT_7seg[Z_NUM_X];  // 빈칸
						}
						
						if (mu16_VolValue >= 100)
						{
							SEG_XOX = DT_7seg[mu8_Hundreds];
						}
						else
						{
							SEG_XOX = DT_7seg[Z_NUM_X];  // 빈칸
						}
						
						if (mu16_VolValue >= 10)
						{
							SEG_XXO = DT_7seg[mu8_Tens];
						}
						else
						{
							SEG_XXO = DT_7seg[Z_NUM_X];  // 빈칸
						}
						break;
						
					case 2:  // TEM 텍스트 표시
						SEG_OXX = DT_7seg[Z_NUM_t];  // T
						SEG_XOX = DT_7seg[Z_NUM_E];  // E
						SEG_XXO = DT_7seg[Z_NUM_n];  // M (n 대신 다른 문자 사용 가능)
						break;
						
					case 3:  // TEM 값 표시

						mu16_TemValue = calculate_tem_current(ADC_RESULT_TEM_FB);

						mu8_Thousands = (U8)((mu16_TemValue / 1000) % 10);
						mu8_Hundreds = (U8)((mu16_TemValue / 100) % 10);
						mu8_Tens = (U8)((mu16_TemValue / 10) % 10);
						
						if (mu16_TemValue >= 1000)
						{
							SEG_OXX = DT_7seg[mu8_Thousands];
						}
						else
						{
							SEG_OXX = DT_7seg[Z_NUM_X];  // 빈칸
						}
						
						if (mu16_TemValue >= 100)
						{
							SEG_XOX = DT_7seg[mu8_Hundreds];
						}
						else
						{
							SEG_XOX = DT_7seg[Z_NUM_X];  // 빈칸
						}
						
						if (mu16_TemValue >= 10)
						{
							SEG_XXO = DT_7seg[mu8_Tens];
						}
						else
						{
							SEG_XXO = DT_7seg[Z_NUM_X];  // 빈칸
						}
						break;
						
					default:
						break;
				}
				
				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
					break;
			
			
				default:
					break;
				#else
				
				// MIN/MAX 범위 확인 (±10%)
				// VOL 값 범위 확인
				if (check_smps_voltage_range(SMPS_24V_FEEDBACK_VOLTAGE_MIN, SMPS_24V_FEEDBACK_VOLTAGE_MAX) != 0)
				{
					SEG_XXO = DT_7seg[Z_NUM_1];
				}					
				// TEM 값 범위 확인
				else if (check_tem_current_range(TEM_24V_FEEDBACK_CURRENT_MIN) != 0)
				{
					SEG_XXO = DT_7seg[Z_NUM_2];
				}
				else 
				{
					SEG_XXO = DT_7seg[Z_NUM_0];
				}

				DisplayControl_Data[Z_SEGMENT].Display_CMD = LED_ON;
				#endif
			}
	}

	if ((gu8_SegData_Before[0].byte != gu8_SegData[0].byte)
		|| (gu8_SegData_Before[1].byte != gu8_SegData[1].byte)
		|| (gu8_SegData_Before[2].byte != gu8_SegData[2].byte))
	{
		F_CommunicationCMD = SET;
	}

	gu8_SegData_Before[0].byte = gu8_SegData[0].byte;
	gu8_SegData_Before[1].byte = gu8_SegData[1].byte;
	gu8_SegData_Before[2].byte = gu8_SegData[2].byte;
}

/// @brief
/// @param    void
/// @return   void
static void LED_PURE(void)
{
	switch (DisplayControl_Data[Z_LED_PURE].Operation_CMD)
	{
//		case LED_OFF:
//			break;
//		case LED_FADE_F:
//			DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_FADE_F;
//			break;
//		case LED_FADE_F_S:
//			DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_FADE_F_S;
//			break;
//		case LED_FLIC:
//			DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_FLIC;
//			break;
		case LED_STATUS:		
		case LED_EFF:
			if (gu8_WaterSelect == PURE_WATER)
			{
				DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_ON;
			}
			else
			{
				//JHKO_260417
				DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_OFF;
				//DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_DIMM;
			}
			break;
		case LED_BOOT:
			DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_FADE_F;
			break;
//		case LED_DIMM:
//			DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_DIMM;
//			break;
//		case LED_ON:
//			DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_ON;
//			break;
		case LED_FRONTTEST:
			if (gu8_FrontTest_PureButton_Counter < 1)
			{
				DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_ON;
			}
			break;
		case LED_LINETEST:
			if (F_TestMode_LineAuto == CLEAR)
			{
				if (F_LineFrontTest_Pure_Select == SET)
				{
					DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_ON;
				}
				else
				{
					DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_DIMM_LOW;
				}
			}
			break;
		case LED_ALL_LOCK:
			if (gu8_ModelType_forCHP == MODEL_CHP)
			{
				if (gu8_TimerAllLockDisplay != 0)
				{
					DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_FADE_F;
				}
				else
				{
					DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_DIMM;
				}
			}
			else
			{
				DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_DIMM;
			}
			break;
		case LED_PBATEST:
			if (F_TestMode_PBA_2 == SET)
			{
				DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_DIMM;
			}
			else
			{
				if (F_EEPROM_CommunicationError == SET)
				{
					DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_FADE_F;
				}
				else
				{
					DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_DIMM;
				}
			}
			break;
		case LED_STEMODE:
			if ((gu16_TimerKITMode > (Z_TimerKITMode-30)) || (gu16_TimerKITMode < 30))
			{
				DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_FADE_F_S;
			}
			break;
		case LED_TOPCOVER:
			DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_DIMM;
			break;
		case LED_LINETEST2:
			if (F_LineTest2_Valve == SET)
			{
				DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_ON;
			}
			else
			{
				DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_DIMM_LOW;
			}
			break;
		case LED_WIFI_READY:											//@033
			DisplayControl_Data[Z_LED_PURE].Display_CMD = LED_DIMM;		//@033
			break;														//@033
		default:
			DisplayControl_Data[Z_LED_PURE].Display_CMD = DisplayControl_Data[Z_LED_PURE].Operation_CMD;
			break;
	}
}

/// @brief
/// @param    void
/// @return   void
static void LED_AMOUNT(void)
{
	switch (DisplayControl_Data[Z_LED_AMOUNT].Operation_CMD)
	{
//		case LED_OFF:
//			break;
//		case LED_FADE_F:
//			DisplayControl_Data[Z_LED_AMOUNT].Display_CMD = LED_FADE_F;
//			break;
//		case LED_FADE_F_S:
//			DisplayControl_Data[Z_LED_AMOUNT].Display_CMD = LED_FADE_F_S;
//			break;
//		case LED_FLIC:
//			DisplayControl_Data[Z_LED_AMOUNT].Display_CMD = LED_FLIC;
//			break;
		case LED_STATUS:
			if (gu8_TimerCupSelect != 0)
			{
				DisplayControl_Data[Z_LED_AMOUNT].Display_CMD = LED_ON;
			}
			else
			{
				//JHKO_260417
				DisplayControl_Data[Z_LED_AMOUNT].Display_CMD = LED_OFF;
				//DisplayControl_Data[Z_LED_AMOUNT].Display_CMD = LED_DIMM;  
			}
			break;
		case LED_EFF:

			DisplayControl_Data[Z_LED_AMOUNT].Display_CMD = LED_DIMM;

			break;
		case LED_BOOT:
			DisplayControl_Data[Z_LED_AMOUNT].Display_CMD = LED_FADE_F;
			break;
//		case LED_DIMM:
//			DisplayControl_Data[Z_LED_AMOUNT].Display_CMD = LED_DIMM;
//			break;
//		case LED_ON:
//			DisplayControl_Data[Z_LED_AMOUNT].Display_CMD = LED_ON;
//			break;
		case LED_FRONTTEST:
			if (gu8_FrontTest_AmountButton_Counter < 1)
			{
				DisplayControl_Data[Z_LED_AMOUNT].Display_CMD = LED_ON;
			}
			break;
		case LED_LINETEST:
			if (F_TestMode_LineAuto == CLEAR)
			{
				if (F_LineFrontTest_Amount_Select == SET)
				{
					DisplayControl_Data[Z_LED_AMOUNT].Display_CMD = LED_ON;
				}
				else
				{
					DisplayControl_Data[Z_LED_AMOUNT].Display_CMD = LED_DIMM_LOW;
				}
			}
			break;
		case LED_PBATEST:
			if (F_TestMode_PBA_2 == SET)
			{
				DisplayControl_Data[Z_LED_AMOUNT].Display_CMD = LED_DIMM;
			}
			else
			{
				if ((gi16_OutDoorAirSensor.DEGREE >= Z_TEMP15) && (gi16_OutDoorAirSensor.DEGREE <= Z_TEMP35))
				{
					DisplayControl_Data[Z_LED_AMOUNT].Display_CMD = LED_ON;
				}
			}
			break;
		case LED_SOUND:
			if (gu8_TimerSoundChange != 0)
			{
				DisplayControl_Data[Z_LED_AMOUNT].Display_CMD = LED_ON;
			}
			else
			{
				DisplayControl_Data[Z_LED_AMOUNT].Display_CMD = LED_DIMM;
			}
			break;
		case LED_TOPCOVER:
			DisplayControl_Data[Z_LED_AMOUNT].Display_CMD = LED_DIMM;
			break;
		default:
			DisplayControl_Data[Z_LED_AMOUNT].Display_CMD = DisplayControl_Data[Z_LED_AMOUNT].Operation_CMD;
			break;
	}
}

/// @brief
/// @param    void
/// @return   void
static void LED_COLD(void)
{
	switch (DisplayControl_Data[Z_LED_COLD].Operation_CMD)
	{
//		case LED_OFF:
//			break;
//		case LED_FADE_F:
//			DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_FADE_F;
//			break;
//		case LED_FADE_F_S:
//			DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_FADE_F_S;
//			break;
//		case LED_FLIC:
//			DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_FLIC;
//			break;
		case LED_STATUS:

		case LED_EFF:
			if (gu8_WaterSelect == COLD_WATER)
			{
				DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_ON;
			}
			else
			{
				//JHKO_260417
				//DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_DIMM;
				DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_OFF;	//	LED Off 표시
			}
			break;
		case LED_BOOT:
			DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_FADE_F;
			break;
//		case LED_DIMM:
//			DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_DIMM;
//			break;
//		case LED_ON:
//			DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_ON;
//			break;
		case LED_FRONTTEST:
			if (gu8_FrontTest_ColdButton_Counter < 1)
			{
				DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_ON;
			}
			break;
		case LED_LINETEST:
			if (F_TestMode_LineAuto == CLEAR)
			{
				if (F_LineFrontTest_Cold_Select == SET)
				{
					DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_ON;
				}
				else
				{
					DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_DIMM_LOW;
				}
			}
			break;
		case LED_PBATEST:
			if (F_TestMode_PBA_2 == SET)
			{
				DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_DIMM;
			}
			else
			{
				if ((gi16_ColdSensor.DEGREE >= Z_TEMP3) && (gi16_ColdSensor.DEGREE <= Z_TEMP9))
				{
					DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_ON;
				}
				else
				{
					DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_DIMM;
				}
			}
			break;
		case LED_STEMODE:
			if ((gu16_TimerKITMode <= (Z_TimerKITMode-30)) && (gu16_TimerKITMode >= 30))
			{
				DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_FADE_F_S;
			}
			break;
		case LED_TOPCOVER:
			DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_DIMM;
			break;
		case LED_LINETEST2:
			if (F_LineTest2_Cooling == SET)
			{
				DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_ON;
			}
			else
			{
				DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_DIMM_LOW;
			}
			break;
		case LED_OFF_DISP:
			if (gu8_TimerOFF_Flic != 0)
			{
				DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_FADE_F;
			}
			else
			{
				DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_ON;
			}
			break;
		case LED_FLU_READY:
			if ((F_FilterChange_Detect == SET) || (F_FaucetChange_Detect == SET))
			{
				DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_DIMM;
			}
			break;
		case LED_WIFI_READY:											//@033
			DisplayControl_Data[Z_LED_COLD].Display_CMD = LED_DIMM;		//@033
			break;														//@033
		default:
			DisplayControl_Data[Z_LED_COLD].Display_CMD = DisplayControl_Data[Z_LED_COLD].Operation_CMD;
			break;
	}
}

/// @brief
/// @param    void
/// @return   void
static void LED_HOT(void)
{
	switch (DisplayControl_Data[Z_LED_HOT].Operation_CMD)
	{
//		case LED_OFF:
//			break;
//		case LED_FADE_F:
//			DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_FADE_F;
//			break;
//		case LED_FADE_F_S:
//			DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_FADE_F_S;
//			break;
//		case LED_FLIC:
//			DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_FLIC;
//			break;
		case LED_STATUS:
			if (gu8_TimerHotLock_Display != 0)
			{
				DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_FADE_F;
			}
			else if (gu8_WaterSelect == HOT_WATER)
			{
				if (gu8_HotStep == Z_HotStep_35)
				{
					DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_ON;
				}
				else if ((gu8_HotStep >= Z_HotStep_2F) && (gu8_HotStep < Z_HotStep_35) && (gu8_HotStep != Z_HotStep_32))
				{
					DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_FADE_F;
				}
				else
				{
					DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_ON;
				}
			}
			else
			{
				if (gu8_ModelType_forNational == MODEL_JAPAN)
				{
					if (F_HotLock == CLEAR)
					{
						DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_FADE_F_S;	//	온수 선택 가능하다는 표시
					}
					else
					{
						DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_DIMM;
					}
				}
				else
				{
					//DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_DIMM;
					//JHKO_260417
					DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_OFF;	//	LED Off 표시
				}
			}
			break;
		case LED_EFF:
			if (gu8_WaterSelect == HOT_WATER)
			{
//				if (gu8_HotStep == Z_HotStep_02)
//				{
//					DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_FADE_F_S;
//				}
//				else
//				{
					DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_ON;
//				}
			}
			else
			{
				//DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_DIMM;
				DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_OFF;	//	LED Off 표시
			}
			break;
		case LED_BOOT:
			DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_FADE_F;
			break;
//		case LED_DIMM:
//			DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_DIMM;
//			break;
//		case LED_ON:
//			DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_ON;
//			break;
		case LED_FRONTTEST:
			if (gu8_FrontTest_HotButton_Counter < 1)
			{
				DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_ON;
			}
			break;
		case LED_LINETEST:
			if (F_TestMode_LineAuto == CLEAR)
			{
				if (F_LineFrontTest_Hot_Select == SET)
				{
					DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_ON;
				}
				else
				{
					DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_DIMM_LOW;
				}
			}
			else
			{
				DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_ON;
			}
			break;
		case LED_PBATEST:
			if (F_TestMode_PBA_2 == SET)
			{
				DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_DIMM;
			}
			else
			{
				if ((gi16_HotInSensor.DEGREE >= Z_TEMP17) && (gi16_HotInSensor.DEGREE <= Z_TEMP23))
				{
					DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_ON;
				}
				else if ((gi16_HotOutSensor.DEGREE >= Z_TEMP77) && (gi16_HotOutSensor.DEGREE <= Z_TEMP80))
				{
					DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_ON;
				}
				else if ((gi16_HeaterSensor.DEGREE >= Z_TEMP77) && (gi16_HeaterSensor.DEGREE <= Z_TEMP80))
				{
					DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_ON;
				}
				else
				{
					DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_DIMM;
				}
			}
			break;
		case LED_ALL_LOCK:
			if (gu8_TimerAllLockDisplay != 0)
			{
				DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_FADE_F;
			}
			else
			{
				DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_DIMM;
			}
			break;
		case LED_SOUND:
			if (gu8_TimerSoundChange != 0)
			{
				DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_ON;
			}
			else
			{
				DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_DIMM;
			}
			break;
		case LED_TOPCOVER:
			DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_DIMM;
			break;
		case LED_LINETEST2:
			if (F_LineTest2_Heater == SET)
			{
				DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_ON;
			}
			else
			{
				DisplayControl_Data[Z_LED_HOT].Display_CMD = LED_DIMM_LOW;
			}
			break;
		default:
			DisplayControl_Data[Z_LED_HOT].Display_CMD = DisplayControl_Data[Z_LED_HOT].Operation_CMD;
			break;
	}
}

/// @brief
/// @param    void
/// @return   void
static void LED_EFFLUENT(void)
{
	switch (DisplayControl_Data[Z_LED_EFFLUENT].Operation_CMD)
	{
//		case LED_OFF:
//			break;
//		case LED_FADE_F:
//			DisplayControl_Data[Z_LED_EFFLUENT].Display_CMD = LED_FADE_F;
//			break;
//		case LED_FADE_F_S:
//			DisplayControl_Data[Z_LED_EFFLUENT].Display_CMD = LED_FADE_F_S;
//			break;
//		case LED_FLIC:
//			DisplayControl_Data[Z_LED_EFFLUENT].Display_CMD = LED_FLIC;
//			break;
		case LED_STATUS:
			break;
		case LED_BOOT:
			DisplayControl_Data[Z_LED_EFFLUENT].Display_CMD = LED_FADE_F;
			break;
		case LED_DIMM:
			if ((gu8_OperationStep_hi == 0x10) || (gu8_OperationStep_hi == 0x20)	//	대기/설정/슬립/UV 동작
			|| (gu8_OperationStep_hi == 0x30) || (gu8_OperationStep_hi == 0x40))
			{
				if (F_EffluentLED_OFF == CLEAR)
				{
					DisplayControl_Data[Z_LED_EFFLUENT].Display_CMD = LED_DIMM;
				}
			}
			else
			{
				DisplayControl_Data[Z_LED_EFFLUENT].Display_CMD = LED_DIMM;
			}
			break;
		case LED_ON:
			if ((gu8_OperationStep_hi == 0x10) || (gu8_OperationStep_hi == 0x20)	//	대기/설정/슬립/UV 동작
			|| (gu8_OperationStep_hi == 0x30) || (gu8_OperationStep_hi == 0x40))
			{
				if (F_EffluentLED_OFF == CLEAR)
				{
					DisplayControl_Data[Z_LED_EFFLUENT].Display_CMD = LED_ON;
				}
			}
			else
			{
				DisplayControl_Data[Z_LED_EFFLUENT].Display_CMD = LED_ON;
			}
			break;
		case LED_EFF:
			if (gu8_HotStep == Z_HotStep_02)
			{
				DisplayControl_Data[Z_LED_EFFLUENT].Display_CMD = LED_FADE_F_S;
			}
			else
			{
				DisplayControl_Data[Z_LED_EFFLUENT].Display_CMD = LED_ON;
			}
			break;
		case LED_FRONTTEST:
			if (F_FrontTest_EffluentButton == CLEAR)
			{
				DisplayControl_Data[Z_LED_EFFLUENT].Display_CMD = LED_ON;
			}
			break;
		case LED_PBATEST:
			DisplayControl_Data[Z_LED_EFFLUENT].Display_CMD = LED_DIMM;
			break;
		case LED_LINETEST:
			if (F_LineFrontTest_Effluent_Oper == SET)
			{
				DisplayControl_Data[Z_LED_EFFLUENT].Display_CMD = LED_ON;
			}
			else
			{
				DisplayControl_Data[Z_LED_EFFLUENT].Display_CMD = LED_DIMM_LOW;
			}
			break;
		default:
			DisplayControl_Data[Z_LED_EFFLUENT].Display_CMD = DisplayControl_Data[Z_LED_EFFLUENT].Operation_CMD;
			break;
	}
}

/// @brief
/// @param    void
/// @return   void
static void LED_CUP(void)
{       U8 mu8_i = 0;
		U8 mu8_enabled_cnt = 0; // 활성화된 용량 개수 파악용 변수 JHKO

	switch (DisplayControl_Data[Z_LED_2CUP].Operation_CMD)
	{
//		case LED_OFF:
//			break;
//		case LED_FADE_F:
//			DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_FADE_F;
//			DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_FADE_F;
//			DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_FADE_F;
//			DisplayControl_Data[Z_LED_CON].Display_CMD = LED_FADE_F;
//			break;
//		case LED_FADE_F_S:
//			DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_FADE_F_S;
//			DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_FADE_F_S;
//			DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_FADE_F_S;
//			DisplayControl_Data[Z_LED_CON].Display_CMD = LED_FADE_F_S;
//			break;
//		case LED_FLIC:
//			DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_FLIC;
//			DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_FLIC;
//			DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_FLIC;
//			DisplayControl_Data[Z_LED_CON].Display_CMD = LED_FLIC;
//			break;
		case LED_EFF:
#ifdef _FOR_CONDENSATION_TEST_
			return;
#endif
//			if (gu8_HotStep == Z_HotStep_02)
			if ((gu8_WaterSelect == HOT_WATER) && (F_FirstHotEffluent == CLEAR))
			{
				DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_ON;
			}
			else
			{	
				#if 0
				switch (gu16_CupSize)
				{
					case CUPSIZE_HALFCUP:
						DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_ON;
						break;
					case CUPSIZE_1CUP:
						DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_ON;
						break;
					case CUPSIZE_2CUP:
						DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_ON;
						break;
					case CUPSIZE_CONTINUE:
#ifndef _AFTER_LPP_IMDCHANGE_
						if (gu8_ModelType_forNational == MODEL_JAPAN)
						{
							DisplayControl_Data[Z_LED_CON].Display_CMD = LED_ON;
						}
#else
						DisplayControl_Data[Z_LED_CON].Display_CMD = LED_ON;
#endif
						break;
					default:
						break;
				}
				#endif
				
				//#if 0
				for (mu8_i = 0; mu8_i < (CUPSIZE_FIVE + 1); mu8_i++)
				{
					if(gu8_Cup_size_Array[mu8_i].enable == 0)
					{
						DisplayControl_Data[Z_LED_HALFCUP - mu8_i].Display_CMD = LED_OFF;
					}
					else
					{
						if(gu8_CupIndex == mu8_i)
						{
							//DisplayControl_Data[Z_LED_HALFCUP - mu8_i].Display_CMD = LED_ON;
							if(gu16_CupSize == CUPSIZE_HALFCUP)   DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_ON;
							else if(gu16_CupSize == CUPSIZE_1CUP) DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_ON;
							else if(gu16_CupSize == CUPSIZE_2CUP) DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_ON;
							else if(gu16_CupSize == CUPSIZE_CONTINUE) DisplayControl_Data[Z_LED_CON].Display_CMD = LED_ON;
							else DisplayControl_Data[Z_LED_HALFCUP - mu8_i].Display_CMD = LED_OFF;
						}
						else
						{
							//DisplayControl_Data[Z_LED_HALFCUP - mu8_i].Display_CMD = LED_DIMM;
							     if(gu16_CupSize == CUPSIZE_HALFCUP)   DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_DIMM;
							else if(gu16_CupSize == CUPSIZE_1CUP) DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_DIMM;
							else if(gu16_CupSize == CUPSIZE_2CUP) DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_DIMM;
							else if(gu16_CupSize == CUPSIZE_CONTINUE) DisplayControl_Data[Z_LED_CON].Display_CMD = LED_DIMM;
							else DisplayControl_Data[Z_LED_HALFCUP - mu8_i].Display_CMD = LED_OFF;
						}
					}
				}
				//#endif
				
			}
			break;
		case LED_STATUS:
#ifndef _AFTER_LPP_IMDCHANGE_
			if (gu8_TimerMySetFilker != 0)
			{
				DisplayControl_Data[Z_LED_CON].Display_CMD = LED_FADE_F;
			}
			else
#endif
			if (gu8_TimerDefaultCupSize_Set_LEDFlic != 0)
			{
				//switch (gu16_CupSize)
				switch (gu8_CupIndex)
				{
					//case CUPSIZE_HALFCUP:
					case CUPSIZE_ONE:
						DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_FADE_F;
						break;
					//case CUPSIZE_1CUP:
					case CUPSIZE_TWO:
						DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_FADE_F;
						break;
					//case CUPSIZE_2CUP:
					case CUPSIZE_THREE:
						DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_FADE_F;
						break;
					/*
#ifndef _AFTER_LPP_IMDCHANGE_          
					case CUPSIZE_MY:
					
						DisplayControl_Data[Z_LED_CON].Display_CMD = LED_FADE_F;
						break;
#endif                                  */
					default:
						break;
				}
			}
			else if (gu8_TimerCupSize_LEDFlic_forJAPAN_Limit != 0)
			{
				DisplayControl_Data[Z_LED_CON].Display_CMD = LED_DIMM;
				DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_DIMM;
				DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_DIMM;
				DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_FADE_F;
			}
//			else if ((gu8_TimerCupSelect != 0) || (gu8_TimerWaterSelect != 0))
			else if (gu8_TimerCupSelect != 0)
			{
				if ((gu8_WaterSelect == HOT_WATER) && (F_HotDegreeDisplay == SET) && (F_FirstHotEffluent == CLEAR))
				{
					DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_ON;
				}
				else if (gu8_TimerSleepMode != 0)
				{
					U8 mu8_enabled_cnt = 0;
					
					// 1. 최소 1개 용량 강제 On (안전 검증 로직)
					for (mu8_i = Z_CUP_SIZE_ARRAY_ONE; mu8_i <= Z_CUP_SIZE_ARRAY_CONTINUE; mu8_i++)
					{
						if (gu8_Cup_size_Array[mu8_i].enable == 1)
						{
							mu8_enabled_cnt++;
						}
					}
					
					// 모두 Off(0)인 경우 반컵을 강제로 활성화
					if (mu8_enabled_cnt == 0)
					{
						gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_ONE].enable = 1;
						gu8_CupIndex = Z_CUP_SIZE_ARRAY_ONE;
					}
					// 현재 선택된 용량이 Off 상태라면, 다음 활성화된 용량으로 인덱스 이동
					else if (gu8_Cup_size_Array[gu8_CupIndex].enable == 0)
					{
						for (mu8_i = Z_CUP_SIZE_ARRAY_ONE; mu8_i <= Z_CUP_SIZE_ARRAY_CONTINUE; mu8_i++)
						{
							if (gu8_Cup_size_Array[mu8_i].enable == 1)
							{
								gu8_CupIndex = mu8_i;
								break;
							}
						}
					}

					// 2. 용량 LED 밝기 업데이트 (선택: ON, 미선택: DIMM, Off: OFF)
					for (mu8_i = Z_CUP_SIZE_ARRAY_ONE; mu8_i <= Z_CUP_SIZE_ARRAY_CONTINUE; mu8_i++)
					{
						U8 current_cmd = LED_OFF;
						
						if (gu8_Cup_size_Array[mu8_i].enable == 1)
						{
							if (gu8_CupIndex == mu8_i)
							{
								current_cmd = LED_ON;
							}
							else
							{
								current_cmd = LED_DIMM;
							}
						}
						
						if (mu8_i == Z_CUP_SIZE_ARRAY_ONE)
							DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = current_cmd;
						else if (mu8_i == Z_CUP_SIZE_ARRAY_TWO)
							DisplayControl_Data[Z_LED_1CUP].Display_CMD = current_cmd;
						else if (mu8_i == Z_CUP_SIZE_ARRAY_THREE)
							DisplayControl_Data[Z_LED_2CUP].Display_CMD = current_cmd;
						else if (mu8_i == Z_CUP_SIZE_ARRAY_CONTINUE)
							DisplayControl_Data[Z_LED_CON].Display_CMD = current_cmd;
					}
				}
			}
			else									//	타이머 경과 후 대기모드
			{
				if ((gu8_WaterSelect == HOT_WATER) && (F_HotDegreeDisplay == SET) && (F_FirstHotEffluent == CLEAR))
				{
					DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_ON;
				}
				else if (gu8_TimerSleepMode != 0)
				{	
					U8 mu8_enabled_cnt = 0;
					
					// 1. 최소 1개 용량 강제 On (안전 검증 로직)
					for (mu8_i = Z_CUP_SIZE_ARRAY_ONE; mu8_i <= Z_CUP_SIZE_ARRAY_CONTINUE; mu8_i++)
					{
						if (gu8_Cup_size_Array[mu8_i].enable == 1)
						{
							mu8_enabled_cnt++;
						}
					}
					
					// 모두 Off(0)인 경우 반컵을 강제로 활성화
					if (mu8_enabled_cnt == 0)
					{
						gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_ONE].enable = 1;
						gu8_CupIndex = Z_CUP_SIZE_ARRAY_ONE;
					}
					// 현재 선택된 용량이 Off 상태라면, 다음 활성화된 용량으로 인덱스 이동
					else if (gu8_Cup_size_Array[gu8_CupIndex].enable == 0)
					{
						for (mu8_i = Z_CUP_SIZE_ARRAY_ONE; mu8_i <= Z_CUP_SIZE_ARRAY_CONTINUE; mu8_i++)
						{
							if (gu8_Cup_size_Array[mu8_i].enable == 1)
							{
								gu8_CupIndex = mu8_i;
								break;
							}
						}
					}

					// [수정] 2. 용량 LED 밝기 업데이트 (대기 상태: 선택은 ON, 나머지는 OFF)
					for (mu8_i = Z_CUP_SIZE_ARRAY_ONE; mu8_i <= Z_CUP_SIZE_ARRAY_CONTINUE; mu8_i++)
					{
						U8 current_cmd = LED_OFF;
						
						if (gu8_Cup_size_Array[mu8_i].enable == 1)
						{
							if(gu8_CupIndex == mu8_i) {
								current_cmd = LED_ON;
							} else {
								current_cmd = LED_OFF; // <--- 기존 LED_DIMM 에서 LED_OFF로 변경
							}
						}
						
						if (mu8_i == Z_CUP_SIZE_ARRAY_ONE) DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = current_cmd;
						else if (mu8_i == Z_CUP_SIZE_ARRAY_TWO) DisplayControl_Data[Z_LED_1CUP].Display_CMD = current_cmd;
						else if (mu8_i == Z_CUP_SIZE_ARRAY_THREE) DisplayControl_Data[Z_LED_2CUP].Display_CMD = current_cmd;
						else if (mu8_i == Z_CUP_SIZE_ARRAY_CONTINUE) DisplayControl_Data[Z_LED_CON].Display_CMD = current_cmd;
					}
					// ==========================================
				}
			}
			/*
			if (gu8_TimerDefaultCupSize_Set_LEDFlic != 0)
			{
				//switch (gu16_CupSize)
				switch (gu8_CupIndex)
				{
					//case CUPSIZE_HALFCUP:
					case CUPSIZE_ONE:
						DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_FADE_F;
						break;
					//case CUPSIZE_1CUP:
					case CUPSIZE_TWO:
						DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_FADE_F;
						break;
					//case CUPSIZE_2CUP:
					case CUPSIZE_THREE:
						DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_FADE_F;
						break;
					default:
						break;
				}
			}
			else if (gu8_TimerCupSize_LEDFlic_forJAPAN_Limit != 0)
			{
				DisplayControl_Data[Z_LED_CON].Display_CMD = LED_DIMM;
				DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_DIMM;
				DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_DIMM;
				DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_FADE_F;
			}

			else if (gu8_TimerCupSelect != 0)
			{
				if ((gu8_WaterSelect == HOT_WATER) && (F_HotDegreeDisplay == SET) && (F_FirstHotEffluent == CLEAR))
				{
					DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_ON;
				}
				else if (gu8_TimerSleepMode != 0)
				{
					//switch (gu16_CupSize)
					switch (gu8_CupIndex)
					{
						//case CUPSIZE_HALFCUP:
						case CUPSIZE_ONE:
							//DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_DIMM;
							//DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_DIMM;
							//DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_ON;
							
							//gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_ONE].amount_volume
							
	
							if((gu8_Cup_size_Array[CUPSIZE_THREE].amount_volume * 10) == CUPSIZE_2CUP)    
							DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_DIMM;
							else
							DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_OFF;
							if((gu8_Cup_size_Array[CUPSIZE_TWO].amount_volume * 10) == CUPSIZE_1CUP)    
							DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_DIMM;
							else 
							DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_OFF;
							if((gu8_Cup_size_Array[CUPSIZE_ONE].amount_volume * 10) == CUPSIZE_HALFCUP) 
							DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_ON;
							else 
							DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_OFF;
							break;
							
						//case CUPSIZE_1CUP:
						case CUPSIZE_TWO:
							//DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_DIMM;
							//DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_ON;
							//DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_DIMM;

							if((gu8_Cup_size_Array[CUPSIZE_THREE].amount_volume * 10) == CUPSIZE_2CUP)   
							DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_DIMM;
							else 
							DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_OFF;
							
							if((gu8_Cup_size_Array[CUPSIZE_TWO].amount_volume * 10) == CUPSIZE_1CUP)    
							DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_ON;
							else 
							DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_OFF;
							
							if((gu8_Cup_size_Array[CUPSIZE_ONE].amount_volume * 10) == CUPSIZE_HALFCUP) 
							DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_DIMM;
							else 
							DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_OFF;
							break;
						//case CUPSIZE_2CUP:
						case CUPSIZE_THREE:
							//DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_ON;
							//DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_DIMM;
							//DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_DIMM;

							if((gu8_Cup_size_Array[CUPSIZE_THREE].amount_volume * 10) == CUPSIZE_2CUP)    
							DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_ON;
							else 
							DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_OFF;
							
							if((gu8_Cup_size_Array[CUPSIZE_TWO].amount_volume * 10) == CUPSIZE_1CUP)    
							DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_DIMM;
							else 
							DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_OFF;
							
							if((gu8_Cup_size_Array[CUPSIZE_ONE].amount_volume * 10) == CUPSIZE_HALFCUP) 
							DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_DIMM;
							else
							DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_OFF;
							break;
						//case CUPSIZE_CONTINUE: 
						case CUPSIZE_FOUR: 
							//DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_DIMM;
							//DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_DIMM;
							//DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_DIMM;

							if((gu8_Cup_size_Array[CUPSIZE_THREE].amount_volume * 10) == CUPSIZE_2CUP)    DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_DIMM;
							else DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_OFF;
							if((gu8_Cup_size_Array[CUPSIZE_TWO].amount_volume * 10) == CUPSIZE_1CUP)    DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_DIMM;
							else DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_OFF;
							if((gu8_Cup_size_Array[CUPSIZE_ONE].amount_volume * 10) == CUPSIZE_HALFCUP) DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_DIMM;
							else DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_OFF;
							break;
							
						case CUPSIZE_MY:
							DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_DIMM;
							DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_DIMM;
							DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_DIMM;

							break;
							
						default:
							break;
					}
				}
			}
			else									//	타이머 경과 후 대기모드
			{
				if ((gu8_WaterSelect == HOT_WATER) && (F_HotDegreeDisplay == SET) && (F_FirstHotEffluent == CLEAR))
				{
					DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_ON;
				}
				else if (gu8_TimerSleepMode != 0)
				{	
				       
					for (mu8_i = CUPSIZE_ONE; mu8_i <= CUPSIZE_FIVE; mu8_i++)
					{
						// gu16_Setting_Amount_Value[i] ???? 0?? ????
						if(gu8_Cup_size_Array[mu8_i].enable == 0)
						{
							DisplayControl_Data[Z_LED_HALFCUP - mu8_i].Display_CMD = LED_OFF;
						}
						else
						{
							if(gu8_CupIndex == mu8_i)
							{
								DisplayControl_Data[Z_LED_HALFCUP - mu8_i].Display_CMD = LED_ON;
							}
							else
							{
								//DisplayControl_Data[Z_LED_HALFCUP - mu8_i].Display_CMD = LED_DIMM;
								if((gu8_Cup_size_Array[CUPSIZE_THREE].amount_volume * 10) == CUPSIZE_2CUP)   
								 DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_DIMM;
								else 
								 DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_OFF;
								
								if((gu8_Cup_size_Array[CUPSIZE_TWO].amount_volume * 10) == CUPSIZE_1CUP)    
								DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_DIMM;
								else 
								DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_OFF;
								if((gu8_Cup_size_Array[CUPSIZE_ONE].amount_volume * 10) == CUPSIZE_HALFCUP)
								DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_DIMM;
								else 
								DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_OFF;
							}
						}
					}
					
				}
			}
			*/
			break;
		case LED_BOOT:
			DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_FADE_F;
			DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_FADE_F;
			DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_FADE_F;
			DisplayControl_Data[Z_LED_CON].Display_CMD = LED_FADE_F;
			break;
//		case LED_DIMM:
//			DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_DIMM;
//			DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_DIMM;
//			DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_DIMM;
//			DisplayControl_Data[Z_LED_CON].Display_CMD = LED_DIMM;
//			break;
//		case LED_ON:
//			break;
		case LED_FRONTTEST:
			if (gu8_FrontTest_HotButton_Counter < 3)
			{
				DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_CON].Display_CMD = LED_ON;
			}
			else if (gu8_FrontTest_HotButton_Counter < 4)
			{
				DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_ON;
			}
			else if (gu8_FrontTest_HotButton_Counter < 5)
			{
				DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_ON;
			}
			else if (gu8_FrontTest_HotButton_Counter < 6)
			{
				DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_ON;
			}
			break;
		case LED_PBATEST:
			if ((gi16_HotInSensor.DEGREE >= Z_TEMP17) && (gi16_HotInSensor.DEGREE <= Z_TEMP23))
			{
				DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_ON;
			}
			if ((gi16_HotOutSensor.DEGREE >= Z_TEMP77) && (gi16_HotOutSensor.DEGREE <= Z_TEMP83))
			{
				DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_ON;
			}
			if ((gi16_HeaterSensor.DEGREE >= Z_TEMP77) && (gi16_HeaterSensor.DEGREE <= Z_TEMP83))
			{
				DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_ON;
			}
			if ((gi16_HeatSinkSensor.DEGREE >= Z_TEMP47) && (gi16_HeatSinkSensor.DEGREE <= Z_TEMP53))
			{
				DisplayControl_Data[Z_LED_CON].Display_CMD = LED_ON;
			}
			break;
		case LED_LINETEST:
			if (F_TestMode_LineAuto == CLEAR)
			{
				DisplayControl_Data[Z_LED_CON].Display_CMD = LED_FADE_F;
				switch (gu8_LineFrontTest_Amount)
				{
					case 1:
						DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_ON;
						DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_DIMM_LOW;
						DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_DIMM_LOW;
						break;
					case 2:
						DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_ON;
						DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_ON;
						DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_DIMM_LOW;
						break;
					case 3:
						DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_ON;
						DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_ON;
						DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_ON;
						break;
					default:
						DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = LED_DIMM_LOW;
						DisplayControl_Data[Z_LED_1CUP].Display_CMD = LED_DIMM_LOW;
						DisplayControl_Data[Z_LED_2CUP].Display_CMD = LED_DIMM_LOW;
						break;
				}
			}
			else
			{
				DisplayControl_Data[Z_LED_CON].Display_CMD = LED_ON;
			}
			break;
		default:
			DisplayControl_Data[Z_LED_HALFCUP].Display_CMD = DisplayControl_Data[Z_LED_HALFCUP].Operation_CMD;
			DisplayControl_Data[Z_LED_1CUP].Display_CMD = DisplayControl_Data[Z_LED_1CUP].Operation_CMD;
			DisplayControl_Data[Z_LED_2CUP].Display_CMD = DisplayControl_Data[Z_LED_2CUP].Operation_CMD;
			DisplayControl_Data[Z_LED_CON].Display_CMD = DisplayControl_Data[Z_LED_CON].Operation_CMD;
			break;
	}
}

/// @brief
/// @param    void
/// @return   void
static void LED_COLD_TEMP(void)
{
	switch (DisplayControl_Data[Z_LED_COLD_BLUE].Operation_CMD)
	{
		case LED_OFF:
			break;
		case LED_FADE_F:
			DisplayControl_Data[Z_LED_COLD_BLUE].Display_CMD = LED_FADE_F;
//			DisplayControl_Data[Z_LED_COLD_WHITE].Display_CMD = LED_FADE_F;
			break;
		case LED_FADE_F_S:
			DisplayControl_Data[Z_LED_COLD_BLUE].Display_CMD = LED_FADE_F_S;
//			DisplayControl_Data[Z_LED_COLD_WHITE].Display_CMD = LED_FADE_F_S;
			break;
		case LED_FLIC:
			DisplayControl_Data[Z_LED_COLD_BLUE].Display_CMD = LED_FLIC;
//			DisplayControl_Data[Z_LED_COLD_WHITE].Display_CMD = LED_FLIC;
			break;
		case LED_STATUS:
		case LED_EFF:
			if (F_ColdOff == CLEAR)
			{
				if ((F_Err25_HeatSinkTH == SET) || (F_Err44_ColdTH == SET) || (F_Err29_TEM_OverDrive1 == SET) || (F_Err29_TEM_OverDrive2 == SET))		//	단선단락이 있는 경우 or TEM 에러가 있는 경우
				{
					DisplayControl_Data[Z_LED_COLD_WHITE].Display_CMD = LED_FADE_F;
				}
				else
				{
					if (gi16_ColdSensor.DEGREE > RAPID_COMPLETE_DEGREE)
					{
						gu8_TimerColdTempDisplay_forBlue = Z_TimerColdTempDisplay;
					}

					if (gi16_ColdSensor.DEGREE < Z_TEMP10)
					{
						gu8_TimerColdTempDisplay_forWhite = Z_TimerColdTempDisplay;
					}

					switch (F_ColdTemp_Display)
					{
						case 0:							//	WHITE
						default:
							if (gu8_TimerColdTempDisplay_forBlue == 0)
							{
								F_ColdTemp_Display = 1;
							}

//							if (gu8_WaterSelect == COLD_WATER)
//							{
								DisplayControl_Data[Z_LED_COLD_WHITE].Display_CMD = LED_ON;
//							}
//							else
//							{
//								DisplayControl_Data[Z_LED_COLD_WHITE].Display_CMD =	LED_DIMM;
//							}
							break;
						case 1:							//	BLUE
							if (gu8_TimerColdTempDisplay_forWhite == 0)
							{
								F_ColdTemp_Display = 0;
							}

//							if (gu8_WaterSelect == COLD_WATER)
//							{
								DisplayControl_Data[Z_LED_COLD_BLUE].Display_CMD = LED_ON;
//							}
//							else
//							{
//								DisplayControl_Data[Z_LED_COLD_BLUE].Display_CMD = LED_DIMM;
//							}
							break;
					}
				}
			}
			break;
		case LED_BOOT:
			DisplayControl_Data[Z_LED_COLD_BLUE].Display_CMD = LED_FADE_F;
			DisplayControl_Data[Z_LED_COLD_WHITE].Display_CMD = LED_FADE_F;
			break;
//		case LED_DIMM:
//			DisplayControl_Data[Z_LED_COLD_BLUE].Display_CMD = LED_DIMM;
//			DisplayControl_Data[Z_LED_COLD_WHITE].Display_CMD = LED_DIMM;
//			break;
//		case LED_ON:
//			DisplayControl_Data[Z_LED_COLD_BLUE].Display_CMD = LED_ON;
//			DisplayControl_Data[Z_LED_COLD_WHITE].Display_CMD = LED_ON;
//			break;
		case LED_FRONTTEST:
			if (gu8_FrontTest_ColdButton_Counter < 2)
			{
				DisplayControl_Data[Z_LED_COLD_WHITE].Display_CMD = LED_ON;
			}
			else if (gu8_FrontTest_ColdButton_Counter < 3)
			{
				DisplayControl_Data[Z_LED_COLD_BLUE].Display_CMD = LED_ON;
			}
			break;
		case LED_LINETEST:
			if (F_TestMode_LineAuto == CLEAR)
			{
				if ((F_LineFrontTest_Cold_Select == SET) && (F_LineFrontTest_Effluent_Oper == SET))
				{
					DisplayControl_Data[Z_LED_COLD_BLUE].Display_CMD = LED_ON;
				}
				else
				{
					DisplayControl_Data[Z_LED_COLD_WHITE].Display_CMD = LED_ON;
				}
			}
			break;
		case LED_LINETEST2:
			if (F_LineTest2_Cooling == SET)
			{
				if (F_LineTest2_CoolingResult_OK == SET)
				{
					DisplayControl_Data[Z_LED_COLD_BLUE].Display_CMD = LED_ON;
				}
				else
				{
					if (gu8_TimerLineTest2_Cooling != 0)
					{
						DisplayControl_Data[Z_LED_COLD_WHITE].Display_CMD = LED_ON;
					}
					else
					{
						DisplayControl_Data[Z_LED_COLD_WHITE].Display_CMD = LED_FADE_F;
					}
				}
			}
			break;
		default:
			DisplayControl_Data[Z_LED_COLD_WHITE].Display_CMD = DisplayControl_Data[Z_LED_COLD_WHITE].Operation_CMD;
			DisplayControl_Data[Z_LED_COLD_BLUE].Display_CMD = DisplayControl_Data[Z_LED_COLD_BLUE].Operation_CMD;
			break;
	}
}

/// @brief
/// @param    void
/// @return   void
static void LED_FAUCET(void)
{
	switch (DisplayControl_Data[Z_LED_FAUCET].Operation_CMD)
	{
//		case LED_OFF:
//			break;
//		case LED_FADE_F:
//			DisplayControl_Data[Z_LED_FAUCET].Display_CMD = LED_FADE_F;
//			break;
//		case LED_FADE_F_S:
//			DisplayControl_Data[Z_LED_FAUCET].Display_CMD = LED_FADE_F_S;
//			break;
//		case LED_FLIC:
//			DisplayControl_Data[Z_LED_FAUCET].Display_CMD = LED_FLIC;
//			break;
		case LED_STATUS:
#ifndef _AFTER_LPP_IMDCHANGE_
			if ((F_FilterChange_Display == SET) || (F_FaucetChange_Display == SET))
			{
				DisplayControl_Data[Z_LED_FAUCET].Display_CMD = LED_ON;
			}
#else
//			if (F_FilterChange_Display == SET)
//			{
//				DisplayControl_Data[Z_LED_FAUCET].Display_CMD = LED_DIMM;
//			}
			if (F_FaucetChange_Display == SET)			//	위에 if문 처리가 없어지게 일부로 else if 처리 안함
			{
//				DisplayControl_Data[Z_LED_FAUCET].Display_CMD = LED_ON;
//				DisplayControl_Data[Z_LED_FAUCET].Display_CMD = LED_FADE_F;
				if (gu8_TimerChangeDisplay != 0)
				{
					DisplayControl_Data[Z_LED_FAUCET].Display_CMD = LED_FADE_F_S2;
				}
			}
#endif
			break;
		case LED_BOOT:
			DisplayControl_Data[Z_LED_FAUCET].Display_CMD = LED_FADE_F;
			break;
//		case LED_DIMM:
//			DisplayControl_Data[Z_LED_FAUCET].Display_CMD = LED_DIMM;
//			break;
//		case LED_DIMM_LOW:
//			DisplayControl_Data[Z_LED_FAUCET].Display_CMD = LED_DIMM_LOW;
//			break;
//		case LED_ON:
//			DisplayControl_Data[Z_LED_FAUCET].Display_CMD = LED_ON;
//			break;
		case LED_FRONTTEST:
			if (gu8_FrontTest_PureButton_Counter < 8)
			{
				DisplayControl_Data[Z_LED_FAUCET].Display_CMD = LED_ON;
			}
			break;
		case LED_LINETEST:
			if (F_TestMode_LineAuto == CLEAR)
			{
				if (INPUT_REED_SW_COVER == SET)
				{
					DisplayControl_Data[Z_LED_FAUCET].Display_CMD = LED_FADE_F;
				}
				else
				{
					DisplayControl_Data[Z_LED_FAUCET].Display_CMD = LED_ON;
				}
			}
			break;
		case LED_TOPCOVER:
			if (INPUT_REED_SW_FAUCET == CLEAR)
			{
				DisplayControl_Data[Z_LED_FAUCET].Display_CMD = LED_FADE_F;
			}
			else
			{
				DisplayControl_Data[Z_LED_FAUCET].Display_CMD = LED_ON;
			}
			break;
		default:
			DisplayControl_Data[Z_LED_FAUCET].Display_CMD = DisplayControl_Data[Z_LED_FAUCET].Operation_CMD;
			break;
	}
}

/// @brief
/// @param    void
/// @return   void
static void LED_WIFI(void)
{
	U8 mu8_WifiDisp = 0;

	switch (DisplayControl_Data[Z_LED_WIFI].Operation_CMD)
	{
//		case LED_OFF:
//			break;
//		case LED_FADE_F:
//			DisplayControl_Data[Z_LED_WIFI].Display_CMD = LED_FADE_F;
//			break;
//		case LED_FADE_F_S:
//			DisplayControl_Data[Z_LED_WIFI].Display_CMD = LED_FADE_F_S;
//			break;
//		case LED_FLIC:
//			DisplayControl_Data[Z_LED_WIFI].Display_CMD = LED_FLIC;
//			break;
		case LED_STATUS:
			mu8_WifiDisp = GetWifiStatusValue(WIFI_STATUS_DISP);
			switch (mu8_WifiDisp)
			{
				case WIFI_DISP_OFF:
					break;
				case WIFI_DISP_APMODE:
				case WIFI_DISP_AP_START:
				case WIFI_DISP_AP_STEP0:
				case WIFI_DISP_AP_STEP1:
				case WIFI_DISP_AP_STEP2:
				case WIFI_DISP_AP_STEP3:
					DisplayControl_Data[Z_LED_WIFI].Display_CMD = LED_APMODE;
					break;
				case WIFI_DISP_BLE_START:
				case WIFI_DISP_BLE_STEP0:
				case WIFI_DISP_BLE_STEP1:
				case WIFI_DISP_BLE_STEP2:
				case WIFI_DISP_BLE_STEP3:
					DisplayControl_Data[Z_LED_WIFI].Display_CMD = LED_APMODE;
					break;
				case WIFI_DISP_CONNECT:
					if ((gu8_OperationStep == Z_OperationStep_10) && (gu8_TimerSleepMode == 0))
					{
						//	슬립모드 1시간 경과 시는 미점등
					}
					else
					{
						DisplayControl_Data[Z_LED_WIFI].Display_CMD = LED_ON;
					}
					break;
				default:
					DisplayControl_Data[Z_LED_WIFI].Display_CMD = LED_FADE_F;
					break;
			}
			break;
		case LED_BOOT:
			DisplayControl_Data[Z_LED_WIFI].Display_CMD = LED_FADE_F;
			break;
//		case LED_DIMM:
//			DisplayControl_Data[Z_LED_WIFI].Display_CMD = LED_DIMM;
//			break;
//		case LED_ON:
//			DisplayControl_Data[Z_LED_WIFI].Display_CMD = LED_ON;
//			break;
		case LED_FRONTTEST:
			if (gu8_FrontTest_AmountButton_Counter < 2)
			{
				DisplayControl_Data[Z_LED_WIFI].Display_CMD = LED_ON;
			}
			break;
		case LED_LINETEST:
		case LED_PBATEST:
			if (gu8_ModelType_forNational == MODEL_KOREA)
			{
				mu8_WifiDisp = GetWifiStatusValue(WIFI_STATUS_TEST);
				if (mu8_WifiDisp == SET)
				{
					DisplayControl_Data[Z_LED_WIFI].Display_CMD = LED_ON;
					F_WIFI_Pairing_forLineTest = SET;
				}
				else
				{
					DisplayControl_Data[Z_LED_WIFI].Display_CMD = LED_FADE_F;
				}
			}
			else									//	일본향은 % LED 점등
			{
				DisplayControl_Data[Z_LED_WIFI].Display_CMD = LED_ON;
			}
			break;
		case LED_FOTA:
			if (gu8_TimerFOTA_Display == 0)
			{
				if (F_FOTA_Display_forWIFI_LED_ONOFF == CLEAR)
				{
					F_FOTA_Display_forWIFI_LED_ONOFF = SET;
					gu8_TimerFOTA_Display = Z_TimerFOTA_Display;
				}
				else
				{
					F_FOTA_Display_forWIFI_LED_ONOFF = CLEAR;
					gu8_TimerFOTA_Display = Z_TimerFOTA_Display - 20;
				}
			}
			if (F_FOTA_Display_forWIFI_LED_ONOFF == SET)
			{
				DisplayControl_Data[Z_LED_WIFI].Display_CMD = LED_ON;
			}
			break;
		case LED_forSEG:
			//	SEG Data 처리에서 처리함
			break;
		default:
			DisplayControl_Data[Z_LED_WIFI].Display_CMD = DisplayControl_Data[Z_LED_WIFI].Operation_CMD;
			break;
	}
}

/// @brief
/// @param    void
/// @return   void
static void LED_FILTER_CHANGE(void)
{
	switch (DisplayControl_Data[Z_LED_FILTER_CHANGE].Operation_CMD)
	{
//		case LED_OFF:
//			break;
//		case LED_FADE_F:
//			DisplayControl_Data[Z_LED_FILTER_CHANGE].Display_CMD = LED_FADE_F;
//			break;
//		case LED_FADE_F_S:
//			DisplayControl_Data[Z_LED_FILTER_CHANGE].Display_CMD = LED_FADE_F_S;
//			break;
//		case LED_FLIC:
//			DisplayControl_Data[Z_LED_FILTER_CHANGE].Display_CMD = LED_FLIC;
//			break;
		case LED_STATUS:
#ifdef _AFTER_LPP_IMDCHANGE_
//			if (F_FaucetChange_Display == SET)
//			{
//				DisplayControl_Data[Z_LED_FILTER_CHANGE].Display_CMD = LED_DIMM;
//			}
			if (F_FilterChange_Display == SET)	//	위에 if문 처리가 없어지게 일부로 else if 처리 안함
			{
//				DisplayControl_Data[Z_LED_FILTER_CHANGE].Display_CMD = LED_ON;
//				DisplayControl_Data[Z_LED_FILTER_CHANGE].Display_CMD = LED_FADE_F;
				if (gu8_TimerChangeDisplay != 0)
				{
					DisplayControl_Data[Z_LED_FILTER_CHANGE].Display_CMD = LED_FADE_F_S2;
				}
			}
#endif
			break;
		case LED_BOOT:
			DisplayControl_Data[Z_LED_FILTER_CHANGE].Display_CMD = LED_FADE_F;
			break;
//		case LED_DIMM:
//			DisplayControl_Data[Z_LED_FILTER_CHANGE].Display_CMD = LED_DIMM;
//			break;
//		case LED_DIMM_LOW:
//			DisplayControl_Data[Z_LED_FILTER_CHANGE].Display_CMD = LED_DIMM_LOW;
//			break;
//		case LED_ON:
//			DisplayControl_Data[Z_LED_FILTER_CHANGE].Display_CMD = LED_ON;
//			break;
		case LED_FRONTTEST:
			if (gu8_FrontTest_ColdButton_Counter < 6)
			{
				DisplayControl_Data[Z_LED_FILTER_CHANGE].Display_CMD = LED_ON;
			}
			break;
		case LED_LINETEST:
			if (F_TestMode_LineAuto == CLEAR)
			{
				if (INPUT_REED_SW_FILTER == SET)
				{
					DisplayControl_Data[Z_LED_FILTER_CHANGE].Display_CMD = LED_ON;
				}
				else
				{
					DisplayControl_Data[Z_LED_FILTER_CHANGE].Display_CMD = LED_FADE_F;
				}
			}
			break;
		case LED_TOPCOVER:
			if (INPUT_REED_SW_FILTER == CLEAR)
			{
				DisplayControl_Data[Z_LED_FILTER_CHANGE].Display_CMD = LED_FADE_F;
			}
			else
			{
				DisplayControl_Data[Z_LED_FILTER_CHANGE].Display_CMD = LED_ON;
			}
			break;
		default:
			DisplayControl_Data[Z_LED_FILTER_CHANGE].Display_CMD = DisplayControl_Data[Z_LED_FILTER_CHANGE].Operation_CMD;
			break;
	}
}

/// @brief
/// @param    void
/// @return   void
static void LED_ML(void)
{
	switch (DisplayControl_Data[Z_LED_mL].Operation_CMD)
	{
//		case LED_OFF:
//			break;
//		case LED_FADE_F:
//			DisplayControl_Data[Z_LED_mL].Display_CMD = LED_FADE_F;
//			break;
//		case LED_FADE_F_S:
//			DisplayControl_Data[Z_LED_mL].Display_CMD = LED_FADE_F_S;
//			break;
//		case LED_FLIC:
//			DisplayControl_Data[Z_LED_mL].Display_CMD = LED_FLIC;
//			break;
		case LED_STATUS:
			break;
		case LED_forSEG:
			//	SEG Data 처리에서 처리함
			break;
		case LED_BOOT:
			DisplayControl_Data[Z_LED_mL].Display_CMD = LED_FADE_F;
			break;
//		case LED_DIMM:
//			DisplayControl_Data[Z_LED_mL].Display_CMD = LED_DIMM;
//			break;
//		case LED_ON:
//			DisplayControl_Data[Z_LED_mL].Display_CMD = LED_ON;
//			break;
		case LED_FRONTTEST:
			if (gu8_FrontTest_ColdButton_Counter < 4)
			{
				DisplayControl_Data[Z_LED_mL].Display_CMD = LED_ON;
			}
			break;
		case LED_LINETEST:
			if (F_TestMode_LineAuto == CLEAR)
			{
				DisplayControl_Data[Z_LED_mL].Display_CMD = LED_ON;
			}
			break;
		case LED_PBATEST:
			DisplayControl_Data[Z_LED_mL].Display_CMD = LED_FADE_F;
			break;
		default:
			DisplayControl_Data[Z_LED_mL].Display_CMD = DisplayControl_Data[Z_LED_mL].Operation_CMD;
			break;
	}
}

/// @brief
/// @param    void
/// @return   void
static void LED_DEGREE(void)
{
	switch (DisplayControl_Data[Z_LED_DEGREE].Operation_CMD)
	{
//		case LED_OFF:
//			break;
//		case LED_FADE_F:
//			DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_FADE_F;
//			break;
//		case LED_FADE_F_S:
//			DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_FADE_F_S;
//			break;
//		case LED_FLIC:
//			DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_FLIC;
//			break;
		case LED_STATUS:
			break;
		case LED_forSEG:
			//	SEG Data 처리에서 처리함
			break;
		case LED_BOOT:
			DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_FADE_F;
			break;
//		case LED_DIMM:
//			DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_DIMM;
//			break;
//		case LED_ON:
//			DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_ON;
//			break;
		case LED_FRONTTEST:
			if (gu8_FrontTest_ColdButton_Counter < 5)
			{
				DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_ON;
			}
			break;
		case LED_LINETEST:
			if (F_TestMode_LineAuto == CLEAR)
			{
				DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_ON;
			}
			break;
		case LED_PBATEST:
			DisplayControl_Data[Z_LED_DEGREE].Display_CMD = LED_FADE_F;
			break;
		default:
			DisplayControl_Data[Z_LED_DEGREE].Display_CMD = DisplayControl_Data[Z_LED_DEGREE].Operation_CMD;
			break;
	}
}

/// @brief
/// @param    void
/// @return   void
static void LED_STE(void)
{
	switch (DisplayControl_Data[Z_LED_STE].Operation_CMD)
	{
//		case LED_OFF:
//			break;
//		case LED_FADE_F:
//			DisplayControl_Data[Z_LED_STE].Display_CMD = LED_FADE_F;
//			break;
//		case LED_FADE_F_S:
//			DisplayControl_Data[Z_LED_STE].Display_CMD = LED_FADE_F_S;
//			break;
//		case LED_FLIC:
//			DisplayControl_Data[Z_LED_STE].Display_CMD = LED_FLIC;
//			break;
		case LED_STATUS:
			break;
		case LED_BOOT:
			DisplayControl_Data[Z_LED_STE].Display_CMD = LED_FADE_F;
			break;
//		case LED_DIMM:
//			DisplayControl_Data[Z_LED_STE].Display_CMD = LED_DIMM;
//			break;
//		case LED_ON:
//			DisplayControl_Data[Z_LED_STE].Display_CMD = LED_ON;
//			break;
		case LED_FRONTTEST:
			if (F_FrontTest_UV_FBError == SET)
			{
				DisplayControl_Data[Z_LED_STE].Display_CMD = LED_FADE_F;
			}
			else if (gu8_FrontTest_PureButton_Counter < 7)
			{
				DisplayControl_Data[Z_LED_STE].Display_CMD = LED_ON;
			}
			else
			{

			}
			break;
		case LED_LINETEST:
			if (F_TestMode_LineAuto == CLEAR)
			{
				if (F_LineTest_UV_FBError == SET)
				{
					DisplayControl_Data[Z_LED_STE].Display_CMD = LED_FADE_F;
				}
				else
				{
					DisplayControl_Data[Z_LED_STE].Display_CMD = LED_ON;
				}
			}
			break;
		default:
			DisplayControl_Data[Z_LED_STE].Display_CMD = DisplayControl_Data[Z_LED_STE].Operation_CMD;
			break;
	}
}

/// @brief
/// @param    void
/// @return   void
static void LED_UV(void)
{
	U8 mu8_TotalPercent = 0;

	switch (DisplayControl_Data[Z_LED_UV1].Operation_CMD)
	{
//		case LED_OFF:
//			break;
//		case LED_FADE_F:
//			DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_FADE_F;
//			DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_FADE_F;
//			DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_FADE_F;
//			DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_FADE_F;
//			DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_FADE_F;
//			break;
//		case LED_FADE_F_S:
//			DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_FADE_F_S;
//			DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_FADE_F_S;
//			DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_FADE_F_S;
//			DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_FADE_F_S;
//			DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_FADE_F_S;
//			break;
//		case LED_FLIC:
//			DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_FLIC;
//			DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_FLIC;
//			DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_FLIC;
//			DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_FLIC;
//			DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_FLIC;
//			break;
		case LED_BOOT:
			DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_FADE_F;
			DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_FADE_F;
			DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_FADE_F;
			DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_FADE_F;
			DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_FADE_F;
			break;
//		case LED_DIMM:
//			DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_DIMM;
//			DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_DIMM;
//			DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_DIMM;
//			DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_DIMM;
//			DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_DIMM;
//			break;
//		case LED_ON:
//			DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
//			DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
//			DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
//			DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
//			DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
//			break;
		case LED_FRONTTEST:
			if (gu8_FrontTest_PureButton_Counter < 2)
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
			}
			else if (gu8_FrontTest_PureButton_Counter < 3)
			{
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
			}
			else if (gu8_FrontTest_PureButton_Counter < 4)
			{
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
			}
			else if (gu8_FrontTest_PureButton_Counter < 5)
			{
				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
			}
			else if (gu8_FrontTest_PureButton_Counter < 6)
			{
				DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
			}
			break;
		case LED_FLU:
//			if (gu8_TimerFlushingDisplay == 0)
//			{
//				gu8_TimerFlushingDisplay = Z_TimerFlushingDisplay;
//			}
//
//			if (gu8_TimerFlushingDisplay > ((Z_TimerFlushingDisplay/8)*7))
//			{
//				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
//			}
//			else if (gu8_TimerFlushingDisplay > ((Z_TimerFlushingDisplay/8)*6))
//			{
//				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
//				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
//			}
//			else if (gu8_TimerFlushingDisplay > ((Z_TimerFlushingDisplay/8)*5))
//			{
//				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
//				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
//				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
//			}
//			else if (gu8_TimerFlushingDisplay > ((Z_TimerFlushingDisplay/8)*4))
//			{
//				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
//				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
//				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
//				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
//			}
//			else if (gu8_TimerFlushingDisplay > ((Z_TimerFlushingDisplay/8)*3))
//			{
//				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
//				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
//				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
//				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
//				DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
//			}
//			else
//			{
//
//			}
			break;
		case LED_UVCARE1:
			if (gu16_TimerUV_Oper > ((Z_TimerUV_Oper/5)*4))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
			}
			else if (gu16_TimerUV_Oper > ((Z_TimerUV_Oper/5)*3))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
			}
			else if (gu16_TimerUV_Oper > ((Z_TimerUV_Oper/5)*2))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
			}
			else if (gu16_TimerUV_Oper > ((Z_TimerUV_Oper/5)*1))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
			}
			else
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
			}
			break;
		case LED_UVCARE2:
			if (gu16_TimerUV_Oper > ((Z_TimerUV_Oper_15min / 5) * 4))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
			}
			else if (gu16_TimerUV_Oper > ((Z_TimerUV_Oper_15min / 5) * 3))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
			}
			else if (gu16_TimerUV_Oper > ((Z_TimerUV_Oper_15min / 5) * 2))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
			}
			else if (gu16_TimerUV_Oper > ((Z_TimerUV_Oper_15min / 5) * 1))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
			}
			else
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
			}
			break;
		case LED_STATUS:
//			if (gu8_HotStep == Z_HotStep_02)
//			{
//				if (gu8_TimerFirstHotDisplay == 0)
//				{
//					gu8_TimerFirstHotDisplay = Z_TimerFirstHotDisplay;
//				}
//
//				if (gu8_TimerFirstHotDisplay > ((Z_TimerFirstHotDisplay/8)*7))
//				{
//					DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
//				}
//				else if (gu8_TimerFirstHotDisplay > ((Z_TimerFirstHotDisplay/8)*6))
//				{
//					DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
//					DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
//				}
//				else if (gu8_TimerFirstHotDisplay > ((Z_TimerFirstHotDisplay/8)*5))
//				{
//					DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
//					DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
//					DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
//				}
//				else if (gu8_TimerFirstHotDisplay > ((Z_TimerFirstHotDisplay/8)*4))
//				{
//					DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
//					DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
//					DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
//					DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
//				}
//				else if (gu8_TimerFirstHotDisplay > ((Z_TimerFirstHotDisplay/8)*3))
//				{
//					DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
//					DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
//					DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
//					DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
//					DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
//				}
//				else if (gu8_TimerFirstHotDisplay > ((Z_TimerFirstHotDisplay/8)*2))
//				{
//					DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
//					DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
//					DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
//					DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
//					DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
//				}
//				else
//				{
//
//				}
//			}
			break;
		case LED_COLDTANK:
			if (gu8_TimerColdTankClearDisplay == 0)
			{
				gu8_TimerColdTankClearDisplay = Z_TimerColdTankClearDisplay;
			}

			if (gu8_TimerColdTankClearDisplay > ((Z_TimerColdTankClearDisplay/8)*7))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
			}
			else if (gu8_TimerColdTankClearDisplay > ((Z_TimerColdTankClearDisplay/8)*6))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
			}
			else if (gu8_TimerColdTankClearDisplay > ((Z_TimerColdTankClearDisplay/8)*5))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
			}
			else if (gu8_TimerColdTankClearDisplay > ((Z_TimerColdTankClearDisplay/8)*4))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
			}
			else if (gu8_TimerColdTankClearDisplay > ((Z_TimerColdTankClearDisplay/8)*3))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
			}
			else if (gu8_TimerColdTankClearDisplay > ((Z_TimerColdTankClearDisplay/8)*2))
			{

			}
			else
			{

			}
			break;
		case LED_SMART:
			if (gu16_TimerSmartCheckDisplay == 0)
			{
				gu16_TimerSmartCheckDisplay = Z_TimerSmartCheckDisplay;
			}

			if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*24))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*23))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*22))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*21))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*20))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*19))
			{
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*18))
			{
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*17))
			{
				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*16))
			{
				DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*15))
			{

			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*14))
			{

			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*13))
			{

			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*12))
			{
				DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*11))
			{
				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*10))
			{
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*9))
			{
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*8))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*7))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*6))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*5))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*4))
			{
				DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*3))
			{

			}
			else if (gu16_TimerSmartCheckDisplay > ((Z_TimerSmartCheckDisplay/25)*2))
			{

			}
			else
			{

			}
			break;
		case LED_LINETEST:
			if (F_TestMode_LineAuto == CLEAR)
			{
				if (INPUT_REED_SW_FAUCET == CLEAR)
				{
					DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_FADE_F;
					DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_FADE_F;
					DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_FADE_F;
					DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_FADE_F;
					DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_FADE_F;
				}
				else
				{
					DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
					DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
					DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
					DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
					DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
				}
			}
			break;
		case LED_PBATEST:
			if ((gu16_FlowDecide_Hz >= 200) && (gu16_FlowDecide_Hz <= 1000))
			{
				if (gu8_TimerFlowSensorTestDisplay == 0)
				{
					gu8_TimerFlowSensorTestDisplay = Z_TimerFlowSensorTestDisplay;
				}

				if (gu8_TimerFlowSensorTestDisplay > ((Z_TimerFlowSensorTestDisplay/8)*7))
				{
					DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
				}
				else if (gu8_TimerFlowSensorTestDisplay > ((Z_TimerFlowSensorTestDisplay/8)*6))
				{
					DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
					DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
				}
				else if (gu8_TimerFlowSensorTestDisplay > ((Z_TimerFlowSensorTestDisplay/8)*5))
				{
					DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
					DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
					DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
				}
				else if (gu8_TimerFlowSensorTestDisplay > ((Z_TimerFlowSensorTestDisplay/8)*4))
				{
					DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
					DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
					DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
					DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
				}
				else if (gu8_TimerFlowSensorTestDisplay > ((Z_TimerFlowSensorTestDisplay/8)*3))
				{
					DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
					DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
					DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
					DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
					DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
				}
				else if (gu8_TimerFlowSensorTestDisplay > ((Z_TimerFlowSensorTestDisplay/8)*2))
				{
					DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
					DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
					DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
					DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
					DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
				}
				else
				{

				}
			}
			else
			{
				gu8_TimerFlowSensorTestDisplay = Z_TimerFlowSensorTestDisplay;
			}
			break;
		case LED_VOLUME:
			if (gu8_Sound_Type > 1)
			{
				switch (gu8_VoiceVolumeLevel)
				{
					case 0:
					default:
						DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
						break;
					case 1:
						DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
						DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
						break;
					case 2:
						DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
						DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
						DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
						break;
					case 3:
						DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
						DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
						DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
						DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
						break;
					case 4:
						DisplayControl_Data[Z_LED_UV1].Display_CMD = LED_ON;
						DisplayControl_Data[Z_LED_UV2].Display_CMD = LED_ON;
						DisplayControl_Data[Z_LED_UV3].Display_CMD = LED_ON;
						DisplayControl_Data[Z_LED_UV4].Display_CMD = LED_ON;
						DisplayControl_Data[Z_LED_UV5].Display_CMD = LED_ON;
						break;
				}
			}
			break;
		default:
			DisplayControl_Data[Z_LED_UV1].Display_CMD = DisplayControl_Data[Z_LED_UV1].Operation_CMD;
			DisplayControl_Data[Z_LED_UV2].Display_CMD = DisplayControl_Data[Z_LED_UV2].Operation_CMD;
			DisplayControl_Data[Z_LED_UV3].Display_CMD = DisplayControl_Data[Z_LED_UV3].Operation_CMD;
			DisplayControl_Data[Z_LED_UV4].Display_CMD = DisplayControl_Data[Z_LED_UV4].Operation_CMD;
			DisplayControl_Data[Z_LED_UV5].Display_CMD = DisplayControl_Data[Z_LED_UV5].Operation_CMD;
			break;
	}
}

/// @brief
/// @param    void
/// @return   void
static void LED_HOTLOCK(void)
{
	switch (DisplayControl_Data[Z_LED_HOTLOCK].Operation_CMD)
	{
//			case LED_OFF:
//				break;
//			case LED_FADE_F:
//				DisplayControl_Data[Z_LED_HOTLOCK].Display_CMD = LED_FADE_F;
//				break;
//			case LED_FADE_F_S:
//				DisplayControl_Data[Z_LED_HOTLOCK].Display_CMD = LED_FADE_F_S;
//				break;
//			case LED_FLIC:
//				DisplayControl_Data[Z_LED_HOTLOCK].Display_CMD = LED_FLIC;
//				break;
			case LED_STATUS:
				if (gu8_ModelType_forCHP == MODEL_CHP)
				{
					if (gu8_TimerHotLock_Display != 0)
					{
						DisplayControl_Data[Z_LED_HOTLOCK].Display_CMD = LED_FADE_F;
					}
					else if (F_HotLock == SET)
					{
						DisplayControl_Data[Z_LED_HOTLOCK].Display_CMD = LED_DIMM;
					}
				}
				break;
			case LED_BOOT:
				DisplayControl_Data[Z_LED_HOTLOCK].Display_CMD = LED_FADE_F;
				break;
//			case LED_DIMM:
//				DisplayControl_Data[Z_LED_HOTLOCK].Display_CMD = LED_DIMM;
//				break;
//			case LED_ON:
//				DisplayControl_Data[Z_LED_HOTLOCK].Display_CMD = LED_ON;
//				break;
			case LED_FRONTTEST:
				if (gu8_FrontTest_HotButton_Counter < 2)
				{
					DisplayControl_Data[Z_LED_HOTLOCK].Display_CMD = LED_ON;
				}
				break;
			case LED_LINETEST:
				if (F_TestMode_LineAuto == CLEAR)
				{
					if (F_LineFrontTest_Hot_Select == SET)
					{
						DisplayControl_Data[Z_LED_HOTLOCK].Display_CMD = LED_ON;
					}
				}
				else
				{
					if (gu8_ModelType_forCHP == MODEL_CHP)
					{
						DisplayControl_Data[Z_LED_HOTLOCK].Display_CMD = LED_ON;
					}
				}
				break;
			case LED_ALL_LOCK:
				if (gu8_TimerAllLockDisplay != 0)
				{
					DisplayControl_Data[Z_LED_HOTLOCK].Display_CMD = LED_FADE_F;
				}
				else
				{
					DisplayControl_Data[Z_LED_HOTLOCK].Display_CMD = LED_DIMM;
				}
				break;
			default:
				DisplayControl_Data[Z_LED_HOTLOCK].Display_CMD = DisplayControl_Data[Z_LED_HOTLOCK].Operation_CMD;
				break;
	}
}
