// copyright lolol

#include "ScriptRuntime/Commands/ChemicodeCommand.h"
#include "ScriptRuntime/ChemicodeVM.h"
#include "ChemicodePrototype/ChemicodePrototype.h"

void UChemicodeCommand::PostInitProperties()
{
	Super::PostInitProperties();
	auto Items = TokeniseFormat(GetFormat());
	for (auto& Item : Items)
	{
		if (Item.TokenType == CCTTArgument)
			Arguments.Add(Item.VariableName, GetDefaultArgumentValue(Item.VariableType));
	}
}

bool UChemicodeCommand::Execute(UChemicodeVM* VM)
{
	UE_LOG(LogChemicode, Error, TEXT("Base chemicode command executed!"));
	return false;
}

void UChemicodeCommand::SerialiseCommand(FArchive& Archive)
{
	if (Archive.IsSaving())
	{
		FString CmdName = GetCommandName();
		Archive << CmdName;
	}

	Archive << Arguments;
}

bool UChemicodeCommand::CheckVariableNameIsValid(UChemicodeVM* VM, FString Key)
{
	// Check it exists and is not empty
	if (!Arguments.Contains(Key) || Arguments[Key].IsEmpty())
	{
		VM->ThrowError("Variable name is empty!", this);
		return false;
	}

	// Check VM doesn't already have variable with that name
	// TODO: Maybe throw a warning instead
	if (VM->Variables.Contains(Arguments[Key]))
	{
		VM->ThrowError(FString::Printf(TEXT("Variable with name %ls already exists!"), *Arguments[Key]), this);
		return false;
	}

	return true;
}

bool UChemicodeCommand::CheckInputVariableNameIsValid(UChemicodeVM* VM, FString Name,
	TSubclassOf<UChemicodeVariable> RequiredClass)
{
	// Check argument exists and is not empty
	if (!Arguments.Contains(Name) || Arguments[Name].IsEmpty())
	{
		VM->ThrowError(FString::Printf(TEXT("Argument %s is undefined or empty!"), *Name), this);
		return false;
	}
	
	// Check it exists and is not empty
	if (!VM->Variables.Contains(Arguments[Name]))
	{
		VM->ThrowError(FString::Printf(TEXT("There is no variable called %s!"), *Arguments[Name]), this);
		return false;
	}

	// Check the variable types match
	if (!VM->Variables[Arguments[Name]]->IsA(RequiredClass))
	{
		VM->ThrowError(FString::Printf(TEXT("Variable %s should be a %s, but is a %s!"), *Arguments[Name],
			*RequiredClass.GetDefaultObject()->GetTypeName(), *VM->Variables[Arguments[Name]]->GetTypeName()), this);
		return false;
	}

	return true;
}

TArray<FChemicodeCommandFormatToken> UChemicodeCommand::TokeniseFormat(FString FormatString)
{
	TArray<FChemicodeCommandFormatToken> Output;
	FString TextBuffer = "";
	for (const char CurrentChar : FormatString)
	{
		if (CurrentChar == '{')
		{
			if (!TextBuffer.IsEmpty())
				Output.Add(FChemicodeCommandFormatToken(TextBuffer));
			TextBuffer = "";
		} else if (CurrentChar == '}')
		{
			FString Left, Right;
			TextBuffer.Split(":", &Left, &Right);
			Output.Add(FChemicodeCommandFormatToken(Left, Right));
			TextBuffer = "";
		} else {
			TextBuffer += CurrentChar;
		}
	}
	Output.Add(FChemicodeCommandFormatToken(TextBuffer));
	return Output;
}

FString UChemicodeCommand::GetDefaultArgumentValue(FString Type)
{
	if (Type == "String")
		return "";
	else if (Type == "Float")
		return "0";
	else if (Type == "Resource")
		return "Hydrogen";
	else if (Type == "BBS")
		return "0";
	else if (Type == "BoolTF")
		return "True";
	else if (Type == "BoolOF")
		return "True";
	else if (Type == "BoolYN")
		return "True";
	else if (Type == "GasTap")
		return "0";
	else
		return "";
}

FString UChemicodeCommand::CommandToString(UChemicodeCommand* Command)
{
	FStringBuilderBase Builder;
	for (auto& Element : TokeniseFormat(Command->GetFormat()))
	{
		switch (Element.TokenType)
		{
		case CCTTArgument:
			{
				if (!Command->Arguments.Contains(Element.VariableName))
				{
					Builder.Append("Null");
					break;	
				}
				FString Value = Command->Arguments[Element.VariableName];
				Builder.Append(Value.IsEmpty() ? "Empty" : Value);
				break;	
			}
		case CCTTText:
			Builder.Append(Element.TextValue);
			break;
		default: break;
		}
	}
	return Builder.GetData();
}
