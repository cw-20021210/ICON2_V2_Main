
#include "WIFI_Common.h"
#include "WIFI_SetFunctionData.h"
#include "WIFI_SetSensorData.h"
#include "WIFI_SetErrorData.h"
#include "WIFI_SetPartData.h"
#include "WIFI_SetAccumulateData.h"

#ifdef MODEL_NO_2


void UserWriteEep ( U16 mu16Addr, U8 mu8Val )
{
    EepromByteWrite(mu16Addr,mu8Val);
}

U8 UserReadEep ( U16 mu16Addr )
{
    U8 mu8Data = 0U;
    EepromSeqRead(mu16Addr, gu8_eeprom_rbuf, 1);
    mu8Data = gu8_eeprom_rbuf[0];
    return mu8Data;
}

void UserBuzzSound ( U8 mu8Buzz )
{
  gu8WifiBuzzSound = mu8Buzz;
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
    U8 mu8Return = 0;
    if ( mu8Value == USER_MODEL_TYPE )
    {
        if ( gu8ModelType == MODEL_CP)
        {
            mu8Return = USER_MODEL_CP;
        }
        else
        {
            mu8Return = USER_MODEL_CHP;
        }
    }
    else if ( mu8Value == USER_FACTORY_STATUS ) 
    { /* 공장테스트모드 확인 */
        if ( F_PCBTestMode == SET || F_AutoTestMode == SET || F_FaucetTestMode == SET )
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
        if ( (gu8ProductStatus == PRODUCT_STATUS_EFFLUENT) ||
          (gu8ProductStatus == PRODUCT_STATUS_FIRST_HOT_FLUSHING) ||
          (gu8ProductStatus == PRODUCT_STATUS_FLUSHING) ||
          (gu8ProductStatus == PRODUCT_STATUS_EW_STERILIZATION) ||
          (gu8ProductStatus == PRODUCT_STATUS_SETUP_MODE) ||
          F_FilterCoverOpen || !F_FilterConnect)
        {   // 추출, 온수 첫잔 드레인, 최초 플러싱, 유로 살균, 설정모드 동작 상태인 경우
            mu8Return = FALSE; // 어플동작금지
        }
        else
        {
            mu8Return = TRUE; // 어플동작가능
        }
    }
    else if ( mu8Value == USER_SMART_POSSIBLE_STATUS )
    { // 추출 / 플러싱 / ew살균
        if ( (gu8ProductStatus == PRODUCT_STATUS_EFFLUENT) ||
          (gu8ProductStatus == PRODUCT_STATUS_FIRST_HOT_FLUSHING) ||
          (gu8ProductStatus == PRODUCT_STATUS_FLUSHING) ||
          (gu8ProductStatus == PRODUCT_STATUS_EW_STERILIZATION) )
        {
            mu8Return = FALSE; // 정밀진단 불가
        }
        else
        {
            mu8Return = TRUE; // 정밀진단 가능
        }

    }
    else if ( mu8Value == USER_ERROR_STATUS )
    { /* 제품 에러확인 */
        if ( gu8LastOperatingError != 0 )
        {
            mu8Return = TRUE;   // 에러
        }
        else
        {
            mu8Return = FALSE;  // 정상
        }
    }
    else if ( mu8Value == USER_FOTA_STATUS )
    { // 추출 / 플러싱 / EW살균
        if ( (gu8ProductStatus == PRODUCT_STATUS_EFFLUENT) ||
          (gu8ProductStatus == PRODUCT_STATUS_FIRST_HOT_FLUSHING) ||
          (gu8ProductStatus == PRODUCT_STATUS_FLUSHING) ||
          (gu8ProductStatus == PRODUCT_STATUS_EW_STERILIZATION) )
        {
            mu8Return = FALSE; // 포타 불가
        }
        else
        {
            mu8Return = TRUE; // 포타 가능
        }
    }
    else if ( mu8Value == USER_SMART_SENSING_STATUS )
    {
        if (gu8ProductStatus == PRODUCT_STATUS_SELF_CHECKING)
        {
            mu8Return = TRUE ; // 정밀진단 중
        }
        else
        {
            mu8Return = FALSE; // 일반
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
  gu16FilterCoverOpenTime = 0;
}

void SetUserSystemApplyData ( U16 mId )
{
    if ( mId == USER_APPLY_SMART_REJECT )
    {
//    	F_SmartCheck_Prohibit = SET;
    }
}

/* User Display Setting */
void SetUserSystemDisplay ( U8 mu8Display )
{
    if ( mu8Display == USER_DISPLAY_FOTA ) // FOTA
    {
        F_FOTA_Start = SET;
    }
    else if ( mu8Display == USER_DISPLAY_SELFCHECK ) // 자가진단
    {

    }
    else if ( mu8Display == USER_DISPLAY_PARTCHECK ) // 고장진단
    {

    }
    else
    {

    }
}

/* Time Setting */
void SetUserSystemTime ( U16 mYear, U16 mMonth, U16 mDate, U16 mHour, U16 mMin, U16 mSec, U16 mDay )
{
  F_RTC_Setting = SET;
  RTC_Time_Calibration(mYear, mMonth, mDate, mHour, mMin, mSec, mDay);
  F_RTC_Setting = CLEAR;
}


/* User Error Data */
U16 GetUserSystemError ( U16 mu16Err )
{
    U16 mu16Data = 0U;
  
    switch ( mu16Err ) // 0:Normal / 1:Error
    {
        case WIFI_ERROR_0006_HOTIN_TEMP :
          mu16Data = F_HotInSensorError;
          break;

        case WIFI_ERROR_0007_HOTOUT_TEMP :
          mu16Data = F_HotOutSensorError;
          break;

        case WIFI_ERROR_0008_COLD_TEMP :
          mu16Data = F_ColdSensorError;
          break;

        case WIFI_ERROR_0009_NATURAL_TEMP :
          mu16Data = F_EVSensorError;
          break;

        case WIFI_ERROR_000C_BLDC_COMP :
          mu16Data = F_BLDC_UART_CommunicationError;
          break;

        case WIFI_ERROR_0019_STOP_WATER :
          mu16Data = F_WaterInputError;
          break;

        case WIFI_ERROR_001B_STOP_HOTWATER :
          mu16Data = F_DrainCutOffError;
          break;

        case WIFI_ERROR_001C_LEAK_FLOW :
          mu16Data = F_FlowLeakageError;
          break;

        case WIFI_ERROR_001D_LEAK_SENSOR :
          mu16Data = F_LeakageError;
          break;

        case WIFI_ERROR_001E_HOT_OVER :
          mu16Data = F_OverHeatError;
          break;

        case WIFI_ERROR_001F_HEATER_SENSOR :
          mu16Data = F_HeaterBodySensorError;
          break;

        case WIFI_ERROR_0028_ICE_SHAFT_LEVEL :
          mu16Data = F_WaterLevelHighError;
          break;

        case WIFI_ERROR_0029_BLDC_STATUS :
          mu16Data = F_BLDCCompError;
          break;

        case WIFI_ERROR_002A_STIRRING_MOTOR :
          mu16Data = F_ColdAgitatorActError;
          break;

        case WIFI_ERROR_002B_COMP_OUT_TEMP:
          // 현재 미사용
          break;

        case WIFI_ERROR_002C_STERILIZE_MODULE :
          mu16Data = F_EW_Error;
          break;

        case WIFI_ERROR_002D_UV_OPERATION :
          mu16Data = F_UV_Error;
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

    switch ( mu16Func )
    {        
        case WIFI_FUNC_0002_COLD_SEL : // 0 / 1
          if (F_ColdLock)
          {
            mu16Data = 0;
          }
          else
          {
            mu16Data = 1;
          }
          break;

        case WIFI_FUNC_0003_HOT_LOCK_SEL : // 0 / 1 / 2 (H)
          if (F_HotLock)
          {
            mu16Data = 2;
          }
          else
          {
            mu16Data = 1;
          }
          break;

        case WIFI_FUNC_0005_LOCK_SEL : // 0 / 1
          if (F_AllLock)
          {
            mu16Data = 1;
          }
          else
          {
            mu16Data = 0;
          }
          break;

        case WIFI_FUNC_000A_COLD_TEMP : 
          if (gu8ColdDisplay == COLD_DISPLAY_OFF)
          {
            mu16Data = 0;
          }
          else if (gu8ColdDisplay == COLD_DISPLAY_WHITE)
          {
            mu16Data = 1;
          }
          else if (gu8ColdDisplay == COLD_DISPLAY_BLUE)
          {
            mu16Data = 2;
          }
          break;
        
        case WIFI_FUNC_000B_WATER_SEL : // 1(A) / 2(C) / 3(H)
          if (gu8WaterSelect == NORMAL_WATER)
          {
            mu16Data = 1;
          }
          else if (gu8WaterSelect == COLD_WATER)
          {
            mu16Data = 2;
          }
          else if (gu8WaterSelect == HOT_WATER)
          {
            mu16Data = 3;
          }
          break;        

        case WIFI_FUNC_000D_WATER_OUTQUANTITY :
          mu16Data = (U16)((gu32EffluentHz / 35) * 10);
          gu32EffluentHz = 0;
          break;
        
        case WIFI_FUNC_0013_STERILIZE_TIME :
          mu16Data = (U16)(gu8SterilizeReserveHour);
          break;       

        case WIFI_FUNC_0014_STERILIZE_STATUS :
          if (gu8SterilizeWifiStep == 1)
          {
            mu16Data = 1;
            gu8SterilizeWifiStep = 2;
          }
          else if (gu8SterilizeWifiStep == 2)
          {
            mu16Data = 2;
          }
          else 
          {
            mu16Data = 0;
          }
          break;

        case WIFI_FUNC_001A_HOT_INTEMP :
          mu16Data = gu8HotInTemp;
          break;

        case WIFI_FUNC_001B_HOT_MEANTEMP :
          mu16Data = gu8HotAverageTemp;
          break;

        case WIFI_FUNC_001C_HOT_SELTEMP :
          if (gu8SetupHotTemp == HOT_TEMP_SETUP_STEP1)
          {
            mu16Data = 45;
          }
          else if (gu8SetupHotTemp == HOT_TEMP_SETUP_STEP2)
          {
            mu16Data = 70;
          }
          else if (gu8SetupHotTemp == HOT_TEMP_SETUP_STEP3)
          {
            mu16Data = 85;
          }
          break;

        case WIFI_FUNC_001F_DRAIN_STATUS :
          if (gu8AutoDrainWifiStep == 1)
          {
            mu16Data = 1;
            gu8AutoDrainWifiStep = 2;
          }
          else if (gu8AutoDrainWifiStep == 2)
          {
            mu16Data = 2;
          }
          else 
          {
            mu16Data = 0;
          }
          break;

        case WIFI_FUNC_0024_HEART_TIME4 :
          if (!F_FilterCoverOpen)
          {
            if (gu16FilterCoverOpenTime)
            {
              mu16Data = gu16FilterCoverOpenTime;
              gu16FilterCoverOpenTime = 0;
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

        case WIFI_FUNC_002C_SILVER_CARE : // 0 / 1
          if (gu848HoursNoUseStep == 1)
          {
            mu16Data = 1;
            gu848HoursNoUseStep = 2;
          }
          else if (gu848HoursNoUseStep == 2)
          {
            mu16Data = 2;
          }
          else
          {
            mu16Data = 0;
          }
          break;

        case WIFI_FUNC_0031_VOICE_SEL :
          if (gu8VoiceType == VOICE_TYPE_OFF)
          {
            mu16Data = 1;
          }
          else if (gu8VoiceType == VOICE_TYPE_MELODY)
          {
            mu16Data = 2;
          }
          else if (gu8VoiceType == VOICE_TYPE_VOICE)
          {
            mu16Data = 3;
          }
          break;

        case WIFI_FUNC_0033_VOICE_VOL_SEL :
          mu16Data = gu8VoiceVolume;
          break;

        case WIFI_FUNC_0035_COVER1_OPEN :
          if (F_FilterCoverOpen)
          {
            mu16Data = 1;
          }
          else
          {
            mu16Data = 0;
          }
          break;

        case WIFI_FUNC_0040_SMART_CHECK_RQST :
          break;

        case WIFI_FUNC_0041_SMART_CHECK_STATUS :
          if (((gu8ProductStatus == PRODUCT_STATUS_EFFLUENT) && !F_EffluentActEndInfo) ||
            ((gu8ProductStatus == PRODUCT_STATUS_FLUSHING) && !F_FlushingActEndInfo) ||
            ((gu8ProductStatus == PRODUCT_STATUS_FIRST_HOT_FLUSHING) && !F_FirstHotDrainActEndInfo) ||
            ((gu8ProductStatus == PRODUCT_STATUS_EW_STERILIZATION) && !F_EW_SterilizationActEndInfo))
          {
            mu16Data = 5;
            gu16SelfCheckStatus = 0;
            gu8SelfCheckStep = 0;
          }
          else
          {
            if (gu16SelfCheckStatus == 0)
            {
              mu16Data = 0;
            }
            else if (gu16SelfCheckStatus == 1)
            {
              mu16Data = 1;
              gu16SelfCheckStatus = 2;
            }
            else if (gu16SelfCheckStatus == 2)
            {
              mu16Data = 2;
            }
            else if (gu16SelfCheckStatus == 3)
            {
              mu16Data = 3;
            }
            else if (gu16SelfCheckStatus == 4)
            {
              mu16Data = 4;
              gu16SelfCheckStatus = 0;
              gu8SelfCheckStep = 0;
            }
            else if (gu16SelfCheckStatus == 5)
            {
              mu16Data = 5;
              gu16SelfCheckStatus = 0;
              gu8SelfCheckStep = 0;
            }
          }
          break;
          
        case WIFI_FUNC_0042_SMART_CHECK_PROGRESS :
          mu16Data = gu8SelfCheckStep;
          if ((gu16SelfCheckStatus == 3) && (gu8SelfCheckStep == 100))
          {
            gu16SelfCheckStatus = 0;
            gu8SelfCheckStep = 0;
          }
          break;

        case WIFI_FUNC_0043_FILTER_STATUS1 :
          if (F_FilterConnect)
          {   // 필터 체결 상태
            mu16Data = 0;
          }
          else
          {   // 필터 미체결 상태
            mu16Data = 1;
          }
          break;

        case WIFI_FUNC_0047_WATER_QUANTITY_SEL :
          mu16Data = gu16UserSetCupSize;
          break;

        case WIFI_FUNC_004A_WELCOME_LIGHT :
          if (F_WelcomeLightOn)
          {
            mu16Data = 1;
          }
          else
          {
            mu16Data = 0;
          }
          break;

        case WIFI_FUNC_004B_HIDDEN_LCD_SEL :
          if (gu8StandByDisplayData == DISPLAY_WATER)
          {
            mu16Data = 0;
          }
          else if (gu8StandByDisplayData == DISPLAY_CLOCK)
          {
            mu16Data = 1;
          }
          else if (gu8StandByDisplayData == DISPLAY_WEATHER)
          {
            mu16Data = 3;
          }
          break;

        case WIFI_FUNC_004C_FAUCET_MOVE_SEL :
          if (gu8HiddenDelayTime == HIDDEN_DELAY_TIME_1MIN)
          {
            mu16Data = 1;
          }
          else if (gu8HiddenDelayTime == HIDDEN_DELAY_TIME_5MIN)
          {
            mu16Data = 5;
          }
          else if (gu8HiddenDelayTime == HIDDEN_DELAY_TIME_10MIN)
          {
            mu16Data = 10;
          }
          else if (gu8HiddenDelayTime == HIDDEN_DELAY_TIME_30MIN)
          {
            mu16Data = 30;
          }
          else if (gu8HiddenDelayTime == HIDDEN_DELAY_TIME_60MIN)
          {
            mu16Data = 60;
          }
          break;

        case WIFI_FUNC_004D_FAUCET_MOVE_STATUS :
          mu16Data = F_FaucetHiddenStatus;
          break;

        case WIFI_FUNC_004E_CUP_SENSING_SEL :
          if (F_CupSensingUse)
          {
            mu16Data = 1;
          }
          else
          {
            mu16Data = 0;
          }
          break;

        case WIFI_FUNC_0050_UV_STERILIZE_STATUS :
          if (gu8UV_SterilizeWifiStep == 1)
          {
            mu16Data = 1;
            gu8UV_SterilizeWifiStep = 2;
          }
          else if (gu8UV_SterilizeWifiStep == 2)
          {
            mu16Data = 2;
          }
          else
          {
            mu16Data = 0;
          }
          break;

        case WIFI_FUNC_0051_QUANTITY_USE_SEL :
          if (F_UserCupsizeUse)
          {
            mu16Data = 1;
          }
          else
          {
            mu16Data = 0;
          }
          break;

//        case WIFI_FUNC_0052_VOICE_MODE_SEL :
//          if (F_OptionVoiceUse)
//          {
//            mu16Data = 1;
//          }
//          else
//          {
//            mu16Data = 0;
//          }
//          break;

        case WIFI_FUNC_0053_FILTER_RESET1 :
          if (F_FilterReset)
          {
            mu16Data = 1;
            F_FilterReset = CLEAR;
          }
          else
          {
            mu16Data = 0;
          }
          break;

        case WIFI_FUNC_0055_CUP_SENSING_SENSITIVITY :
          mu16Data = gu8CupSensorSensitivity;
          break;
        
        default:
          mu16Data = 0U;
          break;
    }
    return mu16Data;
}

/* User Sensor Data */
U16 GetUserSystemSensor ( U16 mu16Sen )
{
    U16 mu16Data = 0U;

    switch ( mu16Sen )
    {
        case WIFI_SENSOR_0001_COLD_TEMP : // 0 ~ 99
          mu16Data = gu8ColdTemp;
          break;

        case WIFI_SENSOR_0004_OUT_TEMP : //  0 ~ 99
          mu16Data = gu8EVTemp;
          break;

        case WIFI_SENSOR_000E_WATT_DATA : // 0.01~250.00 (ex -> 1.78 = 178)
          mu16Data = gu16WattData;
          gu16WattData = 0;   // 데이터 전달 후 초기화
          break;

        case WIFI_SENSOR_000F_COMP_DATA : //  0 / 1
          if (P_COOLER_COMP)
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

/* User Part Data */
U16 GetUserSystemPart ( U16 mu16Part )
{
    U16 mu16Data = 0U;
    
    switch ( mu16Part )
    {
        case WIFI_PART_0001_VALVE_NOS :         // NOS밸브
          mu16Data = gu16SelfCheck_NosValve_C;
          gu16SelfCheck_NosValve_C = 0xFFFF;
          break;

        case WIFI_PART_0006_VALVE_CHP_OUT :     // 추출 밸브
          mu16Data = gu16SelfCheck_FaucetValve_C;
          gu16SelfCheck_FaucetValve_C = 0xFFFF;
          break;

        case WIFI_PART_0007_VALVE_PURE_IN :     // 정수 입수 밸브
          mu16Data = gu16SelfCheck_AmbiValve_C;
          gu16SelfCheck_AmbiValve_C = 0xFFFF;
          break;

        case WIFI_PART_0009_VALVE_HOT_IN :      // 온수 입수 밸브
          mu16Data = gu16SelfCheck_HotInValve_C;
          gu16SelfCheck_HotInValve_C = 0xFFFF;
          break;

        case WIFI_PART_000C_VALVE_COLD_IN :     // 냉수 입수 밸브
          mu16Data = gu16SelfCheck_ColdValve_C;
          gu16SelfCheck_ColdValve_C = 0xFFFF;
          break;

        case WIFI_PART_000E_VALVE_ICE_STORAGE_IN :  // 빙축수 입수 밸브
          mu16Data = gu16SelfCheck_IceValve_C;
          gu16SelfCheck_IceValve_C = 0xFFFF;
          break;

        case WIFI_PART_0011_VALVE_STERILIZE :   // 살균수 입수 밸브
          mu16Data = gu16SelfCheck_EWValve_C;
          gu16SelfCheck_EWValve_C = 0xFFFF;
          break;

        case WIFI_PART_0012_VALVE_DRAIN_NOS :   // 드레인 NOS 밸브
          mu16Data = gu16SelfCheck_DrainNosValve_C;
          gu16SelfCheck_DrainNosValve_C = 0xFFFF;
          break;

        case WIFI_PART_0104_LEVEL_ICE_STORAGE_HIGH :  // 빙축수 수위 센서
          if (F_TankEmpty)
          {   // 이상
            mu16Data = 1;  
          }
          else
          {   // 정상
            mu16Data = 0;
          }
          break;

        case WIFI_PART_0201_COLD_BLDC :         // BLDC COMP 모듈
          mu16Data = gu16SelfCheck_CompErrorCode;
          gu16SelfCheck_CompErrorCode = 0xFFFF;
          break;

        case WIFI_PART_0204_COLD_FAN :          // FAN
          mu16Data = gu16SelfCheck_FAN_C;
          gu16SelfCheck_FAN_C = 0xFFFF;
          break;

        case WIFI_PART_0205_COLD_STIRRING_MOTOR :   // 교반 모터
          mu16Data = gu16SelfCheck_AgitatorHz;
          gu16SelfCheck_AgitatorHz = 0xFFFF;
          break;

        case WIFI_PART_0206_COLD_TEMP_1 :       // 냉수 온도 센서
          mu16Data = (U16)(gu8ColdTemp);
          break;

        case WIFI_PART_0208_COLD_ROOM_TEMP :    // 환경 온도 센서
          mu16Data = (U16)(gu8EVTemp);
          break;

        case WIFI_PART_0302_HOT_INSTANT_HEATER_1 :  // 순간온수 히터1
          mu16Data = gu16SelfCheck_Heater1Detect;
          gu16SelfCheck_Heater1Detect = 0xFFFF;
          break;

        case WIFI_PART_0303_HOT_INSTANT_HEATER_2 :  // 순간온수 히터2
          mu16Data = gu16SelfCheck_Heater2Detect;
          gu16SelfCheck_Heater2Detect = 0xFFFF;
          break;

        case WIFI_PART_0304_HOT_FLOW_MOTOR :      // 온수 디스크 밸브
          mu16Data = gu16SelfCheck_DiskValve_C;
          gu16SelfCheck_DiskValve_C = 0xFFFF;
          break;

        case WIFI_PART_0305_HOT_TEMP_IN :         // 온수 입수 온도 센서
          mu16Data = (U16)gu8HotInTemp;
          break;

        case WIFI_PART_0306_HOT_TEMP_OUT :        // 온수 출수 온도 센서
          mu16Data = (U16)gu8HotOutTemp;
          break;

        case WIFI_PART_0402_SENSOR_LEAK :         // 누수 감지 센서
          if (F_Leakage_Detect)
          {
            mu16Data = 1;
          }
          else
          {
            mu16Data = 0;
          }
          break;

        case WIFI_PART_0403_SENSOR_FLOW :         // 유량 감지
          mu16Data = gu16SelfCheck_FlowSensor;
          gu16SelfCheck_FlowSensor = 0xFFFF;
          break;

        case WIFI_PART_0405_SENSOR_FILTER_REED :  // 필터 커버 리드 스위치
          if (F_FilterCoverOpen)
          {   // 이상
            mu16Data = 1;
          }
          else
          {   // 정상
            mu16Data = 0;
          }
          break;

        case WIFI_PART_0406_SENSOR_FILTER_SW_1 :  // 필터 리드 스위치
          if (F_FilterConnect)
          {   // 정상
            mu16Data = 0;
          }
          else
          {   // 이상
            mu16Data = 1;
          }
          break;

        case WIFI_PART_0500_STER_CSI_MODULE :   // EW 모듈
          mu16Data = gu16SelfCheck_EW_C;
          gu16SelfCheck_EW_C = 0xFFFF;
          break;

        case WIFI_PART_0501_STER_UV_FAUCET :    // UV
          mu16Data = gu16SelfCheck_UV_C;
          gu16SelfCheck_UV_C = 0xFFFF;
          break;

        case WIFI_PART_0700_FAUCET_REED_1 :       // 파우셋 리드 스위치
          if (F_FaucetHiddenStatus)
          {   // 이상
            mu16Data = 1;
          }
          else
          {
            mu16Data = 0;
          }
          break;

        case WIFI_PART_0702_FAUCET_CUP_SENSOR :   // 컵 감지 센서
          mu16Data = gu16SelfCheck_CupSensing;
          gu16SelfCheck_CupSensing = 0xFFFF;
          break;
          
        default:
          mu16Data = 0U;
        break;
    }
    return mu16Data;
}
    

U32 GetUserSystemAccumulate ( U16 mu16Acc )
{
    U32 mu32Data = 0U;
    
    switch ( mu16Acc ) // 0 ~ 999999
    {
        case WIFI_ACCM_0000_MONTH_WATER_USER :
            mu32Data = 1000000;
        break;

        case WIFI_ACCM_0004_DAY_WATER_USER :
            mu32Data = 1010;
        break;
        
        default:
          mu32Data = 0U;
        break;
    }
    /* Max 999999 */
    if ( mu32Data  >= 999999U )
    {
        mu32Data = 999999U;
    }
    return mu32Data;
}

/* App Control -> User Function Data */
void SetUserSystemFunction ( U16 mu16Func, U16 mData )
{
    switch ( mu16Func )
    {
        case WIFI_FUNC_0002_COLD_SEL :
          if (mData)
          {
            if (F_ColdLock)
            {
              F_ColdLock = CLEAR;
              EepromByteWrite(EEPROM_ADDR_COLDLOCK, 0x00);
              F_WifiStatusChange = SET;
            }            
          }
          else
          {
            if (!F_ColdLock)
            {
              F_ColdLock = SET;
              EepromByteWrite(EEPROM_ADDR_COLDLOCK, 0x01);
              F_WifiStatusChange = SET;
            }            
          } 
          break;

        case WIFI_FUNC_0003_HOT_LOCK_SEL :
          if (mData == 2)
          {
            if (!F_HotLock)
            {
              F_HotLock = SET;
              EepromByteWrite(EEPROM_ADDR_HOTLOCK, 0x01);
              F_WifiStatusChange = SET;
            }            
          }
          else
          {
            if (F_HotLock)
            {
              F_HotLock = CLEAR;
              EepromByteWrite(EEPROM_ADDR_HOTLOCK, 0x00);
              F_WifiStatusChange = SET;
            }            
          }   
          break;

        case WIFI_FUNC_0005_LOCK_SEL :
          if (mData)
          {
            if (!F_AllLock)
            {
              F_AllLock = SET;
              EepromByteWrite(EEPROM_ADDR_ALL_LOCK, 0x01);
              F_WifiStatusChange = SET;
            }
          }
          else
          {
            if (F_AllLock)
            {
              F_AllLock = CLEAR;
              EepromByteWrite(EEPROM_ADDR_ALL_LOCK, 0x00);
              F_WifiStatusChange = SET;
            }
          }
          break;

        case WIFI_FUNC_000A_COLD_TEMP:
          break;

        case WIFI_FUNC_000B_WATER_SEL :
          break;

        case WIFI_FUNC_000D_WATER_OUTQUANTITY :
          break;

        case WIFI_FUNC_0013_STERILIZE_TIME :
          if (gu8SterilizeReserveHour != (U8)(mData))
          {
            gu8SterilizeReserveHour = (U8)(mData);
            EepromByteWrite(EEPROM_ADDR_RESERVE_HOUR, gu8SterilizeReserveHour);
            F_WifiStatusChange = SET;
          }
          break;

        case WIFI_FUNC_0014_STERILIZE_STATUS :
          break;

        case WIFI_FUNC_001A_HOT_INTEMP :
          break;

        case WIFI_FUNC_001B_HOT_MEANTEMP :
          break;

        case WIFI_FUNC_001C_HOT_SELTEMP :
          break;

        case WIFI_FUNC_001F_DRAIN_STATUS :
          break;

        case WIFI_FUNC_0024_HEART_TIME4 :
          break;

        case WIFI_FUNC_002C_SILVER_CARE :
          break;

        case WIFI_FUNC_0031_VOICE_SEL :
          if (mData == 1)
          {
            if (gu8VoiceType != VOICE_TYPE_OFF)
            {
              gu8VoiceType = VOICE_TYPE_OFF;
              EepromByteWrite(EEPROM_ADDR_VOICE_TYPE, gu8VoiceType);
              F_WifiStatusChange = SET;
            }
          }
          else if (mData == 2)
          {
            if (gu8VoiceType != VOICE_TYPE_MELODY)
            {
              gu8VoiceType = VOICE_TYPE_MELODY;
              EepromByteWrite(EEPROM_ADDR_VOICE_TYPE, gu8VoiceType);
              F_WifiStatusChange = SET;
            }
          }
          else if (mData == 3)
          {
            if (gu8VoiceType != VOICE_TYPE_VOICE)
            {
              gu8VoiceType = VOICE_TYPE_VOICE;
              EepromByteWrite(EEPROM_ADDR_VOICE_TYPE, gu8VoiceType);
              F_WifiStatusChange = SET;
            }
          }          
          break;

        case WIFI_FUNC_0033_VOICE_VOL_SEL :
          if (gu8VoiceVolume != (U8)(mData))
          {
            gu8VoiceVolume = (U8)(mData);
            EepromByteWrite(EEPROM_ADDR_VOICE_VOLUME, gu8VoiceVolume);
            F_WifiStatusChange = SET;
          }
          break;

        case WIFI_FUNC_0035_COVER1_OPEN :
          break;

        case WIFI_FUNC_0040_SMART_CHECK_RQST :
          if (mData == 1)
          {
            F_SelfCheckActRequest = SET;    // 스마트 진단 모드 시작
          }
          else if (mData == 0)
          {
            gu16SelfCheckDelay = 0;
            gu8SelfCheck_Seq = SELF_CHECK_STEP_14;   // 종료 단계 이동
          }
          break;

        case WIFI_FUNC_0041_SMART_CHECK_STATUS :
          break;

        case WIFI_FUNC_0042_SMART_CHECK_PROGRESS :
          break;

        case WIFI_FUNC_0043_FILTER_STATUS1 :
          break;

        case WIFI_FUNC_0047_WATER_QUANTITY_SEL :
          if (mData < 130)
          {
            if (gu16UserSetCupSize != 130)
            {
              gu16UserSetCupSize = 130;
              EepromByteWrite(EEPROM_ADDR_USER_CUPSIZE_H, (U8)((gu16UserSetCupSize >> 8) & 0x00FF));
              EepromByteWrite(EEPROM_ADDR_USER_CUPSIZE_L, (U8)(gu16UserSetCupSize & 0x00FF));
              F_WifiStatusChange = SET;
            }
          }
          else if (mData > 1000)
          {
            if (gu16UserSetCupSize != 1000)
            {
              gu16UserSetCupSize = 1000;
              EepromByteWrite(EEPROM_ADDR_USER_CUPSIZE_H, (U8)((gu16UserSetCupSize >> 8) & 0x00FF));
              EepromByteWrite(EEPROM_ADDR_USER_CUPSIZE_L, (U8)(gu16UserSetCupSize & 0x00FF));
              F_WifiStatusChange = SET;
            }
          }
          else
          {
            if (gu16UserSetCupSize != mData)
            {
              gu16UserSetCupSize = mData;
              EepromByteWrite(EEPROM_ADDR_USER_CUPSIZE_H, (U8)((gu16UserSetCupSize >> 8) & 0x00FF));
              EepromByteWrite(EEPROM_ADDR_USER_CUPSIZE_L, (U8)(gu16UserSetCupSize & 0x00FF));
              F_WifiStatusChange = SET;
            }
          }
          break;

        case WIFI_FUNC_0048_HOT_TEMP_SEL :
          break;

        case WIFI_FUNC_004A_WELCOME_LIGHT :
          if (mData)
          {
            if (!F_WelcomeLightOn)
            {
              F_WelcomeLightOn = SET;
              EepromByteWrite(EEPROM_ADDR_WELCOME_LIGHT_USE, 0x01);
              F_WifiStatusChange = SET;
            }
          }
          else
          {
            if (F_WelcomeLightOn)
            {
              F_WelcomeLightOn = CLEAR;
              EepromByteWrite(EEPROM_ADDR_WELCOME_LIGHT_USE, 0x00);
              F_WifiStatusChange = SET;
            }
          }
          break;

        case WIFI_FUNC_004B_HIDDEN_LCD_SEL :
          if (mData == 0)
          {
            if (gu8StandByDisplayData != DISPLAY_WATER)
            {
              gu8StandByDisplayData = DISPLAY_WATER;
              EepromByteWrite(EEPROM_ADDR_STANDBY_MODE, gu8StandByDisplayData);
              F_WifiStatusChange = SET;
            }
          }
          else if (mData == 1)
          {
            if (gu8StandByDisplayData != DISPLAY_CLOCK)
            {
              gu8StandByDisplayData = DISPLAY_CLOCK;
              EepromByteWrite(EEPROM_ADDR_STANDBY_MODE, gu8StandByDisplayData);
              F_WifiStatusChange = SET;
            }
          }
          else if (mData == 3)
          {
            if (gu8StandByDisplayData != DISPLAY_WEATHER)
            {
              gu8StandByDisplayData = DISPLAY_WEATHER;
              EepromByteWrite(EEPROM_ADDR_STANDBY_MODE, gu8StandByDisplayData);
              F_WifiStatusChange = SET;
            }
          }
          break;

        case WIFI_FUNC_004C_FAUCET_MOVE_SEL :
          if (mData == 1)
          {
            if (gu8HiddenDelayTime != HIDDEN_DELAY_TIME_1MIN)
            {
              gu8HiddenDelayTime = HIDDEN_DELAY_TIME_1MIN;
              EepromByteWrite(EEPROM_ADDR_HIDDEN_DELAY_TIME, gu8HiddenDelayTime);
              F_WifiStatusChange = SET;
            }
          }
          else if (mData == 5)
          {
            if (gu8HiddenDelayTime != HIDDEN_DELAY_TIME_5MIN)
            {
              gu8HiddenDelayTime = HIDDEN_DELAY_TIME_5MIN;
              EepromByteWrite(EEPROM_ADDR_HIDDEN_DELAY_TIME, gu8HiddenDelayTime);
              F_WifiStatusChange = SET;
            }
          }
          else if (mData == 10)
          {
            if (gu8HiddenDelayTime != HIDDEN_DELAY_TIME_10MIN)
            {
              gu8HiddenDelayTime = HIDDEN_DELAY_TIME_10MIN;
              EepromByteWrite(EEPROM_ADDR_HIDDEN_DELAY_TIME, gu8HiddenDelayTime);
              F_WifiStatusChange = SET;
            }
          }
          else if (mData == 30)
          {
            if (gu8HiddenDelayTime != HIDDEN_DELAY_TIME_30MIN)
            {
              gu8HiddenDelayTime = HIDDEN_DELAY_TIME_30MIN;
              EepromByteWrite(EEPROM_ADDR_HIDDEN_DELAY_TIME, gu8HiddenDelayTime);
              F_WifiStatusChange = SET;
            }
          }
          else if (mData == 60)
          {
            if (gu8HiddenDelayTime != HIDDEN_DELAY_TIME_60MIN)
            {
              gu8HiddenDelayTime = HIDDEN_DELAY_TIME_60MIN;
              EepromByteWrite(EEPROM_ADDR_HIDDEN_DELAY_TIME, gu8HiddenDelayTime);
              F_WifiStatusChange = SET;
            }
          }
          break;

        case WIFI_FUNC_004D_FAUCET_MOVE_STATUS :
          break;

        case WIFI_FUNC_004E_CUP_SENSING_SEL :
          if (mData)
          {
            if (!F_CupSensingUse)
            {
              F_CupSensingUse = SET;
              EepromByteWrite(EEPROM_ADDR_CUP_SENSING_USE, 0x01);
              F_WifiStatusChange = SET;
            }
          }
          else
          {
            if (F_CupSensingUse)
            {
              F_CupSensingUse = CLEAR;
              EepromByteWrite(EEPROM_ADDR_CUP_SENSING_USE, 0x00);
              F_WifiStatusChange = SET;
            }
          }
          break;

        case WIFI_FUNC_0050_UV_STERILIZE_STATUS :
          break;

        case WIFI_FUNC_0051_QUANTITY_USE_SEL :
          if (mData)
          {
            if (!F_UserCupsizeUse)
            {
              F_UserCupsizeUse = SET;
              EepromByteWrite(EEPROM_ADDR_USER_CUPSIZE_USE, 0x01);
              F_WifiStatusChange = SET;
            }
          }
          else
          {
            if (F_UserCupsizeUse)
            {
              F_UserCupsizeUse = CLEAR;
              EepromByteWrite(EEPROM_ADDR_USER_CUPSIZE_USE, 0x00);
              F_WifiStatusChange = SET;
            }
          }
          break;

//        case WIFI_FUNC_0052_VOICE_MODE_SEL :
//          if (mData)
//          {
//            if (!F_OptionVoiceUse)
//            {
//              F_OptionVoiceUse = SET;
//             EepromByteWrite(EEPROM_ADDR_OPTION_VOICE_USE, 0x01);
//              F_WifiStatusChange = SET;
//            }
//          }
//          else
//          {
//            if (F_OptionVoiceUse)
//            {
//              F_OptionVoiceUse = CLEAR;
//              EepromByteWrite(EEPROM_ADDR_OPTION_VOICE_USE, 0x00);
//              F_WifiStatusChange = SET;
//            }
//          }
//          break;

        case WIFI_FUNC_0053_FILTER_RESET1 :
          break;

        case WIFI_FUNC_0055_CUP_SENSING_SENSITIVITY :
          if (gu8CupSensorSensitivity != (U8)(mData))
          {
            gu8CupSensorSensitivity = (U8)(mData);
            EepromByteWrite(EEPROM_ADDR_CUP_SENSITIVITY, gu8CupSensorSensitivity);
          }
          break;

        default:
          break;
    }
}


#endif

