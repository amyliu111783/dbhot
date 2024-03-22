/************************************************************
 * Copyright (C), 2009-2011, Tech. Co., Ltd.
 **File name: / / file name
 **Author: / / Author
 **Date: / / date
 **Description: / / module description
 **Version: / / version information
 **Function list: / / main functions and their functions* 1. -------
 **History: / / history modification record
 * <author> <time> <version > <desc>* xxxxx 16/10/12 1.0 build this moudle

 ***********************************************************/


/***************************************************

   LT8619C HDMI to BT1120/BT656 register settings

***************************************************/

u8 I2CADR = 0x64;   // IIC address // bit0 is IIC read and write flag bit.
//I2CADR = 0x32;// IIC address // bit7 is IIC read and write flag bit.

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

#define bug 1
#define bugone 2

#define amyone 2
#define one 1

//************************************

extern LT8619C_I2C_Write_Byte( u8 RegAddr, u8 data );                   // IIC Write operation

extern u8 LT8619C_I2C_Read_Byte( u8 RegAddr );                          // IIC Read operation


// Write Num data to RegAddr register continuously. Register address is not incremented.
extern bool LT8619C_I2C_Write_ByteN( u8 RegAddr, u8 *data, u16 Num );   //


//***************************************************//

#define _Internal_	0x88                                                // internal resistance
#define _External_	0x80                                                // external resistance(Pin 16 - REXT, 2K resistance)

//================================//

//#define CLK_DDRMODE_EN // CLK divided by 2

//================================//
//xxPC:full range;	xxTV:limit range

#define SDTV	0x00    // limit range
#define SDPC	0x10    // full range
#define HDTV	0x20    //limit range
#define HDPC	0x30    // full range

//================================//

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

// Output sync Polrity config
#define _Follow_Input_		0x11
#define _Negative_Polarity_ 0x10
#define _Positive_Polarity_ 0x01

//================================//

#define _BT656_Output_			0x34                                // 8bit, Y/C interleave
#define _BT1120_Output_			0x33                                // 16/20/24 bit BT1120 , Y/C parallel
#define _BT1120_8bit_Output_	0x35                                // 8bit BT1120, Y/C interleave

#define _YUV422_16bit_Output_ 0x00

//================================//

// BT1120 Mapping:
// Y and C can be swap


/***********************************************************
   Output	16-bit		16-bit		20-bit		20-bit		24-bit
   Port	Mapping1	Mapping2	Mapping1	Mapping2	Mapping
   -----|--------- |--------------------|----------|--------
   D0		C[0]		X			C[0]		X			C[0]
   D1		C[1]		X			C[1]		X			C[1]
   D2		C[2]		X			C[2]		X			C[2]
   D3		C[3]		X			C[3]		X			C[3]
   D4		C[4]		X			C[4]		C[0]		C[4]
   D5		C[5]		X			C[5]		C[1]		C[5]
   D6		C[6]		X			C[6]		C[2]		C[6]
   D7		C[7]		X			C[7]		C[3]		C[7]
   D8		Y[0]		C[0]		C[8]		C[4]		C[8]
   D9		Y[1]		C[1]		C[9]		C[5]		C[9]
   D10		Y[2]		C[2]		Y[0]		C[6]		C[10]
   D11      Y[3]		C[3]		Y[1]		C[7]		C[11]
   D12      Y[4]		C[4]		Y[2]		C[8]		Y[0]
   D13      Y[5]		C[5]		Y[3]		C[9]		Y[1]
   D14      Y[6]		C[6]		Y[4]		Y[0]		Y[2]
   D15      Y[7]		C[7]		Y[5]		Y[1]		Y[3]
   D16      X			Y[0]		Y[6]		Y[2]		Y[4]
   D17      X			Y[1]		Y[7]		Y[3]		Y[5]
   D18      X			Y[2]		Y[8]		Y[4]		Y[6]
   D19      X			Y[3]		Y[9]		Y[5]		Y[7]
   D20      X			Y[4]		X			Y[6]		Y[8]
   D21      X			Y[5]		X			Y[7]		Y[9]
   D22      X			Y[6]		X			Y[8]		Y[10]
   D23		X			Y[7]		X			Y[9]		Y[11]

***********************************************************/

//BT656 Mapping:


/*******************************************************************************
   Output		8-bit		8-bit       8-bit       10-bit		10-bit		12-bit		12-bit
   Port		Mapping1	Mapping2	Mapping3	Mapping1	Mapping2	Mapping1	Mapping2
   -------|------------|---------|---------|---------|-----------|---------|---------
   D0           Y/C[0]		X			X			Y/C[0]      X			Y/C[0]		X
   D1           Y/C[1]		X			X			Y/C[1]      X			Y/C[1]		X
   D2           Y/C[2]		X			X			Y/C[2]      X			Y/C[2]		X
   D3           Y/C[3]		X			X			Y/C[3]      X			Y/C[3]		X
   D4           Y/C[4]		X			X			Y/C[4]      X			Y/C[4]		X
   D5           Y/C[5]		X			X			Y/C[5]      X			Y/C[5]		X
   D6           Y/C[6]		X			X			Y/C[6]      X			Y/C[6]		X
   D7           Y/C[7]		X			X			Y/C[7]      X			Y/C[7]		X
   D8           X			Y/C[0]      X			Y/C[8]      X			Y/C[8]		X
   D9           X			Y/C[1]      X			Y/C[9]      X			Y/C[9]		X
   D10			X			Y/C[2]      X			X			X			Y/C[10]		X
   D11      X			Y/C[3]      X			X			X			Y/C[11]		X
   D12      X			Y/C[4]      X			X			X			X			Y/C[0]
   D13      X			Y/C[5]      X			X			X			X			Y/C[1]
   D14      X			Y/C[6]      X			X			Y/C[0]      X			Y/C[2]
   D15      X			Y/C[7]      X			X			Y/C[1]      X			Y/C[3]
   D16      X			X			Y/C[0]		X			Y/C[2]      X			Y/C[4]
   D17      X			X			Y/C[1]		X			Y/C[3]      X			Y/C[5]
   D18      X			X			Y/C[2]		X			Y/C[4]      X			Y/C[6]
   D19      X			X			Y/C[3]		X			Y/C[5]      X			Y/C[7]
   D20      X			X			Y/C[4]		X			Y/C[6]      X			Y/C[8]
   D21      X			X			Y/C[5]		X			Y/C[7]      X			Y/C[9]
   D22      X			X			Y/C[6]		X			Y/C[8]      X			Y/C[10]
   D23      X			X			Y/C[7]		X			Y/C[9]      X			Y/C[11]

 *********************************************************************************/

#define _8bit_YC_ // 16-bit BT1120 Mapping 1/2 / 8-bit BT656 mapping 1/2/3

//#define _10bit_YC_H_ // 20-bit BT1120 Mapping 2 / 10-bit BT656 mapping 2
//#define _10bit_YC_L_ // 20-bit BT1120 mapping 1 / 10-bit BT656 Mapping 1

//#define _12bit_YC_ // 24-bit BT1120 / 12-bit BT656 mapping 1/2

//------------------------------------------//

#define _YC_swap_En 0x08            // Y and C channel swap enable

#define _YC_swap_Dis 0x00           // Y and C channel swap Disable


//------------------------------------------/

//================================//
// YUV422_24-bit / BT1120 24-bit / BT656 12bit
#define _Output_24bit_ 0x00
// when _YC_swap_En : D0 ~ D11 output BT656 12-bit
//                    D0 ~ D23 is BT1120 24-bit : D0 ~ D11 Y ; D12 ~ D23 C output BT1120 24bit
// when _YC_swap_Dis : D12 ~ D23 is BT656 12-bit
//                     D0 ~ D23 is BT1120 24-bit : D0 ~ D11 C ; D12 ~ D23 Y

//---------------------------------------------//

//high 20 bit(D4 ~ D23) active
#define _Output_20bit_High_ 0x04
// when _YC_swap_Dis : D4 ~ D13 is BT656 10-bit£»
//                     D4 ~ D23 is BT1120 20-it: D4 ~ D13 Y ; D14 ~ D23 C
// when _YC_swap_Dis : D14 ~ D23 is BT656 10-bit£»
//                     D4 ~ D23 is BT1120 20-bit: D4 ~ D13 C ; D14 ~ D23 Y

//---------------------------------------------//

//low 20-bit(D0 ~ D19) active
#define _Output_20bit_Low_ 0x05
// when _YC_swap_En : D0 ~ D9 is BT656 10-bit£»
//                    D0 ~ D19 is BT1120 20-bit: D0 ~ D09 Y ; D10 ~ D19 C
// when _YC_swap_Dis : D10 ~ D19 is BT656 10-bit£»
//                     D0 ~ D19 is BT1120 20-bit: D0 ~ D09 C ; D10 ~ D19 Y

//---------------------------------------------//

//high 16-bit(D8 ~ D23) active
#define _Output_16bit_High_ 0x06
// when _YC_swap_En : D8 ~ D15 is BT656 8-bit£»
//                    D8 ~ D23 is BT1120 16-bit: D8 ~ D15 Y ; D16 ~ D23 C
// when _YC_swap_Dis : D16 ~ D23 is BT656 8-bit£»
//                     D8 ~ D23 is BT1120 16-bit: D8 ~ D15 C ; D16 ~ D23 Y

//---------------------------------------------//

//low 16-bit(D0 ~ D15) active
#define _Output_16bit_Low_ 0x07
// when _YC_swap_En : D0 ~ D7 is BT656 8-bit;
//                    D0 ~ D15 is BT1120 16-bit : D0 ~ D7 Y ; D8 ~ D15 C
// when _YC_swap_Dis : D8 ~ D15 is BT656 8-bit£»
//                     D0 ~ D15 is BT1120 16-bit : D0 ~ D7 C ; D8 ~ D15 Y

//================================//

// Set IIS/SPDIF audio output, IIS and SPDIF can only choose one of them.
#define _IIS_Output_ 0x11

#define _SPDIF_Output_ 0x39

//************************************************************//

// u8 LT8619C_Output_Mode ;
u8		Refer_Resistance;

bool	Load_HDCPKey_En;

bool	CLK_DDRMODE_EN;

u8		CP_Convert_Mode;

u8		Sync_Polarity;

// when LT8619C_Output_Mode == _YUV_Output_
u8		YUV_Output_Mode;
u8		YC_swap;

u8		YUV_ColorDepth;

u8		Channel_Num;

u8		Audio_Output_Mode;  // = _IIS_Output_;
bool	Audio_Output_En;    // = 1;// 1 : enable Audio

bool	Interlace_In;

u8		I_P_SEL;

u8		Phase_Num[10] = { 0x20, 0x28, 0x21, 0x29, 0x22, 0x2a, 0x23, 0x2b, 0x24, 0x2c };

//******************************************//

void LT8619C_setting( void )
{
//	LT8619C_Output_Mode = _YUV_Output_ ;// _YUV_Output_ : HDMI to BT1120 / BT656

//  Internal reference resistance is selected by default
	Refer_Resistance = _Internal_;  // _Internal_	0x88 // internal resistance
	// _External_	0x80 // external resistance(Pin 16 - REXT, 2K resistance)

	Load_HDCPKey_En	   = 0;         //  1:External HDCP key; 0:Do not use HDCP Key
	CLK_DDRMODE_EN	   = 0;         // 1: CLK divided by 2 ; 0 : Normal

	//===============Full/Limit range=================//


	/*****************************************************
	   CSC RGB to YUV mode  |	Input		|		Output
	   -------------------------------------------------------
	   SDPC/HDPC				16~235				30~216
	       0~255				16~235
	   ------------------------------------------------------
	   SDTV/HDTV				16~235				16~235
	       0~255				0~255
	   ------------------------------------------------------
	 ******************************************************/
	CP_Convert_Mode = HDPC;
	//========================+++++++++++++++========//

	Interlace_In = 0;            // HDMI de-interlace input : 0  ; HDMI interlace input: 1

	if( Interlace_In )
	{
		I_P_SEL = 0x08;
	}else
	{
		I_P_SEL = 0x00;
	}

	// Output sync Polrity config
	Sync_Polarity = _Positive_Polarity_;    // _Follow_Input_
	//--------------------------------------// _Negative_Polarity_
	//--------------------------------------// _Positive_Polarity_

	Audio_Output_Mode = _IIS_Output_;       // _IIS_Output_
	//--------------------------------------// _SPDIF_Output_

	Audio_Output_En = 1;                    // 1 : enable Audio Output

	//*******************************************//

	YUV_Output_Mode = _BT1120_Output_;
	// _YUV422_16bit_Output_
	// _BT1120_Output_ : HDMI to BT1120, Y/C parallel
	// _BT1120_8bit_Output_ : HDMI to BT1120, Y/C interleave
	// _BT656_Output_ : HDMI to BT656, Y/C interleave

	//******************************************************************************************//

	YC_swap = _YC_swap_En;                  //	_YC_swap_Dis

	//******************************************************************************************//
#ifdef _8bit_YC_
	YUV_ColorDepth = _Output_16bit_Low_;    // _Output_16bit_Low_

	// If select _Output_16bit_High_ :
	// when _YC_swap_En : [D8 ~ D15 is BT656(8-bit Mapping 2) ] / [D8 ~ D15 Y ; D16 ~ D23 C is BT1120(16bit) -- 16-bit Mapping 2]
	// when _YC_swap_Dis : [D16 ~ D23 is BT656(8-bit Mapping 3) ] / [D8 ~ D15 C ; D16 ~ D23 Y is BT1120(16bit)-- 16-bit Mapping 2]

	// If select _Output_16bit_Low_ :
	// when _YC_swap_En : [D0 ~ D7 is BT656(8-bit Mapping 1) ] / [D0 ~ D7 Y ; D8 ~ D15 C is BT1120(16bit)-- 16-bit Mapping 1]
	// when _YC_swap_Dis : [D8 ~ D15 is BT656(8-bit Mapping 2) ] / [D0 ~ D7 C ; D8 ~ D15 Y is BT1120(16bit)-- 16-bit Mapping 1]


	//******************************************************************************************//
#endif

#ifdef _12bit_YC_

	YUV_ColorDepth = _Output_24bit_;
	// when _YC_swap_En : D0 ~ D11 output BT656 12bit(Mapping 1)
	// D0 ~ D23 is BT1120 24bit : D0 ~ D11 Y ; D12 ~ D23 C output BT1120 24bit

	// when _YC_swap_Dis : D12 ~ D23 is BT656 12bit(Mapping 2)
							// D0 ~ D23 is BT1120 24bit : D0 ~ D11 C ; D12 ~ D23 Y


	//******************************************************************************************//
#endif

#ifdef _10bit_YC_H_

	YUV_ColorDepth = _Output_20bit_High_;   //
	// when _YC_swap_En : x
	// D4 ~ D23 is BT1120 20bit: D4 ~ D13 Y ; D14 ~ D23 C -- 20-bit Mapping 2

	// when _YC_swap_Dis : D14 ~ D23 is BT656 10bit(10-bit Mappong 2)
	// D4 ~ D23 is BT1120 20bit: D4 ~ D13 C ; D14 ~ D23 Y -- 20-bit Mapping 2

#endif //==================================================//

#ifdef _10bit_YC_L_

	YUV_ColorDepth = _Output_20bit_Low_; //
	// when _YC_swap_En : D0 ~ D9 is BT656 10bit(10-bit Mappong 1)
	// D0 ~ D19 is BT1120 20bit: D0 ~ D09 Y ; D10 ~ D19 C -- 20-bit Mapping 1

	// when _YC_swap_Dis : x
	// D0 ~ D19 is BT1120 20bit: D0 ~ D09 C ; D10 ~ D19 Y -- 20-bit Mapping 1

#endif
	//******************************************************************************************//
}

/***********************************************************/

void LT8619C_Chip_ID( void )
{
	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );

#ifdef _DEBUG_MODE_
//	Debug_Printf( "\r\nRead LT8619C ID " );
	Debug_Printf( "\r\n " );
	Debug_DispStrNum( "\r\nLT8619C Chip ID = ", LT8619C_I2C_Read_Byte( 0x00 ) );    // ID0:0x16
	Debug_DispStrNum( " ", LT8619C_I2C_Read_Byte( 0x01 ) );                         // ID1:0x04
	Debug_Printf( "\r\n " );
#endif
}

//**********************Timing**************************//

//By reading the status register of LT8619C, you can know the Timing of the HDMI signal.

void HDMI_Video_Check( void )
{
#ifdef _uart_debug_

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

	LT8619C_I2C_Write_Byte( 0xFF, 0x80 );                                           //register bank

	HDMI_CLK_Cnt   = (u32)( LT8619C_I2C_Read_Byte( 0x44 ) & 0x07 ) * 0x10000;       //+ LT8619C_I2C_Read_Byte( 0x45 ) * 0x100 + LT8619C_I2C_Read_Byte( 0x46 );
	HDMI_CLK_Cnt   = HDMI_CLK_Cnt + ( (u32)LT8619C_I2C_Read_Byte( 0x45 ) ) * 0x100; //+ LT8619C_I2C_Read_Byte( 0x46 );
	HDMI_CLK_Cnt   = HDMI_CLK_Cnt + (u32)LT8619C_I2C_Read_Byte( 0x46 );
	// HDMI_CLK = HDMI_CLK_Cnt * 1000 (Hz);

	// LOG of HDMI Timing
	print( "\r\n====== Read LT8619C Video Check ======" );
	print( "\r\n" );

	print( "\r\nHDMI Timing : " );
	print( "\r\n" );
	print( "\r\nHorizon  Timing :  H_FP / H_pluse / H_BP / H_act / H_tol = " );
//	print( "\r\n");
	printdec_u32( (u32)H_FrontP );              // HFB
	print( ", " );
	printdec_u32( (u32)H_SyncWid );             // Hs_wid
	print( ", " );
	printdec_u32( (u32)H_BackP );               // HBP
	print( ", " );
	printdec_u32( (u32)H_Active );              // Hact
	print( ", " );
	printdec_u32( (u32)H_total );               // Htotal
	print( ", " );

	print( "\r\n " );
	print( "\r\nVertical Timing :  V_FP / V_pluse / V_BP / V_act / V_tol  = " );
//	print( "\r\n");
	printdec_u32( (u32)V_FrontP );              // VFB
	print( ", " );
	printdec_u32( (u32)V_SyncWid );             // Vs_wid
	print( ", " );
	printdec_u32( (u32)V_BackP );               // VBP
	print( ", " );
	printdec_u32( (u32)V_Active );              // Vact
	print( ", " );
	printdec_u32( (u32)V_total );               // Vtotal
	print( ", " );
	print( "\r\n" );

	print( "\r\nHDMI Pixel clock : " );
	printdec_u32( (u32)HDMI_CLK_Cnt );
	print( "\r\n Hz " );
	print( "\r\n" );

#endif
}

//**********************************************//

void LT8619C_ColorConfig( void )
{
	u8 ColorSpace = 0x00;

	// ColorSpace convert
	LT8619C_I2C_Write_Byte( 0xFF, 0x80 );
	ColorSpace = ( LT8619C_I2C_Read_Byte( 0x71 ) ) & 0x60;

	if( ColorSpace != 0x20 )                                        // If HDMI signal color space is not YCbCr422, color space conversion is needed.
	{
		LT8619C_I2C_Write_Byte( 0xFF, 0x60 );
		LT8619C_I2C_Write_Byte( 0x07, 0xf0 );                       // 20170126

		if( ColorSpace == 0x00 )
		{
			LT8619C_I2C_Write_Byte( 0x52, 0x0a + CP_Convert_Mode ); // RGB convert YCbCr422 enable
#ifdef _DEBUG_MODE_

			print( "\r\nHDMI Color Space is	RGB" );
#endif
		}else
		if( ColorSpace == 0x40 )
		{
			LT8619C_I2C_Write_Byte( 0x52, 0x02 + CP_Convert_Mode ); // YCbCr444 convert YCbCr422 enable
#ifdef _DEBUG_MODE_

			print( "\r\nHDMI Color Space is	YUV444" );
#endif
		}
	}else
	{
		LT8619C_I2C_Write_Byte( 0xFF, 0x60 );
		LT8619C_I2C_Write_Byte( 0x07, 0x80 );
		LT8619C_I2C_Write_Byte( 0x52, 0x00 );

#ifdef _DEBUG_MODE_

		print( "\r\nHDMI Color Space is	YUV422" );
#endif
	}
}

//************************************************

// The BT1120/BT656 output of LT8619C requires setting parameters such as Timing Register (Active, Total, SyncWidth) of LT8619C.
// Just write the value of the read status register to the corresponding Timing register.

void BT_Timing_Config( void )
{
	u16 H_BackP	   = 0x0000;
	u16 H_SyncWId  = 0x0000;
	u16 V_total	   = 0x0000;
	u16 V_active   = 0x0000;

	u8	Double_Cnt = 0x01;

	if( Interlace_In )                                                                                          // If it is interlaced signal input, the Timing setting of BT1120/BT656, v_active & v_total should be doubled.
	{
		Double_Cnt = 0x02;
	}

	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );                                                                       //register bank
	//timing related start
	H_SyncWId  = LT8619C_I2C_Read_Byte( 0x14 ) * 0x100 + LT8619C_I2C_Read_Byte( 0x15 );
	H_BackP	   = LT8619C_I2C_Read_Byte( 0x18 ) * 0x100 + LT8619C_I2C_Read_Byte( 0x19 );
	V_total	   = ( LT8619C_I2C_Read_Byte( 0x1c ) & 0x0F ) * 0x100 + LT8619C_I2C_Read_Byte( 0x1d );
	V_active   = ( LT8619C_I2C_Read_Byte( 0x20 ) & 0x0F ) * 0x100 + LT8619C_I2C_Read_Byte( 0x21 );

	LT8619C_I2C_Write_Byte( 0x61, (u8)( ( H_SyncWId + H_BackP ) / 256 ) );                                      // HBP+H_syncWid[15:8]
	LT8619C_I2C_Write_Byte( 0x62, (u8)( ( H_SyncWId + H_BackP ) % 256 ) );                                      // HBP+H_syncWid[7:0]
	LT8619C_I2C_Write_Byte( 0x63, LT8619C_I2C_Read_Byte( 0x22 ) );                                              // H_Act[15:8]
	LT8619C_I2C_Write_Byte( 0x64, LT8619C_I2C_Read_Byte( 0x23 ) );                                              // H_Act[7:0]
	LT8619C_I2C_Write_Byte( 0x65, LT8619C_I2C_Read_Byte( 0x1E ) );                                              // H_tol[15:8]
	LT8619C_I2C_Write_Byte( 0x66, LT8619C_I2C_Read_Byte( 0x1F ) );                                              // H_tol[7:0]
	LT8619C_I2C_Write_Byte( 0x67, ( LT8619C_I2C_Read_Byte( 0x16 ) + LT8619C_I2C_Read_Byte( 0x13 ) ) % 256 );    // VBP V_syncWid[7:0]

	LT8619C_I2C_Write_Byte( 0x68, (u8)( ( V_total - V_active ) % 256 ) );                                       // V blanking

	LT8619C_I2C_Write_Byte( 0x69, (u8)( ( V_active * Double_Cnt ) / 256 ) );                                    // V_Act[15:8]
	LT8619C_I2C_Write_Byte( 0x6a, (u8)( ( V_active * Double_Cnt ) % 256 ) );                                    // V_Act[7:0]
	LT8619C_I2C_Write_Byte( 0x6b, (u8)( ( V_total * Double_Cnt ) / 256 ) );                                     // V_tol[15:8]
	LT8619C_I2C_Write_Byte( 0x6c, (u8)( ( V_total * Double_Cnt ) % 256 ) );                                     // V_tol[7:0]
	//timing related end.
}

//*****************************EDID write*************************************//

const u8 ONCHIP_EDID[256] = {
//* 1280x720
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
//*/
};


/***********************************************************

***********************************************************/
// Write Num data to RegAddr register continuously. Register address is not incremented.
bool HDMI_WriteI2C_ByteN( u8 RegAddr, u8 *d, u16 N )
{
	if( i2c_write_byte( I2CADR, RegAddr, d, N ) == FALSE )
	{
		FLAG_I2C_ERROR = TRUE;
	}
	return FLAG_I2C_ERROR;
}

/***********************************************************

***********************************************************/
//The EDID writing of 8619C can operate 256 times to write 0x90 address once and write data again, or write 0x90 address once and write 256 times data continuously.
// When writing an EDID, do not increment the register address.
void Write_EDID2HDMI_Shadow( void )
{
	LT8619C_I2C_Write_Byte( 0x8f, 0x00 );
	LT8619C_I2C_Write_ByteN( 0x90, &ONCHIP_EDID[0], 256 );
}

//********************HPD Contorl************************//

void LT8619C_SetHPD( bool level )
{
	LT8619C_I2C_Write_Byte( 0xFF, 0x80 ); //register bank
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

	LT8619C_I2C_Write_Byte( 0xFF, 0x80 );   //register bank
	LT8619C_I2C_Write_Byte( 0xb2, 0x50 );
	LT8619C_I2C_Write_Byte( 0xa3, 0x77 );
	while( loop_cnt <= 5 )                  //wait load hdcp key done.
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

// LT8619C power on the default is IIS audio output, if you only need IIS audio output, you do not need to set the Audio register..
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

//	#ifdef _YUV_
		//	if( LT8619C_Output_Mode == _YUV_Output_ )
		{
			LT8619C_I2C_Write_Byte( 0xFF, 0x60 );

			LT8619C_I2C_Write_Byte( 0x60, I_P_SEL + YUV_Output_Mode - 0x30 );
		}
//	#endif
	}else

	if( Sync_Polarity == _Positive_Polarity_ ) // Set H/V sync Positive output.
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

//	#ifdef _YUV_
		//	if( LT8619C_Output_Mode == _YUV_Output_ )
		{
			LT8619C_I2C_Write_Byte( 0xFF, 0x60 );
			LT8619C_I2C_Write_Byte( 0x60, I_P_SEL + YUV_Output_Mode );
		}
//	#endif
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

	while( ( LT8619C_I2C_Read_Byte( 0x87 ) & 0x20 ) == 0x00 )
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

/***********************************************************

***********************************************************/
void Wait_HDMI_stable( void )
{
	LT8619C_I2C_Write_Byte( 0xFF, 0x80 );

	while( ( LT8619C_I2C_Read_Byte( 0x44 ) & 0x08 ) == 0x00 ) // Wait for the HDMI CLK signal. If there is no HDMI signal, wait.
	{
		Delay_ms( 100 );
	}

	Delay_ms( 100 );
//	Delay_ms( 200 );
//	Delay_ms( 200 );

	while( ( LT8619C_I2C_Read_Byte( 0x44 ) & 0x08 ) == 0x00 ) // Reconfirm HDMI CLK signal.
	{
		Delay_ms( 100 );
	}

	Delay_ms( 100 );
//	Delay_ms( 200 );

	while( ( LT8619C_I2C_Read_Byte( 0x13 ) & 0x01 ) == 0x00 ) // Waiting for H sync signal stabilization
	{
		Delay_ms( 100 );
	}

	Delay_ms( 100 );
//	Delay_ms( 200 );

	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );
	LT8619C_I2C_Write_Byte( 0x09, 0x7f );
	Delay_ms( 10 );
	LT8619C_I2C_Write_Byte( 0x09, 0xFF );   // reset HDMI RX logic
	Delay_ms( 100 );

	HDMI_Video_Check( );                    // for debug
}

/***********************************************************

***********************************************************/
void LT8619C_phase_config( void )
{
	u8		BTCLK_DLY_DET, Pre_BTCLK_DLY_DET;
	u8		i, Num_Mark = 0;
	u8		Num_Mark_Star  = 0;
	bool	BT_clk_lag	   = 0;

	LT8619C_I2C_Write_Byte( 0xFF, 0x80 );

	while( ( LT8619C_I2C_Read_Byte( 0x87 ) & 0x20 ) != 0x20 )
	{
		// PLL reset
		LT8619C_I2C_Write_Byte( 0xFF, 0x60 );
		LT8619C_I2C_Write_Byte( 0x0e, 0xfd );
		LT8619C_I2C_Write_Byte( 0x0e, 0xff );
		LT8619C_I2C_Write_Byte( 0x0a, 0x3f );
		LT8619C_I2C_Write_Byte( 0x0a, 0x7f );
		Delay_ms( 100 );
		LT8619C_I2C_Write_Byte( 0xFF, 0x80 );
	}

	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );                  //register bank
	LT8619C_I2C_Write_Byte( 0xa2, 0x20 );
	Delay_ms( 10 );

	Pre_BTCLK_DLY_DET = LT8619C_I2C_Read_Byte( 0x91 );

	for( i = 0; i < 10; i++ )
	{
		LT8619C_I2C_Write_Byte( 0xa2, Phase_Num[i] );
		Delay_ms( 50 );

		BTCLK_DLY_DET = LT8619C_I2C_Read_Byte( 0x91 );

		if( BTCLK_DLY_DET != Pre_BTCLK_DLY_DET )
		{
			if( BTCLK_DLY_DET == 0x05 )
			{
				//	Pre_BTCLK_DLY_DET = BTCLK_DLY_DET;

				//	Num_Mark = i - 1;
				Num_Mark   = i;
				BT_clk_lag = 1;

				break;
			}else
			if( BTCLK_DLY_DET == 0x01 )
			{
				Pre_BTCLK_DLY_DET  = BTCLK_DLY_DET;
				Num_Mark_Star	   = i;
			}
		}
	}

	if( BT_clk_lag )
	{
		LT8619C_I2C_Write_Byte( 0xa2, Phase_Num[Num_Mark] );
	}else
	{
		LT8619C_I2C_Write_Byte( 0xa2, Phase_Num[Num_Mark_Star + ( Num_Mark - Num_Mark_Star ) / 2] );
	}
}

//**********************LT8619C YUV Output config******************************//

void LT8619C_YUV_Config( void )
{
	LT8619C_I2C_Write_Byte( 0xFF, 0x80 );                                   //register bank
	LT8619C_I2C_Write_Byte( 0x2c, LT8619C_I2C_Read_Byte( 0x2c ) | 0x30 );   //RGD_CLK_STABLE_OPT[1:0]

	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );                                   //register bank
	LT8619C_I2C_Write_Byte( 0x80, 0x08 );

	LT8619C_I2C_Write_Byte( 0x89, Refer_Resistance );

	LT8619C_I2C_Write_Byte( 0x8b, 0x90 );                                   // Normal AFE

//----------------------------------------//

	LT8619C_I2C_Write_Byte( 0xa3, 0x30 );                                   // for U5, pin68 as CLK output


/* // reg 0x6083 bit3 ~ bit0: PCLK(pin68) driver current setting
   2b'0000 = Disable IO output;
   2b'0001 = IO output drive strength 5mA;
   2b'0011 = IO output drive strength 15mA;
   2b'0111 = IO output drive strength 25mA;
   2b'1111 = IO output drive strength 35mA.	//*/

	LT8619C_I2C_Write_Byte( 0x83, 0x3f );                                   // for U5, Set PCLK(pin68) the driving current

//----------------------------------------//

//	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );                               //register bank
	LT8619C_I2C_Write_Byte( 0xa8, 0x07 );                                   // Turn off BT output

	LT8619C_I2C_Write_Byte( 0x04, 0xf2 );                                   // enable  PLL detect

	if( YUV_Output_Mode == _BT656_Output_ )                                 //  (YUV_Output_Mode == _BT656_Output_))
	{
		LT8619C_I2C_Write_Byte( 0x96, 0x71 );
		LT8619C_I2C_Write_Byte( 0xa0, 0x51 );                               // 0x51
		LT8619C_I2C_Write_Byte( 0xa3, 0x74 );                               // for U5
		LT8619C_I2C_Write_Byte( 0xa2, 0x20 );
	}else
	{
		LT8619C_I2C_Write_Byte( 0x96, 0x71 );
		LT8619C_I2C_Write_Byte( 0xa0, 0x50 );                               // 0x50
		LT8619C_I2C_Write_Byte( 0xa3, 0x74 );                               // for U5
		LT8619C_I2C_Write_Byte( 0xa2, 0x20 );
	}

	LT8619C_I2C_Write_Byte( 0x60, I_P_SEL + YUV_Output_Mode );

	if( CLK_DDRMODE_EN )
	{
		LT8619C_I2C_Write_Byte( 0xa4, 0x14 );       //bit2: 1--DDR mode;
	}else
	{
		LT8619C_I2C_Write_Byte( 0xa4, 0x10 );       //bit2: 0--SDR mode
	}

//===============================//

	Wait_HDMI_stable( );

//===============================//
// When HDMI signal is stable, LVDS PLL lock status needs to be detected. If it is not locked, LVDS PLL needs to be reset.
	LVDSPLL_Lock_Det( ); // Even for RGB or bt1120 / bt656 output, this configuration cannot be missing.

//===============================//

	if( Sync_Polarity != _Follow_Input_ )
	{
		Sync_Polarity_Config( );
	}

	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );                  //register bank
	LT8619C_I2C_Write_Byte( 0x0d, 0xfc );
	Delay_ms( 10 );
	LT8619C_I2C_Write_Byte( 0x0d, 0xff );

//================================

	LT8619C_ColorConfig( );

	BT_Timing_Config( );                                        // Read RX status register and set bt1120 / bt656 output directly

//================================

	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );                       //register bank
	LT8619C_I2C_Write_Byte( 0x6d, YC_swap );
	LT8619C_I2C_Write_Byte( 0x6e, YUV_ColorDepth );

	LT8619C_I2C_Write_Byte( 0x0e, 0xfd );
	Delay_ms( 10 );
	LT8619C_I2C_Write_Byte( 0x0e, 0xff );

	LT8619C_I2C_Write_Byte( 0x0d, 0xfc );
	Delay_ms( 10 );
	LT8619C_I2C_Write_Byte( 0x0d, 0xff );

//=====================================
	// phase config
	LT8619C_phase_config( );

//=================================================

	LT8619C_I2C_Write_Byte( 0xFF, 0x60 );
	LT8619C_I2C_Write_Byte( 0xa8, 0x0f );   // Turn on BT output
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

//****************************Initital LT8619C******************************************/

void main( void )
{
	I2CADR = 0x64; // IIC address // bit0 is Read and write flag bit
	//I2CADR = 0x32;// IIC address // bit7 is Read and write flag bit

//------------------------------------//

// Reset LT8619C, First pull down 100ms, then pull up, and delay 100ms.
	LT8619C_Reset( );

//------------------------------------//

	LT8619C_setting( );

//------------------------------------//

	LT8619C_Chip_ID( );                     // read chip ID

//------------------------------------//

// If HDMI source is a mandatory output, you may not need to write EDID to LT8619C shadow.

	LT8619C_SetHPD( LOW );                  // Pull down HPD

	Delay_ms( 100 );
	LT8619C_I2C_Write_Byte( 0xFF, 0x80 );   // register bank
	LT8619C_I2C_Write_Byte( 0x8E, 0x07 );   // Enable EDID shadow operation
	Write_EDID2HDMI_Shadow( );              // write EDID to shadow
	LT8619C_I2C_Write_Byte( 0x8E, 0x02 );

//------------------------------------//

	LT8619C_AudioConfig( );

	if( Load_HDCPKey_En )
	{
		LT8619C_LoadHDCPKey( ); // set HDCP key
	}

	LT8619C_SetHPD( HIGH );     // Pull up HPD

//-----------------------------------//

#ifdef _LT8619_GPIO13_15_En_

	LT8619C_GPIO_config( );

#endif

	//-----------------------------------//

	LT8619C_YUV_Config( );

#ifdef _LT8619_GPIO13_15_En_

	Read_GPIO13_status = Read_GPIO13( );

#endif


/*
   while(1)
   {
   };
   //*/
}

/************************************** The End Of File **************************************/
