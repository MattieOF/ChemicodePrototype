// copyright lolol

#include "ScriptRuntime/Commands/ChemicodeTestTubeCommand.h"

#include "ScriptRuntime/ChemicodeVM.h"
#include "ScriptRuntime/Variables/ChemicodeTestTubeVariable.h"

bool UChemicodeTestTubeCommand::Execute(UChemicodeVM* VM)
{
	if (!CheckVariableNameIsValid(VM, "Variable Name"))
		return false;

	// Create test tube variable
	UChemicodeTestTubeVariable* TestTubeVar = NewObject<UChemicodeTestTubeVariable>(VM);
	TestTubeVar->InitialiseWithNew();
	VM->Variables.Add(Arguments["Variable Name"], TestTubeVar);
	
	VM->RunTime += 0.5;
	return true;
}
