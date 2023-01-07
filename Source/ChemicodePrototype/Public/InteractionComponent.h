// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class UResourceData;
class AResourceItem;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractSignature);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractWithSignature, AResourceItem*, Item);

UCLASS( ClassGroup=(Chemicode), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class CHEMICODEPROTOTYPE_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnInteract"), Category = Events)
	void OnInteract();

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "OnInteractWith"), Category = Events)
	void OnInteractWith(AResourceItem* Item);

	UPROPERTY(EditAnywhere, Category = Events,
		meta=(ToolTip=
			"Resource types mapped to the name of a function to be called when it is used. Function should have 1 parameter of type AResourceItem."
		))
	TMap<UResourceData*, FName> ItemInteractions;
	
	UPROPERTY(BlueprintReadWrite)
	AResourceItem* OwnerItem;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
