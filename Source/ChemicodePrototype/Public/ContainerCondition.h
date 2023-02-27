#pragma once

#include "CoreMinimal.h"
#include "ContainerCondition.generated.h"

class UResourceData;

USTRUCT(BlueprintType)
struct FContainerCondition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UResourceData* Resource;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, ClampMax=1))
	float MinProportion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, ClampMax=1))
	float MaxProportion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0))
	int MinimumAmount;
};

UCLASS()
class UContainerConditionStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString ConditionToString(FContainerCondition Condition);	
};
