/// @file     Voice_NXD1005.c
/// @date     2022.01.02
/// @author   PSM
/// @brief    음성 처리 file
#define VOICE_H_
#include "include/_include.h"

static U8 SilentMode(U16 vocie_address);
static U8 SoundMode(U16 vocie_address);
static U8 VoiceMode(U16 vocie_address);

//---------------------------------------------------------
//       Voice Volume Address 지정(Next Lab 전용)
//---------------------------------------------------------
U16 gu16VoiceVolumeRegList[] = 
{ 
    VOICE_COMMAND_VOLUME_1,
    VOICE_COMMAND_VOLUME_2,
    VOICE_COMMAND_VOLUME_3,
    VOICE_COMMAND_VOLUME_4,
    VOICE_COMMAND_VOLUME_5
};
U16 gu16BeepVolumeRegList[] = 
{ 
    BEEP_COMMAND_VOLUME_1,
    BEEP_COMMAND_VOLUME_2,
    BEEP_COMMAND_VOLUME_3,
    BEEP_COMMAND_VOLUME_4,
    BEEP_COMMAND_VOLUME_5
};

//---------------------------------------------------------
//       Voice IC  초기화(Next Lab 전용)
//---------------------------------------------------------
void Initial_Voice(void)
{
//   P_VOICE_RESET = ON;
   Delay(400);    // 약25us
   P_VOICE_RESET = LOW;
   Delay(400);    // 약25us
   P_VOICE_RESET = HIGH;

   //set E2, E3 control Register
   COMMAND = VOICE_COMMAND_CONTROL2; //set Reg. E2 = 0x60
   SEND_SPI_COMMAND();



   COMMAND = VOICE_COMMAND_CONTROL3; //set Reg. E3 = 0x05
   SEND_SPI_COMMAND();


}
//---------------------------------------------------------
//       미  사  용
//---------------------------------------------------------
void SetVoiceLevel(U8 mu8Val)
{
    gu8_Sound_Type = mu8Val;
    if( gu8_Sound_Type == SOUND_TYPE_0 )
    {
        F_OneMoreSound = 1; /*절전 해지시 부저음 1회 출력*/
    }
}
//---------------------------------------------------------
//      음성타입 체크(무음,효과음,음성)
//---------------------------------------------------------
U8 Get_Sound_Type(void)
{
    return gu8_Sound_Type;
}
////---------------------------------------------------------
////      미  사  용
////---------------------------------------------------------
//void Voice_Coway_Start()
//{
//     if (F_Effluent_Hot)
//     {
//         if( Get_Sound_Type() >= SOUND_TYPE_3 )
//         {
//           if(F_New_Voice){Play_Voice(VOICE_EFFLUENT_COMPLETE);}
//           else           {Play_Voice(VOICE_EFFLUENT_COMPLETE);}
//         }
//         else
//         {
//           if(F_New_Voice){Play_Voice(VOICE_EFFLUENT_COMPLETE);}
//           else           {Play_Voice(VOICE_EFFLUENT_COMPLETE);}
//         }
//     }
//     else
//     {
//       if ( Get_Sound_Type() >= SOUND_TYPE_2 )
//       {
//           if(F_New_Voice){Play_Voice(VOICE_EFFLUENT_COMPLETE);}
//           else           {Play_Voice(VOICE_EFFLUENT_COMPLETE);}
//       }
//       else
//       {
//     //     VoiceStop();
//       }
//     }
//}
//---------------------------------------------------------
//       Voice 출력 정지(Next Lab 전용)
//---------------------------------------------------------
void VoiceStop(void)
{
   COMMAND = VOICE_COMMAND_STOP;
   SEND_SPI_COMMAND();
}
//---------------------------------------------------------
//       Voice 출력 지시(공용 가능 / STOP만 구분)
//---------------------------------------------------------
//static U8 IsLevel_0(U16 addr);
//static U8 IsLevel_1(U16 addr);
//static U8 IsLevel_2(U16 addr);
//static U8 IsLevel_3(U16 addr);
//static U8 IsCriticalVoice(U16 addr);
/*key 입력시 Paly_A_Voice(VOICE_XXXX); 선언*/
//////////U16 the_voice_addr;
//////////U16 gu16VoiceDelay = 3; /* 100ms */

void Play_Voice(U16 mu16MemoryAddress)
{
    U8 mu8_S_Type;

//    switch (mu16MemoryAddress)
//    {
//    	case VOICE_HOTWATER_EFFLUENT:
//    		break;
//    	case VOICE_HOTWATER_DRAIN:
//    		BuzzerSet(BUZZER_EFFLUENT);
//    		break;
//    	case VOICE_PRE_HEATING:
//    		if (gu8_CupSize != CUPSIZE_CONTINUE)
//    		{
//    			BuzzerSet(BUZZER_EFFLUENT);
//    		}
//    		else
//			{
//				BuzzerSet(BUZZER_EFFLUENT_CONTINUE);
//			}
//			break;
//		case VOICE_CONTIUNE_EFFLUENT:
//		case VOICE_FLUSHING_START:
//			BuzzerSet(BUZZER_EFFLUENT_CONTINUE);
//			break;
//		case VOICE_EFFLUENT_START:
//			BuzzerSet(BUZZER_EFFLUENT);
//			break;
//		case VOICE_EFFLUENT_COMPLETE:
//			BuzzerSet(BUZZER_EFFLUENT_END);
//			break;
//		case VOICE_POWER_ON:
//			BuzzerSet(BUZZER_POWER_ON);
//			break;
//		case VOICE_BEEP_SELECT:
//			BuzzerSet(BUZZER_SELECT);
//			break;
//		case VOICE_BEEP_WARNING:
//			BuzzerSet(BUZZER_ERROR);
//			break;
//		case VOICE_BEEP_ON:
//			BuzzerSet(BUZZER_SETUP);
//			break;
//		case VOICE_BEEP_OFF:
//			BuzzerSet(BUZZER_CANCEL);
//			break;
//		case VOICE_BEEP_BAD:
//			BuzzerSet(BUZZER_ERROR);
//			break;
//		case VOICE_BEEP_ALARM_1:
//			BuzzerSet(BUZZER_ERROR);
//			break;
//		case VOICE_BEEP_ALARM_2:
//			BuzzerSet(BUZZER_ERROR);
//			break;
//		case VOICE_CONTIUNE_EFFLUENT_2:
//			BuzzerSet(BUZZER_EFFLUENT_CONTINUE);
//			break;
//		case VOICE_CAREMODE_1:
//			BuzzerSet(BUZZER_SETUP);
//			break;
//		case VOICE_CAREMODE_2:
//			BuzzerSet(BUZZER_SETUP);
//			break;
//		case VOICE_TOPCOVER_WARNING:
//			BuzzerSet(BUZZER_Beep_2);
//			break;
//		case VOICE_INSTALL:
//			//	무음
//			break;
//		case VOICE_FILTER_CONNECT_CHECK:
//		case VOICE_FILTER_CONNECT_CHECK_B:
//			BuzzerSet(BUZZER_ERROR);
//			break;
//		case VOICE_ALL_LOCK_ICON22:
//			BuzzerSet(BUZZER_ERROR);
//			break;
//		default:
//			BuzzerSet(BUZZER_SETUP);
//			break;
//	}

//	if (gu8_ModelType_forNational == MODEL_KOREA)
//	{
		gu16_Voice_Address = mu16MemoryAddress;

		mu8_S_Type = Get_Sound_Type();

//		// 음성타입 [0] 해당없음 (무음모드와 동일처리)
//		if (mu8_S_Type == SOUND_TYPE_0)
//		{
//			if (IsLevel_0(mu16MemoryAddress) == TRUE)
//			{
//				F_OneMoreSound = 1;
//				F_PlayVoice = 1;
//			}
//		}
//		// 음성타입 [1] 무음모드
//		else if (mu8_S_Type == SOUND_TYPE_1)
//		{
//			if (IsLevel_0(mu16MemoryAddress) == TRUE
//				|| IsLevel_1(mu16MemoryAddress) == TRUE)
//			{
//				F_PlayVoice = 1;
//			}
//		}
//		// 음성타입 [2] 효과음모드
//		else if (mu8_S_Type == SOUND_TYPE_2)
//		{
//			if (IsLevel_0(mu16MemoryAddress) == TRUE
//				|| IsLevel_1(mu16MemoryAddress) == TRUE
//				|| IsLevel_2(mu16MemoryAddress) == TRUE)
//			{
//				F_PlayVoice = 1;
//			}
//		}
//		// 음성타입 [3] 안내모드
//		else if (mu8_S_Type == SOUND_TYPE_3)
//		{
//			if (IsLevel_0(mu16MemoryAddress) == TRUE
//				|| IsLevel_1(mu16MemoryAddress) == TRUE
//				|| IsLevel_2(mu16MemoryAddress) == TRUE
//				|| IsLevel_3(mu16MemoryAddress) == TRUE)
//			{
//				F_PlayVoice = 1;
//			}
//		}
		// 음성타입 [0] 해당없음 (무음모드와 동일처리)
		if (mu8_S_Type == SOUND_TYPE_0)
		{
			if (SilentMode(mu16MemoryAddress) == TRUE)
			{
				F_OneMoreSound = 1;
				F_PlayVoice = 1;
			}
		}
		// 음성타입 [1] 무음모드
		else if (mu8_S_Type == SOUND_TYPE_1)
		{
			if (SilentMode(mu16MemoryAddress) == TRUE)
			{
				F_PlayVoice = 1;
			}
		}
		// 음성타입 [2] 효과음모드
		else if (mu8_S_Type == SOUND_TYPE_2)
		{
			if (SoundMode(mu16MemoryAddress) == TRUE)
			{
				F_PlayVoice = 1;
			}
		}
		// 음성타입 [3] 안내모드
		else if (mu8_S_Type == SOUND_TYPE_3)
		{
			if (VoiceMode(mu16MemoryAddress) == TRUE)
			{
				F_PlayVoice = 1;
			}
		}

		if (mu8_S_Type == SOUND_TYPE_0)    //사운드 모드 아니면면
		{
			if (F_OneMoreSound)
			{
				F_OneMoreSound = 0;
			}
			else
			{
				return;
			}
		}

		/*Voice Stop*/
		//   VoiceStop();
		//   gu8Voice_IC_Type = VOICE_TYPE_NEXTLAB;
		//   if(gu8Voice_IC_Type == VOICE_TYPE_NEXTLAB)
		//   {
		VoiceStop();                        // NEXT Lab
		//   }
		//   else
		//   {
		//      gu16SPI_Clock_Gen = 100;           // Delay (1ms)
		//      gu8SPI_Tx_Step = 0x00;
		//   }
//	}
}
//---------------------------------------------------------
//       Voice 출력 처리(per 2ms) (Next Lab 전용)
// 효과음/음성안내 구분 -> 출력 볼륨결정
//---------------------------------------------------------
void ProcessVoice(void)
{
	if (gu16VoiceDelay)
	{
		gu16VoiceDelay--;
		return;
	}
	//==================================================
	// 보이스 출력 지시
	if (F_PlayVoice)
	{
		//-------------------------------------------------
		// (1) 출력 볼륨 제어
		//-------------------------------------------------
		if (gu16_Voice_Address == VOICE_CONTIUNE_EFFLUENT
			|| gu16_Voice_Address == VOICE_EFFLUENT_START
			|| gu16_Voice_Address == VOICE_EFFLUENT_COMPLETE
			|| gu16_Voice_Address == VOICE_POWER_ON
			|| gu16_Voice_Address == VOICE_BEEP_SELECT
			|| gu16_Voice_Address == VOICE_BEEP_WARNING
			|| gu16_Voice_Address == VOICE_BEEP_ON
			|| gu16_Voice_Address == VOICE_BEEP_OFF
			|| gu16_Voice_Address == VOICE_BEEP_BAD
			|| gu16_Voice_Address == VOICE_BEEP_ALARM_1
			|| gu16_Voice_Address == VOICE_BEEP_ALARM_2
//			|| gu16_Voice_Address == VOICE_CONTIUNE_EFFLUENT_2
//			|| gu16_Voice_Address == VOICE_CAREMODE_1
//			|| gu16_Voice_Address == VOICE_CAREMODE_2
			)
		{
			F_Beep_Sound = 1;                         // 효과음 판단 (볼륨 크기 구별-> 데시벨)
		}

		gu8PlayVoice_Count--;
		if (!gu8PlayVoice_Count)
		{
			F_PlayVoice = 0;
		}

		/*볼륨 설정*/
		// Critcal 상태인경우에는 볼륨 크게
		if ((IsCriticalVoice(gu16_Voice_Address) == TRUE) || (F_TestMode.byte != 0x00))
		{
			if (F_Beep_Sound)
			{
				COMMAND = 0xE100 + BEEP_COMMAND_VOLUME_5;      // 최대5단
			}
			else
			{
				COMMAND = 0xE100 + VOICE_COMMAND_VOLUME_5;
			}
		}
		// 일반 출력 항목
		else
		{
			if (F_Beep_Sound)
			{
				COMMAND = 0xE100 + gu16BeepVolumeRegList[gu8_VoiceVolumeLevel];
			}
			else
			{
				COMMAND = 0xE100 + gu16VoiceVolumeRegList[gu8_VoiceVolumeLevel];
			}
		}
		Delay(400);    // 약25us

		SEND_SPI_COMMAND();
		Delay(400);    // 약25us

      //-------------------------------------------------
      // (2) 출력 음성 제어
		//-------------------------------------------------
		//set $E4 register = 00 if address is < 256
		if (gu16_Voice_Address < 256)
		{              //set phrase group & higher 3 bit address on $E4 register
			COMMAND = VOICE_COMMAND_ADDRESS_256;    //phase group=0, high address = 000
			SEND_SPI_COMMAND();
			//set lower 8 bit address on $E0 register
			Delay(400);    // 약25us
			COMMAND = VOICE_COMMAND_ADDRESS_0 + gu16_Voice_Address;    //set lower 8 bit address on $E0 register
			SEND_SPI_COMMAND();
		}
        //set $E4 register = 0x01 if address is 256~511
        else if ( gu16_Voice_Address < 512)
		{              //set phrase group & higher 3 bit address on $E4 register
			COMMAND = VOICE_COMMAND_ADDRESS_512;    //phrase group = 1, high address = 1
			SEND_SPI_COMMAND();
			//set lower 8 bit address on $E0 register
			Delay(400);    // 약25us
			COMMAND = VOICE_COMMAND_ADDRESS_0 + (gu16_Voice_Address - 256);    //set lower 8 bit address on $E0 register
			SEND_SPI_COMMAND();
		}
	}
	//==================================================
	// 출력 지시 대기
	else
	{
		//gu8PlayVoice_Count = 2;                      // 연속 2번씩 출력 지시
		gu8PlayVoice_Count = 1;                      // 연속 1번만 출력 지시
	}
	//==================================================
	F_Beep_Sound = 0;

    
    //update the address for next play in case of SBT or Play_All mode
    //Delay_ms(16); //Audio_Out Delay Time at FS=8KHz
}

/****************************************************************************
                Output One Word Command to the Voice IC (공용가능)
 ***************************************************************************/
void SEND_SPI_COMMAND(void)
{
   int i = 0;

   P_VOICE_CSB = 0;                // start SPI

//   Delay(640);                    // 약40us

   //Delay(16000);                    // 약1ms (X)
   //Delay(20000);                    // 약1.25ms (X)
   Delay(24000);                    // 약1.5ms
   //Delay(32000);                    // 약2ms


   P_VOICE_CLK = 0;                // stand-by status,

   /*16bit 처리*/
   while (i < 16)
   {
      P_VOICE_CLK = 0;

      Delay(160);                 // 약10us

      if (COMMAND & MK_COMMAND)
      {
         P_VOICE_DATA = 1;
      }
      else
      {
         P_VOICE_DATA = 0;
      }

      COMMAND <<= 1;              // shift left

      Delay(160);                 // 약10us


      P_VOICE_CLK = 1;
      i++;

      if(i == 8)
      {
        Delay(640);               // 약40us (1Byte-1byte 패킷구분)
      }
      else
      {
        Delay(320);              // 약20us
      }
   }

   P_VOICE_DATA = 0;
   Delay(8);                    // 0.5us

   P_VOICE_CSB = 1;              // stand-by status

   Delay(320);                  // 20us
}

//------------------
// [4] Critical 상태
//------------------
U8 IsCriticalVoice(U16 addr)
{
    switch( addr )
    {
		// 플러그인
        case VOICE_POWER_ON:

//        //  에러 관련
//        case VOICE_COLD_WATER_ERROR:
//        case VOICE_HOT_WATER_ERROR:
//        case VOICE_SHORTAGE_ERROR:
//        case VOICE_LEAKAGE_ERROR:
//        case VOICE_SMART_CHECK_ERROR:
//        case VOICE_DETECT_ERROR:
//
//        // Wifi 관련
//        case VOICE_WIFI_START:
//        case VOICE_WIFI_COMPLETE:
//        case VOICE_WIFI_FAIL:
//        case VOICE_WIFI_OFF:
//        case VOICE_WIFI_START_A:
//        case VOICE_WIFI_COMPLETE_IOCARE:
//        case VOICE_WIFI_START_B:
//
//        // 출력 모드
//        case VOICE_VOICE_SETTING_3:
//        case VOICE_VOICE_SETTING_2:
//        case VOICE_VOICE_SETTING_1:
//        case VOICE_SETTING_VOICE:
//        case VOICE_SETTING_MELODY:
//        case VOICE_SETTING_SILENT:

        // 추가
        case VOICE_SPECIAL_FUNCTION:
            return TRUE;

        default :
            return FALSE;
    }

    return FALSE;
}


U8 SilentMode(U16 vocie_address)
{
	switch (vocie_address)
	{
//		case VOICE_VOICE_SETTING_3:
//		case VOICE_VOICE_SETTING_2:
//		case VOICE_VOICE_SETTING_1:
//		case VOICE_WIFI_START:
		case VOICE_POWER_ON:
		case VOICE_HOT_WATER_ERROR:
		case VOICE_SHORTAGE_ERROR:
		case VOICE_LEAKAGE_ERROR:
		case VOICE_CUP_SENSOR_ERROR:
		case VOICE_WIFI_START_A:
		case VOICE_WIFI_COMPLETE_IOCARE:
		case VOICE_WIFI_FAIL:
//		case VOICE_WIFI_START_B:
		case VOICE_WIFI_COMPLETE:
		case VOICE_WIFI_OFF:
		case VOICE_SPECIAL_FUNCTION:
//		case VOICE_WIFI_FAIL_B:
//		case VOICE_BLE_FAIL_B:
//		case VOICE_IOCARE_START_B:
//		case VOICE_IOCARE_COMPLETE_B:
//		case VOICE_IOCARE_FAIL_B:
		case VOICE_BEEP_BAD:
		case VOICE_BEEP_ALARM_1:
		case VOICE_BEEP_ALARM_2:
		case VOICE_BLE_START:
		case VOICE_BLE_COMPLETE:
		case VOICE_BLE_FAIL:
		case VOICE_SETTING_VOICE:
		case VOICE_SETTING_MELODY:
		case VOICE_SETTING_SILENT:
		case VOICE_DETECT_ERROR:
		case VOICE_CALL_SERVICE:
		case VOICE_COLD_WATER_ERROR:
		case VOICE_CLEAN_DUST_FILTER:
		case VOICE_SMART_CHECK_ERROR:
		case VOICE_SMART_CHECK_START:
		case VOICE_SMART_CHECK_OK:
		case VOICE_TOP_COVER_WARNING_B:
		case VOICE_CHANGE_FLOWFAUCET:
		case VOICE_CHANGE_FILTER_FLOWFAUCET:
		case VOICE_FLOWFAUCET_CONNECT_CHECK:
		case VOICE_FILTER_FLOWFAUCET_FLUSHING:
		case VOICE_STERIZATION_RINSE_ICON22:
		case VOICE_MYCUP_SET:
		case VOICE_CHANGE_FILTER:
//		case VOICE_FILTER_CONNECT_CHECK:
//		case VOICE_FILTER_CONNECT_WARNING:
		case VOICE_FILTER_CONNECT_CHECK_B:
		case VOICE_FILTER_FLUSHING:
		case VOICE_FILTER_SYSTEM_READY:
		case VOICE_INSTALL:
		case VOICE_FLUSHING_START:
		case VOICE_FLUSHING_COMPLETE:
		case VOICE_STERIZATION_START_ICON22:
		case VOICE_STERIZATION_END_ICON22:
		case VOICE_FILTER_OFF:
		case VOICE_FILTER_ON:
		case VOICE_FLOWFAUCET_OFF:
		case VOICE_FLOWFAUCET_ON:
			return TRUE;
			break;
		case VOICE_BEEP_SELECT:
		case VOICE_BEEP_WARNING:
		case VOICE_BEEP_ON:
		case VOICE_BEEP_OFF:
		case VOICE_CONTIUNE_EFFLUENT:
		case VOICE_EFFLUENT_START:
		case VOICE_EFFLUENT_COMPLETE:
//		case VOICE_CONTIUNE_EFFLUENT_2:
//		case VOICE_HOTLOCK_SETTING:
//		case VOICE_ALLLOCK_SETTING:
//		case VOICE_CAREMODE_1:
//		case VOICE_CAREMODE_2:
		case VOICE_CLEAN_WATER_COMPLETE:
		case VOICE_EW_START:
		case VOICE_HOT_LOCK_SETTING:
		case VOICE_ALL_LOCK_SETTING_A:
//		case VOICE_ALL_LOCK_SETTING_B:
		case VOICE_COLD_ON_SETTING:
//		case VOICE_COLD_ON_SETTING_JP:
		case VOICE_SETTING_MODE:
//		case VOICE_FILTER_CHANGE_END_ICON22:
//		case VOICE_HOTLOCK_CLEAR:
//		case VOICE_ALLLOCK_CLEAR:
//		case VOICE_COLD_OFF:
		case VOICE_HOT_LOCK_CLEAR:
		case VOICE_ALL_LOCK_CLEAR:
		case VOICE_COLD_OFF_SETTING:
//		case VOICE_HOTLOCK_CHECK:
//		case VOICE_ALLLOCK_CHECK:
//		case VOICE_TOPCOVER_CHECK:
//		case VOICE_TOPCOVER_WARNING:
//		case VOICE_LOWER_COVER_WARNING_A:
//		case VOICE_FRONT_COVER_WARNING_A:
//		case VOICE_SIDE_COVER_WARNING_A:
//		case VOICE_LOWER_COVER_WARNING_B:
//		case VOICE_FRONT_COVER_WARNING_B:
//		case VOICE_SIDE_COVER_WARNING_B:
		case VOICE_HOT_LOCK_CHECK:
		case VOICE_COLD_OFF_CHECK:
		case VOICE_ALL_LOCK_CHECK_CP:
//		case VOICE_ALL_LOCK_CHECK_CHP:
		case VOICE_TOP_COVER_CHECK:
		case VOICE_ALL_LOCK_ICON22:
		case VOICE_HOTWATER_READY:
//		case VOICE_HOTWATER_EFFLUENT:
		case VOICE_PRE_HEATING:
		case VOICE_HOTWATER_EFFLUENT_A:
//		case VOICE_HOTWATER_EFFLUENT_B:
		case VOICE_CLEAN_WATER_PREPARE:
		case VOICE_HIGH_HOTTEMP_EFFLUENT_1:
		case VOICE_HIGH_HOTTEMP_EFFLUENT_2:
		case VOICE_HOTWATER_FIRST:
		case 0xCF:
		default:
			return FALSE;
			break;
	}
}

U8 SoundMode(U16 vocie_address)
{
	switch (vocie_address)
	{
		/*	음성출력	*/
//		case VOICE_VOICE_SETTING_3:
//		case VOICE_VOICE_SETTING_2:
//		case VOICE_VOICE_SETTING_1:
//		case VOICE_WIFI_START:
		case VOICE_BEEP_SELECT:
		case VOICE_BEEP_WARNING:
		case VOICE_BEEP_ON:
		case VOICE_BEEP_OFF:
		case VOICE_POWER_ON:
		case VOICE_CONTIUNE_EFFLUENT:
		case VOICE_EFFLUENT_START:
		case VOICE_EFFLUENT_COMPLETE:
		case VOICE_HOT_WATER_ERROR:
		case VOICE_SHORTAGE_ERROR:
		case VOICE_LEAKAGE_ERROR:
		case VOICE_CUP_SENSOR_ERROR:
		case VOICE_WIFI_START_A:
		case VOICE_WIFI_COMPLETE_IOCARE:
		case VOICE_WIFI_FAIL:
//		case VOICE_WIFI_START_B:
		case VOICE_WIFI_COMPLETE:
		case VOICE_WIFI_OFF:
		case VOICE_SPECIAL_FUNCTION:
//		case VOICE_WIFI_FAIL_B:
//		case VOICE_BLE_FAIL_B:
//		case VOICE_IOCARE_START_B:
//		case VOICE_IOCARE_COMPLETE_B:
//		case VOICE_IOCARE_FAIL_B:
		case VOICE_BEEP_BAD:
		case VOICE_BEEP_ALARM_1:
		case VOICE_BEEP_ALARM_2:
		case VOICE_BLE_START:
		case VOICE_BLE_COMPLETE:
		case VOICE_BLE_FAIL:
		case VOICE_SETTING_VOICE:
		case VOICE_SETTING_MELODY:
		case VOICE_SETTING_SILENT:
		case VOICE_DETECT_ERROR:
		case VOICE_CALL_SERVICE:
//		case VOICE_CONTIUNE_EFFLUENT_2:
		case VOICE_COLD_WATER_ERROR:
		case VOICE_CLEAN_DUST_FILTER:
		case VOICE_SMART_CHECK_ERROR:
//		case VOICE_TOP_COVER_WARNING_A:
		case VOICE_CHANGE_FILTER:
		case VOICE_CHANGE_FLOWFAUCET:
		case VOICE_CHANGE_FILTER_FLOWFAUCET:
		case VOICE_FLOWFAUCET_CONNECT_CHECK:
		case VOICE_FILTER_FLOWFAUCET_FLUSHING:
		case VOICE_STERIZATION_RINSE_ICON22:
		case VOICE_MYCUP_SET:
		case VOICE_SMART_CHECK_START:
		case VOICE_SMART_CHECK_OK:
//		case VOICE_FILTER_CONNECT_CHECK:
//		case VOICE_FILTER_CONNECT_WARNING:
		case VOICE_FILTER_CONNECT_CHECK_B:
		case VOICE_FILTER_FLUSHING:
		case VOICE_FILTER_SYSTEM_READY:
		case VOICE_INSTALL:
		case VOICE_FLUSHING_START:
		case VOICE_FLUSHING_COMPLETE:
		case VOICE_STERIZATION_START_ICON22:
		case VOICE_STERIZATION_END_ICON22:
		case VOICE_TOP_COVER_WARNING_B:
		case VOICE_FILTER_OFF:
		case VOICE_FILTER_ON:
		case VOICE_FLOWFAUCET_OFF:
		case VOICE_FLOWFAUCET_ON:
			return TRUE;
			break;
//		case VOICE_HOTLOCK_SETTING:
//		case VOICE_ALLLOCK_SETTING:
//		case VOICE_CAREMODE_1:
//		case VOICE_CAREMODE_2:
		case VOICE_CLEAN_WATER_COMPLETE:
		case VOICE_EW_START:
		case VOICE_HOT_LOCK_SETTING:
		case VOICE_ALL_LOCK_SETTING_A:
//		case VOICE_ALL_LOCK_SETTING_B:
		case VOICE_COLD_ON_SETTING:
//		case VOICE_COLD_ON_SETTING_JP:
		case VOICE_SETTING_MODE:
//		case VOICE_FILTER_CHANGE_END_ICON22:
			gu16_Voice_Address = VOICE_BEEP_ON;
			return TRUE;
			break;

//		case VOICE_HOTLOCK_CLEAR:
//		case VOICE_ALLLOCK_CLEAR:
//		case VOICE_COLD_OFF:
		case VOICE_HOT_LOCK_CLEAR:
		case VOICE_ALL_LOCK_CLEAR:
		case VOICE_COLD_OFF_SETTING:
			gu16_Voice_Address = VOICE_BEEP_OFF;
			return TRUE;
			break;
//		case VOICE_HOTLOCK_CHECK:
//		case VOICE_ALLLOCK_CHECK:
//		case VOICE_TOPCOVER_CHECK:
//		case VOICE_TOPCOVER_WARNING:
//		case VOICE_LOWER_COVER_WARNING_A:
//		case VOICE_FRONT_COVER_WARNING_A:
//		case VOICE_SIDE_COVER_WARNING_A:
//		case VOICE_LOWER_COVER_WARNING_B:
//		case VOICE_FRONT_COVER_WARNING_B:
//		case VOICE_SIDE_COVER_WARNING_B:
		case VOICE_HOT_LOCK_CHECK:
		case VOICE_COLD_OFF_CHECK:
		case VOICE_ALL_LOCK_CHECK_CP:
//		case VOICE_ALL_LOCK_CHECK_CHP:
		case VOICE_TOP_COVER_CHECK:
		case VOICE_ALL_LOCK_ICON22:
			gu16_Voice_Address = VOICE_BEEP_WARNING;
			return TRUE;
			break;
//		case VOICE_HOTWATER_EFFLUENT:
		case VOICE_PRE_HEATING:
		case VOICE_HOTWATER_EFFLUENT_A:
//		case VOICE_HOTWATER_EFFLUENT_B:
		case VOICE_CLEAN_WATER_PREPARE:
		case VOICE_HIGH_HOTTEMP_EFFLUENT_1:
		case VOICE_HIGH_HOTTEMP_EFFLUENT_2:
			//if (gu8_CupSize == CUPSIZE_CONTINUE)
			#if 0
			if (gu16_CupSize == CUPSIZE_CONTINUE)
			#endif
			if(gu8_Cup_size_Array[gu8_CupIndex].amount_volume == CUPSIZE_CONTINUE)			//	i¿½i¿½i¿½i¿½i¿½I¸i¿½?
			{
				gu16_Voice_Address = VOICE_CONTIUNE_EFFLUENT;
			}
			else
			{
				gu16_Voice_Address = VOICE_EFFLUENT_START;
			}
			return TRUE;
			break;
		case VOICE_HOTWATER_READY:
		case VOICE_HOTWATER_FIRST:
		case 0xCF:
			if (gu8_OperationStep == Z_OperationStep_50)
			{
				/*
				if (gu8_CupSize == CUPSIZE_CONTINUE)
				
				{
					gu16_Voice_Address = VOICE_CONTIUNE_EFFLUENT;
				}
				else
				{
					gu16_Voice_Address = VOICE_EFFLUENT_START;
				}
				*/
				#if 0
				if (gu16_CupSize == CUPSIZE_CONTINUE)
				#endif
				if(gu8_Cup_size_Array[gu8_CupIndex].amount_volume == CUPSIZE_CONTINUE)		//	i¿½i¿½i¿½i¿½i¿½I¸i¿½?
				{
					gu16_Voice_Address = VOICE_CONTIUNE_EFFLUENT;
				}
				else
				{
					gu16_Voice_Address = VOICE_EFFLUENT_START;
				}
				
			}
			else
			{
				gu16_Voice_Address = VOICE_BEEP_WARNING;
			}
			return TRUE;
			break;
		default:
			return FALSE;
			break;
	}
}

U8 VoiceMode(U16 vocie_address)
{
	switch (vocie_address)
	{
//		case VOICE_HOTLOCK_SETTING:
//		case VOICE_HOTLOCK_CLEAR:
//		case VOICE_ALLLOCK_SETTING:
//		case VOICE_ALLLOCK_CLEAR:
//		case VOICE_HOTLOCK_CHECK:
//		case VOICE_COLD_OFF:
//		case VOICE_ALLLOCK_CHECK:
		case VOICE_HOTWATER_READY:
//		case VOICE_HOTWATER_EFFLUENT:
//		case VOICE_TOPCOVER_CHECK:
//		case VOICE_TOPCOVER_WARNING:
		// Wifi관련
//		case VOICE_WIFI_START:
		// 음성선택
//		case VOICE_VOICE_SETTING_3:
//		case VOICE_VOICE_SETTING_2:
//		case VOICE_VOICE_SETTING_1:
		// 에러
		// 추가
		case VOICE_DETECT_ERROR:
		case VOICE_CALL_SERVICE:
		// 효과
		case VOICE_BEEP_BAD:
		case VOICE_BEEP_ALARM_1:
		case VOICE_BEEP_ALARM_2:
//		case VOICE_CONTIUNE_EFFLUENT_2:
//		case VOICE_CAREMODE_1:
//		case VOICE_CAREMODE_2:
//		case VOICE_LOWER_COVER_WARNING_A:
//		case VOICE_TOP_COVER_WARNING_A:
//		case VOICE_FRONT_COVER_WARNING_A:
//		case VOICE_SIDE_COVER_WARNING_A:
		case VOICE_COLD_WATER_ERROR:
		case VOICE_CLEAN_DUST_FILTER:
		case VOICE_HOT_WATER_ERROR:
		case VOICE_SHORTAGE_ERROR:
		case VOICE_LEAKAGE_ERROR:
		case VOICE_CUP_SENSOR_ERROR:
		case VOICE_CHANGE_FILTER:
//		case VOICE_FILTER_CONNECT_CHECK:
		case VOICE_FILTER_FLUSHING:
		case VOICE_FILTER_SYSTEM_READY:
		case VOICE_SMART_CHECK_ERROR:
		case VOICE_PRE_HEATING:
		case VOICE_WIFI_START_A:
		case VOICE_WIFI_COMPLETE_IOCARE:
		case VOICE_WIFI_FAIL:
//		case VOICE_WIFI_START_B:
		case VOICE_WIFI_COMPLETE:
		case VOICE_WIFI_OFF:
		case VOICE_INSTALL:
		case VOICE_FLUSHING_START:
		case VOICE_FLUSHING_COMPLETE:
		case VOICE_BLE_START:
		case VOICE_BLE_COMPLETE:
		case VOICE_BLE_FAIL:
		case VOICE_HOTWATER_EFFLUENT_A:
//		case VOICE_HOTWATER_EFFLUENT_B:
//		case VOICE_LOWER_COVER_WARNING_B:
		case VOICE_TOP_COVER_WARNING_B:
//		case VOICE_FRONT_COVER_WARNING_B:
//		case VOICE_SIDE_COVER_WARNING_B:
		case VOICE_HOT_LOCK_CHECK:
		case VOICE_COLD_OFF_CHECK:
		case VOICE_ALL_LOCK_CHECK_CP:
//		case VOICE_ALL_LOCK_CHECK_CHP:
		case VOICE_CLEAN_WATER_PREPARE:
		case VOICE_CLEAN_WATER_COMPLETE:
		case VOICE_EW_START:
		case VOICE_HOT_LOCK_SETTING:
		case VOICE_HOT_LOCK_CLEAR:
		case VOICE_ALL_LOCK_SETTING_A:
//		case VOICE_ALL_LOCK_SETTING_B:
		case VOICE_ALL_LOCK_CLEAR:
		case VOICE_SETTING_VOICE:
		case VOICE_SETTING_MELODY:
		case VOICE_SETTING_SILENT:
		case VOICE_COLD_OFF_SETTING:
		case VOICE_COLD_ON_SETTING:
//		case VOICE_COLD_ON_SETTING_JP:
		case VOICE_SETTING_MODE:
		case VOICE_CONTIUNE_EFFLUENT:
		case VOICE_EFFLUENT_START:
		case VOICE_EFFLUENT_COMPLETE:
		case VOICE_POWER_ON:
		case VOICE_BEEP_SELECT:
		case VOICE_BEEP_WARNING:
		case VOICE_BEEP_ON:
		case VOICE_BEEP_OFF:
		// 추가
		case VOICE_TOP_COVER_CHECK:
		case VOICE_SPECIAL_FUNCTION:
//		case VOICE_WIFI_FAIL_B:
//		case VOICE_BLE_FAIL_B:
//		case VOICE_IOCARE_START_B:
//		case VOICE_IOCARE_COMPLETE_B:
//		case VOICE_IOCARE_FAIL_B:
//		case VOICE_FILTER_CONNECT_WARNING:
		case VOICE_FILTER_CONNECT_CHECK_B:
		case VOICE_ALL_LOCK_ICON22:
		case VOICE_STERIZATION_START_ICON22:
		case VOICE_STERIZATION_END_ICON22:
//		case VOICE_FILTER_CHANGE_END_ICON22:
		case VOICE_CHANGE_FLOWFAUCET:
		case VOICE_CHANGE_FILTER_FLOWFAUCET:
		case VOICE_FLOWFAUCET_CONNECT_CHECK:
		case VOICE_FILTER_FLOWFAUCET_FLUSHING:
		case VOICE_STERIZATION_RINSE_ICON22:
		case VOICE_MYCUP_SET:
		case VOICE_SMART_CHECK_START:
		case VOICE_SMART_CHECK_OK:
		case VOICE_HIGH_HOTTEMP_EFFLUENT_1:
		case VOICE_HIGH_HOTTEMP_EFFLUENT_2:
		case VOICE_HOTWATER_FIRST:
		case VOICE_FILTER_OFF:
		case VOICE_FILTER_ON:
		case VOICE_FLOWFAUCET_OFF:
		case VOICE_FLOWFAUCET_ON:
		case 0xCF:
			return TRUE;
			break;
		default:
			return FALSE;
			break;
	}
}


