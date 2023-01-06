// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

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

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnInteractWith"), Category = Events)
	void OnInteractWith(AResourceItem* Item);
	
	UPROPERTY(BlueprintReadWrite)
	AResourceItem* OwnerItem;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
