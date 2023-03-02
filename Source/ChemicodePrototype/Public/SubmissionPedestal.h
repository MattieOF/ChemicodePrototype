// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeObject.h"
#include "ResourceContainer.h"
#include "SubmissionPedestal.generated.h"

UCLASS()
class CHEMICODEPROTOTYPE_API ASubmissionPedestal : public AChemicodeObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASubmissionPedestal();

	virtual void Tick(float DeltaSeconds) override;
	
	virtual bool InteractWith(AChemicodeObject* OtherObject) override;

	UPROPERTY(BlueprintReadWrite)
	AResourceContainer* Container = nullptr;

	FDelegateHandle ContainerCallbackHandle;

	bool bShouldRemoveContainer = false;

	UPROPERTY(BlueprintReadWrite)
	bool bEnabled = true;

	UPROPERTY(EditAnywhere, meta=(MakeEditWidget = true))
	FVector PlacementOffset;
};
