// copyright lolol

#include "InteractionComponent.h"

#include "ResourceItem.h"
#include "ChemicodePrototype/ChemicodePrototype.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerItem = Cast<AResourceItem>(GetOwner());
	if (!OwnerItem)
		UE_LOG(LogChemicode, Error, TEXT("Interaction component on %s is not a child of an AResourceItem!"), *AActor::GetDebugName(GetOwner()));
}
