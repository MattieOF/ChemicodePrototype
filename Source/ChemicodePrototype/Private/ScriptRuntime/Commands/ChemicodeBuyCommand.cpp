// copyright lolol

#include "ScriptRuntime/Commands/ChemicodeBuyCommand.h"

#include "ChemicodeStatics.h"
#include "ScriptRuntime/Variables/ChemicodeResourceItemVariable.h"
#include "ScriptRuntime/ChemicodeVM.h"

bool UChemicodeBuyCommand::Execute(UChemicodeVM* VM)
{
	if (!CheckVariableNameIsValid(VM, "Variable Name"))
		return false;
	
	// Get resource and check it exists
	UResourceData* Resource = UChemicodeStatics::GetChemicodeGameInstance(GetWorld())->Resources[Arguments["Resource"]];
	if (!Resource)
	{
		VM->ThrowError(FString::Printf(TEXT("There is no resource called %ls!"), *Arguments["Resource"]), this);
		return false;
	}

	// Create resource variable
	UChemicodeResourceItemVariable* ResourceVar = NewObject<UChemicodeResourceItemVariable>(VM);
	ResourceVar->InitialiseWithResource(Resource);
	VM->Variables.Add(Arguments["Variable Name"], ResourceVar);

	// Add run time
	VM->RunTime += 1.0f;
	
	return true;
}
