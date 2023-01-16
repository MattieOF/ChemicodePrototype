// copyright lolol

#include "ResourceMeasurement.h"

#include "ChemicodeStatics.h"

bool FResourceMeasurement::operator==(const FResourceMeasurement& Other) const
{
	if (Unit == MULitres || Unit == MUMillilitres) // Litres
	{
		if (Other.Unit != MULitres && Other.Unit != MUMillilitres)
			return false;

		return UChemicodeStatics::MeasurementAsMinimumUnit(*this) == UChemicodeStatics::MeasurementAsMinimumUnit(Other);
	} else // Grams
	{
		if (Other.Unit != MUMilligrams && Other.Unit != MUGrams && Other.Unit != MUKilograms)
			return false;

		return UChemicodeStatics::MeasurementAsMinimumUnit(*this) == UChemicodeStatics::MeasurementAsMinimumUnit(Other);
	}
}

bool FResourceMeasurement::operator<(const FResourceMeasurement& Other) const
{
	if (!UChemicodeStatics::MeasurementIsSameType(*this, Other))
		return false;

	return UChemicodeStatics::MeasurementAsMinimumUnit(*this) < UChemicodeStatics::MeasurementAsMinimumUnit(Other);
}

bool FResourceMeasurement::operator>(const FResourceMeasurement& Other) const
{
	if (!UChemicodeStatics::MeasurementIsSameType(*this, Other))
		return false;

	return UChemicodeStatics::MeasurementAsMinimumUnit(*this) > UChemicodeStatics::MeasurementAsMinimumUnit(Other);
}

bool FResourceMeasurement::operator<=(const FResourceMeasurement& Other) const
{
	if (!UChemicodeStatics::MeasurementIsSameType(*this, Other))
		return false;

	return UChemicodeStatics::MeasurementAsMinimumUnit(*this) <= UChemicodeStatics::MeasurementAsMinimumUnit(Other);
}

bool FResourceMeasurement::operator>=(const FResourceMeasurement& Other) const
{
	if (!UChemicodeStatics::MeasurementIsSameType(*this, Other))
		return false;

	return UChemicodeStatics::MeasurementAsMinimumUnit(*this) >= UChemicodeStatics::MeasurementAsMinimumUnit(Other);
}

FResourceMeasurement& FResourceMeasurement::operator+=(const FResourceMeasurement& Other)
{
	Value += UChemicodeStatics::ConvertMeasurementType(Other.Value, Other.Unit, Unit);
	return *this;
}

FResourceMeasurement& FResourceMeasurement::operator-=(const FResourceMeasurement& Other)
{
	Value -= UChemicodeStatics::ConvertMeasurementType(Other.Value, Other.Unit, Unit);
	return *this;
}
