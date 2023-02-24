// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeCommand.h"
#include "GasTapStatusCommand.generated.h"

/**
 * Command to turn a gas tap on or off
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UGasTapStatusCommand : public UChemicodeCommand
{
	GENERATED_BODY()

public:
	virtual bool Execute(UChemicodeVM* VM) override;
	
	static inline FString Format = "Turn {GasTap:Tap} {BoolOF:New Power}";
	virtual FString GetFormat() override { return Format; }
	
	static inline FString CommandName = "GasTapStatus";
	virtual FString GetCommandName() override { return CommandName; }

	static inline FLinearColor BackgroundColour = FLinearColor(1.0, 0.12, 0.12, 1.0);
	virtual FLinearColor GetBackgroundColour() override { return BackgroundColour; }
};
