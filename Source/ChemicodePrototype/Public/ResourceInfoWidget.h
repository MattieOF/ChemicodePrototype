// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ResourceData.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "ResourceInfoWidget.generated.h"

// Forward decls
class UTextBlock;

/**
 * Base class for resource info widgets, allowing communication between C++ code and the blueprint implementation of the widget
 */
UCLASS(Abstract)
class CHEMICODEPROTOTYPE_API UResourceInfoWidget : public UUserWidget
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

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UCanvasPanel* RootCanvas;
};
