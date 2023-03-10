// copyright lolol

#include "ScriptRuntime/ChemicodeVM.h"

#include "ChemicodeStatics.h"
#include "ChemicodePrototype/ChemicodePrototype.h"
#include "ScriptRuntime/ChemicodeScript.h"

FChemicodeScriptOutput UChemicodeVM::ExecuteScript(UChemicodeScript* Script)
{
	// Reset state
	CurrentScript = Script;
	CurrentCommand = 0;
	RunTime = 0;
	ClearError();
	Variables.Empty();
	GasTaps.Empty();

	// Start timer
	int Seconds;
	float PartialSeconds;
	UGameplayStatics::GetAccurateRealTime(Seconds, PartialSeconds);

	// Initialise VM
	for (int i = 0; i < VM_GAS_TAP_COUNT; i++)
		GasTaps.Add(NewObject<AGasTap>(this));

	// Iterate through commands
	for (UChemicodeCommand* Command : CurrentScript->Commands)
	{
		// Execute command
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
		
		// Tick variables
		const float DeltaTime = RunTime - PrevRunTime;
		for (const TTuple<FString, UChemicodeVariable*>& Variable : Variables)
		{
			Variable.Value->Tick(DeltaTime);
		}

		PrevRunTime = RunTime;
	}

	// End timer
	int EndSeconds;
	float EndPartialSeconds;
	UGameplayStatics::GetAccurateRealTime(EndSeconds, EndPartialSeconds);
	UE_LOG(LogChemicode, Log, TEXT("Script execution took %fs"), (EndPartialSeconds + EndSeconds) - (PartialSeconds + Seconds));
	
	// Return output
	FChemicodeScriptOutput Output;
	Output.Successful = true;
	Output.RunTime = RunTime;
	Output.Result = Submission;

	// Do a GC. If we don't, FPS continually rises
	UChemicodeStatics::ForceFullPurgeGC();
	
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
