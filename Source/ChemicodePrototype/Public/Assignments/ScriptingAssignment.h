﻿// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Assignment.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FContainerCondition> SubmissionConditions;
};