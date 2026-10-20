#include "WIFI_Common.h"
#include "WIFI_SetFunctionData.h"
#include "WIFI_SetSensorData.h"
#include "WIFI_SetErrorData.h"
#include "WIFI_SetPartData.h"
#include "WIFI_SetAccumulateData.h"
#include "WIFI_Control.h"

#ifdef MODEL_NO_3

#define _HOT_TEMP_4STEP_

#include "include/_type_define.h"
#include "operation.h"
#include "buzzer.h"
#include "eeprom.h"
#include "pure.h"
#include "error.h"
#include "adc.h"
#include "cold.h"
#include "hot.h"
#include "display.h"
#include "input.h"
#include "_fix_ram.h"
#include "timer.h"
#include "_port_define.h"
#include "Voice_NXD1005.h"
#include "smartcheck.h"


void UserWriteEep ( U16 mu8Addr, U8 mu8Val )
{
//    WriteEx(mu8Addr,mu8Val);
	EEPROM_Data_Table[mu8Addr].write_flag = SET;
	*EEPROM_Data_Table[mu8Addr].data = mu8Val;
}

U8 UserReadEep ( U16 mu8Addr )
{
    U8 mu8Data = 0U;
//    mu8Data = ReadEx(mu8Addr);
    mu8Data = *EEPROM_Data_Table[mu8Addr].data;
    return mu8Data;
}

void UserBuzzSound ( U8 mu8Buzz )
{
	Play_Voice(mu8Buzz);
}

U8 GetUserSystemStatus ( U8 mu8Value )
{
    /*
    mVal = USER_MODEL_TYPE;
        0 : CHP
        1 : CP
    mVal = USER_FACTORY_STATUS;
        0 CLEAR : Normal
        1 SET : Factory/PCB Test mode
    mVal = USER_RX_STATUS;
        0 FALSE : App Control X (Operating...)
        1 TRUE : App Control O
    mVal = USER_ERROR_STATUS;
        0 FALSE : Normal
        1 TRUE : Error
    */
    U8 mu8Return = 0U;
    
    if ( mu8Value == USER_MODEL_TYPE )
    {
    	if (gu8_ModelType_forCHP == MODEL_CHP)
    	{
    		mu8Return = USER_MODEL_CHP;
    	}
    	else
    	{
    		mu8Return = USER_MODEL_CP;
    	}
    }
    else if ( mu8Value == USER_FACTORY_STATUS ) 
    { /* 공장테스트모드 확인 */
        if (( F_TestMode_LineFront == SET ) || (F_TestMode_PBA == SET ))
        {
            mu8Return = SET; // 공장모드
        }
        else
        {
            mu8Return = CLEAR; // 일반모드
        }
    }
    else if ( mu8Value == USER_RX_STATUS )
    { /* 추출중 or 기타설정 중인경우 WIFI 제어 동작 금지 (제품 설정중에 앱으로 설정 시 오작동) */
//        if ((gu8_OperationStep_hi == 0x50) || (gu8_OperationStep_hi == 0x00) || (gu8_OperationStep_hi == 0xB0))
//        {
//            mu8Return = FALSE; // 어플동작금지
//        }
//        else
//        {
//            mu8Return = TRUE; // 어플동작가능
//        }
//        if ( (gu8_OperationStep_hi == 0x10) || (gu8_OperationStep_hi == 0x20))
//        {
//            mu8Return = TRUE; // 어플동작가능
//        }
//        else
//        {
//            mu8Return = FALSE; // 어플동작금지
//        }
        // 플러싱 / 추출 / EW살균 / 커버,필터 오픈
        if ((gu8_OperationStep == Z_OperationStep_03) || (gu8_OperationStep == Z_OperationStep_04) || (gu8_OperationStep == Z_OperationStep_05)
        	|| (gu8_OperationStep == Z_OperationStep_50) || (gu8_OperationStep == Z_OperationStep_80) || (gu8_OperationStep == Z_OperationStep_B0))
        {
        	mu8Return = FALSE; // 어플동작금지
        }
        else
        {
        	mu8Return = TRUE; // 어플동작가능
        }
    }
    else if ( mu8Value == USER_SMART_POSSIBLE_STATUS )
    {
        // 추출 / 플러싱 / EW살균 
        if ((gu8_OperationStep == Z_OperationStep_03) || (gu8_OperationStep == Z_OperationStep_04) || (gu8_OperationStep == Z_OperationStep_05)
        	|| (gu8_OperationStep == Z_OperationStep_50))
		{
			mu8Return = FALSE;    // 정밀진단 불가
		}
		else
		{
			mu8Return = TRUE;    // 정밀진단 가능
		}
    }
    else if ( mu8Value == USER_ERROR_STATUS )
    { /* 제품 에러확인 */
        if (gu8_ErrorCode != 0)
        {
            mu8Return = TRUE;   // 에러
        }
        else
        {
            mu8Return = FALSE;  // 정상
        }
    }
    else if ( mu8Value == USER_FOTA_POSSIBLE_STATUS )
    {
    	if ((gu8_OperationStep == Z_OperationStep_03) || (gu8_OperationStep == Z_OperationStep_04) || (gu8_OperationStep == Z_OperationStep_05)
    		|| (gu8_OperationStep == Z_OperationStep_50))
        {
            mu8Return = FALSE; // Fota 불가 (제품동작중)
        }
        else
        {
            mu8Return = TRUE; // Fota 가능
        }
    }
    else if ( mu8Value == USER_SMART_SENSING_STATUS )
    {
        if ( gu8_OperationStep_hi == 0x80 )
        {
            mu8Return = TRUE; // 정밀진단 중
        }
        else
        {
            mu8Return = FALSE;
        }
    }
    else
    {
        mu8Return = 0U;
    }
    return mu8Return;
}

/* User Data Clear */
void ClearUserSystemData ( void ) 
{
	gu32_Watt_per_Min_Result = 0;					//	서버 연결안되었을 경우 데이터 클리어
}

void SetUserSystemApplyData ( U16 mId )
{

}


/* User Display Setting */
void SetUserSystemDisplay ( U8 mu8Display )
{
    if ( mu8Display == USER_DISPLAY_FOTA ) // FOTA
    {
    	F_FOTA_Display = SET;
    	F_FOTA_Setup = SET;
    }
}

/* Time Setting */
void SetUserSystemTime ( U16 mYear, U16 mMonth, U16 mDate, U16 mHour, U16 mMin, U16 mSec, U16 mDay )
{

}


/* User Error Data */
U16 GetUserSystemError ( U16 mu16Err )
{
    U16 mu16Data = 0U;
  
    switch ( mu16Err ) // 0:Normal / 1:Error
    {
        case WIFI_ERROR_0006_HOTIN_TEMP:
        	mu16Data = F_Err46_HotInTH;
        	break;

        case WIFI_ERROR_0007_HOTOUT_TEMP:
        	mu16Data = F_Err47_HotOutTH;
        	break;

        case WIFI_ERROR_0008_COLD_TEMP :
        	mu16Data = F_Err44_ColdTH;
        break;

        case WIFI_ERROR_0009_NATURAL_TEMP :
        	mu16Data = F_Err43_OutDoorAirTH;
        break;

        case WIFI_ERROR_0018_TEM_TEMP:
        	mu16Data = F_Err25_HeatSinkTH;
        	break;

        case WIFI_ERROR_0019_STOP_WATER :
        	mu16Data = F_Err09_CutWater;
        	break;

        case WIFI_ERROR_001A_TEM_OPERATION:
        	if ((F_Err29_TEM_OverDrive1 == SET) || (F_Err29_TEM_OverDrive2 == SET))
        	{
        		mu16Data = SET;
        	}
        	else
        	{
        		mu16Data = CLEAR;
        	}
        	break;

        case WIFI_ERROR_001B_STOP_HOTWATER:
        	mu16Data = F_Err08_HotCutWater;
        	break;

        case WIFI_ERROR_001C_LEAK_FLOW :
//@026        	mu16Data = F_Err02_LeakWater;
        	break;

        case WIFI_ERROR_001D_LEAK_SENSOR :
        	mu16Data = F_Err01_LeakWater;
        	break;

        case WIFI_ERROR_001E_HOT_OVER:
        	mu16Data = F_Err40_OverHeat;
        	break;

        case WIFI_ERROR_001F_HEATER_SENSOR:
        	mu16Data = F_Err48_HeaterTH;
        	break;

        default:
            mu16Data = 0U;
        break;
    }
    return mu16Data;
}

/* User Function Data */
U16 GetUserSystemFunction ( U16 mu16Func )
{
    U16 mu16Data = 0U;
    U16 buf = 0;

    switch ( mu16Func )
    {        
        case WIFI_FUNC_0002_COLD_SEL : // 0 / 1
            if (F_ColdOff == CLEAR)
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
        break;

        case WIFI_FUNC_0003_HOT_LOCK_SEL : // 1 / 2(Lock)
            if (F_HotLock == SET)
            {
            	mu16Data = 2;
            }
            else
            {
                mu16Data  = 1;
            }
        break;
        
        case WIFI_FUNC_0005_LOCK_SEL:
        	mu16Data = F_AllLock;
        break;

        case WIFI_FUNC_000A_COLD_TEMP : // 0 ~ 3 / 65535(ERR)
			if (F_ColdOff == SET)
			{
				mu16Data = 0;
			}
			else
			{
				if (F_ColdTemp_Display == 0)		//	WHITE
				{
					mu16Data = 1;
				}
				else
				{
					mu16Data = 2;
				}
			}
        break;
        
        case WIFI_FUNC_000B_WATER_SEL : // 1(A) / 2(C) / 3(H)
        	switch (gu8_WaterSelect)
        	{
        		case PURE_WATER:
        		default:
        			mu16Data = 1;
        			break;
        		case COLD_WATER:
        			mu16Data = 2;
        			break;
        		case HOT_WATER:
        			mu16Data = 3;
        		    break;
        	}
        break;

        case WIFI_FUNC_000D_WATER_OUTQUANTITY : // 0 ~ 65535 cc
//        	mu16Data = gu16_Effluent_Hz_Amount_forWIFI;
			mu16Data = (U16)((U32)gu16_Effluent_Hz_Amount_forWIFI * gu16_FlowMeter_Standard / 10000);
			mu16Data /= 10;							//	1의 자리 제거
			mu16Data *= 10;							//	1의 자리 제거
			gu16_Effluent_Hz_Amount_forWIFI = 0;	//
        break;

        case WIFI_FUNC_001A_HOT_INTEMP:
        	if (gu8_WaterSelect == HOT_WATER)
        	{
            	if (gi16_HotInSensor.DEGREE >= 0)
            	{
    				buf = (U16)(gi16_HotInSensor.DEGREE / 10);
    				if (buf > 99)
    				{
    					buf = 99;
    				}
    				mu16Data = buf;
            	}
            	else
            	{
            		mu16Data = 0;
            	}
        	}
        	else
        	{
        		mu16Data = 0;
        	}
        	break;

        case WIFI_FUNC_001B_HOT_MEANTEMP:
        	if (gu8_WaterSelect == HOT_WATER)
        	{
        		mu16Data = gu16_HotOut_Degree_Ave/10;
        	}
        	else
        	{
        		mu16Data = 0;
        	}
        	break;
		 //-------------------------------------------20260201--------------------------
/*
        case WIFI_FUNC_001C_HOT_SELTEMP:
        	if (gu8_WaterSelect == HOT_WATER)
        	{
        		switch (gu8_HotTempSetup)
        		{
#ifdef _HOT_TEMP_4STEP_
        			case HOT_TEMP_SETUP_KOR100_JP90:
        			default:
        				mu16Data = 100;
        				break;
        			case HOT_TEMP_SETUP_KOR85_JP80:
        				mu16Data = 85;
        				break;
        			case HOT_TEMP_SETUP_KOR70_JP70:
        				mu16Data = 70;
        				break;
					case HOT_TEMP_SETUP_KOR45_JPX:
						mu16Data = 45;
						break;
#else
        			case HOT_TEMP_SETUP_90:
        				mu16Data = 90;
        				break;
        			case HOT_TEMP_SETUP_80:
        			default:
        				mu16Data = 80;
        				break;
        			case HOT_TEMP_SETUP_KOR70_JP70:
        				mu16Data = 70;
        				break;
					case HOT_TEMP_SETUP_KOR45_JPX:
						mu16Data = 45;
						break;
#endif
        		}
        	}
        	else
        	{
        		mu16Data = 0;
        	}
        	break;
*/
	case WIFI_FUNC_001C_HOT_SELTEMP:
        	if ((gu8_WaterSelect == HOT_WATER) && (gu8_ModelType_forCHP == MODEL_CHP))
        	{
        		switch (gu8_HotTempSetup)
        		{
                    		case 12:
                        		mu16Data = 100;
                   		 	break;
        			case 11:
        				mu16Data = 95;
        				break;

        			case 10:
        				mu16Data = 90;
        				break;

        			case 9:
        				mu16Data = 85;
        				break;

				case 8:
					mu16Data = 80;
					break;

				case 7:
					mu16Data = 75;
					break;

				case 6:
					mu16Data = 70;
					break;

				case 5:
					mu16Data = 65;
					break;

				case 4:
					mu16Data = 60;
					break;

				case 3:
					mu16Data = 55;
					break;

				case 2:
					mu16Data = 50;
					break;

				case 1:
					mu16Data = 45;
					break;

		                default:
		                        mu16Data = 45;
		                        break;
        		}
        	}
        	else
        	{
        		mu16Data = 0;
        	}        
        break;        	

        case WIFI_FUNC_0024_HEART_TIME4:
        	mu16Data = gu16_HeartServiceTime;
        	if (gu8_OperationStep == Z_OperationStep_10)
        	{
        		gu16_HeartServiceTime = 0;
        	}
        	break;

        case WIFI_FUNC_002C_SILVER_CARE : // 0 / 1
        	if (F_SilverCare_Notice == SET)
        	{
        		mu16Data = 1;
        		F_SilverCare_Notice = CLEAR;
        	}
        	else if (gu16_TimerSilverCare == 0)
        	{
        		mu16Data = 2;
        	}
        	else
        	{
        		mu16Data = 0;
        	}
        break;

        case WIFI_FUNC_0031_VOICE_SEL:
        	mu16Data = gu8_Sound_Type;
        	break;

        case WIFI_FUNC_0033_VOICE_VOL_SEL:
        	mu16Data = gu8_VoiceVolumeLevel+1;
        	break;

        case WIFI_FUNC_0035_COVER1_OPEN:
			if (INPUT_REED_SW_COVER == SET)
			{
				mu16Data = 0;
			}
			else
			{
				mu16Data = 1;
			}
			break;

        case WIFI_FUNC_0037_SLEEP_SEL:
        	mu16Data = F_SleepMode;
        	break;
        
        case WIFI_FUNC_003B_HOT_TEMP_SEL : // 0 / 1
        	if (gu8_ModelType_forCHP == MODEL_CHP)
        	{
        		mu16Data = F_HotWater_100Degree;
        	}
            break;
            
        case WIFI_FUNC_003E_WATER_COURSE_STATUS:
			if (INPUT_REED_SW_FAUCET == CLEAR)
			{
				mu16Data = 1;
			}
			else
			{
				mu16Data = 0;
			}
        	break;

        case WIFI_FUNC_003F_WATER_COURSE_RESET:
			if (F_Faucet_Reset == SET)
			{
				mu16Data = 1;
				F_Faucet_Reset = CLEAR;
			}
        	break;
   
        case WIFI_FUNC_0040_SMART_CHECK_RQST : // 0 / 1(정밀진단 완료되었을때)
        	if ((gu8_SmartCheckStep & 0xF0) == 0x20)
        	{
        		mu16Data = 1;
        	}
        	else if (F_SmartCheck_Complete_2 == SET)
        	{
        		mu16Data = 1;
        		F_SmartCheck_Complete_2 = CLEAR;
        	}
        	else
        	{
        		mu16Data = 0;
        	}
        break;
        
        case WIFI_FUNC_0041_SMART_CHECK_STATUS : // 0(일반) / 1(시작) / 2(진행중) / 3(완료) / 4(중지) / 5(불가)
			if (F_SmartCheck_Start == SET)
			{
				mu16Data = 1;
				F_SmartCheck_Start = CLEAR;
			}
			else if ((gu8_SmartCheckStep & 0xF0) == 0x20)
			{
				mu16Data = 2;
			}
			else if (F_SmartCheck_Complete == SET)
			{
				mu16Data = 3;
				F_SmartCheck_Complete = CLEAR;
			}
			else if (F_SmartCheck_Stop == SET)
			{
				mu16Data = 4;
				F_SmartCheck_Stop = CLEAR;
			}
			else if ((F_SmartCheck_Prohibit == SET) || (gu8_OperationStep == Z_OperationStep_03) || (gu8_OperationStep == Z_OperationStep_04) || (gu8_OperationStep == Z_OperationStep_05)
		        	|| (gu8_OperationStep == Z_OperationStep_50) || (gu8_OperationStep_hi == 0x50))
			{
				mu16Data = 5;
				F_SmartCheck_Prohibit = CLEAR;
			}
			else
			{
				mu16Data = 0;
			}
        break;
        
        case WIFI_FUNC_0042_SMART_CHECK_PROGRESS : // 0 ~ 100 %
        	mu16Data = (U16)gu8_SmartCheck_Percent;
        	gu8_SmartCheck_Percent = 0;
        break;

        case WIFI_FUNC_0043_FILTER_STATUS1 : // 0 / 1(분리)
        	if (INPUT_REED_SW_FILTER == CLEAR)
        	{
        		mu16Data = 1;
        	}
        	else
        	{
        		mu16Data = 0;
        	}
        break;
        
        case WIFI_FUNC_0047_WATER_QUANTITY_SEL:
        	mu16Data = gu16_CupSize_My;
        	break;

        case WIFI_FUNC_0049_OUT_BUTTON_LIGHT:
        	mu16Data = !F_EffluentLED_OFF;
        	break;

        case WIFI_FUNC_0050_UV_STERILIZE_STATUS:
        	if (F_UVCare_Start_forWIFI == SET)
        	{
        		F_UVCare_Start_forWIFI = CLEAR;
        		mu16Data = 1;
        	}
        	else if (gu8_OperationStep_hi == 0x30)
        	{
        		mu16Data = 2;
                WifiSendData(WIFI_DATA_FUNCTION);
        	}
        	else
        	{
        		mu16Data = 0;
        	}
        	break;

        case WIFI_FUNC_0051_QUANTITY_USE_SEL:
        	mu16Data = F_CupSize_MySet;
        	break;

        case WIFI_FUNC_0053_FILTER_RESET1 :
			if (F_Filter_Reset == SET)
			{
				mu16Data = 1;
				F_Filter_Reset = CLEAR;
			}
        break;

        case WIFI_FUNC_005C_SMART_CHECK_RESULT :   //JHKO 
            // ?????? ?????? ?????? ?????? ??? ???????????? ??????(0)??? ????????? ?????? ?????? ??????(1) ??????
            // (?????? ???????????? ????????? ??? SET(1)??? ?????? ????????? ??????)
            if (gu16_SmartCheck_Result.Word != 0x1FFF) // ?????? ????????? ??????(1)??? ?????????
            {
                mu16Data = 1; // ?????? ??????
            }
            else 
            {
                mu16Data = 0; // ??????
            }
            break;
    //-------------------------------------------20260223--------------------------JHKO
        case WIFI_FUNC_00B5_FILTER_FLUSHING :

        	if(gu8_FlushingProgress_forWifi == Z_FLUSHING_PROGRESS_ON_FOR_WIFI)
        	{
        		mu16Data = (U16)(gu8_FlushingProgress_forWifi);
        		gu8_FlushingProgress_forWifi = Z_FLUSHING_PROGRESS_GOING_FOR_WIFI;
        	}
			else if(gu8_FlushingProgress_forWifi == Z_FLUSHING_PROGRESS_GOING_FOR_WIFI)
			{
				mu16Data = (U16)(gu8_FlushingProgress_forWifi);
			}
        	else if(gu8_FlushingProgress_forWifi == Z_FLUSHING_PROGRESS_DONE_FOR_WIFI)
        	{
        		mu16Data = (U16)(gu8_FlushingProgress_forWifi);
        		gu8_FlushingProgress_forWifi = 0;
        	}
        	else
        	{
        		mu16Data = 0;
        	}

            break;


        case WIFI_FUNC_00CB_WATER_EXTRACT_STATUS :
                if(gu8_OperationStep == Z_OperationStep_50)
                {
                    mu16Data = 1U;
                }
                else
                {
                    mu16Data = 0U;
                }
               // }
            break;

	 //-------------------------------------------20260201--------------------------   
	case WIFI_FUNC_0073_WATER_TEMP_SORT_SEL:
            if(gu8_f_hot_temp_sorting == Z_HOT_TEMP_ASCENDING)
            {
                mu16Data = 0;
            }
            else
            {
                mu16Data = 1;
            }
            break;
	 
	case WIFI_FUNC_007F_DEFAULT_TEMP_SET:

            if(selected_hot_temp[gu8_DefaultHotTempSetup] == 1)
            {
                mu16Data = 45;
            }
            else if(selected_hot_temp[gu8_DefaultHotTempSetup] == 2)
            {
                mu16Data = 50;
            }
            else if(selected_hot_temp[gu8_DefaultHotTempSetup] == 3)
            {
                mu16Data = 55;
            }
            else if(selected_hot_temp[gu8_DefaultHotTempSetup] == 4)
            {
                mu16Data = 60;
            }
            else if(selected_hot_temp[gu8_DefaultHotTempSetup] == 5)
            {
                mu16Data = 65;
            }
            else if(selected_hot_temp[gu8_DefaultHotTempSetup] == 6)
            {
                mu16Data = 70;
            }
            else if(selected_hot_temp[gu8_DefaultHotTempSetup] == 7)
            {
                mu16Data = 75;
            }
            else if(selected_hot_temp[gu8_DefaultHotTempSetup] == 8)
            {
                mu16Data = 80;
            }
            else if(selected_hot_temp[gu8_DefaultHotTempSetup] == 9)
            {
                mu16Data = 85;
            }
            else if(selected_hot_temp[gu8_DefaultHotTempSetup] == 10)
            {
                mu16Data = 90;
            }
            else if(selected_hot_temp[gu8_DefaultHotTempSetup] == 11)
            {
                mu16Data = 95;
            }
            else if(selected_hot_temp[gu8_DefaultHotTempSetup] == 12)
            {
                mu16Data = 100;
            }
            else
            {
                mu16Data = 45;
            }

        break;
	
	case WIFI_FUNC_00C1_HOTTEMP_USE_SEL0:

            if(gu8_hot_temp_data_for_wifi.Bit.b0 == SET)
            {
                mu16Data = 1U;
            }
            else
            {
                mu16Data = 0U;
            }

        break;

        case WIFI_FUNC_00C2_HOTTEMP_USE_SEL1:

            if(gu8_hot_temp_data_for_wifi.Bit.b1 == SET)
            {
                mu16Data = 1U;
            }
            else
            {
                mu16Data = 0U;
            }


        break;

        case WIFI_FUNC_00C3_HOTTEMP_USE_SEL2:

            if(gu8_hot_temp_data_for_wifi.Bit.b2 == SET)
            {
                mu16Data = 1U;
            }
            else
            {
                mu16Data = 0U;
            }


        break;

        case WIFI_FUNC_00C4_HOTTEMP_USE_SEL3:

            if(gu8_hot_temp_data_for_wifi.Bit.b3 == SET)
            {
                mu16Data = 1U;
            }
            else
            {
                mu16Data = 0U;
            }

        break;

        case WIFI_FUNC_00C5_HOTTEMP_USE_SEL4:

            if(gu8_hot_temp_data_for_wifi.Bit.b4 == SET)
            {
                mu16Data = 1U;
            }
            else
            {
                mu16Data = 0U;
            }

        break;

        case WIFI_FUNC_00C6_HOTTEMP_USE_SEL5:

            if(gu8_hot_temp_data_for_wifi.Bit.b5 == SET)
            {
                mu16Data = 1U;
            }
            else
            {
                mu16Data = 0U;
            }

        break;        
        case WIFI_FUNC_00C7_HOTTEMP_USE_SEL6:

            if(gu8_hot_temp_data_for_wifi.Bit.b6 == SET)
            {
                mu16Data = 1U;
            }
            else
            {
                mu16Data = 0U;
            }

        break;

        case WIFI_FUNC_00C8_HOTTEMP_USE_SEL7:
            if(gu8_hot_temp_data_for_wifi.Bit.b7 == SET)
            {
                mu16Data = 1U;
            }
            else
            {
                mu16Data = 0U;
            }
        break;

        case WIFI_FUNC_00C9_HOTTEMP_USE_SEL8:
            if(gu8_hot_temp_data_for_wifi.Bit.b8 == SET)
            {
                mu16Data = 1U;
            }
            else
            {
                mu16Data = 0U;
            }

        break;

        case WIFI_FUNC_00CA_HOTTEMP_USE_SEL9:

            if(gu8_hot_temp_data_for_wifi.Bit.b9 == SET)
            {
                mu16Data = 1U;
            }
            else
            {
                mu16Data = 0U;
            }

        break;

        case WIFI_FUNC_00CE_HOTTEMP_USE_SEL10:
            if(gu8_hot_temp_data_for_wifi.Bit.b10 == SET)
            {
                mu16Data = 1U;
            }
            else
            {
                mu16Data = 0U;
            }

        break;

        case WIFI_FUNC_00CF_HOTTEMP_USE_SEL11:
            if(gu8_hot_temp_data_for_wifi.Bit.b11 == SET)
            {
                mu16Data = 1U;
            }
            else
            {
                mu16Data = 0U;
            }            

        break;

        case WIFI_FUNC_011E_HOT_TEMP_MIN_COUNT:
            mu16Data = (U16)1;
        break;

        case WIFI_FUNC_0120_HOT_TEMP_MAX_COUNT:
            mu16Data = (U16)5;
        break;
	
	 case WIFI_FUNC_0129_AMOUNT_SLOT1_ENABLE:
            if(gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_ONE].enable == 1)
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
        break;

        case WIFI_FUNC_012A_AMOUNT_SLOT2_ENABLE:
            if(gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_TWO].enable == 1)
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
        break;

        case WIFI_FUNC_012B_AMOUNT_SLOT3_ENABLE:
            if(gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_THREE].enable == 1)
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
        break;
        /*
        case WIFI_FUNC_012C_AMOUNT_SLOT4_ENABLE:
            if(gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_FOUR].enable == 1)
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }
        break;
        */
        case WIFI_FUNC_0132_START_WATER_QUANTITY_SLOT:
        mu16Data = (U16)(gu8_DefaultCupSize + 1);
        break;

        case WIFI_FUNC_0133_START_WATER_QUANTITY_SEL_1:
        
            mu16Data = ((U16)gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_ONE].amount_volume) * 10;

        break;

        case WIFI_FUNC_0134_START_WATER_QUANTITY_SEL_2:
        
            mu16Data = ((U16)gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_TWO].amount_volume) * 10;

        break;

        case WIFI_FUNC_0135_START_WATER_QUANTITY_SEL_3:
        
            mu16Data = ((U16)gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_THREE].amount_volume) * 10;

        break;
        /*
        case WIFI_FUNC_0136_START_WATER_QUANTITY_SEL_4:
        
            mu16Data = ((U16)gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_FOUR].amount_volume) * 10;

        break;
	*/
        case WIFI_FUNC_013C_START_WATER_QUANTITY_SEL_10:

            if(gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_CONTINUE].enable != 0)
            {
                mu16Data = 1;
            }
            else
            {
                mu16Data = 0;
            }

        break;

        default:
            mu16Data = 0U;
        break;
    }
    return mu16Data;
}

/* User Sensor Data */
I16 GetUserSystemSensor ( U16 mu16Sen )
{
//	I16 buf = 0;
    I16 mi16Data = 0;

    switch ( mu16Sen )
    {
        case WIFI_SENSOR_0001_COLD_TEMP : //  -127 ~ 127
        	mi16Data = gi16_ColdSensor.DEGREE / 10;
			if (mi16Data > 127)
			{
				mi16Data = 127;
			}
			else if (mi16Data < -127)
			{
				mi16Data = -127;
			}
			else
			{

			}
        break;

        case WIFI_SENSOR_0004_OUT_TEMP : //  -127 ~ 127
        	mi16Data = gi16_OutDoorAirSensor.DEGREE / 10;
			if (mi16Data > 127)
			{
				mi16Data = 127;
			}
			else if (mi16Data < -127)
			{
				mi16Data = -127;
			}
			else
			{

			}
        break;

        case WIFI_SENSOR_000E_WATT_DATA : // 0.01~250.00 (ex -> 1.78 = 178)
        	mi16Data = (U16)gu32_Watt_per_Min_Result;				//	[1/100Wh] (※ 100배수로 올려야 1임. 100 → 1Wh)
        	gu32_Watt_per_Min_Result = 0;
            if ( mi16Data >= 3000U )
            {
                mi16Data = 3000U;
            }
        break;

        case WIFI_SENSOR_0010_TEM_TEMP:	//  -127 ~ 127
        	mi16Data = gi16_HeatSinkSensor.DEGREE / 10;
			if (mi16Data > 127)
			{
				mi16Data = 127;
			}
			else if (mi16Data < -127)
			{
				mi16Data = -127;
			}
			else
			{

			}
        	break;
        default:
          mi16Data = 0;
        break;
    }
    return mi16Data;
}

/* User Part Data */
U16 GetUserSystemPart ( U16 mu16Part )
{
    U16 mu16Data = 9999U;
    U8 mu8_OPENSHORT_Error = 0;
    switch ( mu16Part )
    {
    	case WIFI_PART_0001_VALVE_NOS:
    		mu16Data = gu16_SmartCheck_Result_1stNos_VV_mA;
    		gu16_SmartCheck_Result_1stNos_VV_mA = 0xFFFFU;
    		break;
    	case WIFI_PART_0002_VALVE_HOT_OUT:
    		if (gu8_ModelType_forCHP == MODEL_CHP)
    		{
    			mu16Data = gu16_SmartCheck_Result_HotOut_VV_mA;
    		}
    		else
    		{
    			mu16Data = 0xFFFFU;
    		}
    		gu16_SmartCheck_Result_HotOut_VV_mA = 0xFFFFU;
    		break;
    	case WIFI_PART_0003_VALVE_PURE_OUT:
    		mu16Data = gu16_SmartCheck_Result_Pure_VV_mA;
    		gu16_SmartCheck_Result_Pure_VV_mA = 0xFFFFU;
    		break;
    	case WIFI_PART_0004_VALVE_COLD_OUT:
    		mu16Data = gu16_SmartCheck_Result_ColdOut_VV_mA;
    		gu16_SmartCheck_Result_ColdOut_VV_mA = 0xFFFFU;
    		break;
    	case WIFI_PART_0009_VALVE_HOT_IN:
    		if (gu8_ModelType_forCHP == MODEL_CHP)
    		{
    			mu16Data = gu16_SmartCheck_Result_HotIn_VV_mA;
    		}
    		else
    		{
    			mu16Data = 0xFFFFU;
    		}
    		gu16_SmartCheck_Result_HotIn_VV_mA = 0xFFFFU;
    		break;
    	case WIFI_PART_000C_VALVE_COLD_IN:
    		mu16Data = gu16_SmartCheck_Result_ColdIn_VV_mA;
    		gu16_SmartCheck_Result_ColdIn_VV_mA = 0xFFFFU;
    		break;
    	case WIFI_PART_0202_COLD_TEM:
    		mu16Data = gu16_SmartCheck_Result_TEM_mA;
    		gu16_SmartCheck_Result_TEM_mA = 0xFFFFU;
    		break;
    	case WIFI_PART_0203_COLD_TEM_TEMP:
    		mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_HeatSinkSensor.TH_ADC_AVERAGE);     //	OPEN SHORT 체크
    		if (mu8_OPENSHORT_Error != 0)
    		{
    			mu16Data = 0x00ffU;
    		}
    		else if (gi16_HeatSinkSensor.DEGREE >= 0)
    		{
    			mu16Data = gi16_HeatSinkSensor.DEGREE / 10;
    		}
    		else
    		{
    			mu16Data = 0;
    		}
    		break;
    	case WIFI_PART_0204_COLD_FAN:
    		mu16Data = gu16_SmartCheck_Result_Fan_mA;
    		gu16_SmartCheck_Result_Fan_mA = 0xFFFFU;   
    		break;
    	case WIFI_PART_0206_COLD_TEMP_1:
    		mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_ColdSensor.TH_ADC_AVERAGE);     //	OPEN SHORT 체크
    		if (mu8_OPENSHORT_Error != 0)
    		{
    			mu16Data = 0x00ffU;
    		}
    		else if (gi16_ColdSensor.DEGREE >= 0)
    		{
    			mu16Data = gi16_ColdSensor.DEGREE / 10;
    		}
    		else
    		{
    			mu16Data = 0;
    		}
    		break;
    	case WIFI_PART_0208_COLD_ROOM_TEMP:
    		mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_OutDoorAirSensor.TH_ADC_AVERAGE);     //	OPEN SHORT 체크
    		if (mu8_OPENSHORT_Error != 0)
    		{
    			mu16Data = 0x00ffU;
    		}
    		else if (gi16_OutDoorAirSensor.DEGREE >= 0)
    		{
    			mu16Data = gi16_OutDoorAirSensor.DEGREE / 10;
    		}
    		else
    		{
    			mu16Data = 0;
    		}
    		break;
    	case WIFI_PART_0302_HOT_INSTANT_HEATER_1:
    		mu16Data = gu16_SmartCheck_Result_HeaterRelay_ADC;
    		gu16_SmartCheck_Result_HeaterRelay_ADC = 0xFFFFU;
    		break;
    	case WIFI_PART_0303_HOT_INSTANT_HEATER_2:
    		mu16Data = gu16_SmartCheck_Result_HeaterTriac_ADC;
    		gu16_SmartCheck_Result_HeaterTriac_ADC = 0xFFFFU;
    	    break;
    	case WIFI_PART_0304_HOT_FLOW_MOTOR:
    		mu16Data = gu16_SmartCheck_Result_SteppingMotor_mA;
    		gu16_SmartCheck_Result_SteppingMotor_mA = 0xFFFFU;
    		break;
    	case WIFI_PART_0305_HOT_TEMP_IN:
    		mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_HotInSensor.TH_ADC_AVERAGE);     //	OPEN SHORT 체크
    		if (mu8_OPENSHORT_Error != 0)
    		{
    			mu16Data = 0x00ffU;
    		}
    		else if (gi16_HotInSensor.DEGREE >= 0)
    		{
    			mu16Data = gi16_HotInSensor.DEGREE / 10;
    		}
    		else
    		{
    			mu16Data = 0;
    		}
    		break;
    	case WIFI_PART_0306_HOT_TEMP_OUT:
    		mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_HotOutSensor.TH_ADC_AVERAGE);     //	OPEN SHORT 체크
    		if (mu8_OPENSHORT_Error != 0)
    		{
    			mu16Data = 0x00ffU;
    		}
    		else if (gi16_HotOutSensor.DEGREE >= 0)
    		{
    			mu16Data = gi16_HotOutSensor.DEGREE / 10;
    		}
    		else
    		{
    			mu16Data = 0;
    		}
    		break;
    	case WIFI_PART_0307_HOT_HEATER_TEMP:
    		mu8_OPENSHORT_Error = TH_Open_Short_Check(gi16_HeaterSensor.TH_ADC_AVERAGE);     //	OPEN SHORT 체크
    		if (mu8_OPENSHORT_Error != 0)
    		{
    			mu16Data = 0x00ffU;
    		}
    		else if (gi16_HeaterSensor.DEGREE >= 0)
    		{
    			mu16Data = gi16_HeaterSensor.DEGREE / 10;
    		}
    		else
    		{
    			mu16Data = 0;
    		}
    		break;
    	case WIFI_PART_0402_SENSOR_LEAK:
    		if (ADC_RESULT_LEAKSENSOR > Z_LeakSensor_Error)	//	누수감지 시
    		{
    			mu16Data = 1;
    		}
    		else
    		{
    			mu16Data = 0;
    		}
    		break;
    	case WIFI_PART_0403_SENSOR_FLOW:
    		mu16Data = gu16_SmartCheck_Result_FlowMeter;
    		gu16_SmartCheck_Result_FlowMeter = 0xFFFFU;
    		break;
    	case WIFI_PART_0405_SENSOR_FILTER_REED:
    		mu16Data = !INPUT_REED_SW_COVER;
    		break;
    	case WIFI_PART_0406_SENSOR_FILTER_SW_1:
    		mu16Data = !INPUT_REED_SW_FILTER;
    		break;
    	case WIFI_PART_040B_SENSOR_WATER_COURSE_REED:
    		mu16Data = !INPUT_REED_SW_FAUCET;
    		break;
    	case WIFI_PART_0501_STER_UV_FAUCET:
    		mu16Data = gu16_SmartCheck_Result_UV_mA;
    		gu16_SmartCheck_Result_UV_mA = 0xFFFFU;
    		break;
        default:
        	mu16Data = 0xFFFF;
        	break;
    }
//    gu16_SmartCheck_Result_Comp_mA = 0;
//    gu16_SmartCheck_Result_Heater_mA = 0;
//    gu16_SmartCheck_Result_Pure_VV_mA = 0;
//    gu16_SmartCheck_Result_HotIn_VV_mA = 0;
//    gu16_SmartCheck_Result_HotOut_VV_mA = 0;
//    gu16_SmartCheck_Result_HotAir_VV_mA = 0;
//    gu16_SmartCheck_Result_Cold_VV_mA = 0;
//    gu16_SmartCheck_Result_ColdAir_VV_mA = 0;
//    gu16_SmartCheck_Result_EXC_VV_mA = 0;
//    gu16_SmartCheck_Result_Inocense_VV_mA = 0;
//    gu16_SmartCheck_Result_1stNos_VV_mA = 0;
//    gu16_SmartCheck_Result_Fan_mA = 0;
//    gu16_SmartCheck_Result_Pump_mA = 0;
//    gu16_SmartCheck_Result_FlowMeter = 0;

    return mu16Data;
}

U32 GetUserSystemAccumulate ( U16 mu16Acc )
{
    U32 mu32Data = 0U;

    return mu32Data;
}

/* App Control -> User Function Data */
void SetUserSystemFunction ( U16 mu16Func, U16 mData )
{
    switch ( mu16Func )
    {
        case WIFI_FUNC_0002_COLD_SEL :
            if (mData == ON) // 1 : 냉수ON
            {
                F_ColdOff = CLEAR;
        		Ini_OperationStep_15();
        		Play_Voice(VOICE_COLD_ON_SETTING);
            }
            else // 0 : OFF
            {
            	F_ColdOff = SET;
        		Ini_OperationStep_18();
        		gu8_WaterSelect = PURE_WATER;
        		gu8_DefaultWaterSelect = PURE_WATER;
        		Play_Voice(VOICE_COLD_OFF_SETTING);
            }
        break;

        case WIFI_FUNC_0003_HOT_LOCK_SEL : // 1 / 2(Lock)
            if (mData == 2)
            {
                F_HotLock = SET;
                Play_Voice(VOICE_HOT_LOCK_SETTING);
            }
            else
            {
                F_HotLock = CLEAR;
                Play_Voice(VOICE_HOT_LOCK_CLEAR);
            }
        break;

        case WIFI_FUNC_0005_LOCK_SEL:
            if (mData == 1)
            {
                F_AllLock = SET;
        		Play_Voice(VOICE_ALL_LOCK_SETTING_A);
            }
            else
            {
            	F_AllLock = CLEAR;
                Play_Voice(VOICE_ALL_LOCK_CLEAR);
            }
        	break;

        case WIFI_FUNC_0031_VOICE_SEL:
        	switch (mData)
        	{
        		case 1:
        			gu8_Sound_Type = (U8)mData;
        			Play_Voice(VOICE_SETTING_SILENT);
        			break;
        		case 2:
        			gu8_Sound_Type = (U8)mData;
        			Play_Voice(VOICE_SETTING_MELODY);
        			break;
        		case 3:
        			gu8_Sound_Type = (U8)mData;
        			Play_Voice(VOICE_SETTING_VOICE);
        			break;
        		default:
        			break;
        	}
        	break;

        case WIFI_FUNC_0033_VOICE_VOL_SEL:
        	if ((mData >= 1) && (mData <= 5))
        	{
        		gu8_VoiceVolumeLevel = (U8)(mData-1);
        		Play_Voice(VOICE_BEEP_SELECT);
        	}
        	break;

        case WIFI_FUNC_0037_SLEEP_SEL:
        	if (mData <= 1)
        	{
				F_SleepMode = (U8)mData;
				F_SleepMode_Display = SET;
				Ini_OperationStep_17();
				if (F_SleepMode == SET)
				{
					Play_Voice(VOICE_BEEP_ON);
				}
				else
				{
					Play_Voice(VOICE_BEEP_OFF);
				}
        	}
        	break;

        case WIFI_FUNC_003B_HOT_TEMP_SEL : // 0 / 1
        	if (gu8_ModelType_forCHP == MODEL_CHP)
        	{
				F_HotWater_100Degree = (U8)mData;
				if (F_HotWater_100Degree == SET)
				{
					gu8_HotTempSetup = HOT_TEMP_SETUP_KOR100_JP90;
					gu8_DefaultHotTempSetup = HOT_TEMP_SETUP_KOR100_JP90;
					Play_Voice(VOICE_BEEP_ON);
				}
				else
				{
					gu8_HotTempSetup = HOT_TEMP_SETUP_KOR85_JP80;
					gu8_DefaultHotTempSetup = HOT_TEMP_SETUP_KOR85_JP80;
					Play_Voice(VOICE_BEEP_OFF);
				}
        	}
        	else
        	{
        		Play_Voice(VOICE_BEEP_WARNING);
        	}
            break;

        case WIFI_FUNC_0040_SMART_CHECK_RQST : // 0 / 1
            if ((gu8_OperationStep == Z_OperationStep_03) || (gu8_OperationStep == Z_OperationStep_04) || (gu8_OperationStep == Z_OperationStep_05)
            	|| (gu8_OperationStep == Z_OperationStep_50) || (gu8_OperationStep_hi == 0x50))
        	{
        		F_SmartCheck_Prohibit = SET;
        	}
        	else
        	{
        		F_SmartCheck = (U8)mData;					//
        	}
        break;

        case WIFI_FUNC_0047_WATER_QUANTITY_SEL:
        	if ((mData >= 130) && (mData <= 990))
        	{
				gu16_CupSize_My = mData;
				Play_Voice(VOICE_MYCUP_SET);
				gu8_TimerMySetFilker = Z_TimerMySetFilker;
        	}
        	break;

        case WIFI_FUNC_0049_OUT_BUTTON_LIGHT:
        	if (mData == 0)
        	{
        		F_EffluentLED_OFF = SET;
        		Play_Voice(VOICE_BEEP_OFF);
        	}
        	else
        	{
        		F_EffluentLED_OFF = CLEAR;
        		Play_Voice(VOICE_BEEP_ON);
        	}
        	break;

        case WIFI_FUNC_0051_QUANTITY_USE_SEL:
	/*
        	F_CupSize_MySet = (U8)mData;
        	if (F_CupSize_MySet == SET)
        	{
        		Play_Voice(VOICE_BEEP_ON);
        	}
        	else
        	{
        		if (gu8_DefaultCupSize == CUPSIZE_MY)
        		{
        			gu8_DefaultCupSize = CUPSIZE_HALFCUP;
        		}
        		Play_Voice(VOICE_BEEP_OFF);
        	}
		*/
        	break;

        case WIFI_FUNC_005C_SMART_CHECK_RESULT :
        	if (mData == CLEAR)						//	????
        	{
        		Play_Voice(VOICE_SMART_CHECK_OK);
        	}
        	else									//	NG
        	{
        		Play_Voice(VOICE_SMART_CHECK_ERROR);
        	}
            break;
           //-------------------------------------------20260201--------------------------
	   
	case WIFI_FUNC_0073_WATER_TEMP_SORT_SEL:

            if (mData == 0)						//	????????
        	{
        		gu8_f_hot_temp_sorting = Z_HOT_TEMP_ASCENDING;
        	}
        	else									//	????????
        	{
        		gu8_f_hot_temp_sorting = Z_HOT_TEMP_DESCENDING;
        	}

            Play_Voice(VOICE_BEEP_ON);        

        break;

        case WIFI_FUNC_007F_DEFAULT_TEMP_SET:

            // gu8_hot_temp_default_data_for_wifi = (U8)mData;

            if(mData == 45)
            {
                gu8_hot_temp_default_data_for_wifi = 1;
            }
            else if(mData == 50)
            {
                gu8_hot_temp_default_data_for_wifi = 2;
            }
            else if(mData == 55)
            {
                gu8_hot_temp_default_data_for_wifi = 3;
            }
            else if(mData == 60)
            {
                gu8_hot_temp_default_data_for_wifi = 4;
            }
            else if(mData == 65)
            {
                gu8_hot_temp_default_data_for_wifi = 5;
            }
            else if(mData == 70)
            {
                gu8_hot_temp_default_data_for_wifi = 6;
            }
            else if(mData == 75)
            {
                gu8_hot_temp_default_data_for_wifi = 7;
            }
            else if(mData == 80)
            {
                gu8_hot_temp_default_data_for_wifi = 8;
            }
            else if(mData == 85)
            {
                gu8_hot_temp_default_data_for_wifi = 9;
            }
            else if(mData == 90)
            {
                gu8_hot_temp_default_data_for_wifi = 10;
            }
            else if(mData == 95)
            {
                gu8_hot_temp_default_data_for_wifi = 11;
            }                                                                                    
            else if(mData == 100)
            {
                gu8_hot_temp_default_data_for_wifi = 12;
            }  
            else
            {
                gu8_hot_temp_default_data_for_wifi = 1;
            }
            HotTemp_Setting();
            return_default_hot_temp_setup();
            Play_Voice(VOICE_BEEP_ON);
        break;
        
        case WIFI_FUNC_00C1_HOTTEMP_USE_SEL0:

            if(mData == 1)
            {
                gu8_hot_temp_data_for_wifi.Bit.b0 = SET;
            }    
            else
            {
                gu8_hot_temp_data_for_wifi.Bit.b0 = CLEAR;
            }

        break;

        case WIFI_FUNC_00C2_HOTTEMP_USE_SEL1:
            if(mData == 1)
            {
                gu8_hot_temp_data_for_wifi.Bit.b1 = SET;
            }    
            else
            {
                gu8_hot_temp_data_for_wifi.Bit.b1 = CLEAR;
            }
        break;

        case WIFI_FUNC_00C3_HOTTEMP_USE_SEL2:
            if(mData == 1)
            {
                gu8_hot_temp_data_for_wifi.Bit.b2 = SET;
            }    
            else
            {
                gu8_hot_temp_data_for_wifi.Bit.b2 = CLEAR;
            }

        break;

        case WIFI_FUNC_00C4_HOTTEMP_USE_SEL3:

            if(mData == 1)
            {
                gu8_hot_temp_data_for_wifi.Bit.b3 = SET;
            }    
            else
            {
                gu8_hot_temp_data_for_wifi.Bit.b3 = CLEAR;
            }

        break;

        case WIFI_FUNC_00C5_HOTTEMP_USE_SEL4:

            if(mData == 1)
            {
                gu8_hot_temp_data_for_wifi.Bit.b4 = SET;
            }    
            else
            {
                gu8_hot_temp_data_for_wifi.Bit.b4 = CLEAR;
            }

        break;

        case WIFI_FUNC_00C6_HOTTEMP_USE_SEL5:

            if(mData == 1)
            {
                gu8_hot_temp_data_for_wifi.Bit.b5 = SET;
            }    
            else
            {
                gu8_hot_temp_data_for_wifi.Bit.b5 = CLEAR;
            }

        break;        
        case WIFI_FUNC_00C7_HOTTEMP_USE_SEL6:
            if(mData == 1)
            {
                gu8_hot_temp_data_for_wifi.Bit.b6 = SET;
            }    
            else
            {
                gu8_hot_temp_data_for_wifi.Bit.b6 = CLEAR;
            }
        
        break;

        case WIFI_FUNC_00C8_HOTTEMP_USE_SEL7:
             if(mData == 1)
            {
                gu8_hot_temp_data_for_wifi.Bit.b7 = SET;
            }    
            else
            {
                gu8_hot_temp_data_for_wifi.Bit.b7 = CLEAR;
            }

        break;

        case WIFI_FUNC_00C9_HOTTEMP_USE_SEL8:

            if(mData == 1)
            {
                gu8_hot_temp_data_for_wifi.Bit.b8 = SET;
            }    
            else
            {
                gu8_hot_temp_data_for_wifi.Bit.b8 = CLEAR;
            }
        break;

        case WIFI_FUNC_00CA_HOTTEMP_USE_SEL9:

            if(mData == 1)
            {
                gu8_hot_temp_data_for_wifi.Bit.b9 = SET;
            }    
            else
            {
                gu8_hot_temp_data_for_wifi.Bit.b9 = CLEAR;
            }

        break;

        case WIFI_FUNC_00CE_HOTTEMP_USE_SEL10:

            if(mData == 1)
            {
                gu8_hot_temp_data_for_wifi.Bit.b10 = SET;
            }    
            else
            {
                gu8_hot_temp_data_for_wifi.Bit.b10 = CLEAR;
            }

        break;

        case WIFI_FUNC_00CF_HOTTEMP_USE_SEL11:

            if(mData == 1)
            {
                gu8_hot_temp_data_for_wifi.Bit.b11 = SET;
            }    
            else
            {
                gu8_hot_temp_data_for_wifi.Bit.b11 = CLEAR;
            }

            HotTemp_Setting();
            Update_CurrentHotTemp_If_Disabled();

            Play_Voice(VOICE_BEEP_ON);

        break;
	
	case WIFI_FUNC_0129_AMOUNT_SLOT1_ENABLE:
            if(mData == 1)
            {
                gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_ONE].enable = 1;
            }
            else
            {
                gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_ONE].enable = 0;
            }
        break;
        case WIFI_FUNC_012A_AMOUNT_SLOT2_ENABLE:
            if(mData == 1)
            {
                gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_TWO].enable = 1;
            }
            else
            {
                gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_TWO].enable = 0;
            }
        break;
        case WIFI_FUNC_012B_AMOUNT_SLOT3_ENABLE:
            if(mData == 1)
            {
                gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_THREE].enable = 1;
            }
            else
            {
                gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_THREE].enable = 0;
            }
	     Update_CurrentCupSize_If_Disabled();
            Update_DefaultCupSize_If_Disabled();
	    
        break;
	/*
        case WIFI_FUNC_012C_AMOUNT_SLOT4_ENABLE:
            if(mData == 1)
            {
                gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_FOUR].enable = 1;
            }
            else
            {
                gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_FOUR].enable = 0;
            }
            Update_CurrentCupSize_If_Disabled();
            Update_DefaultCupSize_If_Disabled();
        break;
	*/

        case WIFI_FUNC_0132_START_WATER_QUANTITY_SLOT:
            gu8_DefaultCupSize = (U8)mData - 1;
            Update_DefaultCupSize_If_Disabled();
            Play_Voice(VOICE_BEEP_ON); 
        break;

        case WIFI_FUNC_0133_START_WATER_QUANTITY_SEL_1:            
            gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_ONE].amount_volume = (U8)(mData / 10);
        break;

        case WIFI_FUNC_0134_START_WATER_QUANTITY_SEL_2:
            gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_TWO].amount_volume = (U8)(mData / 10);
        break;

        case WIFI_FUNC_0135_START_WATER_QUANTITY_SEL_3:
            gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_THREE].amount_volume = (U8)(mData / 10);
        break;
        /*
        case WIFI_FUNC_0136_START_WATER_QUANTITY_SEL_4:
            gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_FOUR].amount_volume = (U8)(mData / 10);                       
        break;        
	*/
	
        case WIFI_FUNC_013C_START_WATER_QUANTITY_SEL_10:
            if(mData == 1)
            {
                gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_CONTINUE].enable = 1;
            }
            else
            {
                gu8_Cup_size_Array[Z_CUP_SIZE_ARRAY_CONTINUE].enable = 0;
            }

            Update_CurrentCupSize_If_Disabled();

            Play_Voice(VOICE_BEEP_ON); 
        break;   
	   
	   
        default:
        break;
    }
}

#endif

