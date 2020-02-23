#pragma once
#include "ICmac.h"

// Base CMAC class
class Cmac
	: public ICmac
{
protected:
	class CmacImpl;
	CmacImpl* pImpl;

	Cmac(CmacImpl *pimpl);

public:
	// Constructor with a scalar learning rate
	Cmac(uint numOutputs, uint numQ, uint numLayers
		, vecd maxlimit, vecd minlimit, double beta, double nu);
	// Constructor with array learning rates
	Cmac(uint numOutputs, uint numQ, uint numLayers
		, vecd maxlimit, vecd minlimit, vecd beta, double nu);
	~Cmac();


	// Inherited via ICmac
	virtual vecd Calculate(vecd &input) override;

	virtual void Train(vecd &error, double damp) override;

	virtual vecd GetRmsWeights() override;

};

