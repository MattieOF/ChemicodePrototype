// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ResourceData.h"
#include "Blueprint/UserWidget.h"
#include "ResourceItemTooltipWidget.generated.h"

class UCanvasPanel;

/**
 * Base class for resource item tooltip widgets, allowing communication between C++ code and the blueprint implementation of the widget
 */
UCLASS(Abstract)
class CHEMICODEPROTOTYPE_API UResourceItemTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void Show(bool bInstant = false);

	UFUNCTION(BlueprintNativeEvent)
	void Hide(bool bInstant = false);
	
	UFUNCTION(BlueprintNativeEvent)
	void SetResource(UResourceData* Resource);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UCanvasPanel* GetRootCanvas() { return RootCanvas; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsShown() { return bIsVisible; }

protected:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UCanvasPanel* RootCanvas;

	UPROPERTY()
	bool bIsVisible;
};
