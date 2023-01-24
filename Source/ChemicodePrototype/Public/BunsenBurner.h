// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeObject.h"
#include "CableComponent.h"
#include "GasTap.h"
#include "BunsenBurner.generated.h"

/// Enum representing current burner state
UENUM(BlueprintType)
enum EBunsenBurnerState
{
	BBSOff             UMETA(DisplayName = "Off"),
	BBSCollarClosed    UMETA(DisplayName = "Collar Closed"),
	BBSCollarHalfOpen  UMETA(DisplayName = "Collar Half Open"),
	BBSCollarFullyOpen UMETA(DisplayName = "Collar Fully Open")
};

/**
 * Bunsen burner object
 */
UCLASS()
class CHEMICODEPROTOTYPE_API ABunsenBurner : public AChemicodeObject
{
	GENERATED_BODY()

public:
	ABunsenBurner();

	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable)
	void ConnectToGasTap(AGasTap* GasTap);
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void DisconnectFromGasTap() { ConnectToGasTap(nullptr); }
	UFUNCTION(BlueprintCallable)
	void SetState(EBunsenBurnerState NewState);

	UFUNCTION(BlueprintImplementableEvent)
	void OnStateUpdated(EBunsenBurnerState NewState);

	virtual void GetActorBounds(bool bOnlyCollidingComponents, FVector& Origin, FVector& BoxExtent, bool bIncludeFromChildActors) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCableComponent* Cable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* CollarMesh;

	UPROPERTY(BlueprintReadWrite)
	AGasTap* ConnectedGasTap;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EBunsenBurnerState> State;
};
