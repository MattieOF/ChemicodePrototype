// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "CameraPlaneCam.generated.h"

UCLASS(ClassGroup=(Chemicode), Blueprintable)
class CHEMICODEPROTOTYPE_API ACameraPlaneCam : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraPlaneCam();

	UFUNCTION(BlueprintCallable)
	void SetFOV(float NewFOV);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetFOV() { return CameraComponent->FieldOfView; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UCameraComponent* GetCameraComponent();
	
private:
	UPROPERTY()
	UCameraComponent* CameraComponent;

};
