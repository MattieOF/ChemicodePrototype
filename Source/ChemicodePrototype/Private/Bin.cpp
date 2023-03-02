// copyright lolol

#include "Bin.h"

#include "ChemicodePawn.h"
#include "ChemicodeStatics.h"

// Sets default values
ABin::ABin()
{
	PrimaryActorTick.bCanEverTick = false;

	bHoldable = false;
	bResourceLike = false;

	Name = FText::FromString("Bin");
	Description = FText::FromString("Used to trash resources, test tubes, bunsen burners, and children - potentially\nI think there's a black hole inside");
}

bool ABin::InteractWith(AChemicodeObject* OtherObject)
{
	UChemicodeStatics::GetChemicodePawn(GetWorld())->DropItem();
	OtherObject->Destroy();
	return true;
}
