// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "OutlineComponent.h"
#include "GameFramework/Actor.h"
#include "ChemicodeObject.generated.h"

UCLASS()
class CHEMICODEPROTOTYPE_API AChemicodeObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChemicodeObject();

	/**
	 * @return The outline component for this object
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UOutlineComponent* GetOutline() const { return OutlineComponent; }
	
	/**
	 * @return The main mesh of this object
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UStaticMeshComponent* GetMainMesh() const { return MainMesh; }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * @brief Main mesh of this item
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MainMesh;

	/**
	 * @brief Outline component of this item
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UOutlineComponent* OutlineComponent;
	
};
