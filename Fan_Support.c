#include <p18f4620.h>
#include "Main.h"
#include "Fan_Support.h"
#include "stdio.h"

#define RPM_RGB_RED   PORTEbits.RE0
#define RPM_RGB_GREEN PORTEbits.RE1
#define FAN_LED PORTEbits.RE2

extern char FAN;
extern char HEATER;
extern char duty_cycle;
extern char heater_set_temp;
extern signed int DS1621_tempF;

int get_duty_cycle(signed int temp, int set_temp)
{
    if(temp > set_temp) duty_cycle = 0;     // Check if temp is greater than set_temp. If so, duty_cycle = 0
    else duty_cycle = 2 * (set_temp - temp);// Else dc = 2 times of difference of set_temp and temp
    
    if(duty_cycle > 100) duty_cycle = 100;  // Check if dc is greater than 100. If so, set it to 100
    return duty_cycle;                          
}       

void Monitor_Heater()
{
    duty_cycle = get_duty_cycle(DS1621_tempF, heater_set_temp);
    do_update_pwm(duty_cycle);
    if (FAN == 0) Turn_Off_Fan();
    else Turn_On_Fan();
}

void Toggle_Heater()
{
    if (FAN == 0) FAN = 1; // Flip FAN variable
    else FAN = 0;
}

int get_RPM()
{
    T3CON = 0x03;
    int RPS = TMR3L / 2; // read the count. Since there are 2 pulses per rev
                         // then RPS = count /2
    TMR3L = 0;           // clear out the count
    return (RPS * 60);   // return RPM = 60 * RPS
}

void Turn_On_Fan()
{
    FAN = 1;
    do_update_pwm(duty_cycle);
    FAN_EN = 1;
    FAN_LED = 1;

}

void Turn_Off_Fan()
{
    FAN = 0;
    FAN_EN = 0;
    FAN_LED = 0;
}

void Set_RPM_RGB(int rpm)
{
    if(rpm == 0)                        // OFF
    {
        RPM_RGB_RED = 0;
        RPM_RGB_GREEN = 0;
    }
    else if(rpm > 0 && rpm < 1800)      // RED
    {
        RPM_RGB_RED = 1;
        RPM_RGB_GREEN = 0;
    }
    else if(rpm >= 1800 && rpm < 2700)  // YELLOW
    {
        RPM_RGB_RED = 1;
        RPM_RGB_GREEN = 1;
    }
    else if(rpm >= 2700)                // GREEN
    {
        RPM_RGB_RED = 0;
        RPM_RGB_GREEN = 1;
    }  
}



