// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Assignment.generated.h"

/**
 * Object representing an assignment
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UAssignment : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	UFUNCTION(BlueprintCallable)
	virtual bool IsCompleted(UObject* Submission);
};
