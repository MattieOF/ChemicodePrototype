// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "OutlineComponent.h"
#include "GameFramework/Actor.h"
#include "ResourceMeasurement.h"
#include "ChemicodeObject.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnItemPickedUp)
DECLARE_MULTICAST_DELEGATE(FOnItemPlaced)

UCLASS()
class CHEMICODEPROTOTYPE_API AChemicodeObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChemicodeObject();

	/**
	 * @return The outline component for this object
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UOutlineComponent* GetOutline() const { return OutlineComponent; }

	/**
	 * @return The main mesh of this object
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UStaticMeshComponent* GetMainMesh() const { return MainMesh; }

	UFUNCTION(BlueprintCallable)
	virtual bool Use();
	
	UFUNCTION(BlueprintCallable)
	virtual bool Interact();

	UFUNCTION(BlueprintImplementableEvent)
	void OnInteract();

	UFUNCTION(BlueprintCallable)
	virtual bool AltInteract();
	
	UFUNCTION(BlueprintCallable)
	virtual bool InteractWith(AChemicodeObject* OtherObject);

	UFUNCTION(BlueprintImplementableEvent)
	void OnInteractWith(AChemicodeObject* OtherObject);

	UFUNCTION(BlueprintCallable)
	virtual bool AltInteractWith(AChemicodeObject* OtherObject);

	UFUNCTION(BlueprintImplementableEvent)
	void AltInteractionBegin();
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAltInteractionPosition(FVector EndPosition);

	UFUNCTION(BlueprintImplementableEvent)
	void AltInteractionEnd();

	UFUNCTION(BlueprintCallable)
	virtual void FireTick(AChemicodeObject* Source, float DeltaTime);

	UFUNCTION(BlueprintCallable)
	virtual void ReceiveResource(UResourceData* Resource, FResourceMeasurement Amount);

	UFUNCTION(BlueprintCallable)
	virtual bool HasResource(UResourceData* Resource);

	UFUNCTION(BlueprintCallable) 
	virtual FResourceMeasurement GetResourceAmount(UResourceData* Resource);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Name;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(MultiLine = true))
	FText Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHoldable = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bResourceLike = true;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bDragInteraction = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bRequireFreeSpace = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bNonTable = false;

	FOnItemPickedUp OnItemPickedUp;
	
	FOnItemPlaced OnItemPlaced;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * @brief Main mesh of this item
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MainMesh;

	/**
	 * @brief Outline component of this item
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UOutlineComponent* OutlineComponent;
	
};
