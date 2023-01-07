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
	/**
	 * @brief Initialise default object 
	 */
	ACameraPlaneCam();

	/**
	 * @brief Set the field of view of the camera.
	 * @param NewFOV New FOV
	 */
	UFUNCTION(BlueprintCallable)
	void SetFOV(float NewFOV);

	/**
	 * @return Field of view of the camera
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetFOV() const { return CameraComponent->FieldOfView; }

	/**
	 * @return Internal camera component
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; };
	
private:
	/**
	 * @brief Internal camera component
	 */
	UPROPERTY()
	UCameraComponent* CameraComponent;

};
