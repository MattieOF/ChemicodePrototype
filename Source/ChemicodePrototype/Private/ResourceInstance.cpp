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

UResourceProperty* UResourceInstance::GetProperty(FName Name)
{
	for (auto Element : Properties)
	{
		if (Element->GetFName() == Name)
			return Element;
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
		UDecimalResourceProperty* DecimalProperty = NewObject<UDecimalResourceProperty>(this, Name);
		DecimalProperty->Value = Value;
		Properties.Add(DecimalProperty);
		return true;
	} else
	{
		if (UDecimalResourceProperty* DecimalProperty = Cast<UDecimalResourceProperty>(Property))
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
		UStringResourceProperty* StringProperty = NewObject<UStringResourceProperty>(this, Name);
		StringProperty->Value = Value;
		Properties.Add(StringProperty);
		return true;
	} else
	{
		if (UStringResourceProperty* StringProperty = Cast<UStringResourceProperty>(Property))
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
	const auto DecimalProperty = Cast<UDecimalResourceProperty>(Property);
	if (!DecimalProperty) return DefaultValue;
	return DecimalProperty->Value;
}

FString UResourceInstance::GetStringProperty(FName Name, bool& bSuccess, FString DefaultValue)
{
	const auto Property = GetProperty(Name);
	if (!Property) return DefaultValue;
	const auto StringProperty = Cast<UStringResourceProperty>(Property);
	if (!StringProperty) return DefaultValue;
	return StringProperty->Value;
}
