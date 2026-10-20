/// @file     keycontrol.c
/// @date     2019.07.10
/// @author   PSM
/// @brief    키입력 제어 관련 file
#define KEYCONTROL_C
#include "include/_include.h"

typedef struct
{
	const U8 KeyData;
	const U16 KeyNum;
	const U16 ShortKeyTime;
	const U8 ShortKeyType;
	const U16 LongKeyTime;
	const U8 LongKeyType;
	const U16 VeryLongKeyTime;
	const U8 VeryLongKeyType;
	const U16 FCTLongKeyTime;
	const U8 FCTLongKeyType;
	const U8 OnBreak;
} KeyTable;

#define OFFEDGE (0)
#define LEVEL (1)

enum
{
	enum_KEY_HOTWATER,
	enum_KEY_COLDWATER,
	enum_KEY_PUREWATER,
	enum_KEY_AMOUNT,
	enum_KEY_EFFLUENT,
	enum_KEY_LINETEST_MANAGE,
	enum_KEY_COLDTANK_PBATEST,
	enum_KEY_ERROR_HIS,
	enum_KEY_ALL_LOCK,
	enum_KEY_WIFI_AP_PAIRING,
	enum_KEY_WIFI_ONOFF,
	enum_KEY_FLU_CAN_TH_CON,
	enum_KEY_SLEEPMODE,
	enum_KEY_STE_KITMODE,
	enum_KEY_SOUND_TYPE,
	enum_KEY_VALUE_TEM_SMPS_TEST,
	enum_KEY_MAX
};

#define KEY_UNDEFINE				(0)
#define KEY_VALUE_HOTWATER			(0x01)
#define KEY_VALUE_COLDWATER			(0x02)
#define KEY_VALUE_PUREWATER			(0x04)
#define KEY_VALUE_AMOUNT			(0x08)
#define KEY_VALUE_EFFLUENT			(0x10)

#define KEY_VALUE_LINETEST_MANAGE	(KEY_VALUE_HOTWATER | KEY_VALUE_AMOUNT)
#define KEY_VALUE_COLDTANK_PBATEST	(KEY_VALUE_HOTWATER | KEY_VALUE_COLDWATER)
#define KEY_VALUE_ERROR_HIS		(KEY_VALUE_HOTWATER | KEY_VALUE_PUREWATER | KEY_VALUE_COLDWATER)
#define KEY_VALUE_ALL_LOCK		(KEY_VALUE_HOTWATER | KEY_VALUE_PUREWATER)
#define KEY_VALUE_WIFI_AP_PAIRING	(KEY_VALUE_PUREWATER | KEY_VALUE_COLDWATER)
#define KEY_VALUE_WIFI_ONOFF		(KEY_VALUE_COLDWATER | KEY_VALUE_AMOUNT)
#define KEY_VALUE_SLEEPMODE		(KEY_VALUE_HOTWATER | KEY_VALUE_COLDWATER | KEY_VALUE_PUREWATER | KEY_VALUE_AMOUNT)
#define KEY_VALUE_FLU_CAN_TH_CON	(KEY_VALUE_PUREWATER | KEY_VALUE_COLDWATER | KEY_VALUE_AMOUNT)
#define KEY_VALUE_STE_KITMODE		(KEY_VALUE_HOTWATER | KEY_VALUE_PUREWATER | KEY_VALUE_AMOUNT)
#define KEY_VALUE_SOUND_TYPE		(KEY_VALUE_PUREWATER | KEY_VALUE_AMOUNT)

#define KEY_VALUE_TEM_SMPS_TEST		(KEY_VALUE_HOTWATER | KEY_VALUE_COLDWATER | KEY_VALUE_AMOUNT)


//	※ 단키는 프론트에서 정의
KeyTable KeyArray[enum_KEY_MAX] = {
//	Key Value				Key Number			Short Time	Short Type	Long Time	Long Type		V_Long Time		V_Long Type		FCT Long Time	FCT Long Type	On Break
	{KEY_VALUE_HOTWATER, 			enum_KEY_HOTWATER, 		KEY_UNDEFINE, 	KEY_UNDEFINE,	250, 		LEVEL, 			500, 			LEVEL,		 	KEY_UNDEFINE, 	KEY_UNDEFINE,	OFF },
	{KEY_VALUE_COLDWATER, 			enum_KEY_COLDWATER, 		KEY_UNDEFINE, 	KEY_UNDEFINE,	250, 		LEVEL,			KEY_UNDEFINE, 		KEY_UNDEFINE, 		KEY_UNDEFINE, 	KEY_UNDEFINE,	OFF },
	{KEY_VALUE_PUREWATER, 			enum_KEY_PUREWATER, 		KEY_UNDEFINE, 	KEY_UNDEFINE,	250, 		LEVEL, 			KEY_UNDEFINE, 		KEY_UNDEFINE, 		KEY_UNDEFINE, 	KEY_UNDEFINE,	OFF },
	{KEY_VALUE_AMOUNT, 			enum_KEY_AMOUNT, 		KEY_UNDEFINE, 	KEY_UNDEFINE,	250, 		LEVEL, 			KEY_UNDEFINE,		KEY_UNDEFINE, 		KEY_UNDEFINE, 	KEY_UNDEFINE,	OFF },
	{KEY_VALUE_EFFLUENT, 			enum_KEY_EFFLUENT, 		KEY_UNDEFINE, 	KEY_UNDEFINE,	200, 		LEVEL, 			800, 			LEVEL, 			KEY_UNDEFINE, 	KEY_UNDEFINE,	OFF	},
//@023	{KEY_VALUE_LINETEST_MANAGE,		enum_KEY_LINETEST_MANAGE,	KEY_UNDEFINE, 	KEY_UNDEFINE, 	150, 		LEVEL, 			250, 			LEVEL, 			KEY_UNDEFINE, 	KEY_UNDEFINE, 	OFF },
	{KEY_VALUE_LINETEST_MANAGE,		enum_KEY_LINETEST_MANAGE,	KEY_UNDEFINE, 	KEY_UNDEFINE, 	150, 		LEVEL, 			250, 			LEVEL, 			150, 		LEVEL, 		OFF },	//@023
	{KEY_VALUE_COLDTANK_PBATEST,	        enum_KEY_COLDTANK_PBATEST,	KEY_UNDEFINE, 	KEY_UNDEFINE, 	100, 		LEVEL, 			250,			LEVEL, 			KEY_UNDEFINE, 	KEY_UNDEFINE, 	OFF },
	{KEY_VALUE_ERROR_HIS,			enum_KEY_ERROR_HIS,		KEY_UNDEFINE, 	KEY_UNDEFINE, 	250, 		LEVEL, 			KEY_UNDEFINE,		KEY_UNDEFINE, 		KEY_UNDEFINE, 	KEY_UNDEFINE, 	OFF },
	{KEY_VALUE_ALL_LOCK,			enum_KEY_ALL_LOCK,		KEY_UNDEFINE,	KEY_UNDEFINE, 	250, 		LEVEL, 			KEY_UNDEFINE, 		KEY_UNDEFINE, 		KEY_UNDEFINE, 	KEY_UNDEFINE, 	OFF },
//@023	{KEY_VALUE_WIFI_AP_PAIRING,		enum_KEY_WIFI_AP_PAIRING,	KEY_UNDEFINE,	KEY_UNDEFINE, 	250, 		LEVEL, 			KEY_UNDEFINE, 		KEY_UNDEFINE, 		KEY_UNDEFINE, 	KEY_UNDEFINE, 	OFF },
	{KEY_VALUE_WIFI_AP_PAIRING,		enum_KEY_WIFI_AP_PAIRING,	KEY_UNDEFINE,	KEY_UNDEFINE, 	250, 		LEVEL, 			KEY_UNDEFINE, 		KEY_UNDEFINE, 		100, 		LEVEL, 		OFF },	//@023
	{KEY_VALUE_WIFI_ONOFF,			enum_KEY_WIFI_ONOFF,		KEY_UNDEFINE,	KEY_UNDEFINE, 	250, 		LEVEL, 			KEY_UNDEFINE, 		KEY_UNDEFINE, 		KEY_UNDEFINE, 	KEY_UNDEFINE, 	OFF },
//@023#ifdef _FOR_DEVELOP_
//@023	{KEY_VALUE_FLU_CAN_TH_CON,		enum_KEY_FLU_CAN_TH_CON,	KEY_UNDEFINE,	KEY_UNDEFINE, 	150, 		LEVEL, 			KEY_UNDEFINE, 		KEY_UNDEFINE, 		KEY_UNDEFINE, 	KEY_UNDEFINE, 	OFF },
//@023#else
//@023	{KEY_VALUE_FLU_CAN_TH_CON,		enum_KEY_FLU_CAN_TH_CON,	KEY_UNDEFINE,	KEY_UNDEFINE, 	600, 		LEVEL, 			KEY_UNDEFINE, 		KEY_UNDEFINE, 		KEY_UNDEFINE, 	KEY_UNDEFINE, 	OFF },
//@023#endif
#ifdef _FOR_DEVELOP_
	{KEY_VALUE_FLU_CAN_TH_CON,		enum_KEY_FLU_CAN_TH_CON,	KEY_UNDEFINE,	KEY_UNDEFINE, 	150, 		LEVEL, 			KEY_UNDEFINE, 		KEY_UNDEFINE, 		150, 		LEVEL, 		OFF },	//@023
#else
	{KEY_VALUE_FLU_CAN_TH_CON,		enum_KEY_FLU_CAN_TH_CON,	KEY_UNDEFINE,	KEY_UNDEFINE, 	600, 		LEVEL, 			KEY_UNDEFINE, 		KEY_UNDEFINE, 		150, 		LEVEL, 		OFF },	//@023
#endif  
	{KEY_VALUE_TEM_SMPS_TEST,		enum_KEY_VALUE_TEM_SMPS_TEST,	KEY_UNDEFINE,	KEY_UNDEFINE, 	600, 		LEVEL, 			KEY_UNDEFINE, 		KEY_UNDEFINE, 		150, 		LEVEL, 		OFF },	//@023

	{KEY_VALUE_SLEEPMODE,			enum_KEY_SLEEPMODE,		KEY_UNDEFINE,	KEY_UNDEFINE, 	450, 		LEVEL, 			KEY_UNDEFINE, 		KEY_UNDEFINE, 		KEY_UNDEFINE, 	KEY_UNDEFINE, 	OFF },
	{KEY_VALUE_STE_KITMODE,			enum_KEY_STE_KITMODE,		KEY_UNDEFINE,	KEY_UNDEFINE, 	250, 		LEVEL, 			KEY_UNDEFINE, 		KEY_UNDEFINE, 		KEY_UNDEFINE, 	KEY_UNDEFINE, 	OFF },
	{KEY_VALUE_SOUND_TYPE,			enum_KEY_SOUND_TYPE,		KEY_UNDEFINE,	KEY_UNDEFINE, 	250, 		LEVEL, 			KEY_UNDEFINE, 		KEY_UNDEFINE, 		KEY_UNDEFINE, 	KEY_UNDEFINE, 	OFF },
};

/// @brief    Key 입력 처리 초기화
/// @param    void
/// @return   void
void KeyControl_Initialize(void)
{

}

///// @brief    Key 입력 처리
///// @param    void
///// @return   void
//void KeyControl(void)
//{
//	U8 data;
//	BYTE_BIT work;
//
//	Delay_Nop();
//
//	if (P_KEY_MATRIX == 0)
//	{
//		data = (U8) (1 << (gu8_ComPortChange - 1));
//		gu8_TouchData.byte |= data;
//	}
//	else
//	{
//		data = (U8) (~(1 << (gu8_ComPortChange - 1)));
//		gu8_TouchData.byte &= data;
//	}
//}

/// @brief    Key 확정
/// @param    void
/// @return   void
void KeyDecide(ITTI1* p)
{
	U8 i = 0;
	U8 data = p->Level.byte;						//	3회 일치모듈의 데이터를 받아서
	static U8 buf = 0;								//	키 버퍼 저장
	static KeyTable *KeyPnt = NULL;					//	해당 KeyTable 포인터

//	Cold_Fauset_Decide();							//	파우셋은 키 단독 처리
//	Hot_Fauset_Decide();							//	파우셋은 키 단독 처리

//	gu16_ShortKeyArray.WORD = 0x0000U;				//	데이터 CLEAR
	gu32_LongKeyArray.LONG = 0x00000000U;				//	데이터 CLEAR
	gu32_VeryLongKeyArray.LONG = 0x00000000U;           //	데이터 CLEAR
	gu32_FCTLongKeyArray.LONG = 0x00000000U;            //	데이터 CLEAR

	switch (gu8_KeyControlStep)						//		step에 따라
	{
		case 0:										//			대기 스텝
			gu8_TimerKeyOnBreak = 100;				//				온고장 확인 타이머
			F_VeryLongKey_Rejection = CLEAR;		//				베리롱키 금지 플래그 CLEAR
			if (data != 0)							//				Key 입력이 있으면
			{
				for (i = 0; i < enum_KEY_MAX; i++)	//				해당하는 Key 검색
				{
					if (data == KeyArray[i].KeyData)//				Key가 일치 할 시
					{
						if ((KeyArray[i].OnBreak == ON) && (gu8_KeyOnBreak == KeyArray[i].KeyData))	//			온고장 해당키 and 전원부팅 데이터 비교
						{
							break;
						}
						buf = data;					//					버퍼 저장
						KeyPnt = &KeyArray[i];		//					Key 포인터
						if (KeyPnt->ShortKeyTime != 0)	 //			Short Key 데이터가 있을 경우
						{
							gu8_KeyControlStep = 1;	//					Short Key 확정 스텝
							gu16_TimerKeyDecide = KeyPnt->ShortKeyTime;     //				Short Timer 설정
						}
						else						//			Short Key 데이터가 없을 경우
						{
							if (F_TestMode.byte != 0x00)	//			테스트모드인 경우
							{
								if (KeyPnt->FCTLongKeyTime != 0)	//				테스트전용 롱키 유무 확인
								{
									gu8_KeyControlStep = 10;	//					테스트전용 롱키 스텝
									gu16_TimerKeyDecide = KeyPnt->FCTLongKeyTime;     //				Short Timer 설정
								}
								else if (KeyPnt->LongKeyTime != 0)	//			Long Key 데이터가 있을 경우
								{
									gu8_KeyControlStep = 2;		//					2-Step
									gu16_TimerKeyDecide = KeyPnt->LongKeyTime;     //				Short Timer 설정
								}
								else
								{

								}
							}
							else					//			일반모드 경우
							{
								if (KeyPnt->LongKeyTime != 0)	//			Long Key 데이터가 있을 경우
								{
									gu8_KeyControlStep = 2;		//					2-Step
									gu16_TimerKeyDecide = KeyPnt->LongKeyTime;     //				Short Timer 설정
								}
							}
						}
					}
				}
			}
			break;
		case 1:										//		Short Key 확정
			if ((gu16_TimerKeyDecide <= 0)) 		//			Short Timer 경과 시
			{
				if ((F_ShortKeyProhibit == CLEAR)
					&& ((KeyPnt->ShortKeyType == LEVEL)
					|| ((KeyPnt->ShortKeyType == OFFEDGE) && (buf == p->OffEdge.byte))))	//				Short Key가 OFF edge 검출이 아닐 시
				{
//					gu16_ShortKeyArray.WORD = 1 << KeyPnt->KeyNum;
					gu8_TimerKeyProhibit = Z_TimerKeyProhibit;		//					접수금지
					gu8_KeyControlStep = 0xFF;
					if (KeyPnt->ShortKeyType == OFFEDGE)	//				OFF Edge 키면
					{
						break;						//				switch 구조문 탈출 (하기 if문에서 스텝2(롱키)로 넘어가지 않게끔)
					}
				}

//				if (F_TestMode == SET)			//			테스트모드인 경우
//				{
//					if (KeyPnt->FCTLongKeyTime != 0)	//			테스트 전용 Long Key 데이터가 있을 경우
//					{
//						gu16_TimerKeyDecide = KeyPnt->FCTLongKeyTime - KeyPnt->ShortKeyTime;     //				Timer 설정
//						gu8_KeyControlStep = 10;    //					테스트 전용 Long Key 확정 스텝
//					}
//				}
//				else								//			일반모드인 경우
//				{
					if (KeyPnt->LongKeyTime != 0)	//			Long Key 데이터가 있을 경우
					{
						gu16_TimerKeyDecide = KeyPnt->LongKeyTime - KeyPnt->ShortKeyTime;     //				Timer 설정
						gu8_KeyControlStep = 2;		//					Long Key 확정 스텝
					}
					else if (data == 0x00)					//		현재키가 모두 안눌려져있는 상태
					{
						gu8_TimerKeyProhibit = Z_TimerKeyProhibit;    //					접수금지
						gu8_KeyControlStep = 0xFF;			//					접수금지
					}
					else if (buf != data)					//				데이터가 다를 시
					{
						gu8_KeyControlStep = 0;				//				복합키 판단
					}
					else
					{

					}
//				}
			}
			else if (data == 0x00)					//		현재키가 모두 안눌려져있는 상태
			{
				gu8_TimerKeyProhibit = Z_TimerKeyProhibit;    //					접수금지
				gu8_KeyControlStep = 0xFF;			//					접수금지
			}
			else if (buf != data)					//				데이터가 다를 시
			{
				gu8_KeyControlStep = 0;				//				복합키 판단
			}
			else
			{

			}
			break;

//			if (data == 0x00)					//		현재키가 모두 안눌려져있는 상태
//			{
//				gu8_TimerKeyProhibit = Z_TimerKeyProhibit;    //					접수금지
//				gu8_KeyControlStep = 0xFF;			//					접수금지
//			}
//			else if (buf != data)					//				데이터가 다를 시
//			{
//				gu8_KeyControlStep = 0;				//				복합키 판단
//			}
//			else if ((gu16_TimerKeyDecide <= 0)) 		//			Short Timer 경과 시
//			{
//				if ((F_ShortKeyProhibit == CLEAR)
//					&& ((KeyPnt->ShortKeyType == LEVEL)
//					|| ((KeyPnt->ShortKeyType == OFFEDGE) && (buf == p->OffEdge.byte))))	//				Short Key가 OFF edge 검출이 아닐 시
//				{
////					gu16_ShortKeyArray.WORD = 1 << KeyPnt->KeyNum;
//					gu8_TimerKeyProhibit = Z_TimerKeyProhibit;		//					접수금지
//					gu8_KeyControlStep = 0xFF;
//					if (KeyPnt->ShortKeyType == OFFEDGE)	//				OFF Edge 키면
//					{
//						break;						//				switch 구조문 탈출 (하기 if문에서 스텝2(롱키)로 넘어가지 않게끔)
//					}
//				}
//
//				if (F_TestMode.byte != 0x00)			//			테스트모드인 경우
//				{
//					if (KeyPnt->FCTLongKeyTime != 0)	//			테스트 전용 Long Key 데이터가 있을 경우
//					{
//						gu16_TimerKeyDecide = KeyPnt->FCTLongKeyTime - KeyPnt->ShortKeyTime;     //				Timer 설정
//						gu8_KeyControlStep = 10;    //					테스트 전용 Long Key 확정 스텝
//					}
//					else if (KeyPnt->LongKeyTime != 0)    //			Long Key 데이터가 있을 경우
//					{
//						gu8_KeyControlStep = 2;		//					2-Step
//						gu16_TimerKeyDecide = KeyPnt->LongKeyTime;    //				Short Timer 설정
//					}
//					else
//					{
//
//					}
//				}
//				else								//			일반모드인 경우
//				{
//					if (KeyPnt->LongKeyTime != 0)	//			Long Key 데이터가 있을 경우
//					{
//						gu16_TimerKeyDecide = KeyPnt->LongKeyTime - KeyPnt->ShortKeyTime;     //				Timer 설정
//						gu8_KeyControlStep = 2;		//					Long Key 확정 스텝
//					}
//				}
//			}
//			else
//			{
//
//			}
			break;
		case 2:										//	Long Key 확정
			if ((gu16_TimerKeyDecide <= 0))
			{										//
				if ((KeyPnt->LongKeyType == LEVEL) || ((KeyPnt->LongKeyType == OFFEDGE) && (buf == p->OffEdge.byte)))
				{
					gu32_LongKeyArray.LONG = (U32)1 << KeyPnt->KeyNum;
					gu8_TimerKeyProhibit = Z_TimerKeyProhibit;	//					접수금지
					gu8_KeyControlStep = 0xFF;		//					접수금지
				}
				if (KeyPnt->VeryLongKeyTime != 0)	//			Very Long Key 데이터가 있을 경우
				{
					gu16_TimerKeyDecide = KeyPnt->VeryLongKeyTime - KeyPnt->LongKeyTime - KeyPnt->ShortKeyTime;     //				Timer 설정
					gu8_KeyControlStep = 3;			//					4-Step
				}
			}
			else if ((KeyPnt->ShortKeyType == OFFEDGE) && (buf == p->OffEdge.byte))	//		Short Key가 OffEdge 타입인 경우
			{
//				gu16_ShortKeyArray.WORD = 1 << KeyPnt->KeyNum;	//			Short Key 확정
				gu8_TimerKeyProhibit = Z_TimerKeyProhibit;		//					접수금지
				gu8_KeyControlStep = 0xFF;			//					접수금지
			}
			else if (data == 0x00)					//		현재키가 모두 안눌려져있는 상태
			{
				gu8_TimerKeyProhibit = Z_TimerKeyProhibit;     //					접수금지
				gu8_KeyControlStep = 0xFF;			//					접수금지
			}
			else if (buf != data)
			{										//				데이터가 다를 시
//				gu8_TimerKeyProhibit = Z_TimerKeyProhibit;	//					접수금지
//				gu8_KeyControlStep = 0xFF;						//					접수금지
				F_ShortKeyProhibit = SET;			//				단키 중복 금지
				gu8_KeyControlStep = 0;				//				복합키 판단
			}
			else
			{

			}
			break;
		case 3:										//		Very Long Key 확정 스텝
			if ((gu16_TimerKeyDecide <= 0))			//			타이머 경과인 경우
			{										//
				if ((KeyPnt->VeryLongKeyType == LEVEL)	//				Level이 ON상태
					|| ((KeyPnt->VeryLongKeyType == OFFEDGE) && (buf == p->OffEdge.byte)))		//				OFFEdge 상태
				{
					if (F_VeryLongKey_Rejection == CLEAR)	//			VeryLongKey 거부 flag CLEAR인 경우
					{
						gu32_VeryLongKeyArray.LONG = (U32)1 << KeyPnt->KeyNum;
						gu8_TimerKeyProhibit = Z_TimerKeyProhibit;		//					접수금지
					}
					gu8_KeyControlStep = 0xFF;		//					접수금지
				}
			}
			else if (data == 0x00)					//		현재키가 모두 안눌려져있는 상태
			{
				gu8_TimerKeyProhibit = Z_TimerKeyProhibit;	//					접수금지
				gu8_KeyControlStep = 0xFF;			//					접수금지
			}
			else if (buf != data)					//				데이터가 다를 시
			{
//				gu8_TimerKeyProhibit = Z_TimerKeyProhibit;	//					접수금지
//				gu8_KeyControlStep = 0xFF;						//					접수금지
				F_ShortKeyProhibit = SET;			//				단키 중복 금지
				gu8_KeyControlStep = 0;				//				복합키 판단
			}
			else
			{

			}
			break;
		case 10:									//	FCT 전용 Long Key
			if ((gu16_TimerKeyDecide <= 0))			//		타이머 경과 시
			{
				if ((KeyPnt->FCTLongKeyType == LEVEL)
				|| ((KeyPnt->FCTLongKeyType == OFFEDGE) && (buf == p->OffEdge.byte)))
				{
					gu32_FCTLongKeyArray.LONG = (U32)1 << KeyPnt->KeyNum;
					gu32_LongKeyArray.LONG = (U32)1 << KeyPnt->KeyNum;	//
					gu8_TimerKeyProhibit = Z_TimerKeyProhibit;		//					접수금지
					gu8_KeyControlStep = 0xFF;		//					접수금지
				}
			}
			else if ((KeyPnt->ShortKeyType == OFFEDGE) && (buf == p->OffEdge.byte))
			{
//				gu16_ShortKeyArray.WORD = 1 << KeyPnt->KeyNum;
				gu8_TimerKeyProhibit = Z_TimerKeyProhibit;		//					접수금지
				gu8_KeyControlStep = 0xFF;						//					접수금지
			}
			else if (data == 0x00)					//		현재키가 모두 안눌려져있는 상태
			{
				gu8_TimerKeyProhibit = Z_TimerKeyProhibit;     //					접수금지
				gu8_KeyControlStep = 0xFF;			//					접수금지
			}
			else if (buf != data)
			{										//				데이터가 다를 시
//				gu8_TimerKeyProhibit = Z_TimerKeyProhibit;	//					접수금지
//				gu8_KeyControlStep = 0xFF;						//					접수금지
				F_ShortKeyProhibit = SET;			//				단키 중복 금지
				gu8_KeyControlStep = 0;				//				복합키 판단
			}
			else
			{

			}
			break;
		case 0xFF:									//		입력금지 seq
			F_ShortKeyProhibit = CLEAR;
			if ((data == 0x00) && (gu8_TimerKeyProhibit == 0))     //			키 데이터 clear and 입력금지 타이머 경과 시
			{
				gu8_KeyControlStep = 0;				//		대기 seq
			}

			/*	온고장 처리	*/
			for (i = 0; i < enum_KEY_MAX; i++)		//				해당하는 Key 검색
			{
				if (data == KeyArray[i].KeyData)	//				Key가 일치 할 시
				{
					if ((KeyArray[i].OnBreak == ON) && (gu8_TimerKeyOnBreak == 0))     //			온고장 확인 Key인 경우 and 온고장 타이머 경과 시
					{
//						WDTE = 0xFFU;
						//	※ 온고장 시에 전원을 부팅하는 것은 사용자에게 보이는 것이니 금지. 차 회만 입력 안받도록 설계
					}
				}
			}
			break;
		default:
			break;
	}

	if (gu8_KeyControlStep == 0)					//	대기스텝인 경우
	{
		buf = 0;									//		버퍼 CLEAR
		KeyPnt = NULL;								//		포인터 해제
	}

//	if ((gu16_ShortKeyArray.WORD != 0x0000U) || (gu16_LongKeyArray.WORD != 0x0000U) || (gu16_VeryLongKeyArray.WORD != 0x0000U) || (gu16_FCTLongKeyArray.WORD == 0x0000U))
//	{
//		F_CommunicationCMD = SET;
//	}
}
