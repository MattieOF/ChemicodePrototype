// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "OutlineComponent.h"
#include "GameFramework/Actor.h"
#include "ResourceItem.generated.h"

// Forward decl, if not we get a circular dependency
class UResourceData;

UCLASS(ClassGroup=(Chemicode), Blueprintable)
class CHEMICODEPROTOTYPE_API AResourceItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResourceItem();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UResourceData* Resource;

	void SetResource(UResourceData* ResourceData);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

	UPROPERTY()
	UOutlineComponent* Outline;
	
};
