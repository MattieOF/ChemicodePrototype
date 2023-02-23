﻿// copyright lolol

#include "ScriptRuntime/ChemicodeScript.h"

#include "ChemicodePrototype/ChemicodePrototype.h"

// Command includes
#include "ScriptRuntime/Commands/BunsenBurnerStateCommand.h"
#include "ScriptRuntime/Commands/ChemicodeBunsenBurnerCommand.h"
#include "ScriptRuntime/Commands/ChemicodeBuyCommand.h"
#include "ScriptRuntime/Commands/ChemicodeSubmitCommand.h"
#include "ScriptRuntime/Commands/ChemicodeTestTubeCommand.h"
#include "ScriptRuntime/Commands/ChemicodeTransferCommand.h"

void UChemicodeScript::SerialiseScript(FArchive& Archive)
{
	Archive << Name;
	int Count = Commands.Num();
	Archive << Count;
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
