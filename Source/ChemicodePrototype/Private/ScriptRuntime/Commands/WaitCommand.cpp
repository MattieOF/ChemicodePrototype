// copyright lolol

#include "ScriptRuntime/Commands/WaitCommand.h"

#include "ScriptRuntime/ChemicodeVM.h"

bool UWaitCommand::Execute(UChemicodeVM* VM)
{
	// Check time exists
	if (!Arguments.Contains("Time") || Arguments["Time"].IsEmpty())
	{
		VM->ThrowError("Time parameter is empty!", this);
		return false;
	}

	// Check time parameter is valid
	if (!Arguments["Time"].IsNumeric())
	{
		VM->ThrowError(FString::Printf(TEXT("Time parameter must be numeric, but it's \"%s\""), *Arguments["Time"]), this);
		return false;
	}

	const float Time = FCString::Atof(*Arguments["Time"]);

	// Check time value is valid
	if (Time < 0)
	{
		VM->ThrowError(FString::Printf(TEXT("Time parameter must be positive, but it's %f"), Time), this);
		return false;
	}
	
	VM->RunTime += Time; // Add runtime
	
	return true;
}
