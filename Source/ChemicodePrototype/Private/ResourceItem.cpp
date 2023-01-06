// copyright lolol


#include "ResourceItem.h"

#include "ChemicodePawn.h"
#include "ResourceData.h"
#include "ChemicodePrototype/ChemicodePrototype.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AResourceItem::AResourceItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->BodyInstance.SetResponseToChannel(COLLISION_CHANNEL_BLOCKITEM, ECR_Block);	
	MeshComponent->SetAllUseCCD(true);
	MeshComponent->ComponentTags.Add("Outline");
	Outline = CreateDefaultSubobject<UOutlineComponent>(TEXT("Outline"));

	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void AResourceItem::BeginPlay()
{
	Super::BeginPlay();
	
	Player = Cast<AChemicodePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (Resource)
		SetResource(Resource);

	if (!InteractionComponent && Resource)
		SetInteractionType(Resource->DefaultInteraction);
}

void AResourceItem::SetResource(UResourceData* ResourceData)
{
	Resource = ResourceData;
	MeshComponent->SetStaticMesh(ResourceData->Mesh);
	if (ResourceData->MeshMaterial)
		MeshComponent->SetMaterial(0, Resource->MeshMaterial);
}

void AResourceItem::SetInteractionType(TSubclassOf<UInteractionComponent> NewType)
{
	if (InteractionComponent)
		InteractionComponent->DestroyComponent();
	InteractionComponent = NewObject<UInteractionComponent>(this, NewType);
	InteractionComponent->RegisterComponent();
}

bool AResourceItem::Interact()
{
	if (!InteractionComponent)
		return false;
	InteractionComponent->OnInteract();
	return true;
}

bool AResourceItem::InteractWith(AResourceItem* Item)
{
	if (!InteractionComponent)
		return false;
	InteractionComponent->OnInteractWith(Item);
	return true;
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
