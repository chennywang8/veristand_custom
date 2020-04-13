//==============================================================================
//
// Title:		model.h
// Purpose:		A short description of the interface.
//
// Created on:	4/12/2020 at 2:33:20 PM by Chenny Wang.
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __model_H__
#define __model_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

//==============================================================================
// Constants
#ifndef MODEL_h
# define MODEL_h 
typedef struct {
	double a[2][2];
	double b11;
	double c12;
	double idleRPM;
	double redlineRPM;
	double temperature_timeConstant;
	double temperature_roomTemp;
	double temperature_operatingTempDelta;
	double temperature_redlineTempDelta;
} Parameters;
#endif

//==============================================================================
// Types

//==============================================================================
// External variables

//==============================================================================
// Global functions

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __model_H__ */
