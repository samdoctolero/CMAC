#include "Cmac.h"
#include "CmacImpl.h"
#include <random>
#include <math.h>

// =========================================================
// CmacImpl Implementation
// =========================================================
// CmacImpl constructor
Cmac::CmacImpl::CmacImpl(uint numOutputs, uint numQ, uint numLayers
	, vecd maxlimit, vecd minlimit, vecd beta, double nu)
	: m_Weights(numOutputs, vecdp(numLayers)), m_Gammas(numLayers, 0.0)
		, m_Hashtable(maxlimit.size()*numLayers*numQ + numQ*numLayers + numQ)
		, m_Locations(numLayers, 0), m_Offsets(maxlimit.size(), vecd(numLayers, 0.0))
		, m_Output(numOutputs,0.0), m_NormalizingDenom(maxlimit.size(),0.0)
		, m_MaxLimit(maxlimit), m_MinLimit(minlimit), m_RmsWeights(numOutputs, 0.0)
		, m_Beta(beta), m_Nu(nu), m_NumInputs(maxlimit.size()), m_NumOutputs(numOutputs)
		, m_NumQ(numQ), m_NumLayers(numLayers), m_NormalizedInputs(maxlimit.size(), 0.0)
{
	if (maxlimit.size() != minlimit.size())
	{
		throw "Limit sizes do not match";
	}

	InitializeHashtableAndMemory();
	InitializeNormalizedDenominator();
	InitializeOffsets();
}

Cmac::CmacImpl::~CmacImpl(){}

void Cmac::CmacImpl::InitializeHashtableAndMemory()
{
	m_MemSize = 0;
	std::random_device r;
	for (uint i = 0; i < m_NumInputs*m_NumLayers*m_NumQ; i++)
	{
		double num = 10000.0*((double)r()) / ((double)r.max());
		m_Hashtable[i] = (uint)num;
		// check for max
		if (m_Hashtable[i] > m_MemSize)
		{
			m_MemSize = m_Hashtable[i];
		}
	}

	m_Memory = matd(m_NumOutputs, vecd(m_MemSize, 0.0));
}

void Cmac::CmacImpl::InitializeNormalizedDenominator()
{
	for (uint i = 0; i < m_NumInputs; i++)
	{
		m_NormalizingDenom[i] = m_MaxLimit[i] - m_MinLimit[i];
	}
}

void Cmac::CmacImpl::InitializeOffsets()
{
	std::random_device r;
	for (uint i = 0; i < m_NumInputs; i++)
	{
		for (uint j = 0; j < m_NumLayers; j++)
		{
			double offset = ((double)r()) / ((double)r.max());
			m_Offsets[i][j] = offset;
		}
	}
}

void Cmac::CmacImpl::NormalizeInput(vecd & input)
{
	for (uint i = 0; i < m_NumInputs; i++)
	{
		m_NormalizedInputs[i] = (input[i] - m_MinLimit[i]) / m_NormalizingDenom[i];
		m_NormalizedInputs[i] = m_NormalizedInputs[i] < 1.0 ? m_NormalizedInputs[i] : 1.0;
		m_NormalizedInputs[i] = m_NormalizedInputs[i] > 0.0 ? m_NormalizedInputs[i] : 0.0;
	}
}

void Cmac::CmacImpl::CalculateGammas()
{
	double sum_gamma = 0.0;
	for (uint i = 0; i < m_NumLayers; i++)
	{
		double total_locations = 0.0;
		vecd cell(m_NumInputs, 0.0);
		m_Gammas[i] = 1.0;
		for (uint j = 0; j < m_NumInputs; j++)
		{
			double place = m_NormalizedInputs[j] * ((double)(m_NumQ - 1)) + m_Offsets[j][i];
			cell[j] = (uint)place;
			double h = place - cell[j];

			double func = 16.0*(h*h - 2.0*h*h*h + h*h*h*h);
			m_Gammas[i] *= func;

			uint index = (uint)cell[j] + m_NumQ*i + m_NumQ*m_NumLayers*j;

			total_locations = total_locations + m_Hashtable[index];
			uint loc = (uint)fmod(total_locations, (double)(m_MemSize - 1));
			m_Locations[i] = loc;
		}

		sum_gamma += m_Gammas[i];

		// get weights
		for (uint k = 0; k < m_NumOutputs; k++)
		{
			m_Weights[k][i] = &m_Memory[k][m_Locations[i]];
		}
	}

	// normalize gammas
	for (int i = 0; i < m_NumLayers; i++)
	{
		m_Gammas[i] /= sum_gamma;
	}
}

void Cmac::CmacImpl::MultiplyGammasWeights()
{
	for (uint i = 0; i < m_NumOutputs; i++)
	{
		m_Output[i] = 0.0;
		m_RmsWeights[i] = 0.0;
		for (uint j = 0; j < m_NumLayers; j++)
		{
			m_Output[i] += m_Gammas[j] * (*m_Weights[i][j]);
			m_RmsWeights[i] += (*m_Weights[i][j])*(*m_Weights[i][j]);
		}
		m_RmsWeights[i] = std::sqrt(m_RmsWeights[i] / m_NumLayers);
	}
}

void Cmac::CmacImpl::AdjustWeight(uint outputIndex, uint layerIndex, vecd & error, double damp)
{
	double error_adjustment = m_Gammas[layerIndex] * error[outputIndex];
	double damping_adjustment = m_Nu*damp*(*m_Weights[outputIndex][layerIndex]);
	double dw = m_Beta[outputIndex] * (error_adjustment - damping_adjustment);
	*m_Weights[outputIndex][layerIndex] += dw;
}


vecd Cmac::CmacImpl::Calculate(vecd & input)
{
	NormalizeInput(input);
	CalculateGammas();
	MultiplyGammasWeights();

	return m_Output;
}

void Cmac::CmacImpl::Train(vecd & error, double damp)
{
	for (uint i = 0; i < m_NumOutputs; i++)
	{
		for (uint j = 0; j < m_NumLayers; j++)
		{
			AdjustWeight(i, j, error, damp);
		}
	}
}

vecd Cmac::CmacImpl::GetRmsWeights()
{
	return m_RmsWeights;
}


// =========================================================
// Public CMAC implementation
// =========================================================

Cmac::Cmac(CmacImpl * pimpl)
{
	if (pImpl != NULL)
	{
		delete pImpl;
	}

	pImpl = pimpl;
}

Cmac::Cmac(uint numOutputs, uint numQ, uint numLayers
	, vecd maxlimit, vecd minlimit, double beta, double nu)
	: pImpl(new Cmac::CmacImpl(numOutputs, numQ, numLayers
		, maxlimit, minlimit, vecd(numOutputs, beta), nu)){}

Cmac::Cmac(uint numOutputs, uint numQ, uint numLayers
	, vecd maxlimit, vecd minlimit, vecd beta, double nu)
	: pImpl(new Cmac::CmacImpl(numOutputs, numQ, numLayers
		, maxlimit, minlimit, beta, nu)){}


Cmac::~Cmac()
{
	if (pImpl != NULL)
	{
		delete pImpl;
	}
}

vecd Cmac::Calculate(vecd &input)
{
	return pImpl->Calculate(input);
}

void Cmac::Train(vecd &error, double damp)
{
	pImpl->Train(error, damp);
}

vecd Cmac::GetRmsWeights()
{
	return pImpl->GetRmsWeights();
}

