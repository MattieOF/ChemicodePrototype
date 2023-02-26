// copyright lolol

#include "ScriptRuntime/Variables/ChemicodeBunsenBurnerVariable.h"

#include "ChemicodeGameInstance.h"
#include "ChemicodeStatics.h"
#include "ChemicodePrototype/ChemicodePrototype.h"

void UChemicodeBunsenBurnerVariable::SerialiseVariable(FArchive& Archive)
{
	if (Archive.IsSaving())
	{
		FString Type = "BunsenBurner";
		Archive << Type;
	}
	
	if (Archive.IsLoading())
		InitialiseWithNew();
}

void UChemicodeBunsenBurnerVariable::Tick(float DeltaTime)
{
	UE_LOG(LogChemicode, Log, TEXT("%s"), *Value->GetClass()->GetName());
	Value->Tick(DeltaTime);
}

void UChemicodeBunsenBurnerVariable::InitialiseWithNew()
{
	const UChemicodeGameInstance* GameInstance = UChemicodeStatics::GetChemicodeGameInstance(GetWorld());
	check(GameInstance->BunsenBurnerClass != nullptr);
	Value = NewObject<ABunsenBurner>(GetWorld()->GetCurrentLevel(), GameInstance->BunsenBurnerClass);
	Value->bSimulated = true;
	Value->WorldRef = GetWorld();
	if (Value->GetWorld())
		UE_LOG(LogChemicode, Log, TEXT("World is not null :)"));
}
