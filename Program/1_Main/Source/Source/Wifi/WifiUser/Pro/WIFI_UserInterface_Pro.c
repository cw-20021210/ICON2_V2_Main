
#include "WIFI_Common.h"
#include "WIFI_UserInterface.h"
#include "WIFI_SetFunctionData.h"
#include "WIFI_SetSensorData.h"
#include "WIFI_SetErrorData.h"
#include "WIFI_SetPartData.h"
#include "WIFI_SetAccumulateData.h"

#ifdef MODEL_NO_0



void UserWriteEep ( U16 mu8Addr, U8 mu8Val )
{

}

U8 UserReadEep ( U16 mu8Addr )
{
    U8 mu8Data = 0U;

    return mu8Data;
}

void UserBuzzSound ( U8 mu8Buzz )
{

}

U8 GetUserSystemStatus ( U8 mu8Value )
{
    U8 mu8Return = 0;
    
    return mu8Return;
}

/* User Error Data */
U16 GetUserSystemError ( U16 mu16Err )
{
    U16 mu16Data = 0U;
  
    switch ( mu16Err ) // 0:Normal / 1:Error
    {
        case WIFI_ERROR_0009_NATURAL_TEMP :
          
        break;
        
        case WIFI_ERROR_0016_COLD_LEVEL :
            
        break;
        
        case WIFI_ERROR_0019_STOP_WATER :
            
        break;

        case WIFI_ERROR_001B_STOP_HOTWATER :
            
        break;

        case WIFI_ERROR_001C_LEAK_FLOW :
            
        break;

        case WIFI_ERROR_0024_COLD_TEMP_UP :
            
        break;

        case WIFI_ERROR_0025_COLD_TEMP_DOWN :
            
        break;

        case WIFI_ERROR_0026_HOT_TANK_TEMP :
            
        break;

        case WIFI_ERROR_0027_HOT_LEVEL :
            
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
            if(F_FuncSet_Cold)
            { 
                mu16Data =  1;
            }
            else
            {
                mu16Data =  0;
            }
        break;

        case WIFI_FUNC_0003_HOT_LOCK_SEL : // 1 / 2 (H)
            
        break;

        case WIFI_FUNC_0005_LOCK_SEL : // 0 / 1
            
        break;

        case WIFI_FUNC_000A_COLD_TEMP : // 0 ~ 3 / 65535(ERR)

        break;
        
        case WIFI_FUNC_000B_WATER_SEL : // 1(A) / 2(C) / 3(H)

        break;

        case WIFI_FUNC_000D_WATER_OUTQUANTITY : // 0 ~ 65535 cc
            
        break;

        case WIFI_FUNC_001A_HOT_INTEMP :
            
        break;

        case WIFI_FUNC_001B_HOT_MEANTEMP :
            
        break;

        case WIFI_FUNC_001C_HOT_SELTEMP :
            
        break;

        case WIFI_FUNC_001F_DRAIN_STATUS :
            
        break;
        
        case WIFI_FUNC_0024_HEART_TIME4 : // 0 ~ 65535 sec
            
        break;

        case WIFI_FUNC_002C_SILVER_CARE :
            
        break;

        case WIFI_FUNC_0031_VOICE_SEL :
            
        break;

        case WIFI_FUNC_0033_VOICE_VOL_SEL :
            
        break;

        case WIFI_FUNC_0035_COVER1_OPEN :
            
        break;

        case WIFI_FUNC_0036_COVER2_OPEN :
            
        break;

        case WIFI_FUNC_003C_COLD_STRENGTH_SEL :
            
        break;

        case WIFI_FUNC_0040_SMART_CHECK_RQST :
            
        break;

        case WIFI_FUNC_0041_SMART_CHECK_STATUS :
            
        break;

        case WIFI_FUNC_0042_SMART_CHECK_PROGRESS :
            
        break;

        case WIFI_FUNC_0043_FILTER_STATUS1 :
            
        break;
        
        case WIFI_FUNC_0047_WATER_QUANTITY_SEL :
            
        break;

        case WIFI_FUNC_004A_WELCOME_LIGHT :
            
        break;

        case WIFI_FUNC_004B_HIDDEN_LCD_SEL :
            
        break;

        case WIFI_FUNC_004C_FAUCET_MOVE_SEL :
            
        break;

        case WIFI_FUNC_004D_FAUCET_MOVE_STATUS :
            
        break;

        case WIFI_FUNC_004E_CUP_SENSING_SEL :
            
        break;

        
        case WIFI_FUNC_0050_UV_STERILIZE_STATUS :
            
        break;

        case WIFI_FUNC_0051_QUANTITY_USE_SEL :
            
        break;

        case WIFI_FUNC_0052_VOICE_MODE_SEL :
            
        break;

        case WIFI_FUNC_0053_FILTER_RESET1 :
            
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
        case WIFI_SENSOR_0001_COLD_TEMP :
            mu16Data = u8Cold_Temp;
            if ( mu16Data >= 99 )
            {
                mu16Data = 99;
            }
        break;
            
        case WIFI_SENSOR_0004_OUT_TEMP : //  0 ~ 99

        break;

        case WIFI_SENSOR_000E_WATT_DATA : // 0.01~250.00 (ex -> 1.78 = 178)

        break;

        case WIFI_SENSOR_0010_TEM_TEMP : // 0~99

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
    U16 mu16Data = 9999U;
    /*switch ( mu16Part )
    {
        case WIFI_SENSOR_0002_HOT_TEMP : // 0 ~ 99

        break;
        
        default:
          mu16Data = 0U;
        break;

    WIFI_PART_0000_VALVE_FEED = 0x0000,
    WIFI_PART_0001_VALVE_NOS,
    WIFI_PART_0002_VALVE_PURE_OUT,
    WIFI_PART_0003_VALVE_COLD_OUT,
    WIFI_PART_0004_VALVE_COLD_IN,
    WIFI_PART_0005_VALVE_COLD_DRAIN,
    WIFI_PART_0006_VALVE_HOT_OUT,
    WIFI_PART_0007_VALVE_HOT_IN,
    WIFI_PART_0008_VALVE_HOT_DRAIN,
    WIFI_PART_0009_VALVE_ICE_TRAY_IN,
    WIFI_PART_000A_VALVE_STERILIZE,
    WIFI_PART_000B_VALVE_PURE_IN,
    WIFI_PART_000C_VALVE_CHP_OUT,
    WIFI_PART_000D_VALVE_DRAIN_NOS,
    WIFI_PART_000E_VALVE_ICE_STORAGE_IN,
    WIFI_PART_000F_VALVE_HOT_AIR,
    WIFI_PART_0010_VALVE_COLD_AIR,
    WIFI_PART_0011_VALVE_ION_CHANGE,
    WIFI_PART_0012_VALVE_INNO_CENCE,
    WIFI_PART_0013_VALVE_PRESSURE_IN,
    WIFI_PART_0014_VALVE_PRESSURE_OUT,
    WIFI_PART_0015_VALVE_CP_OUT,

    WIFI_PART_0100_LEVEL_COLD_OVER = 0x0100,
    WIFI_PART_0101_LEVEL_COLD_HIGH,
    WIFI_PART_0102_LEVEL_COLD_MID,
    WIFI_PART_0103_LEVEL_COLD_LOW,
    WIFI_PART_0104_LEVEL_PURE_OVER,
    WIFI_PART_0105_LEVEL_PURE_HIGH,
    WIFI_PART_0106_LEVEL_PURE_MID,
    WIFI_PART_0107_LEVEL_PURE_LOW,
    WIFI_PART_0108_LEVEL_HOT_HIGH,
    WIFI_PART_0109_LEVEL_ICE_STORAGE_HIGH,
    
    WIFI_PART_0200_COLD_COMP = 0x0200,
    WIFI_PART_0201_COLD_BLDC,
    WIFI_PART_0202_COLD_TEM,
    WIFI_PART_0203_COLD_TEM_SENSOR,
    WIFI_PART_0204_COLD_FAN,
    WIFI_PART_0205_COLD_STIRRING_MOTOR,
    WIFI_PART_0206_COLD_SENSOR_1,
    WIFI_PART_0207_COLD_SENSOR_2,
    WIFI_PART_0208_COLD_ROOM_SENSOR,
    
    WIFI_PART_0300_HOT_TANK_HEATER = 0x0300,
    WIFI_PART_0301_HOT_SENSOR,
    WIFI_PART_0302_HOT_INSTANT_HEATER_1,
    WIFI_PART_0303_HOT_INSTANT_HEATER_2,
    WIFI_PART_0304_HOT_FLOW_CONTROL_MOTOR,
    WIFI_PART_0305_HOT_SNESOR_IN,
    WIFI_PART_0306_HOT_SNESOR_OUT,
    WIFI_PART_0307_HOT_HEATER_SNESOR,

    WIFI_PART_0400_SENSOR_LEAK = 0x0400,
    WIFI_PART_0401_SENSOR_FLOW,
    WIFI_PART_0402_SENSOR_PRESSURE_1,
    WIFI_PART_0403_SENSOR_PRESSURE_2,
    WIFI_PART_0404_SENSOR_FILTER_REED,
    WIFI_PART_0405_SENSOR_TOP_REED,
    WIFI_PART_0406_SENSOR_FRONT_REED,
    WIFI_PART_0407_SENSOR_FILTER_SW_1,
    WIFI_PART_0408_SENSOR_FILTER_SW_2,
    
    WIFI_PART_0500_STER_CSI_MODULE = 0x0500,
    WIFI_PART_0501_STER_UV_FAUCET,
    WIFI_PART_0502_STER_UV_ICE_TANK,
    WIFI_PART_0503_STER_UV_ICE_TRAY,
    
    WIFI_PART_0600_TDS_TEMP_IN = 0x0600,
    WIFI_PART_0601_TDS_TEMP_OUT,
    WIFI_PART_0602_TDS_SENSOR_IN,
    WIFI_PART_0603_TDS_SENSOR_OUT,
    
    WIFI_PART_0700_FAUCET_LOCATE_SENSING_REED_1 = 0x0700,
    WIFI_PART_0701_FAUCET_LOCATE_SENSING_REED_2,
    WIFI_PART_0702_FAUCET_CUP_SENSING_SENSOR,
    WIFI_PART_0703_FAUCET_MOTOR,
    }*/
    return mu16Data;
}
    

U32 GetUserSystemAccumulate ( U16 mu16Acc )
{
    U32 mu32Data = 0U;

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
            if ( mData == ON )      // ON
            {
                Play_Voice(VOICE_BEEP_ON);
            }
            else
            {
                Play_Voice(VOICE_BEEP_OFF);
            }
        break;

        case WIFI_FUNC_0003_HOT_LOCK_SEL :
           
        break;

        case WIFI_FUNC_0005_LOCK_SEL :
            
        break;

        case WIFI_FUNC_0031_VOICE_SEL :
           
        break;

        case WIFI_FUNC_0033_VOICE_VOL_SEL :

        break;
        
        case WIFI_FUNC_0040_SMART_CHECK_RQST :

        break;

        case WIFI_FUNC_0047_WATER_QUANTITY_SEL :

        break;

        case WIFI_FUNC_004A_WELCOME_LIGHT :

        break;

        case WIFI_FUNC_004B_HIDDEN_LCD_SEL :

        break;

        case WIFI_FUNC_004C_FAUCET_MOVE_SEL :

        break;

        case WIFI_FUNC_004E_CUP_SENSING_SEL :

        break;

        case WIFI_FUNC_0051_QUANTITY_USE_SEL :

        break;

        case WIFI_FUNC_0052_VOICE_MODE_SEL :

        break;

        default:
        
        break;
    }
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
    
}

#endif


