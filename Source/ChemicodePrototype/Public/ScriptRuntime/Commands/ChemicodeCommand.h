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

	UFUNCTION(BlueprintCallable)
	virtual bool CheckVariableNameIsValid(UChemicodeVM* VM, FString Name);

	UFUNCTION(BlueprintCallable)
	virtual bool CheckInputVariableNameIsValid(UChemicodeVM* VM, FString Name, TSubclassOf<UChemicodeVariable> RequiredClass);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static TArray<FChemicodeCommandFormatToken> TokeniseFormat(FString FormatString);
};
