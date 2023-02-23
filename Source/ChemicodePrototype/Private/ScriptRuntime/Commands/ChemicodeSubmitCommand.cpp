// copyright lolol

#include "ScriptRuntime/Commands/ChemicodeSubmitCommand.h"

#include "ScriptRuntime/ChemicodeVM.h"
#include "ScriptRuntime/Variables/ChemicodeTestTubeVariable.h"

bool UChemicodeSubmitCommand::Execute(UChemicodeVM* VM)
{
	if (!CheckInputVariableNameIsValid(VM, "Container Name", UChemicodeTestTubeVariable::StaticClass()))
		return false;

	VM->Submission = Cast<UChemicodeTestTubeVariable>(VM->Variables[Arguments["Container Name"]])->Value;
	
	return true;
}
