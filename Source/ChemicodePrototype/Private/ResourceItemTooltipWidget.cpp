// copyright lolol


#include "ResourceItemTooltipWidget.h"
#include "ChemicodePrototype/ChemicodePrototype.h"

#include "Components/CanvasPanel.h"

void UResourceItemTooltipWidget::Show_Implementation(bool bInstant)
{
	RootCanvas->SetVisibility(ESlateVisibility::HitTestInvisible);
	bIsVisible = true;
}

void UResourceItemTooltipWidget::Hide_Implementation(bool bInstant)
{
	RootCanvas->SetVisibility(ESlateVisibility::Visible);
	bIsVisible = false;
}

void UResourceItemTooltipWidget::SetResource_Implementation(UResourceData* Resource)
{
	UE_LOG(LogChemicode, Log, TEXT("Resource item tooltip widget had no-op set resource c++ impl called. Make sure to implement it in blueprint!"));
}
