// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OutlineComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHEMICODEPROTOTYPE_API UOutlineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOutlineComponent();

	UFUNCTION(BlueprintCallable)
	void ShowOutline();

	UFUNCTION(BlueprintCallable)
	void HideOutline();

	UFUNCTION(BlueprintCallable)
	void RefreshOutlinedComponents();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Outline")
	FName OutlineTag = "Outline";
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<UPrimitiveComponent*> OutlinedComponents;
	
};
