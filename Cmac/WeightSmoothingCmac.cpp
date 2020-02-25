#include "WeightSmoothingCmac.h"
#include "CmacImpl.h"

// ===============================================================
// WeightSmoothingCmacImpl Implementation
// ===============================================================

class WeightSmoothingCmac::WeightSmoothingCmacImpl
	: public Cmac::CmacImpl
{
private:
	// override adjust weight function
	void AdjustWeight(uint outputIndex, uint layerIndex, vecd& error, double damp) override;

public:
	WeightSmoothingCmacImpl(uint numOutputs
		, uint numQ, uint numLayers, vecd maxlimit
		, vecd minlimit, vecd beta, double nu);
};

void WeightSmoothingCmac::WeightSmoothingCmacImpl::AdjustWeight(
	uint outputIndex, uint layerIndex, vecd & error, double damp)
{
	// get the left weight 
	double leftWeight = 0.0;
	if (layerIndex > 0)
	{
		// grab the weight on the left
		leftWeight = *m_Weights[outputIndex][layerIndex - 1];
	}
	else
	{
		// grab the last one
		leftWeight = *m_Weights[outputIndex][m_NumLayers - 1];
	}

	// get the right weight
	double rightWeight = 0.0;
	if (layerIndex < m_NumLayers - 1)
	{
		rightWeight = *m_Weights[outputIndex][layerIndex + 1];
	}
	else
	{
		rightWeight = *m_Weights[outputIndex][0];
	}

	double error_adjustment = m_Gammas[layerIndex] * error[outputIndex];
	double damping_adjustment = m_Nu*damp*(2*(*m_Weights[outputIndex][layerIndex]) 
												- rightWeight - leftWeight);
	double dw = m_Beta[outputIndex] * (error_adjustment - damping_adjustment);
	*m_Weights[outputIndex][layerIndex] += dw;
}

WeightSmoothingCmac::WeightSmoothingCmacImpl::WeightSmoothingCmacImpl(
	uint numOutputs, uint numQ, uint numLayers, vecd maxlimit, 
	vecd minlimit, vecd beta, double nu)
	: Cmac::CmacImpl(numOutputs, numQ, numLayers
		, maxlimit, minlimit, beta, nu){}



// ==============================================================
// WeightSmoothingCmac Implementation
// ==============================================================


WeightSmoothingCmac::WeightSmoothingCmac(uint numOutputs
	, uint numQ, uint numLayers, vecd maxlimit
	, vecd minlimit, double beta, double nu)
	: Cmac(new WeightSmoothingCmac::WeightSmoothingCmacImpl(
		numOutputs, numQ, numLayers, maxlimit,
		minlimit, vecd(numOutputs, beta), nu)){}

WeightSmoothingCmac::WeightSmoothingCmac(uint numOutputs
	, uint numQ, uint numLayers, vecd maxlimit
	, vecd minlimit, vecd beta, double nu)
	: Cmac(new WeightSmoothingCmac::WeightSmoothingCmacImpl(
		numOutputs, numQ, numLayers, maxlimit,
		minlimit, beta, nu)) {}

WeightSmoothingCmac::~WeightSmoothingCmac()
{
	if (pImpl != NULL)
	{
		delete pImpl;
	}
}
