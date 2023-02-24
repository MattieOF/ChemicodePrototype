// copyright lolol

#include "ScriptRuntime/Commands/ConnectBunsenBurnerCommand.h"

#include "ScriptRuntime/ChemicodeVM.h"
#include "ScriptRuntime/Variables/ChemicodeBunsenBurnerVariable.h"

bool UConnectBunsenBurnerCommand::Execute(UChemicodeVM* VM)
{
	// Check arguments exist
	if (!Arguments.Contains("Gas Tap"))
	{
		VM->ThrowError("Gas tap is null!", this);
		return false;
	}

	if (!CheckInputVariableNameIsValid(VM, "Bunsen Burner", UChemicodeBunsenBurnerVariable::StaticClass()))
		return false;

	// Check tap argument is valid
	const FString TapValue = Arguments["Gas Tap"];
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

	if (VM->GasTaps[TapValueInt]->IsConnected())
	{
		VM->ThrowError(FString::Printf(TEXT("Tap %i is already connected!"), TapValueInt), this);
		return false;
	}

	// Connect burner to tap
	auto Burner = Cast<UChemicodeBunsenBurnerVariable>(VM->Variables[Arguments["Bunsen Burner"]])->Value;
	Burner->ConnectToGasTap(VM->GasTaps[TapValueInt]);
	
	VM->RunTime += 1.5; // Add runtime
	
	return true;
}
