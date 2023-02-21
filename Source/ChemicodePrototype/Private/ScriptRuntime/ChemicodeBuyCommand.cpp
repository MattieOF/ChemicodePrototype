// copyright lolol

#include "ScriptRuntime/ChemicodeBuyCommand.h"

#include "ChemicodeStatics.h"
#include "ScriptRuntime/ChemicodeResourceItemVariable.h"
#include "ScriptRuntime/ChemicodeVM.h"

bool UChemicodeBuyCommand::Execute(UChemicodeVM* VM)
{
	// Get resource and check it exists
	UResourceData* Resource = UChemicodeStatics::GetChemicodeGameInstance(GetWorld())->Resources[Arguments["Resource"]];
	if (!Resource)
	{
		VM->ThrowError(FString::Printf(TEXT("There is no resource called %ls!"), *Arguments["Resource"]), this);
		return false;
	}

	// Check variable name is set
	if (!Arguments.Contains("Variable Name") || Arguments["Variable Name"].IsEmpty())
	{
		VM->ThrowError("Variable name is empty!", this);
		return false;
	}
	
	// Create resource variable
	UChemicodeResourceItemVariable* ResourceVar = NewObject<UChemicodeResourceItemVariable>(VM);
	ResourceVar->InitialiseWithResource(Resource);
	VM->Variables.Add(Arguments["Variable Name"], ResourceVar);

	// Add run time
	VM->RunTime += 0.1f;
	
	return true;
}
