// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "HazardData.h"
#include "ResourceItem.h"
#include "ResourceData.generated.h"

/**
 * Basic data class for resources
 */
UCLASS(ClassGroup=(Chemicode), Blueprintable)
class CHEMICODEPROTOTYPE_API UResourceData : public UDataAsset
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText Name;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText Description;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText ChemicalFormula;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UStaticMesh* Mesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UMaterialInterface* MeshMaterial;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AResourceItem> ResourceItemClass = AResourceItem::StaticClass();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<UHazardData*> Hazards; 
};
