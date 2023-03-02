// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "NotificationPanelWidget.h"
#include "GameFramework/GameModeBase.h"
#include "ResourceData.h" // I don't like this include
#include "ChemicodeGameMode.generated.h"

class ACameraPlane;

/**
 * Basic gamemode for Chemicode. Contains object references used by other game classes and settings for the assignment
 */
UCLASS(ClassGroup=(Chemicode), Blueprintable)
class CHEMICODEPROTOTYPE_API AChemicodeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	/**
	 * @brief Initialise default object. Mainly default static classes.
	 */
	AChemicodeGameMode();

	// Utility to get CamPlanes objects from the game world
	// TODO: Maybe make it a map or something and use some FNames to get them
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ACameraPlane* GetTableCamPlane();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ACameraPlane* GetCabinetCamPlane();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ACameraPlane* GetComputerCamPlane();
	// UFUNCTION(BlueprintCallable, BlueprintPure)
	// ACameraPlane* GetBinCamPlane();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ACameraPlane* GetSubmissionCamPlane();

	/**
	 * @return Current notification panel object 
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UNotificationPanelWidget* GetNotificationPanel() { return NotificationPanel; }

	/**
	 * @brief Adds a notification to the current notification panel
	 * @param Notification Notification to be added
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AddNotification(FNotification Notification) { NotificationPanel->AddNotification(Notification); }

	/**
	 * @brief Class instantiated for the notification panel
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UNotificationPanelWidget> NotificationPanelWidgetClass;

	/**
	 * @brief If the computer is enabled currently or not
	 */
	UPROPERTY(BlueprintReadOnly)
	bool bComputerEnabled = false;
	
	/**
	 * @brief Properties to be applied to every resource instance
	 */
	UPROPERTY(EditAnywhere)
	TArray<FDefaultResourceProperty> GlobalDefaultProperties;

private:
	// Cam plane object references
	UPROPERTY()
	ACameraPlane *TableCamPlane;
	UPROPERTY()
	ACameraPlane *CabinetCamPlane;
	UPROPERTY()
	ACameraPlane *ComputerCamPlane;
	// UPROPERTY()
	// ACameraPlane *BinCamPlane;
	UPROPERTY()
	ACameraPlane *SubmissionCamPlane;
	
	/**
	 * @brief Current notification panel object
	 */
	UPROPERTY()
	UNotificationPanelWidget* NotificationPanel;

	/**
	 * @brief Sets CamPlane object references by looking for the first actor with the corresponding tag
	 */
	void FindCamPlanes();

protected:
	/**
	 * @brief Initialise some stuff, including notification panel.
	 */
	virtual void BeginPlay() override;
};
