// copyright lolol

#include "InteractionComponent.h"

#include "ChemicodeStatics.h"
#include "ResourceData.h"
#include "ResourceItem.h"
#include "ChemicodePrototype/ChemicodePrototype.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionComponent::OnInteractWith_Implementation(AResourceItem* Item)
{
	if (ItemInteractions.Contains(Item->Resource))
	{
		const FName FuncName = ItemInteractions[Item->Resource];
		const auto Function = FindFunction(FuncName);
		if (!Function || Function->NumParms != 1)
		{
			UE_LOG(LogChemicode, Error, TEXT("Failed to find function %s for interaction with resource %s"),
			       *FuncName.ToString(), *Item->Resource->Name.ToString());
			UChemicodeStatics::DebugErrorNotification(
				GetWorld(), "Non-existent or invalid function provided in interaction map",
				"Check output log for more info");
			return;
		}
		ProcessEvent(Function, Item);
	}
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerItem = Cast<AResourceItem>(GetOwner());
	if (!OwnerItem)
		UE_LOG(LogChemicode, Error, TEXT("Interaction component on %s is not a child of an AResourceItem!"), *AActor::GetDebugName(GetOwner()));
}
