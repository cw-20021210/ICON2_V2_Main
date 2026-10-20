/// @file     macro.c
/// @date     2019.07.10
/// @author   PSM
/// @brief    공용 Macro 처리 file
#define MACRO_C
#include "include/_include.h"


/// @brief    delay 함수
/// @param    mu8Time : ??
/// @return   void
void Delay(U16 mu8Time)
{
	while(--mu8Time)
	{
//		NOP();NOP();NOP();NOP();NOP();
//		NOP();NOP();NOP();NOP();NOP();
//		NOP();NOP();NOP();NOP();NOP();

		NOP();NOP();NOP();NOP();NOP();
		NOP();NOP();NOP();NOP();NOP();
		NOP();NOP();NOP();NOP();NOP();
		NOP();NOP();NOP();NOP();NOP();
		NOP();NOP();NOP();NOP();NOP();
	}
}

//void Delay(U16 time)
//{
//	while(--time) NOP();
//}

/// @brief    delay ms 함수
/// @param    mu8Time : delay 시간(ms)
/// @return   void
void Delay_ms(U16 mu8Time)
{
	U16 mu8X;
	while (mu8Time)
	{
		mu8X = 395 * 4;        /// constant value for 1msec counter
		R_WDT_Restart();
		while (mu8X)
		{
			R_WDT_Restart();
			--mu8X;
		}
		--mu8Time;
	}
}

/// @brief    Hex to Decimal
/// @param    mu8DecData : HEX Data
/// @return   mu8ReturnData : DEC Data
U8 HEX2DEC(U8 mu8HexData)
{
	U8 mu8ReturnData = 0;

	mu8ReturnData = (U8)((((mu8HexData & 0xF0) >> 4) * 10) + (mu8HexData & 0x0F));

	return mu8ReturnData;
}

/// @brief    Decimal to HEX
/// @param    mu8DecData : DEC Data
/// @return   mu8ReturnData : HEX Data
U8 DEC2HEX(U8 mu8DecData)
{
	U8 mu8ReturnData = 0;

	mu8ReturnData = (U8)((((mu8DecData / 10) << 4) & 0xF0) + (mu8DecData % 10));

	return mu8ReturnData;
}

/// @brief    ASCII CODE 역변환 (1바이트분)
/// @param    U8 *pBuffer (변환전 DATA로의 POINTER) / U8 *pByteDat (변환후 DATA 보존 RAM의 POINTER)
/// @return   0=이상종료	1=정상종료
U8 DisAsciiByte(U8 *pBuffer, U8 *pByteDat)
{
	U8 work1, work2;
	/****************************************************/
	if (DisAsciiNibble(pBuffer, &work1) == 0)
	{ /*	상위nibble data 변환인 경우*/
		return (0); /*		반환값=[0:이상종료]*/
	}
	if (DisAsciiNibble(pBuffer + 1, &work2) == 0)
	{ /*	하위nibble data 변환인 경우*/
		return (0); /*		반환값=[0:이상종료]*/
	}
	*pByteDat = (U8) ((work1 * 0x10) + work2);
	return (1); /*	반환값=[1:정상종료]*/
}

/// @brief    ASCII CODE 역변환 (NIBBLE)
/// @param    U8 *pBuffer (변환전 DATA로의 POINTER) / U8 *pNibbleDat (변환후 DATA 보존 RAM의 POINTER)
/// @return   0=이상종료	1=정상종료
U8 DisAsciiNibble(U8 *pBuffer, U8 *pNibbleDat)
{
	U8 work;
	/****************************************************/
	work = *pBuffer; /* ASCII CODE 변환대상 data*/
	if ((work < 0x30) || (work > 0x46) || ((work > 0x39) && (work < 0x41)))
	{
		/*	ASCII CODE 이상치인 경우*/
		return (0); /*		반환값=[0:이상종료]*/
	}

	if (work < 0x41)
	{ /*	0x30~0x39 인 경우*/
		*pNibbleDat = (U8) (work - 0x30); /*		0 ~ 9*/
	}
	else
	{ /*	0x41~0x46 인 경우*/
		*pNibbleDat = (U8) (work - 0x37); /*		A(10) ~ F(15)*/
	}
	return (1); /*	반환값=[1:정상종료]*/
}

/// @brief    ASCII CODE 역변환 (2바이트분)
/// @param    U8 *pBuffer (변환전 DATA의 POINTER) / U16 *pByteDat (변환후 DATA 보존 RAM의 POINTER)
/// @return   0=이상종료	1=정상종료
U8 DisAsciiWord(U8 *pBuffer, U16 *pWordDat)
{
	U8 work1, work2;
	/****************************************************/
	if (DisAsciiByte(pBuffer, &work1) == 0)
	{ /*	상위byte data 변환 이상인 경우*/
		return (0);
	}
	if (DisAsciiByte(pBuffer + 2, &work2) == 0)
	{ /*	하위byte data 변환 이상인 경우*/
		return (0);
	}
	*pWordDat = ((U16) work1 * 0x0100U) + work2;
	return (1);
}

/// @brief    ASCII 변환 (NIBBLE)
/// @param    U8 data (변환 원DATA) / U8 hilo (상위하위 지정 (0:Lo,1:Hi))
/// @return   U8 (변환후 DATA)
U8 Ascii(U8 data, U8 hilo)
{
	/****************************************************/
	if (hilo == Z_asciihi)
	{ /*	상위4비트 지정인 경우*/
		data >>= 4; /*		4Bit 우로 shift*/
	}
	data &= 0x0f; /*	하위4비트만 유효*/
	if (data < 10)
	{ /*	0 ~ 9 인 경우*/
		data += 0x30; /*		0 ~ 9 (0x30 ~ 0x39)*/
	}
	else
	{ /*	10 ~ 15 인 경우*/
		data += 0x37; /*		10 ~ 15 (0x41 ~ 0x46)*/
	}
	return (data); /*	반환값=[ASCII 변환 DATA]*/
}

/// @brief    Decimal to HEX
/// @param    mu8DecData : DEC Data
/// @return   mu8ReturnData : HEX Data
U16 DEC2HEX_16BIT(U16 mu16DecData)
{
	U16 mu16ReturnData = 0;
    mu16ReturnData = ((((mu16DecData / 1000) % 10) << 12) & 0xF000) + ((((mu16DecData / 100) % 10) << 8) & 0x0F00) + ((((mu16DecData / 10) % 10) << 4) & 0x00F0) + (mu16DecData % 10);
    return  mu16ReturnData;
}
