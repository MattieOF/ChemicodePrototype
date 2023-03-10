// copyright lolol

#include "ChemicodeGameInstance.h"

#include "ChemicodePawn.h"
#include "ChemicodeStatics.h"
#include "ResourceData.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Assignments/PracticalAssignment.h"
#include "Assignments/ScriptingAssignment.h"

void UChemicodeGameInstance::Init()
{
	Super::Init();

	// Get the asset registry module
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(
		FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	// Since asset registry scanning is asyncronous, we initiate a synchronous scan 
	// of the ResourceData directory to ensure they've all been discovered.
	TArray<FString> ContentPaths;
	ContentPaths.Add(TEXT("/Game/Resources/Data"));
	AssetRegistry.ScanPathsSynchronous(ContentPaths);

	// Find all the resource data assets and add them to the resources map
	TArray<FAssetData> ResourceAssetData;
	AssetRegistry.GetAssetsByClass(UResourceData::StaticClass()->GetFName(), ResourceAssetData, true);
	for (auto& Asset : ResourceAssetData)
	{
		UResourceData* Resource = Cast<UResourceData>(Asset.GetAsset());
		Resources.Add(Resource->Name.ToString(), Resource);
	}

	// Load campaign data and progress
	MainCampaign = NewObject<UCampaign>(this);
	MainCampaign->LoadAssignments(MainCampaignData);
	MainCampaign->LoadProgress();
}

void UChemicodeGameInstance::BeginAssignment(UAssignment* Assignment)
{
	CurrentAssignment = Assignment;
	bIsPracticalAssignment = Cast<UPracticalAssignment>(Assignment) != nullptr;
	bIsScriptingAssignment = Cast<UScriptingAssignment>(Assignment) != nullptr;

	OnAssignmentChanged.Broadcast(Assignment);
	
	AChemicodePawn* Pawn = UChemicodeStatics::GetChemicodePawn(GetWorld());
	Pawn->EnableInteraction();
	Pawn->SetCamPlane(Assignment->GetDefaultCamPlane(GetWorld()));
}

void UChemicodeGameInstance::ClearAssignment()
{
	CurrentAssignment = nullptr;
	bIsPracticalAssignment = false;
	bIsScriptingAssignment = false;

	OnAssignmentChanged.Broadcast(nullptr);
}
