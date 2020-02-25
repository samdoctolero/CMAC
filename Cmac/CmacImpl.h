#pragma once
#include "Cmac.h"
// =========================================================
// NOTE: This file does not go out externally !!!!
// =========================================================


// =========================================================
// CmacImpl
//==========================================================
// Basic CMAC implementation
class Cmac::CmacImpl
{
protected:
	// weights related
	matd m_Memory;		// indexing: [output_index][memory_index]
	matdp m_Weights;	// indexing: [output_index][memory_index]
	vecd m_Gammas;
	veci m_Hashtable;
	veci m_Locations;
	matd m_Offsets;		// indexing: [input_index][layer_index]
	vecd m_Output;
	vecd m_NormalizingDenom;
	vecd m_MaxLimit;
	vecd m_MinLimit;
	vecd m_RmsWeights;
	vecd m_NormalizedInputs;

	// training related
	vecd m_Beta;
	double m_Nu;

	// constant values
	uint m_NumInputs;
	uint m_NumOutputs;
	uint m_NumQ;
	uint m_NumLayers;
	uint m_MemSize;

	// initialization methods
	// Initializes the hashtable and memory arrays
	void InitializeHashtableAndMemory();
	// Initializes the normalize denominator
	void InitializeNormalizedDenominator();
	// Initializes the offsets
	void InitializeOffsets();

	// methods
	// Normalizes the inputs and stores it
	void NormalizeInput(vecd &input);
	// Grabs the gamma values and weight locations
	void CalculateGammas();
	// Multiplies the gammas with weights then sums them up
	void MultiplyGammasWeights();
	// Calculates the weight adjustments and adds it to the current weight
	virtual void AdjustWeight(uint outputIndex, uint layerIndex, vecd& error, double damp);

public:
	// CmacImpl constructor
	CmacImpl(uint numOutputs, uint numQ, uint numLayers
		, vecd maxlimit, vecd minlimit, vecd beta, double nu);

	~CmacImpl();

	virtual vecd Calculate(vecd &input);
	void Train(vecd &error, double damp);
	vecd GetRmsWeights();
};