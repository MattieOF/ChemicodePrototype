// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeObject.h"
#include "ResourceMeasurement.h"
#include "GameFramework/Actor.h"
#include "ResourceItem.generated.h"

class AChemicodeGameMode;
// Forward declarations
class UOutlineComponent;
class UInteractionComponent;
class UResourceData;
class UResourceInstance;
class AChemicodePawn;

UENUM(BlueprintType)
enum EResourceItemState
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

	void Initialise(UWorld* World);
	
	/**
	 * @brief Resource this item represents
	 */
	UPROPERTY(BlueprintReadWrite)
	UResourceInstance* Resource;

	/**
	 * @brief If true, Measurement will not be set to Resource.DefaultMeasurement on start
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOverrideDefaultMeasurement = false;

	/**
	 * @brief Sets the resource, including setting the mesh and material.
	 * @param ResourceData Resource to use
	 * @param bRefreshTooltip If true, the players tooltip is refreshed, to reflect any changes that happened due to
	 * the change of resource type.
	 * @param bPreserveMeasurement If true, the measurement will not be set to ResourceData.DefaultMeasurement.
	 */
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay="1"))
	void SetResource(UResourceData* ResourceData, bool bRefreshTooltip = true, bool bPreserveMeasurement = true);

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
	 * @param bPreserveMeasurement If true, the measurement is kept the same. If not, it's reset to NewResource.DefaultMeasurement.
	 */
	UFUNCTION(BlueprintCallable)
	void SetResourceAndInteraction(UResourceData* NewResource, TSubclassOf<UInteractionComponent> NewInteraction, bool bPreserveMeasurement = true);

	/**
	 * @brief Interacts with the item.
	 * @return True if the interaction could be performed, false if not.
	 */
	virtual bool Use() override;

	/**
	 * @brief Interacts with this item with another.
	 * @param OtherObject Object to use on this item
	 * @return True if the interaction could be performed, false if not.
	 */
	virtual bool InteractWith(AChemicodeObject* OtherObject) override;

	/**
	 * @return The interaction component for this item. Can be nullptr! 
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }

	virtual void FireTick(AChemicodeObject* Source) override;
	
	/**
	 * @brief Set measurement of resource in this item to NewMeasurement. Also updates its unit.
	 * @param NewMeasurement New measurement
	 */
	UFUNCTION(BlueprintCallable)
	void SetMeasurement(FResourceMeasurement NewMeasurement);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EResourceItemState> DefaultItemState = Usable;

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

	UPROPERTY()
	AChemicodeGameMode* GameMode;

	/**
	 * @brief Current interaction component of this item. Set it with SetInteractionType().
	 */
	UPROPERTY()
	UInteractionComponent* InteractionComponent;
};
