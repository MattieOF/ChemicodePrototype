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
	/**
	 * @brief Adds a notification to the panel. Once this is done, the timer starts.
	 * @param Notification Notification to be added
	 */
	UFUNCTION(BlueprintNativeEvent)
	void AddNotification(FNotification Notification);
};
