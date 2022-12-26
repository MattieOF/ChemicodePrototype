// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodePawn.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
	static AChemicodePawn* GetChemicodePawn(UObject* World);

	UFUNCTION(BlueprintCallable, Category = "Actor References")
	static AChemicodeGameMode* GetChemicodeGameMode(UObject* World);
};
