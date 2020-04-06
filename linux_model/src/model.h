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
