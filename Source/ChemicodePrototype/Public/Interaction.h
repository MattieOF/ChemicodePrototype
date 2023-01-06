#pragma once

#include "CoreMinimal.h"
#include "Interaction.generated.h"

USTRUCT(BlueprintType)
struct FInteraction
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Interaction")
	FText InteractionName;
};
