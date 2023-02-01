// copyright lolol


#include "ResourceItem.h"

#include "ChemicodePawn.h"
#include "ChemicodeStatics.h"
#include "InteractionComponent.h"
#include "ResourceData.h"
#include "ChemicodePrototype/ChemicodePrototype.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AResourceItem::AResourceItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AResourceItem::BeginPlay()
{
	Super::BeginPlay();
	
	Player = Cast<AChemicodePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (Resource)
		SetResource(Resource, true, false);
	
	if (!InteractionComponent && Resource)
		SetInteractionType(Resource->DefaultInteraction);
}

void AResourceItem::SetResource(UResourceData* ResourceData, bool bRefreshTooltip, bool bPreserveMeasurement)
{
	Resource = ResourceData;
	Name = Resource->Name;
	Description = Resource->Description;
	MainMesh->SetStaticMesh(ResourceData->Mesh);
	if (ResourceData->MeshMaterial)
		MainMesh->SetMaterial(0, Resource->MeshMaterial);
	if (bRefreshTooltip)
		UChemicodeStatics::GetChemicodePawn(GetWorld())->RefreshTooltip();
	if (!bOverrideDefaultMeasurement && !bPreserveMeasurement)
		Measurement = Resource->BaseMeasurement;
}

void AResourceItem::SetInteractionType(TSubclassOf<UInteractionComponent> NewType, bool bRefreshTooltip)
{
	if (InteractionComponent)
		InteractionComponent->DestroyComponent();
	InteractionComponent = NewObject<UInteractionComponent>(this, NewType);
	InteractionComponent->RegisterComponent();
	if (bRefreshTooltip)
		UChemicodeStatics::GetChemicodePawn(GetWorld())->RefreshTooltip();
}

void AResourceItem::SetResourceAndInteraction(UResourceData* NewResource,
	TSubclassOf<UInteractionComponent> NewInteraction,
	bool bPreserveMeasurement)
{
	SetResource(NewResource, false, bPreserveMeasurement);
	SetInteractionType(NewInteraction, false);
	UChemicodeStatics::GetChemicodePawn(GetWorld())->RefreshTooltip();
}

bool AResourceItem::Use() 
{
	if (!InteractionComponent)
		return false;
	InteractionComponent->OnInteract();
	return true;
}

bool AResourceItem::InteractWith(AChemicodeObject* OtherObject)
{
	AResourceItem* Item = Cast<AResourceItem>(OtherObject);
	if (!Item)
		return false;
	if (!InteractionComponent)
		return false;
	InteractionComponent->OnInteractWith(Item);
	return true;
}

void AResourceItem::FireTick(AChemicodeObject* Source)
{
	InteractionComponent->FireTick(Source);
}

void AResourceItem::SetMeasurement(FResourceMeasurement NewMeasurement)
{
	Measurement = NewMeasurement;
	if (Measurement.Value < 1)
	{
		Measurement.Value = 0;
		ResourceState = Empty;
	}
}

#if WITH_EDITOR
void AResourceItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetName() == "Resource" && Resource != nullptr)
		SetResource(Resource, false, false);
}
#endif
