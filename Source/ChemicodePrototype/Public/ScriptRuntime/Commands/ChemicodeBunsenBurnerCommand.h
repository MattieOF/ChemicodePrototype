// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ScriptRuntime/Commands/ChemicodeCommand.h"
#include "ChemicodeBunsenBurnerCommand.generated.h"

/**
 * Command to create a bunsen burner
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UChemicodeBunsenBurnerCommand : public UChemicodeCommand
{
	GENERATED_BODY()

public:
	virtual bool Execute(UChemicodeVM* VM) override;
	
	static inline FString Format = "Grab a bunsen burner, and call it {String:Variable Name}";
	virtual FString GetFormat() override { return Format; }
	
	static inline FString CommandName = "GetBunsenBurner";
	virtual FString GetCommandName() override { return CommandName; }
};
