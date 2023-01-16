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

void AChemicodeObject::BeginPlay()
{
	Super::BeginPlay();
}
