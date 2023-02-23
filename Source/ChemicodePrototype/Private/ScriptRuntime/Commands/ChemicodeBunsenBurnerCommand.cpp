// copyright lolol

#include "ScriptRuntime/Commands/ChemicodeBunsenBurnerCommand.h"

#include "ScriptRuntime/ChemicodeVM.h"
#include "ScriptRuntime/Variables/ChemicodeBunsenBurnerVariable.h"

bool UChemicodeBunsenBurnerCommand::Execute(UChemicodeVM* VM)
{
	if (!CheckVariableNameIsValid(VM, "Variable Name"))
		return false;

	// Create test tube variable
	UChemicodeBunsenBurnerVariable* BunsenBurnerVar = NewObject<UChemicodeBunsenBurnerVariable>(VM);
	BunsenBurnerVar->InitialiseWithNew();
	VM->Variables.Add(Arguments["Variable Name"], BunsenBurnerVar);
	
	VM->RunTime += 2.5;
	return true;	
}
