// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "CableComponent.h"
#include "ChemicodeObject.h"
#include "ResourceData.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "ResourceTube.generated.h"

UCLASS()
class CHEMICODEPROTOTYPE_API AResourceTube : public AChemicodeObject
{
	GENERATED_BODY()

public:
	AResourceTube();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool TransferResource(AChemicodeObject* From, UResourceData* Resource, FResourceMeasurement Amount,
	                                  bool bMultiplyByDeltaTime = true)
	{
		return TransferResourceAs(From, Resource, Resource, Amount, bMultiplyByDeltaTime);
	}
	
	UFUNCTION(BlueprintCallable)
	bool TransferResourceAs(AChemicodeObject* From, UResourceData* Resource, UResourceData* As,
	                        FResourceMeasurement Amount, bool bMultiplyByDeltaTime = true);
	
	UFUNCTION(BlueprintCallable)
	bool TransferResources(AChemicodeObject* From, TArray<FMeasuredResource> Resources, bool bMultiplyByDeltaTime = true);

	UFUNCTION(BlueprintCallable)
	bool ConnectObject(AChemicodeObject* Object);

	virtual void GetActorBounds(bool bOnlyCollidingComponents, FVector& Origin, FVector& BoxExtent,
	                            bool bIncludeFromChildActors) const override;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCableComponent* Cable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UBoxComponent* Box;
	
	UPROPERTY(BlueprintReadWrite)
	AChemicodeObject* LHSConnection;
	UPROPERTY(BlueprintReadWrite)
	AChemicodeObject* RHSConnection;
};
