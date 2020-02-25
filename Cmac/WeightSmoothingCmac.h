#pragma once
#include "Cmac.h"

class WeightSmoothingCmac :
	public Cmac
{
private:
	class WeightSmoothingCmacImpl;

public:
	WeightSmoothingCmac(uint numOutputs, uint numQ, uint numLayers
		, vecd maxlimit, vecd minlimit, double beta, double nu);
	WeightSmoothingCmac(uint numOutputs, uint numQ, uint numLayers
		, vecd maxlimit, vecd minlimit, vecd beta, double nu);
	~WeightSmoothingCmac();
};

