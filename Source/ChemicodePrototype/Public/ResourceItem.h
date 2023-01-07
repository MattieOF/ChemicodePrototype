// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceItem.generated.h"

// Forward declarations
class UOutlineComponent;
class UInteractionComponent;
class UResourceData;
class AChemicodePawn;

UENUM(BlueprintType)
enum EResourceState
{
	Usable UMETA(DisplayName = "Usable"),
	HasLid UMETA(DisplayName = "Has Lid"),
	Empty  UMETA(DisplayName = "Empty"),
};

UCLASS(ClassGroup=(Chemicode), Blueprintable)
class CHEMICODEPROTOTYPE_API AResourceItem : public AActor
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief Sets default properties, including creating the basic mesh component and outline component.
	 */
	AResourceItem();

	/**
	 * @brief Resource data for this item.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UResourceData* Resource;

	/**
	 * @brief Sets the resource, including setting the mesh and material.
	 * @param ResourceData Resource to use
	 */
	UFUNCTION(BlueprintCallable)
	void SetResource(UResourceData* ResourceData);

	/**
	 * @brief State of this resource item
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EResourceState> ResourceState;

	/**
	 * @brief Destroys current interaction component if it exists and creates a new one of the provided class.
	 * use this to change how an item behaves during runtime.
	 * @param NewType Class of the new interaction type
	 */
	UFUNCTION(BlueprintCallable)
	void SetInteractionType(TSubclassOf<UInteractionComponent> NewType);

	/**
	 * @brief Interacts with the item.
	 * @return True if the interaction could be performed, false if not.
	 */
	UFUNCTION(BlueprintCallable)
	bool Interact() const;

	/**
	 * @brief Interacts with this item with another.
	 * @param Item The item to use on this item.
	 * @return True if the interaction could be performed, false if not.
	 */
	UFUNCTION(BlueprintCallable)
	bool InteractWith(AResourceItem* Item) const;

	/**
	 * @return The outline component for this item
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UOutlineComponent* GetOutline() const { return Outline; }

protected:
	/**
	 * @brief Initialises the item
	 */
	virtual void BeginPlay() override;

#if WITH_EDITOR
	/**
	 * @brief Changes the mesh in the editor when the resource is changed
	 */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	/**
	 * @brief Main mesh of this item
	 */
	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

	/**
	 * @brief Reference to the chemicode player
	 */
	UPROPERTY()
	AChemicodePawn* Player;

	/**
	 * @brief Outline component of this item
	 */
	UPROPERTY()
	UOutlineComponent* Outline;

	/**
	 * @brief Current interaction component of this item. Set it with SetInteractionType().
	 */
	UPROPERTY()
	UInteractionComponent* InteractionComponent;
};
