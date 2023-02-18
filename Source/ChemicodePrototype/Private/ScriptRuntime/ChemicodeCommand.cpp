// copyright lolol

#include "ScriptRuntime/ChemicodeCommand.h"
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
