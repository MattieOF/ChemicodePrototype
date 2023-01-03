// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "CameraPlane.h"
#include "ResourceData.h"
#include "GameFramework/Pawn.h"
#include "ChemicodePawn.generated.h"

class UResourceInfoWidget;
class AChemicodeGameMode;
UCLASS(ClassGroup=(Chemicode), Blueprintable)
class CHEMICODEPROTOTYPE_API AChemicodePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AChemicodePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void LookUp();
	
	UFUNCTION(BlueprintCallable)
	void LookDown();
	
	UFUNCTION(BlueprintCallable)
	void LookLeft();
	
	UFUNCTION(BlueprintCallable)
	void LookRight();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE ACameraPlane* GetCurrentCamPlane() { return CurrentCamPlane; }

	UFUNCTION(BlueprintCallable)
	void SetCamPlane(ACameraPlane* NewCamPlane, float BlendTime = 0.75f);

	UFUNCTION(BlueprintCallable)
	bool ResourceHovered(UResourceData* Resource);

	UFUNCTION(BlueprintCallable)
	void TryBuyResource(UResourceData* Resource);

	UFUNCTION(BlueprintCallable)
	void ResourceLostHover();

	UFUNCTION(BlueprintCallable)
	void HighlightItem(AResourceItem* Item);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE AResourceItem* GetHeldItem() { return HeldItem; }

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 7.f;

	UPROPERTY(EditAnywhere)
	float ItemMoveSpeed = 1700.f; // Units per second to move

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float FOVMin = 80;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float FOVMax = 135;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UResourceInfoWidget> ResourceInfoWidgetClass;


private:
	void MoveHorizontal(float Value);
	void MoveVertical(float Value);

	void OnScroll(float Value);

	void OnUse();
	void OnInteract();
	
	UPROPERTY()
	AChemicodeGameMode* GameMode;
	UPROPERTY()
	ACameraPlane* CurrentCamPlane;
	UPROPERTY()
	ACameraPlane* PrevCamPlane;
	UPROPERTY()
	APlayerController* PlayerController;
	UPROPERTY()
	UResourceInfoWidget* InfoWidget;
	UPROPERTY()
	AResourceItem* HeldItem;
	UPROPERTY()
	AResourceItem* HighlightedItem;

	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery>> ItemObjectTypeArray;

	bool bResourceInfoVisible = false;
	float LookCooldown = 0;
	FVector TargetItemPosition = FVector::Zero();
};
