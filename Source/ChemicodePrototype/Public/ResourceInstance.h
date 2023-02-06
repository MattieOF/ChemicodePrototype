// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ResourceData.h"
#include "ResourceMeasurement.h"
#include "UObject/Object.h"
#include "ResourceInstance.generated.h"

/**
 * Instance of a resource
 */
UCLASS(BlueprintType)
class CHEMICODEPROTOTYPE_API UResourceInstance : public UObject
{
	GENERATED_BODY()

public:
	void SetResourceData(UResourceData* NewData, bool bOverwriteMeasurement = false);

public:
	/**
	 * @brief Data for this resource
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UResourceData* Data;

	/**
	 * @brief Measurement of this resource
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FResourceMeasurement Measurement;

	/**
	 * @brief State of this resource
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EResourceItemState> ResourceItemState;
};
