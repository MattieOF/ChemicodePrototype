// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "BunsenBurner.h"
#include "ChemicodeVariable.h"
#include "ChemicodeBunsenBurnerVariable.generated.h"

/**
 * Variable representing a bunsen burner
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UChemicodeBunsenBurnerVariable : public UChemicodeVariable
{
	GENERATED_BODY()

public:
	UPROPERTY()
	ABunsenBurner* Value;
	
	virtual void SerialiseVariable(FArchive& Archive) override;

	virtual void Tick(float DeltaTime) override;

	static inline FString VariableTypeName = "Bunsen Burner Reference";
	virtual FString GetTypeName() override { return VariableTypeName; }
	
	UFUNCTION(BlueprintCallable)
	void InitialiseWithNew();
};
