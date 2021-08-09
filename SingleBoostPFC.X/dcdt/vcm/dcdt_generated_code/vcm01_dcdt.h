/**
  Summary:
  This header file was created using Microchip Digital Compensator Design Tool (DCDT)
  and includes system summary and digital compensator coefficients. This file
  should be included in the MPLAB X project directory.

  File Name:     vcm01_dcdt.h
  Project Name:  vcm
  Date:          10/29/2019
  Time:          10:02.39

  Software License Agreement

  Copyright © 2019 Microchip Technology Inc.  All rights reserved.
  Microchip licenses to you the right to use, modify, copy and distribute
  Software only when embedded on a Microchip microcontroller or digital
  signal controller, which is integrated into your product or third party
  product (pursuant to the sublicense terms in the accompanying license
  agreement).

  You should refer to the license agreement accompanying this Software
  for additional information regarding your rights and obligations.

  SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
  KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
  OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
  PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
  OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
  BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
  DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
  INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
  COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
  CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
  OR OTHER SIMILAR COSTS.
 **/
#ifndef _VCM01_DCDT_DEFINES_H
#define _VCM01_DCDT_DEFINES_H

/**
  Compensator Type:  2P2Z
      Entry                Value  
    ---------            ---------
  Pole 0                 2.0000e+00 Hz
  Pole 2                 5.0000e+02 Hz
  Zero 1                 9.0000e+00 Hz
  Gain(Kdc)              3.000
  Warp                   false
  PWM Frequency          6.6000e+04
  PWM Sampling Ratio     11
  Sampling Frequency     6.0000e+03
  PWM Max Resolution     5.0000e-10
  Computational Delay    1.3500e-06
  Gate Drive Delay       1.5000e-07
  Control Output Min.    0
  Control Output Max.    30000
  Kuc Gain               2.4420e+01
  Use Kuc Gain           false


  PWM Calculations
      Name                Value  
    ---------           ---------
  Bits of Resolution    18.347
  Gain                  3.000e-06


  s-domain transfer function

               Wp0   Wp2(Wz1 + s)
  H(s) = Kdc X --- X ------------
                s    Wz1(Wp2 + s)

                  1.26e+01   3.14e+03(5.65e+01 + s)
  H(s) = 3.000 X -------- X ----------------------
                     s       5.65e+01(3.14e+03 + s)



  Digital Compensator Coefficients

  Name    Value     Normalized    Q15      Hex
  ----    -----     ----------    ---      ---
  a1      1.585     1.000         32764    0x7FFC
  a2      -0.585    -0.369        -12093   0xD0C3
  b0      0.139     0.088         2872     0x0B38
  b1      0.001     0.001         26       0x001A
  b2      -0.138    -0.087        -2845    0xF4E3


  z-domain transfer function

         u(z)  B0 + B1z^(-1) + B2z^(-2)
  H(z) = --- = ------------------------
         e(z)  A0 - A1z^(-1) - A2z^(-2)

          (0.139) + (0.001)z^(-1) + (-0.138)z^(-2)
  H(z) = ---------------------------------------------
          1 - (1.585)z^(-1) - (-0.585)z^(-2)

**/
//------------------------------------------------------------------------------
//P0=2Hz, P1=500Hz, Z1=9Hz
//------------------------------------------------------------------------------
// FTX lib with dual bit shift 
#define VCM01_COMP_2P2Z_COEFF_A1      0x6570
#define VCM01_COMP_2P2Z_COEFF_A2      0xDA8F
#define VCM01_COMP_2P2Z_COEFF_B0      0x472A
#define VCM01_COMP_2P2Z_COEFF_B1      0x00AB
#define VCM01_COMP_2P2Z_COEFF_B2      0xB982

#define VCM01_COMP_2P2Z_POSTSCALER    0

#define VCM01_COMP_2P2Z_POSTSHIFT     0x0002    //+2
#define VCM01_COMP_2P2Z_PRESHIFT      0xFFFF    //-1

// Compensator Clamp Limits
#define VCM01_COMP_2P2Z_MIN_CLAMP    0x0000
#define VCM01_COMP_2P2Z_MAX_CLAMP    0x7530


#endif

