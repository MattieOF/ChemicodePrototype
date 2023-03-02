// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeObject.h"
#include "SubmissionPedestal.h"
#include "GodButton.generated.h"

UCLASS()
class CHEMICODEPROTOTYPE_API AGodButton : public AChemicodeObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGodButton();

	virtual bool Interact() override;

	UFUNCTION(BlueprintImplementableEvent)
	void DoSubmission();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASubmissionPedestal* Pedestal;
};
