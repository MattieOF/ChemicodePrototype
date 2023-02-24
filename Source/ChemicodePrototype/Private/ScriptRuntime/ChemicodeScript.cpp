// copyright lolol

#include "ScriptRuntime/ChemicodeScript.h"

#include "ChemicodePrototype/ChemicodePrototype.h"

// Command includes
#include "ScriptRuntime/Commands/BunsenBurnerStateCommand.h"
#include "ScriptRuntime/Commands/ChemicodeBunsenBurnerCommand.h"
#include "ScriptRuntime/Commands/ChemicodeBuyCommand.h"
#include "ScriptRuntime/Commands/ChemicodeSubmitCommand.h"
#include "ScriptRuntime/Commands/ChemicodeTestTubeCommand.h"
#include "ScriptRuntime/Commands/ChemicodeTransferCommand.h"
#include "ScriptRuntime/Commands/GasTapStatusCommand.h"
#include "Serialization/BufferArchive.h"

void UChemicodeScript::SerialiseScript(FArchive& Archive)
{
	Archive << Name;
	int Count = Commands.Num();
	Archive << Count;

	// Clear state
	if (Archive.IsLoading())
		Commands.Empty();
	
	for (int i = 0; i < Count; i++)
	{
		if (Archive.IsSaving())
		{
			Commands[i]->SerialiseCommand(Archive);
		} else
		{
			// Create new command of correct type
			FString Type;
			Archive << Type;
			UChemicodeCommand* NewCommand;
			
			if (Type == "Buy")
				NewCommand = NewObject<UChemicodeBuyCommand>(this);
			else if (Type == "GetTestTube")
				NewCommand = NewObject<UChemicodeTestTubeCommand>(this);
			else if (Type == "SetSubmission")
				NewCommand = NewObject<UChemicodeSubmitCommand>(this);
			else if (Type == "Transfer")
				NewCommand = NewObject<UChemicodeTransferCommand>(this);
			else if (Type == "GetBunsenBurner")
				NewCommand = NewObject<UChemicodeBunsenBurnerCommand>(this);
			else if (Type == "SetBunsenBurnerState")
				NewCommand = NewObject<UBunsenBurnerStateCommand>(this);
			else if (Type == "GasTapStatus")
				NewCommand = NewObject<UGasTapStatusCommand>(this);
			else
			{
				UE_LOG(LogChemicode, Error, TEXT("Invalid command type %s in script %s!"), *Type, *Name);
				Archive.SetCriticalError();
				return;
			}

			// Load parameters and add to array
			NewCommand->SerialiseCommand(Archive);
			Commands.Add(NewCommand);
		}
	}
}

// Save functions from https://www.orfeasel.com/saving-and-loading-your-progress/
bool UChemicodeScript::SaveToFile(FString FileName)
{
	// Serialise with buffer archive
	FBufferArchive BinarySaveArchive;
	SerialiseScript(BinarySaveArchive);

	// Save to disk
	const FString Path = *FString::Printf(TEXT("%lsScripts/%ls"), *FPaths::ProjectSavedDir(), *FileName);
	const bool SaveResult = FFileHelper::SaveArrayToFile(BinarySaveArchive, *Path);
	if (!SaveResult)
	{
		UE_LOG(LogChemicode, Error, TEXT("Failed to save script %s to %s!"), *Name, *Path);
		return false;
	}
	
	// Empty buffer
	BinarySaveArchive.FlushCache();
	BinarySaveArchive.Empty();

	return true;
}

bool UChemicodeScript::LoadFromFile(FString FileName)
{
	TArray<uint8> BinaryArray;

	// Load and verify file
	const FString Path = *FString::Printf(TEXT("%lsScripts/%ls"), *FPaths::ProjectSavedDir(), *FileName);
	const bool LoadFileResult = FFileHelper::LoadFileToArray(BinaryArray, *Path);
	if (!LoadFileResult)
	{
		UE_LOG(LogChemicode, Error, TEXT("In Chemicode script loader, failed to open file %s!"), *Path);
		return false;
	}
	if (BinaryArray.Num() <= 0)
	{
		UE_LOG(LogChemicode, Error, TEXT("In Chemicode script loader, file %s is empty!"), *Path);
		return false;
	}

	// Create binary loader
	FMemoryReader BinaryLoader = FMemoryReader(BinaryArray, true);
	BinaryLoader.Seek(0);
	
	// Do the loading
	SerialiseScript(BinaryLoader);

	// Empty buffer
	BinaryLoader.FlushCache();
	BinaryArray.Empty();
	BinaryLoader.Close();

	return true;
}
