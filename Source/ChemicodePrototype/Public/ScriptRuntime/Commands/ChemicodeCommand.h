// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ScriptRuntime/Variables/ChemicodeVariable.h"
#include "UObject/Object.h"
#include "ChemicodeCommand.generated.h"

class UChemicodeVM;

UENUM(BlueprintType)
enum EChemicodeCommandFormatTokenType
{
	CCTTText       UMETA(DisplayName = "Text"),
	CCTTArgument   UMETA(DisplayName = "Argument")
};

USTRUCT(BlueprintType)
struct FChemicodeCommandFormatToken
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EChemicodeCommandFormatTokenType> TokenType;
	UPROPERTY(BlueprintReadOnly)
	FString TextValue = "";
	UPROPERTY(BlueprintReadOnly)
	FString VariableName = "";
	UPROPERTY(BlueprintReadOnly)
	FString VariableType = "";

	FChemicodeCommandFormatToken()
	{
		TokenType = CCTTText;
	}
	
	FChemicodeCommandFormatToken(FString Text)
	{
		TokenType = CCTTText;
		TextValue = Text;
	}

	FChemicodeCommandFormatToken(FString VarType, FString VarName)
	{
		TokenType = CCTTArgument;
		VariableName = VarName;
		VariableType = VarType;
	}
};

/**
 * Object that represents a command in a script. This is a base class; useful commands inherit from this.
 */
UCLASS(BlueprintType)
class CHEMICODEPROTOTYPE_API UChemicodeCommand : public UObject
{
	GENERATED_BODY()

public:
	virtual void PostInitProperties() override;
	
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, FString> Arguments;

	UFUNCTION()
	virtual bool Execute(UChemicodeVM* VM);

	void SerialiseCommand(FArchive& Archive);

	static inline FString Format = "Command";
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual FString GetFormat() { return Format; }
	
	static inline FString CommandName = "Base";
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual FString GetCommandName() { return CommandName; }

	static inline FLinearColor BackgroundColour = FLinearColor(0, 0.35, 1.0, 1.0);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual FLinearColor GetBackgroundColour() { return BackgroundColour; }

	UFUNCTION(BlueprintCallable)
	virtual bool CheckVariableNameIsValid(UChemicodeVM* VM, FString Name);

	UFUNCTION(BlueprintCallable)
	virtual bool CheckInputVariableNameIsValid(UChemicodeVM* VM, FString Name, TSubclassOf<UChemicodeVariable> ValidClass);

	UFUNCTION(BlueprintCallable)
	virtual bool CheckInputVariableNameIsValidMultiType(UChemicodeVM* VM, FString Name,
	                                                    TArray<TSubclassOf<UChemicodeVariable>> ValidClasses);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static TArray<FChemicodeCommandFormatToken> TokeniseFormat(FString FormatString);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString GetDefaultArgumentValue(FString Type);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString CommandToString(UChemicodeCommand* Command);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FORCEINLINE FLinearColor GetCommandColour(TSubclassOf<UChemicodeCommand> CommandClass)
	{ return CommandClass.GetDefaultObject()->GetBackgroundColour(); } 
};
