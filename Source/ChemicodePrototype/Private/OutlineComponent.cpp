// copyright lolol

#include "OutlineComponent.h"

#include "ChemicodePrototype/ChemicodePrototype.h"

// Sets default values for this component's properties
UOutlineComponent::UOutlineComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UOutlineComponent::ShowOutline()
{
	UE_LOG(LogChemicode, Log, TEXT("Outlining %i component(s)."), OutlinedComponents.Num());
	for (const auto Component : OutlinedComponents)
		Component->SetRenderCustomDepth(true);
}

void UOutlineComponent::HideOutline()
{
	for (const auto Component : OutlinedComponents)
		Component->SetRenderCustomDepth(false);
}

void UOutlineComponent::RefreshOutlinedComponents()
{
	OutlinedComponents.Empty();
	for (const auto Component : GetOwner()->GetComponentsByTag(UPrimitiveComponent::StaticClass(), OutlineTag))
		OutlinedComponents.Add(Cast<UPrimitiveComponent>(Component));
}

// Called when the game starts
void UOutlineComponent::BeginPlay()
{
	Super::BeginPlay();
	RefreshOutlinedComponents();
}
