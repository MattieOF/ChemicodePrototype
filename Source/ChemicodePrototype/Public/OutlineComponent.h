// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OutlineComponent.generated.h"


UCLASS( ClassGroup=(Chemicode), meta=(BlueprintSpawnableComponent) )
class CHEMICODEPROTOTYPE_API UOutlineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOutlineComponent();

	UFUNCTION(BlueprintCallable, Category="Outline")
	void ShowOutline();

	UFUNCTION(BlueprintCallable, Category="Outline")
	void HideOutline();

	UFUNCTION(BlueprintCallable, Category="Outline")
	void RefreshOutlinedComponents();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Outline")
	FName OutlineTag = "Outline";
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<UPrimitiveComponent*> OutlinedComponents;
	
};
