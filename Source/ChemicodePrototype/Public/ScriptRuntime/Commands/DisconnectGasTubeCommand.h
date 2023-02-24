// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeCommand.h"
#include "DisconnectGasTubeCommand.generated.h"

/**
 * Command to disconnect a gas tube from any items
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UDisconnectGasTubeCommand : public UChemicodeCommand
{
	GENERATED_BODY()

public:
	virtual bool Execute(UChemicodeVM* VM) override;
	
	static inline FString Format = "Disconnect gas tube {String:Gas Tube} from any containers";
	virtual FString GetFormat() override { return Format; }
	
	static inline FString CommandName = "DisconnectTube";
	virtual FString GetCommandName() override { return CommandName; }

	static inline FLinearColor BackgroundColour = FLinearColor(1.0, 0.12, 0.12, 1.0);
	virtual FLinearColor GetBackgroundColour() override { return BackgroundColour; }
};
