/************************************************************
 * Copyright (C), 2009-2011, Tech. Co., Ltd.
 * FileName:		// 文件名
 * Author:			// 作者
 * Date:			// 日期
 * Description:		// 模块描述
 * Version:			// 版本信息
 * Function List:	// 主要函数及其功能
 *     1. -------
 * History:			// 历史修改记录
 *     <author>  <time>   <version >   <desc>
 *     xxxx    16/10/12     1.0     build this moudle
 ***********************************************************/


/***************************************************

   LT8619C HDMI 转 TTL 寄存器设置

***************************************************/

u8 I2CADR = 0x64;                       // IIC address // bit0 是读写标志位；
//I2CADR = 0x32;				// IIC address // bit7 是读写标志位；

u16 H_FrontP   = 0x0000;
u16 H_BackP	   = 0x0000;
u16 H_SyncWid  = 0x0000;

u16 H_total	   = 0x0000;
u16 V_total	   = 0x0000;

u16 H_Active   = 0x0000;
u16 V_Active   = 0x0000;

u8	V_FrontP   = 0x00;
u8	V_BackP	   = 0x00;
u8	V_SyncWid  = 0x00;

u32 HDMI_CLK_Cnt = 0x00000000;

//**********************************************//

extern LT8619C_I2C_Write_Byte( u8 RegAddr, u8 data );                   // IIC 写操作

extern u8 LT8619C_I2C_Read_Byte( u8 RegAddr );                          // IIC 读操作


extern bool LT8619C_I2C_Write_ByteN( u8 RegAddr, u8 *data, u16 Num );   // 往寄存器写Num 个 byte的数值


//**********************************************//
//xxPC:full range;	xxTV:limit range
#define SDTV	0x00
#define SDPC	0x10
#define HDTV	0x20
#define HDPC	0x30

//*******************************************************//

typedef  enum
{
	LOW	   = 0,
	HIGH   = !LOW
}
Pin_Status;

typedef  enum
{
	_input	   = 0,
	_output	   = !LOW
}
GPIO_Status;

bool Read_GPIO13_status;

//================================//

//参考电阻(Pin 16 - REXT, 2K 电阻)
#define _Internal_	0x88                // internal resistance
#define _External_	0x80                // external resistance(Pin 16 - REXT, 2K resistance)

//*********************RGB Config*****************************//

#define _Output_3x8bit_			0x00    // RGB888 24 bit
#define _Output_3x6bit_High_	0x80    // High 18 bit active
#define _Output_3x6bit_Low_		0xa0    // Low 18 bit active
#define _Output_3x565_High_		0xc0    // High 16 bit active
#define _Output_3x565_Low_		0xe0    // Low 16 bit active

#define _6bit_Dither_En		0x38
#define _6bit_Dither_Dis	0x00

//================================//

#define _Bit_Swap_En	0x10            // bit24 ~ bit0 high/low bit swap enable.
#define _Bit_Swap_Dis	0x00

//================================//

//// RGB888 channel Swap
#define _OutPut_BGR_	0x07                                // B0~B7[D0:D7];G0~G7[D8:D15];R0~R7[D16:D23];
#define _OutPut_GBR_	0x06                                // G0~G7[D0:D7];B0~B7[D8:D15];R0~R7[D16:D23];

#define _OutPut_BRG_	0x05                                // B0~B7[D0:D7];R0~R7[D8:D15];G0~G7[D16:D23];
#define _OutPut_RBG_	0x04                                // R0~R7[D0:D7];B0~B7[D8:D15];G0~G7[D16:D23];

#define _OutPut_GRB_	0x03                                // G0~G7[D0:D7];R0~R7[D8:D15];B0~B7[D16:D23];
#define _OutPut_RGB_	0x00                                // R0~R7[D0:D7];G0~G7[D8:D15];B0~B7[D16:D23];

//================================//

#define _Red_Bit_Swap_En	0x20        // Red bit7 ~ bit0 swap enable
#define _Red_Bit_Swap_Dis	0x00

#define _Green_Bit_Swap_En	0x40        // Green bit7 ~ bit0 swap enable
#define _Green_Bit_Swap_Dis 0x00

#define _Blue_Bit_Swap_En	0x80        // Blue bit7 ~ bit0 swap enable
#define _Blue_Bit_Swap_Dis	0x00

//================================//

// 设置IIS 音频输出，IIS和SPDIF只能二选一
#define _IIS_Output_ 0x11

// 设置SPDIF 音频输出，IIS和SPDIF只能二选一
#define _SPDIF_Output_ 0x39

//================================//

// Output sync Polrity config
#define _Follow_Input_		0x11
#define _Negative_Polarity_ 0x10
#define _Positive_Polarity_ 0x01

//================================//

//**********************************************//

// u8 LT8619C_Output_Mode ;// = _TTL_Output_;
u8		Refer_Resistance;

bool	Load_HDCPKey_En;
bool	CLK_DDRMODE_EN;

u8		CP_Convert_Mode;

u8		Sync_Polarity;

// when LT8619C_Output_Mode == _RGB_Output_
u8		TTL_Color_Mode;
u8		TTL_Bit_Swap;
u8		TTL_Output_RGB_Swap;

u8		Red_Bit_Swap;
u8		Green_Bit_Swap;
u8		Blue_Bit_Swap;

u8		Audio_Output_Mode;  // = _IIS_Output_;
bool	Audio_Output_En;    // = 1;// 1 : enable Audio

//**********************************************//

void LT8619C_Config( void )
{
//	LT8619C_Output_Mode = _TTL_Output_;;

//  Internal / external reference resistance (pin 16 - next, 2K resistor) selection
	Refer_Resistance = _Internal_;          // _Internal_	0x88 // internal resistance
	// _External_	0x80 // external resistance(Pin 16 - REXT, 2K resistance)

	Load_HDCPKey_En = 0;                    //  1:外接 HDCP key; 0:不使用 HDCP Key

	CLK_DDRMODE_EN = 0;                     // 1: CLK divided by 2 ; 0 : Normal

	//===============Full/Limit range=================//
	
	CP_Convert_Mode = HDTV;
	//========================+++++++++++++++========//


	// Output sync Polrity config
	Sync_Polarity = _Follow_Input_;         // _Positive_Polarity_;    // _Follow_Input_
	//--------------------------------------// _Negative_Polarity_
	//--------------------------------------// _Positive_Polarity_

	Audio_Output_Mode = _IIS_Output_;       // _IIS_Output_
	//--------------------------------------// _SPDIF_Output_

	Audio_Output_En = 1;                    // 1 : enable Audio Output

	//******************************************//

	// when LT8619C_Output_Mode == _RGB_Output_
	TTL_Color_Mode = _Output_3x8bit_;       // Output RGB888

	TTL_Bit_Swap = _Bit_Swap_Dis;           // 0x10: bit24 ~ bit0 high/low bit swap enable. 0x00: Normal

	TTL_Output_RGB_Swap = _OutPut_RGB_;     //

	Red_Bit_Swap   = _Red_Bit_Swap_Dis;     // 0x20: Red bit7 ~ bit0 swap enable; 0x00 : Normal
	Green_Bit_Swap = _Green_Bit_Swap_Dis;   // 0x40: Red bit7 ~ bit0 swap enable; 0x00 : Normal
	Blue_Bit_Swap  = _Blue_Bit_Swap_Dis;    // 0x80: Red bit7 ~ bit0 swap enable; 0x00 : Normal
}

/***********************************************************

***********************************************************/
void LT8619C_Chip_ID( void )
{
	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );

#ifdef _DEBUG_MODE_
//	Debug_Printf( "\r\nRead LT8619C ID " );
	Debug_Printf( "\r\n " );
	Debug_DispStrNum( "\r\nLT8619C Chip ID = ", LT8619C_I2C_Read_Byte( 0x00 ) );    // 0x16
	Debug_DispStrNum( " ", LT8619C_I2C_Read_Byte( 0x01 ) );                         // 0x04
	Debug_Printf( "\r\n " );
#endif
}

//******************************************//
// By reading the status register of lt8619c, you can know the timing of the HDMI signal

void HDMI_Timing_Read( void )               // Video check
{
	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );   //register bank

	LT8619C_I2C_Write_Byte( 0x0c, 0xFB );
	Delay_ms( 10 );
	LT8619C_I2C_Write_Byte( 0x0c, 0xFF );   // reset video check
	Delay_ms( 100 );

	H_FrontP   = LT8619C_I2C_Read_Byte( 0x1a ) * 0x100 + LT8619C_I2C_Read_Byte( 0x1b );
	H_SyncWid  = LT8619C_I2C_Read_Byte( 0x14 ) * 0x100 + LT8619C_I2C_Read_Byte( 0x15 );
	H_BackP	   = LT8619C_I2C_Read_Byte( 0x18 ) * 0x100 + LT8619C_I2C_Read_Byte( 0x19 );

	H_Active = LT8619C_I2C_Read_Byte( 0x22 ) * 0x100 + LT8619C_I2C_Read_Byte( 0x23 );

	H_total = LT8619C_I2C_Read_Byte( 0x1E ) * 0x100 + LT8619C_I2C_Read_Byte( 0x1F );

	V_FrontP   = LT8619C_I2C_Read_Byte( 0x17 );
	V_SyncWid  = LT8619C_I2C_Read_Byte( 0x13 );
	V_BackP	   = LT8619C_I2C_Read_Byte( 0x16 );

	V_Active = LT8619C_I2C_Read_Byte( 0x20 ) * 0x100 + LT8619C_I2C_Read_Byte( 0x21 );

	V_total = ( LT8619C_I2C_Read_Byte( 0x1C ) & 0x0F ) * 0x100 + LT8619C_I2C_Read_Byte( 0x1D );

	LT8619C_I2C_Write_Byte( 0xFF, 0x80 ); //register bank

	HDMI_CLK_Cnt = ( LT8619C_I2C_Read_Byte( 0x44 ) & 0x07 ) * 0x10000 + LT8619C_I2C_Read_Byte( 0x45 ) * 0x100 + LT8619C_I2C_Read_Byte( 0x46 );
	// HDMI_CLK = HDMI_CLK_Cnt * 1000 ;
}

//**********************************************//

// Read LT8619C ID
void Read_LT8619C_ID( void )
{
	I2CADR = 0x64;                              // IIC address is 0x64
	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );       // register bank；First write 0x60 to the 0xff register and switch to the Bank of 0x60
	ID0_temp   = LT8619C_I2C_Read_Byte( 0x00 ); // LT8619C ID 0x16
	ID1_temp   = LT8619C_I2C_Read_Byte( 0x01 ); // LT8619C ID 0x04
}

//**********************************************//

void LT8619C_ColorConfig( void )
{
	u8 Temp_CSC = 0x00;

	LT8619C_I2C_Write_Byte( 0xFF, 0x80 );
	Temp_CSC = LT8619C_I2C_Read_Byte( 0x71 );

	// If the color space of HDMI is not RGB, color space conversion is needed.
	if( ( ( Temp_CSC & 0x60 ) == 0x20 ) || ( ( Temp_CSC & 0x60 ) == 0x40 ) )   
	{
		LT8619C_I2C_Write_Byte( 0xFF, 0x60 );
		LT8619C_I2C_Write_Byte( 0x07, 0x8C );                                   // YCbCr to RGB ClK enable

		if( ( Temp_CSC & 0x60 ) == 0x20 )
		{
			LT8619C_I2C_Write_Byte( 0x52, 0x01 );                               // YUV422 to YUV444 enable

#ifdef _DEBUG_MODE_
			Debug_Printf( "\r\nHDMI Color Space is	YUV422");
#endif
		}else
		{
			LT8619C_I2C_Write_Byte( 0x52, 0x00 );                               // YUV444

#ifdef _DEBUG_MODE_
			Debug_Printf( "\r\nHDMI Color Space is	YUV444");
#endif
		}

		LT8619C_I2C_Write_Byte( 0x53, 0x44 + CP_Convert_Mode );                 // 0x40:YUV to RGB enable;
	}else
	{
		LT8619C_I2C_Write_Byte( 0xFF, 0x60 );
		LT8619C_I2C_Write_Byte( 0x07, 0x80 );
		LT8619C_I2C_Write_Byte( 0x52, 0x00 );
		LT8619C_I2C_Write_Byte( 0x53, 0x04 );                                   // 0x00: ColorSpace bypass

#ifdef _DEBUG_MODE_
		Debug_Printf( "\r\nHDMI Color Space is	RGB");
#endif
	}
}

//*****************************EDID write*************************************//

const u8 ONCHIP_EDID[256] = {
// Example:
// 1280x720P 60Hz

	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x32, 0x8d, 0xc5, 0x07, 0x01, 0x00, 0x00, 0x00,
	0x2e, 0x14, 0x01, 0x03, 0x80, 0x59, 0x32, 0x78, 0x0a, 0xee, 0x91, 0xa3, 0x54, 0x4c, 0x99, 0x26,
	0x0f, 0x50, 0x54, 0x21, 0x08, 0x80, 0x71, 0x40, 0x81, 0x00, 0x81, 0x40, 0x81, 0x80, 0x95, 0x00,
	0xa9, 0xc0, 0xa9, 0x40, 0xd1, 0xc0, 0x01, 0x1d, 0x00, 0x72, 0x51, 0xd0, 0x1e, 0x20, 0x6e, 0x28,
	0x55, 0x00, 0xc4, 0x8e, 0x21, 0x00, 0x00, 0x1e, 0x66, 0x21, 0x50, 0xb0, 0x51, 0x00, 0x1b, 0x30,
	0x40, 0x70, 0x36, 0x00, 0xa0, 0x5a, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x18,
	0x4b, 0x1a, 0x51, 0x17, 0x00, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xfc,
	0x00, 0x4c, 0x4f, 0x4e, 0x54, 0x49, 0x55, 0x4d, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x57,

	0x02, 0x03, 0x2f, 0xc1, 0x48, 0x84, 0x13, 0x03, 0x12, 0x02, 0x11, 0x10, 0x1f, 0x23, 0x09, 0x07,
	0x07, 0x83, 0x01, 0x00, 0x00, 0x02, 0x00, 0x0f, 0xe3, 0x05, 0x03, 0x01, 0x72, 0x03, 0x0c, 0x00,
	0x40, 0x00, 0x80, 0x1e, 0x20, 0xd0, 0x08, 0x01, 0x40, 0x07, 0x3f, 0x40, 0x50, 0x90, 0xa0, 0x01,
	0x1d, 0x00, 0xbc, 0x52, 0xd0, 0x1e, 0x20, 0xb8, 0x28, 0x55, 0x40, 0xc4, 0x8e, 0x21, 0x00, 0x00,
	0x1e, 0x66, 0x21, 0x56, 0xaa, 0x51, 0x00, 0x1e, 0x30, 0x46, 0x8f, 0x33, 0x00, 0xc4, 0x8e, 0x21,
	0x00, 0x00, 0x1e, 0x20, 0x1c, 0x56, 0x86, 0x50, 0x00, 0x20, 0x30, 0x0e, 0x38, 0x13, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x1e, 0x0e, 0x1f, 0x00, 0x80, 0x51, 0x00, 0x1e, 0x30, 0x40, 0x80, 0x37,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xef
};

/***********************************************************/
//8619C 的EDID 写入，可以操作256次 写0x90寄存器地址，再写1 byte EDID数据；
// 或者写一次0x90地址，连续写256次 1 byte EDID数据。
// 写EDID的时候，寄存器地址不要递增
void Write_EDID2HDMI_Shadow( void )
{
	LT8619C_I2C_Write_Byte( 0x8f, 0x00 );
	// Write 256 byte EDID values to the 0x90 register continuously, and do not increase the register address.
	LT8619C_I2C_Write_ByteN( 0x90, &ONCHIP_EDID[0], 256 ); // 往0x90寄存器连续写256个EDID 值。
}

//********************HPD Contorl************************//

void LT8619C_SetHPD( bool level )
{
	LT8619C_I2C_Write_Byte( 0xFF, 0x80 );
	if( level )
	{
		LT8619C_I2C_Write_Byte( 0x06, LT8619C_I2C_Read_Byte( 0x06 ) | 0x08 );
	}else
	{
		LT8619C_I2C_Write_Byte( 0x06, LT8619C_I2C_Read_Byte( 0x06 ) & 0xf7 );
	}
}

//**********************Load HDCP key******************************//

bool LT8619C_LoadHDCPKey( void )
{
	u8 flag_load_key_done = 0, loop_cnt = 0;

	LT8619C_I2C_Write_Byte( 0xFF, 0x80 );
	LT8619C_I2C_Write_Byte( 0xb2, 0x50 );
	LT8619C_I2C_Write_Byte( 0xa3, 0x77 );
	while( loop_cnt <= 5 ) //wait load hdcp key done.
	{
		loop_cnt++;
		Delay_ms( 50 );
		flag_load_key_done = LT8619C_I2C_Read_Byte( 0xc0 ) & 0x08;
		if( flag_load_key_done )
		{
			break;
		}
	}
	LT8619C_I2C_Write_Byte( 0xb2, 0xd0 );
	LT8619C_I2C_Write_Byte( 0xa3, 0x57 );
	if( flag_load_key_done )
	{
		//    Debug_Printf("\r\nLoad KEY done^_^");
		return TRUE;
	}else
	{
		//    Debug_Printf("\r\nLoad KEY error#####");
		return FALSE;
	}
}

#ifdef _LT8619_GPIO13_15_En_

//***************GPIO13/GPIO15 Config*****************************//
//*
void Set_GPIO13_Status( bool In_or_Out )    // 1: ouput; 0: input
{
	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );

	if( In_or_Out )                         // set GPIO13 as Output
	{
		LT8619C_I2C_Write_Byte( 0x81, LT8619C_I2C_Read_Byte( 0x81 ) | 0x30 );
	}else // Set GPIO13 as Input
	{
		LT8619C_I2C_Write_Byte( 0x81, LT8619C_I2C_Read_Byte( 0x81 ) & 0x0f );
	}
}

/***********************************************************

***********************************************************/

void Set_GPIO15_Status( bool In_or_Out )    // 1: ouput; 0: input
{
	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );

	if( In_or_Out )                         // set GPIO15 as Output
	{
		LT8619C_I2C_Write_Byte( 0x81, LT8619C_I2C_Read_Byte( 0x81 ) | 0x03 );
	}else // Set GPIO15 as Input
	{
		LT8619C_I2C_Write_Byte( 0x81, LT8619C_I2C_Read_Byte( 0x81 ) & 0xf0 );
	}
}

// when GPIO13 as Input
bool Read_GPIO13( void )
{
	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );

	if( LT8619C_I2C_Read_Byte( 0x48 ) & 0x20 )
	{
		return 1;
	}else
	{
		return 0;
	}
}

// when GPIO15 as Input
bool Read_GPIO15( void )
{
	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );

	if( LT8619C_I2C_Read_Byte( 0x48 ) & 0x10 )
	{
		return 1;
	}else
	{
		return 0;
	}
}

// when GPIO13 as Output
void Set_GPIO13_Output( bool HIGH_or_LOW )
{
	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );

	if( HIGH_or_LOW ) // High
	{
		LT8619C_I2C_Write_Byte( 0x4A, LT8619C_I2C_Read_Byte( 0x4A ) | 0x20 );
	}else // LOW
	{
		LT8619C_I2C_Write_Byte( 0x4A, LT8619C_I2C_Read_Byte( 0x4A ) & 0xdf );
	}
}

// when GPIO15 as Output
void Set_GPIO15_Output( bool HIGH_or_LOW )
{
	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );

	if( HIGH_or_LOW ) // High
	{
		LT8619C_I2C_Write_Byte( 0x4A, LT8619C_I2C_Read_Byte( 0x4A ) | 0x80 );
	}else // Low
	{
		LT8619C_I2C_Write_Byte( 0x4A, LT8619C_I2C_Read_Byte( 0x4A ) & 0x7f );
	}
}

/***********************************************************

***********************************************************/
void LT8619C_GPIO_config( void )
{
// for example

	Set_GPIO13_Status( _input );
	Read_GPIO13_status = Read_GPIO13( );

	Set_GPIO15_Status( _output );
	Set_GPIO15_Output( HIGH );
}

#endif

/*********************Audio config*******************************/

// LT8619C 上电默认是IIS 输出，如果只需要IIS输出，可以不要设置Audio寄存器
void LT8619C_AudioConfig( void )
{
	if( Audio_Output_En )
	{
		if( Audio_Output_Mode == _IIS_Output_ )
		{
			LT8619C_I2C_Write_Byte( 0xFF, 0x80 );
			// SD0 channel selected
			LT8619C_I2C_Write_Byte( 0x07, 0x16 ); // 0x16: Left justified(I2S compatible); default
			// 0x14: Right justified;
			// If the setting is not correct, there will be noise.

			LT8619C_I2C_Write_Byte( 0x08, 0x0F );

			LT8619C_I2C_Write_Byte( 0x5d, 0xc9 );

			LT8619C_I2C_Write_Byte( 0xFF, 0x60 );
			LT8619C_I2C_Write_Byte( 0x4c, 0x00 ); // Setting 19 pin as IIS SD0
		}else // SPDIF output
		{
			LT8619C_I2C_Write_Byte( 0xFF, 0x80 );
			LT8619C_I2C_Write_Byte( 0x08, 0x8F );

			LT8619C_I2C_Write_Byte( 0x5d, 0xc9 );

			LT8619C_I2C_Write_Byte( 0xFF, 0x60 );
			LT8619C_I2C_Write_Byte( 0x4c, 0xbf ); // Setting 19 pin SPDIF
		}
	}else // Close Audio output
	{
		LT8619C_I2C_Write_Byte( 0xFF, 0x60 );
		LT8619C_I2C_Write_Byte( 0x4c, 0xff );
		LT8619C_I2C_Write_Byte( 0x4d, 0xff );
	}
}

/***********************************************************

***********************************************************/
void Sync_Polarity_Config( void )
{
// #ifdef _Negative_Polarity_
	if( Sync_Polarity == _Negative_Polarity_ ) // Set H/V sync negative output.
	{
/**********************************************************************************************************

   Set the polarity of BT1120/BT656 output to be negative. First, check the polarity of the source input.

   If H/V sync is negative, the 0x8005 registers will not be adjusted.

   If the input H/V sync is positive , then modify the 0x8005 register to turn on the polarity adjustment
   to ensure that the adjusted H/V sync is negative .

   It is necessary to determine the stability of HDMI signal before making the following judgment,
   otherwise the input sync polarity judgment will be wrong.

 ***********************************************************************************************************/


/**********************************************************************************************************

   Note 1 : If the polarity reversal function of the 0x8005 register has been turned on (input is originally negative,
   after turning on the 0x8005 polarity reversal enablement, positive polarity is detected), then turn off the reversal function

   Note 2 : If the polarity reversal function of 0x8005 register is not turned on, the input polarity is positive ,
   and the polarity reversal function needs to be turned on.

**********************************************************************************************************/

		LT8619C_I2C_Write_Byte( 0xFF, 0x80 );

		if( ( LT8619C_I2C_Read_Byte( 0x17 ) & 0x40 ) == 0x40 )                          // The polarity of V sync is positive, polarity reversal is needed.
		{
			if( ( LT8619C_I2C_Read_Byte( 0x05 ) & 0x10 ) == 0x10 )                      // *** Note 1 ***
			{
				LT8619C_I2C_Write_Byte( 0x05, LT8619C_I2C_Read_Byte( 0x05 ) & 0xef );   // Turn off V sync polarity swap
			}else // *** Note 2 ***
			{
				LT8619C_I2C_Write_Byte( 0x05, LT8619C_I2C_Read_Byte( 0x05 ) | 0x10 );   // enable V sync polarity swap
			}
		}

		if( ( LT8619C_I2C_Read_Byte( 0x17 ) & 0x80 ) == 0x80 )                          // The polarity of H sync is positive, polarity reversal is needed.
		{
			if( ( LT8619C_I2C_Read_Byte( 0x05 ) & 0x20 ) == 0x20 )                      // *** Note 1 ***
			{
				LT8619C_I2C_Write_Byte( 0x05, LT8619C_I2C_Read_Byte( 0x05 ) & 0xdf );   // Turn off H sync polarity swap
			}else // *** Note 2 ***
			{
				LT8619C_I2C_Write_Byte( 0x05, LT8619C_I2C_Read_Byte( 0x05 ) | 0x20 );   // enable H sync polarity swap
			}
		}
	}else

	if( Sync_Polarity == _Positive_Polarity_ )                                          // Set H/V sync Positive output.
	{
/**********************************************************************************************************

   Set the polarity of BT1120/BT656 output to be positive. First, check the polarity of the source input.

   If H/V sync is positive, the 0x8005 registers will not be adjusted.

   If the input H/V sync is negative , then modify the 0x8005 register to turn on the polarity adjustment
   to ensure that the adjusted H/V sync is positive .

   It is necessary to determine the stability of HDMI signal before making the following judgment,
   otherwise the input sync polarity judgment will be wrong.

 ***********************************************************************************************************/


/**********************************************************************************************************

   Note 3 : If the polarity reversal function of the 0x8005 register has been turned on (input is originally positive,
   after turning on the 0x8005 polarity reversal enablement, negative polarity is detected), then turn off the reversal function

   Note 4 : If the polarity reversal function of 0x8005 register is not turned on, the input polarity is negative ,
   and the polarity reversal function needs to be turned on.

**********************************************************************************************************/

		LT8619C_I2C_Write_Byte( 0xFF, 0x80 );

		if( ( LT8619C_I2C_Read_Byte( 0x17 ) & 0x40 ) == 0x00 )                          // The polarity of V sync is negative,  polarity reversal is needed.
		{
			if( ( LT8619C_I2C_Read_Byte( 0x05 ) & 0x10 ) == 0x10 )                      // *** Note 3 ***
			{
				LT8619C_I2C_Write_Byte( 0x05, LT8619C_I2C_Read_Byte( 0x05 ) & 0xef );   // Turn off V sync polarity swap
			}else // *** Note 4 ***
			{
				LT8619C_I2C_Write_Byte( 0x05, LT8619C_I2C_Read_Byte( 0x05 ) | 0x10 );   // enable V sync polarity swap
			}
		}

		if( ( LT8619C_I2C_Read_Byte( 0x17 ) & 0x80 ) == 0x00 )                          // The polarity of H sync is negative, polarity reversal is needed.
		{
			if( ( LT8619C_I2C_Read_Byte( 0x05 ) & 0x20 ) == 0x20 )                      // *** Note 3 ***
			{
				LT8619C_I2C_Write_Byte( 0x05, LT8619C_I2C_Read_Byte( 0x05 ) & 0xdf );   // Turn off H sync polarity swap
			}else // *** Note 4 ***
			{
				LT8619C_I2C_Write_Byte( 0x05, LT8619C_I2C_Read_Byte( 0x05 ) | 0x20 );   // enable H sync polarity swap
			}
		}
	}
}

/***********************************************************

***********************************************************/
void LVDSPLL_Lock_Det( void )
{
	u8 temp = 0;

	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );
	LT8619C_I2C_Write_Byte( 0x0e, 0xfd );
	Delay_ms( 5 );
	LT8619C_I2C_Write_Byte( 0x0e, 0xff );

	Delay_ms( 100 );

	LT8619C_I2C_Write_Byte( 0xFF, 0x80 );

	while(( LT8619C_I2C_Read_Byte( 0x87 ) & 0x20 ) == 0x00)
	{
		LT8619C_I2C_Write_Byte( 0xFF, 0x60 );
		LT8619C_I2C_Write_Byte( 0x0e, 0xfd );
		Delay_ms( 5 );
		LT8619C_I2C_Write_Byte( 0x0e, 0xff );
		Delay_ms( 100 );

		LT8619C_I2C_Write_Byte( 0xFF, 0x80 );
		
		temp++;
		if( temp > 3 )
		{
			break;
		}
	}
}

/***********************************************************/

void LT8619C_RX_reset( void )
{
	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );
	LT8619C_I2C_Write_Byte( 0x0e, 0xbf );
	LT8619C_I2C_Write_Byte( 0x09, 0xfd );
	Delay_ms( 10 );
	LT8619C_I2C_Write_Byte( 0x0e, 0xff );
	LT8619C_I2C_Write_Byte( 0x09, 0xff );	

	LT8619C_I2C_Write_Byte( 0x0e, 0xc7 );
	LT8619C_I2C_Write_Byte( 0x09, 0x0f );
	Delay_ms( 10 );
	LT8619C_I2C_Write_Byte( 0x0e, 0xff );
	Delay_ms( 10 );
	LT8619C_I2C_Write_Byte( 0x09, 0x8f );
	Delay_ms( 10 );
	LT8619C_I2C_Write_Byte( 0x09, 0xff );
	Delay_ms( 50 );
}

/***********************************************************

***********************************************************/
void Wait_HDMI_stable( void )
{
	LT8619C_I2C_Write_Byte( 0xFF, 0x80 );

	while( ( LT8619C_I2C_Read_Byte( 0x44 ) & 0x08 ) == 0x00 ) // 等待RX HDMI CLK信号
	{
		Delay_ms( 100 );
	}

	Delay_ms( 100 );
//	Delay_ms( 200 );
//	Delay_ms( 200 );

	while( ( LT8619C_I2C_Read_Byte( 0x44 ) & 0x08 ) == 0x00 ) // 等待RX HDMI CLK信号
	{
		Delay_ms( 100 );
	}

	Delay_ms( 100 );

	while( ( LT8619C_I2C_Read_Byte( 0x87 ) & 0x10 ) == 0x00 ) // 等待 RX PLL locked
			{
	#ifdef _DEBUG_MODE_
			Debug_Printf( "\r\nwait RXPLL lock" );
	#endif
			LT8619C_RX_reset();
	
			LT8619C_I2C_Write_Byte(0xFF, 0x60 );
			LT8619C_I2C_Write_Byte(0x0e, 0xfd );
			LT8619C_I2C_Write_Byte(0x0e, 0xff );
			Delay_ms( 100 );
	
			LT8619C_I2C_Write_Byte( 0xFF, 0x80 );
			}
	
	Delay_ms( 100 );


	while( ( LT8619C_I2C_Read_Byte( 0x13 ) & 0x01 ) == 0x00 ) // 等待H SYNC 稳定
	{
		Delay_ms( 100 );
	}

	Delay_ms( 100 );
//	Delay_ms( 200 );
/*
	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );
	LT8619C_I2C_Write_Byte( 0x09, 0x7f );
	Delay_ms( 10 );
	LT8619C_I2C_Write_Byte( 0x09, 0xFF ); // reset HDMI RX logic
	Delay_ms( 100 );
//*/
	HDMI_Timing_Read( );
}

/***********************************************************

***********************************************************/
void LT8619C_Reset( void )
{
	GPIO_WriteLow( GPIOC, GPIO_PIN_3 );     // Low
	Delay_ms( 100 );
	GPIO_WriteHigh( GPIOC, GPIO_PIN_3 );    // High
	Delay_ms( 100 );
}

void LT8619C_TTL_Config(void)
{
	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );
//----------------------------------------//
	LT8619C_I2C_Write_Byte( 0x04, 0xf2 );  // enable  PLL detect
	LT8619C_I2C_Write_Byte( 0x60, 0x00 );

	LT8619C_I2C_Write_Byte( 0x6d, TTL_Color_Mode + TTL_Bit_Swap + TTL_Output_RGB_Swap );	// TTL RGB swap
	LT8619C_I2C_Write_Byte( 0x6e, Red_Bit_Swap + Green_Bit_Swap + Blue_Bit_Swap );			//

	LT8619C_I2C_Write_Byte( 0x96, 0x71 );													// default 0x51
	
	LT8619C_I2C_Write_Byte( 0xa0, 0x50 );
	
	if( CLK_DDRMODE_EN )
	{
		LT8619C_I2C_Write_Byte( 0xa4, 0x14 );												//bit2: 1--DDR mode;	0--SDR mode
	}else
	{
		LT8619C_I2C_Write_Byte( 0xa4, 0x10 );												//bit2: 1--DDR mode;	0--SDR mode
	}

	//8bit to 6bit dither function
	if( TTL_Color_Mode == _Output_3x8bit_ )
	{
		LT8619C_I2C_Write_Byte( 0x5f, _6bit_Dither_Dis );									//0x00=no dither; 0x28=dither; 0x38=dither+round
	}else
	{
		LT8619C_I2C_Write_Byte( 0x5f, _6bit_Dither_En );									//0x00=no dither; 0x28=dither; 0x38=dither+round
	}

//===============================//

	Wait_HDMI_stable( );

//===============================//
// When HDMI signal is stable, LVDS PLL lock status needs to be detected. If it is not locked, LVDS PLL needs to be reset
	LVDSPLL_Lock_Det( );// Even for RGB or bt1120 / bt656 output, this configuration cannot be missing.

//===============================//

	// default setting is follow input
	if( Sync_Polarity != _Follow_Input_ )
	{
		Sync_Polarity_Config( );
	}

//===============================//

	//如果在HDMI信号还没出现的时候，就调用色彩空间转换设置函数LT8619C_ColorConfig（）就有可能会判断错误，
	//因为这个函数是根据前端HDMI信号色彩空间的状态来设置。
	//如果HDMI 是YUV420输入，那只能直接按照420送出去,LT8619C没办法做色彩转换。

	LT8619C_ColorConfig( ); 				// 如果HDMI的色彩空间是YUV422/YUV444，需要进行色彩空间转换。

	LT8619C_AudioConfig( );

	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );
	LT8619C_I2C_Write_Byte( 0xa8, 0x0f );	// Turn on TTL output

}


//****************************Initital LT8619C******************************************//

void main( void )
{
	I2CADR = 0x64; // IIC address // bit0 是读写标志位；
	//I2CADR = 0x32;// IIC address // bit7 是读写标志位；

//------------------------------------//

// 复位LT8619C，如果复位信号有接到前端主控GPIO控制的话。先拉低100ms，再拉高，延时100ms
	LT8619C_Reset( );

//------------------------------------//

	LT8619C_Config( );

//------------------------------------//

	LT8619C_Chip_ID( ); // read chip ID

//------------------------------------//

// 如果HDMI是强制输出，可以不需要写EDID。

	LT8619C_SetHPD( LOW );
	Delay_ms( 300 );
	LT8619C_I2C_Write_Byte( 0xFF, 0x80 );
	LT8619C_I2C_Write_Byte( 0x8E, 0x07 ); // Enable EDID shadow operation
	Write_EDID2HDMI_Shadow( );
	LT8619C_I2C_Write_Byte( 0x8E, 0x02 );

//-----------------------------------//

	if( Load_HDCPKey_En )
	{
		LT8619C_LoadHDCPKey( );
	}

	LT8619C_SetHPD( HIGH );

//-----------------------------------//

	LT8619C_I2C_Write_Byte( 0xFF, 0x80 );
	LT8619C_I2C_Write_Byte( 0x2c, 0x35 ); //RGD_CLK_STABLE_OPT[1:0]

	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );
	LT8619C_I2C_Write_Byte( 0x80, 0x08 );

//	使用外部参考电阻的时候，Pin16 管脚上只接2K 电阻，不能接Vcc1.8以及接地电容。
//	默认使用内部参考电阻
	LT8619C_I2C_Write_Byte( 0x89, Refer_Resistance );

	LT8619C_I2C_Write_Byte( 0x8b, 0x90 );                                                   // Normal AFE

//----------------------------------------//

	LT8619C_I2C_Write_Byte( 0xa3, 0x30 );                                                   // for U5, pin68 as CLK output


/* // reg 0x6083 bit3 ~ bit0: PCLK(pin68) driver current setting
   2b'0000 = Disable IO output;
   2b'0001 = IO output drive strength 5mA;
   2b'0011 = IO output drive strength 15mA;
   2b'0111 = IO output drive strength 25mA;
   2b'1111 = IO output drive strength 35mA.//*/

	LT8619C_I2C_Write_Byte( 0x83, 0x3f );                                                   // for U5, Set PCLK(pin68) the driving current

//----------------------------------------//
	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );
	LT8619C_I2C_Write_Byte( 0xa8, 0x07 );                                                   // Turn off TTL output

	LT8619C_TTL_Config( );
}

/************************************** The End Of File **************************************/
