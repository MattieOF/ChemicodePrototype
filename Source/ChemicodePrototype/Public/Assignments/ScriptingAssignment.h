// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Assignment.h"
#include "ScriptRuntime/ChemicodeVM.h"
#include "ScriptingAssignment.generated.h"

/**
 * Scripting assignment class
 */
UCLASS(BlueprintType)
class CHEMICODEPROTOTYPE_API UScriptingAssignment : public UAssignment
{
	GENERATED_BODY()

public:
	virtual bool IsCompleted(UObject* Submission, FString& Message) override;

	virtual ACameraPlane* GetDefaultCamPlane(UWorld* World) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FContainerCondition> SubmissionConditions;

	UPROPERTY(BlueprintReadWrite)
	UChemicodeVM* VM = nullptr;
};
