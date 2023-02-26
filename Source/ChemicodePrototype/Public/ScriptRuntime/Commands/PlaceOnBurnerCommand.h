// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeCommand.h"
#include "PlaceOnBurnerCommand.generated.h"

/**
 * Command to place a container or resource on a burner
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UPlaceOnBurnerCommand : public UChemicodeCommand
{
	GENERATED_BODY()

public:
	virtual bool Execute(UChemicodeVM* VM) override;
	
	static inline FString Format = "Place {String:Item} on {String:Bunsen Burner}";
	virtual FString GetFormat() override { return Format; }
	
	static inline FString CommandName = "PlaceOnBurner";
	virtual FString GetCommandName() override { return CommandName; }

	static inline FLinearColor BackgroundColour = FLinearColor(1.0, 0.12, 0.12, 1.0);
	virtual FLinearColor GetBackgroundColour() override { return BackgroundColour; }
};
