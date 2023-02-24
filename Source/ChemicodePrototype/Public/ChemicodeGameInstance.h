// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "BunsenBurner.h"
#include "Engine/GameInstance.h"
#include "ChemicodeGameInstance.generated.h"

class AResourceTube;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class References")
	TSubclassOf<AResourceContainer> TestTubeClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class References")
	TSubclassOf<ABunsenBurner> BunsenBurnerClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class References")
	TSubclassOf<AResourceTube> GasTubeClass;
	
	virtual void Init() override;
};
