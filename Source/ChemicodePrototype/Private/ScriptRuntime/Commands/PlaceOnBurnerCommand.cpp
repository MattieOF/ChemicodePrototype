// copyright lolol

#include "ScriptRuntime/Commands/PlaceOnBurnerCommand.h"

#include "ScriptRuntime/ChemicodeVM.h"
#include "ScriptRuntime/Variables/ChemicodeBunsenBurnerVariable.h"
#include "ScriptRuntime/Variables/ChemicodeResourceItemVariable.h"
#include "ScriptRuntime/Variables/ChemicodeTestTubeVariable.h"

bool UPlaceOnBurnerCommand::Execute(UChemicodeVM* VM)
{
	// Check item argument is valid
	TArray<TSubclassOf<UChemicodeVariable>> ValidTypes;
	ValidTypes.Add(UChemicodeResourceItemVariable::StaticClass());
	ValidTypes.Add(UChemicodeTestTubeVariable::StaticClass());
	if (!CheckInputVariableNameIsValidMultiType(VM, "Item", ValidTypes))
		return false;

	// Check burner argument is valid
	if (!CheckInputVariableNameIsValid(VM, "Bunsen Burner", UChemicodeBunsenBurnerVariable::StaticClass()))
		return false;

	// Get references out of variables
	ABunsenBurner* Burner = Cast<UChemicodeBunsenBurnerVariable>(VM->Variables[Arguments["Bunsen Burner"]])->Value;
	AChemicodeObject* Item = nullptr;
	if (const auto RI = Cast<UChemicodeResourceItemVariable>(VM->Variables[Arguments["Item"]]))
		Item = RI->Value;
	else if (const auto TT = Cast<UChemicodeTestTubeVariable>(VM->Variables[Arguments["Item"]]))
		Item = TT->Value;

	// Check burner doesn't already have an item
	if (Burner->HasItem())
	{
		VM->ThrowError(FString::Printf(TEXT("Burner %s already has an item! Take it off first."), *Arguments["Bunsen Burner"]), this);
		return false;
	}

	// Put the item on top
	Burner->InteractWith(Item);
	
	VM->RunTime += 1; // Add runtime
	
	return true;
}
