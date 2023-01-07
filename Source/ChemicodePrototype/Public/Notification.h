#pragma once

#include "CoreMinimal.h"
#include "Notification.generated.h"

// Enum representing the possible types of notifications. This controls the notifications icon and colour.
UENUM(BlueprintType)
enum ENotificationType
{
	NTSuccess UMETA(DisplayName = "Success"),
	NTInfo    UMETA(DisplayName = "Info"),
	NTWarning UMETA(DisplayName = "Warning"),
	NTError   UMETA(DisplayName = "Error"),
};

USTRUCT(BlueprintType)
struct FNotification
{
	GENERATED_BODY()

	/**
	 * @brief Title of the notification. Biggest, boldest, and primary text.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Notification")
	FText Title;

	/**
	 * @brief Description of the notification. Smaller text, use to display secondary information.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Notification")
	FText Description;

	/**
	 * @brief How long the notification should be displayed for, in seconds. If 0, the notification requires manual dismissal.
	 * The timer only starts counting once it has been added to a panel.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Notification",
		meta=(ToolTip=
			"If 0, the notification requires manual dismissal. If not, it'll take that amount of seconds to disappear."
		))
	float DisplayTime = 4;

	/**
	 * @brief The type of notification. This controls icon and colour.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Notification")
	TEnumAsByte<ENotificationType> NotificationType = NTInfo;

	/**
	 * @brief Construct a default notification. Shouldn't be seen in game.
	 */
	FNotification()
	{
		Title = FText::FromString("Default Notification");
		Description = FText::FromString("This shouldn't happen!");
		DisplayTime = 0;
		NotificationType = NTError;
	}

	/**
	 * @brief Construct a new notification.
	 * @param NewTitle Title of the notification. Biggest, boldest, and primary text.
	 * @param NewDesc Description of the notification. Smaller text, use to display secondary information.
	 * @param NewDisplayTime How long the notification should be displayed for, in seconds. If 0, the notification requires manual dismissal.
	 * The timer only starts counting once it has been added to a panel.
	 * @param NewType The type of notification. This controls icon and colour.
	 */
	FNotification(FText NewTitle, FText NewDesc, float NewDisplayTime = 0, ENotificationType NewType = NTInfo)
	{
		Title = NewTitle;
		Description = NewDesc;
		DisplayTime = NewDisplayTime;
		NotificationType = NewType;
	}
};
