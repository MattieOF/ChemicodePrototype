// copyright lolol

#include "BunsenBurner.h"

#include "ChemicodePrototype/ChemicodePrototype.h"

ABunsenBurner::ABunsenBurner()
{
	PrimaryActorTick.bCanEverTick = true;

	CollarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collar"));
	CollarMesh->SetupAttachment(MainMesh);
	Cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	Cable->SetupAttachment(MainMesh);
	Cable->EndLocation = FVector(100, 0, 0);
	Cable->bEnableCollision = true;
	RootComponent = MainMesh;

	bDragInteraction = true;
	Name = FText::FromString("Bunsen Burner");
	Description = FText::FromString("Burn baby burn\nRequires gas for fuel, attach the cable to a gas tap");
}

void ABunsenBurner::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ConnectedGasTap && ConnectedGasTap->IsOpen())
	{
		
	} else SetState(BBSOff);
}

void ABunsenBurner::ConnectToGasTap(AGasTap* GasTap)
{
	if (ConnectedGasTap)
		ConnectedGasTap->Disconnect();
	ConnectedGasTap = GasTap;
	if (GasTap)
	{
		Cable->EndLocation = FVector::ZeroVector;
		Cable->SetAttachEndTo(GasTap, "MainMesh", "Tap");
		Cable->bAttachEnd = true;
		ConnectedGasTap->Connect();
	} else
	{
		Cable->EndLocation = FVector(100, 0, 0);
		Cable->bAttachEnd = false;
	}
}

void ABunsenBurner::SetState(EBunsenBurnerState NewState)
{
	State = NewState;
	OnStateUpdated(NewState);
}

void ABunsenBurner::GetActorBounds(bool bOnlyCollidingComponents, FVector& Origin, FVector& BoxExtent,
	bool bIncludeFromChildActors) const
{
	FBox Box(ForceInit);

	Box += MainMesh->Bounds.GetBox();
	
	Box.GetCenterAndExtents(Origin, BoxExtent);
	auto WorldPos = GetActorLocation();
}
