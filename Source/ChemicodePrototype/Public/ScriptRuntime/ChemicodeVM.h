// copyright lolol

#pragma once

#define VM_GAS_TAP_COUNT 3

#include "CoreMinimal.h"
#include "Commands/ChemicodeCommand.h"
#include "ChemicodeScript.h"
#include "GasTap.h"
#include "Variables/ChemicodeVariable.h"
#include "ResourceContainer.h"
#include "UObject/Object.h"
#include "ChemicodeVM.generated.h"

USTRUCT(BlueprintType)
struct FChemicodeScriptOutput
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	bool Successful = false;
	UPROPERTY(BlueprintReadOnly)
	FString ErrorMessage = "Successful";
	UPROPERTY(BlueprintReadOnly)
	UChemicodeCommand* ErrorSource = nullptr;
	UPROPERTY(BlueprintReadOnly)
	float RunTime = 0;
	UPROPERTY(BlueprintReadOnly)
	AResourceContainer* Result = nullptr;
};

/**
 * Object that runs a script, keeping notes of all variables and outputs, and reporting any errors, and
 * then deciding if it results in the correct end goal and satisfies all conditions of the assignment. 
 */
UCLASS(BlueprintType)
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
	double                             PrevRunTime = 0;
	UPROPERTY()
	bool                               HasError = false;
	UPROPERTY()
	FString                            ErrorMessage = "Successful";
	UPROPERTY()
	UChemicodeCommand*                 ErrorSource = nullptr;
	UPROPERTY(BlueprintReadWrite)
	AResourceContainer*                Submission = nullptr;
	UPROPERTY(BlueprintReadWrite)
	TArray<AGasTap*>                   GasTaps;

	UFUNCTION(BlueprintCallable)
	FChemicodeScriptOutput ExecuteScript(UChemicodeScript* Script);
	UFUNCTION(BlueprintCallable)
	void ThrowError(FString Message, UChemicodeCommand* Source);
	UFUNCTION(BlueprintCallable)
	void ClearError();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FORCEINLINE int GetVMGasTapCount() { return VM_GAS_TAP_COUNT; }
};
