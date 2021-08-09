/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TYPES_H
#define	TYPES_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include "smps_control.h"
//------------------------------------------------------------------------------

typedef union 
{
   struct
   {
        uint16_t _ZCRange       : 1;
        uint16_t _ZCRange_last  : 1;
        uint16_t _SoftStart_ZC  : 1;
        uint16_t _unused        : 13;
   };
   uint16_t wordZCFlags;
}t_ZCFlags;

typedef struct
{
   uint16_t _ZCTimeDiff;
   uint16_t _ZCRangeTime;
}t_ZCVals;

//------------------------------------------------------------------------------
typedef struct
{
   int16_t   fltThreshold;        // Fault threshold limit: Range of -32768 to 32767
   int16_t   fltHysLimit;         // Fault hysteresis limit. If hysteresis is not needed, fltThreshold = fltHysLimit
   uint16_t  fltThresholdCnt;     // Number of consecutive fault events before fault becomes active.  
   uint16_t  fltHysCnt;           // Number of consecutive events when flt input outside hysteresis limit in order to remove fault, set to 0 to disable Hys 
   uint16_t  fltCounter;          // Internal counter for activating/removing fault 
   uint16_t  fltCode;             // Code that can be used to display fault (1st fault occurred) to global variable
   uint8_t   faultActive;         // Set/Cleared inside flt check loop
        
}FLT_STATE;   

typedef union
{
    struct
    {
        uint16_t inputUnderVoltage      :1;
        uint16_t inputOverVoltage       :1;

        uint16_t outputUnderVoltage     :1;
        uint16_t outputOverVoltage      :1;

        uint16_t inputOverCurrent       :1;

        uint16_t auxUnderVoltage        :1;
        uint16_t auxOverVoltage         :1;

        uint16_t zeroRefUnderVoltage    :1;
        uint16_t zeroRefOverVoltage     :1;
        
        uint16_t vacDrop                :1;
        uint16_t catastrophicFault      :1;

        uint16_t unused                 :5;  
    };
    uint16_t wordWriteFaults;
} APPS_FAULTS;

//------------------------------------------------------------------------------

typedef volatile struct 
{
   uint16_t _RelayDelay;
   uint16_t _PWM_Counter;

   union 
   {
      struct 
      {
        uint16_t _VACPol            : 1;    //bit 0
        uint16_t _VACPol_last       : 1;
    
        uint16_t _Stop              : 1;
        uint16_t _Fault             : 1;

        uint16_t _AuxOK             : 1;    //bit5

        uint16_t _Relay             : 1;
        uint16_t _1_5OK             : 1;
        uint16_t _RelayWait         : 1;
        
        uint16_t _BurstModeDetect   : 1;    //bit 8
        uint16_t _BurstMode         : 1;    
        uint16_t _CompFreeze        : 1;    
        uint16_t _dffOff            : 1;    
        
        uint16_t _NotUsed           : 4;

       }_flag;
      uint16_t _FlagsWord;
   }_flags;
   
} t_systemFlags;
//------------------------------------------------------------------------------

typedef volatile struct 
{
    uint16_t _PeriodCount;

    union {

        struct {
            uint16_t _StartupZC             : 1;    //bit 0
            uint16_t _StartUpVinMeasure     : 1;    //bit 1
            uint16_t _StartUpVoutMeasure    : 1;    //Bit 2

            uint16_t _StartUpWaitRelay2s    : 1;    //bit 3
            uint16_t _StartUpWaitForRamp    : 1;    //bit 4

            uint16_t _StartUpIRamp          : 1;    //bit 5
            uint16_t _StartUpVRamp          : 1;    //bit 6
            uint16_t _StartUpDone           : 1;    //bit 7
            
            uint16_t _StartupVPIwait        : 1;    //bit 8
            uint16_t _StartupDCrefwait      : 1;    //bit 9

            uint16_t _none                  : 6;    //0xXXxx
            
        } _flag;
        uint16_t _StartupFlagsWord;
    } _flags;

} t_systemFlagsStartup;
//------------------------------------------------------------------------------
typedef volatile struct
{
    uint16_t _VACraw;
    uint16_t _VACConv;
    uint16_t _VACAvg;
    uint32_t _VACAvgSum;
    uint16_t _VACAvgCounter;
    uint16_t _VACAvgCounter_zw;
    uint16_t _VACmax;

    uint16_t _VACZCFilter;
    uint16_t _VACZCFilterCounter;
    
    uint16_t _IACraw;
    uint16_t _IACrawFiltered;
    uint16_t _IACConv;
    int16_t  _IACConvILOOP;
    uint16_t _IACAvg;
    uint32_t _IACAvgSum;
    
    uint16_t _VOUTraw;
    uint16_t _VOUTAvg;
    uint16_t _VOUTAvgVLOOP;
    uint32_t _VOUTAvgSum;    
    
//    uint16_t _VAUXraw;
//    uint16_t _VAUXAvg;
//    uint32_t _VAUXAvgSum;    

    uint16_t _V1_5raw;
    uint16_t _V1_5Avg;
    uint32_t _V1_5AvgSum; 
    
    uint16_t _VNTCraw;
    
    uint16_t _ISRCountVLOOP;
    uint16_t _ISRCountCHSWITCH;
    
    uint16_t _BurstCounter;
    
} t_ADCValues;
//------------------------------------------------------------------------------
typedef volatile struct
{
    uint16_t _VDCref;
    uint16_t _VDCrefVLOOP;
    uint16_t _VPI;
    uint16_t _VPITestzw;
    uint16_t _VPIStartUp;   
    uint16_t _Vinavg2;
    uint32_t _IACrefL;
    uint16_t _IACref;
    
    uint16_t _DCset;
    
} t_Controller;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#endif	/* XC_HEADER_TEMPLATE_H */

