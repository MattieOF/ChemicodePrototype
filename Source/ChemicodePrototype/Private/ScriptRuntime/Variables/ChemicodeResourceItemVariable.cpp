// copyright lolol

#include "ScriptRuntime/Variables/ChemicodeResourceItemVariable.h"

#include "ChemicodeStatics.h"
#include "ResourceInstance.h"

void UChemicodeResourceItemVariable::SerialiseVariable(FArchive& Archive)
{
	if (Archive.IsSaving())
	{
		FString Type = "ResourceItem";
		Archive << Type;
	}
	
	// If loading, initialise value
	// If saving, save resource name
	if (Archive.IsLoading())
	{
		FString ResourceName;
		Archive << ResourceName;
		InitialiseWithResource(UChemicodeStatics::GetChemicodeGameInstance(GetWorld())->Resources[ResourceName]);
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

void UChemicodeResourceItemVariable::Tick(float DeltaTime)
{
	Value->Tick(DeltaTime);
}

void UChemicodeResourceItemVariable::InitialiseWithResource(UResourceData* Resource)
{
	Value = NewObject<AResourceItem>(GetWorld()->GetCurrentLevel());
	Value->Initialise(GetWorld()); // Begin play isn't called when creating it like this
	Value->SetResource(Resource, false, false);
}
