// copyright lolol

#include "ScriptRuntime/Commands/BunsenBurnerStateCommand.h"

#include "ScriptRuntime/ChemicodeVM.h"
#include "ScriptRuntime/Variables/ChemicodeBunsenBurnerVariable.h"

bool UBunsenBurnerStateCommand::Execute(UChemicodeVM* VM)
{
	if (!Arguments.Contains("State"))
	{
		VM->ThrowError("State is null!", this);
		return false;
	}

	if (!CheckInputVariableNameIsValid(VM, "Bunsen Burner", UChemicodeBunsenBurnerVariable::StaticClass()))
		return false;

	const FString& Value = Arguments["State"];
	if (!Value.IsNumeric())
	{
		VM->ThrowError("State is invalid!", this);
		return false;
	}

	const int ValueInt = FCString::Atoi(*Value);
	if (ValueInt < 0 || ValueInt > 2)
	{
		VM->ThrowError("State is invalid!", this);
		return false;
	}
	
	Cast<UChemicodeBunsenBurnerVariable>(VM->Variables[Arguments["Bunsen Burner"]])->Value->SetState(static_cast<EBunsenBurnerState>(ValueInt));

	VM->RunTime += 0.25;
	
	return true;
}
