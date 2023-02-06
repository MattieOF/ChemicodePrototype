// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ResourceData.h"
#include "ResourceMeasurement.h"
#include "ResourceProperty.h"
#include "UObject/Object.h"
#include "ResourceInstance.generated.h"

/**
 * Instance of a resource
 */
UCLASS(BlueprintType)
class CHEMICODEPROTOTYPE_API UResourceInstance : public UObject
{
	GENERATED_BODY()

public:
	void SetResourceData(UResourceData* NewData, bool bOverwriteMeasurement = false);

	// Not a UFunction as blueprint functions can't return pointers to structs
	FResourceProperty* GetProperty(FName Name);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FResourceProperty GetPropertyCopy(FName Name) { return *GetProperty(Name); }
	
	UFUNCTION(BlueprintCallable)
	void SetPropertyHidden(const FName Name, const bool bNewHidden);
	UFUNCTION(BlueprintCallable)
	bool SetDecimalProperty(const FName Name, const double Value);
	UFUNCTION(BlueprintCallable)
	bool SetStringProperty(FName Name, FString Value);
	UFUNCTION(BlueprintCallable)
	double GetDecimalProperty(FName Name, double DefaultValue = 0);
	UFUNCTION(BlueprintCallable)
	FString GetStringProperty(FName Name, FString DefaultValue = "Empty");

public:
	/**
	 * @brief Data for this resource
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UResourceData* Data;

	/**
	 * @brief Measurement of this resource
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FResourceMeasurement Measurement;

	/**
	 * @brief State of this resource
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EResourceItemState> ResourceItemState;

	/**
	 * @brief Array of properties
	 */
	UPROPERTY(BlueprintReadWrite)
	TArray<FResourceProperty> Properties;
};
