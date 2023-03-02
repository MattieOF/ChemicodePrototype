// copyright lolol

#include "GodButton.h"

#include "ChemicodeStatics.h"

// Sets default values
AGodButton::AGodButton()
{
	PrimaryActorTick.bCanEverTick = false;

	bHoldable = false;
	bResourceLike = false;
	bNonTable = true;

	Name = FText::FromString("God Button");
	Description = FText::FromString("Pressing this button calls God");
}

bool AGodButton::Interact()
{
	if (Pedestal->Container == nullptr)
	{
		UChemicodeStatics::GetChemicodeGameMode(GetWorld())->AddNotification(FNotification(
			FText::FromString("No Submission"), FText::FromString("Place a test tube on the pedestal for God to judge"),
			3, NTError));
		return false;
	}
	
	DoSubmission();
	return true;
}
