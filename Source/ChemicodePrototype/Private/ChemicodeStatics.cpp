// copyright lolol

#include "ChemicodeStatics.h"

FVector2D UChemicodeStatics::ClampVector2D(FVector2D Vector, FVector2D Min, FVector2D Max)
{
	return FVector2D(FMath::Clamp(Vector.X, Min.X, Max.X), FMath::Clamp(Vector.Y, Min.Y, Max.Y));
}

bool UChemicodeStatics::GetHitResultAtCursor(const APlayerController* Controller,
                                             const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, bool bTraceComplex, FHitResult& HitResult,
                                             const TArray<AActor*>& IgnoredActors)
{
	float MouseX, MouseY;
	if (!Controller->GetMousePosition(MouseX, MouseY))
		return false;

	FVector WorldOrigin;
	FVector WorldDirection;
	if (UGameplayStatics::DeprojectScreenToWorld(Controller, FVector2D(MouseX, MouseY), WorldOrigin, WorldDirection) == true)
	{
		FCollisionObjectQueryParams const ObjParam(ObjectTypes);
		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(ClickableTrace));
		QueryParams.bTraceComplex = bTraceComplex;
		QueryParams.AddIgnoredActors(IgnoredActors);
		return Controller->GetWorld()->LineTraceSingleByObjectType(HitResult, WorldOrigin, WorldOrigin + WorldDirection * Controller->HitResultTraceDistance, ObjParam, QueryParams);
	}

	return false;
}

FString UChemicodeStatics::MeasurementAsString(FResourceMeasurement Measurement, bool bShorthand)
{
	switch (Measurement.Unit)
	{
		// Ternary hell incoming, sorry
		case MUMilligrams:
			return FString::Printf(TEXT("%f%s"), Measurement.Value, bShorthand ? TEXT("mg") : (Measurement.Value == 1 ? TEXT(" Milligram") : TEXT(" Milligrams")));
		case MUGrams:
			return FString::Printf(TEXT("%f%s"), Measurement.Value, bShorthand ? TEXT("g") : (Measurement.Value == 1 ? TEXT(" Gram") : TEXT(" Grams")));
		case MUKilograms:
			return FString::Printf(TEXT("%f%s"), Measurement.Value, bShorthand ? TEXT("kg") : (Measurement.Value == 1 ? TEXT(" Kilogram") : TEXT(" Kilograms")));
		case MUMillilitres:
			return FString::Printf(TEXT("%f%s"), Measurement.Value, bShorthand ? TEXT("mL") : (Measurement.Value == 1 ? TEXT(" Millilitre") : TEXT(" Millilitres")));
		case MULitres:
			return FString::Printf(TEXT("%f%s"), Measurement.Value, bShorthand ? TEXT("L") : (Measurement.Value == 1 ? TEXT(" Litre") : TEXT(" Litres")));
		default:
			return "Invalid";
	}
}
