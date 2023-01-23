// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeObject.h"
#include "GameFramework/Actor.h"
#include "GasTap.generated.h"

UCLASS()
class CHEMICODEPROTOTYPE_API AGasTap : public AChemicodeObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGasTap();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void Open()  { bIsOpen = true; OnOpen(); };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void Close() { bIsOpen = false; OnClose(); };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool Toggle() { if (bIsOpen) Close(); else Open(); return bIsOpen; }

	UFUNCTION(BlueprintImplementableEvent)
	void OnOpen();

	UFUNCTION(BlueprintImplementableEvent)
	void OnClose();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsOpen() const { return bIsOpen; }

	virtual bool Interact() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* TapMesh;

	UPROPERTY(BlueprintReadWrite)
	bool bIsOpen = false;
};
