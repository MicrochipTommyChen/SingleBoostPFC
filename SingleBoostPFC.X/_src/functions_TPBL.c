/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    This software and any accompanying information is for suggestion only. It
    does not modify Microchip's standard warranty for its products. You agree
    that you are solely responsible for testing the software and determining its
    suitability. Microchip has no obligation to modify, test, certify, or
    support the software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "xc.h"
#include "../_h/globals.h"
#include "../mcc_generated_files/mcc.h"
#include "../_h/functions_TPBL.h"
#include "../apps/task_fault_handler.h"
#include "../dcdt/iloop/dcdt_generated_code/ccm01_dcdt.h"

//-----------------------------------------------------------------------------
//from main every 1ms
//-----------------------------------------------------------------------------
static uint16_t SecCounterZw = 0;

void Startup(void)
{
  static uint16_t ADC_VAC_Mean_zw = 0, ADC_VOUT_Mean_zw = 0, CounterZw = 0;

  //check for ate least 6 ZC for 2 full sinus waves-----------------------------
  if ((systemFlagStartup._PeriodCount > 10) && (systemFlagStartup._flags._flag._StartupZC) && \
       (systemFlag._flags._flag._AuxOK) && (!systemFlag._flags._flag._Fault))
  {
    systemFlagStartup._flags._flag._StartupZC = 0;
    systemFlagStartup._PeriodCount = 0;
    systemFlagStartup._flags._flag._StartUpVinMeasure = 1;
  }

  //check if Vin is in a certain range------------------------------------------
  if (systemFlagStartup._flags._flag._StartUpVinMeasure)
  {
    if ((ADCValues._VACAvg - ADC_VAC_Mean_zw) < (ADCValues._VACAvg >> 4)) //VAC settled or fixed value ?
    {
      systemFlagStartup._flags._flag._StartUpVinMeasure = 0;
      systemFlagStartup._flags._flag._StartUpVoutMeasure = 1;
    }
    ADC_VAC_Mean_zw = ADCValues._VACAvg;
  }

  //check if Vout is in a certain range-----------------------------------------
  if (systemFlagStartup._flags._flag._StartUpVoutMeasure)
  {
    if ((ADCValues._VOUTAvg - ADC_VOUT_Mean_zw) < (ADCValues._VOUTAvg >> 5))  //Vout settled or fixed value ?
    {
      systemFlagStartup._flags._flag._StartUpVoutMeasure = 0;
      systemFlagStartup._flags._flag._StartUpWaitRelay2s = 1;
    }
    ADC_VOUT_Mean_zw = ADCValues._VOUTAvg;
  }

  //check if 2s delay after for Relay On is over--------------------------------
  if ((systemFlagStartup._flags._flag._StartUpWaitRelay2s) && (!systemFlag._flags._flag._Relay))
  {
    if (SecCounterZw++ > 1000)
    {
      systemFlagStartup._flags._flag._StartUpWaitRelay2s = 0;
      systemFlagStartup._flags._flag._StartUpWaitForRamp = 1;

      SMPS_Controller2P2ZInitialize(&VMC2p2z); // Clear histories    
      SMPS_Controller2P2ZInitialize(&CMC2p2z); // Clear histories
      
      SecCounterZw = 0;
    }
  }
 
  //check if delay after Relay On is over---------------------------------------
  //do ramping up from Vout to Voutref
  if (systemFlagStartup._flags._flag._StartUpWaitForRamp)
  {
    //wait 20ms after Relay on to start switching and Vout ramp and ZC Range
    if (SecCounterZw++ > 100)  //value must be big enough to have an VoutAvg value 
    {      
      systemFlagStartup._flags._flag._StartUpWaitForRamp = 0;
      systemFlagStartup._flags._flag._StartUpIRamp = 1;

#ifdef _DCmode			
      PG1IOCONLbits.OVRENL = 0;
      PG1IOCONLbits.OVRENH = 0;
#endif			
      systemFlag._flags._flag._Stop = 0;
      
      Controller._VDCref = ADCValues._VOUTAvg - (ADCValues._VOUTAvg >> 4); //only works
      
      SecCounterZw = 0;
      
      systemFlag._flags._flag._CompFreeze = 0;
    }
  }

  //check if Current ramp up is done--------------------------------------------
  if (systemFlagStartup._flags._flag._StartUpIRamp)
  {
    systemFlagStartup._flags._flag._StartUpIRamp = 0;
    systemFlagStartup._flags._flag._StartUpVRamp = 1;
  }
  
	//check if voltage ramp up is done--------------------------------------------
	if (systemFlagStartup._flags._flag._StartUpVRamp)
	{
  
    if (Controller._VDCref < VOUTREF)
    {
      if (CounterZw++ > VRampStartupDelay)
      {
        Controller._VDCref += VRampStartupStep;
        CounterZw = 0;
      }
    } 
    else
    {
      Controller._VDCref = VOUTREF;
      systemFlagStartup._flags._flag._StartUpVRamp = 0;
      systemFlagStartup._flags._flag._StartUpDone = 1;
//      LED1_SetHigh(); //LED1 indicates startup done
    }
  }
}

//-----------------------------------------------------------------------------
//from ADC ISR every PWM period
//-----------------------------------------------------------------------------
void ISR_AVG_Calc(void)
{
  ADCValues._VACAvgCounter++;
  ADCValues._VACAvgSum += ADCValues._VACConv;
//  ADCValues._VAUXAvgSum += ADCValues._VAUXraw;
  ADCValues._VOUTAvgSum += ADCValues._VOUTraw;
  ADCValues._IACAvgSum += ADCValues._IACConv;
	ADCValues._V1_5AvgSum += ADCValues._V1_5raw;
 
#ifdef _DCmode  
  if(ADCValues._VACAvgCounter > 1400)  //
#else
  if(((ADCValues._VACAvgCounter > 1) && (!systemFlag._flags._flag._VACPol_last) && (systemFlag._flags._flag._VACPol)) 
     || (ADCValues._VACAvgCounter > 1500)) //
#endif    
    {
      if(systemFlagStartup._flags._flag._StartUpWaitForRamp)  //Startup Relay On at ZC
      {
//        RelayOn_SetHigh();
//        systemFlag._flags._flag._Relay = 1;
        systemFlag._flags._flag._RelayWait = 1;
      }  
      // every period

      //VAC---------------------------------------------------------------------
      ADCValues._VACAvg = (uint16_t) __builtin_divud(ADCValues._VACAvgSum, ADCValues._VACAvgCounter - 1);
      ADCValues._VACAvgSum = 0;
      ADCValues._VACmax = 0;
      if(ADCValues._VACAvg < VACMIN)
      {
        Fault();
        applicationFaults.inputUnderVoltage = 1;
      }  
      else 
      {  
        applicationFaults.inputUnderVoltage = 0;
      }
      
      if(ADCValues._VACAvg > VACMAX)
      {
        Fault();
        applicationFaults.inputOverVoltage = 1;
      }  
      else 
      {  
        applicationFaults.inputOverVoltage = 0;
      }
      
      //VAUX--------------------------------------------------------------------
      // signal not available
      
      //V1_5--------------------------------------------------------------------
      ADCValues._V1_5Avg = (uint16_t) __builtin_divud(ADCValues._V1_5AvgSum, ADCValues._VACAvgCounter - 1);
      ADCValues._V1_5AvgSum = 0;
      if(ADCValues._V1_5Avg > V1_5MAXLEVEL) 
      {
        systemFlag._flags._flag._1_5OK = 0;
//        FTX_Fault();
      }  
      else if(ADCValues._V1_5Avg < V1_5MINLEVEL) 
      {
        systemFlag._flags._flag._1_5OK = 0;
//        FTX_Fault();
      }  
      else if(ADCValues._V1_5Avg > V1_5GOODLEVEL) 
      {  
        systemFlag._flags._flag._1_5OK = 1;
      }
      
      //Vout--------------------------------------------------------------------
      ADCValues._VOUTAvg = (uint16_t) __builtin_divud(ADCValues._VOUTAvgSum, ADCValues._VACAvgCounter - 1);
      ADCValues._VOUTAvgSum = 0;
      if(ADCValues._VOUTAvg < VOUTMIN)
//      if(ADCValues._VOUTraw < VOUTMIN)
      {
        Fault();
        applicationFaults.outputUnderVoltage = 1;
      }  
      else 
      {  
        applicationFaults.outputUnderVoltage = 0;
      }
        
      if(ADCValues._VOUTAvg > VOUTMAX)
//      if(ADCValues._VOUTraw > VOUTMAX)
      {  
        Fault();
        applicationFaults.outputOverVoltage = 1;
      }  
      else 
      {  
        applicationFaults.outputOverVoltage = 0;
      }
      
      //IAC---------------------------------------------------------------------
      ADCValues._IACAvg = (uint16_t) __builtin_divud(ADCValues._IACAvgSum, ADCValues._VACAvgCounter - 1);
      ADCValues._IACAvgSum = 0;
      
      ADCValues._VACAvgCounter_zw = ADCValues._VACAvgCounter; //freeze value to send over UART
      ADCValues._VACAvgCounter = 0;
    
      systemFlagStartup._PeriodCount++;
      //Counts the whole periods
      
      if((applicationFaults.wordWriteFaults > 0) ||     
         (!systemFlag._flags._flag._AuxOK))
      systemFlag._flags._flag._Fault = 1;
      else 
        systemFlag._flags._flag._Fault = 0;
    }
 }
//-----------------------------------------------------------------------------
//from ADC ISR every PWM period
//-----------------------------------------------------------------------------
void ISR_ZC(void)
{
 
#ifndef _DCmode 
  
	//VAC ADC signal is inverted to real AC---------------------------------------
  systemFlag._flags._flag._VACPol_last = systemFlag._flags._flag._VACPol;
  ZCFlags._ZCRange_last = ZCFlags._ZCRange;

	if ((ADCValues._VACraw < ADCValues._V1_5Avg - VACPolRange))
  {
    systemFlag._flags._flag._VACPol = 1;
  } 
  else if ((ADCValues._VACraw > ADCValues._V1_5Avg + VACPolRange))
 {
    systemFlag._flags._flag._VACPol = 0;
  }
  
#else
  systemFlag._flags._flag._VACPol = 1;
  PG1IOCONLbits.SWAP = 1;
#endif

#ifndef _DCmode
  //ZC band for ZC startup handling
  if (systemFlag._flags._flag._AuxOK && !systemFlag._flags._flag._Fault && systemFlag._flags._flag._Relay)
  {
    // phase 1, real AC pos to neg but still pos, STOP (inverted here)
    if ((systemFlag._flags._flag._VACPol) && ((ADCValues._VACraw > (ADCValues._V1_5raw - ZeroCrossRange - 0)))
            && (!ZCFlags._ZCRange))
    {
      if (!systemFlag._flags._flag._BurstMode) //BURST
      {
        PG1IOCONLbits.OVRENH = 1; //PWM OFF
        PG1IOCONLbits.OVRENL = 1; //PWM OFF
      }
      systemFlag._flags._flag._Stop = 1;
      ZCFlags._ZCRange = 1;
    }      // phase 2, real AC pos to neg but just neg, RESTART (inverted here)
    else if ((!systemFlag._flags._flag._VACPol) && ((ADCValues._VACraw > (ADCValues._V1_5raw + ZeroCrossRange + 40)))
            && (ZCFlags._ZCRange))
    {
      PG1IOCONLbits.SWAP = 0;
      if (!systemFlag._flags._flag._BurstMode) //BURST
      {
        if (systemFlagStartup._flags._flag._StartUpDone) PG1IOCONLbits.OVRENH = 0;
        else PG1IOCONLbits.OVRENH = 1;
        PG1IOCONLbits.OVRENL = 0; //1;
      }
      ZCFlags._SoftStart_ZC = 1;
      systemFlag._PWM_Counter = 1;
      systemFlag._flags._flag._Stop = 0;
      ZCFlags._ZCRange = 0;
    }      // phase 3, real AC neg to pos but still neg, STOP (inverted here)
    else if ((!systemFlag._flags._flag._VACPol) && ((ADCValues._VACraw < (ADCValues._V1_5raw + ZeroCrossRange + 0)))
            && (!ZCFlags._ZCRange))
    {
      if (!systemFlag._flags._flag._BurstMode) 
      {
        PG1IOCONLbits.OVRENH = 1; //PWM OFF
        PG1IOCONLbits.OVRENL = 1; //PWM OFF
      }
      systemFlag._flags._flag._Stop = 1;
      ZCFlags._ZCRange = 1;
    }      // phase 4, real AC neg to pos but just pos, RESTART (inverted here)
    else if ((systemFlag._flags._flag._VACPol) && ((ADCValues._VACraw < (ADCValues._V1_5raw - ZeroCrossRange - 40)))
            && (ZCFlags._ZCRange))
    {
      PG1IOCONLbits.SWAP = 1;
      if (!systemFlag._flags._flag._BurstMode) 
      {
        PG1IOCONLbits.OVRENH = 0; 
        if (systemFlagStartup._flags._flag._StartUpDone) PG1IOCONLbits.OVRENL = 0;
        else PG1IOCONLbits.OVRENL = 1;
      }
      ZCFlags._SoftStart_ZC = 1;
      systemFlag._PWM_Counter = 1;
      systemFlag._flags._flag._Stop = 0;
      ZCFlags._ZCRange = 0;
    }
  }
  else
  {
    if (!systemFlag._flags._flag._BurstMode) 
    {
      PG1IOCONLbits.OVRENH = 1; //PWM OFF
      PG1IOCONLbits.OVRENL = 1; //PWM OFF    
    }
  }
#endif  

}

//-----------------------------------------------------------------------------
//from ADC ISR every PWM period
//-----------------------------------------------------------------------------

void ISR_Startup_After_ZC(void)
{
  if (!systemFlag._flags._flag._BurstMode)  
  {
    if ((ZCFlags._SoftStart_ZC) &&
            (systemFlagStartup._flags._flag._StartUpVRamp || systemFlagStartup._flags._flag._StartUpDone))
    {
      if (systemFlag._PWM_Counter < 10) 
      {
        if (systemFlag._flags._flag._VACPol)
        {
          PG1IOCONLbits.OVRENH = 1; //PWM OFF
          PG1IOCONLbits.OVRENL = 0; //PWM ON
        }
        else 
        {  
          PG1IOCONLbits.OVRENL = 1; //PWM OFF
          PG1IOCONLbits.OVRENH = 0; //PWM ON
        }
        
        PG1DC = (systemFlag._PWM_Counter << 11);
        if ((PG1DC > Controller._DCset))
        {
          systemFlag._PWM_Counter = 20;
        }
      }
      else
      {
        ZCFlags._SoftStart_ZC = 0;  //check startup must be OFF 
      }
    }
  }
}

//-----------------------------------------------------------------------------
//Current Controller
//-----------------------------------------------------------------------------

uint32_t var3f_ul, var4f_ul;
uint16_t var3f_ui, var4f_ui;

void CurrentController(void)
{
  Controller._Vinavg2 = (uint16_t) (__builtin_muluu(ADCValues._VACAvg, ADCValues._VACAvg) >> 7); //FTX >> 7

  Controller._IACrefL = (__builtin_muluu((ADCValues._VACConv << 4), Controller._VPI)); // 11Bit * 11Bit = 22Bit
  if (Controller._Vinavg2 > 0)
  {
    Controller._IACref = (uint16_t) (__builtin_divud(Controller._IACrefL, Controller._Vinavg2) >> 4); // 22Bit / 11Bit = 11Bit; scaled to 8 Bit (240=3A)) >>3
  }
  if ((Controller._IACref) > IinREF) Controller._IACref = IinREF;
 
  SMPS_Controller2P2ZUpdate(&CMC2p2z, &ADCValues._IACConv, Controller._IACref, &Controller._DCset);
  
#ifdef _DCFF  
  //Duty Cycle Feedforward-----------------------------------------------------------
  if (!systemFlag._flags._flag._dffOff)
  {
    if ((ADCValues._VACConv > 0) && (ADCValues._VOUTAvg > VOUTMIN) )
    {
      var3f_ul = (uint32_t) (ADCValues._VOUTAvg) - (ADCValues._VACConv); //Vout - Vin
      var3f_ul *= PG1PER;
      var3f_ui = __builtin_divud(var3f_ul, (ADCValues._VOUTAvg));
      var4f_ui = Controller._DCset + (var3f_ui >> 0);
      if (var4f_ui < CCM01_COMP_2P2Z_MAX_CLAMP)
      {
        Controller._DCset = var4f_ui;
      }
      else Controller._DCset = CCM01_COMP_2P2Z_MAX_CLAMP; 
    }
  }
  //Duty Cycle Feedforward-----------------------------------------------------------
#endif  

  PG1TRIGA = Controller._DCset >> 1;

  if ((!systemFlag._flags._flag._Stop) && (!ZCFlags._SoftStart_ZC))
  {
    PG1DC = Controller._DCset;
  }
}
//-----------------------------------------------------------------------------
//Voltage Controller
//-----------------------------------------------------------------------------

void VoltageController(void)
{
	ADCValues._VOUTAvgVLOOP = ADCValues._VOUTAvg << 3;
	ADCValues._VOUTAvgVLOOP = ADCValues._VOUTraw << 3;
	Controller._VDCrefVLOOP = Controller._VDCref << 3;
//	SMPS_Controller2P2ZUpdate(&VMC2p2z, &ADCValues._VOUTAvgVLOOP, Controller._VDCrefVLOOP, &Controller._VPI);
	SMPS_Controller2P2ZUpdate_BSM_VCM(&VMC2p2z, &ADCValues._VOUTAvgVLOOP, Controller._VDCrefVLOOP, &Controller._VPI);
 }

//-----------------------------------------------------------------------------
//  Burst Mode
//-----------------------------------------------------------------------------

void BurstMode (void)
{
  uint16_t BurstActiveCounter = 0;
  
  if((Controller._VPI < 10) && (!systemFlag._flags._flag._BurstMode) && 
     (systemFlagStartup._flags._flag._StartUpDone)) 
 {  
    systemFlag._flags._flag._BurstModeDetect = 1;
  }  
  if(systemFlag._flags._flag._BurstModeDetect && (systemFlag._flags._flag._VACPol_last==1) && (systemFlag._flags._flag._VACPol==0))
  {
    systemFlag._flags._flag._BurstModeDetect = 0;
    systemFlag._flags._flag._BurstMode = 1; 
    PG1IOCONLbits.OVRENH = 1; //PWM OFF
    PG1IOCONLbits.OVRENL = 1; //PWM OFF
//    SW_SR_L_SetLow();
//    SW_SR_H_SetLow();
    
    Controller._VPI = 10;
    systemFlag._flags._flag._CompFreeze = 1;  
  }
  
  if(systemFlag._flags._flag._BurstMode && (systemFlag._flags._flag._VACPol_last==1) && (systemFlag._flags._flag._VACPol==0))
  {
    ADCValues._BurstCounter++;
  }  

  if(ADCValues._VOUTAvg > VOUTREFBurst3)
      systemFlag._flags._flag._dffOff = 1; 
  else if(ADCValues._VOUTAvg > VOUTREFBurst2)
    BurstActiveCounter = 4;
   else if(ADCValues._VOUTAvg > VOUTREFBurst1)
    BurstActiveCounter = 2;
  else
    BurstActiveCounter = 1;
 
 
  if (ADCValues._BurstCounter > BurstActiveCounter)
  {
    systemFlag._flags._flag._BurstMode = 0;
    ADCValues._BurstCounter = 0;
    systemFlag._flags._flag._CompFreeze = 0; //1_2_20
    systemFlag._flags._flag._dffOff = 0; 
  }  

}

//-----------------------------------------------------------------------------
// DCM Mode check. If DCM run asynchron 
//-----------------------------------------------------------------------------

uint32_t var1_ul, var2_ul, var3_ul, var4_ul;
uint16_t var1_ui, var2_ui, var3_ui, var4_ui;

void DCMMode (void)
{
  if (!systemFlag._flags._flag._BurstMode)  //BURST
  {
    if ((systemFlag._flags._flag._Relay) && (systemFlagStartup._flags._flag._StartUpDone) 
         &&(!ZCFlags._ZCRange))  //
    {
      var1_ul = (uint32_t) ADCValues._VACConv * PG1PER;
      var1_ui = (uint16_t) (var1_ul >> 10); //Vin * Period / 1024

      var2_ul = (uint32_t) ADCValues._VOUTAvg * (PG1PER - PG1DC);
      var2_ui = (uint16_t) (var2_ul >> 10);
  
      if (var1_ui < var2_ui)
      {
        if (systemFlag._flags._flag._VACPol)
        {
          PG1IOCONLbits.OVRENH = 1; //PWM OFF
#ifdef _SynchSw        
//          SW_SR_L_SetLow();
#endif
        }
        else
        {
          PG1IOCONLbits.OVRENL = 1; //PWM OFF
#ifdef _SynchSw
//          SW_SR_H_SetLow();
#endif
        }
      }
      else
      {
        if (systemFlag._flags._flag._VACPol)
        {
         PG1IOCONLbits.OVRENH = 0; //PWM ON
#ifdef _SynchSw
//          SW_SR_L_SetHigh();
#endif
        }
        else
        {
           PG1IOCONLbits.OVRENL = 0; //PWM ON
#ifdef _SynchSw
//          SW_SR_H_SetHigh();
#endif
         }
      }
    }
  }
}

//-----------------------------------------------------------------------------
//Check for Vac drop to calculate timing between ZC range events
//-----------------------------------------------------------------------------

void VacDrop(void)
{
  ZCVals._ZCTimeDiff++;
     
  if (ZCFlags._ZCRange == 1)
  {  
    ZCVals._ZCRangeTime++;
  }

  if ((ZCVals._ZCRangeTime > 52)) // 52 * 1/66kHz = 800us
  {
    Fault();
    //applicationFaults.vacDrop = 1; //used for debug
  }

  if ((ZCFlags._ZCRange_last == 0) && (ZCFlags._ZCRange == 1))
  {
    if ((ZCVals._ZCTimeDiff < maxfreq) &&
       (systemFlagStartup._flags._flag._StartUpVRamp || systemFlagStartup._flags._flag._StartUpDone))
    {
      Fault();
      //applicationFaults.vacDrop = 1; //used for debug
    }
    ZCVals._ZCTimeDiff = 0;
    ZCVals._ZCRangeTime = 0;
  }
  
}

//-----------------------------------------------------------------------------
//Fault with whole startup procedure
//-----------------------------------------------------------------------------

void Fault(void)
{
 
  PG1IOCONLbits.OVRENH = 1; //PWM OFF
  PG1IOCONLbits.OVRENL = 1; //PWM OFF
//  SW_SR_L_SetLow();
//  SW_SR_H_SetLow();
//  RelayOn_SetLow();
  PG1DC = 0;
  
  Nop();
  Nop();
  Nop();
  Nop();
  
  systemFlagStartup._flags._StartupFlagsWord = 0x0000;
  systemFlagStartup._flags._flag._StartupZC = 1;  
  systemFlagStartup._PeriodCount = 0;
  
  Controller._VDCref = 0;
  Controller._VPI = 0;  
	Controller._IACref = 0;
  
  systemFlag._flags._flag._CompFreeze = 1;  //without startup after Vac off delayed because no VPI/Iacref
  systemFlag._flags._flag._dffOff = 0; 
   
  systemFlag._flags._flag._Fault = 1;
  systemFlag._flags._flag._Stop = 1;
  systemFlag._flags._flag._Relay = 0;
  systemFlag._flags._flag._RelayWait = 0;
  ZCFlags._ZCRange = 0;
  ZCFlags._SoftStart_ZC = 0;
  systemFlag._RelayDelay = 0x0000;
  systemFlag._PWM_Counter = 0x0000;
  
  ZCVals._ZCTimeDiff = 0;
  ZCVals._ZCRangeTime = 0;

  applicationFaults.vacDrop = 0;
  
//  LED1_SetLow();

}

//-----------------------------------------------------------------------------
//init from main
//-----------------------------------------------------------------------------

void InitFlags(void)
{  
  systemFlagStartup._flags._flag._StartupZC = 0;
  systemFlagStartup._flags._flag._StartUpVinMeasure = 0;
  systemFlagStartup._flags._flag._StartUpVoutMeasure = 0;
  systemFlagStartup._flags._flag._StartUpWaitRelay2s = 0;
  systemFlagStartup._flags._flag._StartUpWaitForRamp = 0;
  systemFlagStartup._flags._flag._StartUpIRamp = 0;
  systemFlagStartup._flags._flag._StartUpVRamp = 0;
  systemFlagStartup._flags._flag._StartUpDone = 0;

  systemFlag._flags._flag._Relay = 0;
  systemFlag._flags._flag._RelayWait = 0;
  ZCFlags._ZCRange = 0;
  ZCFlags._SoftStart_ZC = 0;
  systemFlagStartup._PeriodCount = 0x0000;
  systemFlag._RelayDelay = 0x0000;
  systemFlag._PWM_Counter = 0x0000;
  ZCVals._ZCTimeDiff = 0;
  ZCVals._ZCRangeTime = 0;
  
  fault_check_init();

  systemFlag._flags._flag._Stop = 1;
  systemFlag._flags._flag._Fault = 1;
  systemFlag._flags._flag._AuxOK = 1;
  systemFlag._flags._flag._BurstMode = 0; 
  systemFlag._flags._flag._CompFreeze = 1;
  systemFlag._flags._flag._dffOff = 0; 

  Controller._VDCref = 0;
  Controller._VPI = 0;  
	Controller._IACref = 0;
	
  ADCValues._ISRCountCHSWITCH = 0;
  ADCValues._ISRCountVLOOP = 0;
  
//  LED1_SetLow();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
