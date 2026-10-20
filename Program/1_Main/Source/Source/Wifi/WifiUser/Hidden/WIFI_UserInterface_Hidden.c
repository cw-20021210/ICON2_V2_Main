
#include "WIFI_Common.h"
#include "WIFI_SetFunctionData.h"
#include "WIFI_SetSensorData.h"
#include "WIFI_SetErrorData.h"
#include "WIFI_SetPartData.h"
#include "WIFI_SetAccumulateData.h"


#ifdef MODEL_NO_4

U8 u8FirstSilverCare = 0;


void UserWriteEep ( U16 mu8Addr, U8 mu8Val )
{
    EepromByteWrite(mu8Addr,mu8Val);
}

U8 UserReadEep ( U16 mu8Addr )
{
    U8 mu8Data = 0U;

    EepromSeqRead(mu8Addr, gu8_eeprom_rbuf, 1);
    mu8Data = gu8_eeprom_rbuf[0];

    return mu8Data;
}

void UserBuzzSound ( U8 mu8Buzz )
{
    Play_Voice(mu8Buzz);
}
//==============================================================================
//                           System Status
//==============================================================================
U8 GetUserSystemStatus ( U8 mu8Value )
{
    U8 mu8Return = 0;
    
    if ( mu8Value == USER_MODEL_TYPE )
    {
        if ( F_SPECIFIC_CHP == SET )
        {
            mu8Return = USER_MODEL_CHP;
        }
        else
        {
            mu8Return = USER_MODEL_CP;
        }
    }
    else if ( mu8Value == USER_FACTORY_STATUS ) 
    {
        if ( F_AT_TEST == SET || F_PCB_TEST == SET )
        {
            mu8Return = SET;
        }
    }
// ★ 이거 추가해야함(어플 동작금지 상황)
    else if ( mu8Value == USER_RX_STATUS )
    {
      if ((F_EffluentStatus)           // 추출 중?
         || (F_memento)               // 메멘토?
         || (F_FilterFushing)        // 플러싱중?
         || (F_Pipe_Clean_Service)    // UV 살균중
         || (F_AT_TEST)               // 공장 자동화검사
         || (F_PCB_TEST))             // PBA 검사            
    	{
    	    mu8Return = FALSE; // 제어불가
    	}
      else
      {
          mu8Return = TRUE; // 제어가능
      }
    }
    else if ( mu8Value == USER_ERROR_STATUS )
    {
        if ( F_ERROR_STATE == SET ) // 에러중(에러표시중)?
        {
            mu8Return = TRUE;   // 에러
        }
        else
        {
            mu8Return = FALSE;  // 정상
        }
    }
// ★ 이거 추가해야함(FOTA 동작금지 상황)
    else if ( mu8Value == USER_FOTA_STATUS )
    {
      if ((F_EffluentStatus)           // 추출 중?
         || (F_memento)               // 메멘토?
         || (F_FilterFushing)        // 플러싱중?
         || (F_Pipe_Clean_Service)    // UV 살균중
         || (F_AT_TEST)               // 공장 자동화검사
         || (F_PCB_TEST))             // PBA 검사            
    	{
            mu8Return = FALSE; // Fota 불가 (제품동작중)
    	}
      else
      {
            mu8Return = TRUE; // Fota 가능
      }
    }
    return mu8Return;
}
//==============================================================================
//                           Error Data
//==============================================================================
/* User Error Data */
U16 GetUserSystemError ( U16 mu16Err )
{
    U16 mu16Data = 0U;
  
    switch ( mu16Err ) // 0:Normal / 1:Error
    {
        case WIFI_ERROR_0006_HOTIN_TEMP :
          mu16Data = F_HOT_IN_TH_ERROR;
        break;
        
        case WIFI_ERROR_0007_HOTOUT_TEMP :
          mu16Data = F_HOT_OUT_TH_ERROR;
        break;
        
        case WIFI_ERROR_0008_COLD_TEMP :
          mu16Data = F_COLD_TH_ERROR;
        break;        

        case WIFI_ERROR_0009_NATURAL_TEMP :
          mu16Data = F_AIR_TH_ERROR;
        
        break;
        
        case WIFI_ERROR_0018_TEM_TEMP :
          mu16Data = F_TEM_TH_ERROR;
        break;

        case WIFI_ERROR_0019_STOP_WATER :
          mu16Data = F_WATER_CUTOFF_ERROR;
        break;

        case WIFI_ERROR_001A_TEM_OPERATION :
          if ( F_TEM_OVER_HEAT_ERROR == SET || F_TEM_FAIL_ERROR == SET )
          {
            mu16Data = SET;
          }
        break;

        case WIFI_ERROR_001B_STOP_HOTWATER :
          mu16Data = F_HOT_BLOCKAGE_ERROR;
        break;

        case WIFI_ERROR_001C_LEAK_FLOW :
          mu16Data = F_WATER_LEAK_ERROR;
        break;

        case WIFI_ERROR_001D_LEAK_SENSOR :
          mu16Data = F_SENSOR_LEAK_ERROR;
        break;

        case WIFI_ERROR_001E_HOT_OVER :
          mu16Data = F_OVER_HEAT_ERROR;
        break;

        case WIFI_ERROR_001F_HEATER_SENSOR :
          mu16Data = F_HOT_HEATER_TH_ERROR;
        break;

        default:
            mu16Data = 0U;
        break;    
    }
    return mu16Data;
}

//==============================================================================
//                           Function Data
//==============================================================================
/* User Function Data */
// 상태값
U16 GetUserSystemFunction ( U16 mu16Func )
{
    U16 mu16Data = 0U;

    switch ( mu16Func )
    {   
        //---------------------
    	  // 냉수설정 (ID 0x0002)     
        //---------------------
        case WIFI_FUNC_0002_COLD_SEL : // (0:OFF / 1:ON)
            if(F_FuncSet_Cold)
            { 
                mu16Data =  1;
            }
            else
            {
                mu16Data =  0;
            }

        break;

        //--------------------------
    	  // 온수잠금설정 (ID 0x0003)
        //--------------------------
        case WIFI_FUNC_0003_HOT_LOCK_SEL : // (1:온수잠금 해제 / 2:온수잠금 설정)
            if(F_FuncSet_HotLock)
            { 
                mu16Data =  2;
            }
            else
            {
                mu16Data =  1;
            }            
        break;

        //---------------------
    	  // 잠금설정 (ID 0x0005)
        //---------------------
        case WIFI_FUNC_0005_LOCK_SEL : // (0:전체잠금 해제 / 1: 전체잠금 설정)
            if(F_FuncSet_AllLock)
            { 
                mu16Data =  1;
            }
            else
            {
                mu16Data =  0;
            }            
        break;

        //------------------------------
    	  // 냉수온도표시확인 (ID 0x000A)
        //------------------------------
        case WIFI_FUNC_000A_COLD_TEMP : // 0 ~ 3
            mu16Data = gu8DSP_Cold_Lv;  // (0: OFF /1: 1단white 6.5℃이하 /2: 10℃이상 blue)
        break;
        
        //---------------------
    	  // 물선택 (ID 0x000B)
        //---------------------
        case WIFI_FUNC_000B_WATER_SEL : // 1(A) / 2(C) / 3(H)
            if(gu8WaterSelect == 0)
            {
                mu16Data = 2;                // 냉수
            }
            else if(gu8WaterSelect == 1)
            {
                mu16Data = 1;                // 정수
            }
            else if(gu8WaterSelect == 2)
            {
                mu16Data = 3;               // 온수
            }
            else
            {
                mu16Data = 1;
            }
        break;

        //-----------------------
    	  // 추출 물량 (ID 0x000D)
        //-----------------------
        case WIFI_FUNC_000D_WATER_OUTQUANTITY : // 0 ~ 65535 cc
            // 물량(우선 선택 정량 -> 도중 취소시 올림처리, 연속추출은 반컵단위 적산) 2byte 10cc -> 10
            if ( F_EffluentStatus )
            {
                mu16Data = 0;
            }
            else
            {
                mu16Data = gu16_10CC_Count*10;
                gu16_10CC_Count = 0;
            }            
        break;
        
        //------------------------------
    	  // 순간온수 원수온도(ID 0x001A)
        //------------------------------
        case WIFI_FUNC_001A_HOT_INTEMP : // 0 ~ 99 (온수 아닐때는 0)
             mu16Data =  u8InHot_Temp;          // 0~99           
        break;

        //------------------------------
    	  // 순간온수 평균온도(ID 0x001B)
        //------------------------------
        case WIFI_FUNC_001B_HOT_MEANTEMP : // 0 ~ 99
             mu16Data = u8OutHot_Temp_AVG;           // 온수 출수 평균온도 (0~99) -> ?테 이후 적용            
        break;

        //------------------------------
    	  // 순간온수 설정온도(ID 0x001C)
        //------------------------------
        case WIFI_FUNC_001C_HOT_SELTEMP : // 0 ~ 99
             if(gu8WaterSelect == W_HOT)
             {
               if(gu8Hot_Type == H_COFFEE)    {mu16Data = 85;}
               else if(gu8Hot_Type == H_TEA)  {mu16Data = 70;}
               else if(gu8Hot_Type == H_MILK) {mu16Data = 45;}
             }
             else
             {
                mu16Data = 0;
             }
            
        break;

        //----------------------
    	  // 배수 동작(ID 0x001F)
        //----------------------
        case WIFI_FUNC_001F_DRAIN_STATUS : // 0 ~ 2 (대기0, 시작1, 동작중2)
             if(F_AutoDrain)
             {
               if(gu8AutoDrain_Step > 0x00 && gu8AutoDrain_Step < 0x40)    {mu16Data = 1;}
               else if(gu8AutoDrain_Step == 0x40)                          {mu16Data = 2;}
             }
             else
             {
                mu16Data = 0;
             }         
        break;
        //---------------------------
    	  // 하트서비스시간(ID 0x0024)
        //---------------------------
        case WIFI_FUNC_0024_HEART_TIME4 : // 0 ~ 65535 sec
             
             mu16Data = gu16Cody_Care_Time;         // 서비스 처리시간(탑커버 or 필터커버 열려있는 시간)
             if ( mu16Data >= 65535 )
             {
                mu16Data = 65535;
             }            
        break;

        //---------------------------
    	  // 필터1 교환알림(ID 0x0025)
        //---------------------------
//// ★ 이거 추가해야함(필터교체+n일 알람) => 삭제
//        case WIFI_FUNC_0025_FILTER_NOTICE1 : // 0 / 1
//           
//        break;

        //---------------------
    	  // 실버케어(ID 0x002C)
        //---------------------
        case WIFI_FUNC_002C_SILVER_CARE : // 0 ~ 2              // 48시간 미사용 (0미경과 1:경과시)

            if(!gu8Silver_Care_Check_Time)
            {
                if ( u8FirstSilverCare == CLEAR )
                {
                    u8FirstSilverCare = SET;
                    mu16Data = 1;
                }
                else
                {
                    mu16Data = 2;
                }
            }
            else
            {
                u8FirstSilverCare = CLEAR;
                mu16Data = 0;
            }           
        break;

        //--------------------------
    	  // 음성안내 설정(ID 0x0031)
        //--------------------------
        case WIFI_FUNC_0031_VOICE_SEL : // 1 ~ 3
             mu16Data = (U16)gu8Sound_Type;            
        break;

        //--------------------------
    	  // 음성볼륨 설정(ID 0x0033)
        //--------------------------
        case WIFI_FUNC_0033_VOICE_VOL_SEL : // 1 ~ 5
             mu16Data = (U16)(gu8VoiceVolumeLevel+1);            
        break;

        //-----------------------
    	  // 커버1 열림(ID 0x0035)
        //-----------------------
        case WIFI_FUNC_0035_COVER1_OPEN : // 0 / 1
             if(F_TopCover_Open)
             {
                 mu16Data = 1;
             }
             else
             {
                 gu16Cody_Care_Time = 0;
                 mu16Data = 0;
             }            
        break;
        //-----------------------
    	  // 커버2 열림(ID 0x0036)
        //-----------------------
        case WIFI_FUNC_0036_COVER2_OPEN : // 0 / 1
             if(F_FrontCover_Open)
             {
                 mu16Data = 1;
             }
             else
             {
                 gu16Cody_Care_Time = 0;
                 mu16Data = 0;
             }            
        break;

        //-------------------------
    	  // 냉수상세설정(ID 0x003C)
        //-------------------------
        case WIFI_FUNC_003C_COLD_STRENGTH_SEL : // 0(강) / 1(약)
             if(gu8Set_Cold_Level == 0)
             {
                 mu16Data = 0;                  // 강
             }
             else
             {
                 mu16Data = 1;                  // 약
             }            
        break;

        //---------------------------------
    	  // 스마트진단 모드 설정(ID 0x0040)
        //---------------------------------
        // (평소에는 0, 고장진단 완료후 한번만 1)
        case WIFI_FUNC_0040_SMART_CHECK_RQST : // 0 / 1
             
             if(F_Parts_Check_Complete)
             {
             	  F_Parts_Check_Complete = 0;
             	  gu16Smart_IoCare_Pecent = 0;
                mu16Data = 1;              
             }
             else
             {
                mu16Data = 0;            
             }
        break;


        //----------------------------
    	  // 스마트진단 상태(ID 0x0041)
        //----------------------------
        case WIFI_FUNC_0041_SMART_CHECK_STATUS : // 0 ~ 5
             if((F_EffluentStatus)           // 추출 중?
                || (F_memento)                // 메멘토?
                || (F_FilterFushing)          // 플러싱중?
                || (F_Pipe_Clean_Service)     // 유로 살균중
                || (F_AT_TEST)                // 공장 자동화검사
                || (F_PCB_TEST))              // PBA 검사
             {
               mu16Data = 5;                      // 진단 불가
             }
             else if(!F_Smart_Check_Request)
             {
               mu16Data = 0;                      // 미진단
             }             	  
             else if(F_Parts_Check_Start)
             {
             	 F_Parts_Check_Start = 0;
               mu16Data = 1;                      // 정밀진단 시작
             }
             else if(F_Smart_Check_Request && gu16Parts_Check_Step < 0xFFFF)
             {
             	 F_Parts_Check_Start = 0;
               mu16Data = 2;                      // 정밀진단 중             
             }
             else if(gu16Parts_Check_Step == 0xFFFF)
             {
               mu16Data = 3;                      // 정밀진단 완료             
             }
             else
             {
               mu16Data = 4;                      // 정밀진단 중지              
             }          
        break;

        //-----------------------------
    	  // 스마트진단 진행률(ID 0x0042)
        //-----------------------------
        //(고장진단 진행율 25%50%75% 100%일때는 위에 완료꺼랑 동일화)
        case WIFI_FUNC_0042_SMART_CHECK_PROGRESS : // 0 ~ 100 %
             mu16Data = gu16Smart_IoCare_Pecent;              
        break;

        //---------------------------
    	  // 필터1 장착상태(ID 0x0043)
        //---------------------------
        //(0 장착 1분리)
        case WIFI_FUNC_0043_FILTER_STATUS1 : // 0 장착 1분리
             if(F_Filter_DisConnected)
             {
               mu16Data = 1;             
             }
             else
             {
               mu16Data = 0;
             }
        break;

        //---------------------------
    	  // 맞춤추출량(ID 0x0047)
        //---------------------------
        case WIFI_FUNC_0047_WATER_QUANTITY_SEL : // 130 ~ 1000 ml
             if(F_FuncSet_User_Cup)
             { 
               mu16Data = gu16User_Cup_Size;
             }
             else
             {
               mu16Data = 550;
             }
        break;

        //---------------------------
    	  // 웰컴라이팅(ID 0x004A)
        //---------------------------
        case WIFI_FUNC_004A_WELCOME_LIGHT : // 0 / 1
             if(F_FuncSet_Welcomelight == 1)
             {
                 mu16Data = 1;
             }
             else
             {
                 mu16Data = 0;
             }              
        break;
        //---------------------------
    	  // 히든화면설정(ID 0x004B)
        //---------------------------
        case WIFI_FUNC_004B_HIDDEN_LCD_SEL : // 0 ~ 2
             if(gu16Display_Theme == 3)      {mu16Data = 3;} // 날씨
             else if(gu16Display_Theme == 1) {mu16Data = 1;} // 시계
             else                            {mu16Data = 0;} // 기본
        break;
        //--------------------------------------
    	  // 파우셋 무빙(출시대기)설정(ID 0x004C)
        //--------------------------------------
        case WIFI_FUNC_004C_FAUCET_MOVE_SEL : // 0 3 5 10 30 min
             if(gu8Set_MoveOut_Level==0)      {mu16Data = 5;} // 5분
             else if(gu8Set_MoveOut_Level==1) {mu16Data = 10;} // 10분
             else if(gu8Set_MoveOut_Level==2) {mu16Data = 20;} // 20분
             else if(gu8Set_MoveOut_Level==3) {mu16Data = 0;} // 고정
             else if(gu8Set_MoveOut_Level==4) {mu16Data = 1;} // 1분            
             else if(gu8Set_MoveOut_Level==5) {mu16Data = 3;} // 3분   
        break;

        //----------------------------
    	  // 파우셋 무빙 상태(ID 0x004D)
        //----------------------------
        case WIFI_FUNC_004D_FAUCET_MOVE_STATUS : // 0오픈 / 1히든
             if(gu8Faucet_State == 1)
             {
                 mu16Data = 0;
             }
             else
             {
                 mu16Data = 1;
             }             
        break;
        //------------------------
    	  // 컵센싱 설정(ID 0x004E)
        //------------------------
        case WIFI_FUNC_004E_CUP_SENSING_SEL : // 0 OFF / 1 ON
             if(F_FuncSet_CupSensing == 0)
             {
                 mu16Data = 0;
             }
             else
             {
                 mu16Data = 1;
             }                         
        break;

        //-----------------------------
    	  // UV 살균 동작 상태(ID 0x0050)
        //-----------------------------
        case WIFI_FUNC_0050_UV_STERILIZE_STATUS : // 0 ~ 2
            // 0:살균Xor정지 / 1:살균시작 / 2:살균중
            if ( gu8UV_Type == 0x02 )
            {
                if ( F_UV_Clean_Pause == SET )
                {
                    mu16Data = 0;
                }
                else
                {
                	  // OFF
                    if(gu8UV_Step == 0x00)
                    {
                      mu16Data = 0;
                    }
                    // ON
                    else if((gu8UV_Step == 0x20) && (gu16UV_Step_Time >= 8970))
                    {
                      mu16Data = 1;
                    }
                    // 진행중
                    else if((gu8UV_Step == 0x20) && (gu16UV_Step_Time < 8970))
                    {
                      mu16Data = 2;
                    }
                    else
                    {
                      mu16Data = 0;
                    }
                }       
            }
            else
            {
                mu16Data = 0;
            }            
        break;
        //---------------------------------
    	  // 맞춤 추출량 사용여부(ID 0x0051)
        //---------------------------------
        case WIFI_FUNC_0051_QUANTITY_USE_SEL : // 0 / 1
             if(F_FuncSet_User_Cup)
             {
                 mu16Data = 1;
             }
             else
             {
                 mu16Data = 0;
             }
        break;

        //----------------------
    	  // 음성 모드(ID 0x0052)
        //----------------------
        case WIFI_FUNC_0052_VOICE_MODE_SEL : // 0 / 1
             if(gu8Voice_Actor == VOICE_ACTOR)
             {
                 mu16Data = 0;
             }
             else
             {
                 mu16Data = 1;
             }            
        break;

        //----------------------
    	  // 필터1 리셋(ID 0x0053)
        //----------------------
        case WIFI_FUNC_0053_FILTER_RESET1 :       // 0 / 1리셋
             if(F_Filter_Reset_Report)
             {
             	   F_Filter_Reset_Report = 0;
                 mu16Data = 1;
             }
             else
             {
                 mu16Data = 0;
             }            
        break;

        //-----------------------------------
    	  // 컵감지센서 민감도 설정(ID 0x0055)
        //-----------------------------------
        case WIFI_FUNC_0055_CUP_SENSING_SENSITIVITY :       // 1민감 / 2보통 / 3둔감
//             if(F_Filter_Reset_Report)
//             {
//             	   F_Filter_Reset_Report = 0;
//                 mu16Data = 1;
//             }
//             else
//             {
//                 mu16Data = 0;
//             }            
        break;


        default:
            mu16Data = 0U;
        break;
    }
    return mu16Data;
}
//==============================================================================
//                           Sensor Data
//==============================================================================
/* User Sensor Data */
// 1분단위로
U16 GetUserSystemSensor ( U16 mu16Sen )
{
    U16 mu16Data = 0U;

    switch ( mu16Sen )
    {
        //-----------
        // 냉수센서
        //-----------
        case WIFI_SENSOR_0001_COLD_TEMP : // 0 ~ 99 
            mu16Data = u8Cold_Temp;
            if ( mu16Data >= 99 )
            {
                mu16Data = 99;
            }
        break;
        //-----------
        // 외기온도
        //-----------
        case WIFI_SENSOR_0004_OUT_TEMP : //  0 ~ 99
            mu16Data = u8OutHot_Temp;
            if ( mu16Data >= 99 )
            {
                mu16Data = 99;
            }
        break;
        
//        // 전체 필터 수명(유량) 얼마나 사용했는가?
//        case WIFI_SENSOR_0006_FILTER1_DATA : // 0 ~ 100 %
//
//        break;
        //-------------
        // 전력 데이터
        //-------------
        case WIFI_SENSOR_000E_WATT_DATA : // 0.01~250.00 (ex -> 1.78 = 178)
             // 일단 아이콘 데이터
            gu16Tem_Watt = gu16Tem_Watt/36;
            
            // Heater + TEM
            gu16WattData = gu16Heater_Watt + gu16Tem_Watt;

            if(gu16WattData >= 4500)
            {
              gu16WattData = 4500; //
            }
            
            mu16Data = gu16WattData;
            
            gu16Heater_Watt = 0;
            gu16Tem_Watt = 0;
            gu16WattData = 0;
        break;
        
        //----------
        // TEM 온도
        //----------
        case WIFI_SENSOR_0010_TEM_TEMP : // 0 ~ 99
            mu16Data = u8TEM_Temp; // 테이블 값 매칭필요
            if ( mu16Data >= 99 )
            {
                mu16Data = 99;
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

    /*switch ( mu16Part )
    {
        WIFI_PART_0001_VALVE_NOS,      
        WIFI_PART_0002_VALVE_HOT_OUT,  
        WIFI_PART_0003_VALVE_PURE_OUT, 
        WIFI_PART_0005_VALVE_CP_OUT
        WIFI_PART_0009_VALVE_HOT_IN,    
        WIFI_PART_000A_VALVE_HOT_DRAIN, 
        WIFI_PART_000C_VALVE_COLD_IN,   
        WIFI_PART_0202_COLD_TEM,      
        WIFI_PART_0203_COLD_TEM_TEMP, 
        WIFI_PART_0204_COLD_FAN,      
        WIFI_PART_0206_COLD_TEMP_1
        WIFI_PART_0208_COLD_ROOM_TEMP
        WIFI_PART_0302_HOT_INSTANT_HEATER_1,   
        WIFI_PART_0303_HOT_INSTANT_HEATER_2,   
        WIFI_PART_0304_HOT_FLOW_MOTOR,         
        WIFI_PART_0305_HOT_TEMP_IN,            
        WIFI_PART_0306_HOT_TEMP_OUT,           
        WIFI_PART_0307_HOT_HEATER_TEMP,    
        WIFI_PART_0402_SENSOR_LEAK, 
        WIFI_PART_0403_SENSOR_FLOW, 
        WIFI_PART_0405_SENSOR_FILTER_REED,
        WIFI_PART_0406_SENSOR_FILTER_SW_1,
        WIFI_PART_0407_SENSOR_FILTER_SW_2,
        WIFI_PART_0501_STER_UV_FAUCET
        WIFI_PART_0700_FAUCET_REED_1,        
        WIFI_PART_0701_FAUCET_REED_2,        
        WIFI_PART_0702_FAUCET_CUP_SENSOR,    
        WIFI_PART_0703_FAUCET_MOTOR,         

        default:
          mu16Data = 0xFFFF;
        break;
    }*/
    mu16Data = 0xFFFF;
    return mu16Data;
}
//==============================================================================
//                           Function Data
//==============================================================================
/* App Control -> User Function Data */
void SetUserSystemFunction ( U16 mu16Func, U16 mData )
{
    switch ( mu16Func )
    {
        //---------------------
    	  // 냉수설정 (ID 0x0002)     
        //---------------------
        // (0:OFF / 1:ON)
        case WIFI_FUNC_0002_COLD_SEL :
            // [ON]
            if (mData == ON)
            {
              F_FuncSet_Cold = 1;
             
              // 냉수 켜짐
              gu8WaterSelect = W_COLD;        // 냉수                     
              gu8TEM_Step = 0x00;
              
              EepromByteWrite(EEPROM_ADDR1_COLD_SETTING, (U8)F_FuncSet_Cold); 
              EepromByteWrite(EEPROM_ADDR1_WATER_SELECT_DEFAULT, gu8WaterSelect);
              Play_Voice(VOICE_BEEP_ON);
            }
            // [OFF]
            else
            {
              F_FuncSet_Cold = 0;

              // 물 종류
              gu8WaterSelect_Backup=0x01;
              gu8WaterSelect=gu8WaterSelect_Backup;
                
              gu8TEM_Step = 0x00;
                
              EepromByteWrite(EEPROM_ADDR1_COLD_SETTING, (U8)F_FuncSet_Cold); 
              EepromByteWrite(EEPROM_ADDR1_WATER_SELECT_DEFAULT, gu8WaterSelect);
              Play_Voice(VOICE_BEEP_OFF);

              gu8FuncHotKeep_Time = 0;
            }
        break;

        //--------------------------
    	  // 온수잠금설정 (ID 0x0003)
        //--------------------------
         // (1:온수잠금 해제 / 2:온수잠금 설정)
        case WIFI_FUNC_0003_HOT_LOCK_SEL :
            // [ON]
            if (mData == 2)    
            {
                F_FuncSet_HotLock = 1;
                gu8FuncHotKeep_Time = 0;

                if(gu8WaterSelect == W_HOT)
                {
                   gu8WaterSelect=gu8WaterSelect_Backup;
                }
                
                EepromByteWrite(EEPROM_ADDR1_HOT_LOCK, (U8)F_FuncSet_HotLock);
                Play_Voice(VOICE_BEEP_ON);
            }
            // [OFF]
            else      
            {
                F_FuncSet_HotLock = 0;
                EepromByteWrite(EEPROM_ADDR1_HOT_LOCK, (U8)F_FuncSet_HotLock);
 
                Play_Voice(VOICE_BEEP_OFF);
            }           
        break;

        //---------------------
    	  // 잠금설정 (ID 0x0005)
        //---------------------
        // (0:전체잠금 해제 / 1: 전체잠금 설정)
        case WIFI_FUNC_0005_LOCK_SEL  :
            // [ON]
            if (mData == ON)    
            {
                F_FuncSet_AllLock = 1;

                EepromByteWrite(EEPROM_ADDR1_ALL_LOCK, (U8)F_FuncSet_AllLock);
                Play_Voice(VOICE_BEEP_ON);
            }
            // [OFF]
            else      
            {
                F_FuncSet_AllLock = 0;
                EepromByteWrite(EEPROM_ADDR1_ALL_LOCK, (U8)F_FuncSet_AllLock);
 
                Play_Voice(VOICE_BEEP_OFF);
            } 
        break;
        //--------------------------
    	  // 음성안내 설정(ID 0x0031)
        //--------------------------
        // 1 ~ 3
        case WIFI_FUNC_0031_VOICE_SEL :
            gu8Sound_Type = (U8)mData;
            
            // 무음 모드
            if(gu8Sound_Type <=1 )
            {
                gu8Sound_Type = 1;
                Play_Voice(VOICE_VOICE_SETTING_1);                    
            }
            // 효과음 모드
            else if(gu8Sound_Type == 2)
            {
                Play_Voice(VOICE_VOICE_SETTING_2);
            }
            // 음성 모드
            else if(gu8Sound_Type >= 3)
            {
                gu8Sound_Type = 3;
                Play_Voice(VOICE_VOICE_SETTING_3);
            }
            EepromByteWrite(EEPROM_ADDR2_SOUND_SETTING, gu8Sound_Type);          
        break;
        //--------------------------
    	  // 음성볼륨 설정(ID 0x0033)
        //--------------------------
        // 1 ~ 5
        case WIFI_FUNC_0033_VOICE_VOL_SEL :
            gu8VoiceVolumeLevel = (U8)(mData-1);
            if ( gu8VoiceVolumeLevel > 4 )
            {
                gu8VoiceVolumeLevel = 1;
            }
            EepromByteWrite(EEPROM_ADDR2_SOUND_SETTING, gu8VoiceVolumeLevel);
            Play_Voice(VOICE_BEEP_ON);           
        break;

        //-------------------------
    	  // 냉수상세설정(ID 0x003C)
        //-------------------------
        // 0(강) / 1(약)
        case WIFI_FUNC_003C_COLD_STRENGTH_SEL :
            // [약]
            if (mData == ON)    
            {
                gu8Set_Cold_Level = 1;

                EepromByteWrite(EEPROM_ADDR1_COLD_LEVEL, gu8Set_Cold_Level);

                Play_Voice(VOICE_BEEP_ON);
            }
            // [강]
            else      
            {
                gu8Set_Cold_Level = 0;
                EepromByteWrite(EEPROM_ADDR1_COLD_LEVEL, gu8Set_Cold_Level);
 
                Play_Voice(VOICE_BEEP_ON);
            } 
        break;
        
        //---------------------------------
    	  // 스마트진단 모드 설정(ID 0x0040)
        //---------------------------------
        // 0 / 1
        case WIFI_FUNC_0040_SMART_CHECK_RQST :
            // [ON]
            if (mData == ON)    
            {
                F_Smart_Check_Request = 1;
                Smart_IoCare_Init();
                Play_Voice(VOICE_BEEP_ON);
            }
            // [OFF]
            else      
            {
                F_Smart_Check_Request = 0;
 
                Play_Voice(VOICE_BEEP_OFF);
            } 
        break;
        //---------------------------
    	  // 맞춤추출량(ID 0x0047)
        //---------------------------
        // 130 ~ 1000 ml
        case WIFI_FUNC_0047_WATER_QUANTITY_SEL :

            gu16User_Cup_Size = mData;
            
            gu8User_Cup_Size_EEP = (U8)(gu16User_Cup_Size/10);
            EepromByteWrite(EEPROM_ADDR2_USER_CUP_SIZE, gu8User_Cup_Size_EEP);
            Play_Voice(VOICE_BEEP_ON);
        break;
        //---------------------------
    	  // 웰컴라이팅(ID 0x004A)
        //---------------------------
        case WIFI_FUNC_004A_WELCOME_LIGHT :
            // [ON]
            if (mData == ON)    
            {
                F_FuncSet_Welcomelight = 1;

                EepromByteWrite(EEPROM_ADDR2_WELCOME_LED, (U8)F_FuncSet_Welcomelight);
                Play_Voice(VOICE_BEEP_ON);
            }
            // [OFF]
            else      
            {
                F_FuncSet_Welcomelight = 0;
                EepromByteWrite(EEPROM_ADDR2_WELCOME_LED, (U8)F_FuncSet_Welcomelight);
                Play_Voice(VOICE_BEEP_OFF);
            } 
        break;
        //---------------------------
    	  // 히든화면설정(ID 0x004B)
        //---------------------------
        case WIFI_FUNC_004B_HIDDEN_LCD_SEL :
             gu16Display_Theme = mData;
             Play_Voice(VOICE_BEEP_ON);
        break;
        //--------------------------------------
    	  // 파우셋 무빙(출시대기)설정(ID 0x004C)
        //--------------------------------------
        case WIFI_FUNC_004C_FAUCET_MOVE_SEL :
            if (mData == 0)      {gu8Set_MoveOut_Level = 3;}  // 고정
            else if(mData == 1)  {gu8Set_MoveOut_Level = 4;}  // 1분
            else if(mData == 3)  {gu8Set_MoveOut_Level = 5;}  // 3분
            else if(mData == 5)  {gu8Set_MoveOut_Level = 0;}  // 5분
            else if(mData == 10) {gu8Set_MoveOut_Level = 1;}  // 10분
            else if(mData == 20) {gu8Set_MoveOut_Level = 2;}  // 20분
            
            EepromByteWrite(EEPROM_ADDR2_FAUCET_MOVE, gu8Set_MoveOut_Level);
            Play_Voice(VOICE_BEEP_ON);
        break;
        //------------------------
    	  // 컵센싱 설정(ID 0x004E)
        //------------------------
        case WIFI_FUNC_004E_CUP_SENSING_SEL :
            // [ON]
            if (mData == ON)    
            {
                F_FuncSet_CupSensing = 1;

                EepromByteWrite(EEPROM_ADDR2_CUP_SENSOR, (U8)F_FuncSet_CupSensing);
                Play_Voice(VOICE_BEEP_ON);
            }
            // [OFF]
            else      
            {
                F_FuncSet_CupSensing = 0;
                EepromByteWrite(EEPROM_ADDR2_CUP_SENSOR, (U8)F_FuncSet_CupSensing);
                Play_Voice(VOICE_BEEP_OFF);
            } 
        break;
        //---------------------------------
    	  // 맞춤 추출량 사용여부(ID 0x0051)
        //---------------------------------
        case WIFI_FUNC_0051_QUANTITY_USE_SEL :
            // [ON]
            if (mData == ON)    
            {
                F_FuncSet_User_Cup = 1;

                EepromByteWrite(EEPROM_ADDR2_USER_CUP_ENABLE, (U8)F_FuncSet_User_Cup);
                Play_Voice(VOICE_BEEP_ON);
            }
            // [OFF]
            else      
            {
                F_FuncSet_User_Cup = 0;
                EepromByteWrite(EEPROM_ADDR2_USER_CUP_ENABLE, (U8)F_FuncSet_User_Cup);
                Play_Voice(VOICE_BEEP_OFF);
            } 
        break;
        //----------------------
    	  // 음성 모드(ID 0x0052)
        //----------------------
        case WIFI_FUNC_0052_VOICE_MODE_SEL :
            // [특별]
            if (mData == ON)    
            {
                gu8Voice_Actor = VOICE_CELEB;

                EepromByteWrite(EEPROM_ADDR2_VOICE_ACTOR, gu8Voice_Actor);
                Play_Voice(VOICE_BEEP_ON);
            }
            // [성우]
            else      
            {
                gu8Voice_Actor = VOICE_ACTOR;

                EepromByteWrite(EEPROM_ADDR2_VOICE_ACTOR, gu8Voice_Actor);
                Play_Voice(VOICE_BEEP_OFF);
            } 
        break;

        case WIFI_FUNC_0055_CUP_SENSING_SENSITIVITY : // 1민감 / 2보통 /3둔감
            if (mData == 1)    
            {
                Play_Voice(VOICE_BEEP_ON);
            }
            else if (mData == 3)
            {
                Play_Voice(VOICE_BEEP_ON);
            }
            else      
            {
                Play_Voice(VOICE_BEEP_OFF);
            } 
        break;
        
        default:
        
        break;
    }
}

U32 GetUserSystemAccumulate ( U16 mu16Acc )
{
    U32 mu32Data = 0U;
    
    switch ( mu16Acc ) // 0 ~ 999999
    {
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

/* User Data Clear */
void ClearUserSystemData ( void ) 
{

}

/* User Display Setting */
void SetUserSystemDisplay ( U8 mu8Display )
{
    if ( mu8Display == USER_DISPLAY_FOTA ) // FOTA
    {

    }
    else if ( mu8Display == USER_DISPLAY_SELFCHECK ) // Request Selfcheck
    {

    }
    else
    {

    }
}

/* Time Setting */
void SetUserSystemTime ( U16 mYear, U16 mMonth, U16 mDate, U16 mHour, U16 mMin, U16 mSec, U16 mDay )
{
  gu8_rtc_time_Year = (U8)mYear;      // 0 ~ 99
  gu8_rtc_time_Month = (U8)mMonth;     // 1 ~ 12
  gu8_rtc_time_Date = (U8)mDate;      // 1 ~ 31
  gu8_rtc_time_Hour = (U8)mHour;      // 0 ~ 23
  gu8_rtc_time_Min = (U8)mMin;       // 0 ~ 59
  gu8_rtc_time_Sec = (U8)mSec;       // 0 ~ 59      
}


#endif


