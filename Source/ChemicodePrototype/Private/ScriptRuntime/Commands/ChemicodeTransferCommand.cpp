// copyright lolol

#include "ScriptRuntime/Commands/ChemicodeTransferCommand.h"

#include "ChemicodeStatics.h"
#include "ScriptRuntime/ChemicodeVM.h"
#include "ScriptRuntime/Variables/ChemicodeResourceItemVariable.h"
#include "ScriptRuntime/Variables/ChemicodeTestTubeVariable.h"

bool UChemicodeTransferCommand::Execute(UChemicodeVM* VM)
{
	// Check arguments are valid
	if (!Arguments.Contains("Amount"))
	{
		VM->ThrowError("Amount argument is empty/null!", this);
		return false;
	}
	if (!CheckInputVariableNameIsValid(VM, "Source", UChemicodeResourceItemVariable::StaticClass()))
		return false;
	if (!CheckInputVariableNameIsValid(VM, "Target", UChemicodeTestTubeVariable::StaticClass()))
		return false;

	// Get amount
	const FString& AmountInput = Arguments["Amount"];
	if (!AmountInput.IsNumeric())
	{
		VM->ThrowError("Amount input is not numeric! It should be a decimal number.", this);
		return false;
	}
	float Amount = FCString::Atof(*AmountInput);
	Amount *= 1000; // x1000 to convert from ml/g -> μl/mg

	const UChemicodeResourceItemVariable* Source = Cast<UChemicodeResourceItemVariable>(VM->Variables[Arguments["Source"]]);
	const UChemicodeTestTubeVariable* Target = Cast<UChemicodeTestTubeVariable>(VM->Variables[Arguments["Target"]]);
	
	// First, check the resource item has enough inside it
	if (Source->Value->Resource->Measurement.Value < Amount)
	{
		VM->ThrowError(FString::Printf(TEXT("Resource item %s does not have enough %s! It has %s, but needs %s!"),
			*Arguments["Source"], *Source->Value->Resource->Data->Name.ToString(), *UChemicodeStatics::MeasurementAsString(Source->Value->Resource->Measurement, true),
			*UChemicodeStatics::MeasurementAsString(FResourceMeasurement(Source->Value->Resource->Measurement.Unit, Amount), true)), this);
		return false;
	}
	
	// If it does, do the transfer
	Target->Value->TransferFromItem(Source->Value, Amount);
	
	VM->RunTime += Amount / 500000; // And add time equal to a second per 500g/500ml
	
	return true;
}
