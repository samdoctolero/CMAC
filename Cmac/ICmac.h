#pragma once
#include "Common.h"

// Interface class that all CMAC types will inherit
class ICmac
{
public:
	virtual vecd Calculate(vecd &input) = 0;
	virtual void Train(vecd &error, double damp) = 0;
	virtual vecd GetRmsWeights() = 0;
};

