// copyright lolol


#include "ResourceInfoWidget.h"

#include "ChemicodePrototype/ChemicodePrototype.h"
#include "Components/CanvasPanel.h"

void UResourceInfoWidget::Show_Implementation(bool bInstant)
{
	RootCanvas->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UResourceInfoWidget::Hide_Implementation(bool bInstant)
{
	RootCanvas->SetVisibility(ESlateVisibility::Hidden);
}

void UResourceInfoWidget::SetResource_Implementation(UResourceData* Resource)
{
	UE_LOG(LogChemicode, Log, TEXT("Resource info widget had no-op set resource c++ impl called. Make sure to implement it in blueprint!"));
}
