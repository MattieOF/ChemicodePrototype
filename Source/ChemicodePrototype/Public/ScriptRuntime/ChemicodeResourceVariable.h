// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeVariable.h"
#include "ResourceData.h"
#include "UObject/Object.h"
#include "ChemicodeResourceVariable.generated.h"

/**
 * Variable representing some resource data.
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UChemicodeResourceVariable : public UChemicodeVariable
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UResourceData* Value;
	
	virtual void SerialiseVariable(FArchive& Archive) override;
};
