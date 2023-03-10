// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeVariable.h"
#include "ResourceItem.h"
#include "ChemicodeResourceItemVariable.generated.h"

/**
 * Variable representing a chemicode resource item
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UChemicodeResourceItemVariable : public UChemicodeVariable
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	AResourceItem* Value;

	virtual void SerialiseVariable(FArchive& Archive) override;

	virtual void Tick(float DeltaTime) override;

	static inline FString VariableTypeName = "Resource Item Reference";
	virtual FString GetTypeName() override { return VariableTypeName; }
	
	UFUNCTION(BlueprintCallable)
	void InitialiseWithResource(UResourceData* Resource);
};
