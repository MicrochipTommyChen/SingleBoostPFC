/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system intialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.95-b-SNAPSHOT
        Device            :  dsPIC33CK256MP506
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36
        MPLAB 	          :  MPLAB X v5.10
*/

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

/*
  Section: Included Files
*/
#include "mcc_generated_files/mcc.h"
//#include "mcc_generated_files/system.h"
#include "_h/globals.h"
#include "_h/functions_TPBL.h"
#include "_h/Compensation.h"

/*
												 Main application
 */
int main(void)
{
	uint16_t SecCounter = 0, TaskCounter = 0, UARTsendTaskCounter = 0, RelayWaitCounter = 0;
	uint16_t FastBlinkCounter = 0, SlowBlinkCounter = 0;
	
  initCMC2p2zCompensator();
  initVMC2p2zCompensator();

	// initialize the device
	SYSTEM_Initialize();
  
	InitFlags();
	UARTTxInit();

  systemFlagStartup._flags._flag._StartupZC = 1;	//start FTX_Startup
	
	while (1)
	{
   //-----------------Task scheduler: 10 fixed tasks, each 100us---------------    
    TaskCounter++;
    switch (TaskCounter)
    {
      //xx handler
      case 1:
        UART_load_for_send();
      break;
      
        //xx handler
      case 2:
        UARTsendTaskCounter++;
        if (UARTsendTaskCounter > 2)
        {
          UARTsend();
          UARTsendTaskCounter = 0;
        } 
      break;
      
      //xx 500ms Second handler
      case 3:
        SecCounter++;
        if (SecCounter > 500)
        {
            DEBUG_PIN_01_Toggle();
            SecCounter = 0;
        }        
      break;
      
      //xx handler
      case 4:
        if(!applicationFaults.catastrophicFault)
        {  
          Startup();
        }  
      break;
      
      //xx handler
      case 5:
        if (applicationFaults.catastrophicFault)  //HMI
        {
          FastBlinkCounter++;
          if (FastBlinkCounter > 50)
          {
//            __builtin_btg(&LATD, 6); //LED4
//            __builtin_btg(&LATD, 5); //LED3
            FastBlinkCounter = 0;
          }
        }
      break;
      
      //xx handler
      case 6: 
        if(systemFlag._flags._flag._RelayWait)
        {
          if(RelayWaitCounter++ > 3)
          {
//            RelayOn_SetHigh();
              RELAY_DRV_SetHigh();
            systemFlag._flags._flag._Relay = 1;
            systemFlag._flags._flag._RelayWait = 0;
            RelayWaitCounter = 0;
          }  
        }  
      break;
      
      //xx handler
      case 7:
        if ((applicationFaults.wordWriteFaults & 0x000F) > 0)  //HMI
        {
          SlowBlinkCounter++;
          if (SlowBlinkCounter > 250)
          {
//            __builtin_btg(&LATD, 6); //LED4
            SlowBlinkCounter = 0;
          }
        }
        else if (applicationFaults.wordWriteFaults == 0)
        {
//          LED4_SetLow();
        }  
        if(applicationFaults.vacDrop)
        {
//          LED3_SetHigh();
        }  
        else if (applicationFaults.wordWriteFaults == 0)
        {  
//          LED3_SetLow();
        }
      break;
      
      //xx handler
      case 8:
      break;
      
      //xx handler
      case 9:
      break;
      
      //xx handler
      case 10:
        TaskCounter = 0;
      break;
      
      //Default
      default:
        Nop();
      break;
    }
    //---------------------------Task scheduler---------------------------------    
    // Wait for next task window 
    while (!IFS0bits.CCT1IF);
    IFS0bits.CCT1IF = 0;
  }
	
	return 1;
}
/**
 End of File
*/

