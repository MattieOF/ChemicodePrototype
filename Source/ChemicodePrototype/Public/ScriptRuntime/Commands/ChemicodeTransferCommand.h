// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeCommand.h"
#include "ChemicodeTransferCommand.generated.h"

/**
 * Command to transfer some resource from an item to a container
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UChemicodeTransferCommand : public UChemicodeCommand
{
	GENERATED_BODY()

public:
	virtual bool Execute(UChemicodeVM* VM) override;
	
	static inline FString Format = "Transfer {Amount:Amount}ml/g from {String:Source} to {String:Target}";
	virtual FString GetFormat() override { return Format; }
	
	static inline FString CommandName = "Transfer";
	virtual FString GetCommandName() override { return CommandName; }
};
