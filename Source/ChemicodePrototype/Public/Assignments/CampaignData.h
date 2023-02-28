// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Assignment.h"
#include "Engine/DataAsset.h"
#include "CampaignData.generated.h"

USTRUCT(BlueprintType)
struct FCampaignDataItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAssignment* Assignment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int FollowingExtensions = 0;
};

/**
 * Data for a campaign
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UCampaignData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCampaignDataItem> Data;
};
