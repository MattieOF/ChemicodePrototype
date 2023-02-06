// copyright lolol

#include "ResourceInstance.h"

#include "ChemicodePrototype/ChemicodePrototype.h"

void UResourceInstance::SetResourceData(UResourceData* NewData, bool bPreserveMeasurements, bool bOverwriteMeasurement)
{
	Data = NewData;
	if (bOverwriteMeasurement)
		Measurement = Data->BaseMeasurement;
	if (!bPreserveMeasurements)
		Properties.Empty();
	for (auto& Element : Data->DefaultProperties)
	{
		switch (Element.PropertyType)
		{
		case RPTDecimal:
			SetDecimalProperty(Element.Name, Element.DecimalValue);
			break;
		case RPTString:
			SetStringProperty(Element.Name, Element.StringValue);
			break;
		default:
			UE_LOG(LogChemicode, Error, TEXT("Invalid resource property type (%i) in Resource %s default properties (property: %s)"),
				static_cast<int>(Element.PropertyType), *Data->Name.ToString(), *Element.Name.ToString());
			break;
		}
	}
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

double UResourceInstance::GetDecimalProperty(FName Name, bool& bSuccess, double DefaultValue)
{
	const auto Property = GetProperty(Name);
	if (!Property) return DefaultValue;
	const auto DecimalProperty = dynamic_cast<FDecimalResourceProperty*>(Property);
	if (!DecimalProperty) return DefaultValue;
	return DecimalProperty->Value;
}

FString UResourceInstance::GetStringProperty(FName Name, bool& bSuccess, FString DefaultValue)
{
	const auto Property = GetProperty(Name);
	if (!Property) return DefaultValue;
	const auto StringProperty = dynamic_cast<FStringResourceProperty*>(Property);
	if (!StringProperty) return DefaultValue;
	return StringProperty->Value;
}
