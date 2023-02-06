// copyright lolol

#include "ResourceProperty.h"

#include "ChemicodeStatics.h"

FString FDecimalResourceProperty::ToString()
{
	FString ValueString = FString::Printf(TEXT("%.2f"), Value);
	UChemicodeStatics::TrimTrailingZeros(ValueString);
	return ValueString;
}
