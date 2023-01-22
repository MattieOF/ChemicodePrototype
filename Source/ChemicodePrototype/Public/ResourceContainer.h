// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ResourceData.h"
#include "ResourceItem.h"
#include "ResourceMeasurement.h"
#include "ResourceContainer.generated.h"

USTRUCT(BlueprintType)
struct FContainerInteractionCondition
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

USTRUCT(BlueprintType)
struct FContainerInteraction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FContainerInteractionCondition> Conditions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName FunctionName;
};

UCLASS()
class CHEMICODEPROTOTYPE_API AResourceContainer : public AChemicodeObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ContainerName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ContainerDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<UResourceData*, FResourceMeasurement> Contents;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetTotalAmount();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool HasResource(UResourceData* Res) { return Contents.Contains(Res); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float ResourceProportion(UResourceData* Res);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool HasAmountOfResource(UResourceData* Res, const FResourceMeasurement Amount) { return HasResource(Res) && Contents[Res] == Amount; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool HasAtLeastAmountOfResource(UResourceData* Res, const FResourceMeasurement Amount) { return HasResource(Res) && Contents[Res] >= Amount; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool HasAtMostAmountOfResource(UResourceData* Res, const FResourceMeasurement Amount) { return HasResource(Res) && Contents[Res] <= Amount; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool HasAmountOfResourceRange(UResourceData* Res, const FResourceMeasurement Min, const FResourceMeasurement Max) { return HasResource(Res) && Contents[Res] >= Min && Contents[Res] <= Max; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasProportionOfResource(UResourceData* Res, float Percentage);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasProportionOfResourceRange(UResourceData* Res, float PercentageMin, float PercentageMax);

	UFUNCTION(BlueprintCallable)
	bool AddResource(UResourceData* Res, FResourceMeasurement Amount);

	UFUNCTION(BlueprintCallable)
	bool RemoveResource(UResourceData* Res, FResourceMeasurement Amount);
	
	UFUNCTION(BlueprintCallable)
	bool TransferToContainer(AResourceContainer* Target, UResourceData* Res, FResourceMeasurement Amount);
	
	UFUNCTION(BlueprintCallable)
	bool TransferFromItem(AResourceItem* Source, float Amount);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool SatisfiesCondition(FContainerInteractionCondition Condition) { return HasResource(Condition.Resource)
		&& GetTotalAmount() >= Condition.MinimumAmount
		&& HasProportionOfResourceRange(Condition.Resource, Condition.MinProportion, Condition.MaxProportion); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool SatisfiesInteraction(FContainerInteraction Interaction);

	UFUNCTION(BlueprintCallable)
	bool AttemptInteraction();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FContainerInteraction> Interactions;
	
private:
	float m_TotalAmount = 0;
	bool bDirty = false;
};
