// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ResourceItem.h"
#include "InteractionComponent.h"
#include "ResourceData.generated.h"

class UHazardData;

UENUM(BlueprintType)
enum EResourceType
{
	RTSolid   UMETA(DisplayName = "Solid"),
	RTLiquid  UMETA(DisplayName = "Liquid"),
	RTGas     UMETA(DisplayName = "Gas")
};

// Types related to default properties
UENUM(BlueprintType)
enum EResourcePropertyType
{
	RPTDecimal  UMETA(DisplayName = "Decimal"),
	RPTString   UMETA(DisplayName = "String"),
	RPTBool     UMETA(DisplayName = "Bool")
};

USTRUCT()
struct FDefaultResourceProperty
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName Name;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EResourcePropertyType> PropertyType = RPTDecimal;
	UPROPERTY(EditAnywhere)
	double DecimalValue = 0;
	UPROPERTY(EditAnywhere)
	FString StringValue = "";
	UPROPERTY(EditAnywhere)
	bool BoolValue = false;
	UPROPERTY(EditAnywhere)
	bool bHiddenInUI = false;
};

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
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(MultiLine=true))
	FText Description;

	/**
	 * @brief Chemical formula. Supports superscript and the like.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText ChemicalFormula;

	/**
	 * @brief Default state of the resource
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TEnumAsByte<EResourceType> DefaultState;

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
	 * @brief Amount of the resource in a default item of this resource
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FResourceMeasurement BaseMeasurement;

	/**
	 * @brief Array of default properties, used by ResourceInstance during initialisation
	 */
	UPROPERTY(EditAnywhere)
	TArray<FDefaultResourceProperty> DefaultProperties;
};

USTRUCT(BlueprintType)
struct FMeasuredResource
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UResourceData* Resource;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FResourceMeasurement Amount;
};
