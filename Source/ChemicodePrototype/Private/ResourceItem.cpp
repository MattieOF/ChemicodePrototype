// copyright lolol


#include "ResourceItem.h"

#include "ResourceData.h"
#include "ChemicodePrototype/ChemicodePrototype.h"

// Sets default values
AResourceItem::AResourceItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetAllUseCCD(true);
	Outline = CreateDefaultSubobject<UOutlineComponent>(TEXT("Outline"));

	RootComponent = MeshComponent;
}

void AResourceItem::SetResource(UResourceData* ResourceData)
{
	MeshComponent->SetStaticMesh(ResourceData->Mesh);
	Resource = ResourceData;
}

void AResourceItem::Use()
{
	UE_LOG(LogChemicode, Log, TEXT("Item used!"));
}

// Called when the game starts or when spawned
void AResourceItem::BeginPlay()
{
	Super::BeginPlay();
	if (Resource)
		SetResource(Resource);
}

// Called every frame
void AResourceItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#if WITH_EDITOR
void AResourceItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetName() == "Resource" && Resource != nullptr)
		SetResource(Resource);
}
#endif
