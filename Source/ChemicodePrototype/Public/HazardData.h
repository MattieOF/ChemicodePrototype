// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HazardData.generated.h"

/**
 * Data container for a hazard a resource can have
 */
UCLASS(ClassGroup=(Chemicode), Blueprintable)
class CHEMICODEPROTOTYPE_API UHazardData : public UDataAsset
{
	GENERATED_BODY()
public:
	/**
	 * @brief Name of the hazard
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	/**
	 * @brief Description of the hazard
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(MultiLine=true))
	FText Description;

	/**
	 * @brief Icon used by hazard UI
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon;
};
