// copyright lolol

#include "ScriptRuntime/Variables/ChemicodeBunsenBurnerVariable.h"

#include "ChemicodeGameInstance.h"
#include "ChemicodeStatics.h"

void UChemicodeBunsenBurnerVariable::SerialiseVariable(FArchive& Archive)
{
	if (Archive.IsLoading())
		InitialiseWithNew();
}

void UChemicodeBunsenBurnerVariable::Tick(float DeltaTime)
{
	Value->Tick(DeltaTime);
}

void UChemicodeBunsenBurnerVariable::InitialiseWithNew()
{
	const UChemicodeGameInstance* GameInstance = UChemicodeStatics::GetChemicodeGameInstance(GetWorld());
	check(GameInstance->BunsenBurnerClass != nullptr);
	Value = NewObject<ABunsenBurner>(this, GameInstance->BunsenBurnerClass);
	Value->WorldRef = GetWorld();
}
