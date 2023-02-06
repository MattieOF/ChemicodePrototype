#pragma once

#include "ResourceProperty.generated.h"

UCLASS(BlueprintType)
class UResourceProperty : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHiddenInUI = false;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual FString ToString() { return "Invalid"; };
};

UCLASS(BlueprintType)
class UDecimalResourceProperty : public UResourceProperty
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double Value = 0;

	virtual FString ToString() override;
};

UCLASS(BlueprintType)
class UStringResourceProperty : public UResourceProperty
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Value = "Value";

	virtual FString ToString() override
	{
		return Value;
	};
};
