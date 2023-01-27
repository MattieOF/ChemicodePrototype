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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsOpen() const { return bIsOpen; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void Connect() { bIsConnected = true; OnConnect(); };
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void Disconnect() { bIsConnected = false; OnDisconnect(); };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsConnected() const { return bIsConnected; }
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnOpen();

	UFUNCTION(BlueprintImplementableEvent)
	void OnClose();

	UFUNCTION(BlueprintImplementableEvent)
	void OnConnect();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDisconnect();
	
	virtual bool Interact() override;

	virtual bool AltInteractWith(AChemicodeObject* OtherObject) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* TapMesh;

	UPROPERTY(BlueprintReadWrite)
	bool bIsOpen = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsConnected = false;
};
