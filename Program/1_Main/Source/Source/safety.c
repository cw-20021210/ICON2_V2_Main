/// @file     safety.c
/// @date     2019.07.10
/// @author   PSM
/// @brief    안전 처리 관련 file
#define SAFETY_C
#include "include/_include.h"

/// @brief    안전처리 초기화
/// @param    void
/// @return   void
void Safety_Initialize(void)
{
	gu8_Counter_MainLoopCheck = 0xFF;
}

/// @brief    안전처리
/// @param    void
/// @return   void
void Safety(void)
{
	gu8_Counter_MainLoopCheck = 0xFF;				//	Main Loop 감시 Reset

	gu8_Counter_1msTimerCheck--;

	if (gu8_Counter_1msTimerCheck == 0)				//	타이머 인터럽트 발생이 없으면
	{
		SafetyLoop();								//		Reset
	}
}

/// @brief    WatchDog Timer
/// @param    void
/// @return   void
void WatchdogClear(void)
{
	WDTE = 0xACU;
}

/// @brief    안전정지 (MCU Reset)
/// @param    void
/// @return   void
void SafetyLoop(void)
{
	WDTE = 0xFFU;
}
