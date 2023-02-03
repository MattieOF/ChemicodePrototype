// copyright lolol


#include "ResourceItem.h"

#include "ChemicodePawn.h"
#include "ChemicodeStatics.h"
#include "InteractionComponent.h"
#include "ResourceData.h"
#include "ResourceInstance.h"
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
	Resource = NewObject<UResourceInstance>(this, TEXT("ResourceInstance"));
	UE_LOG(LogChemicode, Log, TEXT("%i"), DefaultItemState);
	Resource->ResourceItemState = DefaultItemState;
}

void AResourceItem::SetResource(UResourceData* ResourceData, bool bRefreshTooltip, bool bPreserveMeasurement)
{
	Resource->Data = ResourceData;
	Name = ResourceData->Name;
	Description = ResourceData->Description;
	MainMesh->SetStaticMesh(ResourceData->Mesh);
	if (ResourceData->MeshMaterial)
		MainMesh->SetMaterial(0, ResourceData->MeshMaterial);
	if (bRefreshTooltip)
		UChemicodeStatics::GetChemicodePawn(GetWorld())->RefreshTooltip();
	if (!bOverrideDefaultMeasurement && !bPreserveMeasurement)
		Resource->Measurement = ResourceData->BaseMeasurement;
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
	UE_LOG(LogChemicode, Log, TEXT("Trying to interact!"));
	if (!InteractionComponent)
		return false;
	UE_LOG(LogChemicode, Log, TEXT("Interacting! Comp: %s"), *InteractionComponent->GetName());
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
	Resource->Measurement = NewMeasurement;
	if (Resource->Measurement.Value < 1)
	{
		Resource->Measurement.Value = 0;
		Resource->ResourceItemState = Empty;
	}
}

#if WITH_EDITOR
void AResourceItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property->GetName() == "Resource" && Resource != nullptr)
		SetResource(Resource->Data, false, false);
}
#endif
