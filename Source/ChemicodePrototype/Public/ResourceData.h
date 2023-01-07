// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ResourceItem.h"
#include "Interaction.h"
#include "InteractionComponent.h"
#include "ResourceData.generated.h"

class UHazardData;

/**
 * Basic data class for resources
 */
UCLASS(ClassGroup=(Chemicode), Blueprintable)
class CHEMICODEPROTOTYPE_API UResourceData : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * @brief Name of the resource.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText Name;

	/**
	 * @brief Description of the resource
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText Description;

	/**
	 * @brief Chemical formula. Supports superscript and the like.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText ChemicalFormula;

	/**
	 * @brief Icon displayed in UI
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UTexture2D* Icon;

	/**
	 * @brief Main mesh for the resource. More can be added with a custom resource item subclass.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UStaticMesh* Mesh;

	/**
	 * @brief Overriding material for the first slot on the main mesh.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UMaterialInterface* MeshMaterial;

	/**
	 * @brief Resource item subclass to be used when the item is bought.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AResourceItem> ResourceItemClass = AResourceItem::StaticClass();

	/**
	 * @brief Default interaction to be added when creating the item.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UInteractionComponent> DefaultInteraction = UInteractionComponent::StaticClass();

	/**
	 * @brief List of hazards this resource has. Displayed in the shop item tooltip.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<UHazardData*> Hazards;

	/**
	 * @brief Interaction data used by the shop item tooltip.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<UResourceData*, FInteraction> Interactions;
};
