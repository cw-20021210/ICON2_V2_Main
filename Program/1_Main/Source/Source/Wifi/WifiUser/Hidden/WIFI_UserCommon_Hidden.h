
#ifndef _WIFI_COMMON_HIDDEN_H_
#define _WIFI_COMMON_HIDDEN_H_

#ifdef HIDDEN_WIDTH

/***** User headers ****************************************************************/
//#include "r_cg_macrodriver.h"
//#include "r_cg_serial.h"
//#include "WIFI_DefUser.h"
#include "INCLUDE.h"

/***** Type Define *****************************************************************/
typedef unsigned char       U8;
typedef signed char         I8;
typedef unsigned int        U16;
typedef signed int          I16;
typedef const signed char   CI8;
typedef const signed int    CI16;
typedef unsigned long int   U32;
typedef float               F32;
typedef double              D64;

/***** library headers ********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

/********** HIDDEN **********/
#define MODEL_NO_4
#define WIFI_VERSION        "V0.4.3.1"
#define MODEL_NAME_CHP      "CHP-8200N"
#define MODEL_NAME_CP       "CP-8200N"
#define MODEL_NAME_CHP_1    "CHP-8210N"
#define MODEL_NAME_CP_1     "CP-8210N"

// USER WIFI Port & Register
#define R_TXD           TXD1
#define R_RXD           RXD1
#define R_STIF          STIF1
#define P_WIFI_ON       P14.5
#define P_WIFI_RSTN     P0.0

// USER Define (Number of User Data)
#define USER_DATA_MAX_NUM   30  // 사용하는 데이터 총 개수
#define USER_INFO_MAX_NUM   10  // 사용하는 정보 총 개수
//#define USER_USE_ACCUMULATE   // 누적데이터 사용여부
#define USER_USE_INFO           // 정보데이터 사용여부

// USER Define (RomSize/ProductNum)
#define FOTA            1           // 0:Unused / 1:Used Fota
#define FOTA_SIZE       "513"       // FotaSize = Rom Size * 4 + 1
#define FOTA_TYPE_CHP   "113264"    // Product Code Number
#define FOTA_TYPE_CP    "113265"    // Product Code Number
#define FOTA_TYPE_CHP_1 "113267"    // Product Code Number
#define FOTA_TYPE_CP_1  "113268"    // Product Code Number

// USER Define (EEPROM Address)
#define EEPROM_ADDR_WIFI_POWER       0x00E0   // WIFI On/Off State
#define EEPROM_ADDR_FIRST_PARING     0x00E1   // WIFI First Paring Check
#define EEPROM_ADDR_FOTA_MODULE      0x00E2   // WIFI Module Fota State
#define EEPROM_ADDR_FOTA_MCU         0x00E3   // WIFI MCU Fota State

//#define EEPROM_ADDR_WIFI_POWER      0xA0   // WIFI On/Off Status
//#define EEPROM_ADDR_FIRST_PARING    0xA1   // WIFI First Paring Status
//#define EEPROM_ADDR_FOTA_MODULE     0xA2   // WIFI Module Fota Status
//#define EEPROM_ADDR_FOTA_MCU        0xA3   // WIFI MCU Fota Status

// USER Define (Buzzer)
#define	WIFI_BUZZER_SELECT          0x1F // 버튼선택음(띵)
#define	WIFI_BUZZER_AP_CONNECT      0x09 // 와이파이 연결을 시작합니다.
#define	WIFI_BUZZER_SETUP           0x09 // 와이파이 연결을 시작합니다.
#define	WIFI_BUZZER_SERVER_CONNECT  0x0A // 와이파이 연결이 완료되었습니다.
#define	WIFI_BUZZER_FAIL            0x0B // 와이파이 연결이 다시 시도해주세요.
#define	WIFI_BUZZER_CANCEL          0x0C // 와이파이 연결이 해제되었습니다.
#define WIFI_BUZZER_AP_START        0x21 // 설정( 띵동 ↗)
#define	WIFI_BUZZER_ERROR           0x20 // 불가음(띵깡띵깡)
#endif

#endif

