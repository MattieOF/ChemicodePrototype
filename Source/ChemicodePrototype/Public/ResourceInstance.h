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
	void SetResourceData(UResourceData* NewData, bool bPreserveProperties = false, bool bOverwriteMeasurement = false);

	// Not a UFunction as blueprint functions can't return pointers to structs
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UResourceProperty* GetProperty(FName Name);
	
	UFUNCTION(BlueprintCallable)
	void SetPropertyHidden(const FName Name, const bool bNewHidden);
	UFUNCTION(BlueprintCallable)
	bool SetDecimalProperty(const FName Name, const double Value);
	UFUNCTION(BlueprintCallable)
	bool SetStringProperty(FName Name, FString Value);
	UFUNCTION(BlueprintCallable)
	bool SetBoolProperty(FName Name, bool Value);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	double GetDecimalProperty(FName Name, bool& bSuccess, double DefaultValue = 0);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetStringProperty(FName Name, bool& bSuccess, FString DefaultValue = "Empty");
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetBoolProperty(FName Name, bool& bSuccess, bool DefaultValue = false);
	
	void AddDefaultResourceProperty(const FDefaultResourceProperty& Property);

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
	TArray<UResourceProperty*> Properties;

	UPROPERTY()
	UWorld* WorldRef;
};
