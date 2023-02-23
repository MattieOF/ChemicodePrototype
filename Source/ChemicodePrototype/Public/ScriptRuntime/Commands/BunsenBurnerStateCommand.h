// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeCommand.h"
#include "BunsenBurnerStateCommand.generated.h"

/**
 * Command to set bunsen burners state
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UBunsenBurnerStateCommand : public UChemicodeCommand
{
	GENERATED_BODY()

public:
	virtual bool Execute(UChemicodeVM* VM) override;
	
	static inline FString Format = "Set the state of {String:Bunsen Burner} to {BBS:State}";
	virtual FString GetFormat() override { return Format; }
	
	static inline FString CommandName = "SetBunsenBurnerState";
	virtual FString GetCommandName() override { return CommandName; }

	static inline FLinearColor BackgroundColour = FLinearColor(1.0, 0.12, 0.12, 1.0);
	virtual FLinearColor GetBackgroundColour() override { return BackgroundColour; }
};
