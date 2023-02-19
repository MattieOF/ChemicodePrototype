// copyright lolol

#include "ScriptRuntime/ChemicodeBuyCommand.h"

#include "ChemicodeStatics.h"
#include "ScriptRuntime/ChemicodeResourceVariable.h"
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

	// Create resource variable
	UChemicodeResourceVariable* ResourceVar = NewObject<UChemicodeResourceVariable>(VM);
	ResourceVar->Value = Resource;
	VM->Variables.Add(Arguments["Variable Name"], ResourceVar);

	// Add run time
	VM->RunTime += 0.1f;
	
	return true;
}
