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

	static inline FLinearColor BackgroundColour = FLinearColor(1.0, 0.12, 0.12, 1.0);
	virtual FLinearColor GetBackgroundColour() override { return BackgroundColour; }
};
