// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeVariable.h"
#include "ResourceTube.h"
#include "GasTubeVariable.generated.h"

/**
 * Variable representing a gas tube
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UGasTubeVariable : public UChemicodeVariable
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AResourceTube* Value;
	
	virtual void SerialiseVariable(FArchive& Archive) override;

	virtual void Tick(float DeltaTime) override;

	static inline FString VariableTypeName = "Gas Tube Reference";
	virtual FString GetTypeName() override { return VariableTypeName; }
	
	UFUNCTION(BlueprintCallable)
	void InitialiseWithNew();
};
