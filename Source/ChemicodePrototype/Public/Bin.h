// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeObject.h"
#include "Bin.generated.h"

UCLASS()
class CHEMICODEPROTOTYPE_API ABin : public AChemicodeObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABin();

	virtual bool InteractWith(AChemicodeObject* OtherObject) override;
};
