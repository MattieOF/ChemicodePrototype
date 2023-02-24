// copyright lolol


#include "ScriptRuntime/Variables/ChemicodeTestTubeVariable.h"

#include "ChemicodeStatics.h"

void UChemicodeTestTubeVariable::SerialiseVariable(FArchive& Archive)
{
	if (Archive.IsSaving())
	{
		FString Type = "TestTube";
		Archive << Type;
	}
	
	if (Archive.IsLoading())
		InitialiseWithNew();

	int ContentsCount = 0;
	Archive << ContentsCount;
	
	if (Archive.IsSaving())
	{
		for (auto Item : Value->Contents)
		{
			Archive << Item->Data->Name;
			Archive << Item->Measurement;
		}
	} else
	{
		UChemicodeGameInstance* GameInstance = UChemicodeStatics::GetChemicodeGameInstance(GetWorld());
		for (int i = 0; i < ContentsCount; i++)
		{
			FString Name;
			Archive << Name;
			
			if (!GameInstance->Resources.Contains(Name))
			{
				Archive.SetCriticalError();
				return;
			}
			
			FResourceMeasurement Measurement;
			Archive << Measurement;

			Value->AddResource(GameInstance->Resources[Name], Measurement);
		}
	}
}

void UChemicodeTestTubeVariable::Tick(float DeltaTime)
{
	Value->Tick(DeltaTime);
}

void UChemicodeTestTubeVariable::InitialiseWithNew()
{
	const UChemicodeGameInstance* GameInstance = UChemicodeStatics::GetChemicodeGameInstance(GetWorld());
	check(GameInstance->TestTubeClass != nullptr);
	Value = NewObject<AResourceContainer>(this, GameInstance->TestTubeClass);
	Value->WorldRef = GetWorld();
	Value->Initialise();
}
