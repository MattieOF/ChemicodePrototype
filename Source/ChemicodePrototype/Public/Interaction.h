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
	 * @brief Amount of time taken by the animation for this interaction. 0 means no animation.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Interaction")
	float Time = 0;

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
		Time = 0;
		bIsValid = false;
	}

	/**
	 * @brief Create a new interaction
	 */
	FInteraction(FText NewName, FText NewDesc, FResourceMeasurement NewCost, float NewTime = 0)
	{
		InteractionName = NewName;
		InteractionDescription = NewDesc;
		Cost = NewCost;
		Time = NewTime;
		bIsValid = true;
	}
};
