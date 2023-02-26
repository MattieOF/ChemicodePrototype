// copyright lolol

#include "BunsenBurner.h"

#include "ChemicodePawn.h"
#include "ChemicodeStatics.h"
#include "ResourceTube.h"
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
	bResourceLike = false;
	Name = FText::FromString("Bunsen Burner");
	Description = FText::FromString("Burn baby burn\nRequires gas for fuel, attach the cable to a gas tap");
}

void ABunsenBurner::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bShouldRemoveTarget)
	{
		ClearItem();
	}

	const bool bHasGas = HasGas();
	if (bHasGas != bHadGasLastFrame)
		OnStateUpdated(State);
	bHadGasLastFrame = bHasGas;

	UE_LOG(LogChemicode, Log, TEXT("Ticking burner..."));
	if (TargetItem)
	{
		UE_LOG(LogChemicode, Log, TEXT("Has target item!"));
		if (!bSimulated)
			TargetItem->SetActorLocation( GetActorLocation() + ItemOffset + FVector( 0, 0, UChemicodeStatics::GetZUnderOrigin(TargetItem) ) );
		if (ConnectedGasTap != nullptr)
			UE_LOG(LogChemicode, Log, TEXT("Connected to gas tap!"));
		if (bHasGas)
		{
			UE_LOG(LogChemicode, Log, TEXT("Has gas!"));
			TargetItem->FireTick(this, DeltaSeconds);
		}
	}
}

void ABunsenBurner::ConnectToGasTap(AGasTap* GasTap)
{
	if (ConnectedGasTap)
		ConnectedGasTap->Disconnect();
	ConnectedGasTap = GasTap;
	if (GasTap)
	{
		if (!bSimulated)
		{
			Cable->EndLocation = FVector::ZeroVector;
			Cable->SetAttachEndTo(GasTap, "MainMesh", "Tap");
			Cable->bAttachEnd = true;
		}
		ConnectedGasTap->Connect();
	} else
	{
		if (!bSimulated)
		{
			Cable->EndLocation = FVector(100, 0, 0);
			Cable->bAttachEnd = false;
		}
	}
}

void ABunsenBurner::SetState(EBunsenBurnerState NewState)
{
	State = NewState;
	OnStateUpdated(NewState);
}

void ABunsenBurner::ClearItem()
{
	bShouldRemoveTarget = false;
	
	if (!TargetItem)
		return;
	
	TargetItem->OnItemPickedUp.Remove(TargetItemDelegateHandle);
	TargetItemDelegateHandle.Reset();
	TargetItem = nullptr;
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
	
	if (OtherObject->bResourceLike)
	{
		// We can use this item, put it where it needs to be
		TargetItem = OtherObject;
		TargetItemDelegateHandle = TargetItem->OnItemPickedUp.AddLambda([this]
		{
			bShouldRemoveTarget = true;
		});
		if (!bSimulated)
		{
			TargetItem->SetActorLocation( GetActorLocation() + ItemOffset + FVector( 0, 0, UChemicodeStatics::GetZUnderOrigin(TargetItem) ) );
			UChemicodeStatics::GetChemicodePawn(WorldRef ? WorldRef : GetWorld())->DropItem();	
		}
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
}
