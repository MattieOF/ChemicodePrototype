// copyright lolol

#include "ScriptRuntime/Commands/DisconnectGasTubeCommand.h"

#include "ScriptRuntime/ChemicodeVM.h"
#include "ScriptRuntime/Variables/GasTubeVariable.h"

bool UDisconnectGasTubeCommand::Execute(UChemicodeVM* VM)
{
	// Check arguments are valid
	if (!CheckInputVariableNameIsValid(VM, "Gas Tube", UGasTubeVariable::StaticClass()))
		return false;

	// Disconnect tube
	AResourceTube* Tube = Cast<UGasTubeVariable>(VM->Variables["Gas Tube"])->Value;
	Tube->DisconnectObjects();
	Tube->OnItemPickedUp.Broadcast(); // Broadcast on picked up to hack in callbacks to connected objects
	
	VM->RunTime += 1; // Add runtime
	
	return true;
}
