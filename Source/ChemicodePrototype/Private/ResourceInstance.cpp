﻿// copyright lolol

#include "ResourceInstance.h"

void UResourceInstance::SetResourceData(UResourceData* NewData, bool bOverwriteMeasurement)
{
	Data = NewData;
	if (bOverwriteMeasurement)
		Measurement = Data->BaseMeasurement;
}

FResourceProperty* UResourceInstance::GetProperty(FName Name)
{
	for (auto& Element : Properties)
	{
		if (Element.PropertyName == Name)
			return &Element;
	}

	return nullptr;
}

void UResourceInstance::SetPropertyHidden(const FName Name, const bool bNewHidden)
{
	if (auto Property = GetProperty(Name))
		Property->bHiddenInUI = bNewHidden;
}

bool UResourceInstance::SetDecimalProperty(const FName Name, const double Value)
{
	const auto Property = GetProperty(Name);
	if (!Property)
	{
		FDecimalResourceProperty DecimalProperty;
		DecimalProperty.Value = Value;
		Properties.Add(DecimalProperty);
		return true;
	} else
	{
		if (FDecimalResourceProperty* DecimalProperty = dynamic_cast<FDecimalResourceProperty*>(Property))
		{
			DecimalProperty->Value = Value;
			return true;
		} else return false;
	}
}

bool UResourceInstance::SetStringProperty(FName Name, FString Value)
{
	const auto Property = GetProperty(Name);
	if (!Property)
	{
		FStringResourceProperty StringProperty;
		StringProperty.Value = Value;
		Properties.Add(StringProperty);
		return true;
	} else
	{
		if (FStringResourceProperty* StringProperty = dynamic_cast<FStringResourceProperty*>(Property))
		{
			StringProperty->Value = Value;
			return true;
		} else return false;
	}
}

double UResourceInstance::GetDecimalProperty(FName Name, double DefaultValue)
{
	const auto Property = GetProperty(Name);
	if (!Property) return DefaultValue;
	const auto DecimalProperty = dynamic_cast<FDecimalResourceProperty*>(Property);
	if (!DecimalProperty) return DefaultValue;
	return DecimalProperty->Value;
}

FString UResourceInstance::GetStringProperty(FName Name, FString DefaultValue)
{
	const auto Property = GetProperty(Name);
	if (!Property) return DefaultValue;
	const auto StringProperty = dynamic_cast<FStringResourceProperty*>(Property);
	if (!StringProperty) return DefaultValue;
	return StringProperty->Value;
}
