// copyright lolol

#include "ScriptRuntime/Variables/GasTubeVariable.h"

#include "ChemicodeStatics.h"

void UGasTubeVariable::SerialiseVariable(FArchive& Archive)
{
	if (Archive.IsSaving())
	{
		FString Type = "GasTube";
		Archive << Type;
	}
	
	if (Archive.IsLoading())
		InitialiseWithNew();
	
	// TODO: Serialise
}

void UGasTubeVariable::Tick(float DeltaTime)
{
	Value->Tick(DeltaTime);
}

void UGasTubeVariable::InitialiseWithNew()
{
	const UChemicodeGameInstance* GameInstance = UChemicodeStatics::GetChemicodeGameInstance(GetWorld());
	check(GameInstance->GasTubeClass != nullptr);
	Value = NewObject<AResourceTube>(GetWorld()->GetCurrentLevel(), GameInstance->GasTubeClass);
	Value->bSimulated = true;
	Value->WorldRef = GetWorld();
}
