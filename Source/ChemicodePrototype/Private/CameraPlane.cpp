// copyright lolol

#include "CameraPlane.h"

#include "Camera/CameraComponent.h"
#include "ChemicodePrototype/ChemicodePrototype.h"

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

AActor* ACameraPlane::GetCamPositionActor()
{
	return CamPosition;
}

AActor* ACameraPlane::GetCamPositionActorChecked()
{
	UE_LOG(LogChemicode, Log, TEXT("At GetCamPositionChecked"));
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
	UE_LOG(LogChemicode, Log, TEXT("At InitialiseCamPlane"));
	// Thanks to https://forums.unrealengine.com/t/accessing-vertex-positions-of-static-mesh/5715/2
	if(!IsValidLowLevel()) return;
	if(!StaticMeshComponent) return;
	if(!StaticMeshComponent->GetStaticMesh()) return;
	UE_LOG(LogChemicode, Log, TEXT("Getting RenderData"));
	FStaticMeshRenderData* RenderData = StaticMeshComponent->GetStaticMesh()->GetRenderData();
	if(!RenderData) return;

	if (RenderData->LODResources.Num() > 0)
	{
		UE_LOG(LogChemicode, Log, TEXT("In RenderData loop"));
		FPositionVertexBuffer& VertexBuffer = RenderData->LODResources[0].VertexBuffers.PositionVertexBuffer;
		PlanePoints.Empty();
		PlanePoints.Reserve(4);

		check(VertexBuffer.GetNumVertices() == 4);
		for (size_t i = 0; i < VertexBuffer.GetNumVertices(); i++)
			PlanePoints.Add(GetActorLocation() + GetTransform().TransformVector(UE::Math::TVector<double>(VertexBuffer.VertexPosition(i))));

		ClampMin = FVector(FGenericPlatformMath::Min(PlanePoints[1].X, PlanePoints[2].X),
			FGenericPlatformMath::Min(PlanePoints[1].Y, PlanePoints[2].Y),
			FGenericPlatformMath::Min(PlanePoints[1].Z, PlanePoints[2].Z));
		ClampMax = FVector(FGenericPlatformMath::Max(PlanePoints[1].X, PlanePoints[2].X),
			FGenericPlatformMath::Max(PlanePoints[1].Y, PlanePoints[2].Y),
			FGenericPlatformMath::Max(PlanePoints[1].Z, PlanePoints[2].Z));
		
		// PlaneNormal = (PlanePoints[1] - PlanePoints[0]).Cross(PlanePoints[2] - PlanePoints[0]).GetSafeNormal();

		Forward = GetActorForwardVector();
		Right   = GetActorRightVector();
		Up      = GetActorUpVector();
	}

	// Spawn camera position actor
	UE_LOG(LogChemicode, Log, TEXT("Creating Camera Plane Pos Actor"));
	CamPosition = GetWorld()->SpawnActor<ACameraPlaneCam>(GetActorLocation(), GetTargetCameraRotation());
	CamPosition->SetFOV(130);
}
