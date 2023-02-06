// copyright lolol

#include "ResourceInstance.h"

void UResourceInstance::SetResourceData(UResourceData* NewData, bool bOverwriteMeasurement)
{
	Data = NewData;
	if (bOverwriteMeasurement)
		Measurement = Data->BaseMeasurement;
}

FResourceProperty* UResourceInstance::GetProperty(FName Name)
{
	for (auto Element : Properties)
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

void UResourceInstance::SetDoubleProperty(const FName Name, const double Value)
{
	
}

void UResourceInstance::SetStringProperty(FName Name, FString Value)
{
	
}

double UResourceInstance::GetDoubleProperty(FName Name, double DefaultValue)
{
	return 0;
}

FString UResourceInstance::GetStringProperty(FName Name, FString DefaultValue)
{
	return "";
}
