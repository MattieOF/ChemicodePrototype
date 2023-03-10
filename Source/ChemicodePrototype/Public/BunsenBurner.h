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
	FORCEINLINE EBunsenBurnerState GetState() { return State; }

	UFUNCTION(BlueprintImplementableEvent)
	void OnStateUpdated(EBunsenBurnerState NewState);

	UFUNCTION(BlueprintCallable)
	void ClearItem();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool HasGas() { return ConnectedGasTap && ConnectedGasTap->IsOpen(); }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool HasItem() { return TargetItem != nullptr; }

	UFUNCTION()
	void OnTargetDestroyed(AActor* Target);

	// Interaction implementations
	virtual bool Use() override;
	virtual bool AltInteract() override;
	virtual bool InteractWith(AChemicodeObject* OtherObject) override;

	virtual void GetActorBounds(bool bOnlyCollidingComponents, FVector& Origin, FVector& BoxExtent, bool bIncludeFromChildActors) const override;

	UPROPERTY()
	UWorld* WorldRef = nullptr;

	bool bSimulated = false;

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
	AChemicodeObject* TargetItem;

	FDelegateHandle TargetItemDelegateHandle;
	
	bool bHadGasLastFrame = false;
	bool bShouldRemoveTarget = false;
};
