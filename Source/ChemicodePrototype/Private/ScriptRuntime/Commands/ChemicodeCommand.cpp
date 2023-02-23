// copyright lolol

#include "ScriptRuntime/Commands/ChemicodeCommand.h"
#include "ScriptRuntime/ChemicodeVM.h"
#include "ChemicodePrototype/ChemicodePrototype.h"

bool UChemicodeCommand::Execute(UChemicodeVM* VM)
{
	UE_LOG(LogChemicode, Error, TEXT("Base chemicode command executed!"));
	return false;
}

void UChemicodeCommand::SerialiseCommand(FArchive& Archive)
{
	if (Archive.IsSaving())
		Archive << CommandName;

	Archive << Arguments;
}

bool UChemicodeCommand::CheckVariableNameIsValid(UChemicodeVM* VM, FString Key)
{
	// Check it exists and is not empty
	if (!Arguments.Contains(Key) || Arguments[Key].IsEmpty())
	{
		VM->ThrowError("Variable name is empty!", this);
		return false;
	}

	// Check VM doesn't already have variable with that name
	// TODO: Maybe throw a warning instead
	if (VM->Variables.Contains(Arguments[Key]))
	{
		VM->ThrowError(FString::Printf(TEXT("Variable with name %ls already exists!"), *Arguments[Key]), this);
		return false;
	}

	return true;
}

TArray<FChemicodeCommandFormatToken> UChemicodeCommand::TokeniseFormat(FString FormatString)
{
	TArray<FChemicodeCommandFormatToken> Output;
	FString TextBuffer = "";
	for (const char CurrentChar : FormatString)
	{
		if (CurrentChar == '{')
		{
			if (!TextBuffer.IsEmpty())
				Output.Add(FChemicodeCommandFormatToken(TextBuffer));
			TextBuffer = "";
		} else if (CurrentChar == '}')
		{
			FString Left, Right;
			TextBuffer.Split(":", &Left, &Right);
			Output.Add(FChemicodeCommandFormatToken(Left, Right));
			TextBuffer = "";
		} else {
			TextBuffer += CurrentChar;
		}
	}
	Output.Add(FChemicodeCommandFormatToken(TextBuffer));
	return Output;
}
