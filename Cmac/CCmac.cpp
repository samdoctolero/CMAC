#include "CCmac.h"
#include "Cmac.h"
#include "SupervisoryCmac.h"
#include "WeightSmoothingCmac.h"
#include <typeinfo>


// ===========================================
// Create Basic CMAC
// ===========================================
// Constructor of a basic CMAC with array of learing rates
void * CreateCmacA(unsigned int numInputs, unsigned int numOutputs, unsigned int numQ, unsigned int numLayers
	, double maxlimit[], double minlimit[], double beta[], double nu)
{
	// create vectors
	vecd max(numInputs, 0.0);
	vecd min(numInputs, 0.0);
	for (uint i = 0; i < numInputs; i++)
	{
		max[i] = maxlimit[i];
		min[i] = minlimit[i];
	}

	// create vectors associated with numOutputs
	vecd be(numOutputs, 0.0);
	for (uint i = 0; i < numOutputs; i++)
	{
		be[i] = beta[i];
	}

	// create instance
	ICmac* result = new Cmac(numOutputs, numQ, numLayers
		, max, min, be, nu);

	return result;
}

// Constructor of a basic CMAC with a scalar learning rate
void * CreateCmacB(unsigned int numInputs, unsigned int numOutputs, unsigned int numQ, unsigned int numLayers
	, double maxlimit[], double minlimit[], double beta, double nu)
{
	// create vectors
	vecd max(numInputs, 0.0);
	vecd min(numInputs, 0.0);
	for (uint i = 0; i < numInputs; i++)
	{
		max[i] = maxlimit[i];
		min[i] = minlimit[i];
	}

	// create instance
	ICmac* result = new Cmac(numOutputs, numQ, numLayers
		, max, min, beta, nu);

	return result;
}


// ============================================
// Create Supervisory CMAC
// ============================================
// Constructor of a supervisory CMAC with array of learning rates
void * CreateSupervisoryCmacA(unsigned int numInputs, unsigned int numOutputs
	, unsigned int numQ, unsigned int numLayers
	, double maxlimit[], double minlimit[]
	, double beta[], double nu
	, double supervisoryValues[], double supervisoryNu)
{
	// create vectors associated with numInputs
	vecd max(numInputs, 0.0);
	vecd min(numInputs, 0.0);
	for (uint i = 0; i < numInputs; i++)
	{
		max[i] = maxlimit[i];
		min[i] = minlimit[i];
		
	}

	// create vectors associated with numOutputs
	vecd be(numOutputs, 0.0);
	vecd supVal(numOutputs, 0.0);
	for (uint i = 0; i < numOutputs; i++)
	{
		supVal[i] = supervisoryValues[i];
		be[i] = beta[i];
	}

	// create instance
	ICmac* result = new SupervisoryCmac(numOutputs, numQ, numLayers
		, max, min, be, nu, supVal, supervisoryNu);

	return result;
}

// Constructor of a Supervisory CMAC with a scalar learning rate
void * CreateSupervisoryCmacB(unsigned int numInputs, unsigned int numOutputs
	, unsigned int numQ, unsigned int numLayers
	, double maxlimit[], double minlimit[]
	, double beta, double nu
	, double supervisoryValues[], double supervisoryNu)
{
	// create vectors associated with numInputs
	vecd max(numInputs, 0.0);
	vecd min(numInputs, 0.0);
	for (uint i = 0; i < numInputs; i++)
	{
		max[i] = maxlimit[i];
		min[i] = minlimit[i];

	}

	// create vectors associated with numOutputs
	vecd supVal(numOutputs, 0.0);
	for (uint i = 0; i < numOutputs; i++)
	{
		supVal[i] = supervisoryValues[i];
	}

	// create instance
	ICmac* result = new SupervisoryCmac(numOutputs, numQ, numLayers
		, max, min, beta, nu, supVal, supervisoryNu);

	return result;
}

// ============================================
// Create Weight Smoothing CMAC
// ============================================

void * CreateWeightSmoothingCmacA(unsigned int numInputs, unsigned int numOutputs, unsigned int numQ, unsigned int numLayers, double maxlimit[], double minlimit[], double beta[], double nu, double supervisoryValues[], double supervisoryNu)
{
	// create vectors
	vecd max(numInputs, 0.0);
	vecd min(numInputs, 0.0);
	for (uint i = 0; i < numInputs; i++)
	{
		max[i] = maxlimit[i];
		min[i] = minlimit[i];
	}

	// create vectors associated with numOutputs
	vecd be(numOutputs, 0.0);
	for (uint i = 0; i < numOutputs; i++)
	{
		be[i] = beta[i];
	}

	// create instance
	ICmac* result = new WeightSmoothingCmac(numOutputs, numQ, numLayers
		, max, min, be, nu);

	return result;
}

void * CreateWeightSmoothingCmacB(unsigned int numInputs, unsigned int numOutputs, unsigned int numQ, unsigned int numLayers, double maxlimit[], double minlimit[], double beta, double nu, double supervisoryValues[], double supervisoryNu)
{
	// create vectors
	vecd max(numInputs, 0.0);
	vecd min(numInputs, 0.0);
	for (uint i = 0; i < numInputs; i++)
	{
		max[i] = maxlimit[i];
		min[i] = minlimit[i];
	}

	// create instance
	ICmac* result = new WeightSmoothingCmac(numOutputs, numQ, numLayers
		, max, min, beta, nu);

	return result;
}

// ====================================
// Destroy methods for all cmacs
// ====================================
// Destroys a CMAC, given a CMAC handle
void DestroyCmac(void * cmacHandle)
{
	if (cmacHandle != NULL)
	{
		delete cmacHandle;
	}
}

// ====================================
// Calculate, Train and RMS methods
// ====================================
// Calls the calculate method of a CMAC
void CalculateCmac(void* handle
	, double inputs[], unsigned int numInputs
	, double output[], unsigned int numOutputs)
{
	if (handle != NULL)
	{
		ICmac* cmac = static_cast<ICmac*>(handle);
		
		// create vector inputs
		vecd in(numInputs, 0.0);
		for (uint i = 0; i < numInputs; i++)
		{
			in[i] = inputs[i];
		}

		// call calculate
		vecd out = cmac->Calculate(in);

		// check if sizes match
		if (out.size() != numOutputs)
		{
			throw "Output sizes do not match in CalculateCmac.";
		}

		// convert out to an array
		for (uint i = 0; i < numOutputs; i++)
		{
			output[i] = out[i];
		}

	}
}

// Calls the train method of a CMAC
void TrainCmac(void* handle
	, double error[], unsigned int numError
	, double damp)
{
	if (handle != NULL)
	{
		ICmac* cmac = static_cast<ICmac*>(handle);

		// create vector errors
		vecd err(numError, 0.0);
		for (uint i = 0; i < numError; i++)
		{
			err[i] = error[i];
		}

		// call train
		cmac->Train(err, damp);
	}
}

// Calls the RMS weight function of a CMAC
void GetRmsWeightsCmac(void* handle
	, double rms[], unsigned int numRms)
{
	if (handle != NULL)
	{
		ICmac* cmac = static_cast<ICmac*>(handle);

		vecd rmsWeights = cmac->GetRmsWeights();

		if (rmsWeights.size() != numRms)
		{
			throw "Output sizes do not match in GetRmsWeightsCmac.";
		}

		for (uint i = 0; i < numRms; i++)
		{
			rms[i] = rmsWeights[i];
		}
	}
}


// ====================================
// Other handy functions
// ====================================
// Gets the norm of an array
double Norm(double input[], unsigned int numInput)
{
	double result = 0.0;
	for (uint i = 0; i < numInput; i++)
	{
		result += input[i] * input[i];
	}

	return sqrt(result);
}

// Gets the rms of an array
double Rms(double input[], unsigned int numInput)
{
	double result = 0.0;
	for (uint i = 0; i < numInput; i++)
	{
		result += input[i] * input[i];
	}

	result /= (double)numInput;

	return sqrt(result);
}