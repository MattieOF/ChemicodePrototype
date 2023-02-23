// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Commands/ChemicodeCommand.h"
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
	UPROPERTY(BlueprintReadWrite)
	FString Name;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<UChemicodeCommand*> Commands;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AddCommand(UChemicodeCommand* Command) { Commands.Add(Command); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void RemoveCommand(UChemicodeCommand* Command) { Commands.Remove(Command); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void RemoveAllCommands() { Commands.Empty(); }

	void SerialiseScript(FArchive& Ar);
};
