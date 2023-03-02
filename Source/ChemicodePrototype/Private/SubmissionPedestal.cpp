// copyright lolol

#include "SubmissionPedestal.h"

#include "ChemicodePawn.h"
#include "ChemicodeStatics.h"

// Sets default values
ASubmissionPedestal::ASubmissionPedestal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bResourceLike = false;
	bHoldable = false;
	bNonTable = true;

	Name = FText::FromString("Submission Pedestal");
	Description = FText::FromString("God will judge what you place on here");
}

void ASubmissionPedestal::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bShouldRemoveContainer)
	{
		bShouldRemoveContainer = false;
		if (!Container)
			return;
		Container->OnItemPickedUp.Remove(ContainerCallbackHandle);
		ContainerCallbackHandle.Reset();
		Container = nullptr;
	}
}

bool ASubmissionPedestal::InteractWith(AChemicodeObject* OtherObject)
{
	if (Container || !bEnabled) // Do nothing if we already have an item
		return false;
	
	if (AResourceContainer* NewContainer = Cast<AResourceContainer>(OtherObject))
	{
		// We can use this item, put it where it needs to be
		Container = NewContainer;
		ContainerCallbackHandle = Container->OnItemPickedUp.AddLambda([this]
		{
			bShouldRemoveContainer = true;
		});
		Container->SetActorLocation( GetActorLocation() + PlacementOffset + FVector( 0, 0, UChemicodeStatics::GetZUnderOrigin(Container) ) );
		UChemicodeStatics::GetChemicodePawn(GetWorld())->DropItem();
		return true;
	} else
	{
		UChemicodeStatics::GetChemicodeGameMode(GetWorld())->AddNotification(FNotification(
			FText::FromString("Not a container"), FText::FromString("God can only judge test tubes"),
			3, NTError));
		return false;
	}

	// Item was invalid, return
	return false;
}
