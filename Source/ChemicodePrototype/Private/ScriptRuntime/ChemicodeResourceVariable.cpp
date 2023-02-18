// copyright lolol

#include "ScriptRuntime/ChemicodeResourceVariable.h"

#include "ChemicodeStatics.h"
#include "ChemicodePrototype/ChemicodePrototype.h"

void UChemicodeResourceVariable::SerialiseVariable(FArchive& Archive)
{
	if (Archive.IsSaving())
	{
		FString Type = "ResourceRef";
		Archive << Type;
		if (Value)
		{
			Archive << Value->Name;
		} else
		{
			FString Null = "NULL";
			Archive << Null;
		}
	} else
	{
		FString Result;
		Archive << Result;
		if (Result == "NULL")
			Value = nullptr;
		else
		{
			auto GI = UChemicodeStatics::GetChemicodeGameInstance(GetWorld());
			if (GI->Resources.Contains(Result))
				Value = GI->Resources[Result];
			else
			{
				Value = nullptr;
				UE_LOG(LogChemicode, Error, TEXT("Found resource %s for a resource variable, but it doesn't exist in the resources array."), *Result);
			}
		}
	}
}
