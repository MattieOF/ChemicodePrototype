// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Assignment.h"
#include "CampaignData.h"
#include "UObject/Object.h"
#include "Campaign.generated.h"

UCLASS(BlueprintType)
class UCampaignAssignment : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAssignment* ThisAssignment;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCampaignAssignment*> Extensions;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCampaignAssignment* Next;

	UPROPERTY(BlueprintReadWrite)
	bool bCompleted = false;
};

/**
 * Class representing a campaign
 */
UCLASS(BlueprintType)
class CHEMICODEPROTOTYPE_API UCampaign : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UCampaignAssignment* RootAssignment;

	UFUNCTION(BlueprintCallable)
	void LoadAssignments(UCampaignData* Data);

	UFUNCTION(BlueprintCallable)
	void SaveProgress(FString Filename = "Progress.ccsav");
	
	UFUNCTION(BlueprintCallable)
	void LoadProgress(FString Filename = "Progress.ccsav");
};
