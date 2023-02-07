// copyright lolol

#include "ChemicodeStatics.h"

#include "ChemicodePawn.h"
#include "EngineUtils.h"
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
	FString UnitString;
	float Value;
	
	if (Measurement.Value < 1000)
	{
		Value = Measurement.Value;
		switch (Measurement.Unit)
		{
			case MUGrams:
				UnitString = bShorthand ? "mg" : (Value == 1 ? " milligram" : " milligrams");
				break;
			case MULitres:
				UnitString = bShorthand ? "μL" : (Value == 1 ? " microlitre" : " microlitres");
				break;
			default:
				UnitString = "INVALID";
				break;
		}
	} else if (Measurement.Value < 1000000)
	{
		Value = static_cast<float>(Measurement.Value) / 1000;
		switch (Measurement.Unit)
		{
			case MUGrams:
				UnitString = bShorthand ? "g" : (Value == 1 ? " gram" : " grams");
				break;
			case MULitres:
				UnitString = bShorthand ? "mL" : (Value == 1 ? " millilitre" : " millilitres");
				break;
			default:
				UnitString = "INVALID";
				break;
		}
	} else
	{
		Value = static_cast<float>(Measurement.Value) / 1000000;
		switch (Measurement.Unit)
		{
			case MUGrams:
				UnitString = bShorthand ? "kg" : (Value == 1 ? " kilogram" : " kilograms");
				break;
			case MULitres:
				UnitString = bShorthand ? "L" : (Value == 1 ? " litre" : " litres");
				break;
			default:
				UnitString = "INVALID";
				break;
		}
	}

	FString ValueString = FString::Printf(TEXT("%.2f"), Value);
	TrimTrailingZeros(ValueString);
	return FString::Printf(TEXT("%ls%ls"), *ValueString, *UnitString);
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

float UChemicodeStatics::GetZUnderOrigin(AActor* Object)
{
	FVector Origin, Extents;
	Object->GetActorBounds(true, Origin, Extents);
	const FVector ObjectOrigin = Object->GetActorLocation();
	const FVector Difference = Origin - ObjectOrigin;
	return Extents.Z - Difference.Z;
}
