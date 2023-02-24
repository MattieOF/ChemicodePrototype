// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeCommand.h"
#include "ConnectBunsenBurnerCommand.generated.h"

/**
 * Command to connect a bunsen burner to a gas tap
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UConnectBunsenBurnerCommand : public UChemicodeCommand
{
	GENERATED_BODY()

public:
	virtual bool Execute(UChemicodeVM* VM) override;
	
	static inline FString Format = "Connect bunsen burner {String:Bunsen Burner} to {GasTap:Gas Tap}";
	virtual FString GetFormat() override { return Format; }
	
	static inline FString CommandName = "ConnectBurner";
	virtual FString GetCommandName() override { return CommandName; }

	static inline FLinearColor BackgroundColour = FLinearColor(1.0, 0.12, 0.12, 1.0);
	virtual FLinearColor GetBackgroundColour() override { return BackgroundColour; }
};
