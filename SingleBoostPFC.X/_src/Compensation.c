/**
    (c) 2017 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

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

#include "../_h/globals.h"
#include "../dcdt/vcm/dcdt_generated_code/vcm01_dcdt.h"
#include "../dcdt/iloop/dcdt_generated_code/ccm01_dcdt.h"
//------------------------------------------------------------------------------
//SMPS_2P2Z_T CMC2p2z; //typedef from smps_control.h to var
//------------------------------------------------------------------------------
int16_t CMC2p2zErrorHistory[3] __attribute__((space(ymemory), far));
int16_t CMC2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t CMC2p2zControlHistory[2] __attribute__((space(ymemory), far));
int16_t CMC2p2zBCoefficients[3] __attribute__ ((space(xmemory)));
uint16_t CMC2p2zReferenceSet;
//------------------------------------------------------------------------------
//SMPS_2P2Z_T VMC2p2z; //typedef from smps_control.h to var
//------------------------------------------------------------------------------
int16_t VMC2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t VMC2p2zBCoefficients[3]__attribute__((space(xmemory)));
int16_t VMC2p2zErrorHistory[3] __attribute__((space(ymemory), far));
int16_t VMC2p2zControlHistory[2] __attribute__((space(ymemory), far));
uint16_t VMC2p2zReferenceSet;
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------

void initCMC2p2zCompensator(void)
{
    CMC2p2z.aCoefficients = &CMC2p2zACoefficients[0]; // Set up pointer to derived coefficients
    CMC2p2z.bCoefficients = &CMC2p2zBCoefficients[0]; // Set up pointer to derived coefficients13p3zBCoefficients[0]; // Set up pointer to derived coefficients
    CMC2p2z.controlHistory = &CMC2p2zControlHistory[0]; // Set up pointer to controller history
    CMC2p2z.errorHistory = &CMC2p2zErrorHistory[0]; // Set up pointer to error history

    CMC2p2z.preShift = CCM01_COMP_2P2Z_PRESHIFT; // Normalization shift for error amplifier results
    CMC2p2z.postScaler = CCM01_COMP_2P2Z_POSTSCALER;
    CMC2p2z.postShift = CCM01_COMP_2P2Z_POSTSHIFT; // Normalization shift for control loop results to peripheral
    CMC2p2z.minOutput = CCM01_COMP_2P2Z_MIN_CLAMP; //;PG1DCmin;
    CMC2p2z.maxOutput = CCM01_COMP_2P2Z_MAX_CLAMP; //PG1DCmax; 0;//FTX FTX
    CMC2p2zReferenceSet = 0;

    SMPS_Controller2P2ZInitialize(&CMC2p2z); // Clear histories

    //Set Boost coefficients
    CMC2p2zACoefficients[0] = CCM01_COMP_2P2Z_COEFF_A1;
    CMC2p2zACoefficients[1] = CCM01_COMP_2P2Z_COEFF_A2;

    CMC2p2zBCoefficients[0] = CCM01_COMP_2P2Z_COEFF_B0;
    CMC2p2zBCoefficients[1] = CCM01_COMP_2P2Z_COEFF_B1;
    CMC2p2zBCoefficients[2] = CCM01_COMP_2P2Z_COEFF_B2;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------

void initVMC2p2zCompensator(void)
{
  VMC2p2z.aCoefficients = &VMC2p2zACoefficients[0]; // Set up pointer to derived coefficients
	VMC2p2z.bCoefficients = &VMC2p2zBCoefficients[0]; // Set up pointer to derived coefficients13p3zBCoefficients[0]; // Set up pointer to derived coefficients
	VMC2p2z.controlHistory = &VMC2p2zControlHistory[0]; // Set up pointer to controller history
	VMC2p2z.errorHistory = &VMC2p2zErrorHistory[0]; // Set up pointer to error history

	VMC2p2z.preShift = VCM01_COMP_2P2Z_PRESHIFT; // Normalization shift for error amplifier results
	VMC2p2z.postScaler = VCM01_COMP_2P2Z_POSTSCALER;
	VMC2p2z.postShift = VCM01_COMP_2P2Z_POSTSHIFT; // Normalization shift for control loop results to peripheral
	VMC2p2z.minOutput = VCM01_COMP_2P2Z_MIN_CLAMP; //;PG1DCmin;
	VMC2p2z.maxOutput = VCM01_COMP_2P2Z_MAX_CLAMP; //PG1DCmax; VCM01_COMP_2P2Z_MIN_CLAMP;//
	VMC2p2zReferenceSet = 0;

	SMPS_Controller2P2ZInitialize(&VMC2p2z); // Clear histories

	//Set Boost coefficients
	VMC2p2zACoefficients[0] = VCM01_COMP_2P2Z_COEFF_A1;
	VMC2p2zACoefficients[1] = VCM01_COMP_2P2Z_COEFF_A2;

	VMC2p2zBCoefficients[0] = VCM01_COMP_2P2Z_COEFF_B0;
	VMC2p2zBCoefficients[1] = VCM01_COMP_2P2Z_COEFF_B1;
	VMC2p2zBCoefficients[2] = VCM01_COMP_2P2Z_COEFF_B2;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
