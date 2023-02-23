// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeCommand.h"
#include "ChemicodeSubmitCommand.generated.h"

/**
 * Command to submit an item
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UChemicodeSubmitCommand : public UChemicodeCommand
{
	GENERATED_BODY()

public:
	virtual bool Execute(UChemicodeVM* VM) override;
	
	static inline FString Format = "Set container {String:Container Name} as submission";
	virtual FString GetFormat() override { return Format; }
	
	static inline FString CommandName = "SetSubmission";
	virtual FString GetCommandName() override { return CommandName; }

	static inline FLinearColor BackgroundColour = FLinearColor(0.9, 0.9, 0.9, 1.0);
	virtual FLinearColor GetBackgroundColour() override { return BackgroundColour; }
};
