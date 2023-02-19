// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeCommand.h"
#include "UObject/Object.h"
#include "ChemicodeScript.generated.h"

/**
 * Object that holds the data for a script.
 */
UCLASS(BlueprintType)
class CHEMICODEPROTOTYPE_API UChemicodeScript : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Name;
	
	UPROPERTY()
	TArray<UChemicodeCommand*> Commands;

	UFUNCTION()
	FORCEINLINE void AddCommand(UChemicodeCommand* Command);
	
	UFUNCTION()
	FORCEINLINE void RemoveCommand(UChemicodeCommand* Command);

	UFUNCTION()
	FORCEINLINE void RemoveAllCommands();

	void SerialiseScript(FArchive& Ar);
};
