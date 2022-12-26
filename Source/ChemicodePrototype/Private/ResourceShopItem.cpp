// copyright lolol


#include "ResourceShopItem.h"

#include "ChemicodePrototype/ChemicodePrototype.h"

// Sets default values
AResourceShopItem::AResourceShopItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
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

// Called every frame
void AResourceShopItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
