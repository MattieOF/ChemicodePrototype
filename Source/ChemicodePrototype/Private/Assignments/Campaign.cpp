// copyright lolol

#include "Assignments/Campaign.h"

#include "ChemicodePrototype/ChemicodePrototype.h"
#include "Serialization/BufferArchive.h"

void UCampaign::LoadAssignments(UCampaignData* Data)
{
	int Extensions = 0;
	RootAssignment = NewObject<UCampaignAssignment>(this);
	UCampaignAssignment* CurrentAssignment = RootAssignment;
	for (int i = 0; i < Data->Data.Num(); i++)
	{
		if (Extensions <= 0)
		{
			CurrentAssignment->ThisAssignment = Data->Data[i].Assignment;
			if (i != Data->Data.Num() - 1)
			{
				CurrentAssignment->Next = NewObject<UCampaignAssignment>(this);
				CurrentAssignment = CurrentAssignment->Next;
			}
			Extensions = Data->Data[i].FollowingExtensions;
		} else
		{
			UCampaignAssignment* Extension = NewObject<UCampaignAssignment>(this);
			Extension->ThisAssignment = Data->Data[i].Assignment;
			CurrentAssignment->Extensions.Add(Extension);
			Extensions--;
		}
	}
}

void UCampaign::SetAssignmentCompleted(UAssignment* Assignment)
{
	UCampaignAssignment* CurrentAssignment = RootAssignment;
	while (CurrentAssignment != nullptr)
	{
		if (CurrentAssignment->ThisAssignment == Assignment)
		{
			CurrentAssignment->bCompleted = true;
			return;
		}
		
		for (UCampaignAssignment* Extension : CurrentAssignment->Extensions)
		{
			if (Extension->ThisAssignment == Assignment)
				Extension->bCompleted = true;
			return;
		}

		CurrentAssignment = CurrentAssignment->Next;
	}
}

void UCampaign::SaveProgress(FString Filename)
{
	TMap<FString, bool> Completion;
	UCampaignAssignment* Assignment = RootAssignment;
	
	while (Assignment != nullptr)
	{
		Completion.Add(Assignment->ThisAssignment->GetName(), Assignment->bCompleted);
		for (const UCampaignAssignment* Extension : Assignment->Extensions)
			Completion.Add(Extension->ThisAssignment->GetName(), Extension->bCompleted);
		Assignment = Assignment->Next;
	}

	// Create buffer and save map
	FBufferArchive BinarySaveArchive;
	BinarySaveArchive << Completion;

	// Save buffer
	const FString Path = *FString::Printf(TEXT("%ls%ls"), *FPaths::ProjectSavedDir(), *Filename);
	const bool SaveResult = FFileHelper::SaveArrayToFile(BinarySaveArchive, *Path);
	if (!SaveResult)
	{
		UE_LOG(LogChemicode, Error, TEXT("Failed to save campaign progress to %s!"), *Path);
		return;
	}
	
	// Empty buffer
	BinarySaveArchive.FlushCache();
	BinarySaveArchive.Empty();
}

void UCampaign::LoadProgress(FString Filename)
{
	TArray<uint8> BinaryArray;

	// Load and verify file
	const FString Path = *FString::Printf(TEXT("%ls%ls"), *FPaths::ProjectSavedDir(), *Filename);
	const bool LoadFileResult = FFileHelper::LoadFileToArray(BinaryArray, *Path);
	if (!LoadFileResult)
	{
		UE_LOG(LogChemicode, Error, TEXT("In campaign progress loader, failed to open file %s!"), *Path);
		return;
	}
	if (BinaryArray.Num() <= 0)
	{
		UE_LOG(LogChemicode, Error, TEXT("In campaign progress loader, file %s is empty!"), *Path);
		return;
	}

	// Create binary loader
	FMemoryReader BinaryLoader = FMemoryReader(BinaryArray, true);
	BinaryLoader.Seek(0);
	
	// Do the loading
	TMap<FString, bool> Completion;
	BinaryLoader << Completion;

	// Empty buffer
	BinaryLoader.FlushCache();
	BinaryArray.Empty();
	BinaryLoader.Close();

	UCampaignAssignment* Assignment = RootAssignment;

	while (Assignment != nullptr)
	{
		if (Completion.Contains(Assignment->ThisAssignment->GetName()))
			Assignment->bCompleted = Completion[Assignment->ThisAssignment->GetName()];

		for (UCampaignAssignment* Extension : Assignment->Extensions)
		{
			if (Completion.Contains(Extension->ThisAssignment->GetName()))
				Extension->bCompleted = Completion[Extension->ThisAssignment->GetName()];
			if (!Extension->bCompleted)
				break;
		}
		
		if (!Assignment->bCompleted)
			Assignment = nullptr;
		else
			Assignment = Assignment->Next;
	}
}
