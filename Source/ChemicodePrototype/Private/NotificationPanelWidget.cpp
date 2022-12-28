// copyright lolol

#include "NotificationPanelWidget.h"

#include "ChemicodePrototype/ChemicodePrototype.h"

void UNotificationPanelWidget::AddNotification_Implementation(FNotification Notification)
{
	UE_LOG(LogChemicode, Warning, TEXT("Default notification panel impl fired. This shouldn't happen"));
}
