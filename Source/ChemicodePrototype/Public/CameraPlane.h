// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "CameraPlaneCam.h"
#include "GameFramework/Actor.h"
#include "CameraPlane.generated.h"

UCLASS(ClassGroup=(Chemicode), Blueprintable)
class CHEMICODEPROTOTYPE_API ACameraPlane : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraPlane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Camera Plane")
	FVector MoveOnPlane(float HorizontalMovement, float VerticalMovement);

	UFUNCTION(BlueprintCallable, Category = "Camera Plane", BlueprintPure)
	FRotator GetTargetCameraRotation();
	
	UFUNCTION(BlueprintCallable, Category = "Camera Plane", BlueprintPure)
	AActor* GetCamPositionActor();
	
	UFUNCTION(BlueprintCallable, Category = "Camera Plane")
	AActor* GetCamPositionActorChecked();
	
	UFUNCTION(BlueprintCallable, Category = "Camera Plane")
	void ResetCameraPosition();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;

private:
	void InitialiseCamPlane();

	UPROPERTY()
	ACameraPlaneCam* CamPosition;

	TArray<FVector> PlanePoints;
	
	FVector Forward, Right, Up;
	FVector ClampMin, ClampMax;
};
