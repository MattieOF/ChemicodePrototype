// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ResourceData.h"
#include "GameFramework/Actor.h"
#include "ResourceShopItem.generated.h"

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Resource")
	UResourceData* ResourceData;

	UPROPERTY()
	bool bAllowMeshOverride = false;

	UPROPERTY()
	UStaticMeshComponent* ItemMesh;
	
};
