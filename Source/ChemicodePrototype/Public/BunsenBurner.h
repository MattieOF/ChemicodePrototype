// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeObject.h"
#include "CableComponent.h"
#include "BunsenBurner.generated.h"

/**
 * Bunsen burner object
 */
UCLASS()
class CHEMICODEPROTOTYPE_API ABunsenBurner : public AChemicodeObject
{
	GENERATED_BODY()

protected:
	UCableComponent* Cable;
};
