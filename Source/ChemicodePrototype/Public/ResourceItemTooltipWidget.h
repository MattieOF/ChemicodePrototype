// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResourceItemTooltipWidget.generated.h"

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
	 * @brief Sets all the UI to reflect this resource
	 * @param Resource Resource data to use
	 */
	UFUNCTION(BlueprintNativeEvent)
	void SetResource(UResourceData* Resource);

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
