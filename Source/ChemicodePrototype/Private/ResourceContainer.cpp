// copyright lolol

#include "ResourceContainer.h"

#include "ChemicodeStatics.h"
#include "ChemicodePrototype/ChemicodePrototype.h"

float AResourceContainer::GetTotalAmount()
{
	if (!bDirty)
		return m_TotalAmount;
	
	float Sum = 0;
	for (const TPair<UResourceData*, FResourceMeasurement>& Res : Contents)
		Sum += UChemicodeStatics::MeasurementAsMinimumUnit(Res.Value);
	bDirty = false;
	m_TotalAmount = Sum;
	return Sum;
}

float AResourceContainer::ResourceProportion(UResourceData* Res)
{
	if (!HasResource(Res))
		return 0;

	return UChemicodeStatics::MeasurementAsMinimumUnit(Contents[Res]) / GetTotalAmount();
}

bool AResourceContainer::HasProportionOfResource(UResourceData* Res, float Percentage)
{
	return HasResource(Res) && (UChemicodeStatics::MeasurementAsMinimumUnit(Contents[Res]) / GetTotalAmount()) == Percentage;
}

bool AResourceContainer::HasProportionOfResourceRange(UResourceData* Res, float PercentageMin, float PercentageMax)
{
	if (!HasResource(Res))
		return false;

	const auto Percentage = UChemicodeStatics::MeasurementAsMinimumUnit(Contents[Res]) / GetTotalAmount();
	return Percentage >= PercentageMin && Percentage <= PercentageMax;
}

bool AResourceContainer::AddResource(UResourceData* Res, FResourceMeasurement Amount)
{
	if (Contents.Num() != 0 && !UChemicodeStatics::MeasurementIsSameType(Contents.begin().Value(), Amount))
		return false;

	if (!HasResource(Res))
		Contents.Add(Res, Amount);
	else
		Contents[Res] += Amount;

	UChemicodeStatics::UpdateMeasurementUnit(Contents[Res]);
	
	bDirty = true;
	
	return true;
}

bool AResourceContainer::RemoveResource(UResourceData* Res, FResourceMeasurement Amount)
{
	if (!HasAtLeastAmountOfResource(Res, Amount))
		return false;
	
	Contents[Res] -= Amount;
	
	UChemicodeStatics::UpdateMeasurementUnit(Contents[Res]);
	
	if (UChemicodeStatics::MeasurementAsMinimumUnit(Contents[Res]) <= 0)
		Contents.Remove(Res);
	
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
	
	const float SourceAmount = UChemicodeStatics::MeasurementAsMinimumUnit(Source->Measurement);
	if (SourceAmount < Amount)
		return false;

	const EMeasurementUnit SourceUnit = UChemicodeStatics::MinimumUnit(Source->Measurement.Unit);
	Source->SetMeasurement(FResourceMeasurement(SourceUnit, SourceAmount - Amount));
	AddResource(Source->Resource, FResourceMeasurement(SourceUnit, Amount));
	return true;
}

bool AResourceContainer::ReplaceResources(TArray<UResourceData*> ResourcesToReplace, UResourceData* NewResource)
{
	float Sum = 0;

	for (const auto& Item : ResourcesToReplace)
	{
		if (HasResource(Item))
		{
			Sum += UChemicodeStatics::MeasurementAsMinimumUnit(Contents[Item]);
			Contents.Remove(Item);
		}
	}

	if (Sum > 0)
	{
		AddResource(NewResource, FResourceMeasurement(UChemicodeStatics::MinimumUnit(NewResource->BaseMeasurement.Unit), Sum));
		return true;
	}
	
	return false;
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

bool AResourceContainer::Interact()
{
	return AttemptInteraction();
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
