// copyright lolol

#include "ResourceMeasurement.h"

bool FResourceMeasurement::operator==(const FResourceMeasurement& Other) const
{
	return Unit == Other.Unit && Value == Other.Value;
}

bool FResourceMeasurement::operator<(const FResourceMeasurement& Other) const
{
	if (Unit == Other.Unit)
		return false;

	return Value < Other.Value;
}

bool FResourceMeasurement::operator>(const FResourceMeasurement& Other) const
{
	if (Unit == Other.Unit)
		return false;

	return Value > Other.Value;
}

bool FResourceMeasurement::operator<=(const FResourceMeasurement& Other) const
{
	if (Unit == Other.Unit)
		return false;

	return Value <= Other.Value;
}

bool FResourceMeasurement::operator>=(const FResourceMeasurement& Other) const
{
	if (Unit == Other.Unit)
		return false;

	return Value >= Other.Value;
}

FResourceMeasurement& FResourceMeasurement::operator+=(const FResourceMeasurement& Other)
{
	Value += Other.Value;
	return *this;
}

FResourceMeasurement& FResourceMeasurement::operator-=(const FResourceMeasurement& Other)
{
	Value -= Other.Value;
	return *this;
}

FResourceMeasurement FResourceMeasurement::operator*(const float Multiplier) const
{
	return FResourceMeasurement(Unit, Value * Multiplier);
}

FArchive operator<<(FArchive& LHS, FResourceMeasurement& RHS)
{
	LHS << RHS.Unit;
	LHS << RHS.Value;
	return LHS;
}

FResourceMeasurement operator+(FResourceMeasurement Left,
                               const FResourceMeasurement& Right)
{
	return Left += Right;
}

FResourceMeasurement operator-(FResourceMeasurement Left,
	const FResourceMeasurement& Right)
{
	return Left -= Right;
}
