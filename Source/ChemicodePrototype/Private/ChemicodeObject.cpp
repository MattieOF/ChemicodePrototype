// copyright lolol

#include "ChemicodeObject.h"

#include "ChemicodePrototype/ChemicodePrototype.h"

// Sets default values
AChemicodeObject::AChemicodeObject()
{
	PrimaryActorTick.bCanEverTick = true;

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MainMesh->BodyInstance.SetResponseToChannel(COLLISION_CHANNEL_BLOCKITEM, ECR_Block);	
	MainMesh->SetAllUseCCD(true);
	MainMesh->ComponentTags.Add("Outline");
	MainMesh->ComponentTags.Add("MainMesh");
	OutlineComponent = CreateDefaultSubobject<UOutlineComponent>(TEXT("Outline"));

	RootComponent = MainMesh;
}

bool AChemicodeObject::Use()
{
	return false;
}

bool AChemicodeObject::Interact()
{
	return false;
}

bool AChemicodeObject::AltInteract()
{
	return false;
}

bool AChemicodeObject::InteractWith(AChemicodeObject* OtherObject)
{
	return false;
}

bool AChemicodeObject::AltInteractWith(AChemicodeObject* OtherObject)
{
	return false;
}

void AChemicodeObject::FireTick(AChemicodeObject* Source, float DeltaTime)
{
	return;
}

void AChemicodeObject::ReceiveResource(UResourceData* Resource, FResourceMeasurement Amount)
{
	return;
}

bool AChemicodeObject::HasResource(UResourceData* Resource)
{
	return false;
}

FResourceMeasurement AChemicodeObject::GetResourceAmount(UResourceData* Resource)
{
	return FResourceMeasurement();
}

void AChemicodeObject::BeginPlay()
{
	Super::BeginPlay();
}
