// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interaction.h"
#include "InteractionComponent.generated.h"

class UResourceData;
class AResourceItem;

// Signature for the interaction event
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractSignature);
// Signature for the interact with event. Parameter is the item used on this item.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractWithSignature, AResourceItem*, Item);

/**
 * @brief Parameter struct for interaction functions.
 * Used by ProcessEvent().
 */
struct FInteractionFuncParams
{
	// Inputs
	AResourceItem* Item;

	// Outputs
	bool bOutSuccess;
};

UCLASS( ClassGroup=(Chemicode), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class CHEMICODEPROTOTYPE_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * @brief Set default values
	 */
	UInteractionComponent();

	/**
	 * @brief On interact event to be implemented by blueprints
	 */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnInteract"), Category = Events)
	void OnInteract();

	/**
	 * @brief Event called when an item is used on this item.
	 * This event can be overridden by child classes, but the default behaviour is generally good enough;
	 * it uses the ItemInteractions map to find a function to be called depending on the resource data of Item.
	 * @param Item Item used on this item
	 */
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "OnInteractWith"), Category = Events)
	void OnInteractWith(AResourceItem* Item);

	/**
	 * @brief Get interaction data for a resource
	 * @param Resource Resource to query for interaction with
	 * @return Interaction data. bIsValid will be false if not valid.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	FInteraction GetInteractionWith(UResourceData* Resource);

	/**
	 * @brief Map of resource data -> function names. If using the default behaviour of OnInteractWith(),
	 * when an item is used on this item the map is checked for the corresponding resource data and if it exists,
	 * the corresponding function is called. The functions should have 1 parameter of type AResourceItem.
	 */
	UPROPERTY(EditAnywhere, Category = Events,
		meta=(ToolTip=
			"Resource types mapped to the name of a function to be called when it is used. Function should have 1 parameter of type AResourceItem."
		))
	TMap<UResourceData*, FName> ItemInteractions;

	/**
	 * @brief Item that owns this interaction component
	 */
	UPROPERTY(BlueprintReadWrite)
	AResourceItem* OwnerItem;
	
	/**
	 * @brief Sets owner item variable and checks its valid
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief Begin a latent event; by default, stops the player interacting so an animation can play.
	 */
	UFUNCTION(BlueprintCallable)
	void BeginLatentInteraction(float Length);

	/**
	 * @brief End a latent event; by default, enables player interaction.
	 */
	UFUNCTION(BlueprintCallable)
	void EndLatentInteraction();

};
