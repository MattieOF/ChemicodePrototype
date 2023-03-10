// copyright lolol

#include "Assignments/ScriptingAssignment.h"

#include "ChemicodeStatics.h"
#include "ScriptRuntime/ChemicodeVM.h"

bool UScriptingAssignment::IsCompleted(UObject* Submission, FString& Message)
{
	check(VM != nullptr);
	
	// Check submission is a script
	UChemicodeScript* Script = Cast<UChemicodeScript>(Submission);
	if (!Script)
	{
		Message = "Submission is not a script :(. This is probably not your fault.";
		return false;
	}

	// Run script
	FChemicodeScriptOutput Output = VM->ExecuteScript(Script);

	// Check script was successful
	if (!Output.Successful)
	{
		Message = FString::Printf(TEXT("Script failed to run! Error: %s"), *Output.ErrorMessage);
		return false;
	}

	// Check a result was submitted
	if (!Output.Result)
	{
		Message = "The script didn't set a submission! Use the \"Set Submission\" node to set a container as your submission.";
		return false;
	}

	return ConditionsSatisfied(Output.Result, SubmissionConditions, Message);
}

ACameraPlane* UScriptingAssignment::GetDefaultCamPlane(UWorld* World)
{
	return UChemicodeStatics::GetChemicodeGameMode(World)->GetComputerCamPlane();
}
