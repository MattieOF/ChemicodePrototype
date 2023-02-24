// copyright lolol

#include "ScriptRuntime/Commands/GasTapStatusCommand.h"

#include "ScriptRuntime/ChemicodeVM.h"

bool UGasTapStatusCommand::Execute(UChemicodeVM* VM)
{
	// Check arguments exist
	if (!Arguments.Contains("Tap"))
	{
		VM->ThrowError("Gas tap is null!", this);
		return false;
	}

	if (!Arguments.Contains("New Power"))
	{
		VM->ThrowError("New state is null!", this);
		return false;
	}

	// Check tap argument is valid
	const FString TapValue = Arguments["Tap"];
	if (!TapValue.IsNumeric())
	{
		VM->ThrowError(FString::Printf(TEXT("Tap argument should be numeric, but it's \"%s\"!"), *TapValue), this);
		return false;
	}
	int TapValueInt = FCString::Atoi(*TapValue);
	TapValueInt--;
	if (TapValueInt < 0 || TapValueInt >= VM_GAS_TAP_COUNT)
	{
		VM->ThrowError(FString::Printf(TEXT("Tap argument is out of range! It should be within 0 and %i, but it's %i!"), VM_GAS_TAP_COUNT, TapValueInt), this);
		return false;
	}

	// Check new power argument is valid
	const FString NewPowerValue = Arguments["New Power"]; 
	if (NewPowerValue != "True" && NewPowerValue != "False")
	{
		VM->ThrowError(FString::Printf(TEXT("New power value is invalid. It's \"%s\", but should be True or False."), *NewPowerValue), this);
		return false;
	}
	bool NewPower = NewPowerValue == "True";

	// Perform operation
	if (NewPower)
		VM->GasTaps[TapValueInt]->Open();
	else
		VM->GasTaps[TapValueInt]->Close();

	// Add runtime
	VM->RunTime += 1;
	
	return true;
}
