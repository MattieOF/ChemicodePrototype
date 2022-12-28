// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Notification.h"
#include "Blueprint/UserWidget.h"
#include "NotificationPanelWidget.generated.h"

/**
 * Base class for notification panel widgets, allowing communication between C++ code and the blueprint implementation of the widget
 */
UCLASS()
class CHEMICODEPROTOTYPE_API UNotificationPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void AddNotification(FNotification Notification);
};
