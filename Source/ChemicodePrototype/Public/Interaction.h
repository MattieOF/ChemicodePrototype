#pragma once

#include "CoreMinimal.h"
#include "ResourceMeasurement.h"
#include "Interaction.generated.h"

USTRUCT(BlueprintType)
struct FInteraction
{
	GENERATED_BODY()

public:
	/**
	 * @brief Name of the interaction
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Interaction")
	FText InteractionName;

	/**
	 * @brief Description of the interaction, include some basic facts
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Interaction")
	FText InteractionDescription;

	/**
	 * @brief Cost of the measurement in terms of volume/weight of material
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Interaction")
	FResourceMeasurement Cost;

	/**
	 * @brief If the interaction was valid or not
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Interaction")
	bool bIsValid = false;

	/**
	 * @brief Create an empty, invalid interaction
	 */
	FInteraction()
	{
		InteractionName = FText::GetEmpty();
		InteractionDescription = FText::GetEmpty();
		Cost = FResourceMeasurement();
		bIsValid = false;
	}

	/**
	 * @brief Create a new interaction
	 */
	FInteraction(FText NewName, FText NewDesc, FResourceMeasurement NewCost)
	{
		InteractionName = NewName;
		InteractionDescription = NewDesc;
		Cost = NewCost;
		bIsValid = true;
	}
};
