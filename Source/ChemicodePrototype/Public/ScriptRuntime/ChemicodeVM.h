﻿// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeCommand.h"
#include "ChemicodeScript.h"
#include "ChemicodeVariable.h"
#include "ResourceContainer.h"
#include "UObject/Object.h"
#include "ChemicodeVM.generated.h"

USTRUCT(BlueprintType)
struct FChemicodeScriptOutput
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool Successful = false;
	UPROPERTY()
	FString ErrorMessage = "Successful";
	UPROPERTY()
	UChemicodeCommand* ErrorSource = nullptr;
	UPROPERTY()
	float RunTime = 0;
	UPROPERTY()
	AResourceContainer* Result = nullptr;
};

/**
 * Object that runs a script, keeping notes of all variables and outputs, and reporting any errors, and
 * then deciding if it results in the correct end goal and satisfies all conditions of the assignment. 
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UChemicodeVM : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UChemicodeScript*                  CurrentScript = nullptr;
	UPROPERTY()
	int                                CurrentCommand = 0;
	UPROPERTY()
	TMap<FString, UChemicodeVariable*> Variables;
	UPROPERTY()
	double                             RunTime = 0;
	UPROPERTY()
	bool                               HasError = false;
	UPROPERTY()
	FString                            ErrorMessage = "Successful";
	UPROPERTY()
	UChemicodeCommand*                 ErrorSource = nullptr;

	FChemicodeScriptOutput ExecuteScript(UChemicodeScript* Script);
	void ThrowError(FString Message, UChemicodeCommand* Source);
	void ClearError();
};
