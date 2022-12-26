// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "OutlineComponent.h"
#include "ResourceData.h"
#include "GameFramework/Actor.h"
#include "ResourceShopItem.generated.h"

class AChemicodeGameMode;
class AChemicodePawn;
UCLASS()
class CHEMICODEPROTOTYPE_API AResourceShopItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResourceShopItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UFUNCTION()
	virtual void BeginMouseOver();

	UFUNCTION()
	virtual void EndMouseOver();

	UFUNCTION()
	virtual void OnClick();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Resource")
	UResourceData* ResourceData;

	UPROPERTY()
	bool bAllowMeshOverride = false;

	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(BlueprintReadOnly)
	UOutlineComponent* OutlineComponent;

private:
	UPROPERTY()
	AChemicodeGameMode* GameMode;
	
	UPROPERTY()
	AChemicodePawn* Player;
};
