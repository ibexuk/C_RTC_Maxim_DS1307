
######################################################################
##### Open sourced by IBEX, an electronic product design company #####
#####    based in South East England.  http://www.ibexuk.com     #####
######################################################################


The Dallas Maxim DS1307 is a I2C serial bus real time clock. It provides low power time keeping for embedded designs.

This driver provides the following functions:

----- SET REAL TIME CLOCK TIME -----
BYTE rtc_set_time (BYTE *seconds, BYTE *minutes, BYTE *hours, BYTE *day, BYTE *date, BYTE *month, BYTE *year)
The fields are converted from binary to BCD before being sent to the RTC
All fields are 0 – #, except day, date and month, which are 1 – #
Pointers are used for comanilty with rtc_get_time
Uses 24 hour clock mode

----- GET REAL TIME CLOCK TIME -----
BYTE rtc_get_time (BYTE *seconds, BYTE *minutes, BYTE *hours, BYTE *day, BYTE *date, BYTE *month, BYTE *year)
Returns
1 if sucessful, 0 if not.
All fields are converted to binary values.
All fields are 0 – #, except day, date and month, which are 1 – #
Uses 24 hour clock mode

Please view the source code files for full information regarding the driver.