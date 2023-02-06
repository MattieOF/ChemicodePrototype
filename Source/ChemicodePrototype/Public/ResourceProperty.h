#pragma once

#include "ResourceProperty.generated.h"

USTRUCT(BlueprintType)
struct FResourceProperty
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PropertyName = TEXT("Property");
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHiddenInUI = false;
};

USTRUCT(BlueprintType)
struct FDecimalResourceProperty : FResourceProperty
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double Value = 0;
};

USTRUCT(BlueprintType)
struct FStringResourceProperty : FResourceProperty
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Value = "Value";
};
