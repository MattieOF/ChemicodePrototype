// copyright lolol

#include "Assignments/Campaign.h"

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
			if (i == Data->Data.Num() - 1)
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

	// TODO: Save to archive
}

void UCampaign::LoadProgress(FString Filename)
{
	// TODO
	// Make sure to stop iterating once we find a root assignment that is incomplete
}
