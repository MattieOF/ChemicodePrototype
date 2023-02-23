// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ChemicodeVariable.generated.h"

/**
 * Object that represents a variable in a script. This is a base class; useful commands inherit from this.
 */
UCLASS(BlueprintType)
class CHEMICODEPROTOTYPE_API UChemicodeVariable : public UObject
{
	GENERATED_BODY()

public:
	virtual void SerialiseVariable(FArchive& Archive);
};
