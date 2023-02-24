// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeCommand.h"
#include "WaitCommand.generated.h"

/**
 * Command to wait a while.
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UWaitCommand : public UChemicodeCommand
{
	GENERATED_BODY()

public:
	virtual bool Execute(UChemicodeVM* VM) override;
	
	static inline FString Format = "Wait for {Float:Time} seconds";
	virtual FString GetFormat() override { return Format; }
	
	static inline FString CommandName = "Wait";
	virtual FString GetCommandName() override { return CommandName; }

	static inline FLinearColor BackgroundColour = FLinearColor(0.9, 0.9, 0.9, 1.0);
	virtual FLinearColor GetBackgroundColour() override { return BackgroundColour; }
};
