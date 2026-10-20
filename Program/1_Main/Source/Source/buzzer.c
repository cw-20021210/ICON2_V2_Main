/// @file     buzzer.c
/// @date     2019.07.10
/// @author   PSM
/// @brief    부저  동작 제어 file
#define BUZZER_C
#include "include/_include.h"


static U8 mu8_BuzzerReqeust = 0;
static U8 mu8_BuzzerCnt = 0;

static void Buzzer_OFF(void);

static U16 Buzzer_PowerON[] = {	//	Power ON
//	음계		ON Time		Echo Time	[1/10ms]
	Si5, 	4, 			19,
	Re6, 	4, 			19,
	Sol6, 	4, 			19,
	Pas6, 	5, 			65,
	Re7, 	2, 			112,
	BuzzerOFF		//	OFF 필수
};

static U16 Buzzer_Effluent[] = {
//	음계		ON Time		Echo Time	[1/10ms]
	Re6, 	3, 			21,
	Re7, 	3, 			21,
	BuzzerOFF		//	OFF 필수
};

static U16 Buzzer_Effluent_END[] = {
//	음계		ON Time		Echo Time	[1/10ms]
	La5, 	3, 			21,
	Pas6, 	3, 			21,
	Re6,	3,			30,
	BuzzerOFF		//	OFF 필수
};

static U16 Buzzer_Cold_Setup[] = {
//	음계		ON Time		Echo Time	[1/10ms]
	Pas6, 	5, 			19,
	Re7,	10,			38,
	BuzzerOFF		//	OFF 필수
};

static U16 Buzzer_Cold_Cancle[] = {
//	음계		ON Time		Echo Time	[1/10ms]
	Sols5, 	3, 			21,
	Si5,	3,			21,
	Mi5,	10,			38,
	BuzzerOFF		//	OFF 필수
};

static U16 Buzzer_Select[] = {
//	음계		ON Time		Echo Time	[1/10ms]
	La6, 	8, 			28,
	BuzzerOFF		//	OFF 필수
};

static U16 Buzzer_Alram[] = {
//	음계		ON Time		Echo Time	[1/10ms]
	Sols6, 	8, 			50,
	BuzzerOFF		//	OFF 필수
};

static U16 Buzzer_Error[] = {
//	음계		ON Time		Echo Time	[1/10ms]
	Re6,	2,			10,
	Re7,	8,			28,
	Re6,	2,			10,
	Re7,	8,			28,
	BuzzerOFF		//	OFF 필수
};

static U16 Buzzer_SetUp[] = {
//	음계		ON Time		Echo Time	[1/10ms]
	Sol6,	5,			19,
	Re7,	10,			38,
	BuzzerOFF		//	OFF 필수
};

static U16 Buzzer_Cancle[] = {
//	음계		ON Time		Echo Time	[1/10ms]
	Re7,	5,			19,
	Sol6,	10,			38,
	BuzzerOFF		//	OFF 필수
};

static U16 Buzzer_Effluent_Continue[] = {
//	음계		ON Time		Echo Time	[1/10ms]
	Re6,	8,			20,
	Re7,	8,			20,
	Dos7,	8,			20,
	La6,	8,			20,
	Mi6,	8,			20,
	La5,	8,			20,
	Re6,	8,			20,
	La6,	8,			20,
	BuzzerOFF		//	OFF 필수
};

static U16 Buzzer_APConnect[] = {
//	음계		ON Time		Echo Time	[1/10ms]
	Do6,	5,			20,
	Re6,	5,			20,
	Mi6,	5,			40,
	BuzzerOFF		//	OFF 필수
};

static U16 Buzzer_ServerConnect[] = {
//	음계		ON Time		Echo Time	[1/10ms]
	Do7,	3,			10,
	Re7,	3,			10,
	Mi7,	3,			50,
	BuzzerOFF		//	OFF 필수
};

//static U16 Buzzer_AP_Pairing[] = {		// Select와 같음
////	음계		ON Time		Echo Time	[1/10ms]
//	La6, 	8, 			28,
//	BuzzerOFF		//	OFF 필수
//};

static U16 Buzzer_Beep_1[] = {
//	음계		ON Time		Echo Time	[1/10ms]
	La6, 	8, 			28,
	BuzzerOFF		//	OFF 필수
};

static U16 Buzzer_Beep_2[] = {
//	음계		ON Time		Echo Time	[1/10ms]
	La6, 	8, 			10,
	La6, 	8, 			28,
	BuzzerOFF		//	OFF 필수
};

static U16 Buzzer_Beep_3[] = {
//	음계		ON Time		Echo Time	[1/10ms]
	La6, 	8, 			10,
	La6, 	8, 			10,
	La6, 	8, 			28,
	BuzzerOFF		//	OFF 필수
};

static U16 Buzzer_Beep_4[] = {
//	음계		ON Time		Echo Time	[1/10ms]
	La6, 	8, 			10,
	La6, 	8, 			10,
	La6, 	8, 			10,
	La6, 	8, 			28,
	BuzzerOFF		//	OFF 필수
};

static U16 Buzzer_Beep_5[] = {
//	음계		ON Time		Echo Time	[1/10ms]
	La6, 	8, 			10,
	La6, 	8, 			10,
	La6, 	8, 			10,
	La6, 	8, 			10,
	La6, 	8, 			28,
	BuzzerOFF		//	OFF 필수
};

static U16* BuzzerArray[] = {
	NULL,
	Buzzer_PowerON,
	Buzzer_Effluent,
	Buzzer_Effluent_END,
	Buzzer_Cold_Setup,
	Buzzer_Cold_Cancle,
	Buzzer_Select,
	Buzzer_Error,
	Buzzer_SetUp,
	Buzzer_Cancle,
	Buzzer_Effluent_Continue,
	Buzzer_APConnect,
	Buzzer_ServerConnect,
//	Buzzer_AP_Pairing,
	Buzzer_Beep_1,
	Buzzer_Beep_2,
	Buzzer_Beep_3,
	Buzzer_Beep_4,
	Buzzer_Beep_5,
	Buzzer_Alram,
	NULL
};

/// @brief    부저 초기화
/// @param    void
/// @return   void
void Buzzer_Initialize(void)
{

}

/// @brief		부저 출력 설정
/// @param    void
/// @return   void
//void BuzzerSet(U8 bz)
//{
////	gu8_CommunicationCMD = SET;
//	if (gu8_VoiceVolumeLevel == 0)					//	무음 상태가 아니라면
//	{
//		if (bz < BUZZER_NULL)						//		부저 유효범위 체크
//		{
//			mu8_BuzzerReqeust = bz;					//			부저번호 set
//			mu8_BuzzerCnt = 0;
//			gu16_TimerBuzzer = 0;
//		}
//	}
//	else											//	무음 상태
//	{
//		if ((bz == BUZZER_COOL_SETUP) || (bz == BUZZER_COOL_CANCEL)
//			|| (bz == BUZZER_SETUP) || (bz == BUZZER_CANCEL)
//			|| (bz == BUZZER_AP_CONNECT) || (bz == BUZZER_SERVER_CONNECT))	//		무음상태에서도 발생하는 부저음
//		{
//			mu8_BuzzerReqeust = bz;					//			부저번호 set
//			mu8_BuzzerCnt = 0;
//			gu16_TimerBuzzer = 0;
//		}
//	}
//}

/// @brief    부저 출력 제어
/// @param    void
/// @return   void
void BuzzerControl(void)
{
	U16* mu16_pnt;
	static U8 mu8_Buzzer_Seq = 0;

	if (mu8_BuzzerReqeust == 0)						//	부저 요청이 없을 경우
	{
		Buzzer_OFF();								//		출력 정지 처리
		mu8_Buzzer_Seq = 0;							//		Seq = 0
	}
	else											//	부저 요청이 있을 경우
	{
		if (gu16_TimerBuzzer == 0)					//		부저 타이머 경과 시
		{
			mu16_pnt = &BuzzerArray[mu8_BuzzerReqeust][mu8_BuzzerCnt];	//			해당 부저 포인터 지정
			if ((const)mu16_pnt[0] == NULL)			//			해당 포인터에 부저 값이 없을 경우 (Invalid)
			{
				Buzzer_OFF();						//				출력 정지 처리
				mu8_Buzzer_Seq = 0;					//				Seq = 0
			}
			else if (mu16_pnt[0] == BuzzerOFF)		//			부저 정지 순서 인 경우
			{
				Buzzer_OFF();                       //				출력 정지 처리
				mu8_Buzzer_Seq = 0;                 //				Seq = 0
			}
			else									//			상기 외
			{
				gu16_TimerBuzzer = mu16_pnt[mu8_Buzzer_Seq + 1];	//			타이머 설정
				if (mu8_Buzzer_Seq == 0)			//			ON Seq
				{
					P_BUZZER = ON;
					BUZZER_TIMER_TDR = mu16_pnt[0];
					BUZZER_TIMER_TOE |= BUZZER_TIMER_SET;
					BUZZER_TIMER_TO |= BUZZER_TIMER_SET;
					BUZZER_TIMER_TS |= BUZZER_TIMER_SET;
				}
				else								//			Echo Seq
				{
					P_BUZZER = OFF;
					mu8_BuzzerCnt += 3;
				}
				mu8_Buzzer_Seq ^= 1;				//			Seq 절체
			}
		}
	}
}

/// @brief    부저 OFF 처리
/// @param    void
/// @return   void
static void Buzzer_OFF(void)
{
	P_BUZZER = OFF;
	BUZZER_TIMER_TT |= BUZZER_TIMER_SET;
	BUZZER_TIMER_TOE &= BUZZER_TIMER_CLR;
	BUZZER_TIMER_TO &= BUZZER_TIMER_CLR;
	mu8_BuzzerReqeust = 0;
	mu8_BuzzerCnt = 0;
}
