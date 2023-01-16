// copyright lolol

#include "ResourceContainer.h"

#include "ChemicodeStatics.h"

float AResourceContainer::GetTotalAmount()
{
	float Sum = 0;
	for (const TPair<UResourceData*, FResourceMeasurement>& Resource : Contents)
		Sum += UChemicodeStatics::MeasurementAsMinimumUnit(Resource.Value);
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

	return false;
}

bool AResourceContainer::RemoveResource(UResourceData* Res, FResourceMeasurement Amount)
{
	if (!HasAtLeastAmountOfResource(Res, Amount))
		return false;
	
	return false;
}

bool AResourceContainer::Transfer(AResourceContainer* Target, UResourceData* Res, FResourceMeasurement Amount)
{
	if (!HasAtLeastAmountOfResource(Res, Amount))
		return false;

	return true;
}
