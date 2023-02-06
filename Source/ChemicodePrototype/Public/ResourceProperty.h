#pragma once

#include "ResourceProperty.generated.h"

USTRUCT(BlueprintType)
struct FResourceProperty
{
	GENERATED_BODY()

	virtual ~FResourceProperty() { }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PropertyName = TEXT("Property");
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHiddenInUI = false;

	virtual FString ToString() { return "Invalid"; };
};

USTRUCT(BlueprintType)
struct FDecimalResourceProperty : public FResourceProperty
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double Value = 0;

	virtual FString ToString() override;
};

USTRUCT(BlueprintType)
struct FStringResourceProperty : public FResourceProperty
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Value = "Value";

	virtual FString ToString() override
	{
		return Value;
	};
};
