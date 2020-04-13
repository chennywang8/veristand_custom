//==============================================================================
//
// Title:		model.c
// Purpose:		A short description of the implementation.
//
// Created on:	4/12/2020 at 2:33:20 PM by Chenny Wang.
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include "ni_modelframework.h"
#include "model.h"
#include <stddef.h>
#include <math.h>


//==============================================================================
// Constants
/* User defined datatypes and constants */
#define rtDBL	0
#define rtINT	1

/* Pull in the parameters structure from codegen.c */
extern Parameters rtParameter[2];
extern int32_t READSIDE;

/* !!!! IMPORTANT !!!!
   Accessing parameters values must be done through rtParameter[READSIDE]
   The macro readParam is defined for you as a simple way to access parameters
   !!!! IMPORTANT !!!! */
#define readParam rtParameter[READSIDE]

//==============================================================================
// Types
/* Define IO and Signals structs */
typedef struct {
	double command_RPM;
	int32_t command_EngineOn;
} Inports;

typedef struct {
  	double RPM;
	double engineTemperature;
} Outports;

typedef struct {
	double RPM;
	double engineTemperature;
	int32_t engineOn;
	double state[2];
} Signals;

/* Instantiate IO and Signals */
Inports rtInport;
Outports rtOutport;
Signals rtSignal;

#define MAXIMUM( x, y) ((x)>(y)?(x):(y))

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions
/* INPUT: ptr, base address of where value should be set.
   INPUT: subindex, offset into ptr where value should be set.
   INPUT: value, the value to be set
   INPUT: type, the user defined type of the parameter being set, as defined in
  		  rtParamAttributes
   RETURN: status, NI_ERROR on error, NI_OK otherwise */
int32_t USER_SetValueByDataType(void* ptr, int32_t subindex, double value, int32_t type)
{
	switch (type) {
		case rtDBL: 
    		((double *)ptr)[subindex] = (double)value;
    		return NI_OK;
		case rtINT:
			((int32_t *)ptr)[subindex] = (int32_t)value;
			return NI_OK;
	}
  	return NI_ERROR;
}

/* INPUT: ptr, base address of value to be retrieved.
   INPUT: subindex, offset into ptr where value should be retrieved.
   INPUT: type, the user defined type of the parameter or signal being 
  		  retrieved, as defined in rtParamAttributes or rtSignalAttributes
   RETURN: value of user-defined type cast to a double */
double USER_GetValueByDataType(void* ptr, int32_t subindex, int32_t type)
{
	switch (type) {
	case rtDBL:
		return (double)(((double *)ptr)[subindex]);
	case rtINT:
		return (int32_t)(((int32_t *)ptr)[subindex]);
  	}
  	{	/* return NaN, ok for vxworks and pharlap */
	  	uint32_t nan[2] = {0xFFFFFFFF, 0xFFFFFFFF};
		return *(double*)nan;
	}
}

/*
// When a model has parameters of the form: "modelname/parameter", these model parameters are considered global parameters (target scoped) in NI VeriStand
// When a model has parameters of the form: "modelname/block/paramter" these model parameters are NOT considered global parameters (model scoped) in NI VeriStand
typedef struct {
  int32_t idx;			// not used
  char* paramname;	// name of the parameter, e.g., "Amplitude"
  uintptr_t addr;// offset of the parameter in the Parameters struct
  int32_t datatype;		// integer describing a user defined datatype. must have a corresponding entry in GetValueByDataType and SetValueByDataType
  int32_t width;		// size of parameter
  int32_t numofdims; 	// number of dimensions
  int32_t dimListOffset;// offset into dimensions array
  int32_t IsComplex;	// not used
} NI_Parameter;

*/
/* Define parameter attributes */
int32_t ParameterSize DataSection(".NIVS.paramlistsize") = 9;
NI_Parameter rtParamAttribs[] DataSection(".NIVS.paramlist") = {
  	{ 0, "engine/a", offsetof(Parameters, a), rtDBL, 4, 2, 0, 0},
  	{ 0, "engine/b11", offsetof(Parameters, b11), rtDBL, 1, 2, 2, 0},
  	{ 0, "engine/c12", offsetof(Parameters, c12), rtDBL, 1, 2, 4, 0},
  	{ 0, "engine/idle speed (RPM)", offsetof(Parameters, idleRPM), rtDBL, 1, 2, 6, 0},
  	{ 0, "engine/red line speed (RPM)", offsetof(Parameters, redlineRPM), rtDBL, 1, 2, 8, 0},
  	{ 0, "engine/temperature time constant (s)", offsetof(Parameters, temperature_timeConstant), rtDBL, 1, 2, 10, 0},
  	{ 0, "engine/environment temperature (C)", offsetof(Parameters, temperature_roomTemp), rtDBL, 1, 2, 12, 0},
  	{ 0, "engine/operating temperature delta (C)", offsetof(Parameters, temperature_operatingTempDelta), rtDBL, 1, 2, 14, 0},
  	{ 0, "engine/redline temperature delta (C)", offsetof(Parameters, temperature_redlineTempDelta), rtDBL, 1, 2, 16, 0},
};
int32_t ParamDimList[] DataSection(".NIVS.paramdimlist") =
{
  2, 2,
  1, 1,
  1, 1,
  1, 1,
  1, 1,
  1, 1,
  1, 1,
  1, 1,
  1, 1,
};

/* Initialize parameters */
Parameters initParams DataSection(".NIVS.defaultparams") = { { {-(7.0/9.0),  /* a11 */
																-(2.0/3.0)}, /* a12 */
																	{0.5,	 /* a21 */
																	0} },	 /* a22 */
																	2.0,	 /* b11 */
																(1.0/3.0),   /* c12 */
																	900.0,   /* idleRPM */
																	7000.0,  /* redlineRPM */
																	10.0,    /* temp time const */
																	25.0,    /* environ temp */
																	65.0,    /* operating delta */
																	100.0 }; /* redline delta */

/*
   This data structure is used to retrieve the size, width, and datatype of the default parameters.
      
   ParamSizeWidth Parameters_sizes[] DataSection(".NIVS.defaultparamsizes") = {
    { sizeof(initParams), 0, 0},  The first element in this array uses only the first field in the typedef.  It is used to specify the size of the default parameters structure.
    { sizeof(double), 1, 0 }, Subsequent elements in the array use all 3 fields, they are: the size (num of bytes per element), the width (num of elements) (2x2 array would have 4 elements), and the datatype of each parameter (which is handled by Get/SetValueByType)
   };  
*/
ParamSizeWidth Parameters_sizes[] DataSection(".NIVS.defaultparamsizes") = {
  { sizeof(initParams)},
  { sizeof(double), 4, rtDBL },
  { sizeof(double), 1, rtDBL },
  { sizeof(double), 1, rtDBL },
  { sizeof(double), 1, rtDBL },
  { sizeof(double), 1, rtDBL },
  { sizeof(double), 1, rtDBL },
  { sizeof(double), 1, rtDBL },
  { sizeof(double), 1, rtDBL },
  { sizeof(double), 1, rtDBL },
};

/*
typedef struct {
  int32_t    idx;		// not used
  char*  blockname; // name of the block where the signals originates, e.g., "sinewave/sine"
  int32_t    portno;	// the port number of the block
  char* signalname; // name of the signal, e.g., "Sinewave + In1"
  uintptr_t addr;// address of the storage for the signal
  uintptr_t baseaddr;		// not used
  int32_t	 datatype;	// integer describing a user defined datatype. must have a corresponding entry in GetValueByDataType
  int32_t width;		// size of signal
  int32_t numofdims; 	// number of dimensions
  int32_t dimListOffset;// offset into dimensions array
  int32_t IsComplex;	// not used
} NI_Signal;
*/

/* Define signal attributes */
int32_t SignalSize DataSection(".NIVS.siglistsize") = 3;
/* must be careful to not get a pointer into .rela.NIVS.siglist */
/* the addr field for these signals is populated in USER_Initialize */
NI_Signal rtSignalAttribs[] DataSection(".NIVS.siglist") = {
  	{ 0, "engine/rpm", 0, "RPM", 0, 0, rtDBL, 1, 2, 0, 0},
	{ 0, "engine/temperature", 0, "Temperature", 0, 0, rtDBL, 1, 2, 2, 0},
	{ 0, "engine/engineOn", 0, "EngineOn", 0, 0, rtINT, 1, 2, 4, 0},
};
int32_t SigDimList[] DataSection(".NIVS.sigdimlist") =
{
	1, 1,
	1, 1,
	1, 1,
};

/*
typedef struct {
  int32_t	idx;	// not used
  char*	name;	// name of the external IO, e.g., "In1"
  int32_t	TID;	// = 0
  int32_t   type; 	// Ext Input: 0, Ext Output: 1
  int32_t  width; 	// not used
  int32_t	dimX;	// 1st dimension size
  int32_t	dimY; 	// 2nd dimension size
} NI_ExternalIO;
*/

/* Define I/O attributes */
int32_t ExtIOSize DataSection(".NIVS.extlistsize") = 4;
int32_t InportSize = 2;
int32_t OutportSize = 2;
NI_ExternalIO rtIOAttribs[] DataSection(".NIVS.extlist") = {
	{ 0, "command_RPM", 0, 0, 1, 1, 1},
	{ 0, "command_EngineOn", 0, 0, 1, 1, 1},
	{ 0, "RPM", 0, 1, 1, 1, 1},
	{ 0, "EngineTemp", 0, 1, 1, 1, 1},
	{ -1 },
};

/* Model name and build information */
const char * USER_ModelName DataSection(".NIVS.compiledmodelname") = "engine";
const char * USER_Builder DataSection(".NIVS.builder") = "Custom Engine Model";

/* baserate is the rate at which the model runs */
double USER_BaseRate = .01;

/*
typedef struct {
  int32_t    tid;		// = 0
  double tstep;		
  double offset;
  int32_t priority;
} NI_Task;
*/
NI_Task rtTaskAttribs DataSection(".NIVS.tasklist") = { 0 /* must be 0 */, .01 /* must be equal to baserate */, 0, 0 };

/* RETURN: status, NI_ERROR on error, NI_OK otherwise */
int32_t USER_Initialize() {
		/* Initialize signal addresses */
		rtSignalAttribs[0].addr = (uintptr_t)&rtSignal.RPM;
		rtSignalAttribs[1].addr = (uintptr_t)&rtSignal.engineTemperature;
		rtSignalAttribs[2].addr = (uintptr_t)&rtSignal.engineOn;

		rtSignal.state[0] = 0.0;
		rtSignal.state[1] = 0.0;
		rtSignal.RPM = 0.0;
		rtSignal.engineTemperature = readParam.temperature_roomTemp; 
		rtSignal.engineOn = 0;
	return NI_OK;
}

/* evaluates a transfer function with numerator [1] and denominator [1 2 3]*/
static double engine_RPM_function(double input);
static double engine_RPM_function(double input)
{
	double *x = &(rtSignal.state[0]);
	double out, a11, a12, a21, b11, c12;

	a11 = readParam.a[0][0];
	a12 = readParam.a[0][1];
	a21 = readParam.a[1][0];
	b11 = readParam.b11;
	c12 = readParam.c12;

	/* this is an Euler ODE solver at dt = 0.01 */
	x[0] += 0.01 * (a11 * x[0] + a12 * x[1] + b11 * input); 
	x[1] += 0.01 * a21 * x[0];

	out = c12 * x[1];

	if (!rtInport.command_EngineOn && out <= 0.0)
	{
		/* if engine is off and the RPM gets to zero (or less), 
		   then zero out the states so the engine will "stop"; 
		   otherwise, let the RPM gradually reach zero. */
		x[0] = 0.0;
		x[1] = 0.0;
	}
	
	/* Update the engineOn signal */
	rtSignal.engineOn = (int32_t)rtInport.command_EngineOn;

	/* never return an rpm value less than zero */
	rtSignal.RPM = MAXIMUM(out, 0.0);
	return rtSignal.RPM;
}

/* evaluates a first order model num = [175] den = [1 100] */
static double engine_temperature_function(double setPoint);
static double engine_temperature_function(double setPoint)
{
	double t1;

	if (readParam.temperature_timeConstant > 0)
		t1 = 1.0/readParam.temperature_timeConstant;
	else
		t1 = 0.0;
	
	rtSignal.engineTemperature += 0.01*(-t1*rtSignal.engineTemperature + setPoint); /* Euler ODE solver at dt = 0.01 */

	return t1 * rtSignal.engineTemperature; 
}

/* INPUT: *inData, pointer to inport data at the current timestamp, to be 
  	      consumed by the function
   OUTPUT: *outData, pointer to outport data at current time + baserate, to be
  	       produced by the function
   INPUT: timestamp, current simulation time */
int32_t USER_TakeOneStep(double *inData, double *outData, double timestamp) 
{
	double rpm_command, idleRPM = readParam.idleRPM, redlineRPM = readParam.redlineRPM, temperature_command;
	if (inData)
	{
		rtInport.command_RPM = inData[0];
		rtInport.command_EngineOn = (int32_t)inData[1];
	}
	else
	{
		rtInport.command_RPM = 0.0;
		rtInport.command_EngineOn = 0;
	}

	
	temperature_command = readParam.temperature_roomTemp;

	if (rtInport.command_EngineOn)
	{
		/* this simulates an "idle", i.e. a minimum rpm command when the engine is running */
		rpm_command = (rtInport.command_RPM > idleRPM ? rtInport.command_RPM : idleRPM); 

		/* determine if the temperature should move toward normal operating temp or redline temp */
		if (rtOutport.RPM < redlineRPM)
			temperature_command += readParam.temperature_operatingTempDelta;
		else
			temperature_command += readParam.temperature_redlineTempDelta;
	}
	else
	{
		/* when the engine is off, send the rpm_command to zero, letting the engine TF continue */
		rpm_command = 0.0; 
	}

	rtOutport.RPM = engine_RPM_function(rpm_command); /* don't let the RPM be less than zero */

	
	if (rtInport.command_EngineOn)
	{
	}

	rtOutport.engineTemperature = engine_temperature_function(temperature_command);
	
	if (outData)
	{
		outData[0] = rtOutport.RPM;	
		outData[1] = rtOutport.engineTemperature ;	
	}
	
	return NI_OK;
}

/* RETURN: status, NI_ERROR on error, NI_OK otherwise */
int32_t USER_Finalize() {
	return NI_OK;
}

/* Non-supported API */

extern struct { 
	int32_t stopExecutionFlag;
	const char *errmsg;
	void* flip;
	uint32_t inCriticalSection;
	int32_t SetParamTxStatus;
	double timestamp;
} NIRT_system;

DLL_EXPORT int32_t NIRT_GetSimState(int32_t* numContStates, char* contStatesNames, double* contStates, int32_t* numDiscStates, char* discStatesNames, double* discStates, int32_t* numClockTicks, char* clockTicksNames, int32_t* clockTicks) 
{
	if (numContStates && numDiscStates && numClockTicks) {
		if (*numContStates < 0 || *numDiscStates < 0 || *numClockTicks < 0) {
			*numContStates = 0;
			*numDiscStates = 0;
			*numClockTicks = 1;
			return NI_OK;
		}
	}
	
	if (clockTicks && clockTicksNames) {
		clockTicks[0] = NIRT_system.timestamp;
		strcpy(clockTicksNames, "clockTick0");
	}	
	return NI_OK;
}

DLL_EXPORT int32_t NIRT_SetSimState(double* contStates, double* discStates, int32_t* clockTicks)
{
	if (clockTicks) {
		NIRT_system.timestamp = clockTicks[0];
	}	
	return NI_OK;
}

int32_t USER_ModelStart() {
	return NI_OK;
}
