 // copyright lolol


#include "ResourceShopItem.h"

#include "ChemicodeGameMode.h"
#include "ChemicodeStatics.h"

 // Sets default values
AResourceShopItem::AResourceShopItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ItemMesh->ComponentTags.Add("Outline");
	OutlineComponent = CreateDefaultSubobject<UOutlineComponent>(TEXT("Outline"));
	RootComponent = ItemMesh;
}

// Called when the game starts or when spawned
void AResourceShopItem::BeginPlay()
{
	Super::BeginPlay();

	if (ItemMesh->GetStaticMesh() == nullptr || !bAllowMeshOverride)
	{
		ItemMesh->SetStaticMesh(ResourceData->Mesh);
		if (ResourceData->MeshMaterial != nullptr)
			ItemMesh->SetMaterial(0, ResourceData->MeshMaterial);
	}

	FScriptDelegate OnCursorOverDelegate;
	OnCursorOverDelegate.BindUFunction(this, "BeginMouseOver");
	OnBeginCursorOver.Add(OnCursorOverDelegate);

	FScriptDelegate OnCursorLeaveDelegate;
	OnCursorLeaveDelegate.BindUFunction(this, "EndMouseOver");
	OnEndCursorOver.Add(OnCursorLeaveDelegate);

	FScriptDelegate OnClickDelegate;
	OnClickDelegate.BindUFunction(this, "OnClicked");
	OnClicked.Add(OnClickDelegate);

	GameMode = UChemicodeStatics::GetChemicodeGameMode(GetWorld());
	Player = UChemicodeStatics::GetChemicodePawn(GetWorld());
}

void AResourceShopItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property->GetName() == "ResourceData" && ResourceData != nullptr && (!bAllowMeshOverride || ItemMesh->GetStaticMesh() == nullptr))
	{
		ItemMesh->SetStaticMesh(ResourceData->Mesh);
		if (ResourceData->MeshMaterial != nullptr)
			ItemMesh->SetMaterial(0, ResourceData->MeshMaterial);
	}
}

 void AResourceShopItem::BeginMouseOver()
 {
	if (Player->GetCurrentCamPlane() != GameMode->GetCabinetCamPlane())
		return;
	
	// Show outline and UI
	OutlineComponent->ShowOutline();
 }

 void AResourceShopItem::EndMouseOver()
 {
	// Hide outline and UI
	// TODO: Subscribe to some sort of event for player changing camnplane and then call end mouse over if the new one is not cabinet.
	OutlineComponent->HideOutline();
 }

 void AResourceShopItem::OnClick()
 {
	// Begin buying
 }

 // Called every frame
void AResourceShopItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
