// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeObject.h"
#include "GameFramework/Pawn.h"
#include "ChemicodePawn.generated.h"

class UChemicodeGameInstance;
// Forward declarations
class UResourceItemTooltipWidget;
class AResourceItem;
class UResourceData;
class ACameraPlane;
class UResourceInfoWidget;
class AChemicodeGameMode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCamPlaneChanged, ACameraPlane*, NewCamPlane);

UCLASS(ClassGroup=(Chemicode), Blueprintable)
class CHEMICODEPROTOTYPE_API AChemicodePawn : public APawn
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets default values
	 */
	AChemicodePawn();

protected:
	/**
	 * @brief Initialises the pawn, including setting it up for mouse controls.
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * @brief Ticks cooldowns and updates hovered and held items
	 * @param DeltaTime Time since this function was last called
	 */
	virtual void Tick(float DeltaTime) override;

	// -- Look functions --
	// These functions set the cam plane depending on the cam plane
	// For example, calling LookUp() while on the table cam plane will set the cam plane to the cabinet
	// They are called by the input component
	
	UFUNCTION(BlueprintCallable)
	void LookUp();
	
	UFUNCTION(BlueprintCallable)
	void LookDown();
	
	UFUNCTION(BlueprintCallable)
	void LookLeft();
	
	UFUNCTION(BlueprintCallable)
	void LookRight();
	
	// -- End look functions --

	/**
	 * @brief Binds functions to input actions and axis
	 * @param PlayerInputComponent Input component to add functionality to
	 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * @return Current ACameraPlane used by the pawn
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE ACameraPlane* GetCurrentCamPlane() const { return CurrentCamPlane; }

	/**
	 * @brief Switches cam plane used by the player to move along and view from.
	 * This transition is smooth.
	 * Also un-hovers, un-highlights, and drops any items. 
	 * @param NewCamPlane Plane to switch to
	 * @param BlendTime Time taken to blend between the previous cam plane and the new one
	 */
	UFUNCTION(BlueprintCallable)
	void SetCamPlane(ACameraPlane* NewCamPlane, float BlendTime = 0.75f);

	/**
	 * @brief Function called by shop items to tell the pawn to update the resource info widget
	 * @param Resource Resource that has been hovered
	 * @return True if the resource tooltip is now visible, false if not
	 */
	// TODO: Maybe this shouldn't be in the pawn
	UFUNCTION(BlueprintCallable)
	bool ResourceHovered(UResourceData* Resource);

	/**
	 * @brief Called when a shop item looses its hover, causing the resource info widget to be hidden
	 */
	UFUNCTION(BlueprintCallable)
	void ResourceLostHover();
	
	/**
	 * @brief Buy a resource and create a ResourceItem for it. Also holds this new item and calls LookDown().
	 * @param Resource Resource to buy
	 */
	UFUNCTION(BlueprintCallable)
	void TryBuyResource(UResourceData* Resource);

	UFUNCTION(BlueprintCallable)
	void HoldNewItem(TSubclassOf<AChemicodeObject> ObjectClass);
	
	/**
	 * @brief De-highlights previous highlighted item and highlights a new one.
	 * Also updates the resource item tooltip.
	 * @param Item Item to be highlighted
	 */
	UFUNCTION(BlueprintCallable)
	void HighlightItem(AChemicodeObject* Item);

	/**
	 * @brief Drops the currently held item (if it isn't nullptr), and picks up a new one. 
	 * Outlines are also managed by this function.
	 * @param Item Item to be held
	 */
	UFUNCTION(BlueprintCallable)
	void HoldItem(AChemicodeObject* Item);

	/**
	 * @brief If it isn't null, drop the currently held item and de-highlight it.
	 */
	UFUNCTION(BlueprintCallable)
	void DropItem();

	/**
	 * @return The currently held item
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE AChemicodeObject* GetHeldItem() const { return HeldItem; }

	/**
	 * @brief Disables the pawns ability to interact with, move, and hover over items.
	 */
	 UFUNCTION(BlueprintCallable)
	void DisableInteraction();

	/**
	 * @brief Enables the pawns ability to interact with, move, and hover over items.
	 */
	UFUNCTION(BlueprintCallable)
	void EnableInteraction();

	/**
	 * @brief Refresh the resource item tooltip.
	 * Use this if the currently hovered item has changed resource data
	 */
	UFUNCTION(BlueprintCallable)
	void RefreshTooltip();
	
	/**
	 * @brief Speed of the movement along the current cam plane
	 */
	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 7.f;

	/**
	 * @brief Speed the currently held item moves towards the target position
	 */
	UPROPERTY(EditAnywhere)
	float ItemMoveSpeed = 1700.f; // Units per second to move

	/**
	 * @brief Minimum allowed field of view when being changed with scroll
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float FOVMin = 80;
	/**
	 * @brief Maximum allowed field of view when being changed with scroll 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float FOVMax = 135;

	/**
	 * @brief Static class of the resource info widget to be instantiated at runtime
	 */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UResourceInfoWidget> ResourceInfoWidgetClass;

	/**
	 * @brief Static class of the resource tooltip widget to be instantiated at runtime
	 */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UResourceItemTooltipWidget> ResourceTooltipWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle CurrentInteractionTimer;

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnCamPlaneChanged OnCamPlaneChanged;

private:
	// Movement input functions
	void MoveHorizontal(float Value);
	void MoveVertical(float Value);

	/**
	 * @brief Callback for scroll movement, used for zooming in and out
	 */
	void OnScroll(float Value);

	/**
	 * @brief Uses with the currently held item 
	 */
	void OnUse();
	/**
	 * @brief If hovering over an item while also holding one: Interacts with the hovered item with the held item\n
	 * If holding an item: drops the current item\n
	 * Else, try and pick up the currently highlighted item
	 */
	void OnInteract();

	void OnAltInteractDown();
	
	void OnAltInteractUp();

	void OnTiltCamDown();

	void OnTiltCamUp();
	/**
	 * @brief Controls if the pawn can interact with, move, and hover over items or not.
	 */
	UPROPERTY()
	bool bInteractionEnabled = true;

	/**
	 * @brief If true, the player is currently tilting the camera
	 */
	UPROPERTY()
	bool bTilting = false;

	/**
	 * @brief Reference to the game mode object
	 */
	UPROPERTY()
	AChemicodeGameMode* GameMode;
	/**
	 * @brief Reference to the game instance object
	 */
	UPROPERTY()
	UChemicodeGameInstance* GameInstance;
	/**
	 * @brief The current CamPlane
	 */
	UPROPERTY()
	ACameraPlane* CurrentCamPlane;
	/**
	 * @brief Previous cam plane. Set by SetCamPlane(). Used to return to the previous plane in some Look functions.
	 */
	UPROPERTY()
	ACameraPlane* PrevCamPlane;
	/**
	 * @brief This pawns controller
	 */
	UPROPERTY()
	APlayerController* PlayerController;
	/**
	 * @brief The resource info widget used for shop items
	 */
	UPROPERTY()
	UResourceInfoWidget* InfoWidget;
	/**
	 * @brief Currently held item
	 */
	UPROPERTY()
	AChemicodeObject* HeldItem;
	/**
	 * @brief Currently highlighted item
	 */
	UPROPERTY()
	AChemicodeObject* HighlightedItem;
	/**
	 * @brief Item currently involved in an alternate interaction
	 */
	UPROPERTY()
	AChemicodeObject* AltInteractionItem;
	/**
	 * @brief The tooltip widget used by resource items
	 */
	UPROPERTY()
	UResourceItemTooltipWidget* TooltipWidget;

	/**
	 * @brief Array of object types used by the traces that check for hovered items
	 */
	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery>> ItemObjectTypeArray;

	/**
	 * @brief If the resource info widget is currently shown
	 */
	bool bResourceInfoVisible = false;
	/**
	 * @brief Time in seconds until the player can change cam plane again
	 */
	float LookCooldown = 0;
	/**
	 * @brief Current position for the held item
	 */
	FVector TargetItemPosition = FVector::Zero();
};
