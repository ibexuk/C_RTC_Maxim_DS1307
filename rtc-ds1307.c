/*
IBEX UK Ltd http://www.ibexuk.com
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


#include "main.h"					//Global data type definitions (see https://github.com/ibexuk/C_Generic_Header_File )
#define RTC_DS1307_C
#include "rtc-ds1307.h"




//**********************************************
//**********************************************
//********** SET REAL TIME CLOCK TIME **********
//**********************************************
//**********************************************
//The fields are converted from binary to BCD before being sent to the RTC
//All fields are 0 - #, except day, date and month, which are 1 - #
//Pointers are used for comanilty with rtc_get_time
//Uses 24 hour clock, not 12 hour
BYTE rtc_set_time  (BYTE *seconds, BYTE *minutes, BYTE *hours, BYTE *day, BYTE *date, BYTE *month, BYTE *year)
{
	BYTE temp;
	BYTE temp1;

	//Send the start condition
	RTC_I2C_IDLE_I2C();
	RTC_I2C_START_I2C();
	while (RTC_I2C_START_IN_PROGRESS_BIT);

	//Send the address with the write bit set
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(DS1307_I2C_ADDRESS & 0xfe);	//Bit 0 low for write
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Send the register address
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(0x00);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Write seconds
	if (*seconds > 59)						//Ensure value is in range
		goto rtc_set_time_fail;
	temp1 = (*seconds / 10);
	temp = (*seconds - (temp1 * 10)) + (temp1 << 4);
	temp &= 0x7f;							//Bit7 = enable oscillator
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(temp);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Write minutes
	if (*minutes > 59)						//Ensure value is in range
		goto rtc_set_time_fail;

	temp1 = (*minutes / 10);
	temp = (*minutes - (temp1 * 10)) + (temp1 << 4);
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(temp);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Write hours
	if (*hours > 23)					//Ensure value is in range
		goto rtc_set_time_fail;

	temp1 = (*hours / 10);
	temp = (*hours - (temp1 * 10)) + (temp1 << 4);
	temp &= 0x3f;						//Bit6 low = set format to 24 hour
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(temp);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Write day
	if (*day > 7)						//Ensure value is in range
		goto rtc_set_time_fail;
	if (*day == 0)
		goto rtc_set_time_fail;

	temp1 = (*day / 10);
	temp = (*day - (temp1 * 10)) + (temp1 << 4);
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(temp);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Write date
	if (*date > 31)						//Ensure value is in range
		goto rtc_set_time_fail;

	temp1 = (*date / 10);
	temp = (*date - (temp1 * 10)) + (temp1 << 4);
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(temp);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Write month
	if (*month > 12)					//Ensure value is in range
		goto rtc_set_time_fail;

	temp1 = (*month / 10);
	temp = (*month - (temp1 * 10)) + (temp1 << 4);
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(temp);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Write year
	if (*year > 99)						//Ensure value is in range
		goto rtc_set_time_fail;

	temp1 = (*year / 10);
	temp = (*year - (temp1 * 10)) + (temp1 << 4);
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(temp);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Write control
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(0x00);				//0x00 = square wave output off
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_set_time_fail;

	//Send Stop
	RTC_I2C_IDLE_I2C();
	RTC_I2C_STOP_I2C();
	while (RTC_I2C_STOP_IN_PROGRESS_BIT)
		;
	
	return (1);

//----- I2C COMMS FAILED -----
rtc_set_time_fail:

	//Send Stop
	RTC_I2C_IDLE_I2C();
	RTC_I2C_STOP_I2C();
	while (RTC_I2C_STOP_IN_PROGRESS_BIT)
		;

	return (0);
}




//**********************************************
//**********************************************
//********** GET REAL TIME CLOCK TIME **********
//**********************************************
//**********************************************
//Returns:
//	1 if sucessful, 0 if not.
//All fields are converted to binary values
//All fields are 0 - #, except day, date and month, which are 1 - #
//Uses 24 hour clock, not 12 hour
BYTE rtc_get_time (BYTE *seconds, BYTE *minutes, BYTE *hours, BYTE *day, BYTE *date, BYTE *month, BYTE *year)
{

	BYTE temp;


	//Send the start condition
	RTC_I2C_IDLE_I2C();
	RTC_I2C_START_I2C();
	while (RTC_I2C_START_IN_PROGRESS_BIT)
		;

	//Send the address with the write bit set
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(DS1307_I2C_ADDRESS & 0xfe);	//Bit 0 low for write
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_get_time_fail;

	//Send the register address
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(0x00);
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_get_time_fail;

	//Send restart condition
	RTC_I2C_IDLE_I2C();
	RTC_I2C_RESTART_I2C();
	while (RTC_I2C_RESTART_IN_PROGRESS_BIT)
		;

	//Send the address with the read bit set
	RTC_I2C_IDLE_I2C();
	RTC_I2C_WRITE_BYTE(DS1307_I2C_ADDRESS | 0x01);	//Bit 1 high for read
	while (RTC_I2C_TX_IN_PROGRESS_BIT)
		;
	if (RTC_I2C_ACK_NOT_RECEIVED_BIT)
		goto rtc_get_time_fail;

	//Read seconds
	RTC_I2C_IDLE_I2C();
	#ifdef RTC_I2C_READ_BYTE_START
		RTC_I2C_READ_BYTE_START
	#endif
	temp = RTC_I2C_READ_BYTE;
	*seconds = (temp & 0x0f) + (((temp & 0x70) >> 4) * 10);		//(Bit 7 is osc flag bit - dump)
	RTC_I2C_ACK();					//Send Ack
	while (RTC_I2C_ACK_IN_PROGRESS_BIT)
		;

	//Read minutes
	RTC_I2C_IDLE_I2C();
	#ifdef RTC_I2C_READ_BYTE_START
		RTC_I2C_READ_BYTE_START
	#endif
	temp = RTC_I2C_READ_BYTE;
	*minutes = (temp & 0x0f) + (((temp & 0x70) >> 4) * 10);
	RTC_I2C_ACK();					//Send Ack
	while (RTC_I2C_ACK_IN_PROGRESS_BIT)
		;

	//Read hours
	RTC_I2C_IDLE_I2C();
	#ifdef RTC_I2C_READ_BYTE_START
		RTC_I2C_READ_BYTE_START
	#endif
	temp = RTC_I2C_READ_BYTE;
	*hours = (temp & 0x0f) + (((temp & 0x30) >> 4) * 10);
	RTC_I2C_ACK();					//Send Ack
	while (RTC_I2C_ACK_IN_PROGRESS_BIT)
		;

	//Read day
	RTC_I2C_IDLE_I2C();
	#ifdef RTC_I2C_READ_BYTE_START
		RTC_I2C_READ_BYTE_START
	#endif
	temp = RTC_I2C_READ_BYTE;
	*day = (temp & 0x07);
	RTC_I2C_ACK();					//Send Ack
	while (RTC_I2C_ACK_IN_PROGRESS_BIT)
		;

	//Read date
	RTC_I2C_IDLE_I2C();
	#ifdef RTC_I2C_READ_BYTE_START
		RTC_I2C_READ_BYTE_START
	#endif
	temp = RTC_I2C_READ_BYTE;
	*date = (temp & 0x0f) + (((temp & 0x30) >> 4) * 10);
	RTC_I2C_ACK();					//Send Ack
	while (RTC_I2C_ACK_IN_PROGRESS_BIT)
		;

	//Read month
	RTC_I2C_IDLE_I2C();
	#ifdef RTC_I2C_READ_BYTE_START
		RTC_I2C_READ_BYTE_START
	#endif
	temp = RTC_I2C_READ_BYTE;
	*month = (temp & 0x0f) + (((temp & 0x10) >> 4) * 10);
	RTC_I2C_ACK();					//Send Ack
	while (RTC_I2C_ACK_IN_PROGRESS_BIT)
		;

	//Read year
	RTC_I2C_IDLE_I2C();
	#ifdef RTC_I2C_READ_BYTE_START
		RTC_I2C_READ_BYTE_START
	#endif
	temp = RTC_I2C_READ_BYTE;
	*year = (temp & 0x0f) + ((temp >> 4) * 10);
	//RTC_I2C_ACK();					//Send Ack
	//while (RTC_I2C_ACK_IN_PROGRESS_BIT)
	//	;

	//Send NAK
	RTC_I2C_IDLE_I2C();
	RTC_I2C_NOT_ACK();
	while (RTC_I2C_ACK_IN_PROGRESS_BIT)
		;

	//Send Stop
	RTC_I2C_IDLE_I2C();
	RTC_I2C_STOP_I2C();
	while (RTC_I2C_STOP_IN_PROGRESS_BIT)
		;
	
	return (1);

//----- I2C COMMS FAILED -----
rtc_get_time_fail:

	*seconds = 0;
	*minutes = 0;
	*hours = 0;
	*day = 0;
	*date = 1;
	*month = 1;
	*year = 0;

	//Send Stop
	RTC_I2C_IDLE_I2C();
	RTC_I2C_STOP_I2C();
	while (RTC_I2C_STOP_IN_PROGRESS_BIT)
		;

	return (0);
}
