#pragma once

#include "CoreMinimal.h"
#include "ResourceMeasurement.generated.h"

UENUM()
enum EMeasurementUnit
{
	MULitres      UMETA(DisplayName = "Microlitres"),
	MUGrams       UMETA(DisplayName = "Milligrams"),
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
	int64 Value;

	FResourceMeasurement()
	{
		Unit = MULitres;
		Value = 0;
	}

	FResourceMeasurement(TEnumAsByte<EMeasurementUnit> NewUnit, int64 NewValue)
	{
		Unit = NewUnit;
		Value = NewValue;
	}

	bool operator==(const FResourceMeasurement& Other) const;

	bool operator<(const FResourceMeasurement& Other) const;

	bool operator>(const FResourceMeasurement& Other) const;

	bool operator<=(const FResourceMeasurement& Other) const;

	bool operator>=(const FResourceMeasurement& Other) const;
	
	FResourceMeasurement& operator+=(const FResourceMeasurement& Other);
	
	FResourceMeasurement& operator-=(const FResourceMeasurement& Other);
	
	FResourceMeasurement operator*(float Multiplier) const;
};
	
FArchive operator<<(FArchive& LHS, FResourceMeasurement& RHS);

FResourceMeasurement operator+(FResourceMeasurement Left, const FResourceMeasurement& Right);
	
FResourceMeasurement operator-(FResourceMeasurement Left, const FResourceMeasurement& Right);
