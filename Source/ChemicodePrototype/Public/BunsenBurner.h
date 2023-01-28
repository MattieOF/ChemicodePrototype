// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeObject.h"
#include "CableComponent.h"
#include "GasTap.h"
#include "BunsenBurner.generated.h"

class AResourceItem;
/// Enum representing current burner state
UENUM(BlueprintType)
enum EBunsenBurnerState
{
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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool HasGas() { return ConnectedGasTap && ConnectedGasTap->IsOpen(); }

	UFUNCTION(BlueprintImplementableEvent)
	void OnStateUpdated(EBunsenBurnerState NewState);

	virtual bool Use() override;
	virtual bool AltInteract() override;
	virtual bool InteractWith(AChemicodeObject* OtherObject) override;

	virtual void GetActorBounds(bool bOnlyCollidingComponents, FVector& Origin, FVector& BoxExtent, bool bIncludeFromChildActors) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MakeEditWidget=true))
	FVector ItemOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCableComponent* Cable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* CollarMesh;

	UPROPERTY(BlueprintReadWrite)
	AGasTap* ConnectedGasTap;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EBunsenBurnerState> State;

	UPROPERTY(BlueprintReadWrite)
	AResourceItem* TargetItem;

	FDelegateHandle TargetItemDelegateHandle;
	
	bool bHadGasLastFrame = false;
};
