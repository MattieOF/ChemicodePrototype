// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "BunsenBurner.h"
#include "Engine/GameInstance.h"
#include "ChemicodeGameInstance.generated.h"

class AResourceContainer;
class UResourceData;

/**
 * Game instance for Chemicode.
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UChemicodeGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TMap<FString, UResourceData*> Resources;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AResourceContainer> TestTubeClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABunsenBurner> BunsenBurnerClass;
	
	virtual void Init() override;
};
