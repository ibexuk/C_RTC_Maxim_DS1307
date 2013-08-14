/*
IBEX UK LTD http://www.ibexuk.com
Electronic Product Design Specialists
RELEASED SOFTWARE

The MIT License (MIT)

Copyright (c) 2013, IBEX UK Ltd, http://ibexuk.com

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//Project Name:		MAXIM DS1307 REAL TIME CLOCK DRIVER


//*************************************
//*************************************
//********** DRIVER REVISION **********
//*************************************
//*************************************
//
//V1.00
//- Original release
//
//V1.01
//- AckI2C() replaced with define RTC_I2C_ACK(), and NotAckI2C1() replaced with define RTC_I2C_NOT_ACK()
//
//V1.02
//- Defines updated to generic I2C

//##### USING THIS DRIVER #####
//Include this header file in any .c files within your project from which you wish to use it's functions.



//########## SETUP I2C IN THE MAIN POWERUP INIT ROUTINE ##########
/*
//***** SETUP SSP AS I2C FOR PIC18 *****
//I2C requirements:- 100kHz clock high time 4.0uS, clock low time 4.7uS, 400kHz clock high time 0.6uS, clock low time 1.3uS
//The PIC18 does not fully conform to the 400kHz I2C spec (which applies to all rates >100kHz) but may be used with care
//where higher rates are required.
//Lower 7 bits of SSPADD are the baud rate.  ((Tcycle / 2) * SSPADD * 2) = time per bit.  1 / time per bit = frequency.
//400kHz @ 40MHz OSC = SSPADD 25
//100kHz @ 40MHz OSC = SSPADD 100
//100kHz @ 32MHz OSC = SSPADD 80
//100kHz @ 4MHz OSC = SSPADD 10
	SSPADD = 100;					//setup i2c clk speed
	OpenI2C(MASTER, SLEW_OFF);		//Slew rate control should be on for 400KHz, off for 100KHz & 1MHz
*/



//*****************************
//*****************************
//********** DEFINES **********
//*****************************
//*****************************
#ifndef RTC_DS1307_C_INIT		//Do only once the first time this file is used
#define	RTC_DS1307_C_INIT

#define	DS1307_I2C_ADDRESS			0xd0


//------ SELECT COMPILER ------
//(Enable one of these only)
#define	RTC_USING_PIC18
//#define	RTC_USING_PIC24
//#define	RTC_USING_PIC32


#ifdef RTC_USING_PIC18
//##################
//##### PIC 18 #####
//##################

#include <i2c.h>

#define	RTC_I2C_START_I2C					StartI2C1					//Generate bus start condition
#define	RTC_I2C_START_IN_PROGRESS_BIT		SSP1CON2bits.SEN			//Bit indicating start is still in progress
#define	RTC_I2C_RESTART_I2C					RestartI2C1					//Generate bus restart condition
#define	RTC_I2C_RESTART_IN_PROGRESS_BIT		SSP1CON2bits.RSEN			//Bit indicating re-start is still in progress
#define	RTC_I2C_STOP_I2C					StopI2C1					//Generate bus stop condition
#define	RTC_I2C_STOP_IN_PROGRESS_BIT		SSP1CON2bits.PEN			//Bit indicating Stop is still in progress
#define	RTC_I2C_WRITE_BYTE(a)				WriteI2C1(a)				//Write byte to I2C device
#define	RTC_I2C_TX_IN_PROGRESS_BIT			SSP1STATbits.R_W			//Bit indicating transmit byte is still in progress
#define	RTC_I2C_ACK_NOT_RECEIVED_BIT		SSP1CON2bits.ACKSTAT		//Bit that is high when ACK was not received
//#define	RTC_I2C_READ_BYTE_START										//Read byte from I2C device function (optional)
#define	RTC_I2C_READ_BYTE					ReadI2C1()					//Read byte from I2C device function / result byte of RTC_I2C_READ_FUNCTION_START
#define RTC_I2C_ACK							AckI2C1						//Generate bus ACK condition
#define RTC_I2C_NOT_ACK						NotAckI2C1					//Generate bus Not ACK condition
#define	RTC_I2C_ACK_IN_PROGRESS_BIT			SSP1CON2bits.ACKEN			//Bit indicating ACK is still in progress
#define	RTC_I2C_IDLE_I2C					IdleI2C1					//Test if I2C1 module is idle (wait until it is ready for next operation)

#endif //#ifdef RTC_USING_PIC18


#ifdef RTC_USING_PIC24
//##################
//##### PIC 24 #####
//##################
#define	RTC_I2C_START_I2C					I2C2CONbits.SEN = 1										//Generate bus start condition
#define	RTC_I2C_START_IN_PROGRESS_BIT		I2C2CONbits.SEN											//Bit indicating start is still in progress
#define	RTC_I2C_RESTART_I2C					I2C2CONbits.RSEN = 1									//Generate bus restart condition
#define	RTC_I2C_RESTART_IN_PROGRESS_BIT		I2C2CONbits.RSEN										//Bit indicating re-start is still in progress
#define	RTC_I2C_STOP_I2C					I2C2CONbits.PEN = 1										//Generate bus stop condition
#define	RTC_I2C_STOP_IN_PROGRESS_BIT		I2C2CONbits.PEN											//Bit indicating Stop is still in progress
#define	RTC_I2C_WRITE_BYTE(a)				I2C2TRN = a												//Write byte to I2C device
#define	RTC_I2C_TX_IN_PROGRESS_BIT			I2C2STATbits.TRSTAT										//Bit indicating transmit byte is still in progress
#define	RTC_I2C_ACK_NOT_RECEIVED_BIT		I2C2STATbits.ACKSTAT									//Bit that is high when ACK was not received
#define	RTC_I2C_READ_BYTE_START				I2C2CONbits.RCEN = 1; while(I2C2STATbits.RBF == 0) ;	//Read byte from I2C device function (optional)
#define	RTC_I2C_READ_BYTE					I2C2RCV													//Read byte from I2C device function / result byte of RTC_I2C_READ_FUNCTION_START
#define RTC_I2C_ACK							I2C2CONbits.ACKDT = 0; I2C2CONbits.ACKEN = 1			//Generate bus ACK condition
#define RTC_I2C_NOT_ACK						I2C2CONbits.ACKDT = 1; I2C2CONbits.ACKEN = 1			//Generate bus Not ACK condition
#define	RTC_I2C_ACK_IN_PROGRESS_BIT			I2C2CONbits.ACKEN										//Bit indicating ACK is still in progress
#define	RTC_I2C_IDLE_I2C					while ((I2C2CON & 0x001F) | (I2C2STATbits.R_W))			//Test if I2C1 module is idle (wait until it is ready for next operation)

#endif //#ifdef RTC_USING_PIC24


#ifdef RTC_USING_PIC32
//##################
//##### PIC 32 #####
//##################
#define	RTC_I2C_START_I2C					StartI2C1					//Generate bus start condition
#define	RTC_I2C_START_IN_PROGRESS_BIT		I2C1CONbits.SEN				//Bit indicating start is still in progress
#define	RTC_I2C_RESTART_I2C					RestartI2C1					//Generate bus restart condition
#define	RTC_I2C_RESTART_IN_PROGRESS_BIT		I2C1CONbits.RSEN			//Bit indicating re-start is still in progress
#define	RTC_I2C_STOP_I2C					StopI2C1					//Generate bus stop condition
#define	RTC_I2C_STOP_IN_PROGRESS_BIT		I2C1CONbits.PEN				//Bit indicating Stop is still in progress
#define	RTC_I2C_WRITE_BYTE(a)				MasterWriteI2C1(a)			//Write byte to I2C device
#define	RTC_I2C_TX_IN_PROGRESS_BIT			I2C1STATbits.TRSTAT			//Bit indicating transmit byte is still in progress
#define	RTC_I2C_ACK_NOT_RECEIVED_BIT		I2C1STATbits.ACKSTAT		//Bit that is high when ACK was not received
//#define	RTC_I2C_READ_BYTE_START										//Read byte from I2C device function (optional)
#define	RTC_I2C_READ_BYTE					MasterReadI2C1()			//Read byte from I2C device function / result byte of RTC_I2C_READ_FUNCTION_START
#define RTC_I2C_ACK							AckI2C1						//Generate bus ACK condition
#define RTC_I2C_NOT_ACK						NotAckI2C1					//Generate bus Not ACK condition
#define	RTC_I2C_ACK_IN_PROGRESS_BIT			I2C1CONbits.ACKEN			//Bit indicating ACK is still in progress
#define	RTC_I2C_IDLE_I2C					IdleI2C1					//Test if I2C1 module is idle (wait until it is ready for next operation)

#endif //#ifdef RTC_USING_PIC32



#endif //RTC_DS1307_C_INIT



//*******************************
//*******************************
//********** FUNCTIONS **********
//*******************************
//*******************************
#ifdef RTC_DS1307_C
//-----------------------------------
//----- INTERNAL ONLY FUNCTIONS -----
//-----------------------------------


//-----------------------------------------
//----- INTERNAL & EXTERNAL FUNCTIONS -----
//-----------------------------------------
//(Also defined below as extern)
//unsigned char flash_read(unsigned long address);
BYTE rtc_set_time  (BYTE *seconds, BYTE *minutes, BYTE *hours, BYTE *day, BYTE *date, BYTE *month, BYTE *year);
BYTE rtc_get_time (BYTE *seconds, BYTE *minutes, BYTE *hours, BYTE *day, BYTE *date, BYTE *month, BYTE *year);


#else
//------------------------------
//----- EXTERNAL FUNCTIONS -----
//------------------------------
//extern unsigned char flash_read(unsigned long address);
extern BYTE rtc_set_time  (BYTE *seconds, BYTE *minutes, BYTE *hours, BYTE *day, BYTE *date, BYTE *month, BYTE *year);
extern BYTE rtc_get_time (BYTE *seconds, BYTE *minutes, BYTE *hours, BYTE *day, BYTE *date, BYTE *month, BYTE *year);


#endif




//****************************
//****************************
//********** MEMORY **********
//****************************
//****************************
#ifdef RTC_DS1307_C
//--------------------------------------------
//----- INTERNAL ONLY MEMORY DEFINITIONS -----
//--------------------------------------------



//--------------------------------------------------
//----- INTERNAL & EXTERNAL MEMORY DEFINITIONS -----
//--------------------------------------------------
//(Also defined below as extern)



#else
//---------------------------------------
//----- EXTERNAL MEMORY DEFINITIONS -----
//---------------------------------------



#endif






