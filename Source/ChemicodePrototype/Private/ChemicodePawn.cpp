// copyright lolol

#include "ChemicodePawn.h"

#include "CameraPlane.h"
#include "CameraPlaneCam.h"
#include "ChemicodeGameMode.h"
#include "ChemicodeStatics.h"
#include "GeneralShopItem.h"
#include "OutlineComponent.h"
#include "ResourceContainer.h"
#include "ResourceData.h"
#include "ResourceInfoWidget.h"
#include "ResourceInstance.h"
#include "ResourceItem.h"
#include "ResourceItemTooltipWidget.h"
#include "Blueprint/UserWidget.h"
#include "ChemicodePrototype/ChemicodePrototype.h"
#include "Kismet/GameplayStatics.h"

DECLARE_CYCLE_STAT(TEXT("Chemicode Pawn - Tick"), STAT_CCPawnTick, STATGROUP_Chemicode);
DECLARE_CYCLE_STAT(TEXT("Chemicode Pawn - Refresh Tooltip"), STAT_CCUpdateTooltip, STATGROUP_Chemicode);

AChemicodePawn::AChemicodePawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AChemicodePawn::BeginPlay()
{
	Super::BeginPlay();

	// Init references
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	GameMode = UChemicodeStatics::GetChemicodeGameMode(GetWorld());
	GameInstance = UChemicodeStatics::GetChemicodeGameInstance(GetWorld());

	// Initialise array of object types used by traces checking for item objects
	ItemObjectTypeArray.Add(UEngineTypes::ConvertToObjectType(COLLISION_CHANNEL_BLOCKITEM));

	// Show the cursor
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;

	// Set initial CamPlane
	CurrentCamPlane = GameMode->GetTableCamPlane();

	// Create resource info widget and hide it
	InfoWidget = CreateWidget<UResourceInfoWidget>(GetWorld(), ResourceInfoWidgetClass);
	InfoWidget->AddToViewport();
	InfoWidget->Hide(true); // True param hides the UI instantly without animating

	// Create resource tooltip widget and hide it
	TooltipWidget = CreateWidget<UResourceItemTooltipWidget>(GetWorld(), ResourceTooltipWidgetClass);
	TooltipWidget->AddToViewport();
	TooltipWidget->Hide(true); // True param hides the UI instantly without animating
}

// Called every frame
void AChemicodePawn::Tick(float DeltaTime)
{
	SCOPE_CYCLE_COUNTER(STAT_CCPawnTick);

	Super::Tick(DeltaTime);

	// Tick cooldowns
	LookCooldown -= DeltaTime;

	// Process tilting
	if (bTilting)
	{
		float XDelta, YDelta;
		PlayerController->GetInputMouseDelta(XDelta, YDelta);
		FRotator Rot = CurrentCamPlane->GetCamPositionActor()->GetActorRotation();
		Rot.Pitch += YDelta * DeltaTime * 3;
		Rot.Yaw += XDelta * DeltaTime * 3;
		// CurrentCamPlane->GetCamPositionActor()->AddActorLocalRotation(FRotator(YDelta * DeltaTime * 3, XDelta * DeltaTime * 2, 0));
		CurrentCamPlane->GetCamPositionActor()->SetActorRotation(Rot);
		int ViewportWidth, ViewportHeight;
		PlayerController->GetViewportSize(ViewportWidth, ViewportHeight);
		PlayerController->SetMouseLocation(ViewportWidth / 2, ViewportHeight / 2);
	}

	// If holding an item, move it towards the cursors position over the table
	FHitResult HitResult;
	if (bInteractionEnabled && HeldItem && !AltInteractionItem && PlayerController->
		GetHitResultUnderCursor(COLLISION_CHANNEL_TABLE, false, HitResult) && HitResult.Normal == FVector::UpVector)
	// Channel 1 is for the table
	{
		TargetItemPosition = HitResult.ImpactPoint;

		// Move the item in direction
		const auto Direction = (HitResult.ImpactPoint - HeldItem->GetActorLocation()).GetUnsafeNormal();
		const auto DistanceToMove = FVector::Distance(HeldItem->GetActorLocation(), TargetItemPosition);
		FVector Position;
		if (DistanceToMove < 20)
			Position = TargetItemPosition;
		else
			Position = HeldItem->GetActorLocation() + (Direction * ItemMoveSpeed * DeltaTime);
		FVector Center, Bounds;
		HeldItem->GetActorBounds(true, Center, Bounds);
		// DrawDebugBox(GetWorld(), Center, Bounds, FColor::Red, false, 0);
		// DrawDebugSphere(GetWorld(), Position, 20, 20, FColor::Red);
		Position.Z = TargetItemPosition.Z + UChemicodeStatics::GetZUnderOrigin(HeldItem);

		// Check if target position is valid (it is if there are no ResourceItems blocking the position)
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(HeldItem);

		TArray<AActor*> OutActors;

		UKismetSystemLibrary::BoxOverlapActors(GetWorld(), Position, Bounds, ItemObjectTypeArray,
		                                       AChemicodeObject::StaticClass(), IgnoredActors, OutActors);
		//DrawDebugBox(GetWorld(), Position, Bounds, FColor::Red);

		if (OutActors.Num() == 0 || !HeldItem->bRequireFreeSpace)
			HeldItem->SetActorLocation(Position);
		else
		{
			// If the target position is blocked, move further in the direction until there is a valid position
			// Only do this up to the distance between current position and target position so we don't overshoot
			// However, add 50 to max distance so we do overshoot a tiny bit, decreasing how far away the cursor
			// has to be from a blocking item to allow the object to move, which feels better.
			auto Distance = FVector::Dist(HeldItem->GetActorLocation(), HitResult.ImpactPoint);
			for (float i = 50.f; i < Distance + 50; i += 30.f)
			{
				Position = HeldItem->GetActorLocation() + (Direction * i);
				Position.Z = TargetItemPosition.Z + UChemicodeStatics::GetZUnderOrigin(HeldItem);
				// DrawDebugString(GetWorld(), Position + FVector(0, 0, 30), FString::SanitizeFloat(i, 2), nullptr, FColor::White, 1);
				// DrawDebugPoint(GetWorld(), Position, 5, FColor::Red, false, 1);
				OutActors.Empty();
				UKismetSystemLibrary::BoxOverlapActors(GetWorld(), Position, Bounds, ItemObjectTypeArray,
				                                       AChemicodeObject::StaticClass(), IgnoredActors, OutActors);
				if (OutActors.Num() == 0)
				{
					HeldItem->SetActorLocation(Position);
					break;
				}
			}
		}
	}
	else if (AltInteractionItem && PlayerController->GetHitResultUnderCursor(COLLISION_CHANNEL_TABLE, false, HitResult))
		AltInteractionItem->UpdateAltInteractionPosition(HitResult.ImpactPoint);

	// Check for hovered resource items
	if (bInteractionEnabled)
	{
		FHitResult Result;

		TArray<AActor*> IgnoredActors;
		if (HeldItem)
			IgnoredActors.Add(HeldItem);

		if (UChemicodeStatics::GetHitResultAtCursor(PlayerController, ItemObjectTypeArray, false, Result,
		                                            IgnoredActors))
		{
			auto Item = Cast<AChemicodeObject>(Result.GetActor());
			if (!Item)
				return;
			if (CurrentCamPlane != GameMode->GetTableCamPlane() && !Item->bNonTable)
			{
				HighlightItem(nullptr);
				return;
			}
			if (AltInteractionItem && AltInteractionItem == Item)
			{
				HighlightItem(nullptr);
				return;
			}	
			if (Item && HighlightedItem != Item)
				HighlightItem(Item);
			else if (Item == nullptr)
				HighlightItem(nullptr);
		}
		else
			HighlightItem(nullptr);
	}
}

void AChemicodePawn::LookUp()
{
	if (!bInteractionEnabled)
		return;

	if (CurrentCamPlane == GameMode->GetTableCamPlane() && LookCooldown <= 0)
	{
		SetCamPlane(GameMode->GetCabinetCamPlane());
		LookCooldown = .75f;
	}
}

void AChemicodePawn::LookDown()
{
	if (!bInteractionEnabled)
		return;

	if (CurrentCamPlane == GameMode->GetCabinetCamPlane() && LookCooldown <= 0)
	{
		SetCamPlane(GameMode->GetTableCamPlane());
		LookCooldown = .75f;
	} 
	else if (CurrentCamPlane == GameMode->GetTableCamPlane() && LookCooldown <= 0)
	{
		SetCamPlane(GameMode->GetSubmissionCamPlane());
		LookCooldown = .75f;
	}
	else if (CurrentCamPlane == GameMode->GetSubmissionCamPlane() && LookCooldown <= 0)
	{
		SetCamPlane(GameMode->GetTableCamPlane());
		LookCooldown = .75f;
	}
}

void AChemicodePawn::LookLeft()
{
	if (!bInteractionEnabled)
		return;

	if ((CurrentCamPlane == GameMode->GetCabinetCamPlane() || CurrentCamPlane == GameMode->GetTableCamPlane()) &&
		LookCooldown <= 0)
	{
		SetCamPlane(GameMode->GetComputerCamPlane());
		LookCooldown = .75f;
	}
	// else if (CurrentCamPlane == GameMode->GetBinCamPlane() && LookCooldown <= 0)
	// {
	// 	if (PrevCamPlane != GameMode->GetTableCamPlane() && PrevCamPlane != GameMode->GetCabinetCamPlane())
	// 		SetCamPlane(GameMode->GetTableCamPlane());
	// 	else
	// 		SetCamPlane(PrevCamPlane);
	// 	LookCooldown = .75f;
	// }
	// else if (CurrentCamPlane == GameMode->GetSubmissionCamPlane() && LookCooldown <= 0)
	// {
	// 	SetCamPlane(GameMode->GetBinCamPlane());
	// 	LookCooldown = .75f;
	// }
}

void AChemicodePawn::LookRight()
{
	if (!bInteractionEnabled)
		return;

	if (CurrentCamPlane == GameMode->GetComputerCamPlane() && LookCooldown <= 0)
	{
		ACameraPlane* Target;
		if (PrevCamPlane != GameMode->GetTableCamPlane() && PrevCamPlane != GameMode->GetCabinetCamPlane())
			Target = GameMode->GetTableCamPlane();
		else
			Target = PrevCamPlane;
		SetCamPlane(Target);
		LookCooldown = .75f;
	}
	// else if ((CurrentCamPlane == GameMode->GetTableCamPlane() || CurrentCamPlane == GameMode->GetCabinetCamPlane()) &&
	// 	LookCooldown <= 0)
	// {
	// 	SetCamPlane(GameMode->GetBinCamPlane());
	// 	LookCooldown = .75f;
	// }
	// else if (CurrentCamPlane == GameMode->GetBinCamPlane() &&
	// 	LookCooldown <= 0)
	// {
	// 	SetCamPlane(GameMode->GetSubmissionCamPlane());
	// 	LookCooldown = .75f;
	// }
}

// Called to bind functionality to input
void AChemicodePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LookUp", EInputEvent::IE_DoubleClick, this, &AChemicodePawn::LookUp);
	PlayerInputComponent->BindAction("LookDown", EInputEvent::IE_DoubleClick, this, &AChemicodePawn::LookDown);
	PlayerInputComponent->BindAction("LookLeft", EInputEvent::IE_DoubleClick, this, &AChemicodePawn::LookLeft);
	PlayerInputComponent->BindAction("LookRight", EInputEvent::IE_DoubleClick, this, &AChemicodePawn::LookRight);
	PlayerInputComponent->BindAction("Use", EInputEvent::IE_Pressed, this, &AChemicodePawn::OnUse);
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &AChemicodePawn::OnInteract);
	PlayerInputComponent->BindAction("AltInteract", EInputEvent::IE_Pressed, this, &AChemicodePawn::OnAltInteractDown);
	PlayerInputComponent->BindAction("AltInteract", EInputEvent::IE_Released, this, &AChemicodePawn::OnAltInteractUp);
	PlayerInputComponent->BindAction("TiltCamera", EInputEvent::IE_Pressed, this, &AChemicodePawn::OnTiltCamDown);
	PlayerInputComponent->BindAction("TiltCamera", EInputEvent::IE_Released, this, &AChemicodePawn::OnTiltCamUp);
	PlayerInputComponent->BindAxis("Horizontal", this, &AChemicodePawn::MoveHorizontal);
	PlayerInputComponent->BindAxis("Vertical", this, &AChemicodePawn::MoveVertical);
	PlayerInputComponent->BindAxis("Scroll", this, &AChemicodePawn::OnScroll);
}

void AChemicodePawn::SetCamPlane(ACameraPlane* NewCamPlane, float BlendTime)
{
	if (NewCamPlane == GameMode->GetComputerCamPlane() && !GameInstance->bIsScriptingAssignment)
	{
		GameMode->AddNotification(FNotification(FText::FromString("Computer disabled"),
												FText::FromString("The computer isn't needed on practical assignments"), 3,
												NTError));
		return;
	}

	if (NewCamPlane == GameMode->GetSubmissionCamPlane() && !GameInstance->bIsPracticalAssignment)
	{
		GameMode->AddNotification(FNotification(FText::FromString("Pedestal disabled"),
												FText::FromString("The pedestal isn't needed on scripting assignments"), 3,
												NTError));
		return;
	}
	
	PrevCamPlane = CurrentCamPlane;
	CurrentCamPlane = NewCamPlane;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(NewCamPlane->GetCamPositionActor(),
		BlendTime, VTBlend_EaseInOut, 2);
	ResourceLostHover();
	HighlightItem(nullptr);
	OnCamPlaneChanged.Broadcast(NewCamPlane);

	if (HeldItem && !NewCamPlane->bCanHoldResources)
	{
		HeldItem->OnItemPlaced.Broadcast();
		DropItem();
	}

	if (AltInteractionItem)
	{
		AltInteractionItem->GetOutline()->HideOutline();
		AltInteractionItem->AltInteractionEnd();
		AltInteractionItem = nullptr;
	}
}

bool AChemicodePawn::ResourceHovered(UResourceData* Resource)
{
	// Only show resource UI if in cabinet
	if (CurrentCamPlane != GameMode->GetCabinetCamPlane())
		return false;

	// Set resource values in UI
	InfoWidget->SetResource(Resource);

	if (bResourceInfoVisible) // Don't play show anim again if we're already visible.
		return true;
	InfoWidget->Show();
	bResourceInfoVisible = true;
	return true;
}

void AChemicodePawn::TryBuyResource(UResourceData* Resource)
{
	if (!Resource)
	{
		UE_LOG(LogChemicode, Error, TEXT("Null resource in AChemicodePawn::TryBuyResource!"));
		return;
	}

	LookDown(); // Look back at the table
	const auto Item = Cast<AResourceItem>(
		GetWorld()->SpawnActor(Resource->ResourceItemClass, &TargetItemPosition, &FRotator::ZeroRotator));
	FVector Center, Bounds;
	Item->SetResourceAndInteraction(Resource, Resource->DefaultInteraction
		                                          ? Resource->DefaultInteraction
		                                          : UInteractionComponent::StaticClass(), false);
	Item->GetActorBounds(true, Center, Bounds);
	Item->AddActorLocalOffset(FVector(0, 0, Bounds.Z + 1));
	HoldItem(Item);
}

void AChemicodePawn::HoldNewItem(TSubclassOf<AChemicodeObject> ObjectClass)
{
	LookDown();
	const auto Item = Cast<AChemicodeObject>(
		GetWorld()->SpawnActor(ObjectClass, &TargetItemPosition, &FRotator::ZeroRotator));
	FVector Center, Bounds;
	Item->GetActorBounds(true, Center, Bounds);
	Item->AddActorLocalOffset(FVector(0, 0, Bounds.Z + 1));
	HoldItem(Item);
}

void AChemicodePawn::ResourceLostHover()
{
	if (!bResourceInfoVisible)
		return;
	InfoWidget->Hide();
	bResourceInfoVisible = false;
}

void AChemicodePawn::HighlightItem(AChemicodeObject* Item)
{
	// Hide outline on previous item
	if (HighlightedItem)
		HighlightedItem->GetOutline()->HideOutline();

	// Set new highlighted item and highlight it
	HighlightedItem = Item;
	if (HighlightedItem)
		HighlightedItem->GetOutline()->ShowOutline();

	RefreshTooltip();
}

/**
 * @brief Sets an item as the currently held item. This also removes highlight from the previous item, and adds it to the new one. 
 * @param Item Item to hold
 */
void AChemicodePawn::HoldItem(AChemicodeObject* Item)
{
	if (!Item)
		return;

	if (HeldItem)
	{
		HeldItem->OnItemPlaced.Broadcast();
		DropItem();
	}

	HighlightItem(nullptr); // De-highlight now so it doesn't happen later and remain un-highlighted
	HeldItem = Item;
	Item->OnItemPickedUp.Broadcast();
	HeldItem->GetOutline()->ShowOutline();
}

/**
 * @brief Drops the currently held item. This also removes highlight from it.
 */
void AChemicodePawn::DropItem()
{
	if (!HeldItem)
		return;
	HeldItem->GetOutline()->HideOutline();
	HeldItem = nullptr;
}

void AChemicodePawn::DisableInteraction()
{
	bInteractionEnabled = false;
	UChemicodeStatics::SetOutlinesEnabled(GetWorld(), false);
}

void AChemicodePawn::EnableInteraction()
{
	bInteractionEnabled = true;
	UChemicodeStatics::SetOutlinesEnabled(GetWorld(), true);
}

void AChemicodePawn::RefreshTooltip()
{
	SCOPE_CYCLE_COUNTER(STAT_CCUpdateTooltip);

	// Prevent deref of nullptr if tooltip widget is null for some reason
	if (!TooltipWidget)
		return;

	// Hide tooltip if we're not hovering anything
	if (!HighlightedItem)
	{
		if (TooltipWidget->IsShown())
			TooltipWidget->Hide();
		return;
	}

	// Show the tooltip if it's hidden
	if (!TooltipWidget->IsShown())
		TooltipWidget->Show();

	// Cast to highlighted item to check for and do item specific things
	AResourceItem* HighlightedAsRI = Cast<AResourceItem>(HighlightedItem);
	const AResourceItem* HeldAsRI = Cast<AResourceItem>(HeldItem);
	const AResourceContainer* HighlightedAsRC = Cast<AResourceContainer>(HighlightedItem);

	// Set resource to account for changes
	if (HighlightedAsRI)
		TooltipWidget->SetResource(HighlightedAsRI->Resource->Data, HighlightedAsRI);
	else if (HighlightedAsRC)
		TooltipWidget->SetContainer(HighlightedAsRC);
	else if (HighlightedItem)
		TooltipWidget->SetBasicObject(HighlightedItem);

	// Check for changes in interactions
	UInteractionComponent* InteractionComponent = Cast<UInteractionComponent>(
		HighlightedItem->GetComponentByClass(UInteractionComponent::StaticClass()));
	if (HeldItem && InteractionComponent)
	{
		const FInteraction Interaction = InteractionComponent->GetInteractionWith(HeldItem);
		if (Interaction.bIsValid)
			TooltipWidget->SetInteraction(Interaction);
		else
			TooltipWidget->ClearInteraction();
	}
	else
		TooltipWidget->ClearInteraction();
}

void AChemicodePawn::MoveHorizontal(float Value)
{
	CurrentCamPlane->MoveOnPlane(Value * Speed, 0);
}

void AChemicodePawn::MoveVertical(float Value)
{
	CurrentCamPlane->MoveOnPlane(0, Value * Speed);
}

void AChemicodePawn::OnScroll(float Value)
{
	CurrentCamPlane->GetCamPositionActor()->SetFOV(
		FMath::Clamp(CurrentCamPlane->GetCamPositionActor()->GetFOV() + (Value * 5), FOVMin, FOVMax));
}

void AChemicodePawn::OnUse()
{
	if (!bInteractionEnabled)
		return;

	if (!HeldItem)
	{
		// Play invalid sound
		return;
	}

	HeldItem->Use();
	// TODO: if returns false, play invalid use sound
}

void AChemicodePawn::OnInteract()
{
	if (!bInteractionEnabled || AltInteractionItem)
		return;

	AResourceItem* HeldAsRI = Cast<AResourceItem>(HeldItem);
	AResourceContainer* HighlightedAsRC = Cast<AResourceContainer>(HighlightedItem);

	if (HeldAsRI && HighlightedAsRC && HeldAsRI->GetInteractionComponent()->CanDepositInto(HighlightedAsRC))
	{
		HighlightedAsRC->TransferFromItem(HeldAsRI, 50000); // Deposit 50g/50ml
	}
	else if (HeldItem != nullptr && HighlightedItem != nullptr)
	{
		HighlightedItem->InteractWith(HeldItem);
		HighlightedItem->OnInteractWith(HeldItem);
	}
	else if (HighlightedItem != nullptr)
	{
		FHitResult Result;
		if (PlayerController->GetHitResultUnderCursorForObjects(ItemObjectTypeArray, false, Result))
		{
			auto Item = Cast<AChemicodeObject>(Result.GetActor());
			if (!Item)
				return;
			if (Item->bHoldable)
				HoldItem(Item);
			else
			{
				Item->Interact();
				Item->OnInteract();
			}
		}
	}
	else if (HeldItem != nullptr)
	{
		HeldItem->OnItemPlaced.Broadcast();
		DropItem();
	}
}

void AChemicodePawn::OnAltInteractDown()
{
	if (HighlightedItem && HighlightedItem->bDragInteraction)
	{
		AltInteractionItem = HighlightedItem;
		HoldItem(nullptr);
		HighlightItem(nullptr);
		AltInteractionItem->AltInteractionBegin();
		AltInteractionItem->GetOutline()->ShowOutline();
	}
}

void AChemicodePawn::OnAltInteractUp()
{
	if (AltInteractionItem)
	{
		if (HighlightedItem)
			HighlightedItem->AltInteractWith(AltInteractionItem);
		else
			AltInteractionItem->AltInteract();

		AltInteractionItem->AltInteractionEnd();
		AltInteractionItem->GetOutline()->HideOutline();
		AltInteractionItem = nullptr;
	}
}

void AChemicodePawn::OnTiltCamDown()
{
	// Hide the cursor
	PlayerController->bShowMouseCursor = false;
	PlayerController->bEnableClickEvents = false;
	PlayerController->bEnableMouseOverEvents = false;
	bTilting = true;
}

void AChemicodePawn::OnTiltCamUp()
{
	// Show the cursor
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;
	bTilting = false;
}
