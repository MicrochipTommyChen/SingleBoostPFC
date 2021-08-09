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
#include "../_h/functions_TPBL.h"
#include "../mcc_generated_files/uart1.h"
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void UARTsend(void)
{
  uint8_t i_;

  if ((UART_RxTx.UartSendCounter >= 0) && (UART_RxTx.UartSendCounter < UART_RxTx.UartSendLength + 5u))
  { 
    //50us @ 16MHz
    UART_RxTx.SendCRC ^= (uint16_t)UART_RxTx.TXBytes[UART_RxTx.UartSendCounter];
    for (i_ = 0; i_ < 8; ++i_)
    {
      if ((UART_RxTx.SendCRC & 1) == 1)
        UART_RxTx.SendCRC = (UART_RxTx.SendCRC >> 1) ^ 0xA001;
      else
        UART_RxTx.SendCRC = (UART_RxTx.SendCRC >> 1);
    }
  }
  if((UART_RxTx.UartSendCounter == UART_RxTx.UartSendLength + 5u))
  {  
    UART_RxTx.TXBytes[UART_RxTx.UartSendLength + 5u] = (uint8_t)(UART_RxTx.SendCRC >> 8);
    UART_RxTx.TXBytes[UART_RxTx.UartSendLength + 6u] = (uint8_t)(UART_RxTx.SendCRC & 0x00FF);
  }

  UART1_Write(UART_RxTx.TXBytes[UART_RxTx.UartSendCounter]);

  UART_RxTx.UartSendCounter++;
  
  if(UART_RxTx.UartSendCounter > (UART_RxTx.UartSendLength + 7u))
  {
    UART_RxTx.UartSendCounter = 0;
    UART_RxTx.SendCRC = 0;
    UART_RxTx.TXSendDone = 1;
  }

}     
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void UART_load_for_send(void)
{
 if (UART_RxTx.TXSendDone)
  {
    UART_RxTx.TXBytes[5] =  (uint8_t) (ADCValues._IACConv >> 8);
    UART_RxTx.TXBytes[6] =  (uint8_t) (ADCValues._IACConv & 0x00FF);
    UART_RxTx.TXBytes[7] =  (uint8_t) (ADCValues._IACAvg >> 8);
    UART_RxTx.TXBytes[8] =  (uint8_t) (ADCValues._IACAvg & 0x00FF);

    UART_RxTx.TXBytes[9] =  (uint8_t) (ADCValues._VACConv >> 8);
    UART_RxTx.TXBytes[10] = (uint8_t) (ADCValues._VACConv & 0x00FF);
    UART_RxTx.TXBytes[11] = (uint8_t) (ADCValues._VACAvg >> 8);
    UART_RxTx.TXBytes[12] = (uint8_t) (ADCValues._VACAvg & 0x00FF);
		
    UART_RxTx.TXBytes[13] = (uint8_t) (ADCValues._VACAvgCounter_zw >> 8);
    UART_RxTx.TXBytes[14] = (uint8_t) (ADCValues._VACAvgCounter_zw & 0x00FF);

    UART_RxTx.TXBytes[15] = (uint8_t) (applicationFaults.wordWriteFaults >> 8);
    UART_RxTx.TXBytes[16] = (uint8_t) (applicationFaults.wordWriteFaults & 0x00FF);
		
    UART_RxTx.TXBytes[17] = (uint8_t) (ADCValues._VNTCraw >> 8);
    UART_RxTx.TXBytes[18] = (uint8_t) (ADCValues._VNTCraw & 0x00FF);

    UART_RxTx.TXBytes[19] = (uint8_t) (ADCValues._VOUTAvg >> 8);
    UART_RxTx.TXBytes[20] = (uint8_t) (ADCValues._VOUTAvg & 0x00FF);

    UART_RxTx.TXBytes[21] = (uint8_t) (systemFlagStartup._flags._StartupFlagsWord >> 8);
    UART_RxTx.TXBytes[22] = (uint8_t) (systemFlagStartup._flags._StartupFlagsWord & 0x00FF);

    UART_RxTx.TXBytes[23] = (uint8_t) (systemFlag._flags._FlagsWord >> 8);
    UART_RxTx.TXBytes[24] = (uint8_t) (systemFlag._flags._FlagsWord & 0x00FF);
        
    UART_RxTx.TXBytes[25] = (uint8_t) (ZCFlags.wordZCFlags >> 8);
    UART_RxTx.TXBytes[26] = (uint8_t) (ZCFlags.wordZCFlags & 0x00FF);

    UART_RxTx.TXBytes[27] = (uint8_t) (ADCValues._V1_5Avg >> 8);
    UART_RxTx.TXBytes[28] = (uint8_t) (ADCValues._V1_5Avg & 0x00FF);
    


    UART_RxTx.TXSendDone = 0;
  }
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
