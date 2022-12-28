// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "CameraPlane.h"
#include "NotificationPanelWidget.h"
#include "GameFramework/GameModeBase.h"
#include "ChemicodeGameMode.generated.h"

/**
 * Basic gamemode for Chemicode. Contains object references used by other game classes and settings for the assignment
 */
UCLASS(ClassGroup=(Chemicode), Blueprintable)
class CHEMICODEPROTOTYPE_API AChemicodeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AChemicodeGameMode();

	ACameraPlane* GetTableCamPlane();
	ACameraPlane* GetCabinetCamPlane();
	ACameraPlane* GetComputerCamPlane();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UNotificationPanelWidget* GetNotificationPanel() { return NotificationPanel; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AddNotification(FNotification Notification) { NotificationPanel->AddNotification(Notification); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UNotificationPanelWidget> NotificationPanelWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	bool bComputerEnabled = false;
	
private:
	UPROPERTY()
	ACameraPlane *TableCamPlane;
	UPROPERTY()
	ACameraPlane *CabinetCamPlane;
	UPROPERTY()
	ACameraPlane *ComputerCamPlane;

	UPROPERTY()
	UNotificationPanelWidget* NotificationPanel;
	
	void FindCamPlanes();

protected:
	virtual void BeginPlay() override;
};
