#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\Cmac\CCmac.h"
#include "..\Cmac\Common.h"
#include <math.h>
#include <random>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CmacUnitTests
{		
	TEST_CLASS(CmacUnitTests)
	{
	public:
		
		TEST_METHOD(BasicCalculation)
		{
			double max[] = { 2.0 * 3.14 };
			double min[] = { -2.0 * 3.14 }; 
			std::random_device r;
			void* cmac = CreateCmacB(1, 2, 20, 50, max, min, 10.0, 0.0);

			unsigned int max_tries = 10000;
			double threshold = 1e-3;
			for (unsigned i = 0; i < max_tries; i++)
			{
				double val = 3.14*(double)r() / (double)r.max();
				double actual[] = { sin(val) + 1.0, 2.0*cos(0.5*val) + 2.5 };

				double input[] = { val };
				double estimated[] = { 0.0, 0.0 };
				CalculateCmac(cmac, input, 1, estimated, 2);

				double error[] = { actual[0] - estimated[0]
								, actual[1] - estimated[1] };

				TrainCmac(cmac, error, 2, 1.0);

				if (abs(error[0]) < threshold && abs(error[1]) < threshold)
				{
					DestroyCmac(cmac);
					Assert::IsTrue(true);
					return;
				}		
			}

			DestroyCmac(cmac);
			Assert::Fail();
		}


		TEST_METHOD(SupervisoryCalculation)
		{
			double max[] = { 2.0 * 3.14 };
			double min[] = { -2.0 * 3.14 };
			double sup[] = { 0.5, 1.0 };
			std::random_device r;
			void* cmac = CreateSupervisoryCmacB(1, 2, 20, 50, max, min, 2.0, 0.0, sup, 0.1);

			unsigned int max_tries = 100000;
			double threshold = 1e-3;
			for (unsigned i = 0; i < max_tries; i++)
			{
				double val = 3.14*(double)r() / (double)r.max();
				double actual[] = { sin(val) + 1.0, 2.0*cos(0.5*val) + 2.5 };

				double input[] = { val };
				double estimated[] = { 0.0, 0.0 };
				CalculateCmac(cmac, input, 1, estimated, 2);

				double error[] = { actual[0] - estimated[0]
					, actual[1] - estimated[1] };

				TrainCmac(cmac, error, 2, 1.0);

				if (abs(error[0]) < threshold && abs(error[1]) < threshold)
				{
					DestroyCmac(cmac);
					Assert::IsTrue(true);
					return;
				}
			}

			DestroyCmac(cmac);
			Assert::Fail();
		}


		TEST_METHOD(SupervisoryCalculationDifferentBeta)
		{
			double max[] = { 2.0 * 3.14 };
			double min[] = { -2.0 * 3.14 };
			double sup[] = { 0.5, 1.0 };
			double betas[] = { 10.0,  1.0 };
			std::random_device r;
			void* cmac = CreateSupervisoryCmacA(1, 2, 20, 50, max, min, betas, 0.0, sup, 0.1);

			unsigned int max_tries = 10000;
			double threshold = 1e-3;
			for (unsigned i = 0; i < max_tries; i++)
			{
				double val = 3.14*(double)r() / (double)r.max();
				double actual[] = { sin(val) + 1.0, 2.0*cos(0.5*val) + 2.5 };

				double input[] = { val };
				double estimated[] = { 0.0, 0.0 };
				CalculateCmac(cmac, input, 1, estimated, 2);

				double error[] = { actual[0] - estimated[0]
					, actual[1] - estimated[1] };

				TrainCmac(cmac, error, 2, 1.0);

				if (abs(error[0]) < threshold && abs(error[1]) < threshold)
				{
					DestroyCmac(cmac);
					Assert::IsTrue(true);
					return;
				}
			}

			DestroyCmac(cmac);
			Assert::Fail();
		}
		

	};
}