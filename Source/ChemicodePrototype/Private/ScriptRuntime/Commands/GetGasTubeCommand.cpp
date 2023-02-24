// copyright lolol

#include "ScriptRuntime/Commands/GetGasTubeCommand.h"

#include "ScriptRuntime/ChemicodeVM.h"
#include "ScriptRuntime/Variables/GasTubeVariable.h"

bool UGetGasTubeCommand::Execute(UChemicodeVM* VM)
{
	if (!CheckVariableNameIsValid(VM, "Variable Name"))
		return false;

	// Create test tube variable
	UGasTubeVariable* GasTubeVar = NewObject<UGasTubeVariable>(VM);
	GasTubeVar->InitialiseWithNew();
	VM->Variables.Add(Arguments["Variable Name"], GasTubeVar);
	
	VM->RunTime += 0.5;
	return true;
}
