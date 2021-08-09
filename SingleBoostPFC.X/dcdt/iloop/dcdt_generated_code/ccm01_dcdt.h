/**
  Summary:
  This header file was created using Microchip Digital Compensator Design Tool (DCDT)
  and includes system summary and digital compensator coefficients. This file
  should be included in the MPLAB X project directory.

  File Name:     ccm01_dcdt.h
  Project Name:  iloop
  Date:          3/27/2020
  Time:          11:50.54

  Software License Agreement

  Copyright © 2020 Microchip Technology Inc.  All rights reserved.
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
#ifndef _CCM01_DCDT_DEFINES_H
#define _CCM01_DCDT_DEFINES_H

/**
  Compensator Type:  2P2Z
      Entry                Value  
    ---------            ---------
  Pole 0                 5.0000e+03 Hz
  Pole 2                 3.3000e+04 Hz
  Zero 1                 4.0000e+02 Hz
  Gain(Kdc)              1.000
  Warp                   false
  PWM Frequency          6.6000e+04
  PWM Sampling Ratio     1
  Sampling Frequency     6.6000e+04
  PWM Max Resolution     5.0000e-10
  Computational Delay    1.0000e-06
  Gate Drive Delay       1.5000e-07
  Control Output Min.    0
  Control Output Max.    29000
  Kuc Gain               2.4420e+01
  Use Kuc Gain           false


  PWM Calculations
      Name                Value  
    ---------           ---------
  Bits of Resolution    14.887
  Gain                  3.300e-05


  s-domain transfer function

               Wp0   Wp2(Wz1 + s)
  H(s) = Kdc X --- X ------------
                s    Wz1(Wp2 + s)

                  3.14e+04   2.07e+05(2.51e+03 + s)
  H(s) = 1.000 X -------- X ----------------------
                     s       2.51e+03(2.07e+05 + s)



  Digital Compensator Coefficients

  Name    Value     Normalized    Q15      Hex
  ----    -----     ----------    ---      ---
  a1      0.778     0.100         3275     0x0CCB
  a2      0.222     0.029         934      0x03A6
  b0      7.783     1.000         32764    0x7FFC
  b1      0.291     0.037         1224     0x04C8
  b2      -7.492    -0.963        -31540   0x84CC


  z-domain transfer function

         u(z)  B0 + B1z^(-1) + B2z^(-2)
  H(z) = --- = ------------------------
         e(z)  A0 - A1z^(-1) - A2z^(-2)

          (7.783) + (0.291)z^(-1) + (-7.492)z^(-2)
  H(z) = ---------------------------------------------
          1 - (0.778)z^(-1) - (0.222)z^(-2)

**/


// Compensator Coefficient Defines
#define CCM01_COMP_2P2Z_COEFF_A1      0x0CCB
#define CCM01_COMP_2P2Z_COEFF_A2      0x03A6
#define CCM01_COMP_2P2Z_COEFF_B0      0x7FFC
#define CCM01_COMP_2P2Z_COEFF_B1      0x04C8
#define CCM01_COMP_2P2Z_COEFF_B2      0x84CC
#define CCM01_COMP_2P2Z_POSTSCALER    0x7C8A
#define CCM01_COMP_2P2Z_POSTSHIFT     0xFFFD
#define CCM01_COMP_2P2Z_PRESHIFT      0x0000


// Compensator Clamp Limits
#define CCM01_COMP_2P2Z_MIN_CLAMP    0x0000
#define CCM01_COMP_2P2Z_MAX_CLAMP    0x7148


#endif
