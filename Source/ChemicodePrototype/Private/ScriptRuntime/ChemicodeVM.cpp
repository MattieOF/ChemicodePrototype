// copyright lolol

#include "ScriptRuntime/ChemicodeVM.h"
#include "ScriptRuntime/ChemicodeScript.h"

FChemicodeScriptOutput UChemicodeVM::ExecuteScript(UChemicodeScript* Script)
{
	// Reset state
	CurrentScript = Script;
	CurrentCommand = 0;
	RunTime = 0;
	ClearError();
	Variables.Empty();

	// Iterate through commands
	for (UChemicodeCommand* Command : CurrentScript->Commands)
	{
		Command->Execute(this);
		if (HasError)
		{
			// There's been an error, fail and return
			FChemicodeScriptOutput Output;
			Output.Successful = false;
			Output.ErrorMessage = ErrorMessage;
			Output.ErrorSource = ErrorSource;
			Output.RunTime = RunTime;
			return Output;
		}
		CurrentCommand++;
	}

	// Return output
	FChemicodeScriptOutput Output;
	Output.Successful = true;
	Output.RunTime = RunTime;
	// TODO: Output.Result =
	return Output;
}

void UChemicodeVM::ThrowError(FString Message, UChemicodeCommand* Source)
{
	HasError = true;
	ErrorMessage = Message;
	ErrorSource = Source;
}

void UChemicodeVM::ClearError()
{
	HasError = false;
	ErrorMessage = "Successful";
	ErrorSource = nullptr;
}
