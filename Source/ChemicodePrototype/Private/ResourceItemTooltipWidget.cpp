// copyright lolol

#include "ResourceItemTooltipWidget.h"
#include "ChemicodePrototype/ChemicodePrototype.h"

#include "Components/CanvasPanel.h"

void UResourceItemTooltipWidget::Show_Implementation(bool bInstant)
{
	RootCanvas->SetVisibility(ESlateVisibility::HitTestInvisible);
	bIsShown = true;
}

void UResourceItemTooltipWidget::Hide_Implementation(bool bInstant)
{
	RootCanvas->SetVisibility(ESlateVisibility::Hidden);
	bIsShown = false;
}

void UResourceItemTooltipWidget::SetResource_Implementation(UResourceData* Resource, AResourceItem* Item)
{
	UE_LOG(LogChemicode, Log, TEXT("Resource item tooltip widget had no-op set resource c++ impl called. Make sure to implement it in blueprint!"));
}
