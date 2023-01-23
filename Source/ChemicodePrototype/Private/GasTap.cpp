// copyright lolol

#include "GasTap.h"

AGasTap::AGasTap()
{
	PrimaryActorTick.bCanEverTick = false;

	Name = FText::FromString("Gas Tap");
	Description = FText::FromString("Used to gas yourself, and power bunsen burners.");
	bHoldable  = false;
	
	TapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tap"));
	TapMesh->SetupAttachment(MainMesh);
	TapMesh->SetCastShadow(false);
	TapMesh->ComponentTags.Add("Outline");
	RootComponent = MainMesh;
}

bool AGasTap::Interact()
{
	Toggle();
	return true;
}

// Called when the game starts or when spawned
void AGasTap::BeginPlay()
{
	Super::BeginPlay();
	
}
