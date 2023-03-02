// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "BunsenBurner.h"
#include "Assignments/Campaign.h"
#include "Engine/GameInstance.h"
#include "ChemicodeGameInstance.generated.h"

class AResourceTube;
class AResourceContainer;
class UResourceData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAssignmentChanged, UAssignment*, Assignment);

/**
 * Game instance for Chemicode.
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UChemicodeGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FString, UResourceData*> Resources;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class References")
	TSubclassOf<AResourceContainer> TestTubeClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class References")
	TSubclassOf<ABunsenBurner> BunsenBurnerClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class References")
	TSubclassOf<AResourceTube> GasTubeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Campaign")
	UCampaignData* MainCampaignData;
	
	UPROPERTY(BlueprintReadOnly, Category = "Campaign")
	UCampaign* MainCampaign;

	UPROPERTY(BlueprintReadOnly, Category = "Assignment")
	UAssignment* CurrentAssignment;

	UPROPERTY(BlueprintReadOnly, Category = "Assignment")
	bool bIsScriptingAssignment = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Assignment")
	bool bIsPracticalAssignment = false;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Assignment")
	FOnAssignmentChanged OnAssignmentChanged;

	UFUNCTION(BlueprintCallable, Category = "Assignment")
	void BeginAssignment(UAssignment* Assignment);

	UFUNCTION(BlueprintCallable, Category = "Assignment")
	void ClearAssignment();
};
