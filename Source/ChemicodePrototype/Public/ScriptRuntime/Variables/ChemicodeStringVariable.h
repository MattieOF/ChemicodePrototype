// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeVariable.h"
#include "UObject/Object.h"
#include "ChemicodeStringVariable.generated.h"

/**
 * Variable representing a String.
 */
UCLASS(BlueprintType)
class CHEMICODEPROTOTYPE_API UChemicodeStringVariable : public UChemicodeVariable
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FString Value;

	virtual void SerialiseVariable(FArchive& Archive) override;
};
