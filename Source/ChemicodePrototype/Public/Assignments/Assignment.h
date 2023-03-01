// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "CameraPlane.h"
#include "ResourceContainer.h"
#include "UObject/Object.h"
#include "Assignment.generated.h"

/**
 * Object representing an assignment
 */
UCLASS(BlueprintType)
class CHEMICODEPROTOTYPE_API UAssignment : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(MultiLine=True))
	FText Description;

	UFUNCTION(BlueprintCallable)
	virtual bool IsCompleted(UObject* Submission, FString& Message);

	UFUNCTION(BlueprintCallable)
	static bool ConditionsSatisfied(AResourceContainer* Container, TArray<FContainerCondition> Conditions, FString& Message);

	UFUNCTION(BlueprintCallable)
	virtual ACameraPlane* GetDefaultCamPlane(UWorld* World);
};
