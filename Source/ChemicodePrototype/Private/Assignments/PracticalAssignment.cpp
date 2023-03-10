// copyright lolol

#include "Assignments/PracticalAssignment.h"

#include "ChemicodeStatics.h"
#include "ResourceContainer.h"

bool UPracticalAssignment::IsCompleted(UObject* Submission, FString& Message)
{
	AResourceContainer* Container = Cast<AResourceContainer>(Submission);

	// Check submission is of the right type
	if (!Container)
	{
		Message = "Submission must be a resource container!";
		return false;
	}

	return ConditionsSatisfied(Container, SubmissionConditions, Message);
}

ACameraPlane* UPracticalAssignment::GetDefaultCamPlane(UWorld* World)
{
	return UChemicodeStatics::GetChemicodeGameMode(World)->GetTableCamPlane();
}
