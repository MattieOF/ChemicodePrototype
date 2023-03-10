// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Interaction.h"
#include "Blueprint/UserWidget.h"
#include "ResourceItemTooltipWidget.generated.h"

class AChemicodeObject;
class AResourceItem;
class AResourceContainer;
class UResourceData;
class UCanvasPanel;

/**
 * Base class for resource item tooltip widgets, allowing communication between C++ code and the blueprint implementation of the widget
 */
UCLASS(Abstract)
class CHEMICODEPROTOTYPE_API UResourceItemTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief Show the widget.
	 * @param bInstant If true, no animation should be played.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void Show(bool bInstant = false);

	/**
	 * @brief Hide the widget.
	 * @param bInstant If true, no animation should be played.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void Hide(bool bInstant = false);

	/**
	 * @brief Sets the UI to reflect a basic chemicode object
	 * @param Object The object to reflect
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBasicObject(AChemicodeObject* Object);
	
	/**
	 * @brief Sets all the UI to reflect this resource
	 * @param Resource Resource data to use
	 */
	UFUNCTION(BlueprintNativeEvent)
	void SetResource(UResourceData* Resource, AResourceItem* Item);

	/**
	 * @brief Sets the interaction UI to reflect an interaction
	 * @param Interaction Interaction to reflect
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetInteraction(FInteraction Interaction);

	/**
	 * @brief Clears the interaction UI
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void ClearInteraction();

	/**
	 * @brief Sets the UI to reflect a container and its contents
	 * @param Container The container to reflect
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetContainer(const AResourceContainer* Container);

	/**
	 * @return The root canvas
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UCanvasPanel* GetRootCanvas() { return RootCanvas; }

	/**
	 * @return True if the widget is currently shown, false if not.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsShown() { return bIsShown; }

protected:
	/**
	 * @brief The root canvas.
	 */
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UCanvasPanel* RootCanvas;

	/**
	 * @brief Keeps track of if the widget is currently visible. Used to decide if the show/hide animations should be played or not.
	 */
	UPROPERTY(BlueprintReadWrite)
	bool bIsShown;
};
