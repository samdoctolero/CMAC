#pragma once
// =========================================
// CMAC library C wrapper
// =========================================

#ifdef __cplusplus
extern "C" 
{
#endif
	// ===================================
	// Create Basic CMAC
	// ===================================
	// This constructor takes in an array of beta (learning rate) values
	void * CreateCmacA(unsigned int numInputs, unsigned int numOutputs
		, unsigned int numQ, unsigned int numLayers
		, double maxlimit[], double minlimit[]
		, double beta[], double nu);

	// This constructor takes in a scalar (learning rate) value
	void * CreateCmacB(unsigned int numInputs, unsigned int numOutputs
		, unsigned int numQ, unsigned int numLayers
		, double maxlimit[], double minlimit[]
		, double beta, double nu);

	// ====================================
	// Create Supervisory CMAC
	// ====================================
	// This constructor takes in an array of beta (learning rate) values
	void * CreateSupervisoryCmacA(unsigned int numInputs, unsigned int numOutputs
		, unsigned int numQ, unsigned int numLayers
		, double maxlimit[], double minlimit[]
		, double beta[], double nu
		, double supervisoryValues[], double supervisoryNu);

	// This constructor takes in a scalar (learning rate) value
	void * CreateSupervisoryCmacB(unsigned int numInputs, unsigned int numOutputs
		, unsigned int numQ, unsigned int numLayers
		, double maxlimit[], double minlimit[]
		, double beta, double nu
		, double supervisoryValues[], double supervisoryNu);

	// ====================================
	// Destroy methods for all cmacs
	// ====================================
	// A singular destroy methods to delete a CMAC
	void DestroyCmac(void* cmacHandle);

	// ====================================
	// Calculate, Train and RMS methods
	// ====================================
	// Calls the calculate method of a CMAC handle
	void CalculateCmac(void* handle
		, double inputs[], unsigned int numInputs
		, double output[], unsigned int numOutputs);

	// Calls the train method of a CMAC handle
	void TrainCmac(void* handle
		, double error[], unsigned int numError
		, double damp);

	// Calls teh get rms weights method of a CMAC handle
	void GetRmsWeightsCmac(void* handle
		, double rms[], unsigned int numRms);

	// ====================================
	// Other handy functions
	// ====================================
	// Takes the norm of an array
	double Norm(double input[], unsigned int numInput);

	// Takes the rms of an array
	double Rms(double input[], unsigned int numInput);

#ifdef __cplusplus
}
#endif
