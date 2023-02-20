// copyright lolol

#include "ScriptRuntime/ChemicodeResourceItemVariable.h"

#include "ChemicodeStatics.h"
#include "ResourceInstance.h"

void UChemicodeResourceItemVariable::SerialiseVariable(FArchive& Archive)
{
	// If loading, initialise value
	// If saving, save resource name
	if (Archive.IsLoading())
	{
		FString ResourceName;
		Archive << ResourceName;
		Value = NewObject<AResourceItem>(this);
		Value->SetResource(UChemicodeStatics::GetChemicodeGameInstance(GetWorld())->Resources[ResourceName], false, false);
	} else
	{
		Archive << Value->Resource->Data->Name;
	}

	// Serialise resource measurement
	Archive << Value->Resource->Measurement.Unit;
	Archive << Value->Resource->Measurement.Value;

	// TODO: Serialise resource properties
	return;
	if (Archive.IsSaving())
	{
		int Num = Value->Resource->Properties.Num();
		Archive << Num;
		for (int i = 0; i < Num; i++)
			Archive << Value->Resource->Properties[i];
	}
}
