// copyright lolol

#include "ScriptRuntime/ChemicodeScript.h"

#include "ChemicodePrototype/ChemicodePrototype.h"
#include "ScriptRuntime/ChemicodeBuyCommand.h"

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
			{
				NewCommand = NewObject<UChemicodeBuyCommand>(this);
			} else
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
