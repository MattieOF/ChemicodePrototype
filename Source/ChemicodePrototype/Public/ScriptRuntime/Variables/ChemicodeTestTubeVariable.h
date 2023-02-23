// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeVariable.h"
#include "ResourceContainer.h"
#include "ChemicodeTestTubeVariable.generated.h"

/**
 * Variable holding a reference to a test tube.
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UChemicodeTestTubeVariable : public UChemicodeVariable
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AResourceContainer* Value;
	
	virtual void SerialiseVariable(FArchive& Archive) override;

	static inline FString VariableTypeName = "Test Tube Reference";
	virtual FString GetTypeName() override { return VariableTypeName; }
	
	UFUNCTION(BlueprintCallable)
	void InitialiseWithNew();
};
