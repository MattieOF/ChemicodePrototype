#pragma once

#include "CoreMinimal.h"
#include "Util/ColorConstants.h"
#include "Interaction.generated.h"

UENUM()
enum EMeasurementUnit
{
	MUMillilitres UMETA(DisplayName = "Millilitres"),
	MULitres      UMETA(DisplayName = "Litres"),
	MUMilligrams  UMETA(DisplayName = "Milligrams"),
	MUGrams       UMETA(DisplayName = "Grams"),
	MUKilograms   UMETA(DisplayName = "Kilograms")
};

// Struct used to define the cost of an interaction, in volume or weight of material.
USTRUCT(BlueprintType)
struct FCost
{
	GENERATED_BODY()

	/**
	 * @brief Unit of the cost
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Cost")
	TEnumAsByte<EMeasurementUnit> Unit;

	/**
	 * @brief Value of the cost
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Interaction")
	float Value;

	FCost()
	{
		Unit = MUMillilitres;
		Value = 0;
	}

	FCost(TEnumAsByte<EMeasurementUnit> NewUnit, float NewValue)
	{
		Unit = NewUnit;
		Value = NewValue;
	}
};

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
	FCost Cost;

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
		Cost = FCost();
		bIsValid = false;
	}

	/**
	 * @brief Create a new interaction
	 */
	FInteraction(FText NewName, FText NewDesc, FCost NewCost)
	{
		InteractionName = NewName;
		InteractionDescription = NewDesc;
		Cost = NewCost;
		bIsValid = true;
	}
};
