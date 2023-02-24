// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeCommand.h"
#include "GetGasTubeCommand.generated.h"

/**
 * Command to get a gas tube
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UGetGasTubeCommand : public UChemicodeCommand
{
	GENERATED_BODY()

public:
	virtual bool Execute(UChemicodeVM* VM) override;
	
	static inline FString Format = "Grab a gas tube, and call it {String:Variable Name}";
	virtual FString GetFormat() override { return Format; }
	
	static inline FString CommandName = "GetGasTube";
	virtual FString GetCommandName() override { return CommandName; }
};
