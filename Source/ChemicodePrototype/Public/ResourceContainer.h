// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ResourceData.h"
#include "ResourceItem.h"
#include "ResourceMeasurement.h"
#include "ResourceContainer.generated.h"

UCLASS()
class CHEMICODEPROTOTYPE_API AResourceContainer : public AResourceItem
{
	GENERATED_BODY()

public:
	TMap<UResourceData*, FResourceMeasurement> Contents;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetTotalAmount();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool HasResource(UResourceData* Res) { return Contents.Contains(Res); }

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
	bool Transfer(AResourceContainer* Target, UResourceData* Res, FResourceMeasurement Amount);
};
