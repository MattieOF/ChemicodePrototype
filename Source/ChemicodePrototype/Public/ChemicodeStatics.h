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
	UFUNCTION(BlueprintCallable, Category = "Actor References", BlueprintPure)
	static FORCEINLINE AChemicodePawn* GetChemicodePawn(UObject* World) { return Cast<AChemicodePawn>(UGameplayStatics::GetPlayerPawn(World, 0)); };

	UFUNCTION(BlueprintCallable, Category = "Actor References", BlueprintPure)
	static FORCEINLINE AChemicodeGameMode* GetChemicodeGameMode(UObject* World) { return Cast<AChemicodeGameMode>(UGameplayStatics::GetGameMode(World)); };

	UFUNCTION(BlueprintCallable, Category = "Math", BlueprintPure)
	static FVector2D ClampVector2D(FVector2D Vector, FVector2D Min, FVector2D Max);
};
