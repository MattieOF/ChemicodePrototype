// copyright lolol

#include "ResourceContainer.h"

#include "ResourceInstance.h"
#include "ResourceTube.h"
#include "ChemicodePrototype/ChemicodePrototype.h"

void AResourceContainer::BeginPlay()
{
	Super::BeginPlay();
	Initialise();
}

void AResourceContainer::Initialise()
{
	// Add resources from initial contents array to container
	for (const auto& Element : InitialContents)
		AddResource(Element.Resource, Element.Amount);
	InitialContents.Empty(); // Clear it after to save a tiny bit of memory
}

void AResourceContainer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bShouldClearConnectedTube)
	{
		ConnectedTube->OnItemPickedUp.Remove(TubeConnectionHandle);
		TubeConnectionHandle.Reset();
		ConnectedTube = nullptr;
		bShouldClearConnectedTube = false;
	}
	
	// Diffusion
	if (!ConnectedTube)
	{
		for (auto& Element : Contents)
		{
			bool Success;
			const double DiffusionRate = Element->GetDecimalProperty("Diffusion Rate", Success);
			if (DiffusionRate > 0)
				Element->Measurement -= FResourceMeasurement(Element->Measurement.Unit, DiffusionRate * DeltaSeconds);
		}
	}
}

float AResourceContainer::GetTotalAmount()
{
	if (!bDirty)
		return TotalAmount;
	
	float Sum = 0;
	for (const auto Element : Contents)
		Sum += Element->Measurement.Value;
	bDirty = false;
	TotalAmount = Sum;
	return Sum;
}

UResourceInstance* AResourceContainer::GetResource(UResourceData* Res)
{
	for(const auto Element : Contents)
	{
		if (Element->Data == Res)
		{
			if (Element->Measurement.Value > 0)
				return Element;
			else
			{
				Contents.Remove(Element);
				return nullptr;	
			}
		}
	}

	return nullptr;
}

float AResourceContainer::ResourceProportion(UResourceData* Res)
{
	const UResourceInstance* Resource = GetResource(Res);
	if (!Resource) return 0;
	
	return static_cast<float>(Resource->Measurement.Value) / GetTotalAmount();
}

bool AResourceContainer::HasProportionOfResource(UResourceData* Res, float Percentage)
{
	const UResourceInstance* Resource = GetResource(Res);
	if (!Resource) return false;
	return (static_cast<float>(Resource->Measurement.Value) / GetTotalAmount()) == Percentage;
}

bool AResourceContainer::HasProportionOfResourceRange(UResourceData* Res, float PercentageMin, float PercentageMax)
{
	const UResourceInstance* Resource = GetResource(Res);
	if (!Resource) return false;
	const auto Percentage = static_cast<float>(Resource->Measurement.Value) / GetTotalAmount();
	return Percentage >= PercentageMin && Percentage <= PercentageMax;
}

bool AResourceContainer::AddResource(UResourceData* Res, FResourceMeasurement Amount)
{
	UResourceInstance* Resource = GetResource(Res);
	
	if (Resource)
		Resource->Measurement += Amount;
	else
	{
		Resource = NewObject<UResourceInstance>(this, FName(*Res->Name.ToString()));
		Resource->SetResourceData(Res);
		Resource->Measurement = Amount;
		Contents.Add(Resource);
	}

	if (Resource->Measurement.Value <= 0)
		Contents.Remove(Resource);
	
	bDirty = true;
	
	return true;
}

bool AResourceContainer::RemoveResource(UResourceData* Res, FResourceMeasurement Amount)
{
	UResourceInstance* Resource = GetResource(Res);
	if (!Resource) return false;
	
	Resource->Measurement -= Amount;
	
	if (Resource->Measurement.Value <= 0)
		Contents.Remove(Resource);
		
	bDirty = true;
	
	return true;
}

bool AResourceContainer::TransferFromItem(AResourceItem* Source, float Amount)
{
	if (!Source)
	{
		UE_LOG(LogChemicode, Error, TEXT("Source is null in AResourceContainer::TransferFromItem!"));
		return false;
	}
	
	// if (Contents.Num() > 0 && !UChemicodeStatics::MeasurementIsSameType(Contents.begin().Value(), Source->Measurement))
	// 	return false;
	
	if (Source->Resource->Measurement.Value < Amount)
		return false;

	Source->SetMeasurement(FResourceMeasurement(Source->Resource->Measurement.Unit, Source->Resource->Measurement.Value - Amount));
	AddResource(Source->Resource->Data, FResourceMeasurement(Source->Resource->Measurement.Unit, Amount));
	return true;
}

bool AResourceContainer::ReplaceResources(TArray<UResourceData*> ResourcesToReplace, UResourceData* NewResource)
{
	float Sum = 0;

	for (const auto& Item : ResourcesToReplace)
	{
		if (UResourceInstance* Resource = GetResource(Item))
		{
			Sum += Resource->Measurement.Value;
			Contents.Remove(Resource);
		}
	}

	if (Sum > 0)
	{
		AddResource(NewResource, FResourceMeasurement(NewResource->BaseMeasurement.Unit, Sum));
		return true;
	}
	
	return false;
}

bool AResourceContainer::ReplaceResource(UResourceData* Resource, UResourceData* NewResource,
	FResourceMeasurement Amount, float Scale)
{
	UE_LOG(LogChemicode, Log, TEXT("Replace %lld of %s with %s (scale %f)"), Amount.Value, *Resource->Name.ToString(), *NewResource->Name.ToString(), Scale);
	
	const UResourceInstance* ResourceInst = GetResource(Resource);
	if (!ResourceInst) return false;

	RemoveResource(Resource, Amount);

	Amount.Value *= Scale;
	AddResource(NewResource, Amount);

	return true;
}

bool AResourceContainer::SatisfiesInteraction(FContainerInteraction Interaction)
{
	for (const auto& Condition : Interaction.Conditions)
	{
		if (!SatisfiesCondition(Condition))
			return false;
	}

	return true;
}

bool AResourceContainer::AttemptInteraction()
{
	for (const auto& Interaction : Interactions)
	{
		if (SatisfiesInteraction(Interaction))
		{
			if (const auto Function = FindFunction(Interaction.FunctionName))
				ProcessEvent(Function, nullptr);
			else
				UE_LOG(LogChemicode, Error, TEXT("Function with name %s not found!"), *Interaction.FunctionName.ToString());
		}
	}

	return false;
}

bool AResourceContainer::Use()
{
	return AttemptInteraction();
}

bool AResourceContainer::InteractWith(AChemicodeObject* OtherObject)
{
	if (AResourceTube* Tube = Cast<AResourceTube>(OtherObject))
	{
		if (ConnectedTube != nullptr)
			return false;
		Tube->ConnectObject(this);
		TubeConnectionHandle = Tube->OnItemPickedUp.AddLambda([this]
		{
			bShouldClearConnectedTube = true;
		});
		ConnectedTube = Tube;
		return true;
	}

	return false;
}

void AResourceContainer::FireTick(AChemicodeObject* Source)
{
	OnFireTick(Source);
}

void AResourceContainer::ReceiveResource(UResourceData* Resource, FResourceMeasurement Amount)
{
	if (!OnReceiveResource(Resource, Amount))
	{
		if (Amount.Value > 0)
			AddResource(Resource, Amount);
		else if (Amount.Value < 0)
		{
			Amount.Value = -Amount.Value;
			RemoveResource(Resource, Amount);
		}
	}
}

bool AResourceContainer::TransferToContainer(AResourceContainer* Target, UResourceData* Res, FResourceMeasurement Amount)
{
	if (!HasAtLeastAmountOfResource(Res, Amount))
		return false;

	// TODO: Check target can contain amount
	if (Target->AddResource(Res, Amount))
		RemoveResource(Res, Amount);
	else
		return false;

	return true;
}
