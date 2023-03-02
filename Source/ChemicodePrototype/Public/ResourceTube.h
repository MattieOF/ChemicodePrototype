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
	FORCEINLINE bool TransferResource(AChemicodeObject* From, UResourceData* Resource, FResourceMeasurement Amount, float AsScale = 1,
	                                  bool bMultiplyByDeltaTime = true, float DeltaTimeOverride = 0)
	{
		return TransferResourceAs(From, Resource, Resource, Amount, AsScale, bMultiplyByDeltaTime, DeltaTimeOverride);
	}
	
	UFUNCTION(BlueprintCallable)
	bool TransferResourceAs(AChemicodeObject* From, UResourceData* Resource, UResourceData* As,
	                        FResourceMeasurement Amount, float AsScale = 1, bool bMultiplyByDeltaTime = true,
	                        float DeltaTimeOverride = 0);
	
	UFUNCTION(BlueprintCallable)
	bool TransferResources(AChemicodeObject* From, TArray<FMeasuredResource> Resources,
		float Scale = 1, bool bMultiplyByDeltaTime = true, float DeltaTimeOverride = 0);

	UFUNCTION(BlueprintCallable)
	bool ConnectObject(AChemicodeObject* Object);

	UFUNCTION(BlueprintCallable)
	void DisconnectObjects();

	virtual void GetActorBounds(bool bOnlyCollidingComponents, FVector& Origin, FVector& BoxExtent,
	                            bool bIncludeFromChildActors) const override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool HasLHSConnection() const { return LHSConnection != nullptr; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool HasRHSConnection() const { return RHSConnection != nullptr; }

	UFUNCTION()
	void OnConnectionDestroyed(AActor* Connection);
	
	UPROPERTY(BlueprintReadOnly)
	bool bSimulated = false;
	
	UPROPERTY()
	UWorld* WorldRef;
	
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
