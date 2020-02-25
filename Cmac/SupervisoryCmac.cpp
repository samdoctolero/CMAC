
#include "SupervisoryCmac.h"
#include "CmacImpl.h"

// ====================================================================
// SupervisoryCmacImpl
// ====================================================================
class SupervisoryCmac::SupervisoryCmacImpl 
	: public Cmac::CmacImpl
{
private:
	vecd m_SupervisingValues;
	double m_SupervisoryNu;

	// override adjust weight function
	void AdjustWeight(uint outputIndex, uint layerIndex, vecd& error, double damp) override;

public:
	// Constructor
	SupervisoryCmacImpl(uint numOutputs, uint numQ, uint numLayers
		, vecd maxlimit, vecd minlimit, vecd beta, double nu
		, vecd supervisingValues, double supervisoryNu);

	vecd Calculate(vecd &input) override;

};

SupervisoryCmac::SupervisoryCmacImpl::SupervisoryCmacImpl(uint numOutputs, uint numQ, uint numLayers
	, vecd maxlimit, vecd minlimit, vecd beta, double nu
	, vecd supervisingValues, double supervisoryNu)
	: Cmac::CmacImpl(numOutputs, numQ, numLayers, maxlimit, minlimit, beta, nu)
		, m_SupervisingValues(supervisingValues), m_SupervisoryNu(supervisoryNu){}

vecd SupervisoryCmac::SupervisoryCmacImpl::Calculate(vecd & input)
{
	vecd result = Cmac::CmacImpl::Calculate(input);

	// append the supervisory values
	for (uint i = 0; i < m_NumOutputs; i++)
	{
		result[i] += m_SupervisingValues[i];
	}

	return result;
}

void SupervisoryCmac::SupervisoryCmacImpl::AdjustWeight(uint outputIndex, uint layerIndex, vecd & error, double damp)
{
	double error_adjustment = m_Gammas[layerIndex] * error[outputIndex];
	double damping_adjustment = m_Nu*damp*(*m_Weights[outputIndex][layerIndex]);
	double supervisory_adjustment = m_Gammas[layerIndex]*m_SupervisoryNu*damp*(m_SupervisingValues[outputIndex] - m_Output[outputIndex]);
	double dw = m_Beta[outputIndex] * (error_adjustment + supervisory_adjustment  - damping_adjustment);
	*m_Weights[outputIndex][layerIndex] += dw;
}



// ===================================================================
// SupervisoryCmac Implementation
// ===================================================================
SupervisoryCmac::SupervisoryCmac(uint numOutputs, uint numQ, uint numLayers
	, vecd maxlimit, vecd minlimit, double beta, double nu
	, vecd supervisingValues, double supervisoryNu)
	: Cmac(new SupervisoryCmac::SupervisoryCmacImpl(
		numOutputs, numQ, numLayers, maxlimit, minlimit
		, vecd(numOutputs, beta), nu
		, supervisingValues, supervisoryNu)){}

SupervisoryCmac::SupervisoryCmac(uint numOutputs, uint numQ, uint numLayers
	, vecd maxlimit, vecd minlimit, vecd beta, double nu
	, vecd supervisingValues, double supervisoryNu)
	: Cmac(new SupervisoryCmac::SupervisoryCmacImpl(
		numOutputs, numQ, numLayers, maxlimit, minlimit
		, beta, nu, supervisingValues, supervisoryNu)){}

SupervisoryCmac::~SupervisoryCmac()
{
	if (pImpl != NULL)
	{
		delete pImpl;
	}
}

