// copyright lolol

#include "BunsenBurner.h"

#include "ChemicodePawn.h"
#include "ChemicodeStatics.h"
#include "ResourceItem.h"

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

	bool bHasGas = HasGas();
	if (bHasGas != bHadGasLastFrame)
		OnStateUpdated(State);
	bHadGasLastFrame = bHasGas;

	if (TargetItem)
		TargetItem->SetActorLocation( GetActorLocation() + ItemOffset + FVector( 0, 0, UChemicodeStatics::GetZUnderOrigin(TargetItem) ) );
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

bool ABunsenBurner::Use()
{
	State = static_cast<EBunsenBurnerState>((static_cast<int>(State) + 1) % 3);
	OnStateUpdated(State);
	return true;
}

bool ABunsenBurner::AltInteract()
{
	ConnectToGasTap(nullptr);
	return true;
}

bool ABunsenBurner::InteractWith(AChemicodeObject* OtherObject)
{
	if (TargetItem) // Do nothing if we already have an item
		return false;
	
	if (AResourceItem* Item = Cast<AResourceItem>(OtherObject))
	{
		// We can use this item, put it where it needs to be
		TargetItem = Item;
		TargetItemDelegateHandle = TargetItem->OnItemPickedUp.AddLambda([this]
		{
			TargetItem->OnItemPickedUp.Remove(TargetItemDelegateHandle);
			TargetItem = nullptr;
		});
		TargetItem->SetActorLocation( GetActorLocation() + ItemOffset + FVector( 0, 0, UChemicodeStatics::GetZUnderOrigin(TargetItem) ) );
		UChemicodeStatics::GetChemicodePawn(GetWorld())->DropItem();
		return true;
	}

	// Item was invalid, return
	return false;
}

void ABunsenBurner::GetActorBounds(bool bOnlyCollidingComponents, FVector& Origin, FVector& BoxExtent,
                                   bool bIncludeFromChildActors) const
{
	FBox Box(ForceInit);

	Box += MainMesh->Bounds.GetBox();
	
	Box.GetCenterAndExtents(Origin, BoxExtent);
	auto WorldPos = GetActorLocation();
}
