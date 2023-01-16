#pragma once

#include "CoreMinimal.h"
#include "ResourceMeasurement.generated.h"

UENUM()
enum EMeasurementUnit
{
	MUMillilitres UMETA(DisplayName = "Millilitres"),
	MULitres      UMETA(DisplayName = "Litres"),
	MUMilligrams  UMETA(DisplayName = "Milligrams"),
	MUGrams       UMETA(DisplayName = "Grams"),
	MUKilograms   UMETA(DisplayName = "Kilograms")
};

// Struct used to define an amount of a material, in volume or weight.
USTRUCT(BlueprintType)
struct FResourceMeasurement
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

	FResourceMeasurement()
	{
		Unit = MUMillilitres;
		Value = 0;
	}

	FResourceMeasurement(TEnumAsByte<EMeasurementUnit> NewUnit, float NewValue)
	{
		Unit = NewUnit;
		Value = NewValue;
	}

	bool operator==(const FResourceMeasurement Other) const;

	bool operator<(const FResourceMeasurement Other) const;

	bool operator>(const FResourceMeasurement Other) const;

	bool operator<=(const FResourceMeasurement Other) const;

	bool operator>=(const FResourceMeasurement Other) const;
};
