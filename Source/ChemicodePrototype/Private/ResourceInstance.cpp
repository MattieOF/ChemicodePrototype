// copyright lolol

#include "ResourceInstance.h"

void UResourceInstance::SetResourceData(UResourceData* NewData, bool bOverwriteMeasurement)
{
	Data = NewData;
	if (bOverwriteMeasurement)
		Measurement = Data->BaseMeasurement;
}
