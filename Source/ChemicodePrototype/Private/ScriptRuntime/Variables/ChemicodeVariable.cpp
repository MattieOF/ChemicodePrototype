// copyright lolol

#include "ScriptRuntime/Variables/ChemicodeVariable.h"

#include "ChemicodePrototype/ChemicodePrototype.h"

void UChemicodeVariable::SerialiseVariable(FArchive& Archive)
{
	UE_LOG(LogChemicode, Error, TEXT("Base variable serialise called!"));
	return;
}

void UChemicodeVariable::Tick(float DeltaTime) { return; }
