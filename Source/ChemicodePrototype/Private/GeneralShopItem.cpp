// copyright lolol

#include "GeneralShopItem.h"

#include "ChemicodePawn.h"
#include "ChemicodeStatics.h"

// Sets default values
AGeneralShopItem::AGeneralShopItem()
{
	PrimaryActorTick.bCanEverTick = false;

	bHoldable = false;
	bResourceLike = false;
	bNonTable = true;
}

bool AGeneralShopItem::Interact()
{
	UChemicodeStatics::GetChemicodePawn(GetWorld())->HoldNewItem(ObjectClass);
	return true;
}
