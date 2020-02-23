#pragma once
#include "Cmac.h"
// A CMAC that has supervisory values (biases)
class SupervisoryCmac
	: public Cmac
{
private:
	class SupervisoryCmacImpl;

public:
	// Constructor with a scalar learning rate
	SupervisoryCmac(uint numOutputs, uint numQ, uint numLayers
		, vecd maxlimit, vecd minlimit, double beta, double nu
		, vecd supervisingValues, double supervisoryNu);
	// Constructor with array of learning rates
	SupervisoryCmac(uint numOutputs, uint numQ, uint numLayers
		, vecd maxlimit, vecd minlimit, vecd beta, double nu
		, vecd supervisingValues, double supervisoryNu);

	~SupervisoryCmac();
};

