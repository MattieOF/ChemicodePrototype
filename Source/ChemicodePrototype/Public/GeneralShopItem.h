// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeObject.h"
#include "GeneralShopItem.generated.h"

UCLASS()
class CHEMICODEPROTOTYPE_API AGeneralShopItem : public AChemicodeObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGeneralShopItem();

	virtual bool Interact() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AChemicodeObject> ObjectClass;
};
