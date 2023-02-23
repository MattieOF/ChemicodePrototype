// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeCommand.h"
#include "ChemicodeTestTubeCommand.generated.h"

/**
 * Command to create a test tube and name it
 */
UCLASS(BlueprintType)
class CHEMICODEPROTOTYPE_API UChemicodeTestTubeCommand : public UChemicodeCommand
{
	GENERATED_BODY()

public:
	virtual bool Execute(UChemicodeVM* VM) override;
	
	static inline FString Format = "Grab a test tube, and call it {String:Variable Name}";
	virtual FString GetFormat() override { return Format; }
	
	static inline FString CommandName = "GetTestTube";
	virtual FString GetCommandName() override { return CommandName; }
};
