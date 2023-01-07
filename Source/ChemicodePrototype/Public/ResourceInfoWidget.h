// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResourceInfoWidget.generated.h"

// Forward declarations
class UCanvasPanel;
class UResourceData;
class UTextBlock;

/**
 * Base class for resource info widgets, allowing communication between C++ code and the blueprint implementation of the widget
 */
UCLASS(Abstract)
class CHEMICODEPROTOTYPE_API UResourceInfoWidget : public UUserWidget
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
	 * @brief The root canvas
	 */
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UCanvasPanel* RootCanvas;
};
