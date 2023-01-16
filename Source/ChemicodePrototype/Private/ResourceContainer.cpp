// copyright lolol

#include "ResourceContainer.h"

#include "ChemicodeStatics.h"

float AResourceContainer::GetTotalAmount()
{
	float Sum = 0;
	for (const TPair<UResourceData*, FResourceMeasurement>& Res : Contents)
		Sum += UChemicodeStatics::MeasurementAsMinimumUnit(Res.Value);
	return Sum;
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
	if (!UChemicodeStatics::MeasurementIsSameType(Contents.begin().Value(), Amount))
		return false;

	if (!HasResource(Res))
		Contents.Add(Res, Amount);
	else
		Contents[Res] += Amount;
	
	return true;
}

bool AResourceContainer::RemoveResource(UResourceData* Res, FResourceMeasurement Amount)
{
	if (!HasAtLeastAmountOfResource(Res, Amount))
		return false;
	
	Contents[Res] -= Amount;
	
	if (UChemicodeStatics::MeasurementAsMinimumUnit(Contents[Res]) <= 0)
		Contents.Remove(Res);
	
	return true;
}

bool AResourceContainer::Transfer(AResourceContainer* Target, UResourceData* Res, FResourceMeasurement Amount)
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
