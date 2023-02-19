// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeCommand.h"
#include "ChemicodeBuyCommand.generated.h"

/**
 * Command for buying a resource
 */
UCLASS(BlueprintType)
class CHEMICODEPROTOTYPE_API UChemicodeBuyCommand : public UChemicodeCommand
{
	GENERATED_BODY()

public:
	virtual bool Execute(UChemicodeVM* VM) override;
	
	static inline FString Format = "Buy some {Resource:Resource}, and call it {String:VarName}";
	virtual FString GetFormat() override { return Format; }
	
	static inline FString CommandName = "Buy";
	virtual FString GetCommandName() override { return CommandName; }
};
