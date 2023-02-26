// copyright lolol


#include "ScriptRuntime/Commands/ClearBurnerCommand.h"

#include "ScriptRuntime/ChemicodeVM.h"
#include "ScriptRuntime/Variables/ChemicodeBunsenBurnerVariable.h"

bool UClearBurnerCommand::Execute(UChemicodeVM* VM)
{
	// Check burner argument is valid
	if (!CheckInputVariableNameIsValid(VM, "Bunsen Burner", UChemicodeBunsenBurnerVariable::StaticClass()))
		return false;

	// Execute
	ABunsenBurner* Burner = Cast<UChemicodeBunsenBurnerVariable>(VM->Variables[Arguments["Bunsen Burner"]])->Value;
	Burner->ClearItem();
	
	VM->RunTime += 1; // Add runtime

	return true;
}
