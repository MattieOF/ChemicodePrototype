// copyright lolol

#include "ChemicodeStatics.h"

#include "ChemicodePawn.h"
#include "EngineUtils.h"
#include "ChemicodePrototype/ChemicodePrototype.h"
#include "Engine/PostProcessVolume.h"

AChemicodePawn* UChemicodeStatics::GetChemicodePawn(UObject* World)
{
	return Cast<AChemicodePawn>(UGameplayStatics::GetPlayerPawn(World, 0));
}

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

FInteraction UChemicodeStatics::GetInvalidInteraction()
{
	return FInteraction();
}

FString UChemicodeStatics::MeasurementAsString(FResourceMeasurement Measurement, bool bShorthand)
{
	auto ValueAsString = FString::Printf(TEXT("%.2f"), Measurement.Value);
	TrimTrailingZeros(ValueAsString);
	
	switch (Measurement.Unit)
	{
		// Ternary hell incoming, sorry
		case MUMilligrams:
			return FString::Printf(TEXT("%s%s"), *ValueAsString, bShorthand ? TEXT("mg") : (Measurement.Value == 1 ? TEXT(" Milligram") : TEXT(" Milligrams")));
		case MUGrams:
			return FString::Printf(TEXT("%s%s"), *ValueAsString, bShorthand ? TEXT("g") : (Measurement.Value == 1 ? TEXT(" Gram") : TEXT(" Grams")));
		case MUKilograms:
			return FString::Printf(TEXT("%s%s"), *ValueAsString, bShorthand ? TEXT("kg") : (Measurement.Value == 1 ? TEXT(" Kilogram") : TEXT(" Kilograms")));
		case MUMillilitres:
			return FString::Printf(TEXT("%s%s"), *ValueAsString, bShorthand ? TEXT("mL") : (Measurement.Value == 1 ? TEXT(" Millilitre") : TEXT(" Millilitres")));
		case MULitres:
			return FString::Printf(TEXT("%s%s"), *ValueAsString, bShorthand ? TEXT("L") : (Measurement.Value == 1 ? TEXT(" Litre") : TEXT(" Litres")));
		default:
			return "Invalid";
	}
}

AActor* UChemicodeStatics::GetFirstActorWithTag(UObject* WorldContext, FName Tag)
{
	// We do nothing if no tag is provided, rather than giving ALL actors!
	if (Tag.IsNone())
	{
		return nullptr;
	}
	
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull))
	{
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			if (Actor->ActorHasTag(Tag))
			{
				return Actor;
			}
		}
	}

	return nullptr;
}

void UChemicodeStatics::SetOutlinesEnabled(UObject* WorldContext, bool NewOutlinesEnabled, FName OutlineVolumeTag)
{
	if (const auto OutlineVolume = GetFirstActorWithTag(WorldContext, OutlineVolumeTag))
		Cast<APostProcessVolume>(OutlineVolume)->bEnabled = NewOutlinesEnabled;
}

float UChemicodeStatics::GetCurrentInteractionProgress(UObject* WorldContext)
{
	if (const auto Pawn = GetChemicodePawn(WorldContext))
	{
		const FTimerManager* TimerManager = &WorldContext->GetWorld()->GetTimerManager();
		if (TimerManager->TimerExists(Pawn->CurrentInteractionTimer))
		{
			return TimerManager->GetTimerElapsed(Pawn->CurrentInteractionTimer) / TimerManager->GetTimerRate(Pawn->CurrentInteractionTimer);
		} else return 0;
	} else return 0;
}

float UChemicodeStatics::MeasurementAsMinimumUnit(const FResourceMeasurement Measurement)
{
	if (Measurement.Unit == MULitres || Measurement.Unit == MUMillilitres) // Litres
	{
		switch (Measurement.Unit)
		{
		case MULitres:
			return Measurement.Value * 1000;
		case MUMillilitres:
			return Measurement.Value;
		default:
			return 0;
		}
	} else // Grams
	{
		switch (Measurement.Unit)
		{
		case MUKilograms:
			return Measurement.Value * 1000000;
		case MUGrams:
			return Measurement.Value * 1000;
		case MUMilligrams:
			return Measurement.Value;
		default:
			return 0;
		}
	}

	return 0;
}

bool UChemicodeStatics::MeasurementIsSameType(FResourceMeasurement A, FResourceMeasurement B)
{
	if (A.Unit == MUGrams || A.Unit == MUKilograms || A.Unit == MUMilligrams)
		return B.Unit == MUGrams || B.Unit == MUKilograms || B.Unit == MUMilligrams;
	else
		return B.Unit == MULitres || B.Unit == MUMillilitres;
}

void UChemicodeStatics::TrimTrailingZeros(FString& String)
{
	// Copy-pasted from FString::SanitizeFloat()
	// Trim all trailing zeros (up-to and including the decimal separator) from the fractional part of the number
	int32 TrimIndex = INDEX_NONE;
	int32 DecimalSeparatorIndex = INDEX_NONE;
	for (int32 CharIndex = String.Len() - 1; CharIndex >= 0; --CharIndex)
	{
		const TCHAR Char = String[CharIndex];
		if (Char == TEXT('.'))
		{
			DecimalSeparatorIndex = CharIndex;
			TrimIndex = FMath::Max(TrimIndex, DecimalSeparatorIndex);
			break;
		}
		if (TrimIndex == INDEX_NONE && Char != TEXT('0'))
		{
			TrimIndex = CharIndex + 1;
		}
	}
	check(TrimIndex != INDEX_NONE && DecimalSeparatorIndex != INDEX_NONE);
	String.RemoveAt(TrimIndex, String.Len() - TrimIndex, /*bAllowShrinking*/false);
}

float UChemicodeStatics::ConvertMeasurementType(float Value, EMeasurementUnit FromUnit, EMeasurementUnit ToUnit)
{
	switch (FromUnit)
	{
	case MUMillilitres:
		switch (ToUnit)
		{
			case MUMillilitres: return Value;
			case MULitres: return Value / 1000;
			default: return Value;
		}
	case MULitres: 
		switch (ToUnit)
		{
			case MUMillilitres: return Value * 1000;
			case MULitres: return Value;
			default: return Value;
		}
	case MUMilligrams: 
		switch (ToUnit)
		{
			case MUMilligrams: return Value;
			case MUGrams: return Value / 1000;
			case MUKilograms: return Value / 1000000;
			default: return Value;
		}
	case MUGrams: 
		switch (ToUnit)
		{
			case MUMilligrams: return Value * 1000;
			case MUGrams: return Value;
			case MUKilograms: return Value / 1000;
			default: return Value;
		}
	case MUKilograms: 
		switch (ToUnit)
		{
			case MUMilligrams: return Value * 1000000;
			case MUGrams: return Value * 1000;
			case MUKilograms: return Value;
			default: return Value;
		}
	default: return Value;
	}
}

EMeasurementUnit UChemicodeStatics::MinimumUnit(EMeasurementUnit Unit)
{
	switch (Unit)
	{
		case MULitres:
		case MUMillilitres: return MUMillilitres;
		case MUMilligrams: 
		case MUGrams: 
		case MUKilograms:   return MUMilligrams;
	}

	// TODO: Add none
	return MUGrams;
}

float UChemicodeStatics::MeasurementUnitDepositMultiplier(EMeasurementUnit Unit)
{
	switch (Unit)
	{
		case MUMillilitres: 
		case MULitres:       return 0.01f;
		case MUMilligrams: 
		case MUGrams: 
		case MUKilograms:    return 1;
	}

	return 0;
}

void UChemicodeStatics::UpdateMeasurementUnit(FResourceMeasurement& Measurement)
{
	switch (Measurement.Unit)
	{
	case MUMilligrams:
		if (Measurement.Value >= 1000)
		{
			Measurement.Value = ConvertMeasurementType(Measurement.Value, MUMilligrams, MUGrams);
			Measurement.Unit = MUGrams;
			UpdateMeasurementUnit(Measurement); // Update again if it's still too big
		}
	case MUGrams:
		if (Measurement.Value < 1)
		{
			Measurement.Value = ConvertMeasurementType(Measurement.Value, MUGrams, MUMilligrams);
			Measurement.Unit = MUMilligrams;
		} else if (Measurement.Value >= 1000)
		{
			Measurement.Value = ConvertMeasurementType(Measurement.Value, MUGrams, MUKilograms);
			Measurement.Unit = MUKilograms;
		}
		break;
	case MUKilograms:
		if (Measurement.Value < 1)
		{
			Measurement.Value = ConvertMeasurementType(Measurement.Value, MUKilograms, MUGrams);
			Measurement.Unit = MUGrams;
			UpdateMeasurementUnit(Measurement); // Update again if it's still below 1
		}
		break;
	case MUMillilitres:
		if (Measurement.Value >= 1000)
		{
			Measurement.Value = ConvertMeasurementType(Measurement.Value, MUMillilitres, MULitres);
			Measurement.Unit = MULitres;
		}
		break;
	case MULitres:
		if (Measurement.Value < 1)
		{
			Measurement.Value = ConvertMeasurementType(Measurement.Value, MULitres, MUMillilitres);
			Measurement.Unit = MUMillilitres;
		}
		break;
	default:
		break;
	}
}

float UChemicodeStatics::GetZUnderOrigin(AActor* Object)
{
	FVector Origin, Extents;
	Object->GetActorBounds(true, Origin, Extents);
	const FVector ObjectOrigin = Object->GetActorLocation();
	const FVector Difference = Origin - ObjectOrigin;
	return Extents.Z - Difference.Z;
}
