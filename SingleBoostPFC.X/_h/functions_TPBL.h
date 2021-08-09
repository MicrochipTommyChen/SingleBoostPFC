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

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef FUNCTIONS_TPBL_H
#define	FUNCTIONS_TPBL_H

#include <xc.h> // include processor files - each processor file is guarded.  

void Startup(void);
void InitFlags(void);
void ISR_AVG_Calc(void);
void ISR_ZC(void);
void ISR_Startup_After_ZC(void);
void Fault(void);
void CurrentController(void);
void VoltageController(void);
void BurstMode(void);
void DCMMode(void);
void VacDrop(void);

void UARTsend(void);
void UART_load_for_send(void);

// setup different options -----------------------------------------------------
#define _DCFF
#define _DCMdetect
#define _BURSTMode
//#define _DCmode
#ifndef _DCmode
    #define _SynchSw
#endif
#define _50Hz

//#define _LVmode
//------------------------------------------------------------------------------

#ifdef _LVmode

#define fVOUTREF (float)((45*7.5/240/3.3)*4096) //120V
#define fVOUTMIN (float)((15*7.5/240/3.3)*4096) //10V
#define fVOUTMAX (float)((85*7.5/240/3.3)*4096) //xxxV
#define fVOUTSTART (float)((25*7.5/240/3.3)*4096) //20V

#define fVACMIN (float)((10*7.5/240/3.3)*4096) //5V
#define fVACMAX (float)((28*7.5/240/3.3)*4096) //xxV

#define VRampStartupDelay 5//5
#define VRampStartupStep 1//4
#define VRampStartupDelayVPI 3
#define VRampStartupStepVPI 1

//Change current----------------------------------------------------------
#define fIinREF (float)(2 * 38.825)     // 3A     
//Change Over current limit-----------------------------------------------
#define fIinOC (float)(fIinREF * 1.5)   //Iref * 1.5

#else

#define fVOUTREF (float)((390*7.5/2200/3.3)*4096) 
#define fVOUTMIN (float)((120*7.5/2200/3.3)*4096) 
#define fVOUTMAX (float)((420*7.5/2200/3.3)*4096) 
#define fVOUTSTART (float)((200*7.5/2200/3.3)*4096) 

#define fVACMIN (float)((50*7.5/2200/3.3)*4096) //5V
#define fVACMAX (float)((270*7.5/2200/3.3)*4096) //xxV

#define VRampStartupDelay 5//15
#define VRampStartupStep 1//1
#define VRampStartupDelayVPI 1
#define VRampStartupStepVPI 1

//Change AVG current----------------------------------------------------------
#define fIinREF (float)(30*32.22)       
//Change Over current limit-----------------------------------------------
#define fIinOC (float)(fIinREF * 1.2)  

#endif

#define VOUTREF (uint16_t)fVOUTREF  //
#define VOUTMIN (uint16_t)fVOUTMIN  //
#define VOUTMAX (uint16_t)fVOUTMAX  //
#define VOUTSTART (uint16_t)fVOUTSTART  //

#define VACMIN (uint16_t)fVACMIN  //
#define VACMAX (uint16_t)fVACMAX  //

#define IinREF  (uint16_t)fIinREF  
#define IinOC  (uint16_t)fIinOC 

#define fVOUTREFBurst1 (float)(fVOUTREF + (fVOUTREF*0.01)) //120V
#define VOUTREFBurst1  (uint16_t)fVOUTREFBurst1   //
#define fVOUTREFBurst2 (float)(fVOUTREF + (fVOUTREF*0.03)) //120V
#define VOUTREFBurst2  (uint16_t)fVOUTREFBurst2   //
#define fVOUTREFBurst3 (float)(fVOUTREF + (fVOUTREF*0.05)) //120V
#define VOUTREFBurst3  (uint16_t)fVOUTREFBurst3   //

#define ZeroCrossRange 20//10 xyz   //digits around ZC switches off
#define VACPolRange 50  //digits around ZC to detect Vac polarity

#define fV1_5GOODLEVEL  (float)(1.4/3.3*4096)    //1.4V 
#define V1_5GOODLEVEL (uint16_t)(fV1_5GOODLEVEL)
#define fV1_5MINLEVEL (float)(1.2/3.3*4096)     //1.2V 
#define V1_5MINLEVEL (uint16_t)(fV1_5MINLEVEL)
#define fV1_5MAXLEVEL (float)(1.65/3.3*4096)    //1.65V 
#define V1_5MAXLEVEL (uint16_t)(fV1_5MAXLEVEL)

//not used
//#define fVAUXGOODLEVEL  (float)(12/3.7/3.3*4096)    //12V with resistor devider by 27K and 10K
//#define VAUXGOODLEVEL (uint16_t)(fVAUXGOODLEVEL)
//#define fVAUXMINLEVEL (float)(10/3.7/3.3*4096)      //10V with resistor devider by 27K and 10K
//#define VAUXMINLEVEL (uint16_t)(fVAUXMINLEVEL)
//#define fVAUXMAXLEVEL (float)(12.1/3.7/3.3*4096)    //15V with resistor devider by 27K and 10K
//#define VAUXMAXLEVEL (uint16_t)(fVAUXMAXLEVEL)

#ifdef _50Hz
#define minfreq (1467>>1)    //66kHz tick for 45Hz, half period
#define maxfreq (1200>>1)    //66kHz tick for 55Hz, half period
#else
#define minfreq (1200>>1)    //66kHz tick for 55Hz
#define maxfreq (1015>>1)    //66kHz tick for 65Hz
#endif

#define CHNTC 17
#define CHVDC 10
#define CH1_5 15

#endif	/* XC_HEADER_TEMPLATE_H */

