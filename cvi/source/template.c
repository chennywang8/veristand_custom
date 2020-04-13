/*========================================================================*
 * NI VeriStand Model Framework
 * Model template
 *
 * Abstract:
 * 	Template for implementing a custom model compatible with the NI VeriStand Model Framework.
 * 	There are interdependent structures between the user code (template.c) and the core interface (ni_modelframework.c)
 *
 *========================================================================*/

/* Include headers */
#include "ni_modelframework.h"

 /*========================================================================*
	 model.h should be generated as part of the codegen process
	 the core component ni_modelframework.c depends on it
	 It should define the following typedef:

	 typedef struct 
	 {
	  <Your parameters here>  	
	 } Parameters;
	 
	 NOTE: All Parameters which are to be exposed to the user should be
	 contained in the Parameters typedef specified in model.h
	 
	 ni_modelframework.c will manage all parameter access, but this typedef must be present
	 in model.h
*/
#include "model.h"

 /*========================================================================*
	Pull in the parameters structure from ni_modelframework.c 
*/
extern Parameters rtParameter[2];
extern int32_t READSIDE;

 /*========================================================================*
	Accessing parameters values must be done through rtParameter[READSIDE]
	The macro readParam is defined as a simple way to access parameter values
*/
#define readParam rtParameter[READSIDE]

 /*========================================================================*
  Define parameter attributes
  + When a model has parameters of the form: "modelname/parameter"
		these model parameters are considered global parameters (target scoped) in NI VeriStand
  + When a model has parameters of the form: "modelname/block/paramter" 
		these model parameters are NOT considered global parameters (model scoped) in NI VeriStand
		
	NI_Parameter defined in ni_modelframework.h	
	
	Example:
	NI_Parameter rtParamAttribs[] DataSection(".NIVS.paramlist") =
	{
	  { 0, "array_indexing/row_idx/Value", offsetof(Parameters,
		row_idx_Value), 23, 1, 2, 0, 0 },

	  { 1, "array_indexing/col_idx/Value", offsetof(Parameters,
		col_idx_Value), 23, 1, 2, 2, 0 },

	  { 2, "array_indexing/MatrixType/Value", offsetof(Parameters,
		MatrixType_Value), 18, 6, 2, 4, 0 },
	};

	int32_t ParameterSize DataSection(".NIVS.paramlistsize") = 3;
	int32_t ParamDimList[] DataSection(".NIVS.paramdimlist") =
	{
	  1, 1,                                Parameter at index 0
	  1, 1,                                Parameter at index 1
	  3, 2,                                Parameter at index 2
	};
*/
int32_t ParameterSize DataSection(".NIVS.paramlistsize") = ;
NI_Parameter rtParamAttribs[] DataSection(".NIVS.paramlist") = ;
int32_t ParamDimList[] DataSection(".NIVS.paramdimlist") = ;

 /*========================================================================*
   Initialize parameters
 */
Parameters initParams DataSection(".NIVS.defaultparams") = ;

 /*========================================================================*
   This data structure is used to retrieve the size, width, and datatype of the default parameters.
      
   ParamSizeWidth Parameters_sizes[] DataSection(".NIVS.defaultparamsizes") = {{ sizeof(initParams), 0, 0}, { sizeof(double), 1, 0 }}
   
   + The first element in this array uses only the first field in the typedef.  It is used to specify the size of the default parameters structure
   + Subsequent elements in the array use all 3 fields, they are: 
		++ the size (num of bytes per element)
		++ the width (num of elements) (2x2 array would have 4 elements)
		++ datatype of each parameter
 */
ParamSizeWidth Parameters_sizes[] DataSection(".NIVS.defaultparamsizes") = ;

 /*========================================================================*
   Define signal attributes 
   NI_Signal defined in ni_modelframework.h
   
   Example:
	NI_Signal rtSignalAttribs[] DataSection(".NIVS.siglist") =
	{
	  { 0, "array_indexing/MatrixType", 0, "matrix(1, 1)", offsetof
		(BlockIO_array_indexing, matrix) + (0*sizeof(real_T)), 0 18, 1, 2,
		0, 0 },

	  { 1, "array_indexing/MatrixType", 0, "matrix(2, 1)", offsetof
		(BlockIO_array_indexing, matrix) + (1*sizeof(real_T)), 0, 18, 1, 2,
		2, 0 }
	};

	int32_t SignalSize DataSection(".NIVS.siglistsize") = 2;
	int32_t SigDimList[] DataSection(".NIVS.sigdimlist") = {1, 1};
 */
int32_t SignalSize DataSection(".NIVS.siglistsize") = ;
NI_Signal rtSignalAttribs[] DataSection(".NIVS.siglist") = ;
int32_t SigDimList[] DataSection(".NIVS.sigdimlist") = ;

 /*========================================================================*
   Define IO attributes 
   NI_ExternalIO defined in ni_modelframework.h
   
	int32_t ExtIOSize DataSection(".NIVS.extlistsize") = 2;
	NI_ExternalIO rtIOAttribs[] DataSection(".NIVS.extlist") =
	{
	  { 0, "Out2", 0, 1, 1, 1, 1 },

	  { 1, "Out1/matrix", 0, 1, 6, 3, 2 },
	};
 */
int32_t ExtIOSize DataSection(".NIVS.extlistsize") = ;
NI_ExternalIO rtIOAttribs[] DataSection(".NIVS.extlist") = ;
int32_t InportSize = ;
int32_t OutportSize = ;

 /*========================================================================*
   Model name and build information 
   
   Example:
   NI_ModelName DataSection(".NIVS.compiledmodelname") = "My Model Name";
   NI_Builder DataSection(".NIVS.builder") = "NI Model Framework for My Product ";
   NI_BuilderVersion NI_BuilderVersion DataSection(".NIVS.builderversion") = "2.0.0.0";
 */
const char * USER_ModelName DataSection(".NIVS.compiledmodelname") = ;
const char * USER_Builder DataSection(".NIVS.builder") = ;
const char * USER_BuilderVersion DataSection(".NIVS.builderversion") = ;

 /*========================================================================*
   baserate is the rate at which the model runs, and timestamp is the current 
   model time 
 */
double USER_BaseRate = ;

NI_Task rtTaskAttribs DataSection(".NIVS.tasklist") = 

 /*========================================================================*
 * Function: USER_SetValueByDataType
 *
 * Abstract:
 *		Implementation for setting values of user defined types of Parameters
 *		The datatype field of NI_Parameter is user defined. In this default 
 *		implementation, we have provided examples of how to support the datatypes 
 *		double and single.
 *
 * Parameters:
 *      ptr : base address of where value should be set.
 *      subindex : offset into ptr where value should be set
 *      value : the value to be set
 *      type : the user defined type 
 *
 * Returns:
 *      NI_ERROR on error, NI_OK otherwise
========================================================================*/
int32_t USER_SetValueByDataType(void* ptr, int32_t subindex, double value, int32_t type)
{	
	switch (type) 
	{
		case 0: 
		{
			/* double */
    		((double *)ptr)[subindex] = (double)value;
    		return NI_OK;
		}
    	case 1: 
		{
			/* single */
    		((float *)ptr)[subindex] = (float)value;
    		return NI_OK;
		}
	}
	
  	return NI_ERROR;
}

/*========================================================================*
 * Function: USER_GetValueByDataType
 *
 * Abstract:
 *		Implementation for getting values of user defined types of Parameters 
 *		and Signals. The datatype field of both NI_Parameter and 
 *		NI_Signal is user defined. In this default implementation, we have 
 *		provided examples of how to support the datatypes double and single.
 *
 * Parameters:
 *      ptr : base address of where value is found
 *      subindex : offset into ptr where value is found
 *      type : the user defined type
 *
 * Returns:
 *      The value as a double data type
========================================================================*/
double USER_GetValueByDataType(void* ptr, int32_t subindex, int32_t type)
{
	switch (type) 
	{
		case 0: 
		{
			/* double */
			return (double)(((double *)ptr)[subindex]);
		}
		case 1: 
		{
			/* single */
			return (double)(((float *)ptr)[subindex]);
		}
  	}
	
	/* return NaN, ok for vxworks and pharlap */
	uint32_t nan[2] = {0xFFFFFFFF, 0xFFFFFFFF};
	return *(double*)nan;
}

/*========================================================================*
 * Function: USER_Initialize
 *
 * Abstract:
 *		User initialization code is placed in this function. The best practice is not to use Parameter values here 
 *		since they can be changed after initialization but before start
 *
 * Parameters:
 *
 * Returns:
 *      NI_ERROR on error, NI_OK otherwise
========================================================================*/
long USER_Initialize() 
{
	return NI_OK;
}

/*========================================================================*
 * Function: USER_ModelStart
 *
 * Abstract:
 *		User code to be executed before Model execution starts is placed in this function.
 *
 * Parameters:
 *
 * Returns:
 *      NI_ERROR on error, NI_OK otherwise
========================================================================*/
long USER_ModelStart() 
{
	return NI_OK;
}

 /*========================================================================*
 * Function: USER_TakeOneStep
 *
 * Abstract:
 *		Place simulation code to be executed on every iteration of the baserate
 *
 * Parameters:
 *      inData : pointer to inport data at the current timestamp, to be consumed by the function
 *      outData : pointer to outport data at current time + baserate, to be produced by the function
 *      timestamp : current simulation time
 *
 * Returns:
 *      NI_ERROR on error, NI_OK otherwise
========================================================================*/
int32_t USER_TakeOneStep(double *inData, double *outData, double timestamp) 
{
	return NI_OK;
}

/*========================================================================*
 * Function: USER_ModelStart
 *
 * Abstract:
 *		User finalization code is placed in this function.
 *
 * Parameters:
 *
 * Returns:
 *      NI_ERROR on error, NI_OK otherwise
========================================================================*/
long USER_Finalize() 
{
	return NI_OK;
}
