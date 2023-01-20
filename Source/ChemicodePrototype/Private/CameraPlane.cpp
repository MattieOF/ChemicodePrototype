// copyright lolol

#include "CameraPlane.h"

#include "CameraPlaneCam.h"

// Sets default values
ACameraPlane::ACameraPlane()
{
	PrimaryActorTick.bCanEverTick = false;

	// Setup plane mesh
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Plane(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	StaticMeshComponent->SetStaticMesh(Plane.Object);
	StaticMeshComponent->SetCastShadow(false);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetHiddenInGame(true);
	// StaticMeshComponent->GetStaticMesh()->bAllowCPUAccess = true;
}

// Called when the game starts or when spawned
void ACameraPlane::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(CamPosition))
		InitialiseCamPlane();
}

FVector ACameraPlane::MoveOnPlane(float HorizontalMovement, float VerticalMovement)
{
	FVector CurrentPosition = CamPosition->GetActorLocation();
	CurrentPosition += Forward * HorizontalMovement + Right * VerticalMovement;
	CurrentPosition = ClampVector(CurrentPosition,
		ClampMin, ClampMax);
	CamPosition->SetActorLocation(CurrentPosition);
	return CurrentPosition;
}

FRotator ACameraPlane::GetTargetCameraRotation()
{
	return (-Up).ToOrientationRotator();
}

ACameraPlaneCam* ACameraPlane::GetCamPositionActor()
{
	return CamPosition;
}

ACameraPlaneCam* ACameraPlane::GetCamPositionActorChecked()
{
	if (!IsValid(CamPosition))
		InitialiseCamPlane();
	return CamPosition;
}

void ACameraPlane::ResetCameraPosition()
{
	CamPosition->SetActorLocation(GetActorLocation());
}

void ACameraPlane::InitialiseCamPlane()
{
	if(!IsValidLowLevel()) return;
	
	Forward = GetActorForwardVector();
	Right   = GetActorRightVector();
	Up      = GetActorUpVector();

	FVector Origin, Extents;
	GetActorBounds(false, Origin, Extents);
	ClampMin = Origin - Extents;
	ClampMax = Origin + Extents;
	
	// Spawn camera position actor
	CamPosition = GetWorld()->SpawnActor<ACameraPlaneCam>(GetActorLocation(), GetTargetCameraRotation());
	CamPosition->SetFOV(130);
}
