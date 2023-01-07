// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeGameMode.h"
#include "ChemicodePawn.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ChemicodeStatics.generated.h"

/**
 * Helper functions for interacting with the Chemicode systems
 */
UCLASS(ClassGroup=(Chemicode))
class CHEMICODEPROTOTYPE_API UChemicodeStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Actor References", BlueprintPure, meta=(WorldContext="World"))
	static FORCEINLINE AChemicodePawn* GetChemicodePawn(UObject* World) { return Cast<AChemicodePawn>(UGameplayStatics::GetPlayerPawn(World, 0)); };

	UFUNCTION(BlueprintCallable, Category = "Actor References", BlueprintPure, meta=(WorldContext="World"))
	static FORCEINLINE AChemicodeGameMode* GetChemicodeGameMode(UObject* World) { return Cast<AChemicodeGameMode>(UGameplayStatics::GetGameMode(World)); };

	UFUNCTION(BlueprintCallable, Category = "Math", BlueprintPure)
	static FVector2D ClampVector2D(FVector2D Vector, FVector2D Min, FVector2D Max);

	static FORCEINLINE void DebugErrorNotification(UObject* World, FString Title, FString Desc, float Time = 3)
	{
#if !UE_BUILD_SHIPPING
		GetChemicodeGameMode(World)->AddNotification(FNotification(FText::FromString(Title), FText::FromString(Desc), Time, NTError));
#endif
	}

	UFUNCTION()
	static bool GetHitResultAtCursor(const APlayerController* Controller, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, bool bTraceComplex, FHitResult& HitResult, const TArray<AActor*>& IgnoredActors);
};
