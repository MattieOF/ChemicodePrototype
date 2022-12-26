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
UCLASS()
class CHEMICODEPROTOTYPE_API UChemicodeStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Actor References")
	static FORCEINLINE AChemicodePawn* GetChemicodePawn(UObject* World) { return Cast<AChemicodePawn>(UGameplayStatics::GetPlayerPawn(World, 0)); };

	UFUNCTION(BlueprintCallable, Category = "Actor References")
	static FORCEINLINE AChemicodeGameMode* GetChemicodeGameMode(UObject* World) { return Cast<AChemicodeGameMode>(UGameplayStatics::GetGameMode(World)); };
};
