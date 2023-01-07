// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraPlane.generated.h"

class ACameraPlaneCam;

UCLASS(ClassGroup=(Chemicode), Blueprintable)
class CHEMICODEPROTOTYPE_API ACameraPlane : public AActor
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief Init actor values and components
	 */
	ACameraPlane();

protected:
	/**
	 * @brief Init the CamPlane
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * @brief Move along the CamPlane. This moves the object returned by GetCamPositionActor().
	 * @param HorizontalMovement Float representing horizontal movement along the plane
	 * @param VerticalMovement Float representing vertical movement along the plane
	 * @return Position on plane after the movement
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera Plane")
	FVector MoveOnPlane(float HorizontalMovement, float VerticalMovement);

	/**
	 * @return Rotation at which the camera on the plane should be facing
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera Plane", BlueprintPure)
	FRotator GetTargetCameraRotation();

	/**
	 * @return Reference to the ACameraPlaneCam actor used by the plane to represent the current position
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera Plane", BlueprintPure)
	ACameraPlaneCam* GetCamPositionActor();

	/**
	 * @brief Returns the CamPlaneCam, and also checks it's not null. If it is, it calls the initialise function.
	 * @return Reference to the ACameraPlaneCam actor used by the plane to represent the current position
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera Plane")
	ACameraPlaneCam* GetCamPositionActorChecked();

	/**
	 * @brief Reset the camera position back to the center of the CamPlane
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera Plane")
	void ResetCameraPosition();

	/**
	 * @brief Mesh component used to represent the plane
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;

private:
	/**
	 * @brief Initialise CamPlane; fill in variables such as Forward vector.
	 */
	void InitialiseCamPlane();

	/**
	 * @brief Cam position actor. Holds the position of the camera, FOV, etc. To be used with SetViewTarget.
	 */
	UPROPERTY()
	ACameraPlaneCam* CamPosition;

	/**
	 * @brief Array containing the points of the plane
	 */
	TArray<FVector> PlanePoints;

	// Vectors holding direction information and what the position should be clamped between
	FVector Forward, Right, Up;
	FVector ClampMin, ClampMax;
};
