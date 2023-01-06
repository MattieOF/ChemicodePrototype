// copyright lolol


#include "ChemicodePawn.h"

#include "ChemicodeGameMode.h"
#include "ChemicodeStatics.h"
#include "ResourceInfoWidget.h"
#include "Blueprint/UserWidget.h"
#include "ChemicodePrototype/ChemicodePrototype.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AChemicodePawn::AChemicodePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChemicodePawn::BeginPlay()
{
	Super::BeginPlay();

	// Init references
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	GameMode = Cast<AChemicodeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	// Initialise array of object types used by traces checking for item objects
	ItemObjectTypeArray.Add(UEngineTypes::ConvertToObjectType(COLLISION_CHANNEL_BLOCKITEM));
	
	// Show the cursor
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;

	// Set view target to initial CamPlane
	PlayerController->SetViewTarget(GameMode->GetTableCamPlane()->GetCamPositionActorChecked());
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
	Super::Tick(DeltaTime);

	// Tick cooldowns
	LookCooldown -= DeltaTime;

	// If holding an item, move it towards the cursors position over the table
	FHitResult HitResult;
	if (HeldItem && PlayerController->GetHitResultUnderCursor(COLLISION_CHANNEL_TABLE, false, HitResult)) // Channel 1 is for the table
	{
		TargetItemPosition = HitResult.ImpactPoint;
		
		// Move the item in direction
		const auto Direction = (HitResult.ImpactPoint - HeldItem->GetActorLocation()).GetUnsafeNormal();
		auto Position = HeldItem->GetActorLocation() + (Direction * ItemMoveSpeed * DeltaTime);
		FVector Center, Bounds;
		HeldItem->GetActorBounds(true, Center, Bounds);
		// DrawDebugBox(GetWorld(), Center, Bounds, FColor::Red, false, 0);
		Position.Z = TargetItemPosition.Z + Bounds.Z;

		// Check if target position is valid (it is if there are no ResourceItems blocking the position)
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(HeldItem);

		TArray<AActor*> OutActors;
		
		UKismetSystemLibrary::BoxOverlapActors(GetWorld(), Position, Bounds, ItemObjectTypeArray,
		                                       AResourceItem::StaticClass(), IgnoredActors, OutActors);
		//DrawDebugBox(GetWorld(), Position, Bounds, FColor::Red);
		
		if (OutActors.Num() == 0)
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
				Position.Z = TargetItemPosition.Z + Bounds.Z;
				// DrawDebugString(GetWorld(), Position + FVector(0, 0, 30), FString::SanitizeFloat(i, 2), nullptr, FColor::White, 1);
				// DrawDebugPoint(GetWorld(), Position, 5, FColor::Red, false, 1);
				OutActors.Empty();
				UKismetSystemLibrary::BoxOverlapActors(GetWorld(), Position, Bounds, ItemObjectTypeArray,
													   AResourceItem::StaticClass(), IgnoredActors, OutActors);
				if (OutActors.Num() == 0)
				{
					HeldItem->SetActorLocation(Position);
					break;
				}
			}
		}
	}

	// Check for hovered resource items
	if (CurrentCamPlane == GameMode->GetTableCamPlane())
	{
		FHitResult Result;
		
		TArray<AActor*> IgnoredActors;
		if (HeldItem)
			IgnoredActors.Add(HeldItem);

		if (UChemicodeStatics::GetHitResultAtCursor(PlayerController, ItemObjectTypeArray, false, Result, IgnoredActors))
		{
			auto Item = Cast<AResourceItem>(Result.GetActor());
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
	if (CurrentCamPlane == GameMode->GetTableCamPlane() && LookCooldown <= 0)
	{
		SetCamPlane(GameMode->GetCabinetCamPlane());
		LookCooldown = .75f;
	}
}

void AChemicodePawn::LookDown()
{
	if (CurrentCamPlane == GameMode->GetCabinetCamPlane() && LookCooldown <= 0)
	{
		SetCamPlane(GameMode->GetTableCamPlane());
		LookCooldown = .75f;
	}
}

void AChemicodePawn::LookLeft()
{
	if ((CurrentCamPlane == GameMode->GetCabinetCamPlane() || CurrentCamPlane == GameMode->GetTableCamPlane()) && LookCooldown <= 0)
	{
		if (!GameMode->bComputerEnabled)
		{
			GameMode->AddNotification(FNotification(FText::FromString("Computer disabled"),
			                                        FText::FromString("This is not an automation assignment!"), 3,
			                                        Error));
			return;
		}
		
		SetCamPlane(GameMode->GetComputerCamPlane());
		LookCooldown = .75f;	
	}
}

void AChemicodePawn::LookRight()
{
	if (CurrentCamPlane == GameMode->GetComputerCamPlane() && LookCooldown <= 0)
	{
		ACameraPlane* Target;
		if (!PrevCamPlane || PrevCamPlane == GameMode->GetComputerCamPlane())
			Target = GameMode->GetTableCamPlane();
		else
			Target = PrevCamPlane;
		SetCamPlane(Target);
		LookCooldown = .75f;	
	}
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
	PlayerInputComponent->BindAxis("Horizontal", this, &AChemicodePawn::MoveHorizontal);
	PlayerInputComponent->BindAxis("Vertical", this, &AChemicodePawn::MoveVertical);
	PlayerInputComponent->BindAxis("Scroll", this, &AChemicodePawn::OnScroll);
}

void AChemicodePawn::SetCamPlane(ACameraPlane* NewCamPlane, float BlendTime)
{
	PrevCamPlane = CurrentCamPlane;
	CurrentCamPlane = NewCamPlane;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(NewCamPlane->GetCamPositionActor(),
		BlendTime, VTBlend_EaseInOut, 2);
	ResourceLostHover();
	HighlightItem(nullptr);
	DropItem();
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
	LookDown(); // Look back at the table
	const auto Item = Cast<AResourceItem>(GetWorld()->SpawnActor(Resource->ResourceItemClass, &TargetItemPosition, &FRotator::ZeroRotator));
	FVector Center, Bounds;
	Item->GetActorBounds(true, Center, Bounds);
	Item->AddActorLocalOffset(FVector(0, 0, Bounds.Z + 1));
	Item->SetResource(Resource);
	HoldItem(Item);
}

void AChemicodePawn::ResourceLostHover()
{
	if (!bResourceInfoVisible)
		return;
	InfoWidget->Hide();
	bResourceInfoVisible = false;
}

void AChemicodePawn::HighlightItem(AResourceItem* Item)
{
	// Hide outline on previous item
	if (HighlightedItem)
		HighlightedItem->GetOutline()->HideOutline();

	// Do tooltip
	if (Item)
	{
		if (!TooltipWidget->IsShown())
			TooltipWidget->Show();
		TooltipWidget->SetResource(Item->Resource);
	} else
	{
		if (TooltipWidget->IsShown())
			TooltipWidget->Hide();
	}

	// Set new highlighted item and highlight it
	HighlightedItem = Item;
	if (HighlightedItem)
		HighlightedItem->GetOutline()->ShowOutline();
}

/**
 * @brief Sets an item as the currently held item. This also removes highlight from the previous item, and adds it to the new one. 
 * @param Item Item to hold
 */
void AChemicodePawn::HoldItem(AResourceItem* Item)
{
	if (!Item)
		return;
	
	if (HeldItem)
		HeldItem->GetOutline()->HideOutline();
	HighlightItem(nullptr); // De-highlight now so it doesn't happen later and remain un-highlighted
	HeldItem = Item;
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
	CurrentCamPlane->GetCamPositionActor()->SetFOV(FMath::Clamp(CurrentCamPlane->GetCamPositionActor()->GetFOV() + (Value * 5), FOVMin, FOVMax));
}

void AChemicodePawn::OnUse()
{
	if (HeldItem)
		HeldItem->Interact();
	// else play invalid use sound
}

void AChemicodePawn::OnInteract()
{
	if (HeldItem != nullptr && HighlightedItem != nullptr)
	{
		HighlightedItem->InteractWith(HeldItem);
	}
	else if (HeldItem != nullptr)
	{
		DropItem();
	}
	else if (CurrentCamPlane == GameMode->GetTableCamPlane())
	{
		FHitResult Result;
		if (PlayerController->GetHitResultUnderCursorForObjects(ItemObjectTypeArray, false, Result))
			HoldItem(Cast<AResourceItem>(Result.GetActor()));
	}
}
