// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceShopItem.generated.h"

class UOutlineComponent;
class UResourceData;
class AChemicodeGameMode;
class AChemicodePawn;

UCLASS(ClassGroup=(Chemicode), Blueprintable)
class CHEMICODEPROTOTYPE_API AResourceShopItem : public AActor
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief Initialise main mesh and outline 
	 */
	AResourceShopItem();

protected:
	/**
	 * @brief Set mesh and material and setup mouse event delegates.
	 */
	virtual void BeginPlay() override;

#if WITH_EDITOR
	/**
	 * @brief Changes the mesh live when resource property is changed in editor.
	 */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	/**
	 * @brief Set tooltip and show outline when mouse is over and we're in the cabinet.
	 */
	UFUNCTION()
	virtual void BeginMouseOver();

	/**
	 * @brief Hide tooltip and outline when mouse leaves.
	 */
	UFUNCTION()
	virtual void EndMouseOver();

public:
	/**
	 * @brief Resource data for this shop item.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Resource")
	UResourceData* ResourceData;

	/**
	 * @brief If true, BeginPlay will not set the main mesh to the one set in ResourceData.
	 * However, if the mesh is null by default, it will still be set.
	 */
	UPROPERTY()
	bool bAllowMeshOverride = false;

	/**
	 * @brief Main mesh component.
	 */
	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	/**
	 * @brief Outline component.
	 */
	UPROPERTY(BlueprintReadOnly)
	UOutlineComponent* OutlineComponent;

private:
	/**
	 * @brief Reference to chemicode gamemode.
	 */
	UPROPERTY()
	AChemicodeGameMode* GameMode;

	/**
	 * @brief Reference to chemicode player.
	 */
	UPROPERTY()
	AChemicodePawn* Player;
};
