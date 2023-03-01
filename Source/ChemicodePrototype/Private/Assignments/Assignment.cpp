// copyright lolol

#include "Assignments/Assignment.h"

#include "ChemicodeStatics.h"

bool UAssignment::IsCompleted(UObject* Submission, FString& Message)
{
	return true;
}

bool UAssignment::ConditionsSatisfied(AResourceContainer* Container, TArray<FContainerCondition> Conditions,
	FString& Message)
{
	for (const auto& Condition : Conditions)
	{
		if (!Container->SatisfiesCondition(Condition))
		{
			// Check it has the resource
			if (!Container->HasResource(Condition.Resource))
			{
				Message = FString::Printf(TEXT("The submission needs to contain some %s!"), *Condition.Resource->Name.ToString());
				return false;
			}

			// Check proportion
			const float Proportion = Container->ResourceProportion(Condition.Resource);
			if (Proportion < Condition.MinProportion)
			{
				Message = FString::Printf(TEXT("There's too little %s in this submission!"), *Condition.Resource->Name.ToString());
				return false;
			} else if (Proportion > Condition.MaxProportion)
			{
				Message = FString::Printf(TEXT("There's too much %s in this submission!"), *Condition.Resource->Name.ToString());
				return false;
			}

			// Check minimum amount
			if (Container->GetTotalAmount() < Condition.MinimumAmount)
			{
				Message = FString::Printf(TEXT("Your submission is only %s total, but it needs to be at least %s!"),
					*UChemicodeStatics::MeasurementAsString(FResourceMeasurement(MUGrams, Container->GetTotalAmount()), true),
					*UChemicodeStatics::MeasurementAsString(FResourceMeasurement(MUGrams, Condition.MinimumAmount), true));
				return false;
			}

			Message = "Unspecified error! Sorry :(";
			
			return false;
		}
	}
	
	return true;
}

ACameraPlane* UAssignment::GetDefaultCamPlane(UWorld* World)
{
	return UChemicodeStatics::GetChemicodeGameMode(World)->GetTableCamPlane();
}
