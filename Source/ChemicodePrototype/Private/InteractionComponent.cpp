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

		if (!Function)
		{
			UE_LOG(LogChemicode, Error, TEXT("Failed to find function %s for interaction with resource %s"),
			       *FuncName.ToString(), *Item->Resource->Name.ToString());
			UChemicodeStatics::DebugErrorNotification(
				GetWorld(), "Non-existent function provided in interaction map",
				"Check output log for more info");
			return;
		}

		if (Function->NumParms != 2)
		{
			UE_LOG(LogChemicode, Error, TEXT("Function %s for interaction with %s is invalid. It has %d parameters, but should have 2: an input of type AResourceItem (the other item in the interaction), and a return of type bool (representing success)."),
			       *FuncName.ToString(), *Item->Resource->Name.ToString(), Function->NumParms);
			UChemicodeStatics::DebugErrorNotification(
				GetWorld(), "Invalid function provided in interaction map",
				"Incorrect amount of parameters. Check output log for more info");
			return;
		}

		if (Function->ParmsSize != sizeof(FInteractionFuncParams))
		{
			UE_LOG(LogChemicode, Error, TEXT("Function %s for interaction with %s has incorrect parameter types. The parameters total %d bytes, but it should be %d bytes. The parameters should be an input of type AResourceItem (the other item in the interaction), and a return of type bool (representing success)."),
				   *FuncName.ToString(), *Item->Resource->Name.ToString(), Function->ParmsSize, sizeof(FInteractionFuncParams));
			UChemicodeStatics::DebugErrorNotification(
				GetWorld(), "Invalid function provided in interaction map",
				"Incorrect parameter types. Check output log for more info");
			return;
		}

		FInteractionFuncParams Params;
		Params.Item = Item;
		ProcessEvent(Function, &Params);
		const auto Interaction = GetInteractionWith(Item->Resource);
		if (Params.OutSuccess && Interaction.bIsValid && Interaction.Time > 0)
			BeginLatentInteraction();
	}
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerItem = Cast<AResourceItem>(GetOwner());
	if (!OwnerItem)
		UE_LOG(LogChemicode, Error, TEXT("Interaction component on %s is not a child of an AResourceItem!"), *AActor::GetDebugName(GetOwner()));
}

void UInteractionComponent::BeginLatentInteraction()
{
	UChemicodeStatics::GetChemicodePawn(GetWorld())->DisableInteraction();
}

void UInteractionComponent::EndLatentInteraction()
{
	UChemicodeStatics::GetChemicodePawn(GetWorld())->EnableInteraction();
}
