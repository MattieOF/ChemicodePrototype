// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ScriptRuntime/Commands/ChemicodeCommand.h"
#include "ConnectGasTubeCommand.generated.h"

/**
 * Command to connect a gas tube to two containers
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UConnectGasTubeCommand : public UChemicodeCommand
{
	GENERATED_BODY()

public:
	virtual bool Execute(UChemicodeVM* VM) override;
	
	static inline FString Format = "Connect gas tube {String:Gas Tube} from {String:Left Connection} to {String:Right Connection}";
	virtual FString GetFormat() override { return Format; }
	
	static inline FString CommandName = "ConnectTube";
	virtual FString GetCommandName() override { return CommandName; }

	static inline FLinearColor BackgroundColour = FLinearColor(1.0, 0.12, 0.12, 1.0);
	virtual FLinearColor GetBackgroundColour() override { return BackgroundColour; }
};
