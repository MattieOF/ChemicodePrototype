// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ChemicodeGameInstance.generated.h"

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
	
	virtual void Init() override;
};
