// copyright lolol

#include "ScriptRuntime/Variables/ChemicodeStringVariable.h"

void UChemicodeStringVariable::SerialiseVariable(FArchive& Archive)
{
	if (Archive.IsSaving())
	{
		FString Type = "String";
		Archive << Type;	
	}
	Archive << Value;
}
