// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeObject.h"
#include "ResourceMeasurement.h"
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
class CHEMICODEPROTOTYPE_API AResourceItem : public AChemicodeObject
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
	 * @brief State of this resource item
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EResourceState> ResourceState;

	/**
	 * @brief If true, Measurement will not be set to Resource.DefaultMeasurement on start
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOverrideDefaultMeasurement = false;

	/**
	 * @brief Current amount of the resource in this item
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FResourceMeasurement Measurement;
	
	/**
	 * @brief Sets the resource, including setting the mesh and material.
	 * @param ResourceData Resource to use
	 * @param bRefreshTooltip If true, the players tooltip is refreshed, to reflect any changes that happened due to
	 * the change of resource type.
	 */
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay="1"))
	void SetResource(UResourceData* ResourceData, bool bRefreshTooltip = true);

	/**
	 * @brief Destroys current interaction component if it exists and creates a new one of the provided class.
	 * Use this to change how an item behaves during runtime.
	 * @param NewType Class of the new interaction type.
	 * @param bRefreshTooltip If true, the players tooltip is refreshed, to reflect any changes that happened due to
	 * the change of interaction type.
	 */
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay="1"))
	void SetInteractionType(TSubclassOf<UInteractionComponent> NewType, bool bRefreshTooltip = true);

	/**
	 * @brief Sets the resource of the item and then the interaction type.
	 * This is the same as calling SetResource() and SetInteractionType() consecutively, but skips a second call to
	 * refresh tooltip.
	 * @param NewResource The new resource
	 * @param NewInteraction The class of the new interaction type
	 */
	UFUNCTION(BlueprintCallable)
	void SetResourceAndInteraction(UResourceData* NewResource, TSubclassOf<UInteractionComponent> NewInteraction);

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
	 * @return The interaction component for this item. Can be nullptr! 
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }

	/**
	 * @brief Set measurement of resource in this item to NewMeasurement. Also updates its unit.
	 * @param NewMeasurement New measurement
	 */
	UFUNCTION(BlueprintCallable)
	void SetMeasurement(FResourceMeasurement NewMeasurement);

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
	 * @brief Reference to the chemicode player
	 */
	UPROPERTY()
	AChemicodePawn* Player;

	/**
	 * @brief Current interaction component of this item. Set it with SetInteractionType().
	 */
	UPROPERTY()
	UInteractionComponent* InteractionComponent;
};
