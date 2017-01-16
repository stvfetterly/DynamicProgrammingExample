// LazerFiche.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <assert.h>
#include <vector>
#include <algorithm>

/// <summary>
/// Facilitates dispensing stamps for a postage stamp machine.
/// </summary>
class StampDispenser
{
	/// <remarks>
	/// m_denominationVector contains the user submitted stamp denominiation values
	/// and is filled upon object creation
	/// </remarks>
	std::vector<int> m_denominationVector;

	/// <summary>
	/// Tests user input for the following conditions:
	/// - Number of stamp denominations are greater than zero
	///	- A value of '1' is included in the denominations recieved
	///	- Denominations recieved are sorted in descending order
	///	- No negative value stamps recieved
	/// <param name="stampDenominations">
	/// The values of the types of stamps that the machine has.  
	/// </param>
	/// <param name="numStampDenominations">
	/// The number of types of stamps in the stampDenominations array.
	/// </param>
	/// <returns>
	/// True if all input conditions are met, false otherwise.
	/// </returns>
	bool ValidateInput(const int* stampDenominations, size_t numStampDenominations) const
	{
		//ensure that input given is a value greater than zero
		assert(0 < numStampDenominations);

		bool oneFound = false;			//Variable used to determine if a stamp of value '1' is in the data provided
		int lastVal = INT_MAX;			//Variable used to determine if stamp denominations are in descending order

		for (unsigned int i = 0; i < numStampDenominations; i++)
		{
			//ensure that values are sorted in descending order
			assert(lastVal >= stampDenominations[i]);

			//stamps should not be negative values
			assert(stampDenominations[i] >= 0);

			//if a value of one has been found, record it
			if (1 == stampDenominations[i])
			{
				oneFound = true;
			}

			//Update last value encountered
			lastVal = stampDenominations[i];
		}

		assert(oneFound);

		//stamps denominations must include the value 1
		return true;
	}

public:
	/// <summary>
	/// Initializes a new instance of the <see cref="StampDispenser"/> class that will be 
	/// able to dispense the given types of stamps.
	/// </summary>
	/// <param name="stampDenominations">
	/// The values of the types of stamps that the machine has.  
	/// Should be sorted in descending order and contain at least a 1.
	/// </param>
	/// <param name="numStampDenominations">
	/// The number of types of stamps in the stampDenominations array. 
	/// </param>
	StampDispenser(const int* stampDenominations, size_t numStampDenominations)
	{
		//Make sure that the data we've been given meets the criteria expected
		if (ValidateInput(stampDenominations, numStampDenominations))
		{
			//Save the data as a vector in our Stamp Dispenser object
			for (unsigned int i = 0; i < numStampDenominations; i++)
			{
				m_denominationVector.push_back(stampDenominations[i]);
			}
		}
	}

	/// <summary>
	/// Returns the minimum number of stamps that the machine can dispense to
	/// fill the given request.  Creates/initializes memoization vector and calls 
	/// CalcNumStamps to generate solution.
	/// </summary>
	/// <param name="request">
	/// The total value of the stamps to be dispensed.
	/// </param>
	/// <returns>
	/// The minimum number of stamps needed to fill the given request.
	/// </returns>
	int CalcNumStampsToFillRequest(int request)
	{
		//Negative stamps are a bad value
		assert(request >= 0);

		//create vector for memoization
		std::vector<int> memoizationVect;
		for (int i = 0; i <= request; i++)
		{
			//Initialize to -1 because we can never have negative stamps
			memoizationVect.push_back(-1);
		}

		return CalcNumStamps(request, memoizationVect);
	}

	/// <summary>
	/// Recursively checks all possible solutions for stamps and returns the 
	/// minimum value found.  Takes in a vector to avoid processing duplicates.
	/// </summary>
	/// <param name="request">
	/// The total value of the stamps to be dispensed.
	/// </param>
	/// <param name="memoizationVect">
	/// A vector of integers where size=request + 1.  Initial values should be 
	/// initialized to -1.
	/// </param>
	/// <returns>
	/// The minimum number of stamps needed to fill the given request.
	/// </returns>
	int CalcNumStamps(int request, std::vector<int>& memoizationVect)
	{
		if (memoizationVect[request] >= 0)
		{
			return memoizationVect[request];
		}

		if (0 == request)	//No stamps are needed if no postage is needed
			return 0;

		int min_val = INT_MAX;	//All values should be less than this

		for (std::vector<int>::iterator it = m_denominationVector.begin(); it != m_denominationVector.end(); ++it)
		{
			//if we can't use the stamp, then ignore it and move on
			if (request / (*it) > 0)
			{
				//if we can use the stamp, then recursively call this function to find min num of stamps for remainder
				min_val = std::min(min_val, (request / (*it)) +
					CalcNumStamps(request % (*it), memoizationVect));
			}
		}

		memoizationVect[request] = min_val;

		return min_val;
	}
};



int main()
{
	int stampDenominations[] = {90, 30, 24, 10, 6, 2, 1};
	StampDispenser stampDispenser(stampDenominations, 7);
	assert(stampDispenser.CalcNumStampsToFillRequest(18) == 3);

	//This test case fails for greedy algorithm
	int stampDenominations2[] = { 6, 4, 1 };
	StampDispenser stampDispenser2(stampDenominations2, 3);
	assert(stampDispenser2.CalcNumStampsToFillRequest(8) == 2);

	//Check when request is for zero stamps
	assert(stampDispenser2.CalcNumStampsToFillRequest(0) == 0);


	//Checking bad data (each of these should trigger an assert)

	//Check for negative stamps
//	assert(stampDispenser2.CalcNumStampsToFillRequest(-1) == 0);

	//Provide an array that's not in descending order
//	int stampDenominations3[] = { 1, 2, 3 };
//	StampDispenser stampDispenser3(stampDenominations3, 3);

	//Provide an array without a 1 in it
//	int stampDenominations4[] = { 4, 3, 2 };
//	StampDispenser stampDispenser4(stampDenominations4, 3);
	
	//Provide an array with a negative value
//	int stampDenominations5[] = { 4, 3, 2, 1, -1 };
//	StampDispenser stampDispenser5(stampDenominations5, 5);

    return 0;
}

