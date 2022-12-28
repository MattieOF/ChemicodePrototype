#pragma once

#include "CoreMinimal.h"
#include "Notification.generated.h"

UENUM(BlueprintType)
enum ENotificationType
{
	Success UMETA(DisplayName = "Success"),
	Info    UMETA(DisplayName = "Info"),
	Warning UMETA(DisplayName = "Warning"),
	Error   UMETA(DisplayName = "Error"),
};

USTRUCT(BlueprintType)
struct FNotification
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Notification")
	FText Title;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Notification")
	FText Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Notification",
		meta=(ToolTip=
			"If 0, the notification requires manual dismissal. If not, it'll take that amount of seconds to disappear."
		))
	float DisplayTime = 4;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Notification")
	TEnumAsByte<ENotificationType> NotificationType = Info;

	FNotification()
	{
		Title = FText::FromString("Default Notification");
		Description = FText::FromString("This shouldn't happen!");
		DisplayTime = 0;
		NotificationType = Error;
	}
	
	FNotification(FText NewTitle, FText NewDesc, float NewDisplayTime = 0, ENotificationType NewType = Info)
	{
		Title = NewTitle;
		Description = NewDesc;
		DisplayTime = NewDisplayTime;
		NotificationType = NewType;
	}
};
