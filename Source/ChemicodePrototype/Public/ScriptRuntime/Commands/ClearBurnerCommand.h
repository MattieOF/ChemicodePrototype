// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeCommand.h"
#include "ClearBurnerCommand.generated.h"

/**
 * Command to clear any items off a bunsen burner
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UClearBurnerCommand : public UChemicodeCommand
{
	GENERATED_BODY()

public:
	virtual bool Execute(UChemicodeVM* VM) override;
	
	static inline FString Format = "Clear any items off of {String:Bunsen Burner}";
	virtual FString GetFormat() override { return Format; }
	
	static inline FString CommandName = "ClearBurner";
	virtual FString GetCommandName() override { return CommandName; }

	static inline FLinearColor BackgroundColour = FLinearColor(1.0, 0.12, 0.12, 1.0);
	virtual FLinearColor GetBackgroundColour() override { return BackgroundColour; }
};
