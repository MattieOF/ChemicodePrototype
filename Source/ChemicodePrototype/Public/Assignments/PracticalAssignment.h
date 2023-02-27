// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Assignment.h"
#include "ContainerCondition.h"
#include "PracticalAssignment.generated.h"

/**
 * Assignment for making something practically
 */
UCLASS(BlueprintType)
class CHEMICODEPROTOTYPE_API UPracticalAssignment : public UAssignment
{
	GENERATED_BODY()

public:
	virtual bool IsCompleted(UObject* Submission, FString& Message) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FContainerCondition> SubmissionConditions;
};
