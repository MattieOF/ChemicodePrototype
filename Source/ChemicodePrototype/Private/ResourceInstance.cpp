// copyright lolol

#include "ResourceInstance.h"

#include "ChemicodeStatics.h"
#include "ChemicodePrototype/ChemicodePrototype.h"

void UResourceInstance::SetResourceData(UResourceData* NewData, bool bPreserveProperties, bool bOverwriteMeasurement)
{
	Data = NewData;
	if (bOverwriteMeasurement)
		Measurement = Data->BaseMeasurement;
	if (!bPreserveProperties)
		Properties.Empty();
	for (auto& Element : UChemicodeStatics::GetChemicodeGameMode(GetWorld())->GlobalDefaultProperties)
		AddDefaultResourceProperty(Element);
	for (auto& Element : Data->DefaultProperties)
		AddDefaultResourceProperty(Element);
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

bool UResourceInstance::SetBoolProperty(FName Name, bool Value)
{
	const auto Property = GetProperty(Name);
	if (!Property)
	{
		UBoolResourceProperty* BoolProperty = NewObject<UBoolResourceProperty>(this, Name);
		BoolProperty->Value = Value;
		Properties.Add(BoolProperty);
		return true;
	} else
	{
		if (UBoolResourceProperty* BoolProperty = Cast<UBoolResourceProperty>(Property))
		{
			BoolProperty->Value = Value;
			return true;
		} else return false;
	}
}

double UResourceInstance::GetDecimalProperty(FName Name, bool& bSuccess, double DefaultValue)
{
	const auto Property = GetProperty(Name);
	if (!Property)
	{
		bSuccess = false;
		return DefaultValue;
	};
	
	const auto DecimalProperty = Cast<UDecimalResourceProperty>(Property);
	if (!DecimalProperty)
	{
		bSuccess = false;
		return DefaultValue;
	};
	
	bSuccess = true;
	return DecimalProperty->Value;
}

FString UResourceInstance::GetStringProperty(FName Name, bool& bSuccess, FString DefaultValue)
{
	const auto Property = GetProperty(Name);
	if (!Property)
	{
		bSuccess = false;
		return DefaultValue;
	};
	
	const auto StringProperty = Cast<UStringResourceProperty>(Property);
	if (!StringProperty)
	{
		bSuccess = false;
		return DefaultValue;
	};
	
	bSuccess = true;
	return StringProperty->Value;
}

bool UResourceInstance::GetBoolProperty(FName Name, bool& bSuccess, bool DefaultValue)
{
	const auto Property = GetProperty(Name);
	if (!Property)
	{
		bSuccess = false;
		return DefaultValue;
	};
	
	const auto BoolProperty = Cast<UBoolResourceProperty>(Property);
	if (!BoolProperty)
	{
		bSuccess = false;
		return DefaultValue;
	};
	
	bSuccess = true;
	return BoolProperty->Value;
}

void UResourceInstance::AddDefaultResourceProperty(const FDefaultResourceProperty& Property)
{
	switch (Property.PropertyType)
	{
	case RPTDecimal:
		SetDecimalProperty(Property.Name, Property.DecimalValue);
		break;
	case RPTString:
		SetStringProperty(Property.Name, Property.StringValue);
		break;
	case RPTBool:
		SetBoolProperty(Property.Name, Property.BoolValue);
		break;
	default:
		UE_LOG(LogChemicode, Error, TEXT("Invalid resource property type (%i) in Resource %s default properties (property: %s)"),
			static_cast<int>(Property.PropertyType), *Data->Name.ToString(), *Property.Name.ToString());
		break;
	}

	SetPropertyHidden(Property.Name, Property.bHiddenInUI);
}
