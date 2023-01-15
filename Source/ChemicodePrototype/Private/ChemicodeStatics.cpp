// copyright lolol

#include "ChemicodeStatics.h"

#include "EngineUtils.h"
#include "Engine/PostProcessVolume.h"

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
	auto ValueAsString = FString::SanitizeFloat(Measurement.Value);
	
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
