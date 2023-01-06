// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "OutlineComponent.h"
#include "GameFramework/Actor.h"
#include "ResourceItem.generated.h"

class AChemicodePawn;
// Forward decl, if not we get a circular dependency
class UResourceData;

UENUM(BlueprintType)
enum EResourceState
{
	Usable UMETA(DisplayName = "Usable"),
	HasLid UMETA(DisplayName = "Has Lid"),
	Empty  UMETA(DisplayName = "Empty"),
};

UCLASS(ClassGroup=(Chemicode), Blueprintable)
class CHEMICODEPROTOTYPE_API AResourceItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResourceItem();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UResourceData* Resource;

	UFUNCTION(BlueprintCallable)
	void SetResource(UResourceData* ResourceData);
		
	// TODO: Maybe make these multicast delegates?
	UFUNCTION(BlueprintNativeEvent)
	void Use();

	UFUNCTION(BlueprintNativeEvent)
	void UseWithItem(AResourceItem* Item);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EResourceState> ResourceState;

	UFUNCTION()
	virtual void BeginMouseOver();

	UFUNCTION()
	virtual void EndMouseOver();

	FORCEINLINE UOutlineComponent* GetOutline() { return Outline; }

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
	AChemicodePawn* Player;

	UPROPERTY()
	UOutlineComponent* Outline;
	
};
