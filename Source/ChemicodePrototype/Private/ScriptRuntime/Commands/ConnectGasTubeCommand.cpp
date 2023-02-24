// copyright lolol

#include "ScriptRuntime/Commands/ConnectGasTubeCommand.h"

#include "ScriptRuntime/ChemicodeVM.h"
#include "ScriptRuntime/Variables/ChemicodeTestTubeVariable.h"
#include "ScriptRuntime/Variables/GasTubeVariable.h"

bool UConnectGasTubeCommand::Execute(UChemicodeVM* VM)
{
	// Verify arguments
	if (!CheckInputVariableNameIsValid(VM, "Gas Tube", UGasTubeVariable::StaticClass()))
		return false;
	if (!CheckInputVariableNameIsValid(VM, "Left Connection", UChemicodeTestTubeVariable::StaticClass()))
		return false;
	if (!CheckInputVariableNameIsValid(VM, "Right Connection", UChemicodeTestTubeVariable::StaticClass()))
		return false;

	// Connect
	AResourceTube* Tube = Cast<UGasTubeVariable>(VM->Variables["Gas Tube"])->Value;
	if (Tube->HasLHSConnection() || Tube->HasRHSConnection())
	{
		VM->ThrowError(FString::Printf(TEXT("Gas tube at %ls is already connected! Disconnect any objects first."), *Arguments["Gas Tube"]), this);
		return false;
	}

	// Check containers are valid
	AResourceContainer* LHS = Cast<UChemicodeTestTubeVariable>(VM->Variables["Left Connection"])->Value;
	AResourceContainer* RHS = Cast<UChemicodeTestTubeVariable>(VM->Variables["Right Connection"])->Value;
	if (LHS->IsConnectedToTube())
	{
		VM->ThrowError(FString::Printf(TEXT("Test tube at %ls is already connected to a gas tube!"), *Arguments["Left Connection"]), this);
		return false;
	}
	if (RHS->IsConnectedToTube())
	{
		VM->ThrowError(FString::Printf(TEXT("Test tube at %ls is already connected to a gas tube!"), *Arguments["Right Connection"]), this);
		return false;
	}

	// Do the connection
	LHS->ConnectTube(Tube);
	RHS->ConnectTube(Tube);
	
	VM->RunTime += 3; // Add runtime

	return true;
}
