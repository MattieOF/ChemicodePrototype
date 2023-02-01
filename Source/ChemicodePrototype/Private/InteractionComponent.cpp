// copyright lolol

#include "InteractionComponent.h"

#include "ChemicodePawn.h"
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

		// This code is commented as it seems to be a bit broken. Params size is 9 but sizeof returns 16.
		// if (Function->ParmsSize != sizeof(FInteractionFuncParams))
		// {
		// 	UE_LOG(LogChemicode, Error, TEXT("Function %s for interaction with %s has incorrect parameter types. The parameters total %d bytes, but it should be %d bytes. The parameters should be an input of type AResourceItem (the other item in the interaction), and a return of type bool (representing success)."),
		// 		   *FuncName.ToString(), *Item->Resource->Name.ToString(), Function->ParmsSize, sizeof(FInteractionFuncParams));
		// 	UChemicodeStatics::DebugErrorNotification(
		// 		GetWorld(), "Invalid function provided in interaction map",
		// 		"Incorrect parameter types. Check output log for more info");
		// 	return;
		// }

		FInteractionFuncParams Params;
		Params.Item = Item;
		ProcessEvent(Function, &Params);
		const auto Interaction = GetInteractionWith(Item);
		if (Params.bOutSuccess && Interaction.bIsValid && Interaction.Time > 0)
			BeginLatentInteraction(Interaction.Time);
	}
}

void UInteractionComponent::FireTick_Implementation(AChemicodeObject* Source)
{
	if (OwnerAsResourceItem)
	{
		OwnerAsResourceItem->SetMeasurement(OwnerAsResourceItem->Measurement - FResourceMeasurement(
			OwnerMeasurementUnit, BurnRate * GetWorld()->DeltaTimeSeconds));
	}
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerItem = Cast<AChemicodeObject>(GetOwner());
	OwnerAsResourceItem = Cast<AResourceItem>(GetOwner());

	if (OwnerAsResourceItem)
		OwnerMeasurementUnit = OwnerAsResourceItem->Measurement.Unit;
}

void UInteractionComponent::BeginLatentInteraction(float Length)
{
	auto Pawn = UChemicodeStatics::GetChemicodePawn(GetWorld());
	Pawn->DisableInteraction();

	FTimerDelegate TimerDelegate;
	// Using the bind UFunction method doesn't work, for some reason. I have no idea why. So we use a lambda instead.
	// TimerDelegate.BindUFunction(this, TEXT("EndLatentInteraction"));
	TimerDelegate.BindLambda([this] { EndLatentInteraction(); });
	GetWorld()->GetTimerManager().SetTimer(Pawn->CurrentInteractionTimer, TimerDelegate, Length, false, -1);
}

void UInteractionComponent::EndLatentInteraction()
{
	UChemicodeStatics::GetChemicodePawn(GetWorld())->EnableInteraction();
	GetWorld()->GetTimerManager().ClearTimer(UChemicodeStatics::GetChemicodePawn(GetWorld())->CurrentInteractionTimer);
}

bool UInteractionComponent::CanDepositInto_Implementation(AResourceContainer* Container)
{
	return true;
}
